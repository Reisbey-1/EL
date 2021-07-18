#pragma once
#include "LiftDlg.h"
extern ICAN_Bus *	theCanBus;
extern CBuilding *	pTheBuilding;
extern CElevator *		pLifts[NUM_CARS];
extern CNonModalDlg<CLiftDlg> * pTheLiftDialogs[NUM_CARS];
extern LONG		lcookie[NUM_CARS + NUM_FLOORS];		// 
extern CPerformanceCounter myClock;

 LONG  clock_tick();
 void  init();
 void  clean_up();
 void  MasterTick();
 bool _Connect();
 