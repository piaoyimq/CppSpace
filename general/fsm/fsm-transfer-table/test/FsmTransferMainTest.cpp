/*
 * FsmTransferMainTest.cpp
 *
 *  Created on: 2016年6月11日
 *      Author: piaoyimq
 */
#include "../include/fsm-transfer.h"


class Sm
{
public:
	enum State
	{
		S_OPENED,
		S_CLOSED,
		S_LOCKED
	};

	enum Event
	{
		E_OPEN,
		E_CLOSE,
		E_LOCK,
		E_UNLOCK
	};

	fsm::CurrentAtr<Sm::State> currentAtr ;
		Sm()
		{
			currentAtr.state = S_CLOSED;
			currentAtr.transferTimes = 0;
		}

		~Sm(){}

 void toOpen(fsm::CurrentAtr<State>& door)
{
	door.state = S_OPENED;
	std::cout << "open the door!\n";
}

void toClose(fsm::CurrentAtr<State>& door)
{
	door.state = S_CLOSED;
	std::cout << "close the door!\n";
}

void toLock(fsm::CurrentAtr<State>& door)
{
	door.state = S_LOCKED;
	std::cout << "lock the door!\n";
}

void printDoor(const fsm::CurrentAtr<State>& door)
{
	std::cout << "\n<<=============================\n";
	std::string stateNote[] = { "opened", "closed", "locked" }; // 下标正好对应状态枚举值
	std::cout << "the door's state is: " << stateNote[door.state] << std::endl;
	std::cout << "the door transfer times is: " << door.transferTimes << std::endl;

	std::cout << "==============================>>\n";
}
};


typedef void (Sm::*pfToState)(fsm::CurrentAtr<Sm::State>&);
pfToState eventCallerTable[] =
{
		&Sm::toOpen,
		&Sm::toClose,
		&Sm::toLock
}; //状态枚举值(State)对应下标


fsm::StateTransfer<Sm::State, Sm::Event> stateTransferTable[]=
{
	  { Sm::S_OPENED, Sm::E_CLOSE, Sm::S_CLOSED },
	  { Sm::S_CLOSED, Sm::E_OPEN,  Sm::S_OPENED },
	  { Sm::S_CLOSED, Sm::E_LOCK,  Sm::S_LOCKED },
	  { Sm::S_LOCKED, Sm::E_UNLOCK,Sm::S_CLOSED },
};

int main()
{
	Sm doorSm;

	doorSm.printDoor(doorSm.currentAtr);

	TRANSFER(doorSm, Sm::E_OPEN);
	doorSm.printDoor(doorSm.currentAtr);

	TRANSFER(doorSm, Sm::E_LOCK);
	doorSm.printDoor(doorSm.currentAtr);

	TRANSFER(doorSm, Sm::E_CLOSE);
	doorSm.printDoor(doorSm.currentAtr);

	return 0;
}



