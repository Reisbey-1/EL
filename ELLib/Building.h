// Building.h: Schnittstelle für die Klasse CBuilding.

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
      
//
//
//////////////////////////////////////////////////////////////////////
/// 
///  ///  UPDATE    adding new version of cabin objects


#if !defined(AFX_BUILDING_H__BE78D113_B298_4E6F_A411_EA331B70985C__INCLUDED_)
#define AFX_BUILDING_H__BE78D113_B298_4E6F_A411_EA331B70985C__INCLUDED_
#include "stdafx.h"
#include "Elevator.h"
#include "Hall.h"

namespace ELLib
{
	typedef  void (*pCBfunc_building) (void** ppObject, BYTE * msg, BYTE size);
	class ELLIB_API CBuilding
	{
		friend class CLift;
		friend class CElevator;
	public:
		CBuilding();
		virtual	~CBuilding();
		CHall* GetStairAt(int nAt) { return m_pStairVector[nAt]; }
	protected:
		CHall *m_pStairVector[NUM_FLOORS];
	};
} // namespace
#endif // !defined(AFX_BUILDING_H__BE78D113_B298_4E6F_A411_EA331B70985C__INCLUDED_)
