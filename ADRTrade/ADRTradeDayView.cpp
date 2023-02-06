
// ADRTradeView.cpp : CADRTradeDayView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRTradeDayDoc.h"
#include "ADRTradeDayView.h"
#include "Util.h"
#include "Tools/DialogPlaceHolder.h"
#include "Tools/DialogPlaceHolderComposite.h"
#include "Tools/ChildDlgTab.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"
#include "DBDataManager.h"
#include "UIDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CADRTradeDayView

IMPLEMENT_DYNCREATE(CADRTradeDayView, CView)

BEGIN_MESSAGE_MAP(CADRTradeDayView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CADRTradeDayView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CADRTradeDayView 构造/析构

CADRTradeDayView::CADRTradeDayView()
{
	// TODO: 在此处添加构造代码

}

CADRTradeDayView::~CADRTradeDayView()
{
}

BOOL CADRTradeDayView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	_LoadLayout();
	return CView::PreCreateWindow(cs);
}

// CADRTradeDayView 绘制

void CADRTradeDayView::OnDraw(CDC* /*pDC*/)
{
	CADRTradeDayDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CADRTradeDayView 打印


void CADRTradeDayView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CADRTradeDayView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CADRTradeDayView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CADRTradeDayView 诊断

#ifdef _DEBUG
void CADRTradeDayView::AssertValid() const
{
	CView::AssertValid();
}

void CADRTradeDayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CADRTradeDayDoc* CADRTradeDayView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADRTradeDayDoc)));
	return (CADRTradeDayDoc*)m_pDocument;
}
#endif //_DEBUG


// CADRTradeDayView 消息处理程序
void CADRTradeDayView::_LoadLayout()
{
	string sFileName = GetModulePath() + "/UI/ADRTradeView.xml";
	bool bRet = true;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	xml_node root = m_doc.child("root");  //根节点
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		data.m_sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		CUIDataMgr::Instance().Register(data.m_sName,data);
		node = node.next_sibling();
	}
}


void CADRTradeDayView::OnInitialUpdate()
{
	_LoadDataFromDB();
	_CreateUI();
	SetTimer(Timer_SaveData2UI,5000,NULL);
	__super::OnInitialUpdate();
}


int CADRTradeDayView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CADRTradeDayView::_CreateUI()
{
	CRect rc;
	GetClientRect(rc);
	map<string,CUIData>& mapUIName2Data = CUIDataMgr::Instance().GetUIData();
	for (map<string,CUIData>::iterator it = mapUIName2Data.begin();
		it != mapUIName2Data.end();it++)
	{
		string sCaption = it->first;
		CUIData& UIData = it->second;
		CDialogPlaceHolder* pDlg = Factory<CDialogPlaceHolder,string>::Instance().BuildProduct(UIData.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(UIData.m_strUIClassName);
		ASSERT(-1 != nIDD);
		CDialogPlaceHolder* pHolder = dynamic_cast<CDialogPlaceHolder*>(pDlg);
		if (pHolder)
		{
			pHolder->SetBusiness(UIData.m_sName);
			pHolder->SetLayout(UIData.m_strLayout);		
		}
		else
		{
			CDialogPlaceHolderComposite* pCompositeDlg = dynamic_cast<CDialogPlaceHolderComposite*>(pDlg);
			if (pCompositeDlg)
			{
				pCompositeDlg->SetLayout(UIData.m_strLayout);
			}
			else
			{
				CChildDlgTab* pTabWnd = dynamic_cast<CChildDlgTab*>(pDlg);
				if (pTabWnd)
				{
					pTabWnd->SetLayout(UIData.m_strLayout);
				}
			}
		}
		pDlg->Create(nIDD,this);
		CRect rcDialog(rc.left + UIData.m_nLeft,rc.top + UIData.m_nTop,rc.left + UIData.m_nLeft + UIData.m_nWidth,rc.top + UIData.m_nHeight);
		pDlg->MoveWindow(rcDialog);	
		pDlg->ShowWindow(SW_SHOW);
		UIData.m_pWnd = pDlg;	
	}
}

void CADRTradeDayView::_LoadDataFromDB()
{
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");
	const int nWeekDay = CaculateWeekDay(dtNOw.GetYear(),dtNOw.GetMonth(),dtNOw.GetDay());
	if (Saturday == nWeekDay || Sunday == nWeekDay)
	{
		return;
	}
	
	FieldValue v;
	v.SetDataType("string");
	v.SetValueString((LPSTR)(LPCTSTR)strDate);
	bool bFestival = CDBDataManager::Instance().RecordExists("Festival","Date","string",v);
	if (bFestival)
	{
		return;
	}


	bool bExist = CDBDataManager::Instance().RecordExists("Trade_DailyMacroEnvironmentAnalyze","TradeDay","string",v);
	if (!bExist)
	{
		CDBDataManager::Instance().InsertRecordWithPrimaryKey("Trade_DailyMacroEnvironmentAnalyze","TradeDay","string",v);
	}

	bExist = CDBDataManager::Instance().RecordExists("Future_DailyTradeSummarize","TradeDay","string",v);
	if (!bExist)
	{
		CDBDataManager::Instance().InsertRecordWithPrimaryKey("Future_DailyTradeSummarize","TradeDay","string",v);
	}

	bExist = CDBDataManager::Instance().RecordExists("Future_IntradayTrade","TradeDay","string",v);
	if (!bExist)
	{
		CDBDataManager::Instance().InsertRecordWithPrimaryKey("Future_IntradayTrade","TradeDay","string",v);
	}
}


void CADRTradeDayView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == Timer_SaveData2UI)
	{
		CUIDataMgr::Instance().UpdateUI2DB();
	}

	__super::OnTimer(nIDEvent);
}