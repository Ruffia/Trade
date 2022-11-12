
// ADRTradeView.h : CADRTradeView ��Ľӿ�
//

#pragma once
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class CADRTradeView : public CView
{
protected: // �������л�����
	CADRTradeView();
	DECLARE_DYNCREATE(CADRTradeView)

// ����
public:
	CADRTradeDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CADRTradeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void _LoadLayout();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

protected:
	xml_document m_doc;
	map<string,CUIData>  m_mapUIName2Data;
};

#ifndef _DEBUG  // ADRTradeView.cpp �еĵ��԰汾
inline CADRTradeDoc* CADRTradeView::GetDocument() const
   { return reinterpret_cast<CADRTradeDoc*>(m_pDocument); }
#endif

