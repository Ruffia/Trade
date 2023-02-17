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
#include "TradeDayPrimaryData.h"


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
			pDlgItem->SetRecordTime(sName);	
			pDlgItem->SetRecordTime_Old(sName);
		}

		pDlg->Create(nIDD,this);
		pDlg->MoveWindow(&rcHold);

		m_pTab->InsertItem(nCount,sName.c_str());

		m_vPage.push_back(pDlg);
		data.m_pWnd = pDlg;

		nCount++;
		node = node.next_sibling();
	}
	
	if (m_pTab->GetItemCount() > 0)
	{
		m_pTab->SetCurSel(0);
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

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += " TradeDay ";
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";
	sSQL += "order by RecordTime asc";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);

	const int nRecordCount = ds.Size();
	for (int i = 0; i < nRecordCount;i++)
	{
		CRecord* pRecord = ds[i];
		if(!pRecord) continue;

		CField* pField = pRecord->GetField("RecordTime");
		if(!pField) continue;
		string strRecordTime = pField->GetValueAsString();

		CDialogPlaceHolder* pPage = m_vPage[i];
		CDialogTabItem_MinorCycleAnalyze* pDlgItem = dynamic_cast<CDialogTabItem_MinorCycleAnalyze*>(pPage);
		if(!pDlgItem) continue;

		m_pTab->SetItemText(i,strRecordTime.c_str());
		pDlgItem->SetRecordTime(strRecordTime);
		pDlgItem->SetRecordTime_Old(strRecordTime);
		pDlgItem->UpdateDB2UI(ds,i);	
	}
}


void CCustomTabCtrlDlg_DailyMinorCycleAnalyze::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	const int nPageCount = m_vPage.size();
	for (int i = 0; i < nPageCount;i++)
	{
		CDialogPlaceHolder* pHolder = m_vPage[i];
		if(!pHolder) continue;
		CDialogTabItem_MinorCycleAnalyze* pDlgItem = dynamic_cast<CDialogTabItem_MinorCycleAnalyze*>(pHolder);
		if(!pDlgItem) continue;

		CString strText = "";
		int nRet = m_pTab->GetItemText(i,strText);
		string strRecordTime = strText.GetBuffer();
		if (!pDlgItem->Need2UpdateRecordTime())
		{
			if (strRecordTime != pDlgItem->GetRecordTime())
			{
				pDlgItem->SetRecordTime_Old(pDlgItem->GetRecordTime());
				pDlgItem->SetRecordTime(strRecordTime);
				pDlgItem->SetNeed2UpdateRecordTime(true);
			}
		}

 		pDlgItem->UpdateUI2DB();	
	}
}