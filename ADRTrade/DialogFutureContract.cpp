#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDlgDailyFutureContract,string,"CDlgDailyFutureContract")
CDlgDailyFutureContract::CDlgDailyFutureContract()
{
	CDialogIDMgr::Instance().Register("CDlgDailyFutureContract",CDlgDailyFutureContract::IDD);
}