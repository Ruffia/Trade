#pragma once
#include "Tools/DialogPlaceHolderDB.h"

class CDialogFutureContract_LongPeriodicAnalyze : public CDialogPlaceHolderBusiness
{
public:
	CDialogFutureContract_LongPeriodicAnalyze(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_Common };

	virtual void UpdateUI2DB();
	virtual void UpdateDB2UI(CDataSet& ds,int index = 0);

protected:
	virtual void _LoadData2UI();
	string _QueryFutureContractName(const string& strTradeDay,const string& strFutureContractName);
};