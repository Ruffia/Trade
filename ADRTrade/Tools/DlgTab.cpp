// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgTab.h"

#include "../Common/Factory.h"
#include "ChildDlgTab.h"
#include "DialogIDManager.h"
#include "DialogPlaceHolder.h"
#include "DialogPlaceHolderComposite.h"
#include "UIData.h"
#include "Util.h"
#include "CollectiveComponentProvider.h"

// CDlgTab 对话框
IMPLEMENT_DYNAMIC(CDlgTab, CDialog)

CDlgTab::CDlgTab(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTab::IDD, pParent)
{
}

CDlgTab::~CDlgTab()
{
	m_vPage.clear();
}

void CDlgTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDlgTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDlgTab::OnTcnSelchangeTabLog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgTab 消息处理程序
BOOL CDlgTab::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CDlgTab::ShowPage(const int nCurPage)
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


void CDlgTab::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_ReLayout();
}


void CDlgTab::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	CRect rc;
	GetClientRect(rc);
	m_Tab.MoveWindow(rc.left + 10,rc.top + 10,rc.Width() - 10,rc.Height() - 10);

	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
	m_Tab.SetFont(pFont);

	int nCount = 0;
	xml_node root = m_doc.child("root");  //根节点
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nID = node.attribute("ID").as_int();
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();

		//设定在Tab内显示的范围
		CRect rcHold;
		rcHold.top = rcTab.top + data.m_nTop;
		rcHold.left = rcTab.left;
		rcHold.right = rcTab.right;
		rcHold.bottom = rcTab.bottom;

		CDialog* pDlg = Factory<CDialog,string>::Instance().BuildProduct(data.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(data.m_strUIClassName);
		ASSERT(-1 != nIDD);
		CDialogPlaceHolder* pHolder = dynamic_cast<CDialogPlaceHolder*>(pDlg);
		if (pHolder)
		{
			pHolder->SetLayout(data.m_strLayout);
		}
		else
		{
			CChildDlgTab* pTabWnd = dynamic_cast<CChildDlgTab*>(pDlg);
			if (pTabWnd)
			{
				pTabWnd->SetLayout(data.m_strLayout);
			}
			else
			{
				CDialogPlaceHolderComposite* pDlgComposite = dynamic_cast<CDialogPlaceHolderComposite*>(pDlg);
				if (pDlgComposite)
				{
					pDlgComposite->SetLayout(data.m_strLayout);
				}
			}
		}

		pDlg->Create(nIDD,&m_Tab);
		pDlg->SetParent(&m_Tab);
		pDlg->MoveWindow(&rcHold);

		m_Tab.InsertItem(nCount,sName.c_str());

		shared_ptr<CDialog> ptr(pDlg);
		m_vPage.push_back(ptr);
		data.m_pWnd = pDlg;

		nCount++;
		node = node.next_sibling();
	}

	//显示初始页面
	ShowPage(0);
	//保存当前选择  
	m_CurSelTab = 0;
}


void CDlgTab::_ReLayout()
{
	if (!IsWindow(m_Tab.m_hWnd)){
		return;
	}
	CRect rc;
	GetClientRect(rc);
	m_Tab.MoveWindow(rc.left + 10,rc.top + 10,rc.Width() - 10,rc.Height() - 10);

	CRect rcTab;
	m_Tab.GetClientRect(rcTab);
}