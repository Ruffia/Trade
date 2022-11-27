#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStockPool_Stock.h"
#include "Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialog,CDlgStockPool_Stock,string,"CDlgStockPool_Stock")
CDlgStockPool_Stock::CDlgStockPool_Stock()
{
	CDialogIDMgr::Instance().Register("CDlgStockPool_Stock",CDlgStockPool_Stock::IDD);
}