//////////////////////////////////////////////////////////////////////
//                                                                  //
// Elevator.h: Schnittstelle für die Klasse CElevator.				//
//                                                                  //
// Author : Dr. Mustafa Dülger                                      //
//                                                                  //
// 1 Eger asansor saftin alt tarafina veya tavana carpmak           //
//   uzere ise dur.                                                 //
//                                                                  //
// 2 Eger bu gidilecek kat ise, yolculari indir.                    //
//                                                                  //
// 3 Eger asansor bu katta iken yukaridaki katlardan talep varsa    //
//   ve biz yukari gidiyorsak, yolculari  bindir.                   //
//                                                                  //
// 4 Eger asansor bu katta iken asagidaki katlardan talep varsa     //
//   ve biz asagi gidiyorsak, yolculari bindir.                     //
//                                                                  //
// 5 Yukariki ve asagiki katlardan talep yoksa ve gidilecek kat     //
//   belirtilmemisse, dur.                                          // 
//                                                                  //
// 6 Yukaridaki katlarda inecek varsa, yukari cik.                  //
//                                                                  //
// 7 Asagimizdaki katlarda inecek varsa, asagi in                   //
//                                                                  //
// 8 Eger duruyorsak veya yukari cikiyorsak ve yukarimizdaki        //
//   katlardan talep varsa ve bizle talepte bulunan kat arasinda    //
//   yukari cikan baska kabin yoksa veya talepte bulunan katin      //
//   ustunde, asagiya dogru inen ve o kata bizden daha yakin olan   //
//   baska bir kabin yoksa, yukari cik.                             //
//                                                                  //
// 9 Eger duruyorsak veya asagi iniyorsak ve asagimizdaki katlardan //
//   talep varsa ve bizle talepte bulunan kat arasinda asagi inen   //
//   baska bir kabin yoksa veya talepte bulunan katin altinda,      //
//   yukari dogru cikan ve o kata bizden daha yakin olan baska bir  //
//   kabin yoksa, asagi in.                                         //
//                                                                  //
//10 Diger kurallar gecerli degilse, dur.                           //
//                                                                  //
//////////////////////////////////////////////////////////////////////
///
///  m_bCarDestination[NUM_FLOORS]
///
/// 'x' : true
/// ' ' : false
///
///   0    1    2     3    4    5    6    7    8    9    10   
/// ---------------------------------------------------------       
///  | x  |    |    |    |    |    |    |    |    | x   |    |      
/// ---------------------------------------------------------       
///
#if !defined(AFX_ELEVATOR_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_)
#define AFX_ELEVATOR_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_
#include <iostream>
#include <fstream>
using namespace std;

//#include "msoftcon.h"
#include "defines.h"
#include "ElevatorState.h"
#include "Passenger.h"
namespace ELLib
{
	
	typedef  void(*pCBfunc_elevator) (void** ppObject, BYTE * msg, BYTE size);

	class CBuilding ;									// ileriye yonelik deklarasyon
	class ELLIB_API CElevator
	{
	protected:
	BYTE				m_nCarNumber;							// kabin numarasi
	BYTE				m_nFloor;								// neredeyiz
	direction			m_nDirection;							// hangi yone gidecegiz (yada duracagiz)
	direction			m_nDirectionHistory;					// hangi yone gidecegiz (yada duracagiz)
	bool				m_bCarDestination[NUM_FLOORS];			// asansore binenler tarafindan secilir istek katlari	
	BYTE				m_nLoadingTimer;						// binen varsa yukleme suresi
	BYTE				m_nUnloadingTimer;						// inen varsa indirme suresi
	bool				m_bRank;	
	int 				m_nModulus;
	bool				m_bCd_Serviced;							// passenger have got off the elevator
	bool				m_bFr_Serviced;							// passenger have got on the elevator
	L_List<CPassenger>   m_pPassengerList;						// passenger list
	bool				m_bShortTarget;							// short target 
	CBuilding*			m_pBuilding;							//
	// CAN PDO
	BYTE				m_PDO[NUM_CARS + NUM_FLOORS][8];		// update through can interface
	bool				m_bFloorRequest[2][NUM_FLOORS];			// floor reequest update through PDO	
	BYTE				m_nFloorRequestDensity[2][NUM_FLOORS];	// floor request density (not included in state varibale)

public:			
	CElevator() ;										// standar contructor
	CElevator(BYTE nCarNumber);	// kurucu fonksiyon
	virtual ~CElevator();

	void	SetCarNumber(BYTE nVal);
	void	SetFloor(BYTE nVal);
	void	SetCarDestination(int destination, bool bVal);
	bool	GetCaDestination(int destination);

	void	car_tick1();																// her kabin icin 1. saat sinyali
	void	car_tick2();																// her kabin icin 2. saat sinyali

	void    ESM(BYTE st_rqst_dns[2][NUM_FLOORS], bool car_dst[NUM_FLOORS], bool rnk);	// elevavator state machine
	void    PSM();																		// process state machine
	virtual bool rank();																// rank function

	BYTE		get_floor() const{return m_nFloor ;}									// su anki kati ogren
	direction	get_car_direction() const 		{return m_nDirection;}					// su anki yonu ogren
	BYTE		get_car_number() const			{ return m_nCarNumber ;}				// kabin numarasi
	void		set_car_direction(direction dr);										// 
	void		set_building(CBuilding* pBuilding) { m_pBuilding = pBuilding; }			// set building
	void		GetState(CElevatorState& st);

	void		sycroniseLOCALS();											// sencronise locals to object PDO[] -> node PDO[] -> (via can bus other) node PDO[] -> other object PDO[]
	bool		setget_PDO(tCAN& msg, bool set = true);						// (via can bus other) node PDO[] -> other object PDO[]

	void dump();	
	bool Is_Cd_Serviced() { return m_bCd_Serviced; }
	bool Is_Fr_Serviced() { return m_bFr_Serviced; }
	bool Is_Exact_On_Floor();
	BYTE canTakePas();

	// can interface
	void InsertNodeInterface(ICAN_Node** pNodeInterface);
	void GetNodeInterface(ICAN_Node** pNodeInterface);

	static void CallBackFunction(void** ppObject, BYTE* f, BYTE size);
protected:
	void	move();															// kabini harekete gecir
	
	ICAN_Node *m_pICAN_Nod;

	void execute(BYTE* f, BYTE size);
	void _UpdateFlorDensities();

	void get_passenger_off();												// yolcu indir
	void get_passenger_on();												// yolcu bindir

	};
}   // namespace
#endif // !defined(AFX_ELEVATOR_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_)
