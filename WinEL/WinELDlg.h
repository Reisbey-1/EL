
// WinELDlg.h : header file
//

#pragma once

#include "LiftDlg.h"
/*
class CLift1 : public CLift
{
public :
	CLift1() {};
	~CLift1() {};
	CLift1::CLift1(int nCarNumber) 
	{
		//CLift::CLift(nCarNumber);
		CElevator::CElevator(nCarNumber);
	}
};*/

class CWinELDlg;

// CWinELDlg dialog
class CWinELDlg : public CDialogEx
{
	CElevator * test;
	bool m_bGridOn;
	void _PaintLifts(CPaintDC& dc);
	BOOL m_bPRESSED;
	WORD m_wKeyPressed;
	
// Construction
public:
	static CWinELDlg * theObjectPtr;				// objet pointer to itself

	CWinELDlg(CWnd* pParent = NULL);	// standard constructor
	bool	theResumed;					// true if the process is stopped

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINEL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int m_nHighlightedDemand_Up;
	int m_nHighlightedDemand_Down;
public:
	afx_msg void OnBnClickedButtonDwn();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonResume();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
//CWinELDlg * theObjectPtr;				// objet pointer to itself			// objet pointer to itself

