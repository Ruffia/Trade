#include "stdafx.h"
#include "DlgMarketTemperature_Chart.h"
#include "../Common/Factory.h"
#include "DBDataManager.h"
#include "Util.h"
#include "Tools/CollectiveComponentProvider.h"
#include "Tools/BusinessEdit.h"
#include "Tools/BusinessComboBox.h"
#include "Tools/BusinessCheckBox.h"
#include "Tools/EditTreeCtrlEx.h"
#include "PrimaryKeyRule.h"
#include "TradeDayPrimaryData.h"
#include "../High-Speed-Charting-Control/ChartBarSerie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialogPlaceHolder,CDlgMarketTemperature_Chart, string,"CDlgMarketTemperature_Chart")

IMPLEMENT_DYNAMIC(CDlgMarketTemperature_Chart, CDialogPlaceHolder)
CDlgMarketTemperature_Chart::CDlgMarketTemperature_Chart(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolder(pParent)
{
	m_strRecordTime = "";
	m_strRecordTime_Old = "";
	m_bNeed2UpdateRecordTime = false;
	m_pChartCtrl = NULL;
	CDialogIDMgr::Instance().Register("CDlgMarketTemperature_Chart",CDlgMarketTemperature_Chart::IDD);
}


BOOL CDlgMarketTemperature_Chart::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	_LoadTradeDayData2UI();
	return TRUE;
}


void CDlgMarketTemperature_Chart::_LoadTradeDayData2UI()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if (!bExists) return;

	vector<CFieldDesc*> vFieldDesc;
	CDBDataManager::Instance().GetFieldMetaData(m_sBusiness,vFieldDesc);

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	const string strTradeDay = "TradeDay";
	const string strRecordTime = "RecordTime";
	CFieldDesc* pTradeDayDesc = NULL;
	CFieldDesc* pRecordTimeDesc = NULL;
	for (int i = 0;i < vPrimaryKey.size();i++)
	{
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if (pFieldDesc && pFieldDesc->m_strFieldName.find(strTradeDay) != string::npos)
		{
			pTradeDayDesc = pFieldDesc;
		}
		else if (pFieldDesc && pFieldDesc->m_strFieldName.find(strRecordTime) != string::npos)
		{
			pRecordTimeDesc = pFieldDesc;
		}
	}

	if(!pTradeDayDesc || !pRecordTimeDesc) return;

	string sSQL = "select * from ";
	sSQL += m_sBusiness;
	sSQL += " where ";
	sSQL += pTradeDayDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strTradeDay;
	sSQL += "'";
	sSQL += " and ";
	sSQL += pRecordTimeDesc->m_strFieldName;
	sSQL += " = '";
	sSQL += m_strRecordTime;
	sSQL += "'";

	CDataSet ds;
	CDBDataManager::Instance().LoadData(sSQL,m_sBusiness,ds);

	UpdateDB2UI(ds);
}


