#include "stdafx.h"
#include "Sim.h"
#include "SimDlg.h"

BEGIN_MESSAGE_MAP(CSimApp, CWinApp)
END_MESSAGE_MAP()

CSimApp SimApp;

CSimApp::CSimApp()
{
}

BOOL CSimApp::InitInstance()
{
	CWinApp::InitInstance();

	CwRand wRand(GetTickCount());
	//RandomSeed = wRand();
	RandomSeed = 2925723681;
	CacheMode = MODE_CACHE_OFF;
	GroupMode = MODE_GROUPING_OFF;
	MaxFloodHopCount = 6;
	PeerCount = 10000;
	SearchContentCount = 10;
	GroupSize = 8;

	CSimDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
