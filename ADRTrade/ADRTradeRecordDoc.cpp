
// ADRTradeDoc.cpp : CADRTradeRecordDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CADRTradeRecordDoc ����/����

CADRTradeRecordDoc::CADRTradeRecordDoc()
{
	// TODO: �ڴ����һ���Թ������
}

CADRTradeRecordDoc::~CADRTradeRecordDoc()
{
}

BOOL CADRTradeRecordDoc::OnNewDocument()
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


BOOL CADRTradeRecordDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

// CADRTradeRecordDoc ���л�

void CADRTradeRecordDoc::Serialize(CArchive& ar)
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



// CADRTradeRecordDoc ���
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


// CADRTradeRecordDoc ����
