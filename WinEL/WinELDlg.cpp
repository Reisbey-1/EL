
// WinELDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinEL.h"
#include "WinELDlg.h"
#include "afxdialogex.h"
#include "ELDrawer.h"
#include "resource.h"
#include "WEL_Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//theObjectPtr = NULL;
//bool		CWinELDlg::theResumed  = false;
CWinELDlg * CWinELDlg::theObjectPtr = NULL;				// objet pointer to itself

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
protected:

public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CWinELDlg dialog

CWinELDlg::CWinELDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINEL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theCanBus = NULL;

	/// drawings parameters for cabin 0
	m_wKeyPressed = -1;
	m_bGridOn = false;
	m_bPRESSED = FALSE;
	theResumed = true;
	m_nHighlightedDemand_Up = -1;
	m_nHighlightedDemand_Down = -1;
}

void CWinELDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinELDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CWinELDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESUME, &CWinELDlg::OnBnClickedButtonResume)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CWinELDlg message handlers
BOOL CWinELDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CPaintDC dc(this); // device context for painting

	CELDrawer::GetDrawer()->DrawBackColor(dc);
	CELDrawer::GetDrawer()->DrawGridOnBuilding(dc);

	theObjectPtr = this;
/*	
	// orientation on screen
	RECT r, rl;
	GetClientRect(&r);
	pTheLiftDialogs[0]->GetClientRect(&rl);
	int shiftX = rl.right - rl.left + 10;
	
	int shiftY = 40;
	int width = r.right - r.left + 10;
	int higth = r.bottom - r.top;

	//MoveWindow(shiftX * NUM_CARS, shiftY, width, higth);
	r.left = r.left + shiftX * NUM_CARS;
	r.right = r.right + shiftX * NUM_CARS;
	r.top = r.top + shiftY;
	//r.bottom = r.top + shiftY;
	r.bottom = r.bottom + shiftY;
	ClientToScreen(&r); 
	MoveWindow(&r);
*/
	init();
	for (int i = 0; i < NUM_CARS; i++)
	{
		//return TRUE;
		/*
		pTheLiftDialogs[i]->Create(CLiftDlg::IDD, this);
		pTheLiftDialogs[i]->ShowWindow(SW_NORMAL);
		pTheLiftDialogs[i]->CloseWindow();
		pTheLiftDialogs[i]->DestroyWindow();
		//delete pTheLiftDialogs[i];
	*/	
		RECT r;
		pTheLiftDialogs[i]->GetClientRect(&r);
		int shiftX = r.right - r.left + 10;
		int shiftY = 40;
		r.left = r.left + shiftX * i;
		r.right = r.right + shiftX * i;
		r.top = r.top + shiftY;
		r.bottom = r.bottom + shiftY;
		ClientToScreen(&r);
		pTheLiftDialogs[i]->MoveWindow(&r);
	}

	// clok
	typedef  LONG(*f)();
	f func = &clock_tick;
	myClock.AddCallBackFunction((LONG*)func);
	myClock.StartThread();
/**/
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CWinELDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CWinELDlg::_PaintLifts(CPaintDC& dc)
{
	//CPaintDC dc(this);							// device context for painting
	//int nOfset = 20;

	//  1) kabin no								: x m_nCarNumber
	//	2) kabinin o´an icerisinde bulundugu kat: m_nCurrentFloor
	//  3) direction							: m_nCurrentDir
	//	4) indir								: m_nUnloadingTimer
	//  5) bindir								: m_nUnloadingTimer

	// Grid
	if (m_bGridOn == true)
	{
		CELDrawer::GetDrawer()->DrawGridOnBuilding(dc);
	}
	
	// Lifts
	CElevatorState st;
	for (int i = 0; i < NUM_CARS; i++)
	{
		pLifts[i]->GetState(st);
		CELDrawer::GetDrawer()->DrawLiftOnBuilding(dc, st);
	}

	// Floors
	// Invalidate();
	//CStairState ss;
	CHallState hs;
	for (int j = 0; j < NUM_FLOORS; j++)
	{
		if (j == 9)
			j = 9;
		pTheBuilding->GetStairAt(j)->GetState(hs);
		CELDrawer::GetDrawer()->DrawStairOnBuilding(dc, hs, m_nHighlightedDemand_Up, m_nHighlightedDemand_Down);
	}
}
void CWinELDlg::OnPaint()
{
	if (!IsWindowEnabled())
		return;
	CPaintDC dc(this);							// device context for painting
	if (IsIconic())
	{
	//		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	
		CELDrawer::GetDrawer()->DrawBackColor(dc);
		CELDrawer::GetDrawer()->DrawGridOnBuilding(dc);
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		_PaintLifts(dc);
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinELDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CWinELDlg::OnBnClickedButtonDwn()
{
	
	Beep(1000, 100);
	//CDialogEx::OnPaint();

	Invalidate();
}

BOOL CWinELDlg::PreTranslateMessage(MSG* pMsg)
{
	//CString csMsg;
	WORD wParam;
	CHall * pDumyStair = NULL;
	wParam = pMsg->wParam;
//	csMsg.Format(L"msg?:%04x  - key: %04x", pMsg->message, wParam);
	int x = (int)pMsg->wParam;

	// VK_CAPITAL
	// VK_SHIFT
	// VK_CONTROL	does not work !
	int nAt = 300;
	unsigned short capital = GetKeyState(VK_CAPITAL); 
	unsigned short shift = GetKeyState(VK_SHIFT);
	shift = shift & 0x8000;
	if (shift == 0x8000)
	{
		if (wParam == '0') 	{
			nAt = 0;
		}
		else if (wParam == '1') {
			nAt = 1;
		}
		else if (wParam == '2') {
			nAt = 2;
		}
		else if (wParam == '3') {
			nAt = 3;
		}			
		else if (wParam == '4') {
			nAt = 4;
		}
		else if (wParam == '5') {
			nAt = 5;
		}			
		else if (wParam == '6') {
			nAt = 6;
		}
		else if (wParam == '7') {
			nAt = 7;
		}
		else if (wParam == '8') {
			nAt = 8;
		}
		else if (wParam == '9') {
			nAt = 9;
		}
		else if ((wParam == 'A') || (wParam == 'a')) {
			nAt = 10;
		}
		else if ((wParam == 'B') || (wParam == 'b')) {
			nAt = 11;
		}
		else if ((wParam == 'C') || (wParam == 'c')) {
			nAt = 12;
		}
		else if ((wParam == 'D') || (wParam == 'd')) {
			nAt = 13;
		}
		else if ((wParam == 'E') || (wParam == 'e')) {
			nAt = 14;
		}
		else if ((wParam == 'F') || (wParam == 'f')) {
			nAt = 15;
		}

		/// then 
		if (nAt < NUM_FLOORS-1) {
			pDumyStair = (pTheBuilding->GetStairAt(nAt));
			//pDumyStair->SetFloorRequest(UP, true);
			pDumyStair->IncreaseFloorRequestDensity(UP);
			Invalidate();
			return TRUE;
		}

	}
	else if((capital == 0xFF80) || (capital == 0xFF81) )
	{
		if (wParam == '0') {
			nAt = 0;
		}
		else if (wParam == '1') {
			nAt = 1;
		}
		else if (wParam == '2') {
		nAt = 2;
		}
		else if (wParam == '3') {
			nAt = 3;
		}
		else if (wParam == '4') {
			nAt = 4;
		}
		else if (wParam == '5') {
			nAt = 5;
		}
		else if (wParam == '6') {
			nAt = 6;
		}
		else if (wParam == '7') {
			nAt = 7;
		}
		else if (wParam == '8') {
			nAt = 8;
		}
		else if (wParam == '9') {
			nAt = 9;
		}
		else if ((wParam == 'A') || (wParam == 'a')) {
			nAt = 10;
		}
		else if ((wParam == 'B') || (wParam == 'b')) {
			nAt = 11;
		}
		else if ((wParam == 'C') || (wParam == 'c')) {
			nAt = 12;
		}
		else if ((wParam == 'D') || (wParam == 'd')) {
			nAt = 13;
		}
		else if ((wParam == 'E') || (wParam == 'e')) {
			nAt = 14;
		}
		else if ((wParam == 'F') || (wParam == 'f')) {
			nAt = 15;
		}

		/// then 
		if (nAt < NUM_FLOORS && nAt > 0) {
			pDumyStair = (pTheBuilding->GetStairAt(nAt));
			//pDumyStair->SetFloorRequest(DOWN, true);
			pDumyStair->IncreaseFloorRequestDensity(DOWN);
			Invalidate();
			return TRUE;
		}
	}
		return CDialogEx::PreTranslateMessage(pMsg);
}
void CWinELDlg::OnDestroy()
{	
	CDialogEx::OnDestroy();
	CELDrawer::GetDrawer()->Kill();
}
void CWinELDlg::OnBnClickedOk()
{
	clean_up();
	for (int i = 0; i < NUM_CARS; i++)
	{
	//			pTheLiftDialogs[i]->SendMessage(WM_CLOSE);
	//			pTheLiftDialogs[i]->Detach();//  Create(CLiftDlg::IDD, this);
				pTheLiftDialogs[i]->DestroyWindow();
	//			pTheLiftDialogs[i]->PostMessage(WM_CLOSE);
	//			pTheLiftDialogs[i]->CloseWindow();
				Sleep(100);
	//			delete pTheLiftDialogs[i];
	}

	CDialogEx::OnOK();
	
}
void CWinELDlg::OnBnClickedButtonResume()
{
	CButton * pResume = (CButton*)GetDlgItem(IDC_BUTTON_RESUME);
	if (theObjectPtr->theResumed == true)
	{ 
		myClock.pause();
		theObjectPtr->theResumed = false;
			pResume->SetWindowTextW(L"resume");
	}
	else {
		theObjectPtr->theResumed = true;
	myClock.resume();
		pResume->SetWindowTextW(L"pause");
	}
}

void CWinELDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CCell rCell;
	CString csDumy;
	/* get active stair   */
	CELDrawer::GetDrawer()->GetCell(point, rCell);
	
	csDumy.Format(L"%d:%d", rCell.GetX(), rCell.GetY());
	CEdit * pCell = (CEdit*)GetDlgItem(IDC_EDIT_CELL);
	pCell->SetWindowTextW(csDumy);
	
	if (rCell.GetX() > 1 && rCell.GetX() < 6)							// if mouse in these x coordinates
		m_nHighlightedDemand_Down = NUM_FLOORS  - rCell.GetY() / (SPACINGY );		// active stair
	else
		m_nHighlightedDemand_Down = -1;

	if (rCell.GetX() >= 6 && rCell.GetX() < 12)							// if mouse in these x coordinates
		m_nHighlightedDemand_Up = NUM_FLOORS  - rCell.GetY() / (SPACINGY);		// active stair
	else
		m_nHighlightedDemand_Up = -1;
/**/


	CDialogEx::OnMouseMove(nFlags, point);
}



void CWinELDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CHall * pDumyStair = NULL;

	if (m_nHighlightedDemand_Up > -1 && m_nHighlightedDemand_Up < NUM_FLOORS -1)
	{
		pDumyStair = (pTheBuilding->GetStairAt(m_nHighlightedDemand_Up));
		pDumyStair->IncreaseFloorRequestDensity(UP);
	}
	if (m_nHighlightedDemand_Down > 0)
	{
		pDumyStair = (pTheBuilding->GetStairAt(m_nHighlightedDemand_Down));
		pDumyStair->IncreaseFloorRequestDensity(DOWN);
	}
	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}
