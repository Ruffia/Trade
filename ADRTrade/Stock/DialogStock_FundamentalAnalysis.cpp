#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStock_FundamentalAnalysis.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogStock_FundamentalAnalysis,string,"CDialogStock_FundamentalAnalysis")
CDialogStock_FundamentalAnalysis::CDialogStock_FundamentalAnalysis()
{
	CDialogIDMgr::Instance().Register("CDialogStock_FundamentalAnalysis",CDialogStock_FundamentalAnalysis::IDD);
}