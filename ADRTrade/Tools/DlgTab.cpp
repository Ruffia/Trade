// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgTab.h"

// CDlgTab 对话框
IMPLEMENT_DYNAMIC(CDlgTab, CDialog)

CDlgTab::CDlgTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab::IDD, pParent)
{
}

CDlgTab::~CDlgTab()
{
	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		if (pPage)
		{
			pPage->DestroyWindow();
			delete pPage;
			pPage = NULL;
		}
	}

	m_vPage.clear();
}

void CDlgTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDlgTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTab::OnTcnSelchangeTabLog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgTab 消息处理程序
BOOL CDlgTab::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitPage();
	return TRUE;
}


void CDlgTab::ShowPage(const int nCurPage)
{
	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		if (!pPage) continue;
		if (nCurPage == i)
		{
			pPage->ShowWindow(SW_SHOW);
		}
		else
		{
			pPage->ShowWindow(SW_HIDE);
		}
	}
}

void CDlgTab::OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	const int nCurSel = m_Tab.GetCurSel();
	ShowPage(nCurSel);  
	*pResult = 0;
}


void CDlgTab::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialog::OnClose();
}
