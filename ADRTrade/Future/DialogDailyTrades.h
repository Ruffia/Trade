#pragma once
#include "Tools/DialogPlaceHolder.h"
#include "Factory.h"

//每日交易
class CDlgDailyTrades :public CDialogPlaceHolder
{
public:
	CDlgDailyTrades();
	enum { IDD = IDD_ChildDLG_Tab_DailyAnalyze };
};