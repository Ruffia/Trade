#pragma once
#include "Tools/DialogPlaceHolderDB.h"
#include "DialogPlaceHolder_MinorCycleAnalyze.h"
#include "Tools/CustomTabCtrlDlg.h"
#include "../Common/Factory.h"

DefineClass(CDailyMacroEnvironmentDlg,CDialogPlaceHolderBusiness,IDD_DLG_Common)

DefineClass(CDialogFutureContract_IntradayTrade,CDialogPlaceHolderBusiness,IDD_DLG_Common)

DefineClass(CDialogDailyTrades_SingleTradeTrack,CDialogPlaceHolderBusiness,IDD_DLG_Common)

DefineClass(CDialogDailyTradeSummarize,CDialogPlaceHolderBusiness,IDD_DLG_Common)

DefineClass(CDialogFutureContract_MinorCycleAnalyze,CDialogTabItem_MinorCycleAnalyze,IDD_DLG_Common)
