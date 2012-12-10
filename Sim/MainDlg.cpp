#include "stdafx.h"
#include "Sim.h"
#include "SimDlg.h"
#include "MainDlg.h"

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS, m_Settings);
	DDX_Control(pDX, IDC_STATISTICS, m_Statistics);
	DDX_Control(pDX, IDC_SETTING_RANDOMSEED, m_Setting_RandomSeed);
	DDX_Control(pDX, IDC_SETTING_START, m_Setting_Start);
	DDX_Control(pDX, IDC_SETTING_CACHE, m_Setting_Cache);
	DDX_Control(pDX, IDC_SETTING_GROUP, m_Setting_Group);
	DDX_Control(pDX, IDC_SETTING_PEERCOUNT, m_Setting_PeerCount);
	DDX_Control(pDX, IDC_SETTING_SEARCHCONTENTCOUNT, m_Setting_SearchContentCount);
	DDX_Control(pDX, IDC_SETTING_GROUPSIZE, m_Setting_GroupSize);
	DDX_Control(pDX, IDC_RESET, m_Setting_Reset);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RESET, &CMainDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_SETTING_START, &CMainDlg::OnBnClickedSettingStart)
	ON_BN_CLICKED(IDC_SETTING_CACHE, &CMainDlg::OnBnClickedSettingCache)
	ON_BN_CLICKED(IDC_SETTING_GROUP, &CMainDlg::OnBnClickedSettingGroup)
END_MESSAGE_MAP()

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	char String[32];
	sprintf(String, "%u", SimApp.RandomSeed);
	m_Setting_RandomSeed.SetWindowText(String);
	sprintf(String, "%u", SimApp.PeerCount);
	m_Setting_PeerCount.SetWindowText(String);
	sprintf(String, "%u", SimApp.SearchContentCount);
	m_Setting_SearchContentCount.SetWindowText(String);
	sprintf(String, "%u", SimApp.GroupSize);
	m_Setting_GroupSize.SetWindowText(String);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_Settings.GetSafeHwnd() == NULL) return;

	CRect Rect1, Rect2;
	GetClientRect(&Rect1);

	m_Settings.GetWindowRect(&Rect2);
	m_Settings.SetWindowPos(NULL, 0, 0, Rect1.Width(), Rect2.Height(), SWP_NOMOVE);

	m_Statistics.GetWindowRect(&Rect2);
	m_Statistics.SetWindowPos(NULL, 0, 0, Rect1.Width(), Rect1.Height() - 126, SWP_NOMOVE);
}

void CMainDlg::OnBnClickedReset()
{
	CwRand wRand(GetTickCount());
	SimApp.RandomSeed = wRand();
	char String[32];
	sprintf(String, "%u", SimApp.RandomSeed);
	m_Setting_RandomSeed.SetWindowText(String);

	m_Statistics.SetWindowText("");
}

void CMainDlg::OnBnClickedSettingCache()
{
	if(m_Setting_Cache.GetState() & BST_CHECKED) m_Setting_Cache.SetWindowText("CACHE_ON");
	else m_Setting_Cache.SetWindowText("CACHE_OFF");
}

void CMainDlg::OnBnClickedSettingGroup()
{
	if(m_Setting_Group.GetState() & BST_CHECKED) m_Setting_Group.SetWindowText("GROUP_ON");
	else m_Setting_Group.SetWindowText("GROUP_OFF");
}

void CMainDlg::OnBnClickedSettingStart()
{
	if(m_SimDlg->hSimulateThread == NULL)
	{
		m_Setting_RandomSeed.EnableWindow(FALSE);
		m_Setting_Reset.EnableWindow(FALSE);
		m_Setting_Cache.EnableWindow(FALSE);
		m_Setting_Group.EnableWindow(FALSE);
		m_Setting_PeerCount.EnableWindow(FALSE);
		m_Setting_SearchContentCount.EnableWindow(FALSE);
		m_Setting_GroupSize.EnableWindow(FALSE);
		m_Statistics.SetWindowText("");

		char String[32];
		m_Setting_RandomSeed.GetWindowText(String, _countof(String));
		SimApp.RandomSeed = (unsigned int)atof(String);
		SimApp.CacheMode = m_Setting_Cache.GetState() & BST_CHECKED ? MODE_CACHE_ON : MODE_CACHE_OFF;
		SimApp.GroupMode = m_Setting_Group.GetState() & BST_CHECKED ? MODE_GROUPING_ON : MODE_GROUPING_OFF;
		m_Setting_PeerCount.GetWindowText(String, _countof(String));
		SimApp.PeerCount = atoi(String);
		m_Setting_SearchContentCount.GetWindowText(String, _countof(String));
		SimApp.SearchContentCount = atoi(String);
		m_Setting_GroupSize.GetWindowText(String, _countof(String));
		SimApp.GroupSize = atoi(String);

		m_SimDlg->hSimulateThread = CreateThread(NULL, 0, CSimDlg::SimulateThread, m_SimDlg, 0, NULL);
		CloseHandle(m_SimDlg->hSimulateThread);

		m_Setting_Start.SetWindowText("Stop");
	}
	else
	{
		m_Setting_RandomSeed.EnableWindow(TRUE);
		m_Setting_Reset.EnableWindow(TRUE);
		m_Setting_Cache.EnableWindow(TRUE);
		m_Setting_Group.EnableWindow(TRUE);
		m_Setting_PeerCount.EnableWindow(TRUE);
		m_Setting_SearchContentCount.EnableWindow(TRUE);
		m_Setting_GroupSize.EnableWindow(TRUE);

		m_Setting_Start.SetWindowText("Start");
	}
}
