#pragma once
#include <afxmt.h>

class AFX_EXT_CLASS CStatusDetector
{
public:
	CStatusDetector(void);
	~CStatusDetector(void);

public:
	virtual BOOL Start();
	BOOL End();
	virtual void Release();
	void SetSleep(DWORD dwSleep){ m_dwSleep = dwSleep; }
	virtual DWORD DoJob() = 0;
	bool m_bExitWorkThread;

protected:
	DWORD  m_dwSleep;
	HANDLE m_hExitEvent;
	CCriticalSection m_cs;
	CWinThread *m_pWorkThread;	
	static UINT WorkThread(LPVOID lpData);
};
