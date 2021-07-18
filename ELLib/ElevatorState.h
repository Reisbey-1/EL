// ElevatorState.h
#pragma once
#include "stdafx.h"
#include "defines.h"
namespace ELLib
{
	class ELLIB_API CElevatorState
	{
		friend class CElevator;
	public:
		CElevatorState();
	
		CElevatorState(
			BYTE nCarNumber,
			BYTE nFloor,
			BYTE nDirection,
			BYTE nDirectionHistory,
			BYTE nUnloadingTimer,
			BYTE nLoadingTimer,
			BYTE nModulus
		);
	

		virtual ~CElevatorState();
		CElevatorState& operator = (const CElevatorState& src);

		bool BuildFromPDOEntry(tCAN& can_ELState);
		void GetCanState(tCAN& can_ELState);
		bool GetCanCommand(tCAN& cmd, BYTE command, BYTE targetNodId, BYTE direction, BYTE passCount);
		void Clear();
		
		BYTE GetMe() { return m_nCarNumber; }
		void SetMe(BYTE val) { m_nCarNumber = val; }
		
		BYTE GetFloor() { return m_nFloor; }
		void SetFloor(BYTE val) { m_nFloor = val; }
		
		BYTE GetDirection() { return m_nDirection; }
		void SetDirection(BYTE val) { m_nDirection = val; }

		BYTE GetDirectionHistory() { return m_nDirectionHistory; }
		void SetDirectionHistory(BYTE val) { m_nDirectionHistory = val; }

	
		BYTE GetModulus() { return m_nModulus; }
		void SetModulus(BYTE val) { m_nModulus = val; }
		
		BYTE GetUnloadingTimer() { return m_nUnloadingTimer; }
		void SetUnloadingTimer(BYTE val) { m_nUnloadingTimer = val; }

		BYTE GetLoadingTimer() { return m_nLoadingTimer; }
		void SetLoadingTimer(BYTE val) { m_nLoadingTimer = val; }
	
		bool GetCarDestination(BYTE i) { return m_bCarDestination[i]; }


		int m_nX;
		int m_nY;
		int m_nOffset;

	protected:
		BYTE m_nCarNumber;					// elevator index
		BYTE m_nFloor;						// current stair
		BYTE m_nDirection;					// direction (UP, DOWN, STOP )
		BYTE m_nDirectionHistory;			// direction before STOP
		BYTE m_nModulus;
		BYTE m_nUnloadingTimer;
		BYTE m_nLoadingTimer;

		bool m_bCarDestination[NUM_FLOORS];		// asansore binenler tarafindan secilir istek katlari	

	};

} // name space

