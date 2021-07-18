// RelativeRqst.h
#pragma once
#include "defines.h"

///   0     1    2    3    4    5    6    7    8    9    10 
///
///
//  |----|---------------------------------------------------       
//  | 4  |    UpReguestFromUP()                             |   (offset) nearest UP request  (ABOVE) 
//  |----|---------------------------------------------------       
//  | 3  |    DownReguestFromUP()                           |   (offset) nearest DOWN request (ABOVE)    
//  |----|--------------------------------------------------|      
//  | 2  |-----Currtent floor-------------------------------|    
//  |----|--------------------------------------------------|      
//  | 1  |    UpReguestFromDOWN()                           |    (offset) nearest UP request (BELOW)    
//  |----|---------------------------------------------------       
//  | 0  |    DownReguestFromDOWN()                         |    (offset) nearest DOWN request (BELOW)   
//  |----|---------------------------------------------------       

namespace ELLib
{
	class ELLIB_API CRelativeRqst
	{
		friend class CElevator;
		friend class CLift;
	public:
		enum rgstIndex
		{
			iDOWN_RQ_FROM_DOWN		= 0,			// nearest request : down request from down  
			iUUPP_RQ_FROM_DOWN		= 1,			// nearest request : up request from down  		
			iCR_FLOOR				= 2,
			iDOWN_RQ_FROM_UUPP		= 3,
			iUUPP_RQ_FROM_UUPP		= 4
		} ;
		CRelativeRqst(void);
		CRelativeRqst(BYTE	 bFloorDensity[2][NUM_FLOORS], const bool  bCarDestination[NUM_FLOORS], BYTE nCurrentFloor);		// floor density 
		~CRelativeRqst(void);
		
		BYTE npd_from_down();			// returns nearest passenger density from down
		BYTE npd_from_up();				// returns nearest passenger density from up
	protected:
		BYTE UpReguestFromUP();
		BYTE UpReguestFromDOWN();
		BYTE DownReguestFromUP();
		BYTE DownReguestFromDOWN();

	protected:
		BYTE m_nRgst[5];						
	
		/**/
		bool m_bSR_UP_Up;				// up stair-request   from UP
		bool m_bSR_UP_Dwn;				// down stair-request from UP
		bool m_bSR_DOWN_Up;				// up stair-request   from DOWN
		bool m_bSR_DOWN_Dwn;			// down stair-request from DOWM
		bool m_bSR_UP_Stair;			// up floor-request from curent stair
		bool m_bSR_DOWN_Stair;			// down floor-request from current stair

		bool m_bCD_UP ;					// up car-request   - kabinden yukariya talep var
		bool m_bCD_DOWN ;				// down car-request  - kabinden asagiya talep var
		bool m_bCD_STAIR ;				// car-request to current stair - kabinden kabinin bulundug kata talep var
		rgstIndex m_indexNearest;		// activated nearest request index
	};
} // namespace
