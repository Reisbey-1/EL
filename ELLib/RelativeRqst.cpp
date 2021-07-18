#include "stdafx.h"
#include "RelativeRqst.h"

namespace ELLib
{
	CRelativeRqst::CRelativeRqst(void)
	{
		for (int i = 0; i < 5; i++)
			m_nRgst[i] = INVALID;  // invalid
	}

	CRelativeRqst::~CRelativeRqst(void)
	{
	}
	
	// nearest passenger density from down
	BYTE CRelativeRqst::npd_from_down()
	{
		if (UpReguestFromDOWN() == INVALID) return DownReguestFromDOWN();
		else if (DownReguestFromDOWN() == INVALID) return UpReguestFromDOWN();
		else  return (UpReguestFromDOWN() < DownReguestFromDOWN()) ? UpReguestFromDOWN() : DownReguestFromDOWN();
	}
	
	// nearest passenger density from up
	BYTE CRelativeRqst::npd_from_up()
	{
		if (UpReguestFromUP() == INVALID) return DownReguestFromUP();
		if (DownReguestFromUP() == INVALID) return UpReguestFromUP();
		else  return (UpReguestFromUP() < DownReguestFromUP()) ? UpReguestFromUP() : DownReguestFromUP();
	}
	
	///////////////////////////////////////////////////
	BYTE CRelativeRqst::UpReguestFromUP()
	{
		return m_nRgst[iUUPP_RQ_FROM_UUPP];
	}
	BYTE CRelativeRqst::DownReguestFromUP()
	{
		return m_nRgst[iDOWN_RQ_FROM_UUPP];
	}
	BYTE CRelativeRqst::UpReguestFromDOWN()
	{
		return m_nRgst[iUUPP_RQ_FROM_DOWN];
	}
	BYTE CRelativeRqst::DownReguestFromDOWN()
	{
		return m_nRgst[iDOWN_RQ_FROM_DOWN];
	}

