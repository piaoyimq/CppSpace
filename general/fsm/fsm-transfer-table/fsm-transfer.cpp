/*
 * fsm-transfer.cpp
 *
 *  Created on: 2016年6月9日
 *      Author: piaoyimq
 */

#include <iostream>

#define TRANS(arg1, arg2, arg3, arg4, fp1, fp2) \
for (int i = 0; i < sizeof(fp1) / sizeof(fsm::StateTransfer<arg3::State, arg3::Event>); ++i)\
{\
	if (arg1.arg4.state == fp1[i].currentState && arg2 == fp1[i].event)\
	{\
		(arg1.*fp2[fp1[i].NextState])(arg1.arg4);\
		arg1.arg4.transferTimes++;\
		std::cout << "transfer ok!\n";\
		break;\
	}\
	if((sizeof(fp1) / sizeof(fsm::StateTransfer<arg3::State, arg3::Event>)) == i+1)\
	{\
		std::cout << "This event cannot transfer current state!!\n";\
	}\
}


//#define TRANS(arg1, arg2, arg3, fp1, fp2) \
//for (int i = 0; i < sizeof(fp1) / sizeof(fsm::StateTransfer<arg1.arg3.State, arg1.arg3.Event>); ++i)\
//{\
//	if (arg1.arg3.state == fp1[i].currentState && arg2 == fp1[i].event)\
//	{\
//		(arg1.*fp2[fp1[i].NextState])(arg1.arg3);\
//		arg1.arg3.transferTimes++;\
//		std::cout << "transfer ok!\n";\
//		break;\
//	}\
//}


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





#define TRANSFER(arg1, arg2) 		TRANS(arg1, arg2, DoorSm, door, g_stateTransferTable, g_pFun)
class DoorSm
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

	fsm::Door<DoorSm::State> door ;
		DoorSm()
		{
			door.state = S_CLOSED;
			door.transferTimes = 0;
		}

		~DoorSm(){}

 void toOpen(fsm::Door<State>& door)
{
	door.state = S_OPENED;
	std::cout << "open the door!\n";
}

void toClose(fsm::Door<State>& door)
{
	door.state = S_CLOSED;
	std::cout << "close the door!\n";
}

void toLock(fsm::Door<State>& door)
{
	door.state = S_LOCKED;
	std::cout << "lock the door!\n";
}

void printDoor(const fsm::Door<State>& door)
{
	std::cout << "\n<<=============================\n";
	std::string stateNote[] = { "opened", "closed", "locked" }; // 下标正好对应状态枚举值
	std::cout << "the door's state is: " << stateNote[door.state] << std::endl;
	std::cout << "the door transfer times is: " << door.transferTimes << std::endl;

	std::cout << "==============================>>\n";
}
};


typedef void (DoorSm::*pfToState)(fsm::Door<DoorSm::State>& door);
pfToState g_pFun[] =
{
		&DoorSm::toOpen,
		&DoorSm::toClose,
		&DoorSm::toLock
}; //状态枚举值(State)对应下标


fsm::StateTransfer<DoorSm::State, DoorSm::Event> g_stateTransferTable[]=
{
	  { DoorSm::S_OPENED, DoorSm::E_CLOSE, DoorSm::S_CLOSED },
	  { DoorSm::S_CLOSED, DoorSm::E_OPEN,  DoorSm::S_OPENED },
	  { DoorSm::S_CLOSED, DoorSm::E_LOCK,  DoorSm::S_LOCKED },
	  { DoorSm::S_LOCKED, DoorSm::E_UNLOCK,DoorSm::S_CLOSED },
};

int main()
{
	DoorSm doorSm;

	doorSm.printDoor(doorSm.door);

	TRANSFER(doorSm, DoorSm::E_OPEN);
	doorSm.printDoor(doorSm.door);

	TRANSFER(doorSm, DoorSm::E_LOCK);
	doorSm.printDoor(doorSm.door);

	TRANSFER(doorSm, DoorSm::E_CLOSE);
	doorSm.printDoor(doorSm.door);

	return 0;
}
