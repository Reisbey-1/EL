#pragma once
namespace ELLib
{
	class ELLIB_API CPassenger
	{
	public:
		CPassenger();
		CPassenger(long id, BYTE nEntrance, BYTE nExit);
		CPassenger& operator= (const CPassenger& src);
		bool operator == (const CPassenger& src) const;

		BYTE GetEntrance() { return m_nEntrance; };
		BYTE GetExit() { return m_nExit; };
		~CPassenger();
		static int m_nObjetCounter;
	protected:
		long m_id;
		bool m_bServiced;
		BYTE m_nEntrance;
		BYTE m_nExit;

	};
}	// name space

