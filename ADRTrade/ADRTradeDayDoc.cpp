
// ADRTradeDoc.cpp : CADRTradeDayDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CADRTradeDayDoc ����/����

CADRTradeDayDoc::CADRTradeDayDoc()
{
	// TODO: �ڴ����һ���Թ������
}

CADRTradeDayDoc::~CADRTradeDayDoc()
{
}

BOOL CADRTradeDayDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
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

// CADRTradeDayDoc ���л�

void CADRTradeDayDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}



// CADRTradeDayDoc ���
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


// CADRTradeDayDoc ����
