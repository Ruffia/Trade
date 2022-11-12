#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract_LongPeriodicAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogFutureContract_LongPeriodicAnalyze,string,"CDialogFutureContract_LongPeriodicAnalyze")
CDialogFutureContract_LongPeriodicAnalyze::CDialogFutureContract_LongPeriodicAnalyze()
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_LongPeriodicAnalyze",CDialogFutureContract_LongPeriodicAnalyze::IDD);
}