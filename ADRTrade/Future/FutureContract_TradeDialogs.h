#pragma once
#include "Tools/DialogPlaceHolder.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "Tools/CustomTabCtrlDlg.h"
#include "../Common/Factory.h"

DefineClass(CDailyMacroEnvironmentDlg,CDialogPlaceHolder,IDD_DLG_Common)

DefineClass(CDialogFutureContract_IntradayTrade,CDialogPlaceHolder,IDD_DLG_Common)

DefineClass(CDialogDailyTrades_SingleTradeTrack,CDialogPlaceHolder,IDD_DLG_Common)

DefineClass(CDialogDailyTradeSummarize,CDialogPlaceHolder,IDD_DLG_Common)

DefineClass(CDialogFutureContract_MinorCycleAnalyze,CDialogTabItem_MinorCycleAnalyze,IDD_DLG_Common)
