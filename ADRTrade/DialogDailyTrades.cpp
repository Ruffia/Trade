#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyTrades.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Util.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_FACTORY(CDialog,CDlgDailyTrades,string,"CDlgDailyTrades")
CDlgDailyTrades::CDlgDailyTrades()
{
	CDialogIDMgr::Instance().Register("CDlgDailyTrades",CDlgDailyTrades::IDD);
}

CDlgDailyTrades::~CDlgDailyTrades()
{

}


void CDlgDailyTrades::_InitLayOut()
{
	return;
	string sFileName = GetModulePath() + "/UI/DailyTrades.xml";
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //����xml�ļ�
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
	xml_node root = m_doc.child("root");  //���ڵ�
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_nID = node.attribute("ID").as_int();
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();

		//�趨��Tab����ʾ�ķ�Χ
		CRect rcHold;
		rcHold.top = rcTab.top + data.m_nTop;
		rcHold.left = rcTab.left;
		rcHold.right = rcTab.right;
		rcHold.bottom = rcTab.bottom;

		CDialog* pDlg = Factory<CDialog,string>::Instance().BuildProduct(data.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(data.m_strUIClassName);
		ASSERT(-1 != nIDD);
		pDlg->Create(nIDD,&m_Tab);
		pDlg->SetParent(&m_Tab);
		pDlg->MoveWindow(&rcHold);

		m_Tab.InsertItem(nCount,sName.c_str());
		m_vPage.push_back(pDlg);
		data.m_pWnd = pDlg;

		nCount++;
		node = node.next_sibling();
	}

	//��ʾ��ʼҳ��
	ShowPage(0);
	//���浱ǰѡ��  
	m_CurSelTab = 0;
}