#include "stdafx.h"
#include "Lift.h"
#include "Building.h"
#include "RelativeRqst.h"
namespace ELLib
{
	CLift::~CLift(void)
	{
		int nHalt;
		nHalt = 1;
	}
	CLift::CLift() : CElevator(0)
	{
	}
	CLift::CLift(int nCarNumber) : CElevator(nCarNumber)
	{

		if (nCarNumber == 0)
		{
		/*	CPassenger * p1 = new CPassenger(110, 0, -1);
			CPassenger * p2 = new CPassenger(120, 4, 9);
			CPassenger * p3 = new CPassenger(130, 4, 10);
			CPassenger * p4 = new CPassenger(140, 4, 10);
			CPassenger * pDumy1 = 0;
			CPassenger * pDumy2 = 0;
			CPassenger * pDumy3 = 0;
			CPassenger * pDumy4 = 0;
		
			m_pPassengerPipe.In(*p1);
			m_pPassengerPipe.In(*p2);
			m_pPassengerPipe.In(*p3);
			m_pPassengerPipe.In(*p4);
			m_pPassengerPipe.Remove(*p2);

			pDumy1 = m_pPassengerPipe.Out();
			pDumy2 = m_pPassengerPipe.Out();
			pDumy3 = m_pPassengerPipe.Out();
			pDumy4 = m_pPassengerPipe.Out();
			pDumy4 = m_pPassengerPipe.Out();*/
		}
	}
	bool CLift::rank()
	{
		WORD wdumy, wD2;
	if (NUM_CARS == 1)
			return true;
		///////////////////// LOCALS /////////////////////////////////////////////////////////   
		/// HOLE DEN LOKALEN VON DER CAN-SCHNITTSTELLE ...
		BYTE			other_cabin_current_floor = 0;				// diger kabinin bulundugu kat
		direction		other_cabin_current_direction = STOP;			// diger kabinin yonu
		BYTE			other_cabin_target_max = INVALID;				// diger kabinin yukaridaki hedefi 
		BYTE			other_cabin_target_min = INVALID;				// diger kabinin asagidaki  hedefi 

		BYTE			j;			// auxlaries
		bool bSC_1 = false;			// sub case 1
		bool bSC_2 = false;			// sub case 2
		bool bSC_3 = false;			// sub case 3
		bool bSC_4 = false;			// sub case 4
		bool bSC_5 = false;			// sub case 5
		bool bSC_6 = false;			// sub case 6
		bool bSC_7 = false;			// sub case 5
		bool bSC_8 = false;			// sub case 6

		_UpdateFlorDensities();															// update m_bFloorRequest
		CRelativeRqst rqst(m_nFloorRequestDensity, m_bCarDestination, m_nFloor);		// relative request object for current lift

		if (rqst.m_bCD_UP || rqst.m_bCD_DOWN)
			return true;																// kabinden olan taleplerde rank 1
		// ///////////////////////////////////////////////////////////////////////////////////////////////////
		// ///////////////////////////////////////////////////////////////////////////////////////////////////
		//	Yukaridan - asagidan bir talep yoksa 
		//  Digerleri beni enterese etmiyor !!
		//


#ifdef _DEBUG
		if ((m_nCarNumber == 0))
			m_nFloor = m_nFloor;
#endif // _DEBUG


		if (!(
			rqst.DownReguestFromDOWN() != INVALID
			|| rqst.DownReguestFromUP() != INVALID
			|| rqst.UpReguestFromDOWN() != INVALID
			|| rqst.UpReguestFromUP() != INVALID
			))
		{
			return false;																	// rank 0
		}

		// her kabini kontorl et
		for (j = 0; j < NUM_CARS; j++)
		{
			if (j == 0 && m_nCarNumber == 1)
			{
				j = j;
			}

			//// C A R    N U M B E R //////////////////////
			if (j != m_nCarNumber)														// biz degilsek
			{
				//////////////////////////////////////////////////////////////////////////////////////////																						
				other_cabin_current_floor = (m_PDO[j][1] & (0x1F));						// first five bits
				other_cabin_current_direction = (direction)(m_PDO[j][1] >> 5);			// first tree bits
				wdumy = m_PDO[j][5];													// high
				wdumy = wdumy << 8;														// high
				wdumy = wdumy | m_PDO[j][4];											// low
				wD2 = wdumy;
				// determine cabin_targets
				WORD wM1 = 0x01;
				WORD wM2 = 0x8000;
				for (int v = 0; v < 16; v++) {
					if ((wD2 & wM1) == wM1) {
						other_cabin_target_min = v;
						break;
					}
					wD2 = wD2 >> 1;
				}

				wD2 = wdumy;
				for (int s = 15; s >= 0; s--) {
					if ((wD2 & wM2) == wM2) {
						other_cabin_target_max = s;
						break;
					}
					wD2 = wD2 << 1;
				}
				// ////////////////////////////////////////////////////////////////////////////////////////																						
				// no request
				// local logic-parameters initialization
				bSC_1 = false;			// sub case 1
				bSC_2 = false;			// sub case 2
				bSC_3 = false;			// sub case 3
				bSC_4 = false;			// sub case 4
				bSC_5 = false;			// sub case 5
				bSC_6 = false;			// sub case 6
				bSC_7 = false;			// sub case 7
				m_bShortTarget = false;
										//bSC_8 = false;			// sub case 8

				// case -A-
				if ( ((m_nDirection == STOP	) || (m_nDirection == DOWN) || (m_nDirection == UP))						// duragan   yada yukari  /////
					&& rqst.DownReguestFromUP() != INVALID) 								// yukaridan asagi yonlu talep var												
				{
					// rank is false 
					// IF any of the followings occurs
					bSC_1 = (other_cabin_current_floor >= rqst.DownReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.DownReguestFromUP()) <= (rqst.DownReguestFromUP() - m_nFloor))				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_2 = (other_cabin_current_floor >= rqst.DownReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.DownReguestFromUP()) < (rqst.DownReguestFromUP() - m_nFloor))					// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_3 = (other_cabin_current_floor >= rqst.DownReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.DownReguestFromUP()) <= (rqst.DownReguestFromUP() - m_nFloor))				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == DOWN);																			// asagi yönlü

																																			//-----------------------------------------------------------------//
					bSC_4 = (other_cabin_current_floor <= rqst.DownReguestFromUP())															// asagida
						&& (other_cabin_current_floor >= m_nFloor)																			// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan,
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_5 = (other_cabin_current_floor <= rqst.DownReguestFromUP())															// asagida
						&& (other_cabin_current_floor > m_nFloor)																	        // yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_6 = (other_cabin_current_floor <= rqst.DownReguestFromUP())															// asagida
						&& (other_cabin_current_floor >= m_nFloor)																			// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == UP)																			// yukari yonlu,
						&& ((other_cabin_target_max == INVALID) || (other_cabin_target_max <= rqst.DownReguestFromUP()));					// kisa hedefli
					m_bShortTarget = ((other_cabin_target_max == INVALID) || (other_cabin_target_max <= rqst.DownReguestFromUP()));			// short targeted ?
					if (bSC_1 || bSC_2 || bSC_3 || bSC_4 || bSC_5 || bSC_6)
					{
						return false;																										// rank 0
					}
				}
				/**/

				// case -B-
				if ( ((m_nDirection == STOP) || (m_nDirection == DOWN) || (m_nDirection == UP))												// duragan   yada asagi  /////
					&& rqst.DownReguestFromDOWN() != INVALID)		// asagıdan  asagi yonlu talep var	
				{
					// rank is false 
					// IF any of the followings occurs
					bSC_1 = (other_cabin_current_floor >= rqst.DownReguestFromDOWN())														// yukarida
						&& (other_cabin_current_floor - rqst.DownReguestFromDOWN()) <= (m_nFloor - rqst.DownReguestFromDOWN())				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_2 = (other_cabin_current_floor >= rqst.DownReguestFromDOWN())														// yukarida
						&& ((other_cabin_current_floor - rqst.DownReguestFromDOWN()) < (m_nFloor - rqst.DownReguestFromDOWN()))				// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_3 = (other_cabin_current_floor >= rqst.DownReguestFromDOWN())														// yukarida
						&& (other_cabin_current_floor - rqst.DownReguestFromDOWN()) <= (m_nFloor - rqst.DownReguestFromDOWN())				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == DOWN);																			// asagi yönlü

																																			//-----------------------------------------------------------------//
					bSC_4 = (other_cabin_current_floor <= rqst.DownReguestFromDOWN())														// asagida
						&& (rqst.DownReguestFromDOWN() - other_cabin_current_floor) <= (m_nFloor - rqst.DownReguestFromDOWN())				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan,
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_5 = (other_cabin_current_floor <= rqst.DownReguestFromDOWN())														// asagida
						&& (rqst.DownReguestFromDOWN() - other_cabin_current_floor) < (m_nFloor - rqst.DownReguestFromDOWN())				// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_6 = (other_cabin_current_floor <= rqst.DownReguestFromDOWN())														// asagida
						&& (rqst.DownReguestFromDOWN() - other_cabin_current_floor) <= (m_nFloor - rqst.DownReguestFromDOWN())				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == UP)																			// yukari yonlu,
						&& ((other_cabin_target_max == INVALID) || (other_cabin_target_max <= rqst.DownReguestFromDOWN()));				    // kisa hedefli

