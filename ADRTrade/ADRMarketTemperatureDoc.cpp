
// ADRTradeDoc.cpp : CADRMarketTemperatureDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CADRMarketTemperatureDoc ����/����

CADRMarketTemperatureDoc::CADRMarketTemperatureDoc()
{
	// TODO: �ڴ����һ���Թ������
}

CADRMarketTemperatureDoc::~CADRMarketTemperatureDoc()
{
}

BOOL CADRMarketTemperatureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
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

// CADRMarketTemperatureDoc ���л�

void CADRMarketTemperatureDoc::Serialize(CArchive& ar)
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



// CADRMarketTemperatureDoc ���
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


// CADRMarketTemperatureDoc ����
