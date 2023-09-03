// DialogPlaceHolder.cpp : 实现文件
//
#include "stdafx.h"
#include "ADRTrade.h"
#include "DialogPlaceHolder.h"
#include "afxdialogex.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"
#include "Tools/StyleManager.h"
#include "Tools/CollectiveComponentProvider.h"
#include "BusinessEdit.h"
#include "BusinessComboBox.h"
#include "BusinessCheckBox.h"
#include "Util.h"
#include "EditTreeCtrlEx.h"
#include "PrimaryKeyRule.h"
#include "TradeDayPrimaryData.h"
#include "ControlCreator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDialogPlaceHolder 对话框

IMPLEMENT_DYNAMIC(CDialogPlaceHolder, CDialog)

CDialogPlaceHolder::CDialogPlaceHolder(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPlaceHolder::IDD, pParent),m_sLayout(""),m_strDataProvider("")
{
	m_mapUIName2Wnd.clear();
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
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogPlaceHolder 消息处理程序
BOOL CDialogPlaceHolder::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	return TRUE;
}


void CDialogPlaceHolder::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout(nType,cx,cy);
}


void CDialogPlaceHolder::_InitLayOut()
{
	if("" == m_sLayout) return;
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
		_ReadUIData(node, data);
		_CreateUI(data, node);
		node = node.next_sibling();
	}
}


void CDialogPlaceHolder::_DesignLayout(UINT nType, int cx, int cy)
{
}

void CDialogPlaceHolder::SetLayout( string& sLayout )
{
	m_sLayout = sLayout;
}


void CDialogPlaceHolder::_ReadUIData( xml_node &node, CUIData &data ) 
{
	data.m_sName = node.attribute("Name").as_string("");
	data.m_strUIClassName = node.attribute("ClassName").as_string("");
	data.m_nID = node.attribute("ID").as_int();
	data.m_nLeft = node.attribute("Left").as_int();
	data.m_nTop = node.attribute("Top").as_int();
	data.m_nWidth = node.attribute("Width").as_int();
	data.m_nHeight = node.attribute("Height").as_int();
	const string sStyle = node.attribute("Style").as_string();
	data.m_dwTotalStyle = CStyleMgr::Instance().GetStyle(sStyle);
	data.m_sCaption = node.attribute("Caption").as_string();
	CFont* pFont = CCollectiveComponentProvider::Instance().GetFont();
}


void CDialogPlaceHolder::_CreateUI( CUIData &data, xml_node node )
{
	IControlCreator* pCreator = Factory<IControlCreator,string>::Instance().BuildProduct(data.m_strUIClassName);
	if(!pCreator) return;
	pCreator->Create(data,node,m_mapUIName2Wnd,this);
	delete pCreator; pCreator = NULL;
}
