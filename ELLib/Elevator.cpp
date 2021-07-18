// Elevator.cpp: Implementierung der Klasse CElevator.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Elevator.h"
#include "Building.h"
#include "RelativeRqst.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
namespace ELLib
{
	//int r = SPACINGY / NUM_FLOORS - 1;
	int r = SPACINGY ;
	static int mSt = 55;
	CElevator::CElevator() : CElevator(0)
	{

	}
	CElevator::CElevator(BYTE nCarNumber) : m_nCarNumber(nCarNumber)
	{
		// can interface
		m_pICAN_Nod = NULL;

		m_nFloor = 0;								// 0 dan basla (kullanici 1)
		m_nModulus = 0;
		m_nDirection = STOP;						// baslangicta hareketsiz
		m_nDirectionHistory = UP;					// baslangic gecmisi STOP dan farki olmali !!!! (niye ?)

		for (BYTE i = 0; i < NUM_FLOORS; i++)
		{
			m_bCarDestination[i] = false;			// asansore binenler hic bir butona basmadi
			m_nFloorRequestDensity[UP][i] = 0;		// asansor  yogunluk 0
			m_nFloorRequestDensity[DOWN][i] = 0;	// asansor  yogunluk 0
		}

		m_nLoadingTimer = 0;						// henuz binen yok
		m_nUnloadingTimer = 0;						// henuz inen yok
		m_bRank = false;
		m_bCd_Serviced = false;						// not yet serviced
		m_bFr_Serviced = false;
		m_bShortTarget = false;						// not yet short targeted

		// initialise PDO[]
		for (int i = 0; i < NUM_NODES; i++)
			for (int j = 0; j < 8; j++)
				m_PDO[i][j] = 0;

		// put its state into m_PDO[nCarNumber]
		CElevatorState es;
		tCAN canstate;
		GetState(es);					// from locales construct state
		es.GetCanState(canstate);		// from state get canmassege
		setget_PDO(canstate);			// from canmessage update its own m_PDO
	}
	CElevator::~CElevator()
	{
		//m_dumpfile.close();
		int n;
		n = 0;
	}
	void CElevator::SetCarNumber(BYTE nVal)
	{
		m_nCarNumber = nVal;
	}

	void CElevator::SetFloor(BYTE nVal) {
		m_nFloor = nVal;
		m_nModulus = m_nFloor * r;
	}

	void CElevator::SetCarDestination(int destination, bool bVal)
	{
		BYTE dstl = m_PDO[m_nCarNumber][4];
		BYTE dsth = m_PDO[m_nCarNumber][5];
		WORD wDst = 0x0001 << destination;

		m_bCarDestination[destination] = bVal;

		if (bVal)
		{
			m_PDO[m_nCarNumber][4] = (BYTE)wDst | m_PDO[m_nCarNumber][4];
			m_PDO[m_nCarNumber][5] = (BYTE)(wDst >> 8) | m_PDO[m_nCarNumber][5];
		}
		else
		{
			m_PDO[m_nCarNumber][4] = (BYTE)(~(wDst))		& m_PDO[m_nCarNumber][4];
			m_PDO[m_nCarNumber][4] = (BYTE)(~(wDst >> 8))	& m_PDO[m_nCarNumber][4];
		}


		//	if (m_nCarNumber == 0 && destination == 2
		//		nhalt = 1;


	}
	bool CElevator::GetCaDestination(int destination)
	{
		return m_bCarDestination[destination];
	}
	void CElevator::CallBackFunction(void** ppObject, BYTE* f, BYTE size)
	{
		//Beep(1000, 1000); 
		//MessageBox(NULL, L" -- CallBackFunction(void** ppObject, BYTE* f, BYTE size) -- not yet impemented !! ",L"CALLBACK",0);
		CElevator * self = static_cast<CElevator*>(*ppObject);
		self->execute(f, size);
	}
	void CElevator::InsertNodeInterface(ICAN_Node** pNodeInterface)
	{
		m_pICAN_Nod = *pNodeInterface;
	}
	void CElevator::GetNodeInterface(ICAN_Node** pNodeInterface)
	{
		*pNodeInterface = m_pICAN_Nod;
	}

