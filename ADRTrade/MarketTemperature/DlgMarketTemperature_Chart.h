#pragma once
#include "../Tools/DialogPlaceHolderDB.h"
#include "../High-Speed-Charting-Control/ChartCtrl.h"

class CDlgMarketTemperature_Chart : public CDialogPlaceHolderBusiness
{
	DECLARE_DYNAMIC(CDlgMarketTemperature_Chart)
public:
	CDlgMarketTemperature_Chart(CWnd* pParent = NULL);

protected:
	BOOL OnInitDialog();
	//初始化Layout
	virtual void _InitLayOut();
	//根据缩放情况调整Layout
	virtual void _DesignLayout();
	//创建图形UI
	virtual void _CreateChart();

protected:
	CChartCtrl* m_pChartCtrl;
};