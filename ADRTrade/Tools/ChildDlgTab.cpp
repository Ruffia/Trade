// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "ChildDlgTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CChildDlgTab 对话框
IMPLEMENT_DYNAMIC(CChildDlgTab, CDialog)

CChildDlgTab::CChildDlgTab(CWnd* pParent /*=NULL*/)
	: CDialog(CChildDlgTab::IDD, pParent)
{
}

CChildDlgTab::~CChildDlgTab()
{
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


// CChildDlgTab 消息处理程序
BOOL CChildDlgTab::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CChildDlgTab::ShowPage(const int nCurPage)
{
	for (int i = 0; i < m_vPage.size();i++)
	{
		shared_ptr<CDialog> pPage = m_vPage[i];
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
	// TODO: 在此添加控件通知处理程序代码
	const int nCurSel = m_Tab.GetCurSel();
	ShowPage(nCurSel);  
	*pResult = 0;
}


void CChildDlgTab::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_ReLayout();
}


void CChildDlgTab::_ReLayout()
{

}