	////////////////////////////////////////////DRAWING//////////////////////////////////////////////////////
	// gidilecek kartlari ogren
	void CElevator::execute(BYTE* f, BYTE size)
	{
		/*
		//			BYTE 0		: node Id  [0-15] (low nibble)				    :	sending node id
		//			BYTE 0		: command  [0-15] (high nibble)				    :	sending node id
		//			BYTE 1		: who											:	the node id whic should execute the action
		//			BYTE 2		: floor number									:
		//			BYTE 3		: UP / DOWN  /BOTH								:
		*/
		BYTE cmd = (f[0] & 0xE0) >> 5;
		BYTE floor = f[2];
		BYTE selector = f[3];
		BYTE sendingNodId = (f[0] & 0x0F);

		// if I did send the message  then quit
		if (sendingNodId == (m_nCarNumber & 0x0F))
			return;
		////////////////////////////////////////////////////////////////////////////


		if (cmd == CMD_CLEAR_REQUEST)
		{
			if (selector == UP)
				m_bFloorRequest[UP][floor] = false;
			else if (selector == DOWN)
				m_bFloorRequest[DOWN][floor] = false;
			else if (selector == BOTH)
			{
				m_bFloorRequest[UP][floor] = false;
				m_bFloorRequest[DOWN][floor] = false;
			}
		}

		else if (cmd == STT_BROADCAST)
		{
			for (int i = 0; i < 8; i++)
				m_PDO[sendingNodId][i] = f[i];
		}
	}
	void CElevator::dump()
	{
		char buffer[250];
		sprintf_s(buffer, "%d\t\t%d\t\t%d\n", m_nCarNumber, m_nFloor, m_nDirection);
		//m_dumpfile << buffer;
	}
	// kabin hedefi  
	bool CElevator::setget_PDO(tCAN& msg, bool set)
	{
		BYTE ndId = msg.data[0] & 0x0F;
		if (set)
		{
			// if the class object is to be syncronised  to msg, msg must be of wright type !
			if (!((msg.data[0] & 0xE0) == STT_BROADCAST))			// 1110 0000 = 0xE0
				return false;										// wrong message type
			else
				for (int i = 0; i < 8; i++)
					m_PDO[ndId][i] = msg.data[i];
		}
		else
		{
			for (int i = 0; i < 8; i++)
				msg.data[i] = m_PDO[ndId][i];
		}
		return true;
	}
	void CElevator::sycroniseLOCALS()
	{
		bool bset = true;
		CElevatorState es;
		tCAN can_sState;
		tCAN* pcan_sState = &can_sState;
		GetState(es);														// from objetc to locale state 
		es.GetCanState(can_sState);											// from state to can-msg
	//	can_sState.id = CMS_ID_ELV;
	//	can_sState.data[0] = 0x00 | m_nCarNumber;							// CMS_ID_ELV , w = 0

		setget_PDO(can_sState, bset);									    // Update PDO 
		if (m_pICAN_Nod)
		{
			m_pICAN_Nod->Out(&pcan_sState);									// Update PDO  throgh can bus
		}

	}
	//////////////////////////////////SERVICE////////////////////////////////////////////////////////////////
	// her kabin icin 1. saat sinyali
	void CElevator::car_tick1()
	{
		if (m_nLoadingTimer)				// yukleme zamanini say
		{
			--m_nLoadingTimer;				// bir eksilt
			if (m_nLoadingTimer == 0)		// kapi kapandi, servis tamamlandi
				m_bFr_Serviced = true;
		}

		// indirme zamanini say
		if (m_nUnloadingTimer)
		{
			--m_nUnloadingTimer;			// bir eksilt
			if (m_nUnloadingTimer == 0)		// kapi kapandi, servis tamamlandi
				m_bCd_Serviced = true;
			//return;
		}

		if ((m_nUnloadingTimer > 0) || (m_nLoadingTimer > 0))	// yolcu indir bindir varsa processi yurutme
			return;
		PSM();								// ne yapilacagina karar ver (density version) process state machine
	}
	// her kabin icin 2. saat sinyali
	void CElevator::car_tick2()
	{
		move();				// musaitse asansoru harekete gecir
	}
	BYTE CElevator::canTakePas()
	{
		return (BYTE)(NUM_OF_PSNGR_PER_LIFT - m_pPassengerList.GetSize());
	}

