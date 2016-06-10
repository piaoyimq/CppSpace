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

	typedef void (Sm::*pfToState)(fsm::CurrentAtr<State>&);
	pfToState eventCallerTable[3];
	fsm::StateTransfer<State, Event> stateTransferTable[4];
	fsm::CurrentAtr<Sm::State> currentAtr ;

	Sm()
	{
		currentAtr.state = S_CLOSED;
		currentAtr.transferTimes = 0;

		//eventCallerTable  must initial in order by the State.
		eventCallerTable[0] = &Sm::toOpen;
		eventCallerTable[1] = &Sm::toClose;
		eventCallerTable[2] = &Sm::toLock;

		//no order
		stateTransferTable[0] =	{S_OPENED, E_CLOSE, S_CLOSED };
		stateTransferTable[1] = {S_CLOSED, E_OPEN,  S_OPENED };
		stateTransferTable[2] = {S_CLOSED, E_LOCK,  S_LOCKED };
		stateTransferTable[3] = {S_LOCKED, E_UNLOCK,S_CLOSED };
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



