#pragma once
#include "DlgMarketTemperature_Chart.h"

class CDlgMarketTemperature_RiseRatioTradeVolume : public CDlgMarketTemperature_Chart
{
	DECLARE_DYNAMIC(CDlgMarketTemperature_RiseRatioTradeVolume)
public:
	CDlgMarketTemperature_RiseRatioTradeVolume(CWnd* pParent = NULL);

protected:
	//����ͼ��UI
	virtual void _CreateChart();
};