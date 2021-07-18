#include "stdafx.h"
#include "WEL_Process.h"
#include "WinELDlg.h"
#include "Lift.h"


// Global CAN-NODES
ICAN_Bus *					theCanBus;					
CBuilding *					pTheBuilding;
CElevator *					pLifts[NUM_CARS];
LONG						lcookie[NUM_CARS + NUM_FLOORS];		// 
CNonModalDlg<CLiftDlg> *	pTheLiftDialogs[NUM_CARS] ;
CPerformanceCounter			myClock(10);

LONG clock_tick()
{
	MasterTick();
	if (myClock.IsAlive())
		myClock.Reset();
	return 1;
}
void init()
{
	for (int i = 0; i < NUM_CARS; i++) {
		lcookie[i] = 0;				// registered cookies null for lift nodes
									//		theLiftNodes[i] = NULL;			// node interface is null at beginning
	}
	lcookie[NUM_CARS] = 0;			// last cookie for bulding node interface; null

	for (int i = 0; i < NUM_CARS; i++)
		pTheLiftDialogs[NUM_CARS] = NULL;
	
	// create buildings
	pTheBuilding = new CBuilding;

	// create lifts
	for (int i = 0; i < NUM_CARS; i++)
	{
		pLifts[i] = new CLift(i);
		pLifts[i]->set_building(pTheBuilding);
	}

	Sleep(100);
	// create can interface
	if (false == _Connect())
		AfxMessageBox(L"can interface error !");
	Sleep(100);


}
void MasterTick()
{
	////////////////////////////////
	/// to do:
	/// ======
	/// 00)	MODEL	-->		NODE
	/// _______________________________________________________________________
	/// 1a) NODE	-->		CANBUS	--->	NODE	(building)
	///	1b) NODE	-->		CANBUS	--->	NODE	(elevator)			
	/// _______________________________________________________________________
	///	2a) NODE	-->		MODEL
	///	2b) MODEL	-->		LOCALS
	/// _______________________________________________________________________
	/// 3a) activate car_tic1 signal to all lift object												(PROGRESS - MODEL)
	/// 3b) activate car_tic2 signal to all lift object												(PROGRESS)

	/// 4)  redraw.

	int j, k;
	CHallState ss;
	CElevatorState  es;
	///tCAN msg;
	ICAN_Node *	pCanNod = 0;							// CURRENT NODE			

														//CLiftDlg * pElevator;
	bool bVal = false;

	for (k = 0; k < NUM_CARS; k++)
	{
		pLifts[k]->sycroniseLOCALS();					// get state object from locales
	}
	for (k = 0; k < NUM_FLOORS; k++)
	{
		pTheBuilding->GetStairAt(k)->sycroniseLOCALS();	// get state object from locales
	}
	//return;



	// RUN
	/////////////////////////////////////////////////////////////
	// -3a-
	for (j = 0; j < NUM_CARS; j++)
		pLifts[j]->car_tick1();					// 1. saat sinyalini gonder

													// RUN
													/////////////////////////////////////////////////////////////
													// -3b-
	for (j = 0; j < NUM_CARS; j++)
		pLifts[j]->car_tick2();					// 2. saat sinyalini gonder

													/////////////////////////////////////////////////////////////
	try {												// -4-
		for (j = 0; j < NUM_CARS; j++)
		{
			pTheLiftDialogs[j]->Invalidate();
		}
	}
	catch (...)
	{
		;

	}
	CWinELDlg::theObjectPtr->Invalidate();				// objet pointer to itself

}
bool _Connect()
{
	// ==================================
	// create CAN_Bus objects:
	// Global objects
	// ICAN_Bus *	theCanBus;					
	// ICAN_Nod *	theStairNodes[NUM_FLOORS]
	// ICAN_Nod *	theLiftNodes[NUM_CARS];
	// ==================================

	LONG cookie;									// dumy local

													/////////////////////////////////////////////////////////////////////////////
													// create theCanBus object
	HRESULT hr = CoCreateInstance(
		CLSID_CAN_Bus,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICAN_Bus,
		(void **)&theCanBus);


	if (hr != S_OK)
		return false;
	/////////////////////////////////////////////////////////////////////////////
	// create lifts as CAN_Nod && register them at theCanBus
	ICAN_Node *CurrentLift;
	for (int i = 0; i < NUM_CARS; i++)
	{
		hr = S_FALSE;
		hr = CoCreateInstance(
			CLSID_CAN_Node,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ICAN_Node,
			(void **)&CurrentLift);
		if (hr == S_OK)
		{
			// nod id
			CurrentLift->put_me(0x0F & (BYTE)i);						// w = 0;
																		//CurrentLift->put_nodId(i);

																		// add call back functions	
			pCBfunc_elevator pcb = &CElevator::CallBackFunction;
			CurrentLift->AddCallBack((long**)&pcb, (long**)&pLifts[i]);

			///			theLiftNodes[i] = CurrentLift;
			CurrentLift->put_CanBus((IUnknown*)theCanBus);
			theCanBus->Advice((IUnknown**)&CurrentLift, &cookie);

			// at this moment cannode interface is created and  
			// successfullyreg reistered at theCanBus
			lcookie[i] = cookie;
			// insert interface object into lift object
			pLifts[i]->InsertNodeInterface(&CurrentLift);

			// dialogs
			pTheLiftDialogs[i] = new CNonModalDlg<CLiftDlg>;
			pTheLiftDialogs[i]->SetNo(i);
			pTheLiftDialogs[i]->SetElevator(pLifts[i]);

			pTheLiftDialogs[i]->Create(CLiftDlg::IDD, 0);
			pTheLiftDialogs[i]->ShowWindow(SW_NORMAL);
		}
		else
			return false;
	}

	ICAN_Node *CurrentStair;
	for (BYTE j = 0; j < NUM_FLOORS; j++)
	{
		hr = S_FALSE;
		hr = CoCreateInstance(
			CLSID_CAN_Node,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ICAN_Node,
			(void **)&CurrentStair);
		if (hr == S_OK)
		{
			// nod id
			CurrentStair->put_me(0x0F & (j + NUM_CARS));
			CurrentStair->put_CanBus((IUnknown*)theCanBus);
			theCanBus->Advice((IUnknown**)&CurrentStair, &cookie);
			lcookie[j + NUM_CARS] = cookie;

			// add call back functions	
			pCStairfunc pcb = &CHall::CallBackFunction;
			CHall *pStair = pTheBuilding->GetStairAt(j);
			CurrentStair->AddCallBack((long**)&pcb, (long**)&pStair);
			//theStairNodes[j] = CurrentStair;

			// insert interface object into stair object
			pStair->InsertNodeInterface(&CurrentStair);
		}
		else
			return false;
	}
	return true;
}
void clean_up()
{
	myClock.KillThread();					// kill timer

	// clear up
	ICAN_Node *CurrentLift = 0;
	long l;
	for (int i = 0; i < NUM_CARS; i++)
	{
		l = 0;
		pLifts[i]->GetNodeInterface(&CurrentLift);
		if (CurrentLift)
		{
			do
			{
				l = CurrentLift->Release(); 
			} while (l);
			CurrentLift = 0;
		}
		delete pLifts[i];
	}


	ICAN_Node *CurrentStair = 0;

	for (int i = 0; i < NUM_FLOORS; i++)
	{
		l = 1;
		CHall* pStair = pTheBuilding->GetStairAt(i);
		pStair->GetNodeInterface(&CurrentStair);
		if (CurrentStair)
			while (l)
				l = CurrentStair->Release();/**/
	}

	l = 1;
	if (theCanBus)
		while (l)
			l = theCanBus->Release();

	delete pTheBuilding;
	int k = CPassenger::m_nObjetCounter;
	int m = k;

}