	// kabini harekete gecir
	void CElevator::move()
	{

		// inen veya binen varsa hareket etme
		if (m_nLoadingTimer || m_nUnloadingTimer)
		{
			set_car_direction(STOP);
			return;
		}

		// yukari gidiyorsa yukari git
		if (m_nDirection == UP) {
			set_car_direction(UP);
			m_nModulus++;
			if (m_nModulus - (m_nModulus / r) * r == 0)
			{
				m_nFloor++;
				if (m_nFloor > NUM_FLOORS - 1)
					m_nFloor--;
			}
		}
		// asagi gidiyorsa asagi gi
		else if (m_nDirection == DOWN) {
			set_car_direction(DOWN);
			m_nModulus--;
			if (m_nModulus - (m_nModulus / r) * r == 0)
			{
				m_nFloor--;
				if (m_nFloor < 0)
					m_nFloor++;

			}
		}
	}
	void CElevator::GetState(CElevatorState& st)
	{
		st.SetMe(m_nCarNumber);
		st.SetFloor(m_nFloor);
		st.SetDirection(m_nDirection);
		st.SetDirectionHistory(m_nDirectionHistory);
		st.SetModulus(m_nModulus);
		st.SetUnloadingTimer(m_nUnloadingTimer);
		st.SetLoadingTimer(m_nLoadingTimer);
		for (int i = 0; i < NUM_FLOORS; i++)
		{
			st.m_bCarDestination[i] = m_bCarDestination[i];
		}

	}

