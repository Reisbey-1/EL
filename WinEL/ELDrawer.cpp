// ELDrawer.cpp : implementation file
//

#include "stdafx.h"
#include "WinEL.h"
#include "ELDrawer.h"


// CELDrawer
IMPLEMENT_SERIAL(CELDrawer, CObject, 2)
CELDrawer* CELDrawer::m_pelDrawer = NULL;

CELDrawer::CELDrawer()
{
	m_bkcolor	= RGB(250, 170, 170);
	m_gridcolor = RGB(0, 0, 0);
	m_stXScale	= 5;			// grid scale  X
	m_stYScale	= 5;			// grid scale  Y

	m_pointOrigin.x = 0;
	m_pointOrigin.y = 0;
	m_RBottom.SetX(40);
	m_RBottom.SetY(140);

	m_GridPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));				// Blau);
	m_DemandPen_Current.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));		// blau);
	m_DemandPen_Active.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));		// rot);
	m_HghLghtPen.CreatePen(PS_SOLID, 1, RGB(255, 0,0));			// blau);


	//m_DemandPen_Passive.CreatePen(PS_SOLID, 1, RGB(209, 209, 209)); // gray);
	m_DemandPen_Passive.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));		// shwarz);

	//m_LiftPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));				// rot);
	m_LiftPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));				// blau);
	m_FloorPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));				// green);
	m_DoorPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));				// green);


//	CPen * pOldPen = DC.SelectObject(&m_GridPen);
}

CELDrawer::~CELDrawer()
{
	m_GridPen.Detach();
	m_GridPen.DeleteObject();

	m_DemandPen_Active.Detach();
	m_DemandPen_Active.DeleteObject(); // get rid of old pen

	m_DemandPen_Passive.Detach();
	m_DemandPen_Passive.DeleteObject(); // get rid of old pen
	
	m_LiftPen.Detach();
	m_LiftPen.DeleteObject(); // get rid of old pen

	m_FloorPen.Detach();
	m_FloorPen.DeleteObject(); // get rid of old pen
	
	m_DoorPen.Detach();
	m_DoorPen.DeleteObject(); // get rid of old pen
}

// singleton
CELDrawer* CELDrawer::GetDrawer()
{
	if (m_pelDrawer == NULL)
		m_pelDrawer = new CELDrawer;

	return m_pelDrawer;
}
// singleton
void CELDrawer::Kill()
{
	delete m_pelDrawer;
	m_pelDrawer = NULL;
}

void CELDrawer::GetCellRect(RECT& rc, CCell cell)
{
	rc.top = (cell.GetY())   * m_stYScale + 1 - m_pointOrigin.y;
	rc.left = (cell.GetX())   * m_stXScale + 1 - m_pointOrigin.x;
	rc.bottom = (cell.GetY() + 1) * m_stYScale - m_pointOrigin.y;
	rc.right = (cell.GetX() + 1) * m_stXScale - m_pointOrigin.x;;
}

int CELDrawer::GetStair(CPoint point)
{
	return 10 - (point.y / m_stYScale) + 5;

}

