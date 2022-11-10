// DlgSEMLog.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "ChildDlgTab.h"

// CChildDlgTab �Ի���
IMPLEMENT_DYNAMIC(CChildDlgTab, CDialog)

CChildDlgTab::CChildDlgTab(CWnd* pParent /*=NULL*/)
	: CDialog(CChildDlgTab::IDD, pParent)
{
}

CChildDlgTab::~CChildDlgTab()
{
	for (int i = 0; i < m_vPage.size();i++)
	{
		CDialog* pPage = m_vPage[i];
		if (pPage)
		{
			delete pPage;
			pPage = NULL;
		}
	}

	m_vPage.clear();
}

void CChildDlgTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CChildDlgTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CChildDlgTab::OnTcnSelchangeTabLog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CChildDlgTab ��Ϣ��������
BOOL CChildDlgTab::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitPage();
	return TRUE;
}


void CChildDlgTab::ShowPage(const int nCurPage)
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

void CChildDlgTab::OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	const int nCurSel = m_Tab.GetCurSel();
	ShowPage(nCurSel);  
	*pResult = 0;
}


void CChildDlgTab::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayOut();
}


void CChildDlgTab::_DesignLayOut()
{

}