#pragma once
#include "defines.h"
namespace ELLib
{
	class ELLIB_API CHallState
	{
	public:
		CHallState();
		CHallState(int nId);
		CHallState(BYTE nFloorRequestDensity[2], int nId);
		CHallState& operator = (const CHallState& src);
		~CHallState();

		void GetCanState(tCAN& can_BLDState);
		bool BuildFromPDOEntry(const tCAN can_BLDState);
		BYTE GetFloorRequestDensity(BYTE updown);
		void SetFloorRequestDensity(BYTE updown, BYTE val);
	
		void SetHallNo(int val) { m_nStairNo = val; }
		BYTE GetHallNo() { return m_nStairNo; }

		void SetMeIndex(int val) { m_nMeIndex = val; }
		BYTE GetMeIndex() { return m_nMeIndex; }
		BYTE GetX() { return m_nX; }
		BYTE GetY() { return m_nY; }
		BYTE GetOffset() { return m_nOffset; }


	protected:
		BYTE	m_nFloorRequestDensity[2];
		BYTE	m_nStairNo;
		BYTE    m_nMeIndex;

		BYTE m_nX;
		BYTE m_nY;
		BYTE m_nOffset;
	};
}	// name space

