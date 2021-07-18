#include "stdafx.h"
#include "Hall.h"
namespace ELLib
{
	CHall::CHall()
	{
		for (int i = 0; i < NUM_NODES; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				m_PDO[i][j] = 0;
			}
		}
		m_pICAN_Nod = NULL;
		m_nFloorRequestDensity[UP] = 0;
		m_nFloorRequestDensity[DOWN] = 0;
		m_nStairNo = 0;
		m_pBuilding = NULL;
		
		for (int i = 0; i < NUM_PASSENGER; i++) {
	//		m_PassengerMatrix[UP][i] = NULL;
	//		m_PassengerMatrix[DOWN][i] = NULL;
			m_bFloorRequestServiced[UP] = false;		// request is not yet serviced 
			m_bFloorRequestServiced[DOWN] = false;		// request is not yet serviced 
		}

	}
	CHall::~CHall()
	{
		CPassenger* psngr = m_PsngrList.GetFirstObject();
		while (psngr)
		{
			m_PsngrList.RemoveHead();
			delete psngr;
			psngr = m_PsngrList.GetFirstObject();
		}
	}
	CHall::CHall(CBuilding *pContainer) : m_pBuilding(pContainer)
	{
		for (int i = 0; i < NUM_NODES; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				m_PDO[i][j] = 0;
			}
		}
		m_pICAN_Nod = NULL;
		m_nFloorRequestDensity[UP] = 0;
		m_nFloorRequestDensity[DOWN] = 0;
		m_nStairNo = 0;
		m_nMeIndex = NUM_CARS;
	
		for (int i = 0; i < NUM_PASSENGER; i++) {
	//		m_PassengerMatrix[UP][i] = NULL;
	//		m_PassengerMatrix[DOWN][i] = NULL;
			m_bFloorRequestServiced[UP] = false;		// request is not yet serviced 
			m_bFloorRequestServiced[DOWN] = false;		// request is not yet serviced 
		}


	}
	void CHall::SetStairNo(int nNo)
	{
		int i = 0;
		m_nStairNo = nNo;
		m_nMeIndex = m_nStairNo + NUM_CARS;
		// initialise PDO[]
		for (int i = 0; i < NUM_NODES; i++)
			for (int j = 0; j < 8; j++)
				m_PDO[i][j] = 0;

		// put its state into m_PDO[nCarNumber]
		// put its state into m_PDO[nCarNumber]
		CHallState hs;
		tCAN canstate;
		GetState(hs);					// from locales construct state
		hs.GetCanState(canstate);		// from state get canmassege
		setget_PDO(canstate);			// from canmessage update its own m_PDO
/*	
		if (nNo == 4)
		{
			for (i = 0; i< 7; i++) 
				IncreaseFloorRequestDensity(UP);

		}

	if (nNo == 8) {
			for (int j = 0; j < 15; j++)
				IncreaseFloorRequestDensity(UP);
		}
*/
		i = i;
	}

	CPassenger * CHall::GetUpPassenger(BYTE bStairNo)
	{
		CNode<CPassenger>* pIterator = m_PsngrList.GetFirst();
		CPassenger * pPsngr = NULL;
		bool bUp = false;
		if (pIterator)
			pPsngr = pIterator->pObj;
		else 
			return NULL;
	
		while (pIterator)
		{
			bUp = ((pPsngr->GetExit()) > bStairNo);			// up passenger
			if (bUp) 
			{
				m_PsngrList.Remove(pIterator);
				return pPsngr;
			}
			else
			{
				pIterator = m_PsngrList.GetNext();
				if (pIterator) 
					pPsngr = pIterator->pObj;
			}
			
		}
		return NULL;

	}
	CPassenger * CHall::GetDownPassenger(BYTE bStairNo)
	{
		CNode<CPassenger>* pIterator = m_PsngrList.GetFirst();
		CPassenger * pPsngr = NULL;
		bool bDown = false;
		if (pIterator)
			pPsngr = pIterator->pObj;
		else
			return NULL;

		while (pIterator)
		{
			bDown = (pPsngr->GetExit() < bStairNo);			// down passenger
			if (bDown) {

				m_PsngrList.Remove(pIterator);
				return pPsngr;
			}
			else
			{
				pIterator = m_PsngrList.GetNext();
				if (pIterator)
					pPsngr = pIterator->pObj;
			}

		}
		return NULL;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// bool sycronisePDO(tCAN& msg, bool download)
	// see implementation notes !
	// note 1 : if the class object is to be syncronised  to msg, msg must be of wright type !
	bool CHall::setget_PDO(tCAN& msg, bool get)
	{
		BYTE meIndex = msg.data[0] & 0x0F;
		// transmit PATH [0.0]
		if (!get)
		{
			for (int i = 0; i < 8; i++)
				msg.data[i] = m_PDO[meIndex][i];
		} // receive PATH [2.1]
		else
		{
			// if the class object is to be syncronised  to msg, msg must be of wright type !
			for (int i = 0; i < 8; i++)
				m_PDO[meIndex][i] = msg.data[i];
			if (!((msg.data[0] & 0xE0) == STT_BROADCAST))		// message is not from a Stair  W != 1
				return false;										// wrong message type

		}
		return true;

	}
	void CHall::IncreaseFloorRequestDensity(const BYTE dir)
	{
		static long id = 0;
	
		BYTE exit = 0;
		CPassenger * psngr = NULL;
		BYTE modulo = NUM_FLOORS -1 - m_nStairNo;
		if (modulo == 0) modulo = 1;

		BYTE vUp= rand() % modulo + m_nStairNo +1;
		BYTE vDown = rand() % ((m_nStairNo > 0 ) ? m_nStairNo : 1);


		if (m_nStairNo == exit)
			exit = exit;

		if (dir == UP)
			exit = vUp;
		else
			exit = vDown;
			
		if (m_nFloorRequestDensity[dir] < NUM_PASSENGER)
		{
			psngr = new CPassenger(++id, m_nStairNo, exit);				// create new passenger
			m_PsngrList.AddHead(*psngr);								// add to list
			m_nFloorRequestDensity[dir]++;
		}

		// add a passenger to passenger list
	}
	void CHall::DecreaseFloorRequestDensity(const BYTE dir)
	{
		if(m_nFloorRequestDensity[dir] > 0)
			m_nFloorRequestDensity[dir]--;
			
	}
	BYTE CHall::GetFloorRequestDensity(const BYTE dir)
	{
		return m_nFloorRequestDensity[dir];
	}
	void CHall::InsertNodeInterface(ICAN_Node** pNodeInterface)
	{
		m_pICAN_Nod = *pNodeInterface;
	}
	void CHall::GetNodeInterface(ICAN_Node** pNodeInterface)
	{
		*pNodeInterface = m_pICAN_Nod;
	}

	void CHall::Update(tCAN& canMsg)
	{
		tCAN* pcanMsg = &canMsg;
		if (m_pICAN_Nod)
		{
			m_pICAN_Nod->In(&pcanMsg);
		}
	}
	void CHall::GetState(CHallState& st)
	{
		st.SetFloorRequestDensity(UP, m_nFloorRequestDensity[UP]);
		st.SetFloorRequestDensity(DOWN, m_nFloorRequestDensity[DOWN]);
		st.SetHallNo(m_nStairNo);
		st.SetMeIndex(m_nMeIndex);
	}
	void CHall::sycroniseLOCALS()
	{
		bool bSet = true;
		CHallState hs;
		tCAN can_sState;
		tCAN* pcan_sState = &can_sState;
		GetState(hs);													// locales from objetc to state 
		hs.GetCanState(can_sState);										// from state to can-msg
		can_sState.id = CMS_ID_BLD;
		can_sState.length = 29;
		can_sState.rtr = 0;
		can_sState.data[0] = (STT_BROADCAST << 5) | 0x10 | m_nMeIndex;	// STT_BROADCAST , w = 1

		setget_PDO(can_sState, bSet);									// update own PDO
		m_pICAN_Nod->Out(&pcan_sState);									// via can bus to all other nodes. from each node to linked process object. PATH [1]
	}
	void CHall::CallBackFunction(void** ppObject, BYTE* f, BYTE size)
	{
		CHall * self = static_cast<CHall*>(*ppObject);
		self->execute(f, size);
	}
	void CHall::execute(BYTE* f, BYTE size)
	{
		/*
		cmd.id = CMS_ID_UPB;
		cmd.data[0] = (CMD_CLEAR_REQUEST << 5) | (0x00) | (0x0F & m_nCarNumber  );		// w = 0
		cmd.data[1] = targetNodId;
		cmd.data[2] = 1;		// number of passengers to be cleared from hall density list default 1
		cmd.data[3] = direction;

		//			BYTE 0		: node Id  [0-15] (low nibble)				    :	sending node id
		//			BYTE 0		: command  [0-15] (high nibble)				    :	sending node id
		//			BYTE 1		: who											:	the node id whic should execute the action
		//			BYTE 2		: floor number									:
		//			BYTE 3		: UP / DOWN  /BOTH								:
		*/

		BYTE cmd = (f[0] & 0xE0) >> 5;
		BYTE pasCount = f[2];
		BYTE selector = f[3];
		BYTE sendingNodId = (f[0] & 0x0F);

		// if I did send the message  then quit
		if (sendingNodId == (m_nMeIndex & 0x0F))
			return;
		////////////////////////////////////////////////////////////////////////////


		if ((cmd == CMD_CLEAR_REQUEST)				// clear request  ?
			&& (m_nMeIndex == f[1]))				// am I requested ?

		{
			/*
			for (BYTE b = 0; b < pasCount; b++)
			{ 
				if (selector == UP)
					DecreaseFloorRequestDensity(UP);
				else if (selector == DOWN)
					DecreaseFloorRequestDensity(DOWN);
				else if (selector == BOTH)
				{
					DecreaseFloorRequestDensity(UP);
					DecreaseFloorRequestDensity(DOWN);
				}
			}*/
		}

	
		
		else if (cmd == STT_BROADCAST)
		{
			for (int i = 0; i < 8; i++)
				m_PDO[sendingNodId][i] = f[i];
			//m_PDO[m_nMeIndex][i] = f[i];
		}
	}

}	// name space