	void CElevator::get_passenger_off()
	{
		m_nUnloadingTimer = LOAD_UNLOAD_TIME;
		m_bCarDestination[m_nFloor] = false;			    // clear cd

		CNode<CPassenger>* nodeDumy = m_pPassengerList.GetFirst();
		if (nodeDumy == NULL)
			return;
		CPassenger* pPassenger = NULL;
		while (nodeDumy)
		{
			pPassenger = nodeDumy->pObj;
			if (pPassenger == NULL)
			{
				nodeDumy = m_pPassengerList.GetNext();
				continue;
			}

			else if (pPassenger->GetExit() == m_nFloor)
			{

				m_pPassengerList.Remove(nodeDumy);
				if (pPassenger)
					delete pPassenger;// remove *pNode from list 
				nodeDumy = m_pPassengerList.GetFirst();

			}
			else
				nodeDumy = m_pPassengerList.GetNext();
		}

	}
	void CElevator::get_passenger_on()
	{
		int nHalt = 0;
		tCAN msg;
		msg.id = CMS_ID_UPB;												// message id
		BYTE target = m_nFloor + NUM_CARS;									// target index which should reduce passenger density.								
		CPassenger * pPsngr = NULL; // m_pBuilding->GetStairAt(m_nFloor)->GetUpPassenger();

		BYTE pasTakenIn = canTakePas();
		BYTE dir_H_L = BOTH;

		CElevatorState es;													// get current state
		GetState(es);

		if (m_nCarNumber == 0 && m_nFloor == 8)
			nHalt = 0;
		dir_H_L = es.GetDirectionHistory();

		if (dir_H_L == UP)
		{
			//		pasDensity = m_PDO[target][1];									// passenger density up   // floor request density UP

			for (BYTE nt = 0; nt < pasTakenIn; nt++)
			{
				pPsngr = NULL;
				pPsngr = m_pBuilding->GetStairAt(m_nFloor)->GetUpPassenger(m_nFloor);
				if (pPsngr)
				{
					m_pPassengerList.AddHead(*pPsngr);						// add passenger to elevator list
					m_bCarDestination[pPsngr->GetExit()] = true;			// press cabine button
					m_pBuilding->GetStairAt(m_nFloor)->DecreaseFloorRequestDensity(UP);
					m_bShortTarget = false;									// do not take down passenger if up passenger exist !!
				}
				else if (pPsngr == NULL && m_bShortTarget)
				{
					pPsngr = m_pBuilding->GetStairAt(m_nFloor)->GetDownPassenger(m_nFloor);
					if (pPsngr)
					{

						m_pPassengerList.AddHead(*pPsngr);						// add passenger to elevator list
						m_bCarDestination[pPsngr->GetExit()] = true;			// press cabine button
						m_pBuilding->GetStairAt(m_nFloor)->DecreaseFloorRequestDensity(DOWN);
					}
					//	break;
				}
				else
					break;

			}


		}
		else if (dir_H_L == DOWN)
		{

			for (BYTE nt = 0; nt < pasTakenIn; nt++)
			{
				pPsngr = NULL;
				pPsngr = m_pBuilding->GetStairAt(m_nFloor)->GetDownPassenger(m_nFloor);
				if (pPsngr)
				{
					m_pPassengerList.AddHead(*pPsngr);						// add passenger to elevator list
					m_bCarDestination[pPsngr->GetExit()] = true;			// press cabine button
					m_pBuilding->GetStairAt(m_nFloor)->DecreaseFloorRequestDensity(DOWN);
					m_bShortTarget = false;									// do not take up passenger if down passenger exist !!
				}
				else if (pPsngr == NULL && m_bShortTarget)
				{
					pPsngr = m_pBuilding->GetStairAt(m_nFloor)->GetUpPassenger(m_nFloor);
					if (pPsngr)
					{
						m_pPassengerList.AddHead(*pPsngr);						// add passenger to elevator list
						m_bCarDestination[pPsngr->GetExit()] = true;			// press cabine button
						m_pBuilding->GetStairAt(m_nFloor)->DecreaseFloorRequestDensity(UP);
					}
					//break;
				}
				else
					break;

			}


		}

		//if (false == es.GetCanCommand(msg, CMD_CLEAR_REQUEST, target, BOTH, pasTakenIn))	// produce request to clear current floor demand
	//	if (false == es.GetCanCommand(msg, CMD_CLEAR_REQUEST, target, dir_H_L, pasTakenIn))	// produce request to clear current floor demand
	//		return;


	//	ICAN_Bus* pICAN_Bus = NULL;											// sent request	
	//	m_pICAN_Nod->get_CanBus((IUnknown**)& pICAN_Bus);
	//	pICAN_Bus->RcvMsg(&msg);

		// get building node state-msg over current node-interface
		///	m_pBuilding->set_floor_req(UP, m_nFloor, false);		// clear fr UP
		///	m_pBuilding->set_floor_req(DOWN, m_nFloor, false);		// clear fr DOWN
		m_nLoadingTimer = LOAD_UNLOAD_TIME;


		//	CPassenger * pPsngr = m_pBuilding->GetStairAt(m_nFloor)->GetUpPassenger();
		//	m_pBuilding->GetStairAt(1)->GetFloorRequestDensity(UP);


		//	if (m_nFloor != 5)
		//		m_bCarDestination[5] = true;
		//	if (m_nFloor != 9)
		//		m_bCarDestination[9] = true;


	}
	void CElevator::set_car_direction(direction dr)
	{
		if ((dr != STOP) && (m_nDirection != STOP))  // if not stop, update history
													 // if (dr != STOP)  // if not stop, update history
			m_nDirectionHistory = m_nDirection;
		m_nDirection = dr;
	}
	// update member matrixes: 
	//		BYTE m_nFloorRequestDensity [2][NUM_FLOORS]
	//		bool m_bFloorRequest		[2][NUM_FLOORS] 
	//	by enumerating m_PDO object
	void CElevator::_UpdateFlorDensities()
	{
		BYTE nUpDensity = 0;
		BYTE nDwnDensity = 0;

		// enumerates over m_PDO object
		for (int i = 0; i < NUM_FLOORS; i++)
		{
			nUpDensity = m_PDO[NUM_CARS + i][1];
			nDwnDensity = m_PDO[NUM_CARS + i][2];
			m_nFloorRequestDensity[UP][i] = nUpDensity;
			m_nFloorRequestDensity[DOWN][i] = nDwnDensity;

			m_bFloorRequest[UP][i] = false;
			m_bFloorRequest[DOWN][i] = false;
			if (nUpDensity)
				m_bFloorRequest[UP][i] = true;
			if (nDwnDensity)
				m_bFloorRequest[DOWN][i] = true;
		}
	}
	bool CElevator::Is_Exact_On_Floor()
	{
		// only decide when i am exactly on stair
	//	if ((m_nModulus - (m_nModulus / r) * r == 0))
		return (m_nModulus - (m_nModulus / r) * r == 0);

	}

