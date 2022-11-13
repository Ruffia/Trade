#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyTrades.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgDailyTrades,string,"CDlgDailyTrades")
CDlgDailyTrades::CDlgDailyTrades()
{
	CDialogIDMgr::Instance().Register("CDlgDailyTrades",CDlgDailyTrades::IDD);
}