	CRelativeRqst::CRelativeRqst(BYTE  bFloorDensity[2][NUM_FLOORS], const bool  bCarDestination[NUM_FLOORS], BYTE nCurrentFloor)
	{
		
		for (int i = 0; i < 5; i++)
			m_nRgst[i] = INVALID;			// invalid


		BYTE lengt = 100;
		m_indexNearest = iCR_FLOOR;
		m_bSR_UP_Up		= false;		// up request   from UP
		m_bSR_UP_Dwn	= false;		// down request from UP
		m_bSR_DOWN_Up	= false;		// up request   from DOWN
		m_bSR_DOWN_Dwn	= false;		// down request from DOWM

		m_bSR_UP_Stair	= false;		// up request from stair
		m_bSR_DOWN_Stair = false;		// down request from stair

		m_bCD_UP		= false;		// kabinden yukariya talep var
		m_bCD_DOWN		= false;		// kabinden asagiya talep var
		m_bCD_STAIR		= false;		// kabinden kabinin bulundug kata talep var


		bFloorDensity[UP][NUM_FLOORS] = 0;
		bFloorDensity[DOWN][0] = 0;

		m_nRgst[iCR_FLOOR] = nCurrentFloor;
		// 1) ust katlari kontrol et
		for (int j = nCurrentFloor + 1; j < NUM_FLOORS; j++) {
				if (bFloorDensity[UP][j]) {
				m_nRgst[iUUPP_RQ_FROM_UUPP] = j;
				lengt = j- nCurrentFloor;
				m_indexNearest = iUUPP_RQ_FROM_UUPP;
				m_bSR_UP_Up = true;
				break;
			}
		}
		for (int j = nCurrentFloor+1; j < NUM_FLOORS; j++) {
			if (bFloorDensity[DOWN][j]) {
				m_nRgst[iDOWN_RQ_FROM_UUPP] = j;
				if (j - nCurrentFloor < lengt)
				{
					m_indexNearest = iDOWN_RQ_FROM_UUPP;
					lengt = j - nCurrentFloor;
					m_bSR_UP_Dwn = true;
				}
				break;
			}
		}

		// false if there is up request from top stair 
		if (nCurrentFloor + 1 == NUM_FLOORS)
			m_nRgst[iUUPP_RQ_FROM_DOWN] = INVALID;

		// 2) alt katlari kontrol et
		///for (int j = nCurrentFloor; j >= 0; j--) {
		for (int j = nCurrentFloor; j >= 0; j--)
		{
				if (bFloorDensity[UP][j] ) {
				m_nRgst[iUUPP_RQ_FROM_DOWN] = j;
				if (nCurrentFloor - j  < lengt)
				{
					m_indexNearest = iUUPP_RQ_FROM_DOWN;
					lengt = nCurrentFloor - j;
					m_bSR_DOWN_Up = true;
				}
				break;
			}
		}
		///for (int j = nCurrentFloor; j >= 0; j--) {
		for (int j = nCurrentFloor; j >= 0; j--)
		{
			if (bFloorDensity[DOWN][j]) 
			{
				m_nRgst[iDOWN_RQ_FROM_DOWN] = j;
				if (nCurrentFloor - j < lengt )
				{
					m_indexNearest = iDOWN_RQ_FROM_DOWN;
					lengt = nCurrentFloor - j;
					m_bSR_DOWN_Dwn = true;
				}
				break;
			}
		}
	
		// false if there is down request from  stair 0
		if (nCurrentFloor  == 0)
			m_nRgst[iDOWN_RQ_FROM_DOWN] = INVALID;



		// 3) kabin katindan talepler
		if (bFloorDensity[UP][nCurrentFloor] && nCurrentFloor < NUM_FLOORS) {
			m_bSR_UP_Stair = true;										/// kabin katindan yukari yonlu talep varsa
		}
		if (bFloorDensity[DOWN][nCurrentFloor] && nCurrentFloor > 0) {
			m_bSR_DOWN_Stair = true;									/// kabin katindan asagi yonlu talep varsa
		}

		// 4) kabinden yukariya talep varsa
		if (nCurrentFloor < NUM_FLOORS - 1)
			for (BYTE i = nCurrentFloor + 1; i < NUM_FLOORS; i++)
			{
				if (true == bCarDestination[i])
				{
					m_bCD_UP = true;
					break;
				}
			}

		// 5) kabinden asagiya talep varsa
		if (nCurrentFloor > 0)											//	UPD_01
			for (int i = nCurrentFloor - 1; i >= 0; i--)
			{
				if (true == bCarDestination[i])
				{
					m_bCD_DOWN = true;
					break;
				}
			}

		// 6) kabinden kabinin bulundugu kata talep varsa
		if ((true == bCarDestination[nCurrentFloor]))
		{
			m_bCD_STAIR = true;
		}
		/////////////////////////////



		/////////////////////
		// keep only nearest
		if (m_indexNearest != iDOWN_RQ_FROM_DOWN)		{
			m_nRgst[iDOWN_RQ_FROM_DOWN] = INVALID;
			m_bSR_DOWN_Dwn = false;
		}

		if (m_indexNearest != iUUPP_RQ_FROM_DOWN) {
			m_nRgst[iUUPP_RQ_FROM_DOWN] = INVALID;
			m_bSR_DOWN_Up = false;
		}

		if (m_indexNearest != iDOWN_RQ_FROM_UUPP) {
			m_nRgst[iDOWN_RQ_FROM_UUPP] = INVALID;
			m_bSR_UP_Dwn = false;
		}

		if (m_indexNearest != iUUPP_RQ_FROM_UUPP) {
			m_nRgst[iUUPP_RQ_FROM_UUPP] = INVALID;
			m_bSR_UP_Up = false;
		}

	/*	*/
	}
} // namespace
