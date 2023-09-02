#pragma once
#include "Tools/DialogPlaceHolder.h"

class CDialogFutureContract_DailyTraceConflict : public CDialogPlaceHolder
{
public:
	CDialogFutureContract_DailyTraceConflict(CWnd* pParent = NULL);
	enum { IDD = IDD_DLG_Common };

	virtual void UpdateUI2DB();
	virtual void UpdateDB2UI(CDataSet& ds,int index = 0);

protected:
	virtual void _LoadData2UI();
};