	bool CElevator::rank() 
	{ 
		return true; 										// rank function
	}
	// ne yapilacagina karar ver
	void CElevator::PSM()
	{
		bool bRnk = false;

		_UpdateFlorDensities();			// floor densities are updated over PDO's
		bRnk = rank();					// calculate rank
		ESM(m_nFloorRequestDensity, m_bCarDestination, bRnk);
	}

	/*
	strqstdns	:	stair request density (not included in state varibale)
	cardst		:	asansore binenler tarafindan secilir istek katlari
	*/
	void CElevator::ESM(BYTE st_rqst_dns[2][NUM_FLOORS], bool car_dst[NUM_FLOORS], bool rnk)
	{
#ifdef _DEBUG
		//if ((m_nCarNumber == 0) && (m_bCarDestination[10] == true) && (m_nFloor == 0))
		if ((m_nCarNumber == 0) && (m_nFloor == 10) && Is_Exact_On_Floor() && m_nFloorRequestDensity[DOWN][10])
			m_nFloor = m_nFloor;
#endif // _DEBUG	


		// only decide when i am exactly on stair
		if (!Is_Exact_On_Floor()  )
				return;
				// SAFETY
		if (
			((m_nFloor == NUM_FLOORS - 1) && (m_nDirection == UP))		// up stairs safety
			|| ((m_nFloor == 0) && (m_nDirection == DOWN))					// down stairs safety
			)
		{
			set_car_direction(STOP);
			return;

		}
		CRelativeRqst   rqst(st_rqst_dns, car_dst, m_nFloor);											// request object 
		bool bUp_Cnd = false;																			// up condition
		bool bDw_Cnd = false;																			// down condition

		// up condition
		//
		bUp_Cnd = (( (rqst.npd_from_up() != INVALID)  || rqst.m_bSR_UP_Stair) && rnk)					// ( rank() AND(there is nearest passenger density from up) OR there is up stair-request from current stair ) AND rank 
			|| (rqst.m_bCD_UP && (m_nDirectionHistory == UP))											// (there is up request from car) AND (car is stopped while driving up)														//yukari giderken durmus ise  ve kabinden yukariya istek var ise
			|| ((!rqst.m_bCD_DOWN && (m_nDirectionHistory == DOWN) && (rqst.m_bCD_UP))					// (there is no down request from car) AND (car is stoped while driving down) AND (there is up request from car )			//asagi giderken durmus ise ve kabinden asagiya istek yok ise ve kabinden yukariya istek var ise
				);

		

		// down condition 
		//
		bDw_Cnd = (((rqst.npd_from_down() != INVALID) || rqst.m_bSR_DOWN_Stair) && rnk)					// (there is nearest passenger density from down OR there is down stair-request from current stair ) AND rank 
			|| (rqst.m_bCD_DOWN && (m_nDirectionHistory == DOWN)										// (there is down request from car) AND (car is stopped while driving down)													// asagi giderken durmus ise  ve kabinden asagiya istek var ise
			|| (!rqst.m_bCD_UP && (m_nDirectionHistory == UP) && (rqst.m_bCD_DOWN))						// (there is no up request from car) AND (car is stoped while driving up) AND (there is down request from car )				// yukariya giderken durmus ise ve kabinden yukariya istek yok ise ve kabinden asagiya istek var ise
				);

		////////////////////////////////////////////////////
		// STOP STATE
		if (get_car_direction() == STOP)
		{

			// services 
			// yolcu indirme servisi
			if (rqst.m_bCD_STAIR									//	   (cd == cf)
				&& !Is_Cd_Serviced())								// && !(cd_serviced()		
			{
				get_passenger_off();								// let passenger get off
				m_bCd_Serviced = true;								// set service cd
			}

			// yolcu bindirme servisi
			//
			if ((rqst.m_bSR_UP_Stair || rqst.m_bSR_DOWN_Stair)		//		(fr == cf)
				&& !Is_Fr_Serviced()								//	&&	fr_serviced
				&& canTakePas() > 0 								//  can take passenger
				&& rnk)
			{
				get_passenger_on();									// let passenger get on
				m_bFr_Serviced = true;								// set service fr
			}

			// history state is up
			//
			if (m_nDirectionHistory == UP)
			{
				if (bUp_Cnd)										// up condition
				{
					set_car_direction(UP);							// change stop state to up 
					m_bCd_Serviced = false;							// clear cd service
					m_bFr_Serviced = false;							// clear fr service
				}
				else if (bDw_Cnd)									// down condition
				{
					set_car_direction(DOWN);						// change stop state to down 
					m_bCd_Serviced = false;							// clear cd service
					m_bFr_Serviced = false;							// clear fr service
				}
				else
					;												// do nothing, stay further in stop state;

			}

			// history state is down
			//
			if (m_nDirectionHistory == DOWN)
			{
				if (bDw_Cnd)										// down condition
				{
					set_car_direction(DOWN);						// change stop state to down
					m_bCd_Serviced = false;							// clear cd service;
					m_bFr_Serviced = false;							// clear fr service
				}
				else if (bUp_Cnd) {
					set_car_direction(UP);							// change stop state to up
					m_bCd_Serviced = false;							// clear cd service;
					m_bFr_Serviced = false;							// clear fr service
				}
				else
					;												// do nothing, stay further in stop state;

			}

		}

		////////////////////////////////////////////////////
		// UP STATE
		else if (get_car_direction() == UP)
		{
			//  kabinden bulunulan kata istek varsa
			//	bulunulan kattan yukari istek varsa 
			//  bulunulan kattan asagi istek varsa VE FAKAT kabinden yukari istek yoksa
			if ( // stop condition while driving up
				rqst.m_bCD_STAIR										// there is car-request to current floor
				|| rqst.m_bSR_UP_Stair										// there is up stair-request from current stair 
				|| (rqst.m_bSR_DOWN_Stair && !rqst.m_bCD_UP)				// there is down stair-request from current stair AND there is no up car-request 
				)
			{
				set_car_direction(STOP);		// dur !
				m_bFr_Serviced = false;			// servisleri sil
				m_bCd_Serviced = false;
			}
			else
				set_car_direction(UP);			// yukari cikmaya devam et
		}

		////////////////////////////////////////////////////
		// DOWN STATE
		else if (get_car_direction() == DOWN)
		{
			//	kabinden bulunulan kata istek varsa
			//	bulunulan kattan asagi istek varsa
			//	bulunulan kattan yukari istek varsa VE FAKAT kabinden asagi istek yoksa
			if (rqst.m_bCD_STAIR || rqst.m_bSR_DOWN_Stair || (rqst.m_bSR_UP_Stair && !rqst.m_bCD_DOWN))
			{
				set_car_direction(STOP);		// dur !
				m_bFr_Serviced = false;			// servisleri sil
				m_bCd_Serviced = false;

			}
			else
			{
				set_car_direction(DOWN);		// asagi inmeye devam et
			}
		}

	}

}// name space