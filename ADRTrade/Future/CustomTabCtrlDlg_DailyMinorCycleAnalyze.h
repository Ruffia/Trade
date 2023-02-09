#pragma once
#include "Tools/CustomTabCtrlDlg.h"

class CCustomTabCtrlDlg_DailyMinorCycleAnalyze : public CCustomTabCtrlDlg
{
public:
	CCustomTabCtrlDlg_DailyMinorCycleAnalyze(CWnd* pParent = NULL);

	enum { IDD = IDD_DLG_Common };

protected:
	virtual void _InitPage(CRect& rcTab); 
	virtual void _LoadTradeDayData2UI();
};
