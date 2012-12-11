#include "stdafx.h"
#include "Sim.h"
#include "SimDlg.h"
#include "../SimModule/PeerInfo.h"
#include "../SimModule/ContentInfo.h"

CSimDlg::CSimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

DWORD CSimDlg::SimulateThread(LPVOID Argument)
{
	CSimDlg *SD = (CSimDlg *)Argument;

	/*if(SimApp.Sim != NULL) delete SimApp.Sim;
	SimApp.Sim = new CSimulatorEx();*/

	SimApp.Sim.Reset(SimApp.RandomSeed);
	SimApp.Sim.SetVerbose(false);
	SimApp.Sim.SetEnvironmentRandomly();
	SimApp.Sim.SetMaxFloodHopCount(SimApp.MaxFloodHopCount);
	SimApp.Sim.SetMode(SimApp.CacheMode, SimApp.GroupMode);
	if(SimApp.GroupMode == MODE_GROUPING_ON)
	{
		SimApp.Sim.SetGroupMaxMemeberNumber(SimApp.GroupSize);
		SimApp.Sim.SetContentInfoFloodingTTL(SimApp.MaxFloodHopCount);
	}

	DWORD StartTime = GetTickCount();

	// 피어 삽입
	for(unsigned int i = 0;i < SimApp.PeerCount / 5;i++) SimApp.Sim.InsertWorkInsertPeer(i + 1, 5);
	unsigned int OldStep = SimApp.Sim.GetStep(), NewStep = OldStep;
	while(true)
	{
		SimApp.Sim.SimulateCount(1);
		NewStep = SimApp.Sim.GetStep();
		SD->m_MainDlg->SetStep(false, NewStep);
		if(NewStep == OldStep) break;
		OldStep = NewStep;
	}

	// 컨텐트 찾기
	CContentInfo *ContentInfo = SimApp.Sim.GetRandomContent();
	for(unsigned int i = 0;i < SimApp.SearchContentCount;i++)
	{
		SimApp.Sim.InsertWorkSearchContent(SimApp.Sim.GetStep() + 1, SIM_RANDOM_VALUE, ContentInfo->ContentID);
		while(true)
		{
			SimApp.Sim.SimulateCount(1);
			NewStep = SimApp.Sim.GetStep();
			SD->m_MainDlg->SetStep(false, NewStep);
			if(NewStep == OldStep) break;
			OldStep = NewStep;
		}
	}
	SD->m_MainDlg->SetStep(true, NewStep);

	DWORD EndTime = GetTickCount();

	CAtlString String;
	String.AppendFormat("*** Simulation is terminated at step %u\r\n\r\n", SimApp.Sim.GetStep());
	String.AppendFormat("*** Simulation Statistics\r\n");
	String.AppendFormat("%s\r\n", SimApp.Sim.GetStatistics());

	String.AppendFormat("*** Simulation Output\r\n");
	String.AppendFormat("%s\r\n", SimApp.Sim.GetLog());

	SD->m_MainDlg->m_Statistics.SetWindowText(String);

	SD->m_MainDlg->OnBnClickedSettingStart();

	SD->hSimulateThread = NULL;

	return 0;
}

void CSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CSimDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CSimDlg::OnTcnSelchangeTab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CSimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	hSimulateThread = NULL;

	m_Tab.InsertItem(0, "Main");
	m_Tab.InsertItem(1, "Log");

	m_MainDlg = new CMainDlg(&m_Tab);
	m_MainDlg->m_SimDlg = this;
	m_MainDlg->Create(IDD_MAIN_DIALOG);
	m_Dialogs[DIALOG_MAIN] = m_MainDlg;

	m_LogDlg = new CLogDlg(&m_Tab);
	m_LogDlg->m_SimDlg = this;
	m_LogDlg->Create(IDD_LOG_DIALOG);
	m_Dialogs[DIALOG_LOG] = m_LogDlg;

	for(int i = 0;i < DIALOG_COUNT;i++) m_Dialogs[i]->SetWindowPos(NULL, 4, 24, 0, 0, SWP_NOSIZE);

	m_TabSel = DIALOG_MAIN;
	m_Dialogs[m_TabSel]->ShowWindow(SW_SHOW);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSimDlg::OnDestroy()
{
	CDialog::OnDestroy();

	for(int i = 0;i < DIALOG_COUNT;i++)
	{
		m_Dialogs[i]->DestroyWindow();
		delete m_Dialogs[i];
	}
}

void CSimDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_Tab.GetSafeHwnd() == NULL) return;

	CRect WRect;
	GetClientRect(&WRect);
	m_Tab.SetWindowPos(NULL, 0, 0, WRect.Width(), WRect.Height(), 0);

	m_Tab.GetClientRect(&WRect);
	for(int i = 0;i < DIALOG_COUNT;i++)
	{
		m_Dialogs[i]->SetWindowPos(NULL, 0, 0, WRect.Width() - 10, WRect.Height() - 28, SWP_NOMOVE);
	}
}

void CSimDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int Sel = m_Tab.GetCurSel();
	if(m_TabSel == Sel) return;

	m_Dialogs[m_TabSel]->ShowWindow(SW_HIDE);
	m_TabSel = Sel;
	m_Dialogs[m_TabSel]->ShowWindow(SW_SHOW);

	*pResult = 0;
}
