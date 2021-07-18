#include "stdafx.h"
#include "Passenger.h"
namespace ELLib
{
	int CPassenger::m_nObjetCounter = 0;
	CPassenger::CPassenger() :
	m_id(0), m_nEntrance(0), m_nExit(0), m_bServiced(false) 
	{
		m_nObjetCounter++;
	}
	CPassenger::CPassenger(long id, BYTE nEntrance, BYTE nExit) :
		m_id(id), m_nEntrance(nEntrance), m_nExit(nExit) , m_bServiced(false)
	{
		m_nObjetCounter++;

	}
	CPassenger& CPassenger::operator = (const CPassenger& src)
	{
		if (this == &src)
			return *this;
		m_id		= src.m_id;
		m_nEntrance = src.m_nEntrance;
		m_nExit		= src.m_nExit;
		m_bServiced = m_bServiced;
		return *this;
	}
	bool CPassenger::operator == (const CPassenger& src) const
	{
		/*
		if (src.m_id == m_id)
			return true;
		return false;	*/
		
		return ((src.m_bServiced == m_bServiced) && (src.m_nEntrance== m_nEntrance) && (src.m_nExit == m_nExit) && (src.m_id == m_id));
	
	}

	CPassenger::~CPassenger()
	{
		int i = 0;
		i = i;
		m_nObjetCounter--;
	}
}	// name space