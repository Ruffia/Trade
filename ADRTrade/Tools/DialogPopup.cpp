// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPopup.h"
#include "afxdialogex.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "DialogPlaceHolder.h"
#include "DialogPlaceHolderComposite.h"
#include "ChildDlgTab.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogPopup, CDialog)

CDialogPopup::CDialogPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPopup::IDD, pParent)
{
	m_mapUIName2Wnd.clear();
}

CDialogPopup::~CDialogPopup()
{
	m_mapUIName2Wnd.clear();
}

void CDialogPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPopup, CDialog)
END_MESSAGE_MAP()


// CDialogPlaceHolder 消息处理程序
BOOL CDialogPopup::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CDialogPopup::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	CRect rc;
	GetClientRect(rc);

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
		const string sStyle = node.attribute("Style").as_string();
		DWORD dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
		const string sCaption = node.attribute("Caption").as_string();
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
		pDlg->Create(nIDD,this);
		CRect rcDialog(rc.left + data.m_nLeft,rc.top + data.m_nTop,rc.left + data.m_nLeft + data.m_nWidth,rc.top + data.m_nHeight);
		pDlg->MoveWindow(rcDialog);	
		pDlg->ShowWindow(SW_SHOW);
		data.m_pWnd = pDlg;	
		node = node.next_sibling();
	}
}