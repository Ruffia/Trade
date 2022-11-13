#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "DialogPlaceHolder.h"
#include "DialogPlaceHolderComposite.h"
#include "Tools/ChildDlgTab.h"
#include "UIData.h"
#include "Util.h"
#include "Tools/CollectiveComponentProvider.h"


IMPLEMENT_FACTORY(CDialog,CDlgDailyAnalyze,string,"CDlgDailyAnalyze")
CDlgDailyAnalyze::CDlgDailyAnalyze()
{
	CDialogIDMgr::Instance().Register("CDlgDailyAnalyze",CDlgDailyAnalyze::IDD);
}

CDlgDailyAnalyze::~CDlgDailyAnalyze()
{

}

void CDlgDailyAnalyze::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	CRect rc;
	GetClientRect(rc);
	m_Tab.MoveWindow(rc.left,rc.top,rc.Width() - 10,rc.Height() - 10);

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