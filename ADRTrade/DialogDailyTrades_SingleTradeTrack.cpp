#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyTrades_SingleTradeTrack.h"
#include "Factory.h"
#include "DialogIDManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogDailyTrades_SingleTradeTrack,string,"CDialogDailyTrades_SingleTradeTrack")
CDialogDailyTrades_SingleTradeTrack::CDialogDailyTrades_SingleTradeTrack()
{
	CDialogIDMgr::Instance().Register("CDialogDailyTrades_SingleTradeTrack",CDialogDailyTrades_SingleTradeTrack::IDD);
}
