
// ADRTradeView.h : CADRTradeRecordView 类的接口
//

#pragma once
#include <map>
#include <string>
using namespace std;
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;


class CADRTradeRecordView : public CView
{
protected: // 仅从序列化创建
	CADRTradeRecordView();
	DECLARE_DYNCREATE(CADRTradeRecordView)

// 特性
public:
	CADRTradeRecordDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CADRTradeRecordView();
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
	afx_msg void OnTimer(UINT nIDEvent); 
	DECLARE_MESSAGE_MAP()

protected:
	xml_document m_doc;
	
};

#ifndef _DEBUG  // ADRTradeView.cpp 中的调试版本
inline CADRTradeRecordDoc* CADRTradeRecordView::GetDocument() const
   { return reinterpret_cast<CADRTradeRecordDoc*>(m_pDocument); }
#endif

