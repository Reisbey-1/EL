#pragma once
#include "Resource.h"

// CLiftDlg dialog

class CLiftDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLiftDlg)

public:
	CLiftDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLiftDlg();

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LIFT };
//#endif
	void SetNo(int no);
	void SetElevator(CElevator* pElevator);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	
	int m_nNr;
	CElevator * m_pElevator;						// pointer on elevator object in DECS
	int			m_nHighlightedStair;				// selected stair number as mouse move on

public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
