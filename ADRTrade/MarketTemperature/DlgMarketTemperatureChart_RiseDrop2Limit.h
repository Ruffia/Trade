#pragma once
#include "DlgMarketTemperature_Chart.h"

class CDlgMarketTemperature_RiseDrop2Limit : public CDlgMarketTemperature_Chart
{
	DECLARE_DYNAMIC(CDlgMarketTemperature_RiseDrop2Limit)
public:
	CDlgMarketTemperature_RiseDrop2Limit(CWnd* pParent = NULL);

protected:
	//����ͼ��UI
	virtual void _CreateChart();
};