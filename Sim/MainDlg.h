#pragma once
#include "afxwin.h"

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void								DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL								OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void								OnSize(UINT nType, int cx, int cy);
	afx_msg void								OnBnClickedReset();
	afx_msg void								OnBnClickedSettingCache();
	afx_msg void								OnBnClickedSettingGroup();
	afx_msg void								OnBnClickedSettingStart();

	class CSimDlg*								m_SimDlg;
	CStatic										m_Settings;
	CEdit										m_Setting_RandomSeed;
	CButton										m_Setting_Reset;
	CButton										m_Setting_Cache;
	CButton										m_Setting_Group;
	CEdit										m_Setting_PeerCount;
	CEdit										m_Setting_SearchContentCount;
	CEdit										m_Setting_GroupSize;
	CButton										m_Setting_Start;
	CEdit										m_Statistics;
};
