// LiftDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinEL.h"
#include "LiftDlg.h"
#include "afxdialogex.h"
#include "ELDrawer.h"


// CLiftDlg dialog

IMPLEMENT_DYNAMIC(CLiftDlg, CDialogEx)

CLiftDlg::CLiftDlg( CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LIFT, pParent)
{
	m_nNr = INVALID;
	m_pElevator = NULL;
	m_nHighlightedStair = -1;
}

CLiftDlg::~CLiftDlg()
{
}
void CLiftDlg::SetNo(int no)
{
	m_nNr = no;
}
void CLiftDlg::SetElevator(CElevator* pElevator)
{
	m_pElevator = pElevator;
}

void CLiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLiftDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CLiftDlg message handlers
void CLiftDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::PostNcDestroy();
	delete this;
}
void CLiftDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	

	CDialogEx::OnClose();
	DestroyWindow();
}
BOOL CLiftDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//enter[Ctrl+nr]
	CString csMsg;
	csMsg.Format(L"Car-Nr: %i  ", m_nNr);
	SetWindowTextW(csMsg);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CLiftDlg::PreTranslateMessage(MSG* pMsg)
{
	WORD wParam;
	wParam = pMsg->wParam;
	WORD b = wParam;
	int nCarDestination = 300;

	unsigned short control = GetKeyState(VK_CONTROL);
	control = control & 0x8000;
	if (control == 0x8000)
	{
		// shift UP
		if ((pMsg->message == WM_KEYUP) || (pMsg->message == WM_SYSKEYUP))
		{
			if (wParam == '0')	{
				nCarDestination = 0;
			}
			else if (wParam == '1') {
				nCarDestination = 1;
			}
			else if (wParam == '2') {
				nCarDestination = 2;
			}
			else if (wParam == '3') {
				nCarDestination = 3;
			}
			else if (wParam == '4') {
				nCarDestination = 4;
			}
			else if (wParam == '5') {
				nCarDestination = 5;
			}
			else if (wParam == '6') {
				nCarDestination = 6;
			}
			else if (wParam == '7') {
				nCarDestination = 7;
			}
			else if (wParam == '8') {
				nCarDestination = 8;
			}
			else if (wParam == '9') {
				nCarDestination = 9;
			}
			else if ((wParam == 'A') || (wParam == 'a')) {
				nCarDestination = 10;
			}
			else if ((wParam == 'B') || (wParam == 'b')) {
				nCarDestination =11;
			}
			else if ((wParam == 'C') || (wParam == 'c')) {
				nCarDestination = 12;
			}
			else if ((wParam == 'D') || (wParam == 'd')) {
				nCarDestination = 13;
			}
			else if ((wParam == 'E') || (wParam == 'e')) {
				nCarDestination = 14;
			}
			else if ((wParam == 'F') || (wParam == 'f')) {
				nCarDestination =15;
			}
			
			/// then 
			if (nCarDestination < NUM_FLOORS) {
				m_pElevator->SetCarDestination(nCarDestination, true);
				Invalidate();
				return TRUE;
			}
		}
	}
	return FALSE;
//	return CDialogEx::PreTranslateMessage(pMsg);
}
void CLiftDlg::OnPaint()
{
	if (!IsWindowEnabled())
		return;

	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CElevatorState es;

	es.Clear();
	m_pElevator->GetState(es);
	CELDrawer::GetDrawer()->DrawLiftDestination(dc, es, m_nHighlightedStair);

}


void CLiftDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	/* get active stair   */
	CCell rCell;
	CELDrawer::GetDrawer()->GetCell(point, rCell);
	if (rCell.GetX() > 1 && rCell.GetX() < 6)						// if mouse in these x coordinates
		m_nHighlightedStair = NUM_FLOORS - rCell.GetY() / 3;		// active stair
	else
		m_nHighlightedStair = -1;

	CDialogEx::OnMouseMove(nFlags, point);

}

void CLiftDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_nHighlightedStair > -1)
		m_pElevator->SetCarDestination(m_nHighlightedStair, true);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLiftDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);
	m_nHighlightedStair = - 1;
	// TODO: Add your message handler code here
}
