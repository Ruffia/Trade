
// ADRTradeView.cpp : CADRMarketTemperatureView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ADRTrade.h"
#endif

#include "ADRMarketTemperatureDoc.h"
#include "ADRMarketTemperatureView.h"
#include "Util.h"
#include "Tools/DialogPlaceHolder.h"
#include "Tools/DialogPlaceHolderComposite.h"
#include "Tools/ChildDlgTab.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"
#include "DBDataManager.h"
#include "UIDataManager.h"
#include "TradeDayPrimaryData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CADRMarketTemperatureView

IMPLEMENT_DYNCREATE(CADRMarketTemperatureView, CView)

BEGIN_MESSAGE_MAP(CADRMarketTemperatureView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CADRMarketTemperatureView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CADRMarketTemperatureView ����/����

CADRMarketTemperatureView::CADRMarketTemperatureView()
{
	// TODO: �ڴ˴���ӹ������

}

CADRMarketTemperatureView::~CADRMarketTemperatureView()
{
}

BOOL CADRMarketTemperatureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	_LoadLayout();
	return CView::PreCreateWindow(cs);
}

// CADRMarketTemperatureView ����

void CADRMarketTemperatureView::OnDraw(CDC* /*pDC*/)
{
	CADRMarketTemperatureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CADRMarketTemperatureView ��ӡ


void CADRMarketTemperatureView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CADRMarketTemperatureView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CADRMarketTemperatureView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CADRMarketTemperatureView ���

#ifdef _DEBUG
void CADRMarketTemperatureView::AssertValid() const
{
	CView::AssertValid();
}

void CADRMarketTemperatureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CADRMarketTemperatureDoc* CADRMarketTemperatureView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADRMarketTemperatureDoc)));
	return (CADRMarketTemperatureDoc*)m_pDocument;
}
#endif //_DEBUG


// CADRMarketTemperatureView ��Ϣ�������
void CADRMarketTemperatureView::_LoadLayout()
{
	string sFileName = GetModulePath() + "/UI/ADRMarketTemperatureView.xml";
	bool bRet = true;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //����xml�ļ�
		return;
	}

	xml_node root = m_doc.child("root");  //���ڵ�
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		data.m_sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		CUIDataMgr::Instance().Register(data.m_sName,data);
		node = node.next_sibling();
	}
}


void CADRMarketTemperatureView::OnInitialUpdate()
{
	_LoadDataFromDB();
	_CreateUI();
	SetTimer(Timer_SaveData2UI,5000,NULL);
	__super::OnInitialUpdate();
}


int CADRMarketTemperatureView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CADRMarketTemperatureView::_CreateUI()
{
	CRect rc;
	GetClientRect(rc);
	map<string,CUIData>& mapUIName2Data = CUIDataMgr::Instance().GetUIData();
	for (map<string,CUIData>::iterator it = mapUIName2Data.begin();
		it != mapUIName2Data.end();it++)
	{
		string sCaption = it->first;
		CUIData& UIData = it->second;
		CDialogPlaceHolder* pHolder = Factory<CDialogPlaceHolder,string>::Instance().BuildProduct(UIData.m_strUIClassName);
		if(!pHolder) continue;
		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(UIData.m_strUIClassName);
		ASSERT(-1 != nIDD);
		pHolder->SetBusiness(UIData.m_sName);
		pHolder->SetLayout(UIData.m_strLayout);		

		pHolder->Create(nIDD,this);
		CRect rcDialog(rc.left + UIData.m_nLeft,rc.top + UIData.m_nTop,rc.left + UIData.m_nLeft + UIData.m_nWidth,rc.top + UIData.m_nHeight);
		pHolder->MoveWindow(rcDialog);	
		pHolder->ShowWindow(SW_SHOW);
		UIData.m_pWnd = pHolder;	
	}
}

void CADRMarketTemperatureView::_LoadDataFromDB()
{
	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");
	//��¼��ǰ��������
	CTradeDayPrimaryData::Instance().m_strTradeDay = strDate; 
	const int nWeekDay = CaculateWeekDay(dtNOw.GetYear(),dtNOw.GetMonth(),dtNOw.GetDay());
	if (Saturday == nWeekDay || Sunday == nWeekDay)
	{
		//return;
	}
	
	FieldValue v;
	v.SetDataType("string");
	v.SetValueString((LPSTR)(LPCTSTR)strDate);
	bool bFestival = CDBDataManager::Instance().RecordExists("Festival","Date","string",v);
	if (bFestival)
	{
		return;
	}

	const string strTableName = "Business";
	const string strFieldName = "BusinessName";
	string strSQL = "select * from ";
	strSQL += strTableName;
	CDataSet dsBusiness;
	int nRet = CDBDataManager::Instance().LoadData(strSQL, strTableName, dsBusiness);
	const int nBusinessCount = dsBusiness.Size();
	for (int i = 0; i < nBusinessCount;i++)
	{
		CRecord* pRecord = dsBusiness[i];
		if(!pRecord) continue;
		CField* pField = pRecord->GetField(strFieldName);
		if(!pField) continue;
		const string sBusinessName = pField->GetValueAsString();
		bool bExist = CDBDataManager::Instance().RecordExists(sBusinessName,"TradeDay","string",v);
		if (!bExist)
		{
			CDBDataManager::Instance().InsertDefaultRecord(sBusinessName);
		}
	}
}


void CADRMarketTemperatureView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == Timer_SaveData2UI)
	{
		CUIDataMgr::Instance().UpdateUI2DB();
	}

	__super::OnTimer(nIDEvent);
}