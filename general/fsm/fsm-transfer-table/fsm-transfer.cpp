/*
 * fsm-transfer.cpp
 *
 *  Created on: 2016年6月9日
 *      Author: piaoyimq
 */

#include <iostream>

#define TRANS(arg1, arg2, fp1, fp2)\
for (int i = 0; i < sizeof(fp1) / sizeof(fsm::StateTransfer<State, Event>); ++i)\
{\
	if (arg1.state == fp1[i].currentState && arg2 == fp1[i].event)\
	{\
		fp2[fp1[i].NextState](arg1);\
		arg1.transferTimes++;\
		std::cout << "transfer ok!\n";\
		break;\
	}\
}

#define TRANSFER(arg1, arg2) 		TRANS(arg1, arg2, g_stateTransferTable, g_pFun)

namespace fsm
{

template<typename S>
struct Door
{
	S state;
	int transferTimes;
};

template<typename S, typename E>
struct StateTransfer
{
	S currentState;
	E event;
	S NextState;
};
#if 0// this function had to implment in user plane, so replace by TRANSFER MACRO, TRANSFER implmented in library.
void transfer(fsm::Door<State>& door, const Event event)
{
	for (int i = 0; i < sizeof(g_stateTransferTable) / sizeof(fsm::StateTransfer<State, Event>); ++i)
	{
		if (door.state == g_stateTransferTable[i].currentState && event == g_stateTransferTable[i].event)
		{
			g_pFun[g_stateTransferTable[i].NextState](door);
			door.transferTimes++;
			std::cout << "transfer ok!\n";
			return;
		}
	}
	std::cout << "This event cannot transfer current state!!\n";
	return;
}
#endif
}

enum State
{
	OPENED,
	CLOSED,
	LOCKED
};

enum Event
{
	OPEN,
	CLOSE,
	LOCK,
	UNLOCK
};

void toOpen(fsm::Door<State>& door);
void toClose(fsm::Door<State>& door);
void toLock(fsm::Door<State>& door);

typedef void (*pfToState)(fsm::Door<State>& door);
pfToState g_pFun[] = { toOpen, toClose, toLock }; //状态枚举值(State)对应下标


fsm::StateTransfer<State, Event> g_stateTransferTable[]=
													  {
															  { OPENED, CLOSE, CLOSED },
															  { CLOSED, OPEN, OPENED },
															  { CLOSED, LOCK, LOCKED },
															  { LOCKED, UNLOCK, CLOSED },
													  };





//class DoorSm
//{

void toOpen(fsm::Door<State>& door)
{
	door.state = OPENED;
	std::cout << "open the door!\n";
}

void toClose(fsm::Door<State>& door)
{
	door.state = CLOSED;
	std::cout << "close the door!\n";
}

void toLock(fsm::Door<State>& door)
{
	door.state = LOCKED;
	std::cout << "lock the door!\n";
}

void printDoor(const fsm::Door<State>& door)
{
	std::string stateNote[] = { "opened", "closed", "locked" }; // 下标正好对应状态枚举值
	std::cout << "the door's state is: " << stateNote[door.state] << std::endl;
	std::cout << "the door transfer times is: " << door.transferTimes << std::endl;
}

int main()
{
	fsm::Door<State> door = { CLOSED, 0 };
	printDoor(door);
	TRANSFER(door, OPEN);
	printDoor(door);
	TRANSFER(door, LOCK);
	printDoor(door);
	TRANSFER(door, CLOSE);
	printDoor(door);
	return 0;
}
