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
	//��ʼ��Layout
	virtual void _InitLayOut();
	//���������������Layout
	virtual void _DesignLayout();
	//����ͼ��UI
	virtual void _CreateChart();

protected:
	CChartCtrl* m_pChartCtrl;
};