#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStock_1DayAnalyze.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogStock_1DayAnalyze,string,"CDialogStock_1DayAnalyze")
CDialogStock_1DayAnalyze::CDialogStock_1DayAnalyze()
{
	CDialogIDMgr::Instance().Register("CDialogStock_1DayAnalyze",CDialogStock_1DayAnalyze::IDD);
}
