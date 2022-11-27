#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStockPool_Stocks.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgStockPool_Stocks,string,"CDlgStockPool_Stocks")
CDlgStockPool_Stocks::CDlgStockPool_Stocks()
{
	CDialogIDMgr::Instance().Register("CDlgStockPool_Stocks",CDlgStockPool_Stocks::IDD);
}