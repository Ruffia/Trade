#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract_LongPeriodicAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Util.h"
#include "Tools/StyleManager.h"
#include "Tools/CollectiveComponentProvider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogFutureContract_LongPeriodicAnalyze,string,"CDialogFutureContract_LongPeriodicAnalyze")
CDialogFutureContract_LongPeriodicAnalyze::CDialogFutureContract_LongPeriodicAnalyze()
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_LongPeriodicAnalyze",CDialogFutureContract_LongPeriodicAnalyze::IDD);
}

CDialogFutureContract_LongPeriodicAnalyze::~CDialogFutureContract_LongPeriodicAnalyze()
{
	m_mapUIName2Wnd.clear();
}


void CDialogFutureContract_LongPeriodicAnalyze::_LoadLayout()
{
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
		const string sStyle = node.attribute("Style").as_string();
		DWORD dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
		const string sCaption = node.attribute("Caption").as_string();
		if (data.m_strUIClassName.find("CEdit") != string::npos)
		{
			CEdit* pEdit = new CEdit;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
			CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
			pEdit->SetFont(pFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetWindowText(sCaption.c_str());
			shared_ptr<CEdit> ptr(pEdit);
			m_mapUIName2Wnd[sName] = ptr;	
		}	
		node = node.next_sibling();
	}
}


void CDialogFutureContract_LongPeriodicAnalyze::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/FutureContract_LongPeriodicAnalyze.xml";
	_LoadXML(CString(sFileName.c_str()));
}