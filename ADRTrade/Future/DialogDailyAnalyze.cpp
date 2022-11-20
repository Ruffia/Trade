#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogDailyAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"


IMPLEMENT_FACTORY(CDialog,CDlgDailyAnalyze,string,"CDlgDailyAnalyze")
CDlgDailyAnalyze::CDlgDailyAnalyze()
{
	CDialogIDMgr::Instance().Register("CDlgDailyAnalyze",CDlgDailyAnalyze::IDD);
}