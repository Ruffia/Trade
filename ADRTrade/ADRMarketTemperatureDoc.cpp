
// ADRTradeDoc.cpp : CADRMarketTemperatureDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRMarketTemperatureDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CADRMarketTemperatureDoc

IMPLEMENT_DYNCREATE(CADRMarketTemperatureDoc, CDocument)

BEGIN_MESSAGE_MAP(CADRMarketTemperatureDoc, CDocument)
END_MESSAGE_MAP()


// CADRMarketTemperatureDoc 构造/析构

CADRMarketTemperatureDoc::CADRMarketTemperatureDoc()
{
	// TODO: 在此添加一次性构造代码
}

CADRMarketTemperatureDoc::~CADRMarketTemperatureDoc()
{
}

BOOL CADRMarketTemperatureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	CString strTitle = "Market Temperature";
	SetTitle(strTitle);
	return TRUE;
}


BOOL CADRMarketTemperatureDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

// CADRMarketTemperatureDoc 序列化

void CADRMarketTemperatureDoc::Serialize(CArchive& ar)
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



// CADRMarketTemperatureDoc 诊断
#ifdef _DEBUG
void CADRMarketTemperatureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADRMarketTemperatureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CADRMarketTemperatureDoc 命令
