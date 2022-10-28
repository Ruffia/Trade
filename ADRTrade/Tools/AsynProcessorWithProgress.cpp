#include "stdafx.h"
#include "AsynProcessorWithProgress.h"

//
// CThreadForProgress
IMPLEMENT_DYNCREATE(CThreadForProgress, CWinThread)
CThreadForProgress::CThreadForProgress()
{
	m_bAutoDelete= FALSE;
	m_bBeDoingJob = FALSE;
}

CThreadForProgress::~CThreadForProgress()
{
	Stop();
}


BOOL CThreadForProgress::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadForProgress::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadForProgress, CWinThread)
	ON_THREAD_MESSAGE( WM_SHOW_PROGESSDLG, OnShowProgressDlg )
    ON_THREAD_MESSAGE( WM_SETPROGRESSSTEP, OnSetProgressStep )
END_MESSAGE_MAP()


// CThreadForProgress 消息处理程序
void CThreadForProgress::Start()
{
	if (m_hThread)
		return ;

	CreateThread(CREATE_SUSPENDED);
	ResumeThread();

	return ;
}


void CThreadForProgress::Stop()
{
	if (m_hThread)
	{
		PostThreadMessage(WM_QUIT,0,0);

		DWORD dwRes= ::WaitForSingleObject(m_hThread,50000);
		if (dwRes == WAIT_TIMEOUT)
		{
			ASSERT(FALSE); //不应该到达超时
		}
	}

	m_hThread= NULL;
	return ;
}

void CThreadForProgress::OnShowProgressDlg(WPARAM wParm, LPARAM lParam)
{
	m_dlgProgress.DoModal();
	
}

void CThreadForProgress::CloseProgressDlg()
{
	::PostMessage(m_dlgProgress.m_hWnd,WM_CLOSE,0,0);	 
}

void CThreadForProgress::ShowProgressDlg()
{
	PostThreadMessage(WM_SHOW_PROGESSDLG,0,0);
}

void CThreadForProgress::SetRange(int nStep)
{
	PostThreadMessage(WM_SETPROGRESSSTEP,WPARAM(nStep),0);
}

void CThreadForProgress::OnSetProgressStep(WPARAM wParm, LPARAM lParam)
{
	int nStepCount = int(wParm);
	m_dlgProgress.SetRange(nStepCount);

}



//////////////////////////////////////////////////////////////////////////
// AsynProcessorWithProgress

IMPLEMENT_DYNCREATE(AsynProcessorWithProgress, CWinThread)

AsynProcessorWithProgress::AsynProcessorWithProgress()
{
	m_bAutoDelete= FALSE;
	m_pJobProxy = NULL;
}

AsynProcessorWithProgress::~AsynProcessorWithProgress()
{
	Stop();
}

BOOL AsynProcessorWithProgress::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int AsynProcessorWithProgress::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(AsynProcessorWithProgress, CWinThread)
	ON_THREAD_MESSAGE( WM_DOJOB, OnDoJob )
END_MESSAGE_MAP()


// AsynProcessorWithProgress 消息处理程序
void AsynProcessorWithProgress::Initial(CJobProxy* pJobProxy/* = NULL*/)
{
	m_pJobProxy = pJobProxy;
	Start();
}

void AsynProcessorWithProgress::Start()
{
	if (m_hThread)
		return ;

	CreateThread(CREATE_SUSPENDED);
	ResumeThread();

	m_threadProgress.Start();
}


void AsynProcessorWithProgress::Stop()
{
	m_threadProgress.Stop();
	if (m_hThread)
	{
		PostThreadMessage(WM_QUIT,0,0);

		DWORD dwRes= ::WaitForSingleObject(m_hThread,50000);
		if (dwRes == WAIT_TIMEOUT)
		{
			ASSERT(FALSE); //不应该到达超时
		}
	}

	m_hThread= NULL;
}

void AsynProcessorWithProgress::OnDoJob(WPARAM wParm, LPARAM lParam)
{
	CSingleLock lock(&m_sec,TRUE);
	if (m_threadProgress.m_bBeDoingJob)
	{
		Sleep(1000);
	}

	m_threadProgress.m_bBeDoingJob = TRUE;
	m_threadProgress.ShowProgressDlg();
	Sleep(100);

	if (m_pJobProxy)
	{
		m_pJobProxy->DoJob(this);
	}

	m_threadProgress.CloseProgressDlg();
	m_threadProgress.m_bBeDoingJob = FALSE; 
	lock.Unlock();
}

void AsynProcessorWithProgress::StartDoJob()
{
	PostThreadMessage(WM_DOJOB,0,0);
}

void AsynProcessorWithProgress::SetTitle(LPCTSTR szTitle)
{
	m_threadProgress.GetDlg().SetTitle(szTitle);
}
void AsynProcessorWithProgress::SetText(LPCTSTR szText)
{
	m_threadProgress.GetDlg().SetText(szText);
}


void AsynProcessorWithProgress::SetProgressInfo(CString strProgressInfo)
{
	m_threadProgress.GetDlg().SetProgressInfo(strProgressInfo);
}

void AsynProcessorWithProgress::DoProgress()
{
	m_threadProgress.GetDlg().ProgressStep();
}

void AsynProcessorWithProgress::SetProgressRange(const int nStep)
{
	//m_threadProgress.GetDlg().SetRange(nStep);
	m_threadProgress.SetRange(nStep);
}



