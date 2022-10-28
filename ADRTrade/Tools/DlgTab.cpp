// DlgSEMLog.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "DlgTab.h"

// CDlgTab �Ի���
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


// CDlgTab ��Ϣ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const int nCurSel = m_Tab.GetCurSel();
	ShowPage(nCurSel);  
	*pResult = 0;
}


void CDlgTab::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialog::OnClose();
}
