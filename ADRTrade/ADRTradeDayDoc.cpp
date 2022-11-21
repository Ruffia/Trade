
// ADRTradeDoc.cpp : CADRTradeDayDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRTradeDayDoc.h"
#include "Business/Business.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CADRTradeDayDoc

IMPLEMENT_DYNCREATE(CADRTradeDayDoc, CDocument)

BEGIN_MESSAGE_MAP(CADRTradeDayDoc, CDocument)
END_MESSAGE_MAP()


// CADRTradeDayDoc 构造/析构

CADRTradeDayDoc::CADRTradeDayDoc()
{
	// TODO: 在此添加一次性构造代码
}

CADRTradeDayDoc::~CADRTradeDayDoc()
{
}

BOOL CADRTradeDayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y%m%d");
	SetTitle(strDate);
	return TRUE;
}


BOOL CADRTradeDayDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

// CADRTradeDayDoc 序列化

void CADRTradeDayDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}



// CADRTradeDayDoc 诊断
#ifdef _DEBUG
void CADRTradeDayDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADRTradeDayDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CADRTradeDayDoc 命令
