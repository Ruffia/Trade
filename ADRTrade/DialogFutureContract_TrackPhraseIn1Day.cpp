#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract_TrackPhraseIn1Day.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogFutureContract_TrackPhraseIn1Day,string,"CDialogFutureContract_TrackPhraseIn1Day")
CDialogFutureContract_TrackPhraseIn1Day::CDialogFutureContract_TrackPhraseIn1Day()
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_TrackPhraseIn1Day",CDialogFutureContract_TrackPhraseIn1Day::IDD);
}

CDialogFutureContract_TrackPhraseIn1Day::~CDialogFutureContract_TrackPhraseIn1Day()
{

}


void CDialogFutureContract_TrackPhraseIn1Day::_LoadLayout()
{
	CRect rc;
	GetClientRect(rc);

	m_Tab.MoveWindow(rc.left,rc.top,rc.Width() - 10,rc.Height() - 10);

	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	//设定在Tab内显示的范围
	CRect rcHold;
	rcHold.top = rcTab.top + 25;
	rcHold.left = rcTab.left;
	rcHold.right = rcTab.right;
	rcHold.bottom = rcTab.bottom;

	int nCount = 0;
	xml_node root = m_doc.child("root");  //根节点
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

	//显示初始页面
	ShowPage(0);
	//保存当前选择  
	m_CurSelTab = 0;
}


void CDialogFutureContract_TrackPhraseIn1Day::InitPage()
{
	string sFileName = GetModulePath() + "/UI/FutureContract_TrackPhraseIn1Day.xml";
	_LoadXML(CString(sFileName.c_str()));
}