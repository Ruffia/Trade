// DlgSEMLog.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgPopup_StockPool.h"
#include "../Common/Factory.h"
#include "DialogIDManager.h"

IMPLEMENT_FACTORY(CDialogPopup,CDlgPopup_StockPool,string,"CDlgPopup_StockPool")
CDlgPopup_StockPool::CDlgPopup_StockPool()
{
	CDialogIDMgr::Instance().Register("CDlgPopup_StockPool",CDlgPopup_StockPool::IDD);
}