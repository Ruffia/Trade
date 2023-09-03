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

IMPLEMENT_FACTORY(CDialogPlaceHolderBusiness,CDlgMarketTemperature_Chart, string,"CDlgMarketTemperature_Chart")

IMPLEMENT_DYNAMIC(CDlgMarketTemperature_Chart, CDialogPlaceHolderBusiness)
CDlgMarketTemperature_Chart::CDlgMarketTemperature_Chart(CWnd* pParent /*=NULL*/)
	: CDialogPlaceHolderBusiness(pParent)
{
	m_pChartCtrl = NULL;
	CDialogIDMgr::Instance().Register("CDlgMarketTemperature_Chart",CDlgMarketTemperature_Chart::IDD);
}


BOOL CDlgMarketTemperature_Chart::OnInitDialog()
{
	CDialog::OnInitDialog();
	_InitLayOut();
	_LoadData2UI();
	return TRUE;
}


void CDlgMarketTemperature_Chart::_InitLayOut()
{
	CRect rc;
	GetClientRect(rc);
	CRect rcChart(rc.left + 10, rc.top + 10,rc.Width() - 20,rc.Height() - 50);
	m_pChartCtrl = new CChartCtrl();
	m_pChartCtrl->Create(this,rcChart,3000,WS_CLIPCHILDREN | WS_TABSTOP);
	m_pChartCtrl->ShowWindow(SW_SHOW);

	_CreateChart();
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



void CDlgMarketTemperature_Chart::_CreateChart() 
{
	srand((unsigned int)time(NULL));
	// Disable the refresh
	COleDateTime Min(2023,1,1,0,0,0);
	COleDateTime Max(2023,1,25,0,0,0);// Create the bottom axis and configure it properly
	CChartDateTimeAxis* pBottomAxis = m_pChartCtrl->CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(Min,Max);
	pBottomAxis->SetDiscrete(true);
	pBottomAxis->SetTickIncrement(false,CChartDateTimeAxis::tiDay,1);
	pBottomAxis->SetTickLabelFormat(false,_T("%d/%m"));// Create the left axis and configure it properly
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
	CChartBarSerie* pBarSeries3 = m_pChartCtrl->CreateBarSerie();
	CChartLineSerie* pLineSeries = m_pChartCtrl->CreateLineSerie(false,true);
	int lowIndex = -1;
	int lowVal = 999;
	for (int i=0;i<23;i++)
	{
		COleDateTime TimeVal(2023,1,i + 1,0,0,0);
		int DesktopVal = 20 + rand()%(100-30);
		pBarSeries1->AddPoint(TimeVal,DesktopVal);
		int LaptopVal = 10 + rand()%(80-20);
		pBarSeries2->AddPoint(TimeVal,LaptopVal);
		int nTradeVolume = rand()%(1-20);
		pBarSeries3->AddPoint(TimeVal,nTradeVolume);
		int Income = DesktopVal + LaptopVal*1.5;
		if (Income < lowVal)
		{
			lowVal = Income;
			lowIndex = i;
		}
		//pLineSeries->AddPoint(TimeVal,Income);
	}
	pBarSeries1->SetColor(RGB(255,0,0));
	pBarSeries1->SetName(_T("Temperature"));

	pBarSeries2->SetColor(RGB(68,68,255));
	pBarSeries2->SetGradient(RGB(200,200,255),gtVerticalDouble);
	pBarSeries2->SetName(_T("Trade Volume(ЧЇвк)"));
	pBarSeries2->SetBorderColor(RGB(0,0,255));
	pBarSeries2->SetBorderWidth(2);

	pBarSeries3->SetColor(RGB(68,0,0));
	pBarSeries3->SetGradient(RGB(68,200,0),gtVerticalDouble);
	pBarSeries3->SetName(_T("Capital Flow(ЪЎвк)"));
	pBarSeries3->SetBorderColor(RGB(0,200,0));
	pBarSeries3->SetBorderWidth(2);

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