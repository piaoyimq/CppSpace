/*
 * fsm-transfer-table.cpp
 *
 *  Created on: 2016年6月9日
 *      Author: piaoyimq
 */

/*
 * 状态机在工程中使用非常的频繁，有如下常见的三种实现方法：
 1. switch-case 实现，适合简单的状态机；
 2. 二维状态表state-event实现，逻辑清晰，但是矩阵通常比较稀疏，而且维护麻烦；
 3. 用状态转移表stateTransfer Table实现，数组大小等于状体转移边个数，易扩展；
 下面用一个例子来进行详细说明，描述的如下场景：

 描述对象：门
 状态：开着、关着、锁着 (这里的关着指关了但未锁的状态)
 事件：开门、关门、上锁、解锁

 代码实现用枚举来定义状态和事件，操作数据节点转移到目的状态用函数实现。枚举本身默认是从0开始的int类型，利用这个特点将状态转移函数放到函数指针数组中与状态对应起来，方便操作。
 核心数据结构如下：

 状态：枚举类型
 事件：枚举类型
 状态转移结构体：{当前状态、事件、下个状态}，定义一个全局数组来使用
 状态变更函数：到下个状态(放到数组中与状态枚举对应起来)

 此种实现方法容易扩展，增加状态和事件都比较容易。如果存在一个状态通过对应事件可以转移到多个状态的情形，则可以扩展状态转移函数，或者在状态转移结构中增加一个判断函数字段。
 *
 *
 * */

#include <iostream>
using namespace std;

typedef enum
{
	OPENED, CLOSED, LOCKED,
} State;

typedef enum
{
	OPEN, CLOSE, LOCK, UNLOCK
} Event;

typedef struct
{
	State currentState;
	Event event;
	State NextState;
} StateTransfer;

typedef struct
{
	State state;
	int transferTimes;
} Door;

StateTransfer g_stateTransferTable[]
								   {
										   { OPENED, CLOSE, CLOSED },
										   { CLOSED, OPEN, OPENED },
										   { CLOSED, LOCK, LOCKED },
										   { LOCKED, UNLOCK, CLOSED },
								   };

void toOpen(Door& door);
void toClose(Door& door);
void toLock(Door& door);
typedef void (*pfToState)(Door& door);
pfToState g_pFun[] = { toOpen, toClose, toLock }; //状态枚举值(State)对应下标

void toOpen(Door& door)
{
	door.state = OPENED;
	cout << "open the door!\n";
}

void toClose(Door& door)
{
	door.state = CLOSED;
	cout << "close the door!\n";
}

void toLock(Door& door)
{
	door.state = LOCKED;
	cout << "lock the door!\n";
}

void transfer(Door& door, const Event event)
{
	for (int i = 0; i < sizeof(g_stateTransferTable) / sizeof(StateTransfer); ++i)
	{
		if (door.state == g_stateTransferTable[i].currentState && event == g_stateTransferTable[i].event)
		{
			g_pFun[g_stateTransferTable[i].NextState](door);
			door.transferTimes++;
			cout << "transfer ok!\n";
			return;
		}
	}
	cout << "This event cannot transfer current state!!\n";
	return;
}

void printDoor(const Door& door)
{
	string stateNote[] = { "opened", "closed", "locked" }; // 下标正好对应状态枚举值
	cout << "the door's state is: " << stateNote[door.state] << endl;
	cout << "the door transfer times is: " << door.transferTimes << endl;
}

int main()
{
	Door door = { CLOSED, 0 };
	printDoor(door);
	transfer(door, OPEN);
	printDoor(door);
	transfer(door, LOCK);
	printDoor(door);
	transfer(door, CLOSE);
	printDoor(door);
	return 0;
}
