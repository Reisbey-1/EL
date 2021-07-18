// Building.cpp: Implementierung der Klasse CBuilding.
//  
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Building.h"

#include "Lift.h" 


namespace ELLib
{
	CBuilding::CBuilding()
	{
		// katlari olustur
		for (int j = 0; j < NUM_FLOORS; j++)
		{
			m_pStairVector[j] = new CHall(this);
			m_pStairVector[j]->SetStairNo(j);
			
		#ifdef _DEBUG
					if (j == 4)
					{ 
						/*
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);
						m_pStairVector[j]->IncreaseFloorRequestDensity(DOWN);*/
						for (int k = 0; k < NUM_FLOORS; k++)
						{
				

						}
					}

		#endif // DEBUG

		}
	 }
	CBuilding::~CBuilding()
	{
		for (int k = 0; k < NUM_FLOORS; k++)
			delete m_pStairVector[k];
	}
} // name space

