/*
 * mainTest.cpp
 *
 *  Created on: 2016年6月11日
 *      Author: piaoyimq
 */
#include "../include/FSM.h"
#include <iostream>

FSM cFSM;
State cState[10];

bool foo = false;

//s0
void s0_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s0_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_e_s0_s21(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

//s1
void s1_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s1_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_a_s1_s1(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_b_s1_s11(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_c_s1_s2(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_d_s1_s0(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_f_s1_s21(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

//s11
void s11_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s11_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_g_s11_s211(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

//s2
void s2_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s2_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_c_s2_s1(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_f_s2_s11(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_h_s2_s2(char *args)
{
	if(true == foo)
	{
		std::cout << __FUNCTION__ << std::endl;
	}
}


//s21
void s21_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s21_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_b_s21_s211(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_g_s21_s0(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_i_s21_s2(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

//s211
void s211_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s211_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void event_d_s211_s21(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}


//s3
void s3_entry(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void s3_exit(char *args)
{
	std::cout << __FUNCTION__ << std::endl;
}

void init()
{
	cState[0].setName("s0");
	cState[0].addAction(State::eOnExit, State::eAction, "Action: none", s0_exit);
	cState[0].addAction(State::eOnEvent, State::eAction, "Action: d", "d", event_d_s1_s0);
	cState[0].addAction(State::eOnEvent, State::eAction, "Action: g", "g", event_g_s21_s0);
	cState[0].addTransition("e", &cState[4]);
	cState[0].addTransition("j", &cState[6]);

	cState[1].setName("s1");
	cState[1].addAction(State::eOnExit, State::eAction, "Action: none", s1_exit);
	cState[1].addAction(State::eOnEntry, State::eAction, "Action: none", s1_entry);
	cState[1].addAction(State::eOnEvent, State::eAction, "Action: a", "a", event_a_s1_s1);
	cState[1].addAction(State::eOnEvent, State::eAction, "Action: c", "c", event_c_s2_s1);
	cState[1].addTransition("a", &cState[1]);	// it will not call the exit action, because it was a self state transition.
	cState[1].addTransition("b", &cState[2]);
	cState[1].addTransition("c", &cState[3]);
	cState[1].addTransition("d", &cState[0]);
	cState[1].addTransition("f", &cState[4]);

	cState[2].setName("s11");
	cState[2].addAction(State::eOnExit, State::eAction, "Action: none", s11_exit);
	cState[2].addAction(State::eOnEvent, State::eAction, "Action: b", "b", event_b_s1_s11);
	cState[2].addAction(State::eOnEvent, State::eAction, "Action: f", "f", event_f_s2_s11);
	cState[2].addTransition("g", &cState[5]);

	cState[3].setName("s2");
	cState[3].addAction(State::eOnExit, State::eAction, "Action: none", s2_exit);
	cState[3].addAction(State::eOnEvent, State::eAction, "Action: c", "c", event_c_s1_s2);
	cState[3].addAction(State::eOnEvent, State::eAction, "Action: h", "h", event_h_s2_s2);
	cState[3].addAction(State::eOnEvent, State::eAction, "Action: i", "i", event_i_s21_s2);
	cState[3].addTransition("c", &cState[1]);
	cState[3].addTransition("f", &cState[2]);
	cState[3].addTransition("h", &cState[3]);

	cState[4].setName("s21");
	cState[4].addAction(State::eOnExit, State::eAction, "Action: none", s21_exit);
	cState[4].addAction(State::eOnEvent, State::eAction, "Action: e", "e", event_e_s0_s21);// listern "e" event on "s21" state.
	cState[4].addAction(State::eOnEvent, State::eAction, "Action: f", "f", event_f_s1_s21);
	cState[4].addAction(State::eOnEvent, State::eAction, "Action: d", "d", event_d_s211_s21);
	cState[4].addTransition("b", &cState[5]);
	cState[4].addTransition("g", &cState[0]);
	cState[4].addTransition("i", &cState[3]);

	cState[5].setName("s211");
	cState[5].addAction(State::eOnExit, State::eAction, "Action: none", s211_exit);
	cState[5].addAction(State::eOnEvent, State::eAction, "Action: g", "g", event_g_s11_s211);
	cState[5].addAction(State::eOnEvent, State::eAction, "Action: b", "b", event_b_s21_s211);
	cState[5].addTransition("d", &cState[4]);

	cState[6].setName("s3");
	cState[6].addAction(State::eOnEntry, State::eAction, "Action: none", s3_entry);//if have no any other action, we can add the "eOnEntry" actin as the default action
	cState[6].addAction(State::eOnExit, State::eAction, "Action: none", s3_exit);
	cState[6].addTransition("j", &cState[0]);

	// Add the states to the FSM controller
	cFSM.addStates(cState);
}


int main()
{
	init();

	while (1)
	{
		char str[100];

		std::cout << "\nCurrent state: " << cFSM.getCurrentStateName() << std::endl;
		std::cout << "Event<-";

		std::cin >> str;
		bool ret = cFSM.outEvent(str, "");
		if (true != ret)
		{
			std::cout << "Unkonw event" << std::endl;
		}
	}
	return 0;
}

