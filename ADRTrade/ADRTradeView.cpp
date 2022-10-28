
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

#ifdef _DEBUG
#define new DEBUG_NEW
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
END_MESSAGE_MAP()

// CADRTradeView ����/����

CADRTradeView::CADRTradeView()
{
	// TODO: �ڴ˴���ӹ������

}

CADRTradeView::~CADRTradeView()
{
}

BOOL CADRTradeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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
