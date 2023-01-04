
// ADRTradeView.h : CADRTradeDayView 类的接口
//

#pragma once
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class CADRTradeDayView : public CView
{
protected: // 仅从序列化创建
	CADRTradeDayView();
	DECLARE_DYNCREATE(CADRTradeDayView)

// 特性
public:
	CADRTradeDayDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CADRTradeDayView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void _LoadLayout();
	void _LoadDataFromDB();
	void _CreateUI();

// 生成的消息映射函数
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

#ifndef _DEBUG  // ADRTradeView.cpp 中的调试版本
inline CADRTradeDayDoc* CADRTradeDayView::GetDocument() const
   { return reinterpret_cast<CADRTradeDayDoc*>(m_pDocument); }
#endif

