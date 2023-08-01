#include "stdafx.h"
#include <string>
using namespace std;
#include "WeatherGlassChartPage.h"
#include "Factory.h"

IMPLEMENT_FACTORY(CChartPageBase,CChartPage_RiseFall, string,"RiseFall")
CChartPage_RiseFall::CChartPage_RiseFall(CWnd* pParent): CChartPageBase(pParent)
{
}

void CChartPage_RiseFall::AddTitles()
{
	CXTPChartTitle* pTitle = m_wndChartControl.GetContent()->GetTitles()->Add(new CXTPChartTitle());
	pTitle->SetText(_T("ÕÇµø±È"));
}

void CChartPage_RiseFall::AddSeries()
{
	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->Add(
		new CXTPChartSeries());

	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(1, 6500, 6500));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(2, 7200, 7200));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(3, 8200, 8200))->m_bSpecial = TRUE;
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(4, 9600, 9600));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(5, 11000, 11000));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(6, 11180, 11180));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(7, 12200, 12200));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(8, 10500, 10500));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(9, 9600, 9600));
}

void CChartPage_RiseFall::ShowChart()
{
	CString strAppearance = "Nature";
	m_wndChartControl.GetContent()->GetAppearance()->LoadAppearance(_T("CHART_APPEARANCE_")
		+ strAppearance);

	CString strPalette = "DANVILLE";
	m_wndChartControl.GetContent()->GetAppearance()->LoadPalette(_T("CHART_PALETTE_") + strPalette);

	m_wndChartControl.GetContent()->OnChartChanged();

	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->GetAt(0);
	pSeries->SetStyle(new CXTPChartSplineSeriesStyle());

	ShowControls(FALSE, FALSE);

	
}


IMPLEMENT_FACTORY(CChartPageBase,CChartPage_RiseFall2Limit, string,"RiseFall2Limit")
CChartPage_RiseFall2Limit::CChartPage_RiseFall2Limit(CWnd* pParent): CChartPageBase(pParent)
{
}

void CChartPage_RiseFall2Limit::AddTitles()
{
	CXTPChartTitle* pTitle = m_wndChartControl.GetContent()->GetTitles()->Add(new CXTPChartTitle());
	pTitle->SetText(_T("ÕÇÍ£/µøÍ£"));
}

void CChartPage_RiseFall2Limit::AddSeries()
{
	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->Add(
		new CXTPChartSeries());

	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(6, 4, 4));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(7, 3.3, 3.3));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(8, 2.8, 2.8))->m_bSpecial = TRUE;
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(9, 3.7, 3.7));
	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(10, 1.9, 1.9));
}

void CChartPage_RiseFall2Limit::ShowChart()
{
	CString strAppearance = "Nature";
	m_wndChartControl.GetContent()->GetAppearance()->LoadAppearance(_T("CHART_APPEARANCE_")
		+ strAppearance);

	CString strPalette = "DANVILLE";
	m_wndChartControl.GetContent()->GetAppearance()->LoadPalette(_T("CHART_PALETTE_") + strPalette);

	m_wndChartControl.GetContent()->OnChartChanged();

	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->GetAt(0);
	pSeries->SetStyle(new CXTPChartSplineSeriesStyle());

	ShowControls(FALSE, FALSE);
}

IMPLEMENT_FACTORY(CChartPageBase,CChartPage_TradeVolume, string,"TradeVolume")
CChartPage_TradeVolume::CChartPage_TradeVolume(CWnd* pParent): CChartPageBase(pParent)
{
}

void CChartPage_TradeVolume::AddTitles()
{

}

void CChartPage_TradeVolume::AddSeries()
{

}
void CChartPage_TradeVolume::ShowChart()
{

}



void CChartPage_ARiseFall::AddTitles()
{

}

void CChartPage_ARiseFall::AddSeries()
{


}

void CChartPage_ARiseFall::ShowChart()
{

}