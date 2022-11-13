#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogFutureContract_MinorCycleAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogFutureContract_MinorCycleAnalyze,string,"CDialogFutureContract_MinorCycleAnalyze")
CDialogFutureContract_MinorCycleAnalyze::CDialogFutureContract_MinorCycleAnalyze()
{
	CDialogIDMgr::Instance().Register("CDialogFutureContract_MinorCycleAnalyze",CDialogFutureContract_MinorCycleAnalyze::IDD);
}
