
// ADRTradeView.h : CADRMarketTemperatureView ��Ľӿ�
//

#pragma once
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;


class CADRMarketTemperatureView : public CView
{
protected: // �������л�����
	CADRMarketTemperatureView();
	DECLARE_DYNCREATE(CADRMarketTemperatureView)

// ����
public:
	CADRMarketTemperatureDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CADRMarketTemperatureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void _LoadLayout();
	void _LoadDataFromDB();
	void _CreateUI();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent); 
	DECLARE_MESSAGE_MAP()

protected:
	xml_document m_doc;
	
};

#ifndef _DEBUG  // ADRTradeView.cpp �еĵ��԰汾
inline CADRTradeDayDoc* CADRMarketTemperatureView::GetDocument() const
   { return reinterpret_cast<CADRTradeDayDoc*>(m_pDocument); }
#endif

