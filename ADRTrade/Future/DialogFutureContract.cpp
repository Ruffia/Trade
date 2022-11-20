#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgDailyFutureContract,string,"CDlgDailyFutureContract")
CDlgDailyFutureContract::CDlgDailyFutureContract()
{
	CDialogIDMgr::Instance().Register("CDlgDailyFutureContract",CDlgDailyFutureContract::IDD);
}