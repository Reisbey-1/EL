// Hall.h: Schnittstelle für die Klasse CBuilding.

/// m_bFloorRequest[2][NUM_FLOORS]
/// 'x' : true
/// ' ' : false
//
///   0     1    2    3    4    5    6    7    8    9    10    
/// ---------------------------------------------------------       
//  | x  |    |    |    | x   |    |    |    |    |    |    |    UP   
/// ---------------------------------------------------------       
//  | x  |    |    |    | x   |    |    |    |    |    |    |    DOWN  
/// ---------------------------------------------------------       

#pragma once
#include "stdafx.h"
#include "Elevator.h"
#include "HallState.h"
#include"Passenger.h"
namespace ELLib
{
	typedef  void(*pCStairfunc) (void** ppObject, BYTE * msg, BYTE size);

	class ELLIB_API CHall
	{
	public:

	public:
		CHall();
		CHall(CBuilding *pContainer);
		virtual	~CHall() ;
		void SetStairNo(int nNo);// { m_nStairNo = nNo; }
		void IncreaseFloorRequestDensity(const BYTE dir);
		void DecreaseFloorRequestDensity(const BYTE dir);
		BYTE GetFloorRequestDensity(const BYTE dir);
		BYTE m_PDO[NUM_NODES][8];

		// can
		void InsertNodeInterface(ICAN_Node** pNodeInterface);
		void GetNodeInterface(ICAN_Node** pNodeInterface);

		void Update(tCAN& canMsg);
		void GetState(CHallState& st);
		bool setget_PDO(tCAN& msg, bool get = true);
		void sycroniseLOCALS();
		static void CallBackFunction(void** ppObject, BYTE* f, BYTE size);

		CPassenger * GetUpPassenger(BYTE bStairNo);
		CPassenger * GetDownPassenger(BYTE bStairNo);

	protected:
		BYTE m_nStairNo;					// starting from 0
		BYTE m_nMeIndex;					// starting from NUM_CARS
		BYTE m_nFloorRequestDensity[2];		// yukari/asagi  kattan gelen talep yogunlugu
		bool m_bFloorRequestServiced[2];	// yukari/asagi  kattan gelen talep yogunlugu
		ICAN_Node *m_pICAN_Nod;
		CBuilding * m_pBuilding;
		L_List<CPassenger> m_PsngrList;		// passenger list

		void execute(BYTE* f, BYTE size);

	};
}	//name space 