void CELDrawer::DrawBackColor(CPaintDC &DC)
{
	RECT rc;
	DC.GetClipBox(&rc);
	//rc.left -= m_pointOrigin.x;
	//rc.top   -= m_pointOrigin.y;
	if (m_BackGroundBrush.m_hObject != NULL) {
		m_BackGroundBrush.DeleteObject();        // get rid of old brush
												 //CBrush::
	}
	m_BackGroundBrush.CreateSolidBrush(m_bkcolor);

	if (m_GridPen.m_hObject != NULL)
		m_GridPen.DeleteObject();				// get rid of old pen
	m_GridPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

	CBrush * pOldBrush = DC.SelectObject(&m_BackGroundBrush);
	DC.FillRect(&rc, &m_BackGroundBrush);
	// delete brush 
	m_BackGroundBrush.DeleteObject();
	/*
	
	*/
}
void CELDrawer::DrawGridOnBuilding(CPaintDC &DC)
{
	RECT rc0;
	RECT rc2;
	RECT rc;

	//DC.GetClipBox(&rc);	
	//CPoint null(0,0);
	//DC.LPtoDP(&rc);// const;
	GetCellRect(rc0, CCell(1, 1));

	//GetCellRect(rc2,  CCell(75,  115));
	GetCellRect(rc2, m_RBottom);
	rc.top = rc0.top;
	rc.left = rc0.left;
	rc.right = rc2.right;
	rc.bottom = rc2.bottom;


	
	if (m_BackGroundBrush.m_hObject != NULL) {
		m_BackGroundBrush.DeleteObject();        // get rid of old brush
												 //CBrush::
	}
	
	m_BackGroundBrush.CreateSolidBrush(m_bkcolor);
/**/


	CBrush * pOldBrush = DC.SelectObject(&m_BackGroundBrush);
	DC.FillRect(&rc, &m_BackGroundBrush);
	// delete brush 
	m_BackGroundBrush.DeleteObject();

	DC.SelectObject(&m_GridPen);

	CFont xFont;
	CFont yFont;

	// draw X Grid
	for (int X = 0; X <= (rc.right - rc.left) / m_stXScale; X++)
	{
		for (int Y = 0; Y <= (rc.bottom - rc.top) / m_stYScale; Y++)
		{
			DC.MoveTo(rc.left + X* m_stXScale, (rc.top) + Y* (m_stYScale)-0);
			DC.LineTo(rc.left + X* m_stXScale, rc.top + Y* (m_stYScale)+1);
		}
	}
	for (int Y = 0; Y <= (rc.bottom - rc.top) / m_stYScale; Y++)
	{
		for (int X = 0; X <= (rc.right - rc.left) / m_stXScale; X++)
		{

			DC.MoveTo(rc.left + X* (m_stXScale)-0, Y* m_stYScale + rc.top);
			DC.LineTo(rc.left + X* (m_stXScale)+1, Y* m_stYScale + rc.top);
		}
	}

	// select old pen pOldBrush
	//DC.SelectObject(pOldPen)->DeleteObject();;
	///DC.SelectObject(pOldBrush)->DeleteObject();;

}
void CELDrawer::GetCell(CPoint& point, CCell& rCell)
{
	rCell.SetX((point.x + m_pointOrigin.x) / m_stXScale);
	rCell.SetY((point.y + m_pointOrigin.y) / m_stYScale);
}
void CELDrawer::DrawLiftOnBuilding(CPaintDC &DC, CElevatorState st)
{
	int nCol = st.GetMe() *st.m_nX + st.m_nOffset;
	int nRow = (NUM_FLOORS - 1)  * st.m_nY - st.GetModulus() +st.m_nOffset;
	
	
	DC.SelectObject(&m_LiftPen);

	RECT rc;
	GetCellRect(rc, CCell(nCol, nRow));
	rc.left		= rc.left + 2 * m_stXScale;
	rc.right	= rc.right + 2 * m_stXScale;
	rc.bottom = rc.bottom -1;

	DC.MoveTo(rc.left , rc.top - 2 * m_stYScale);
	DC.LineTo(rc.right, rc.top - 2 * m_stYScale);
	DC.LineTo(rc.right, rc.bottom);
	DC.LineTo(rc.left, rc.bottom);
	DC.LineTo(rc.left, rc.top - 2 * m_stYScale);

	DC.SelectObject(&m_DoorPen);
	
	for (int j = 0; j < st.GetLoadingTimer(); j++)
	{
		DC.MoveTo(rc.left, rc.top+2 -j * m_stYScale);
		DC.LineTo(rc.right, rc.top+2 - j * m_stYScale);
	}

	for (int j = 0; j < st.GetUnloadingTimer(); j++)
	{
		DC.MoveTo(rc.left, rc.top + 2 - j * m_stYScale);
		DC.LineTo(rc.right, rc.top + 2 - j * m_stYScale);
	}


	// free resources
	//m_GridPen.DeleteObject();
}
bool CELDrawer::GetUpButtonPosition(CRect& r, BYTE i)
{
	if (i >= NUM_FLOORS)
		return false;


	//BYTE sY = SPACINGY / NUM_FLOORS - 1;
	
	BYTE sY = SPACINGY / NUM_FLOORS ;
	BYTE sX = SPACINGX;
	r.left = (m_stYScale +2) *m_RBottom.GetX();
	//r.left = m_stYScale *m_RBottom.GetX();
	//r.left = SPACINGX *NUM_CARS;
	r.right = r.left +20;
	r.top = (NUM_FLOORS - i)* sY * (m_stYScale);// +20);
	r.bottom = r.top + 20;

	return true;
}
void CELDrawer::DrawStairOnBuilding(CPaintDC &DC, CHallState st, int upDmdHgltd = -1, int downDmdHgltd = -1)
{
	int nCol = 0;
	int nRow = 0;
	BYTE i = st.GetHallNo();

	RECT rc0;
	RECT rc1;
	
	// Stair text
	BYTE no = st.GetHallNo();
	char buffer[10];
	sprintf_s ( buffer, "0x%x", (int)no);

	DC.SelectObject(&m_FloorPen);									// select pen
	
	// stair cell on screen
	nCol = st.GetOffset();
	nRow = st.GetY() *(NUM_FLOORS  - 1 - i) + st.GetOffset() ;		
	GetCellRect(rc0, CCell(nCol, nRow));

	// floor line
	nCol = st.GetOffset() + st.GetX() * NUM_CARS;
	GetCellRect(rc1, CCell(nCol, nRow));
	DC.MoveTo(rc0.left + 2 * m_stXScale, rc0.bottom );
	DC.LineTo(rc1.left + 2 * m_stXScale, rc0.bottom);
	// down density
	BYTE b = 0;
	for ( b = 0; b < st.GetFloorRequestDensity(DOWN); b++)
	{
		DC.MoveTo(rc0.left + 4 * m_stXScale, rc0.bottom + (b + 1) * 2);
		DC.LineTo(rc1.left + 4 * m_stXScale, rc0.bottom + (b + 1) * 2);
	}

	// up density
	for ( b = 0; b < st.GetFloorRequestDensity(UP); b++)
	{
		DC.MoveTo(rc0.left + 4 * m_stXScale, rc0.bottom - (b + 1) * 2);
		DC.LineTo(rc1.left + 4 * m_stXScale, rc0.bottom - (b + 1) * 2);
	}
/**/

	PrintText(DC, nCol, nRow, buffer);

	// down demad buttons
	if (downDmdHgltd == st.GetHallNo())
		DC.SelectObject(&m_DemandPen_Active);
	else if (st.GetFloorRequestDensity(DOWN) )
		DC.SelectObject(&m_DemandPen_Active);
	else DC.SelectObject(&m_DemandPen_Passive);


	DC.MoveTo(rc0.left / 2, rc0.top + 2 * m_stYScale);
	DC.LineTo(rc0.left / 2 + (2 * m_stXScale), rc0.top);
	DC.LineTo(rc0.left / 2 - (2 * m_stXScale), rc0.top);
	DC.LineTo(rc0.left / 2, rc0.top + 2 * m_stYScale);

	// up demad buttons
	if (upDmdHgltd == st.GetHallNo())
		DC.SelectObject(&m_DemandPen_Active);
	else if (st.GetFloorRequestDensity(UP))
		DC.SelectObject(&m_DemandPen_Active);
	else
		DC.SelectObject(&m_DemandPen_Passive);

	DC.MoveTo(rc0.left / 2 + 2 * (2 * m_stXScale), rc0.top);
	DC.LineTo(rc0.left / 2 + 3 * (2 * m_stXScale), rc0.top + 2 * m_stYScale);
	DC.LineTo(rc0.left / 2 + 1 * (2 * m_stXScale), rc0.top + 2 * m_stYScale);
	DC.LineTo(rc0.left / 2 + 2 * (2 * m_stXScale), rc0.top);
	// }
	//csText.ReleaseBuffer();
	//csText.FreeExtra();
}
void CELDrawer::PrintText(CPaintDC &DC, int nCol, int nRow, char* zName)
{
	CFont font;
	if (!font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!font.CreatePointFont(80, _T("MS Sans Serif")))
			return;

	CString csDumy(zName);
	CFont* pFontOld = DC.SelectObject(&font);
	RECT rc;
	GetCellRect(rc, CCell(nCol, nRow));
	DC.SetBkColor(m_bkcolor);
	DC.ExtTextOutW(rc.left, rc.top, ETO_OPAQUE, NULL, csDumy, NULL);
	//csDumy.ReleaseBuffer();
	//csDumy.FreeExtra();
	// select old font
	DC.SelectObject(pFontOld)->DeleteObject();
	font.DeleteObject();
}
void CELDrawer::DrawLiftDestination(CPaintDC &DC, CElevatorState st, int nHighLighted)
{
	int nCol = 0;
	int nRow = 0;
	char zText[10]; // (L"");
	RECT rc0;
	RECT rc1;
	for (int i = NUM_FLOORS - 1; i > -1; i--)
	{
		DC.SelectObject(&m_FloorPen);
		nCol = st.m_nOffset;
		nRow = 3*(NUM_FLOORS - 1 - i) + st.m_nOffset/2;
		GetCellRect(rc0, CCell(nCol, nRow));

		// floor line
		nCol = st.m_nOffset + (st.m_nX/4) * NUM_CARS;
		GetCellRect(rc1, CCell(nCol, nRow));
		DC.MoveTo(rc0.left, rc0.top + m_stYScale/30);
		DC.LineTo(rc1.left, rc0.top + m_stYScale/30);


		// down demad buttons
		if (st.GetCarDestination(i) == true)
			DC.SelectObject(&m_DemandPen_Active);
		else if (nHighLighted == i)
			DC.SelectObject(&m_HghLghtPen);
		else if (st.GetFloor() == i)
			DC.SelectObject(&m_DemandPen_Current);

		else DC.SelectObject(&m_DemandPen_Passive);


		DC.MoveTo(rc0.left / 2 + (m_stXScale), rc0.top + m_stYScale);
		DC.LineTo(rc0.left / 2 + (m_stXScale), rc0.top);
		DC.LineTo(rc0.left / 2 - (m_stXScale), rc0.top );
		DC.LineTo(rc0.left / 2 - (m_stXScale), rc0.top+ m_stYScale);
		DC.LineTo(rc0.left / 2 + (m_stXScale), rc0.top + m_stYScale);
	



		
//		csText.Format("%x\0", i);
		sprintf_s(zText, "%x", i);
		PrintText(DC, nCol, nRow-1, zText);
	//	csText.ReleaseBuffer();
	//	csText.FreeExtra();

	}

	nCol = st.m_nOffset;
	nRow = st.m_nOffset / 2;
	GetCellRect(rc0, CCell(nCol, nRow));

	if (st.GetDirection() == DOWN)
	{
		DC.SelectObject(&m_DemandPen_Active);
		DC.MoveTo(rc0.left / 2, rc0.top/2 + m_stYScale);
		DC.LineTo(rc0.left / 2 + (m_stXScale), rc0.top/2);
		DC.LineTo(rc0.left / 2 - (m_stXScale), rc0.top/2);
		DC.LineTo(rc0.left / 2, rc0.top/2 + m_stYScale);

	}

	else if (st.GetDirection() == UP)
	{
		DC.SelectObject(&m_DemandPen_Active);
	
		DC.MoveTo(rc0.left / 2 - (m_stXScale), rc0.top / 2);
		DC.LineTo(rc0.left / 2 + (m_stXScale), rc0.top / 2);
		DC.LineTo(rc0.left / 2, rc0.top / 2 - m_stYScale);
		DC.LineTo(rc0.left / 2 - (m_stXScale), rc0.top / 2 );

		//DC.LineTo(rc0.left / 2, rc0.top / 2 + m_stYScale);

	}
}

// CELDrawer member functions
