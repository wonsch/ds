#pragma once

#include "MainDlg.h"
#include "LogDlg.h"

#define DIALOG_COUNT							2
#define DIALOG_MAIN								0
#define DIALOG_LOG								1

class CSimDlg : public CDialog
{
public:
	CSimDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	enum { IDD = IDD_SIM_DIALOG };

	static DWORD WINAPI							SimulateThread(LPVOID Argument);

protected:
	HICON										m_hIcon;

	virtual void								DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL								OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void								OnDestroy();
	afx_msg void								OnSize(UINT nType, int cx, int cy);
	afx_msg void								OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

	HANDLE										hSimulateThread;
	int											m_TabSel;
	CTabCtrl									m_Tab;
	CDialog*									m_Dialogs[DIALOG_COUNT];
	CMainDlg*									m_MainDlg;
	CLogDlg*									m_LogDlg;
};
