#pragma once
//#include "ELCell.h"

// CELDrawer command target
// singleton
class CELDrawer : public CObject
{
	CELDrawer();
	virtual ~CELDrawer();
	static CELDrawer* m_pelDrawer;
public:
	DECLARE_SERIAL(CELDrawer)
	static CELDrawer* GetDrawer();
	static void Kill();

	void	DrawGridOnBuilding(CPaintDC &DC);
	void	DrawBackColor(CPaintDC &DC);
	void	GetCell(CPoint& point, CCell& rCell);
	void	GetCellRect(RECT& rc, CCell cell);
	void	DrawLiftOnBuilding(CPaintDC &DC, CElevatorState st);

	void    DrawStairOnBuilding(CPaintDC &DC, CHallState st, int upDmdHgltd, int downDmdHgltd);


	void	DrawLiftDestination(CPaintDC &DC, CElevatorState st, int nHighLighted = -1);
	bool	GetUpButtonPosition(CRect& r, BYTE i);
	void	PrintText(CPaintDC &DC, int nCol, int nRow, char* zName);

	int		GetStair(CPoint point);
protected:
	COLORREF	m_bkcolor;			// back color
	COLORREF	m_gridcolor;		// back color
	CBrush		m_BackGroundBrush;	//(m_bkcolor);
	CPen		m_GridPen;
	CPen		m_LiftPen;
	CPen		m_FloorPen;
	CPen		m_DemandPen_Active;
	CPen		m_DemandPen_Passive;
	CPen		m_DemandPen_Current;
	CPen		m_HghLghtPen;
	CPen		m_DoorPen;
	int			m_stXScale;			// grid scale  X
	int			m_stYScale;			// grid scale  Y
	CCell		m_RBottom;			// grid range righ - bottom cell
	CPoint		m_pointOrigin;

};


