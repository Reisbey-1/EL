#include "stdafx.h"
#include "ElevatorState.h"
#include "defines.h"
namespace ELLib
{
	CElevatorState::CElevatorState()
	{
		m_nCarNumber = 0;
		m_nFloor = 0;;
		m_nDirection = STOP;
		m_nUnloadingTimer = 0;
		m_nLoadingTimer = 0;
		m_nModulus = 0;

		for(int i = 0; i < NUM_FLOORS ; i++)
			m_bCarDestination[i]  = false;

		m_nX = SPACINGX;
		//m_nY = SPACINGY / NUM_FLOORS-1;
		m_nY = SPACINGY;
		m_nOffset = SPACINGX;
	};

	void CElevatorState::Clear()
	{
		m_nCarNumber = 0;
		m_nFloor = 0;;
		m_nDirection = STOP;
		m_nUnloadingTimer = 0;
		m_nLoadingTimer = 0;
		m_nModulus = 0;

		for (int i = 0; i < NUM_FLOORS; i++)
			m_bCarDestination[i] = false;
	};
	CElevatorState::CElevatorState(BYTE nCarNumber,
		BYTE nFloor,
		BYTE nDirection,
		BYTE nDirectionHistory,

		BYTE nUnloadingTimer,
		BYTE nLoadingTimer,
		BYTE nModulus)
	{
		m_nCarNumber = nCarNumber;
		m_nFloor = nFloor;
		m_nDirection = nDirection;  // STOP
		m_nDirectionHistory = nDirectionHistory;
		m_nUnloadingTimer = nUnloadingTimer;
		m_nLoadingTimer = nLoadingTimer;
		m_nModulus = nModulus;
		m_nX = SPACINGX;
		//m_nY = SPACINGY / NUM_FLOORS - 1;
		m_nY = SPACINGY;
		m_nOffset = SPACINGX;

		for (int i = 0; i < NUM_FLOORS; i++)
			m_bCarDestination[i] = false;

	}

	CElevatorState::~CElevatorState() 
	{}


