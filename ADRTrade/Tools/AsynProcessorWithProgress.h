#pragma once
#include "Dlg4ProgressThread.h"

class AsynProcessorWithProgress;

class CJobProxy
{
public:
	virtual void DoJob(AsynProcessorWithProgress* pProgress) = 0;
};


#define WM_SHOW_PROGESSDLG 9001
#define WM_DOJOB 9002
#define WM_SETPROGRESSSTEP 9410
// CThreadForProgress

class CThreadForProgress : public CWinThread
{
	DECLARE_DYNCREATE(CThreadForProgress)

public:
	CThreadForProgress();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadForProgress();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void Start();
	void Stop();
	void CloseProgressDlg();
	void ShowProgressDlg();
	CDlg4ProgressThread& GetDlg()
	{
		return m_dlgProgress;
	}

	void SetRange(int nStep);

protected:
	afx_msg void OnShowProgressDlg(WPARAM wParm, LPARAM lParam);
	afx_msg void OnSetProgressStep(WPARAM wParm, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CDlg4ProgressThread m_dlgProgress;
public:
	BOOL m_bBeDoingJob;
};


// AsynProcessorWithProgress

class AsynProcessorWithProgress : public CWinThread
{
	DECLARE_DYNCREATE(AsynProcessorWithProgress)

public:
	AsynProcessorWithProgress();           // 动态创建所使用的受保护的构造函数
	virtual ~AsynProcessorWithProgress();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


	void Initial(CJobProxy* pJobProxy = NULL);
	void StartDoJob();
	void SetTitle(LPCTSTR szTitle);
	void SetText(LPCTSTR szText);
    void SetProgressInfo(CString strProgressInfo);
	void SetProgressRange(const int nStep);
	void DoProgress();
	void Stop();

private:
	void Start();
	

protected:
	afx_msg void OnDoJob(WPARAM wParm, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CThreadForProgress m_threadProgress;
	CJobProxy* m_pJobProxy;
	CCriticalSection m_sec;
};


