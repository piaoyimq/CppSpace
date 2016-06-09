/****************************************
 *	Author:		Nathaniel Meyer			*
 *	E-Mail:		nath_meyer@hotmail.com	*
 *	Website:	http://www.nutty.ca		*
 *										*
 *   You are free to use, redistribute,  *
 *   and alter this file in anyway, so   *
 *   long as credit is given where due.	*
 ****************************************/

#include "Main.h"
#include "resource.h"
#include <iostream>

char selEvent[50], lastState[50];
unsigned int eventID;

/******************
 STATE ACTIONS
 ******************/
// State: TurnedOn: On Entry
void sIdle_OnExit(char *args)
{
	std::cout << "OnExit" << std::endl;
}

void sIdle_TurnOff(char *args)
{
	std::cout << "TurnOff" << std::endl;
}

// State: TurnedOn: On Entry
void sTurnedOn_OnEntry(char *args)
{
	std::cout << "OnEntry" << std::endl;
}

// State: Activity: On Event: Walk
void sActivity_Walk(char *args)
{
	std::cout << "Walk" << std::endl;
}

// State: Activity: On Event: Run
void sActivity_Run(char *args)
{
	std::cout << "Run" << std::endl;
}

// State: Activity: On Event: Talk
void sActivity_Talk(char *args)
{
	std::cout << "Talk" << std::endl;
}

void init()
{
	// Idle state
	cState[0].setName("Idle");
	cState[0].addAction(State::eOnExit, State::eAction, "Action: TurnOn", sIdle_OnExit);
	cState[0].addAction(State::eOnEvent, State::eAction, "Action: TurnOff", "TurnOff", sIdle_TurnOff);
	cState[0].addTransition("TurnOn", &cState[1]);

	// TurnedOn state
	cState[1].setName("TurnedOn");
	cState[1].addAction(State::eOnEntry, State::eAction, "Action: TurnOn", sTurnedOn_OnEntry);
	cState[1].addTransition("TurnOff", &cState[0]);
	cState[1].addTransition("GoToActivity", &cState[2]);

	// Activity state
	cState[2].setName("Activity");
	cState[2].addAction(State::eOnEvent, State::eAction, "Action: Walk", "Walk", sActivity_Walk);
	cState[2].addAction(State::eOnEvent, State::eAction, "Action: Run", "Run", sActivity_Run);
	cState[2].addAction(State::eOnEvent, State::eAction, "Action: Talk", "Talk", sActivity_Talk);
	cState[2].addTransition("RequestTurnOff", &cState[1]);
	cState[2].addTransition("Walk", &cState[2]);
	cState[2].addTransition("Run", &cState[2]);
	cState[2].addTransition("Talk", &cState[2]);

	// Add the states to the FSM controller
	cFSM.addStates(cState);
}
//
///* CarFSM Dialog Box */
//BOOL CALLBACK MainDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
//{
//	switch(Message)
//	{
//		case WM_INITDIALOG:
//			gHWnd = hWnd;
//
//			// Initialize states and FSM
//			init();
//
//			strcpy(selEvent, "");
//
//			// Set combo box events
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"TurnOn");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"TurnOff");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"GoToActivity");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"Walk");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"Run");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"Talk");
//			SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR)"RequestTurnOff");
//
//			// Set default titles
//			SetDlgItemText(hWnd, IDC_LASTSTATE, "none");
//			SetDlgItemText(hWnd, IDC_CURRENTSTATE, "Idle");
//			SetDlgItemText(hWnd, IDC_STATUS, "Ready");
//			SetDlgItemText(hWnd, IDC_RESPONSE, "none");
//		break;
//
//		case WM_CLOSE:
//			EndDialog(hWnd, 0);
//			return 0;
//		break;
//
//		case WM_DESTROY:
//			EndDialog(hWnd, 0);
//			return 0;
//		break;
//
//		case WM_COMMAND:
//			switch(LOWORD(wParam))
//			{
//				case IDC_EVENTLIST:
//					switch(HIWORD(wParam))
//					{
//						case CBN_SELCHANGE:
//							// Get the selected event
//							eventID = SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_GETCURSEL, 0, 0);
//							SendDlgItemMessage(hWnd, IDC_EVENTLIST, CB_GETLBTEXT, eventID, (LPARAM)(LPCTSTR)selEvent);
//						break;
//					}
//				break;
//
//				case IDC_SENDEVENT:
//					/*
//						Validate the event
//						If it exists then process it, otherwise report an unknown event
//					*/
//					if ( strcmp(selEvent, "") != 0 )
//					{
//						// args is left blank, optionally you can add whatever you like
//						if ( cFSM.outEvent(selEvent, "") )
//						{
//							GetDlgItemText(hWnd, IDC_CURRENTSTATE, lastState, 50);
//							SetDlgItemText(hWnd, IDC_LASTSTATE, lastState);
//							SetDlgItemText(hWnd, IDC_CURRENTSTATE, cFSM.getCurrentStateName());
//							SetDlgItemText(hWnd, IDC_STATUS, selEvent);
//						}
//						else
//						{
//							SetDlgItemText(hWnd, IDC_STATUS, "Unknown Event");
//						}
//					}
//				break;
//			}
//		break;
//	}
//	return FALSE;
//}
//
//int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	// Display the dialog box
//	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
//
//	return FALSE;
//}

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
