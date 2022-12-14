// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolderComposite.h"
#include "DialogPlaceHolder.h"
#include "Tools/CustomTabCtrlDlg.h"
#include "afxdialogex.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "UIData.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Util.h"

// CDialogPlaceHolderComposite 对话框

IMPLEMENT_DYNAMIC(CDialogPlaceHolderComposite, CDialogEx)

CDialogPlaceHolderComposite::CDialogPlaceHolderComposite(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogPlaceHolderComposite::IDD, pParent)
{
	m_mapUIName2Data.clear();
}

CDialogPlaceHolderComposite::~CDialogPlaceHolderComposite()
{
	for (map<string,CUIData>::iterator it = m_mapUIName2Data.begin();
		it != m_mapUIName2Data.end();it++)
	{
		CUIData& uiData = it->second;
		CWnd* pWnd = uiData.m_pWnd;
		if (pWnd)
		{
			delete pWnd;
			pWnd = NULL;
		}
	}

	m_mapUIName2Data.clear();
}

void CDialogPlaceHolderComposite::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlaceHolderComposite, CDialogEx)
END_MESSAGE_MAP()


// CDialogPlaceHolderComposite 消息处理程序
BOOL CDialogPlaceHolderComposite::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CDialogPlaceHolderComposite::_InitLayOut()
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
			CCustomTabCtrlDlg* pTabWnd = dynamic_cast<CCustomTabCtrlDlg*>(pDlg);
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
		CRect rcDialog(rc.left + data.m_nLeft,rc.top + data.m_nTop,rc.left + data.m_nLeft + data.m_nWidth,rc.top + data.m_nTop + data.m_nHeight);
		pDlg->MoveWindow(rcDialog);	
		pDlg->ShowWindow(SW_SHOW);
		data.m_pWnd = pDlg;	
		node = node.next_sibling();
	}
}