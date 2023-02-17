#pragma once
#include "Tools/DialogPlaceHolder.h"

class CDialogFutureContract_DailyTraceEvidence : public CDialogPlaceHolder
{
public:
	CDialogFutureContract_DailyTraceEvidence(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_Common };

	virtual void UpdateUI2DB();
	virtual void UpdateDB2UI(CDataSet& ds,int index = 0);

protected:
	virtual void _InitLayOut();
	virtual void _LoadTradeDayData2UI();
};