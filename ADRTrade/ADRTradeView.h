
// ADRTradeView.h : CADRTradeView 类的接口
//

#pragma once
#include "LoadLayout.h"
#include <map>
using namespace std;

class CADRTradeView : public CView,public ILoadLayout
{
protected: // 仅从序列化创建
	CADRTradeView();
	DECLARE_DYNCREATE(CADRTradeView)

// 特性
public:
	CADRTradeDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CADRTradeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void _LoadLayout();

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	map<string,CUIData>  m_mapUIName2Data;
};

#ifndef _DEBUG  // ADRTradeView.cpp 中的调试版本
inline CADRTradeDoc* CADRTradeView::GetDocument() const
   { return reinterpret_cast<CADRTradeDoc*>(m_pDocument); }
#endif

