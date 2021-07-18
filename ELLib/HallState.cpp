#include "stdafx.h"
#include "HallState.h"
namespace ELLib
{
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	CHallState::CHallState()
	{
		m_nFloorRequestDensity[UP] = 0;
		m_nFloorRequestDensity[DOWN] = 0;
		m_nStairNo = 0;
		m_nMeIndex = NUM_CARS;
		m_nX = SPACINGX;
		m_nY = SPACINGY ;
		m_nOffset = SPACINGX;
	};
	CHallState::CHallState(BYTE nFloorRequestDensity[2], int nId)
	{
		m_nFloorRequestDensity[UP] = nFloorRequestDensity[UP];
		m_nFloorRequestDensity[DOWN] = nFloorRequestDensity[DOWN];
		m_nStairNo = nId;
		m_nMeIndex = nId + NUM_CARS;
		m_nX = SPACINGX;
		m_nY = SPACINGY;
		m_nOffset = SPACINGX;
	}
	CHallState::CHallState(int nId)
	{
		m_nFloorRequestDensity[UP] = 0;
		m_nFloorRequestDensity[DOWN] = 0;
		m_nStairNo = nId;
		m_nMeIndex = nId + NUM_CARS;
		m_nX = SPACINGX;
		m_nY = SPACINGY;
		m_nOffset = SPACINGX;
	}
	CHallState& CHallState::operator = (const CHallState& src)
	{
		if (this == &src)
			return *this;
		m_nFloorRequestDensity[UP] = src.m_nFloorRequestDensity[UP];
		m_nFloorRequestDensity[DOWN] = src.m_nFloorRequestDensity[DOWN];
		m_nX = src.m_nX;
		//// ERROR
		m_nY = src.m_nY;
		m_nOffset = src.m_nOffset;
		m_nMeIndex = src.m_nMeIndex;
		m_nStairNo = src.m_nStairNo;
		return *this;
	}
	CHallState::~CHallState()
	{
		
	}
	BYTE CHallState::GetFloorRequestDensity(BYTE updown) {
		return m_nFloorRequestDensity[updown];
	}
	void CHallState::SetFloorRequestDensity(BYTE updown, BYTE val) {

		m_nFloorRequestDensity[updown] = val;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Encoding stair state into tCAN
	//
	//			____________________________________________________________________________________________
	//         |   0                    |   1     |   2    |   3     |   4    |   5     |   6     |   7     |
	//		   |________________________|_________|________|_________|________|_________|_________|_________|
	//		   |                        |         |        |         |        |         |         |         |
	//		   | |0|s|s|w|id|id|id|id|  |         |        |         |        |         |         |         |    CMS_TYPE_STATE : S < 0x04
	//		   |________1_______________|_________|________|_________|________|_________|_________|_________|
	//
	//
	//			BYTE 0		: [- - - - id id id id ]	node Id (meIndex)
	//			BYTE 0		: [- - - w -  -  -  -  ]	identifier w = 0 elevator,  w = 1 floor   
	//			BYTE 0		: [s s s - -  -  -  -  ]	msg type :  state if < 4, 	command if >= 4		
	//			BYTE 1		: [u u u u u  u  u  u  ]    floor request density UP  		
	//			BYTE 2		: [d d d d d  d  d  d ]     floor request DOWN		
	//			BYTE 3		: reserved
	//			BYTE 4		: reserved
	//			BYTE 5		: reserved
	//			BYTE 6		: reserved
	//			BYTE 7		: reserved
	//			___________________________________________
	////////////////////////////////////////////////////////////////////////////////////////////
	void CHallState::GetCanState(tCAN& can_BLDState)
	{
		can_BLDState.data[0] = (STT_BROADCAST << 5) | (0x10) | (0x0F & m_nMeIndex);  // w = 1
		can_BLDState.data[1] = m_nFloorRequestDensity[UP];		// floor request density UP
		can_BLDState.data[2] = m_nFloorRequestDensity[DOWN];	// floor request density DOWN
		can_BLDState.data[3] = 0;								// reserved 
		can_BLDState.data[4] = 0;							// reserved 
		can_BLDState.data[5] = 0;							// reserved 
		can_BLDState.data[6] = 0;							// reserved 
		can_BLDState.data[7] = 0;							// reserved 
	}
	bool CHallState::BuildFromPDOEntry(const tCAN can_BLDState)
	{
		// verify if the message type is OK
		if (!((can_BLDState.data[0] & 0xE0) == STT_BROADCAST))  // 0xE0 = 1110 0000
			return false;										// wrong message type
		else if (!((can_BLDState.data[0] & 0x10) == 0x10))
			return false;										// message is not from a Stair  W != 1
		m_nMeIndex = (can_BLDState.data[0] & 0x0F);
		m_nStairNo = m_nMeIndex - NUM_CARS;
		m_nFloorRequestDensity[UP] = can_BLDState.data[1] ;
		m_nFloorRequestDensity[DOWN] = can_BLDState.data[2];
		return true;
	}
}