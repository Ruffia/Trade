#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyTradeSummarize.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogDailyTradeSummarize,string,"CDialogDailyTradeSummarize")
CDialogDailyTradeSummarize::CDialogDailyTradeSummarize()
{
	CDialogIDMgr::Instance().Register("CDialogDailyTradeSummarize",CDialogDailyTradeSummarize::IDD);
}