void CDlgMarketTemperature_Chart::UpdateUI2DB()
{
	bool bExists = _CheckExistsTradeDayRecord();
	if(!bExists) return;

	map<string,CFieldDesc*>& mapTableName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_sBusiness);

	string sSQL = "update ";
	sSQL += m_sBusiness;
	sSQL += " set ";
	sSQL += " FutureContractName = '";
	sSQL += CTradeDayPrimaryData::Instance().m_strFutureContractName;
	sSQL += "'";
	sSQL += ",";
	sSQL += " RecordTime = '";
	sSQL += m_strRecordTime;
	sSQL += "'";
	sSQL += ",";

	int nUIControlCount = 0;
	for(map<string,CWnd*>::iterator it = m_mapBusiness2Control.begin();
		it != m_mapBusiness2Control.end();it++)
	{
		CWnd* pWnd = it->second;
		nUIControlCount++;

		string sBusinessField = "";
		//截至目前，有3中Business 字段类型
		CBusinessEdit* pBusinessControl = NULL;
		CBusinessComboBox* pComboBoxControl = NULL;
		CBusinessCheckBox* pCheckBoxControl = NULL;

		const int nBussinessType = 3;
		bool ControlType[3] = {false,false,false}; 
		const type_info &typeInfo = typeid(*pWnd);
		string sTypeName = typeInfo.raw_name();
		if (string::npos != sTypeName.find("CBusinessEdit"))
		{
			ControlType[Business_Edit] = true;
			pBusinessControl = dynamic_cast<CBusinessEdit*>(pWnd);
			if(!pBusinessControl) continue;
			sBusinessField = pBusinessControl->m_sBusinessField;
		}
		else if (string::npos != sTypeName.find("CBusinessComboBox"))
		{
			ControlType[Business_ComboBox] = true;
			pComboBoxControl = dynamic_cast<CBusinessComboBox*>(pWnd);
			if(!pComboBoxControl) continue;
			sBusinessField = pComboBoxControl->m_sBusinessField;
		}
		else if (string::npos != sTypeName.find("CBusinessCheckBox"))
		{
			ControlType[Business_CheckBox] = true;
			pCheckBoxControl = dynamic_cast<CBusinessCheckBox*>(pWnd);
			if(!pCheckBoxControl) continue;
			sBusinessField = pCheckBoxControl->m_sBusinessField;
		}

		map<string,CFieldDesc*>::iterator itFieldDesc = mapTableName2FieldDesc.find(sBusinessField);
		if(mapTableName2FieldDesc.end() == itFieldDesc) continue;

		CFieldDesc* pFieldDesc = itFieldDesc->second;
		if (!pFieldDesc) continue;

		char szValue[512] = {0}; 
		pWnd->GetWindowTextA(szValue,512);

		char szSQL[512] = {0}; 
		if (pFieldDesc->m_strDataType == "string")
		{
			sprintf_s(szSQL,512," %s = '%s' ",sBusinessField.c_str(),szValue);
		}
		else if (pFieldDesc->m_strDataType == "float" )
		{
			sprintf_s(szSQL,512," %s = %s ", sBusinessField.c_str(), strcmp(szValue,"")?szValue:"0");
		}
		else if( pFieldDesc->m_strDataType == "integer" || pFieldDesc->m_strDataType == "int")
		{
			if (ControlType[Business_ComboBox])
			{
				int nValue = -1;
				bool bFindValue = pComboBoxControl->GetValueByTranslation(szValue,nValue);
				sprintf_s(szSQL,512," %s = %d ", sBusinessField.c_str(), nValue);
			}
			else if (ControlType[Business_CheckBox])
			{
				int nValue = pCheckBoxControl->GetCheck();
				sprintf_s(szSQL,512," %s = %d ", sBusinessField.c_str(), nValue);
			}
			else
			{
				sprintf_s(szSQL,512," %s = %s ", sBusinessField.c_str(), strcmp(szValue,"")?szValue:"0");
			}		
		}

		sSQL += szSQL;
		if (m_mapBusiness2Control.size() != nUIControlCount)
		{
			sSQL += ",";
		}
		else
		{
			sSQL += " ";
		}

	}

	sSQL += " where ";

	vector<CFieldDesc*> vPrimaryKey;
	CDBDataManager::Instance().GetPrimaryKey(m_sBusiness,vPrimaryKey);

	string strPrimaryKeyClause = "";
	for (int i = 0; i < vPrimaryKey.size();i++)
	{
		char sz[256] = {0};
		CFieldDesc* pFieldDesc = vPrimaryKey[i];
		if(!pFieldDesc) continue;

		if (pFieldDesc->m_strFieldName.find("TradeDay") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strTradeDay.c_str());
		}
		else if (pFieldDesc->m_strFieldName.find("FutureContractName") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),CTradeDayPrimaryData::Instance().m_strFutureContractName_LastTime.c_str());		
		}
		else if (pFieldDesc->m_strFieldName.find("RecordTime") != string::npos)
		{
			sprintf_s(sz,256,"%s = '%s'",pFieldDesc->m_strFieldName.c_str(),m_strRecordTime_Old.c_str());
			if (m_bNeed2UpdateRecordTime)
			{
				m_bNeed2UpdateRecordTime = false;
			}
		}

		if (vPrimaryKey.size() - 1 != i)
		{
			strPrimaryKeyClause += sz;
			strPrimaryKeyClause += " and ";
		}
		else
		{
			strPrimaryKeyClause += sz;
		}	
	}

	sSQL += strPrimaryKeyClause;
	CDBDataManager::Instance().Exec(sSQL);
}


