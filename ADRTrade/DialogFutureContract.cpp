#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDlgDailyFutureContract,string,"CDlgDailyFutureContract")
CDlgDailyFutureContract::CDlgDailyFutureContract()
{
	CDialogIDMgr::Instance().Register("CDlgDailyFutureContract",CDlgDailyFutureContract::IDD);
}

CDlgDailyFutureContract::~CDlgDailyFutureContract()
{
	for (map<string,CWnd*>::iterator it = m_mapUIName2Wnd.begin();
		it != m_mapUIName2Wnd.end();it++)
	{
		CDialog* pDlg = dynamic_cast<CDialog*>(it->second);
		if (pDlg)
		{
			delete pDlg;
			pDlg = NULL;
		}
	}
	m_mapUIName2Wnd.clear();
}


void CDlgDailyFutureContract::_LoadLayout()
{
	CRect rc;
	GetClientRect(rc);

	xml_node root = m_doc.child("root");  //¸ù½Úµã
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();

		CDialog* pDlg = Factory<CDialog,string>::Instance().BuildProduct(data.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(data.m_strUIClassName);
		ASSERT(-1 != nIDD);
		pDlg->Create(nIDD,this);
		CRect rcDialog(rc.left + data.m_nLeft,rc.top + data.m_nTop,rc.left + data.m_nWidth,rc.top + data.m_nHeight);
		pDlg->MoveWindow(rcDialog);	
		pDlg->ShowWindow(SW_SHOW);
		data.m_pWnd = pDlg;	

		m_mapUIName2Wnd[sName] = pDlg;	
		node = node.next_sibling();
	}
}


void CDlgDailyFutureContract::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/DailyFutureContract.xml";
	_LoadXML(CString(sFileName.c_str()));
}