					m_bShortTarget = ((other_cabin_target_max == INVALID) || (other_cabin_target_max <= rqst.DownReguestFromDOWN()));		// short targeted ?
					if (bSC_1 || bSC_2 || bSC_3 || bSC_4 || bSC_5 || bSC_6)
					{
						return false;																										// rank 0
					}
				}


				// case -C-
				else if ( ((m_nDirection == STOP)  || (m_nDirection == DOWN) || (m_nDirection == UP) )										// duragan   yada yukari  /////
					&& rqst.UpReguestFromUP() != INVALID) 		// yukaridan yukari yonlu talep var												
				{
					// rank is false 
					// IF any of the followings occurs
					bSC_1 = (other_cabin_current_floor >= rqst.UpReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromUP()) <= (rqst.UpReguestFromUP() - m_nFloor))					// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin


					bSC_2 = (other_cabin_current_floor >= rqst.UpReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromUP()) < (rqst.UpReguestFromUP() - m_nFloor))						// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_3 = (other_cabin_current_floor >= rqst.UpReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromUP()) <= (rqst.UpReguestFromUP() - m_nFloor))					// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == DOWN);																			// asagi yönlü
																																			/**/

					bSC_4 = (other_cabin_current_floor >= rqst.UpReguestFromUP())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromUP()) <= (rqst.UpReguestFromUP() - m_nFloor))					// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == DOWN)																			// asagi yönlü
						&& ((other_cabin_target_max == INVALID) || (other_cabin_target_max >= rqst.UpReguestFromUP()));						// kisa hedefli

																																			//-----------------------------------------------------------------//
					bSC_5 = (other_cabin_current_floor <= rqst.UpReguestFromUP())															// asagida
						&& ((rqst.UpReguestFromUP() - other_cabin_current_floor) <= (rqst.UpReguestFromUP() - m_nFloor))					// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_6 = (other_cabin_current_floor <= rqst.UpReguestFromUP())															// asagida
						&& ((rqst.UpReguestFromUP() - other_cabin_current_floor) < (rqst.UpReguestFromUP() - m_nFloor))						// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_7 = (other_cabin_current_floor <= rqst.UpReguestFromUP())															// asagida
						&& ((rqst.UpReguestFromUP() - other_cabin_current_floor) <= (rqst.UpReguestFromUP() - m_nFloor))					// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == UP);																			// yukari yonlu

					m_bShortTarget = ((other_cabin_target_max == INVALID) || (other_cabin_target_max >= rqst.UpReguestFromUP()));			// short targeted ?

					if (bSC_1 || bSC_2 || bSC_3 || bSC_4 || bSC_5 || bSC_6 || bSC_7)

					{
						return false;																										// rank 0
					}
				}

				// case -D-
				else if ( ((m_nDirection == STOP) || (m_nDirection == DOWN) || (m_nDirection == UP))										// duragan   yada asagi  /////
					&& rqst.UpReguestFromDOWN() != INVALID ) 		// asagidan yukari yonlu talep var	
				{
					// rank is false 
					// IF any of the following occurs
					bSC_1 = (other_cabin_current_floor >= rqst.UpReguestFromDOWN())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromDOWN()) <= (m_nFloor - rqst.UpReguestFromDOWN()))				// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_2 = (other_cabin_current_floor >= rqst.UpReguestFromDOWN())															// yukarida
						&& ((other_cabin_current_floor < m_nFloor))																			// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_3 = (other_cabin_current_floor >= rqst.UpReguestFromDOWN())															// yukarida
						&& ((other_cabin_current_floor - rqst.UpReguestFromDOWN()) <= (m_nFloor - rqst.UpReguestFromUP()))					// eşit uzaklıkta veya yakın, 
						&& (other_cabin_current_direction == DOWN);																			// asagi yönlü

					bSC_4 = (other_cabin_current_floor >= rqst.UpReguestFromDOWN())															// yukarida
						&& (other_cabin_current_floor <= m_nFloor)																			// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == DOWN)																			// asagi yonlu
						&& ((other_cabin_target_max == INVALID) || (other_cabin_target_max >= rqst.UpReguestFromDOWN()));					// kisa hedefli

																																			//-----------------------------------------------------------------//

					bSC_5 = (other_cabin_current_floor <= rqst.UpReguestFromDOWN())															// asagida
						&& ((rqst.UpReguestFromDOWN() - other_cabin_current_floor) <= (m_nFloor - rqst.UpReguestFromDOWN()))				// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == STOP)																			// duragan
						&& (j < m_nCarNumber);																								// düşük nolu kabin

					bSC_6 = (other_cabin_current_floor <= rqst.UpReguestFromDOWN())															// asagida
						&& ((rqst.UpReguestFromDOWN() - other_cabin_current_floor) < (m_nFloor - rqst.UpReguestFromDOWN()))					// yakın, 
						&& (other_cabin_current_direction == STOP);																			// duragan

					bSC_7 = (other_cabin_current_floor <= rqst.UpReguestFromDOWN())															// asagida
						&& ((rqst.UpReguestFromDOWN() - other_cabin_current_floor) <= (m_nFloor - rqst.UpReguestFromDOWN()))				// esit uzaklikta veya yakın, 
						&& (other_cabin_current_direction == UP);																			// yukari yonlu

					m_bShortTarget = ((other_cabin_target_max == INVALID) || (other_cabin_target_max >= rqst.UpReguestFromDOWN()));
					if (bSC_1 || bSC_2 || bSC_3 || bSC_4 || bSC_5 || bSC_6 || bSC_7)

					{
						return false;																										// rank 0
					}
				}


			}
			//// C A R    N U M B E R //////////////////////
		}
		return true;
	}
} // name space