void CDlgMarketTemperature_Chart::_InitLayOut()
{
	CRect rc;
	GetClientRect(rc);
	CRect rcChart(rc.left + 10, rc.top + 10,rc.Width() - 20,rc.Height() - 50);
	m_pChartCtrl = new CChartCtrl();
	m_pChartCtrl->Create(this,rcChart,3000,WS_CLIPCHILDREN | WS_TABSTOP);
	m_pChartCtrl->ShowWindow(SW_SHOW);

	srand((unsigned int)time(NULL));
	// Disable the refresh
	COleDateTime Min(2023,1,1,0,0,0);
	COleDateTime Max(2023,1,30,0,0,0);// Create the bottom axis and configure it properly
	CChartDateTimeAxis* pBottomAxis = m_pChartCtrl->CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(Min,Max);
	pBottomAxis->SetDiscrete(true);
	pBottomAxis->SetTickIncrement(false,CChartDateTimeAxis::tiDay,1);
	pBottomAxis->SetTickLabelFormat(false,_T("%b"));// Create the left axis and configure it properly
	CChartStandardAxis* pLeftAxis = m_pChartCtrl->CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0,100);
	//pLeftAxis->GetLabel()->SetText(_T("Units sold"));// Create the right axis and configure it properly
	CChartStandardAxis* pRightAxis = m_pChartCtrl->CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetVisible(true);
	//pRightAxis->GetLabel()->SetText(_T("Income (kEuros)"));
	pRightAxis->SetMinMax(0,20);
	// Configure the legend
	m_pChartCtrl->GetLegend()->SetVisible(true);
	m_pChartCtrl->GetLegend()->SetHorizontalMode(true);
	m_pChartCtrl->GetLegend()->UndockLegend(80,50);// Add text to the title and set the font & color
	m_pChartCtrl->GetTitle()->AddString(_T("Market Temperature"));
	CChartFont titleFont;
	titleFont.SetFont(_T("Arial Black"),120,true,false,true);
	m_pChartCtrl->GetTitle()->SetFont(titleFont);
	m_pChartCtrl->GetTitle()->SetColor(RGB(0,0,128));// Sets a gradient background
	m_pChartCtrl->SetBackGradient(RGB(255,255,255),RGB(150,150,255),gtVertical);
	// Create two bar series and a line series and populate them with data
	CChartBarSerie* pBarSeries1 = m_pChartCtrl->CreateBarSerie();
	CChartBarSerie* pBarSeries2 = m_pChartCtrl->CreateBarSerie();
	CChartLineSerie* pLineSeries = m_pChartCtrl->CreateLineSerie(false,true);
	int lowIndex = -1;
	int lowVal = 999;
	for (int i=0;i<30;i++)
	{
		COleDateTime TimeVal(2023,1,i + 1,0,0,0);
		int DesktopVal = 20 + rand()%(100-30);
		pBarSeries1->AddPoint(TimeVal,DesktopVal);
		int LaptopVal = 10 + rand()%(80-20);
		pBarSeries2->AddPoint(TimeVal,LaptopVal);
		int Income = DesktopVal + LaptopVal*1.5;
		if (Income < lowVal)
		{
			lowVal = Income;
			lowIndex = i;
		}
		//pLineSeries->AddPoint(TimeVal,Income);
	}
	pBarSeries1->SetColor(RGB(255,0,0));
	pBarSeries1->SetName(_T("Desktops"));
	pBarSeries2->SetColor(RGB(68,68,255));
	pBarSeries2->SetGradient(RGB(200,200,255),gtVerticalDouble);
	pBarSeries2->SetName(_T("Laptops"));
	pBarSeries2->SetBorderColor(RGB(0,0,255));
	pBarSeries2->SetBorderWidth(2);
	//pLineSeries->SetColor(RGB(0,180,0));
	//pLineSeries->SetName(_T("Total income"));
	//pLineSeries->SetWidth(2);
	//pLineSeries->EnableShadow(true);
	// Add a label on the line series.
	TChartStringStream labelStream;
	labelStream << _T("Min income: ") << lowVal;
	//CChartBalloonLabel<SChartXYPoint>* pLabel = pLineSeries->CreateBalloonLabel(lowIndex, labelStream.str() + _T(" kEuros"));
	//CChartFont labelFont;
	//labelFont.SetFont(_T("Microsoft Sans Serif"),100,false,true,false);
	//pLabel->SetFont(labelFont);
	// Re enable the refreshm_pChartCtrl->EnableRefresh(true);

}


void CDlgMarketTemperature_Chart::_DesignLayout()
{
	CRect rc;
	GetClientRect(rc);
	CRect rcChart(rc.left + 10, rc.top + 10,rc.Width() - 20,rc.Height() - 50);

	if (m_pChartCtrl && m_pChartCtrl->m_hWnd)
	{
		m_pChartCtrl->MoveWindow(rcChart);
	}	
}