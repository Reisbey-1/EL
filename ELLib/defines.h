#if !defined(AFX_DEFINES_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_)
#define AFX_DEFINES_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_
typedef unsigned char       BYTE;
typedef  void (*FP)(void** ppObject, BYTE* f, BYTE size);			// function pointer. 
																	// ppObject : object passed to function
																	// f		: first argument passed to function
																	// size		: second argument passed to function

//typedef  LONG(*fp) ();

enum direction	{ UP = 0, DOWN, STOP} ;								// yon ( sequence is important )
enum msc		{ BOTH = 2 };										// miscalleneous

enum canmsgID	{ CMS_ID_ELV= 0,CMS_ID_BLD,CMS_ID_UPB };			// can msg id: building state, elevator state, UPB
enum canmsgTYPE { CMS_TYPE_STATE = 0, CMS_TYPE_COMMAND = 4};		// can msg type: data, command
enum canmsgCMD  { CMD_CLEAR_REQUEST = CMS_TYPE_COMMAND,				// clear request command
				  CMD2,												// reserved
				  CMD3,												// reserved
				  CMD4};											// reserved

enum canmsgSTT {
	STT_BROADCAST = CMS_TYPE_STATE,									// state broadcast
	STT2,															// reserved
	STT3,															// reserved
	STT4
};																	

const BYTE LOAD_UNLOAD_TIME = 4;									// loading and unloading time (count of clock pulse)		// indirme - bindirme suresi (saat sinyali)
const BYTE SPACINGX			= 10;									// virtual horizontal distance between elevator cars		// kabinler arasi sanal bosluk
const BYTE SPACINGY			= 9;									// virtual hall height										// Bina yüksekligi
const BYTE BUF_LENGTH		= 80;									// yardimci karakter katari karakter uzunlugu 
const BYTE INVALID			= 255;									// invalid													// gecersiz 

const BYTE NUM_PASSENGER	= 20;									// maximum passenger per hall								// her bir katta olabilecek maksimum yolcu sayisi
const BYTE NUM_FLOORS		= 11;									// number of halls											// kat sayisi
const BYTE NUM_CARS = 4;									// number of elecator cars									// kabin sayisi
// NUM_NODES should not exced 16 !!!!!!!!!!!!!!!!
const BYTE NUM_NODES = NUM_CARS + NUM_FLOORS;						// number of can nodes										// kabin sayisi
const BYTE NUM_OF_PSNGR_PER_LIFT = 10;								// maximum passenger per elevator car						// kabinin alabilecegi maximum yolcu sayisi
#endif // !defined(AFX_DEFINES_H__B3AAD762_2D7C_4683_827E_059EDFD22BEA__INCLUDED_)
