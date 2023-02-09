#include "stdafx.h"
#include "ADRTrade.h"
#include "afxdialogex.h"
#include "Util.h"
#include "UIData.h"
#include "Factory.h"
#include "DialogIDManager.h"
#include "Tools/DialogPlaceHolder.h"
#include "CustomTabCtrlDlg_DailyMinorCycleAnalyze.h"
#include "DBDataManager.h"
#include "Tools/CustomTabCtrlDlg.h"
#include "FutureContract_TradeDialogs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialogPlaceHolder,CCustomTabCtrlDlg_DailyMinorCycleAnalyze, string,"CCustomTabCtrlDlg_DailyMinorCycleAnalyze")
CCustomTabCtrlDlg_DailyMinorCycleAnalyze::CCustomTabCtrlDlg_DailyMinorCycleAnalyze(CWnd* pParent /*=NULL*/)
	: CCustomTabCtrlDlg(pParent)
{
	CDialogIDMgr::Instance().Register("CCustomTabCtrlDlg_DailyMinorCycleAnalyze",CCustomTabCtrlDlg_DailyMinorCycleAnalyze::IDD);
}


void CCustomTabCtrlDlg_DailyMinorCycleAnalyze::_InitPage(CRect& rcTab) 
{
	string sFileName = GetModulePath() + "/UI/" + m_sLayout;
	if (!m_doc.load_file(sFileName.c_str())) 
	{ //加载xml文件
		return;
	}

	int nCount = 0;
	xml_node root = m_doc.child("root");  //根节点
	xml_node nodeLayout = root.child("Layout");
	xml_node node = nodeLayout.first_child();
	while (!node.empty())
	{
		CUIData data;
		const string sName = node.attribute("Name").as_string("");
		data.m_strUIClassName = node.attribute("ClassName").as_string("");
		data.m_strLayout = node.attribute("Layout").as_string("");
		data.m_nID = node.attribute("ID").as_int();
		data.m_nLeft = node.attribute("Left").as_int();
		data.m_nTop = node.attribute("Top").as_int();
		data.m_nWidth = node.attribute("Width").as_int();
		data.m_nHeight = node.attribute("Height").as_int();
		string sTitle = node.attribute("Title").as_string("");

		//设定在Tab内显示的范围
		CRect rcHold;
		rcHold.top = rcTab.top + data.m_nTop;
		rcHold.left = rcTab.left;
		rcHold.right = rcTab.right;
		rcHold.bottom = rcTab.bottom;
	
		CDialogPlaceHolder* pDlg = Factory<CDialogPlaceHolder,string>::Instance().BuildProduct(data.m_strUIClassName);
		if(!pDlg) continue;

		pDlg->SetBusiness(m_sBusiness);
		pDlg->SetLayout(data.m_strLayout);

		const int nIDD = CDialogIDMgr::Instance().GetDialogResourceID(data.m_strUIClassName);
		ASSERT(-1 != nIDD);
		CDialogTabItem_MinorCycleAnalyze* pDlgItem = dynamic_cast<CDialogTabItem_MinorCycleAnalyze*>(pDlg);
		if (pDlgItem)
		{
			pDlgItem->SetItem(sName);		
		}

		pDlg->Create(nIDD,this);
		pDlg->MoveWindow(&rcHold);

		m_pTab->InsertItem(nCount,sName.c_str());

		m_vPage.push_back(pDlg);
		data.m_pWnd = pDlg;

		nCount++;
		node = node.next_sibling();
	}

}


void CCustomTabCtrlDlg_DailyMinorCycleAnalyze::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	COleDateTime dtNOw = COleDateTime::GetCurrentTime();
	CString strDate = dtNOw.Format("%Y-%m-%d");

	FieldValue vKey;
	vKey.SetDataType("string");
	vKey.SetValueString(strDate);

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += " TradeDay ";
	sSQL += " = '";
	sSQL += vKey.GetValueAsString();
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);

	const int nRecordCount = ds.Size();
	for (int i = 0; i < nRecordCount;i++)
	{
		CRecord* pRecord = ds[i];
		if(!pRecord) return;

		CDialogPlaceHolder* pPage = m_vPage[i];
		if(pPage)
		{
			pPage->UpdateDB2UI(pRecord);
		}	
	}
}