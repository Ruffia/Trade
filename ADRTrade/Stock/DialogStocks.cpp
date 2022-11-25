#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStocks.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgStocks,string,"CDlgStocks")
CDlgStocks::CDlgStocks()
{
	CDialogIDMgr::Instance().Register("CDlgStocks",CDlgStocks::IDD);
}