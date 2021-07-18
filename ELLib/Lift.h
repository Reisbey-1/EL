#pragma once
#include "elevator.h"
namespace ELLib
{
	class ELLIB_API CLift : public CElevator
	{
	public:
		CLift();
		//CLift(CBuilding* pBuilding, int nCarNumber);
		CLift( int nCarNumber);
		virtual bool rank();
		//virtual bool rankV4();
		//virtual bool rankV4_2();
		virtual ~CLift(void);
	};
}
