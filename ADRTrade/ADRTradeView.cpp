
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
	string sFileName = GetModulePath() + "/UI/ADRTradeView.xml";
	_LoadXML(CString(sFileName.c_str()));

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


void CADRTradeView::_LoadXML(const CString& strLayoutFile)
{
	bool bRet = true;
	if (!m_doc.load_file((LPSTR)(LPCTSTR)strLayoutFile)) { //����xml�ļ�
		return;
	}

	xml_node root = m_doc.child("root");  //���ڵ�
	xml_node nodeLayout = root.child("Layout");
	for (xml_node node = nodeLayout.first_child();node != nodeLayout.last_child(); node = node.next_sibling())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		m_mapUIName2Data[sName] = data;	
	}
}

// CADRTradeView ��Ϣ�������
void CADRTradeView::_LoadLayout()
{
	xml_node root = m_doc.child("root");  //���ڵ�
	xml_node nodeLayout = root.child("Layout");
	for (xml_node node = nodeLayout.first_child();node != nodeLayout.last_child(); node = node.next_sibling())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		m_mapUIName2Data[sName] = data;	
	}
}