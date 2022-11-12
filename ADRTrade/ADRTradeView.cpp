
// ADRTradeView.cpp : CADRTradeView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CADRTradeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CADRTradeView ����/����

CADRTradeView::CADRTradeView()
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	_LoadLayout();
	return CView::PreCreateWindow(cs);
}

// CADRTradeView ����

void CADRTradeView::OnDraw(CDC* /*pDC*/)
{
	CADRTradeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CADRTradeView ��ӡ


void CADRTradeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CADRTradeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CADRTradeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CADRTradeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CADRTradeView ���

#ifdef _DEBUG
void CADRTradeView::AssertValid() const
{
	CView::AssertValid();
}

void CADRTradeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CADRTradeDoc* CADRTradeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADRTradeDoc)));
	return (CADRTradeDoc*)m_pDocument;
}
#endif //_DEBUG


// CADRTradeView ��Ϣ�������
void CADRTradeView::_LoadLayout()
{
	string sFileName = GetModulePath() + "/UI/ADRTradeView.xml";
	bool bRet = true;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //����xml�ļ�
		return;
	}

	xml_node root = m_doc.child("root");  //���ڵ�
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