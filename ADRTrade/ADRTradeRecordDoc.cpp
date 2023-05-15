
// ADRTradeDoc.cpp : CADRTradeRecordDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRTradeRecordDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CADRTradeRecordDoc, CDocument)

BEGIN_MESSAGE_MAP(CADRTradeRecordDoc, CDocument)
END_MESSAGE_MAP()


// CADRTradeRecordDoc 构造/析构

CADRTradeRecordDoc::CADRTradeRecordDoc()
{
	// TODO: 在此添加一次性构造代码
}

CADRTradeRecordDoc::~CADRTradeRecordDoc()
{
}

BOOL CADRTradeRecordDoc::OnNewDocument()
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


BOOL CADRTradeRecordDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

// CADRTradeRecordDoc 序列化

void CADRTradeRecordDoc::Serialize(CArchive& ar)
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



// CADRTradeRecordDoc 诊断
#ifdef _DEBUG
void CADRTradeRecordDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADRTradeRecordDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CADRTradeRecordDoc 命令
