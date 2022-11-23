#include "StdAfx.h"
#include <vector>
using namespace std;
#include "DialogStockPool_StockTree.h"
#include "Factory.h"
#include "DialogIDManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_FACTORY(CDialog,CDialogStockPool_StockTree,string,"CDialogStockPool_StockTree")
CDialogStockPool_StockTree::CDialogStockPool_StockTree()
{
	CDialogIDMgr::Instance().Register("CDialogStockPool_StockTree",CDialogStockPool_StockTree::IDD);
}