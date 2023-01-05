// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "BusinessEdit.h"
#include "Util.h"
#include "EditTreeCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogPlaceHolder, CDialog)

CDialogPlaceHolder::CDialogPlaceHolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPlaceHolder::IDD, pParent)
{
	m_mapUIName2Wnd.clear();
	m_sBusiness = "";
}

CDialogPlaceHolder::~CDialogPlaceHolder()
{
	for (map<string,CWnd*>::iterator it = m_mapUIName2Wnd.begin();
		it != m_mapUIName2Wnd.end();it++)
	{
		CWnd* pWnd = it->second;
		if (pWnd)
		{
			delete pWnd;
			pWnd = NULL;
		}
	}
	m_mapUIName2Wnd.clear();
}

void CDialogPlaceHolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlaceHolder, CDialog)
END_MESSAGE_MAP()


// CDialogPlaceHolder 消息处理程序
BOOL CDialogPlaceHolder::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CDialogPlaceHolder::_InitLayOut()
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	bool bRet = true;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}
	
	CRect rc;
	GetClientRect(rc);
	ClientToScreen(rc);

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
		const string sStyle = node.attribute("Style").as_string();
		DWORD dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
		const string sCaption = node.attribute("Caption").as_string();
		CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
		if (data.m_strUIClassName.find("CEditTreeCtrlEx") != string::npos)
		{
			CEditTreeCtrlEx* pTree = new CEditTreeCtrlEx;
			CRect rcTree(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pTree->Create(dwTotalStyle,rcTree,this,data.m_nID);		
			pTree->SetFont(pFont);
			pTree->ShowWindow(SW_SHOW);
			m_mapUIName2Wnd[sName] = pTree;	
		}
		else if (data.m_strUIClassName.find("CBusinessEdit") != string::npos)
		{
			CBusinessEdit* pEdit = new CBusinessEdit;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
			pEdit->SetFont(pFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetWindowText(sCaption.c_str());
			const string& sBusiness = node.attribute("business").as_string();
			pEdit->m_sBusinessField = sBusiness;
			m_mapUIName2Wnd[sName] = pEdit;	
		}
		else if (data.m_strUIClassName.find("CEdit") != string::npos)
		{
			CEdit* pEdit = new CEdit;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pEdit->Create(dwTotalStyle,rc,this,data.m_nID);	
			pEdit->SetFont(pFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->SetWindowText(sCaption.c_str());
			m_mapUIName2Wnd[sName] = pEdit;	
		}
		else if (data.m_strUIClassName.find("CComBox") != string::npos)
		{
			CComboBox* pCombox = new CComboBox;
			CRect rc(data.m_nLeft,data.m_nTop,data.m_nLeft + data.m_nWidth ,data.m_nTop + data.m_nHeight);
			pCombox->Create(dwTotalStyle,rc,this,data.m_nID);	
			xml_node nodeDropdownItem = node.child("DropdownItem");
			while (!nodeDropdownItem.empty())
			{
				string sDropItem = nodeDropdownItem.attribute("Name").as_string("");
				pCombox->AddString(sDropItem.c_str());
				nodeDropdownItem = nodeDropdownItem.next_sibling();
			}

			pCombox->SetFont(pFont);
			pCombox->ShowWindow(SW_SHOW);
			pCombox->SetWindowText(sCaption.c_str());
			m_mapUIName2Wnd[sName] = pCombox;	
		}
		node = node.next_sibling();
	}
}


void CDialogPlaceHolder::UpdateDB2UI()
{
	for(map<string,CWnd*>::iterator it = m_mapUIName2Wnd.begin();
		it != m_mapUIName2Wnd.end();it++)
	{
		CWnd* pControl = it->second;
		CBusinessEdit* pBusinessControl = dynamic_cast<CBusinessEdit*>(pControl);
		if (pBusinessControl)
		{
		}
	}
	
}


void CDialogPlaceHolder::UpdateUI2DB()
{

}