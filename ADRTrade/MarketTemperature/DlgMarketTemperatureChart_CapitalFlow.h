#pragma once
#include "DlgMarketTemperature_Chart.h"

class CDlgMarketTemperature_CapitalFlow : public CDlgMarketTemperature_Chart
{
	DECLARE_DYNAMIC(CDlgMarketTemperature_CapitalFlow)
public:
	CDlgMarketTemperature_CapitalFlow(CWnd* pParent = NULL);

protected:
	//´´½¨Í¼ÐÎUI
	virtual void _CreateChart();
};