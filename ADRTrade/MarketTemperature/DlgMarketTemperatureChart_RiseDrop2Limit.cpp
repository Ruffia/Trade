#include "stdafx.h"
#include "DlgMarketTemperatureChart_RiseDrop2Limit.h"
#include "../Common/Factory.h"
#include "../High-Speed-Charting-Control/ChartBarSerie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialogPlaceHolder,CDlgMarketTemperature_RiseDrop2Limit, string,"CDlgMarketTemperature_RiseDrop2Limit")

IMPLEMENT_DYNAMIC(CDlgMarketTemperature_RiseDrop2Limit, CDlgMarketTemperature_Chart)
CDlgMarketTemperature_RiseDrop2Limit::CDlgMarketTemperature_RiseDrop2Limit(CWnd* pParent /*=NULL*/)
	: CDlgMarketTemperature_Chart(pParent)
{
	CDialogIDMgr::Instance().Register("CDlgMarketTemperature_RiseDrop2Limit",CDlgMarketTemperature_RiseDrop2Limit::IDD);
}


void CDlgMarketTemperature_RiseDrop2Limit::_CreateChart() 
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
	pLeftAxis->SetMinMax(0,1000);
	//pLeftAxis->GetLabel()->SetText(_T("Units sold"));// Create the right axis and configure it properly
	CChartStandardAxis* pRightAxis = m_pChartCtrl->CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetVisible(true);
	//pRightAxis->GetLabel()->SetText(_T("Income (kEuros)"));
	pRightAxis->SetMinMax(0,1000);
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

	int lowIndex = -1;
	int lowVal = 999;
	for (int i=0;i<23;i++)
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
	}
	pBarSeries1->SetColor(RGB(255,0,0));
	pBarSeries1->SetName(_T("Rise 2 Upper Limit"));

	pBarSeries2->SetColor(RGB(68,68,255));
	pBarSeries2->SetGradient(RGB(200,200,255),gtVerticalDouble);
	pBarSeries2->SetName(_T("Drop 2 Lower Limit"));
	pBarSeries2->SetBorderColor(RGB(0,0,255));
	pBarSeries2->SetBorderWidth(2);
}