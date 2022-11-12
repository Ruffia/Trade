
// ADRTradeView.cpp : CADRTradeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRTradeDoc.h"
#include "ADRTradeView.h"
#include "Util.h"
#include "DialogPlaceHolder.h"
#include "Tools/ChildDlgTab.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CADRTradeView

IMPLEMENT_DYNCREATE(CADRTradeView, CView)

BEGIN_MESSAGE_MAP(CADRTradeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CADRTradeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CADRTradeView 构造/析构

CADRTradeView::CADRTradeView()
{
	// TODO: 在此处添加构造代码

}

CADRTradeView::~CADRTradeView()
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
}

BOOL CADRTradeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	_LoadLayout();
	return CView::PreCreateWindow(cs);
}

// CADRTradeView 绘制

void CADRTradeView::OnDraw(CDC* /*pDC*/)
{
	CADRTradeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CADRTradeView 打印


void CADRTradeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CADRTradeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CADRTradeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CADRTradeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CADRTradeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CADRTradeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CADRTradeView 诊断

#ifdef _DEBUG
void CADRTradeView::AssertValid() const
{
	CView::AssertValid();
}

void CADRTradeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CADRTradeDoc* CADRTradeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADRTradeDoc)));
	return (CADRTradeDoc*)m_pDocument;
}
#endif //_DEBUG


// CADRTradeView 消息处理程序
void CADRTradeView::_LoadLayout()
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
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		m_mapUIName2Data[sName] = data;	
		node = node.next_sibling();
	}
}


void CADRTradeView::OnInitialUpdate()
{
	CRect rc;
	GetClientRect(rc);

	for (map<string,CUIData>::iterator it = m_mapUIName2Data.begin();
		it != m_mapUIName2Data.end();it++)
	{
		string sCaption = it->first;
		CUIData& UIData = it->second;
		CDialog* pDlg = Factory<CDialog,string>::Instance().BuildProduct(UIData.m_strUIClassName);
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(UIData.m_strUIClassName);
		ASSERT(-1 != nIDD);
		CDialogPlaceHolder* pHolder = dynamic_cast<CDialogPlaceHolder*>(pDlg);
		if (pHolder)
		{
			pHolder->SetLayout(UIData.m_strLayout);
		}
		else
		{
			CChildDlgTab* pTabWnd = dynamic_cast<CChildDlgTab*>(pDlg);
			if (pTabWnd)
			{
				pTabWnd->SetLayout(UIData.m_strLayout);
			}
		}
		pDlg->Create(nIDD,this);
		CRect rcDialog(rc.left + UIData.m_nLeft,rc.top + UIData.m_nTop,rc.left + UIData.m_nLeft + UIData.m_nWidth,rc.top + UIData.m_nHeight);
		pDlg->MoveWindow(rcDialog);	
		pDlg->ShowWindow(SW_SHOW);
        UIData.m_pWnd = pDlg;	
	}

	__super::OnInitialUpdate();
}


int CADRTradeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}