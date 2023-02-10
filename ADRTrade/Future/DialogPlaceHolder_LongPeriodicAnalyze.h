#pragma once
#include "Tools/DialogPlaceHolder.h"

class CDialogFutureContract_LongPeriodicAnalyze : public CDialogPlaceHolder
{
public:
	CDialogFutureContract_LongPeriodicAnalyze(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_Common };

	virtual void UpdateUI2DB();
	virtual void UpdateDB2UI(CRecord* pRecord);

protected:
	virtual void _LoadTradeDayData2UI();
	string _QueryFutureContractName(const string& strTradeDay,const string& strFutureContractName);
};