	CElevatorState& CElevatorState::operator = (const CElevatorState& src) 
	{
		if (this == &src)
			return *this;
		m_nCarNumber = src.m_nCarNumber;
		m_nFloor = src.m_nFloor;
		m_nDirection = src.m_nDirection;  // STOP
		m_nDirectionHistory = src.m_nDirectionHistory;
		m_nUnloadingTimer = src.m_nUnloadingTimer;
		m_nLoadingTimer = src.m_nLoadingTimer;
		m_nModulus = src.m_nModulus;
		for (int i = 0; i < NUM_FLOORS; i++)
			m_bCarDestination[i] = src.m_bCarDestination[i];

		m_nX = src.m_nX;
		m_nY = src.m_nY;
		m_nOffset = src.m_nOffset;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Encoding DATA BYTES
	// ===================
	//            BYTE                     BYTE      BYTE     BYTE      BYTE     BYTE     BYTE      BYTE
	//			____________________________________________________________________________________________
	//         |   0                    |   1     |   2    |   3     |   4    |   5     |   6     |   7     |
	//		   |________________________|_________|________|_________|________|_________|_________|_________|
	//		   |                        |         |        |         |        |         |         |         |
	//		   | |0|s|s|w|id|id|id|id|  |         |        |         |        |         |         |         |    
	//		   |________0_______________|_________|________|_________|________|_________|_________|_________|
	//
	//
	//			BYTE 0		: [- - - - id id id id ]	node Id (meIndex)   
	//			BYTE 0		: [- - - w -  -  -  -  ]	identifier w = 0 elevator,  w = 1 floor   
	//			BYTE 0		: [s s s - -  -  -  -  ]	msg type :  state if < 4, 	command if >= 4		CMS_TYPE_STATE = 0x0, CMS_TYPE_COMMAND =  0x04
	//			BYTE 1		: [dr,dr,dr,fr,fr,fr,fr,fr] current direction [UP, DOWN, STOP],	(3 byte)current floor (5 byte),		 
	//			BYTE 2		: modulus								
	//			BYTE 3		: unloading timer [*1000ms] (High nibble),		loading timer [*1000ms] (low nibble)	 		
	//			BYTE 3		: loading timer [*1000ms]	(High nibble),		loading timer [*1000ms] (low nibble)	 		
	//			BYTE 4		: reserved // CarDestination[NUM_FLOORS]  (LOW)
	//			BYTE 5		: reserved // CarDestination[NUM_FLOORS]  (HIGH)
	//			BYTE 6		: reserved
	//			BYTE 7		: reserved							
	//			________________________________________________________________________________________________
	//
	//
	////////////////////////////////////////////////////////////////////////////////////////////
	void CElevatorState::GetCanState(tCAN& can_ELState)
	{
		can_ELState.data[0] = (STT_BROADCAST << 5 ) | (0x00) | (0x0F & m_nCarNumber);
		can_ELState.data[1] = (m_nDirection << 5)  | (0x1F & m_nFloor);
		can_ELState.data[2] = m_nModulus; 
		can_ELState.data[3] = (m_nUnloadingTimer << 4) | (0x0F & m_nLoadingTimer);

		WORD wDmuster = 0;
		// destination 
		for (int k = NUM_CARS-1; k >= 0; k--) 	{
			if (m_bCarDestination[k] == true)
				wDmuster = wDmuster | 0x0001;
			wDmuster = wDmuster << 1;
		}
		wDmuster = wDmuster >> 1;					// one more shifted ! come back...

		can_ELState.data[4] = 0;					// reserved
		can_ELState.data[5] = 0;					// reserved	

		can_ELState.data[6] = 0;					// reserved
		can_ELState.data[7] = 0;					// reserved

		can_ELState.id = CMS_ID_ELV;
		can_ELState.rtr = 1;
		can_ELState.length = 29;
	
	}

	// Encoding COMMAND BYTES
	// ======================
	//            BYTE                     BYTE      BYTE     BYTE      BYTE     BYTE     BYTE      BYTE
	//			____________________________________________________________________________________________
	//         |   0                    |   1     |   2    |   3     |   4    |   5     |   6     |   7     |
	//		   |________________________|_________|________|_________|________|_________|_________|_________|
	//		   |                        |         |        |         |        |         |         |         |
	//         | |1|s|s|w|id|id|id|id|  |         |        |         |        |         |         |         |    CMS_TYPE_COMMAND : S >= 0x4
	//		   |________0_______________|_________|________|_________|________|_________|_________|_________|
	//		   |                        |         |        |         |        |         |         |         |
	//         | |1|0|0|w|id|id|id|id|  | stair   |  -     |   UP    |        |         |         |         |    CMS_TYPE_COMMAND : S = 1			: building node should update  floor-request-matrix (service done)
	//         |	s = 4				| nodId   |        |   DOWN  |        |         |         |         |    CMS_TYPE_COMMAND : S = 1			: building node should update  floor-request-matrix (service done)
	//		   |________________________|_________|________|_________|________|_________|_________|_________|
	//
	//
	//			BYTE 0		: node Id  [0-15] (low nibble)				    :	sending node id
	//			BYTE 0		: command  [0-7] (high nibble)				    :	sending node id
	//			BYTE 1		: who											:	the node id whic should execute the action
	//			BYTE 2		: #passenger									:	number of passengers to be cleared from hall density list default 1		
	//			BYTE 3		: UP / DOWN  /BOTH								:   density list to be cleared
	//			BYTE 4		: reserved
	//			BYTE 5		: reserved  
	//			BYTE 6		: reserved
	//			BYTE 7		: reserved						
	//
	//			action will be performed -->  bFloorRequest[UP/DOWN][floor number] = false ;
	//			___________________________________________
	////////////////////////////////////////////////////////////////////////////////////////////	
	bool CElevatorState::GetCanCommand(tCAN& cmd, BYTE command, BYTE targetNodId, BYTE direction, BYTE passCount)
	{
		if (command == CMD_CLEAR_REQUEST)
		{
			cmd.id = CMS_ID_UPB;
			cmd.data[0] = (CMD_CLEAR_REQUEST << 5) | (0x00) | (0x0F & m_nCarNumber  );		// w = 0
			cmd.data[1] = targetNodId;
			cmd.data[2] = passCount;		// default number to be cleared 	
			cmd.data[3] = direction;
			cmd.data[4] = 0;				// number of passengers to be cleared from hall density list default 1
			cmd.data[5] = 0;				// reserved
			cmd.data[6] = 0;				// reserved
			cmd.data[7] = 0;				// reserved
			return true;
		}
		return false;
	}
	bool CElevatorState::BuildFromPDOEntry(tCAN& can_ELState)
	{
		// valid elevator objec ?
		bool valid = (can_ELState.data[0] & (0xF0)) == STT_BROADCAST;	// STT_BROADCAST = 0; w = 0
		if (!valid)
			return valid;
		WORD wCD = 0;

		m_nCarNumber		= can_ELState.data[0] & 0x0F;
		m_nDirection = can_ELState.data[1] >> 5;
		m_nFloor = can_ELState.data[1] & 0x1F;
		m_nModulus			= can_ELState.data[2] ;
		m_nLoadingTimer		= can_ELState.data[3] & 0x0F ;
		m_nUnloadingTimer	= can_ELState.data[3] >> 4;
	
	/*
		wCD					= can_ELState.data[5];				// high
		wCD					= wCD << 8;							
		wCD					= wCD | can_ELState.data[4];		// low

		//for (int i = 0; i < NUM_FLOORS; i++)
		for (int i = NUM_CARS - 1; i >= 0; i--)
		{
			if ((wCD & 0x0001) == 1)
			{
				m_bCarDestination[i] = true;
			}
			else 
				m_bCarDestination[i] = false;
			wCD	= wCD >> 1;										// SHIFT RIGHT ONE POSITION	
		}
*/

		return valid;
	}

} // name space