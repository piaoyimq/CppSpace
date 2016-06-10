/*
 * fsm-transfer.h
 *
 *  Created on: 2016-06-09
 *      Author: piaoyimq
 */
#ifndef 	FSM_TRANSFER_H
#define 	FSM_TRANSFER_H
#include <iostream>



/* Attention:
 * If you need include this directory,
 * state machine class name must named Sm, and have a attribute named currentAtr defined by CurrentAtr,
 * transfer table must named stateTransferTable.
 *event caller table must named  eventCallerTable.
 * */

#define TRANSFER(Smobject, event) 		STATE_TRANSFER(Smobject, event, Sm, currentAtr, stateTransferTable, eventCallerTable)

#define STATE_TRANSFER(arg1, arg2, arg3, arg4, fp1, fp2) \
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


namespace fsm
{

template<typename S>
struct CurrentAtr
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
}
#endif
