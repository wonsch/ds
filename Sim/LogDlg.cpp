#include "stdafx.h"
#include "Sim.h"
#include "LogDlg.h"

IMPLEMENT_DYNAMIC(CLogDlg, CDialog)

CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
}

CLogDlg::~CLogDlg()
{
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG, m_Log);
}

BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_Log.GetSafeHwnd() == NULL) return;

	CRect Rect1;
	GetClientRect(&Rect1);
	m_Log.SetWindowPos(NULL, 0, 0, Rect1.Width(), Rect1.Height(), SWP_NOMOVE);
}
