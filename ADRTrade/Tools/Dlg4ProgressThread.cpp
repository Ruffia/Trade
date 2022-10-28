#include "stdafx.h"
#include "Dlg4ProgressThread.h"

IMPLEMENT_DYNAMIC(CDlg4ProgressThread, CDialog)
CDlg4ProgressThread::CDlg4ProgressThread()
	: CDialog(CDlg4ProgressThread::IDD)
{
	m_nTimer= -1;
	m_bStopProgress = FALSE;
	m_strTitle = _T("等待对话框");
	m_strText = _T("处理中......");
}

CDlg4ProgressThread::~CDlg4ProgressThread()
{
}

void CDlg4ProgressThread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PROGRESS4Thread,m_ProgCtrl);
}


BEGIN_MESSAGE_MAP(CDlg4ProgressThread, CDialog)
	//ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlg4ProgressThread 消息处理程序
BOOL CDlg4ProgressThread::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ProgCtrl.SetRange(0,500);
	m_ProgCtrl.SetStep(1);
	m_ProgCtrl.SetPos(0);
	m_nTimer= SetTimer(1,10,NULL);
	SetTitle(m_strTitle);
	SetText(m_strText);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg4ProgressThread::OnTimer(UINT nIDEvent) 
{
	static int nPos = 0;
	if (nIDEvent == m_nTimer)
	{
		if (m_bStopProgress)
		{
			m_bStopProgress = FALSE;
			OnOK();
		}
		else
		{
			m_ProgCtrl.StepIt();

		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlg4ProgressThread::OnDestroy() 
{
	if (m_nTimer!= -1)
	{
		KillTimer(m_nTimer);
	}

	CDialog::OnDestroy();
}

void CDlg4ProgressThread::SetTitle(LPCTSTR szTitle)
{
	m_strTitle= szTitle;
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		SetWindowText(m_strTitle);
	}
}

void CDlg4ProgressThread::StopProgress()
{
	//m_bStopProgress = TRUE;
	OnOK();
}

void CDlg4ProgressThread::SetText(LPCTSTR szText)
{
	m_strText= szText;

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		CWnd* pText= GetDlgItem(IDC_STATIC_TEXT);
		if (pText)
		{
			pText->SetWindowText(m_strText);
		}
	}
}

void CDlg4ProgressThread::SetRange(const int nStepCount)
{
	m_ProgCtrl.SetRange(0,nStepCount);
	m_ProgCtrl.SetPos(0);
}

void CDlg4ProgressThread::SetProgressInfo(CString strProgressInfo)
{
	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		CWnd* pText= GetDlgItem(IDC_STATIC_PROGRESS);
		if (pText)
		{
			pText->SetWindowText(strProgressInfo);
		}
	}

}

void CDlg4ProgressThread::OnClose()
{
	OnOK();
}