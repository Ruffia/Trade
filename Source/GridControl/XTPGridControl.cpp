// XTPGridControl.cpp : implementation of the CXTPGridControl class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPImageManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "Behavior/XTPGridBehavior.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridSelectedRows.h"
#include "GridControl/XTPGridTip.h"

#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridNavigator.h"
#include "GridControl/XTPGridSubListControl.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "GridControl/XTPGridRecordItemControls.h"
#include "GridControl/XTPGridHyperlink.h"
#include "GridControl/XTPGridRecordItemRange.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridSections.h"
#include "GridControl/XTPGridBorder.h"
#include "GridControl/XTPGridThickness.h"
#include "GridControl/XTPGridHitTestInfo.h"
#include "GridControl/XTPGridADO.h"
#include "GridControl/XTPGridDataManager.h"
#include "GridControl/XTPGridView.h"
#include "ItemTypes/XTPGridRecordItemPreview.h"
#include "ItemTypes/XTPGridRecordItemVariant.h"
#include "ItemTypes/XTPGridRecordItemIcon.h"
#include "ItemTypes/XTPGridRecordItemText.h"

#include "GridControl/Themes/XTPGridThemeDefault.h"
#include "GridControl/Themes/XTPGridThemeExplorer.h"
#include "GridControl/Themes/XTPGridThemeOffice2003.h"
#include "GridControl/Themes/XTPGridThemeOffice2003Luna.h"
#include "GridControl/Themes/XTPGridThemeOffice2013.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2012.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2015.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2017.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2019.h"
#include "GridControl/Themes/XTPGridThemeVisualStudio2022.h"
#include "GridControl/Themes/XTPGridThemeOfficeXP.h"
#include "GridControl/Themes/XTPGridThemeResource.h"
#include "GridControl/Themes/XTPGridThemeNativeWindows10.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_GRID_HSCROLL_STEP XTP_DPI_X(7)
#define XTP_GRID_VSCROLL_STEP XTP_DPI_Y(7)

#define XTP_GRID_AUTO_SCROLL_TIMER_ID 7
#define XTP_GRID_AUTO_SCROLL_TIMER_RESOLUTION_MS 200

#define XTP_GRID_DELAY_CLICK_TIMER_ID 10
#define XTP_GRID_DELAYEDIT_CLICK_TIMER_ID 20
#define XTP_GRID_CB_RECORDS_DATA_VER 1

#ifndef _UNICODE
#	define XTP_CF_TEXT_T CF_TEXT
#else
#	define XTP_CF_TEXT_T CF_UNICODETEXT
#endif

//////////////////////////////////////////////////////////////////////////
XTP_IMPLEMENT_HEAP_ALLOCATOR(CXTPGridDataAllocator, FALSE)
XTP_IMPLEMENT_HEAP_ALLOCATOR(CXTPGridRowAllocator, FALSE)

// to allocate in app default heap
XTP_IMPLEMENT_HEAP_ALLOCATOR(CXTPGridAllocatorDefault, FALSE)

class CXTPGridRow_Batch : public CXTPBatchAllocObjT<CXTPGridRow, CXTPGridRow_BatchData>
{
};

class CXTPGridGroupRow_Batch
	: public CXTPBatchAllocObjT<CXTPGridGroupRow, CXTPGridGroupRow_BatchData>
{
};

XTP_IMPLEMENT_BATCH_ALLOC_OBJ_DATA(CXTPGridRow_BatchData, CXTPGridRow_Batch, FALSE)
XTP_IMPLEMENT_BATCH_ALLOC_OBJ_DATA(CXTPGridGroupRow_BatchData, CXTPGridGroupRow_Batch, FALSE)

//===========================================================================
HGLOBAL XTPAllocStrInGlobalMem(const CString& strText)
{
	int nLen = XTPToIntChecked((strText.GetLength() + 1) * sizeof(TCHAR));

	UINT nAllocFlags = GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT;
	HGLOBAL hText	= ::GlobalAlloc(nAllocFlags, XTPToSizeTChecked(nLen));

	if (hText != NULL)
	{
		LPTSTR lptstrCopy = (TCHAR*)GlobalLock(hText);
		STRCPY_S(lptstrCopy, XTPToSizeTChecked(strText.GetLength() + 1), (LPCTSTR)strText);
		GlobalUnlock(hText);
	}

	return hText;
}

//===========================================================================
BOOL CXTPGridControl::UseGridCustomHeap()
{
	ASSERT(CXTPGridDataAllocator::ms_dwRefs == 0 || CXTPGridDataAllocator::ms_bUseCustomHeap);
	ASSERT(CXTPGridRowAllocator::ms_dwRefs == 0 || CXTPGridRowAllocator::ms_bUseCustomHeap);

	if (CXTPGridDataAllocator::ms_dwRefs == 0)
		CXTPGridDataAllocator::ms_bUseCustomHeap = TRUE;

	if (CXTPGridRowAllocator::ms_dwRefs == 0)
		CXTPGridRowAllocator::ms_bUseCustomHeap = TRUE;

	return CXTPGridDataAllocator::ms_bUseCustomHeap && CXTPGridRowAllocator::ms_bUseCustomHeap;
}

BOOL CXTPGridControl::UseRowBatchAllocation()
{
	ASSERT(CXTPGridRow_BatchData::IsDataEmpty()
		   || CXTPGridRow_BatchData::m_bBatchAllocationEnabled);
	ASSERT(CXTPGridGroupRow_BatchData::IsDataEmpty()
		   || CXTPGridGroupRow_BatchData::m_bBatchAllocationEnabled);

	if (CXTPGridRow_BatchData::IsDataEmpty())
		CXTPGridRow_BatchData::m_bBatchAllocationEnabled = TRUE;

	if (CXTPGridGroupRow_BatchData::IsDataEmpty())
		CXTPGridGroupRow_BatchData::m_bBatchAllocationEnabled = TRUE;

	return CXTPGridRow_BatchData::m_bBatchAllocationEnabled
		   && CXTPGridGroupRow_BatchData::m_bBatchAllocationEnabled;
}

void CXTPGridControl::FreeRowBatchExtraData()
{
	CXTPGridRow_Batch::FreeExtraData();
	CXTPGridGroupRow_Batch::FreeExtraData();
}

CArray<CXTPGridControlLocale::XTP_TIMESPEC, CXTPGridControlLocale::XTP_TIMESPEC&>
	CXTPGridControlLocale::s_arMappedSpecs;

//////////////////////////////////////////////////////////////////////////
// CGridDropTarget
class CXTPGridControl::CGridDropTarget : public COleDropTarget
{
public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState,
								   CPoint point)
	{
		CXTPGridControl* pGrid = DYNAMIC_DOWNCAST(CXTPGridControl, pWnd);
		if (!pGrid)
			return DROPEFFECT_NONE;

		return pGrid->OnDragOver(pDataObject, dwKeyState, point, 0);
	}

	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState,
								  CPoint point)
	{
		CXTPGridControl* pGrid = DYNAMIC_DOWNCAST(CXTPGridControl, pWnd);
		if (!pGrid)
			return DROPEFFECT_NONE;

		return pGrid->OnDragOver(pDataObject, dwKeyState, point, 2);
	}

	virtual void OnDragLeave(CWnd* pWnd)
	{
		CXTPGridControl* pGrid = DYNAMIC_DOWNCAST(CXTPGridControl, pWnd);

		if (pGrid)
			pGrid->OnDragOver(NULL, 0, CPoint(-1, -1), 1);
	}

	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect,
						CPoint point)
	{
		CXTPGridControl* pGrid = DYNAMIC_DOWNCAST(CXTPGridControl, pWnd);

		if (pGrid)
			return pGrid->OnDrop(pDataObject, dropEffect, point);

		return FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
void XTPStrSplit(LPCTSTR pcszString, LPCTSTR pcszSeparator, CStringArray& rarStrings)
{
	rarStrings.RemoveAll();

	int nSeparatorLen = (int)_tcslen(pcszSeparator);
	CString strString(pcszString);
	CString strItem;
	int nIndex_start = 0;

	// parse data
	BOOL bBreak = FALSE;
	do
	{
		int nIndex = strString.Find(pcszSeparator, nIndex_start);
		if (nIndex >= 0)
		{
			strItem = strString.Mid(nIndex_start, nIndex - nIndex_start);
		}
		else
		{
			strItem = strString.Mid(nIndex_start);
			bBreak  = TRUE;
		}

		//---------------------------------------
		rarStrings.Add(strItem);

		nIndex_start = nIndex + nSeparatorLen;
	} while (!bBreak);
}

CString XTPStrMake(const CStringArray& arStrings, LPCTSTR pcszSeparator)
{
	CString strString;

	int nCount = (int)arStrings.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (i > 0)
			strString += pcszSeparator;

		strString += arStrings[i];
	}

	return strString;
}

// CXTPGridControl
IMPLEMENT_DYNCREATE(CXTPGridControl, CWnd)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridControl, CWnd)
	//{{AFX_MSG_MAP(CXTPGridControl)
	// Window
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_ENABLE()
	ON_WM_TIMER()
	ON_WM_NCHITTEST_EX()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_NCPAINT()

	// Mouse
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()

	// Key events
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()

	// Scrolling
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()

	ON_WM_CAPTURECHANGED()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	ON_WM_STYLECHANGED()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(WM_GETOBJECT, OnGetObject)
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE_VOID(XTP_WM_MARKUPREDRAW, RedrawControl)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPGridControl::CXTPGridControl()
	: m_bThemeMetrics(FALSE)
	, m_dwLastMouseMessage(WM_NULL)
	, m_bDoubleBuffering(TRUE)
	, m_pSectionHeader(NULL)
	, m_pSectionBody(NULL)
	, m_pSectionFooter(NULL)
	, m_pSectionScroll(NULL)
	, m_pMarkupContext(NULL)

	, m_scrollModeH(xtpGridScrollModeSmooth)
	, m_scrollModeV(xtpGridScrollModeBlock)
	, m_nScrollOffsetH(0)
	, m_nScrollStepH(XTP_GRID_HSCROLL_STEP)
	, m_nScrollStepV(XTP_GRID_VSCROLL_STEP)

	, m_bLockExpand(FALSE)
	, m_bEditOnClick(TRUE)
	, m_bEditOnDelayClick(FALSE)
	, m_bEditOnDoubleClick(TRUE)
	, m_bPinFooterRows(FALSE)
	, m_bPinFooterRowsPrinted(TRUE)
	, m_bCreated(FALSE)
{
	RegisterWindowClass();

	m_pBehavior = new CXTPGridBehavior(xtpGridBehaviorCodejockDefault);
	m_pTip		= new CXTPGridTip();

	m_nLockUpdateCount = 0;
	m_nRowsPerWheel	= XTPToIntChecked(GetMouseScrollLines());

	m_pSections		 = new CXTPGridSections(this);
	m_pSectionHeader = m_pSections->GetAt(xtpGridSectionHeader);
	m_pSectionBody   = m_pSections->GetAt(xtpGridSectionBody);
	m_pSectionFooter = m_pSections->GetAt(xtpGridSectionFooter);
	m_pSectionScroll = m_pSectionBody; // Default scroll section

	m_pSectionFooter->GetBorder()->GetBorderThickness()->SetTop(1);

	m_pSectionHeader->SetHeightMode(xtpGridSectionHeightModeAuto);
	m_pSectionBody->SetHeightMode(xtpGridSectionHeightModeExpand);
	m_pSectionFooter->SetHeightMode(xtpGridSectionHeightModeAuto);

	m_pSectionHeader->SetDividerPosition(xtpGridSectionDividerPositionBottom);
	m_pSectionFooter->SetDividerPosition(xtpGridSectionDividerPositionTop);
	m_pSectionHeader->SetDividerStyle(xtpGridFixedRowsDividerThin);
	m_pSectionFooter->SetDividerStyle(xtpGridFixedRowsDividerThin);

	// Allow group
	m_pSectionHeader->AllowGroup(FALSE);
	m_pSectionBody->AllowGroup(TRUE);
	m_pSectionFooter->AllowGroup(FALSE);

	// Allow sort
	m_pSectionHeader->AllowSort(FALSE);
	m_pSectionBody->AllowSort(TRUE);
	m_pSectionFooter->AllowSort(FALSE);

	// Allow filter
	m_pSectionHeader->AllowFilter(FALSE);
	m_pSectionBody->AllowFilter(TRUE);
	m_pSectionFooter->AllowFilter(FALSE);

	m_pSectionBody->SetVisible(TRUE);

	m_pRows = m_pSectionBody->GetRows();

	m_pColumns	= new CXTPGridColumns(this);
	m_pGridHeader = new CXTPGridHeader(this, m_pColumns);
	// ICON_VIEW_MODE RELATED <<
	m_bIconView			   = FALSE;
	m_iIconWidth		   = 0;
	m_iIconHeight		   = 0;
	m_iIconWidthSpacing	= 0;
	m_iIconHeightSpacing   = 0;
	m_iIconViewColumn	  = -1;
	m_iIconViewRowsPerLine = 0;
	m_bUseIconColumnForNum = FALSE;

	m_iColumnForNumPrev   = -1;
	m_iColumnForNum		  = -1;
	m_bPrevFocusSubItems  = FALSE;
	m_bPrevHeaderAutoSize = FALSE;
	m_iIconPropNum		  = 0;
	m_iIconNum			  = 0;
	m_pPrevVisible		  = NULL;
	m_pPrevGroupsOrder	= NULL;
	m_nPrevTreeIndent	 = 0;

	m_bIconColumnIndexNotValid = FALSE;

	m_bPrevHeaderRows = FALSE;
	m_bPrevFooterRows = FALSE;
	m_bPrevHeaderShow = FALSE;
	m_bPrevFooterShow = FALSE;

	m_bRClickDrag = FALSE;

	// ICON_VIEW_MODE RELATED >>
	m_pPaintManager = NULL;
	SetTheme(xtpGridThemeOfficeXP);

	m_pNavigatorGrid = new CXTPGridNavigator(this);
	m_pNavigatorIcon = new CXTPGridIconNavigator(this);
	m_pNavigator	 = m_pNavigatorGrid;

	// m_nSelectionLastBlockStartRow = -1;
	m_bKeepSelectionAfterSort = TRUE;
	m_bUnrestrictedDragDrop   = FALSE;

	m_bFreeHeightMode		= FALSE;
	m_nDefaultRowFreeHeight = XTP_DPI_Y(20);

	m_uiDelayEditTimer   = NULL;
	m_uiDelayEditMaxTime = GetDoubleClickTime() + 50;

	m_iLastRqstEditRow		= -1;
	m_iLastRqstEditCol		= -1;
	m_uRqstEditTimer		= 0;
	m_mouseMode				= xtpGridMouseNothing;
	m_bWasShiftKey			= FALSE;
	m_bSortedDragDrop		= FALSE;
	m_bNoNeedSortedDragDrop = TRUE;
	m_bInternalMove			= FALSE;
	m_bTrapTabKey			= FALSE;

	m_pSelectedRows = new CXTPGridSelectedRows(this);

	m_bStrictBestFit = FALSE;
	m_bChanged		 = TRUE;

	m_bGroupByEnabled = FALSE;
	m_bHeaderVisible  = TRUE;
	m_bFooterVisible  = FALSE;

	m_bBlockSelection	 = TRUE;
	m_bMultiSelectionMode = FALSE;

	m_bShowTooltips = TRUE;

	m_bSkipGroupsFocus			 = TRUE;
	m_bSelectionExcludeGroupRows = TRUE;

	m_pImageManager = new CXTPImageManager();

	m_pFocusedColumn = NULL;
	m_pActiveItem	= NULL;

	m_bEnsureFocusedRowVisible = TRUE;
	m_bFocusSubItems		   = FALSE;
	m_bPreviewAllowEdit		   = FALSE;
	m_bInitialSelectionEnable  = TRUE;
	m_bRowFocusVisible		   = TRUE;

	m_bAutoCheckItems = TRUE;

	m_bShowIconWhenEditing = TRUE;

	m_pInplaceEdit	= new CXTPGridInplaceEdit();
	m_pInplaceButtons = new CXTPGridInplaceButtons();
	m_pInplaceList	= new CXTPGridInplaceList();

	m_rcGroupByArea.SetRectEmpty();
	m_rcHeaderArea.SetRectEmpty();
	m_rcFooterArea.SetRectEmpty();

	m_bMovePivot	   = FALSE;
	m_bForcePagination = FALSE;

	m_nDisableReorderColumnsCount = 0;
	m_pHotRow					  = NULL;
	m_pHotExpandButtonRow		  = NULL;
	m_pHotHyperlink				  = NULL;
	m_nOLEDropMode				  = 0;
	m_nOLEDropAbove				  = FALSE;

	m_bVScrollBarVisible = FALSE;
	m_bHScrollBarVisible = FALSE;

	m_bPrepareDrag		= FALSE;
	m_ptDrag			= CPoint(0, 0);
	m_nDragSensitivityX = 3;
	m_nDragSensitivityY = 3;

	m_pToolTipContext	= new CXTPToolTipContext;
	m_pCachedToolTipInfo = new XTP_NM_GRIDTOOLTIPINFO;

	m_cfGrid				  = NULL;
	m_pDropTarget			  = new CGridDropTarget;
	m_bDragMode				  = FALSE;
	m_bInternalDrag			  = FALSE;
	m_dwDragDropFlags		  = 0;
	m_dwDropMarkerFlags		  = xtpGridDropBetween; // Default to how it has always been.
	m_pSelectedRowsBeforeDrag = NULL;
	m_nDropPos				  = -1;
	m_pDropRecords			  = NULL;

	m_bFastDeselectMode	= FALSE;
	m_bAdjustLayoutRunning = FALSE;

	m_uAutoScrollTimerID = 0;

	m_bSortRecordChilds = FALSE;

	m_pRowsCompareFunc = NULL;

	m_ptrVirtualEditingRow = NULL;

	m_bFilterHiddenColumns   = FALSE;
	m_nRecordsTreeFilterMode = xtpGridFilterTreeSimple;

	m_hbmpWatermark			= NULL;
	m_WatermarkTransparency = 100;
	m_WatermarkAlignment	= xtpGridWatermarkStretch;

	m_nEnsureVisibleRowIdx = m_nEnsureVisibleColumnIdx = -1;

	m_pDataManager = NULL;

	m_nAutoVScrollTimerResolution = XTP_GRID_AUTO_SCROLL_TIMER_RESOLUTION_MS;

	m_bMarkupEnabled = FALSE;

	m_bDesktopTrackerMode = FALSE;

	m_bStrictFiltering = FALSE;

	m_pTip->m_pGridControl = this;

#ifdef _XTP_ACTIVEX
	m_bCustomMeasureRow			= FALSE;
	m_bCustomDrawItem			= FALSE;
	m_bCustomDrawPreviewItem	= FALSE;
	m_bCustomBeforeDrawRow		= FALSE;
	m_bCustomMeasurePreviewItem = FALSE;
#endif

	EnableAutomation();
}

CXTPGridControl::~CXTPGridControl()
{
	if (::IsWindow(m_pTip->GetSafeHwnd()))
		m_pTip->DestroyWindow();

	EditItem(NULL);

	ResetContent(FALSE);
	EnableMarkup(FALSE);

	CMDTARGET_RELEASE(m_pSelectedRows);
	CMDTARGET_RELEASE(m_pColumns);
	CMDTARGET_RELEASE(m_pPaintManager);
	CMDTARGET_RELEASE(m_pNavigatorGrid);
	CMDTARGET_RELEASE(m_pNavigatorIcon);

	CMDTARGET_RELEASE(m_pImageManager);
	CMDTARGET_RELEASE(m_pGridHeader);

	CMDTARGET_RELEASE(m_pToolTipContext);
	SAFE_DELETE(m_pCachedToolTipInfo);

	SAFE_DELETE(m_pInplaceEdit);
	SAFE_DELETE(m_pInplaceButtons);
	SAFE_DELETE(m_pInplaceList);

	CMDTARGET_RELEASE(m_pDropTarget);
	CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag);
	CMDTARGET_RELEASE(m_ptrVirtualEditingRow);

	CMDTARGET_RELEASE(m_pDropRecords);

	if (m_hbmpWatermark)
		::DeleteObject(m_hbmpWatermark);
	// ICON_VIEW_MODE RELATED <<
	CMDTARGET_RELEASE(m_pPrevVisible);
	CMDTARGET_RELEASE(m_pPrevGroupsOrder);
	// ICON_VIEW_MODE RELATED >>
	CMDTARGET_RELEASE(m_pDataManager);

	SAFE_DELETE(m_pBehavior);
	SAFE_DELETE(m_pTip);

	SAFE_DELETE(m_pSections);
}

BOOL CXTPGridControl::RegisterWindowClass(HINSTANCE hInstance)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPGRIDCTRL_CLASSNAME, CS_DBLCLKS);
}

void CXTPGridControl::ResetContent(BOOL bUpdateControl)
{
	EditItem(NULL);

	m_pSections->ResetContent();

	if (m_pSelectedRows)
		m_pSelectedRows->Clear();

	if (bUpdateControl)
	{
		AdjustIndentation();
		AdjustScrollBars();
		RedrawControl();
	}
}

void CXTPGridControl::SetGridHeader(CXTPGridHeader* pGridHeader)
{
	if (NULL != pGridHeader)
	{
		m_pGridHeader->InternalRelease();
		m_pGridHeader = pGridHeader;
		AdjustLayout();
	}
}

void CXTPGridControl::SetImageManager(CXTPImageManager* pImageManager)
{
	if (NULL != pImageManager)
	{
		m_pImageManager->InternalRelease();
		m_pImageManager = pImageManager;
	}
}

void CXTPGridControl::SetImageList(CImageList* pImageList)
{
	for (int i = 0; i < pImageList->GetImageCount(); i++)
	{
		HICON hIcon = pImageList->ExtractIcon(i);
		m_pImageManager->SetIcon(hIcon, XTPToUInt(i));

		DestroyIcon(hIcon);
	}
}

void CXTPGridControl::SetPaintManager(CXTPGridPaintManager* pPaintManager)
{
	if (NULL != pPaintManager)
	{
		if (m_pPaintManager != NULL)
		{
			m_pPaintManager->SetGridControl(NULL);
			m_pPaintManager->InternalRelease();
		}

		m_pPaintManager = pPaintManager;
		m_pPaintManager->SetGridControl(this);
		m_pPaintManager->RefreshMetrics();

		AdjustLayout();
		AdjustScrollBars();
		RedrawControl();

		// Force redraw in non client area.
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
					   SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}
}

int CXTPGridControl::GetIndent(int nLevel) const
{
	return max(0, (nLevel - 1) * m_pPaintManager->m_nTreeIndent);
}

void CXTPGridControl::SetTreeIndent(int nIndent)
{
	m_pPaintManager->m_nTreeIndent = nIndent;
}

void CXTPGridControl::BeginUpdate()
{
	m_nLockUpdateCount++;
}

void CXTPGridControl::EndUpdate()
{
	SetChanged();
	m_nLockUpdateCount--;

	if (m_nLockUpdateCount == 0)
	{
		RedrawControl();
	}
}

void CXTPGridControl::RedrawControl()
{
	SetChanged();
	if (m_nLockUpdateCount == 0 && GetSafeHwnd())
		Invalidate(FALSE);
}

void CXTPGridControl::UpdateSubList()
{
	CXTPGridHeader* pHeader = GetGridHeader();
	if (pHeader && pHeader->m_pSubList && (pHeader->m_pSubList->GetGridCtrl() == this))
	{
		pHeader->m_pSubList->UpdateList();
	}
}

CLIPFORMAT CXTPGridControl::EnableDragDrop(LPCTSTR lpszClipboardFormat, DWORD dwFlags,
										   DWORD dwDropMarkerFlags)
{
	if (m_dwDragDropFlags != 0)
	{
		if (m_pDropTarget)
			m_pDropTarget->Revoke();
	}

	m_dwDragDropFlags   = dwFlags;
	m_dwDropMarkerFlags = dwDropMarkerFlags;
	m_cfGrid			= NULL;

	if (m_dwDragDropFlags != 0)
	{
		if (m_pDropTarget)
		{
			m_cfGrid = (CLIPFORMAT)::RegisterClipboardFormat(lpszClipboardFormat);

			m_pDropTarget->Revoke(); // to ensure kill previous registration.

			m_pDropTarget->Register(this);
		}
	}

	return m_cfGrid;
}

BOOL CXTPGridControl::ApplyFilter(CXTPGridRecord* pRecord, CString strFilterText,
								  BOOL bIncludePreview)
{
	// not filtered if filter text is empty
	if (!pRecord)
		return FALSE;

	if (pRecord->IsFiltered())
		return TRUE;

	if (strFilterText.IsEmpty())
		return FALSE;

	BOOL bFilterSimple = GetRecordsTreeFilterMode() == xtpGridFilterTreeSimple;

	if (!bFilterSimple && pRecord->HasChildren() && pRecord->GetChilds())
	{
		CXTPGridRecords* pChildren = pRecord->GetChilds();
		int nCount				   = pChildren->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPGridRecord* pR = pChildren->GetAt(i);
			if (!ApplyFilter(pR, strFilterText, bIncludePreview))
				return FALSE;
		}
	}

	if (GetRecordsTreeFilterMode() == xtpGridFilterTreeByEndChildrenOnly && pRecord->HasChildren())
		return TRUE;

	return _ApplyFilter(pRecord, strFilterText, bIncludePreview);
}

BOOL CXTPGridControl::_ApplyFilter(CXTPGridRecord* pRecord, CString strFilterText,
								   BOOL bIncludePreview)
{
	// not filtered if filter text is empty
	if (!pRecord)
		return FALSE;

	if (pRecord->IsFiltered())
		return TRUE;

	if (strFilterText.IsEmpty())
		return FALSE;

	BOOL bFilterHidden = IsFilterHiddenColumns();

	// process each token in the filter string
	TCHAR szSeps[]		 = _T(" \t");
	TCHAR szSepsStrict[] = _T("\t");

	TCHAR *szToken, *lpszContext = 0;
	// int nCurPos = 0;

	szToken = STRTOK_S(strFilterText.GetBuffer(strFilterText.GetLength()),
					   m_bStrictFiltering ? szSepsStrict : szSeps, &lpszContext);

	while (szToken != NULL)
	{
		CString strToken(szToken);
		strToken.MakeLower();
		BOOL bTokenFound = FALSE;
		// enumerate all visible columns
		int nColumnsCount = m_pColumns->GetCount();
		for (int nCol = 0; nCol < nColumnsCount; nCol++)
		{
			CXTPGridColumn* pCol = m_pColumns->GetAt(nCol);
			if (pCol && (pCol->IsVisible() || bFilterHidden) && pCol->IsFilterable())
			{
				CXTPGridRecordItem* pItem = pRecord->GetItem(pCol);
				if (pItem)
				{
					CString sItemText = pItem->GetCaption(pCol);
					// case-insensitive search
					sItemText.MakeLower();
					bTokenFound = sItemText.Find(strToken) != -1;

					if (bTokenFound)
					{
						// stop search current token - passed
						break;
					}
				}
			}
		}
		// also check preview text
		if (bIncludePreview && !bTokenFound && pRecord->GetItemPreview())
		{
			CString sItemText = pRecord->GetItemPreview()->GetCaption(NULL);
			// case-insensitive search
			sItemText.MakeLower();
			bTokenFound = sItemText.Find(strToken) != -1;
		}

		// Token not found - filter this record
		if (!bTokenFound)
		{
			strFilterText.ReleaseBuffer();
			return TRUE;
		}

		// get next token
		if (m_bStrictFiltering)
			szToken = STRTOK_S(NULL, szSepsStrict, &lpszContext);
		else
			szToken = STRTOK_S(NULL, szSeps, &lpszContext);
	}

	strFilterText.ReleaseBuffer();
	return FALSE;
}

void CXTPGridControl::Populate()
{
	EditItem(NULL);

	BeginUpdate();

	m_pSelectedRows->Clear();

	m_pSectionHeader->Populate(FALSE);
	m_pSectionBody->Populate(FALSE);
	m_pSectionFooter->Populate(FALSE);

	AdjustIndentation();
	AdjustLayout();
	AdjustScrollBars();

	UpdateSubList();

	if (GetRows()->m_nFocusedRow == -1)
	{
		SetTopRow(0);

		if (GetRows()->GetCount() > 0)
		{
			GetRows()->m_nFocusedRow = 0;
			if (IsSelectionEnabled() && IsInitialSelectionEnabled())
			{
				CXTPGridRow* pFirst = m_pRows->GetAt(0);
				if (pFirst)
				{
					if (!IsSkipGroupsFocusEnabled() || !pFirst->IsGroupRow())
						m_pSelectedRows->Select(pFirst);
					else
					{
						while (pFirst->IsGroupRow())
						{
							if (pFirst->HasChildren())
								pFirst = pFirst->GetChilds()->GetAt(0);
						}
						m_pSelectedRows->Select(pFirst);
					}
				}
			}
		}
	}
	else
	{
		EnsureVisible(GetFocusedRow());
	}

	if (m_pFocusedColumn == NULL && m_bFocusSubItems)
	{
		m_pFocusedColumn = m_pColumns->GetFirstVisibleColumn();
		//------------ Forum proposal 2009 June 19 ---------------------------- <<
		if (GetRows())
		{
			CXTPGridRow* pRow = GetRows()->GetAt(GetTopRowIndex());
			if (pRow)
				m_pFocusedColumn = GetNextFocusableColumn(pRow, 0, 1);
		}
		//------------ Forum proposal 2009 June 19 ---------------------------- >>
	}
	EndUpdate();

	AdjustScrollBars();

	if (GetBehavior()->Notifications->Populate->bSelectionChanged)
	{
		if (m_pSelectedRows->IsChanged())
		{
			OnSelectionChanged();
		}
	}
}

void CXTPGridControl::AdjustIndentation()
{
	GetGridHeader()->m_nIndentLevel = m_pColumns->GetGroupsOrder()->GetCount();
}

CXTPGridColumn* CXTPGridControl::AddColumn(CXTPGridColumn* pColumn)
{
	ASSERT(pColumn);
	m_pColumns->Add(pColumn);

	return pColumn;
}

void CXTPGridControl::DrawNoItems(CDC* pDC, const CRect& rcClient)
{
	pDC->SetTextColor(GetPaintManager()->m_clrWindowText);
	CString strNoItems = GetPaintManager()->m_strNoItems;

	if (!strNoItems.IsEmpty())
	{
		CRect rcText(rcClient);
		rcText.DeflateRect(XTP_DPI_X(5), XTP_DPI_Y(5), XTP_DPI_X(5), XTP_DPI_Y(5));
		CXTPFontDC font(pDC, &GetPaintManager()->m_xtpFontText);
		UINT uFlags = DT_CENTER | DT_TOP | DT_NOPREFIX | DT_WORDBREAK | DT_WORD_ELLIPSIS
					  | DT_END_ELLIPSIS | DT_EDITCONTROL;

		XTPMarkupDrawText(GetMarkupContext(), pDC->GetSafeHdc(), strNoItems, rcText, uFlags);
	}
}

int CXTPGridControl::DrawRows(CDC* pDC, CRect& rcClient, int y, CXTPGridRows* pRows, int nTopRow,
							  int nColumnFrom, int nColumnTo, int* pnHeight)
{
	const BOOL bIsPrinting = pDC->IsPrinting();

	CXTPGridRecordMergeItems mergeItems;

	CXTPGridScreenRows* pScreenRows = pRows->GetScreenRows();
	pScreenRows->Clear();
	pScreenRows->SetSize(0, 100);

	CXTPGridHeader* pHeader = GetGridHeader();
	CRgn rgnClip;
	int nRow = nTopRow; // Row index

	if (NULL != pHeader->GetNextVisibleColumn(-1, xtpGridColumnDirectionRight))
	{
		int nRowCount  = pRows->GetCount(); // Number of rows in the rows collection
		int nRowWidth  = 0;
		int nRowHeight = 0;

		if (bIsPrinting)
		{
			nRowWidth = rcClient.Width();
		}
		else
		{
			nRowWidth = pHeader->GetWidth();
		}

		//////////////////////////////////////////////////////////////////////////
		// Draw non-merged items
		//////////////////////////////////////////////////////////////////////////
#if 1

		int nFreezeRight		   = 0;
		int nLastFreezeColumnIndex = -1;
		int nFreezeColumnsCount	= GetFreezeColumnsCount(nLastFreezeColumnIndex);

		if (nFreezeColumnsCount > 0)
		{
			CXTPGridColumn* pColumn = GetColumns()->GetAt(nLastFreezeColumnIndex);

			if (NULL != pColumn)
			{
				nFreezeRight = pColumn->GetRect().right;
			}
		}

		pRows->ClearMergeHeight();

		for (nRow = nTopRow; nRow < nRowCount; nRow++)
		{
			CXTPGridRow* pRow = pRows->GetAt(nRow);
			ASSERT(pRow);

			if (NULL != pRow)
			{
				nRowHeight = pRow->GetHeight(pDC, nRowWidth);

				// Test if row is outside of client rectangle
				if (bIsPrinting)
				{
					// Clipped rows are printed on the next page
					if (y + nRowHeight > rcClient.bottom)
					{
						break;
					}
				}
				else
				{
					// Clipped rows are drawn
					if (y > rcClient.bottom)
					{
						break;
					}
				}

				// Rectangles in Windows don't include the right and bottom edge.
				CRect rcRow(rcClient.left, y, rcClient.left + nRowWidth, y + nRowHeight);

				int nLeftOffset = 0;

				if (!pDC->IsPrinting())
				{
					nLeftOffset = m_nScrollOffsetH;
				}

				// Draw row
				pRow->Draw(pDC, rcRow, rcClient, nLeftOffset, mergeItems, nColumnFrom, nColumnTo);

				y += nRowHeight;

				CXTPGridRow* pScreenRow = NULL;
				if (IsVirtualMode())
				{
					pScreenRow = CreateRow();
					pScreenRow->InitRow(pRow);
				}
				else
				{
					pScreenRow = pRow;
					pScreenRow->InternalAddRef();
				}

				pScreenRows->InsertAt(pScreenRows->GetSize(), pScreenRow);
			}
		} // for (nRow)
#endif

		//////////////////////////////////////////////////////////////////////////
		// Draw merged items
		//////////////////////////////////////////////////////////////////////////
#if 1
		BOOL bDrawFreeze = nFreezeColumnsCount > 0;

		while (true)
		{
			if (bDrawFreeze)
				VERIFY(rgnClip.CreateRectRgnIndirect(rcClient));
			else
				VERIFY(rgnClip.CreateRectRgn(nFreezeRight, rcClient.top, rcClient.right,
											 rcClient.bottom));

			if (!pDC->IsPrinting())
			{
				pDC->SelectClipRgn(&rgnClip, RGN_COPY);
			}

			// copy mergeItems, because it will be cleaned through FindRange()
			CXTPGridRecordMergeItems mergeItemsF;
			POSITION pos = mergeItems.GetStartPosition();
			while (pos)
			{
				CXTPGridRecordItem* pMergeItem = NULL;
				CXTPGridRecordMergeItem mergeItem;
				mergeItems.GetNextAssoc(pos, pMergeItem, mergeItem);
				mergeItemsF.SetAt(pMergeItem, mergeItem);
			}

			pos = mergeItemsF.GetStartPosition();

			while (pos)
			{
				CXTPGridRecordItem* pItem;
				CXTPGridRecordMergeItem mergeItem;

				mergeItemsF.GetNextAssoc(pos, pItem, mergeItem);

				CXTPGridRecordItemRange range;
				CRect rcItem;

				while (mergeItem.FindRange(range, rcItem))
				{
					// rcItem.left   = GetColumns()->GetAt(range.m_nColumnFrom)->GetRect().left;
					// rcItem.right  = GetColumns()->GetAt(range.m_nColumnTo  )->GetRect().right;
					// rcItem.top    = pRows->GetAt(range.m_nRecordFrom)->GetRect().top;
					// rcItem.bottom = pRows->GetAt(range.m_nRecordTo  )->GetRect().bottom;

					if (bDrawFreeze && range.m_nColumnFrom > nLastFreezeColumnIndex
						|| !bDrawFreeze && range.m_nColumnFrom <= nLastFreezeColumnIndex)
						continue;

					BOOL bDrawFreezeColsDivider = FALSE;
					CRect rcGrid(rcItem);
					rcItem.DeflateRect(0, 0, XTP_DPI_X(1), XTP_DPI_Y(1));

					XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
					drawArgs.pDC		= pDC;
					drawArgs.pControl   = this;
					drawArgs.pColumn	= GetColumns()->GetAt(range.m_nColumnFrom);
					drawArgs.pRow		= pRows->GetAt(range.m_nRecordFrom);
					drawArgs.rcItem		= rcItem;
					drawArgs.nTextAlign = drawArgs.pColumn->GetAlignment();
					drawArgs.pItem		= pItem;

					pItem->Draw(&drawArgs);

					drawArgs.pRow->DrawItemGrid(pDC, drawArgs.pColumn, rcGrid);

					if (!bDrawFreeze)
					{ // draw frozen border again to prevent showing grid under border
						CXTPGridColumn* pColumn = GetColumns()->GetAt(range.m_nColumnFrom);
						if (NULL != pColumn && pColumn->GetRect().left <= nFreezeRight)
						{
							rcGrid.right		   = nFreezeRight;
							bDrawFreezeColsDivider = TRUE;
						}
					}
					else if (/*bDrawFreeze && */ range.m_nColumnTo == nLastFreezeColumnIndex)
					{
						bDrawFreezeColsDivider = TRUE;
					}

					if (bDrawFreezeColsDivider)
					{
						// choose selected row if any
						CXTPGridRow* pRowFromMerge = NULL;
						for (int i = range.m_nRecordFrom; i <= range.m_nRecordTo; i++)
						{
							pRowFromMerge = pRows->GetAt(i);
							if (pRowFromMerge && pRowFromMerge->IsSelected())
								break;
						}

						CRgn rgnClipFrozen;
						if (!pDC->IsPrinting())
						{
							pDC->SelectClipRgn(NULL, RGN_COPY);
							VERIFY(rgnClipFrozen.CreateRectRgnIndirect(rcClient));
							pDC->SelectClipRgn(&rgnClipFrozen, RGN_COPY);
						}
						GetPaintManager()->DrawFreezeColsDivider(pDC, rcGrid, this, pRowFromMerge);

						if (!pDC->IsPrinting())
						{
							pDC->SelectClipRgn(NULL, RGN_COPY);
							VERIFY(rgnClipFrozen.DeleteObject());
							pDC->SelectClipRgn(&rgnClip, RGN_COPY); // restore
						}
					}
				}
			}

			if (!pDC->IsPrinting())
			{
				pDC->SelectClipRgn(NULL, RGN_COPY);
			}
			VERIFY(rgnClip.DeleteObject());

			if (bDrawFreeze)
				bDrawFreeze = FALSE;
			else
				break;
		} // while bDrawFreeze

		mergeItems.RemoveAll();
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Fill the rest of space with empty grid
	//////////////////////////////////////////////////////////////////////////

	if (GetPaintManager()->IsDrawGridForEmptySpace() && y < rcClient.bottom)
	{
		VERIFY(rgnClip.CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom));

		if (!pDC->IsPrinting())
		{
			pDC->SelectClipRgn(&rgnClip, RGN_COPY);
		}

		CRect rcEmpty(rcClient);
		rcEmpty.top = y;
		DrawDefaultGrid(pDC, rcEmpty, GetPaintManager()->m_nRowHeight,
						m_nScrollOffsetH); // always use standard row height

		if (!pDC->IsPrinting())
		{
			pDC->SelectClipRgn(NULL, RGN_COPY);
		}

		VERIFY(rgnClip.DeleteObject());
	}
	if (0 == pRows->GetCount())
	{
		DrawNoItems(pDC, rcClient);
	}

	if (NULL != pnHeight)
	{
		*pnHeight = y - rcClient.top;
	}

	return nRow;
}

void CXTPGridControl::DrawIconView(CDC* pDC, CRect& rcClient)
{
	CXTPGridRecordMergeItems mergeItems;

	int nHeaderWidth = GetGridHeader()->GetWidth();
	int nRowCount	= m_pRows->GetCount();
	int y			 = rcClient.top;

	m_iIconViewRowsPerLine = GetNumRowsOnLine(nHeaderWidth);
	int x				   = rcClient.left;
	int iLastMainRow	   = m_pSectionBody->GetScrollIndexV();

	while (iLastMainRow % m_iIconViewRowsPerLine)
	{
		iLastMainRow++;
	}

	for (int i = iLastMainRow; i < nRowCount; i++)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(i);
		ASSERT(pRow);
		if (!pRow)
			continue;

		if (y > rcClient.bottom)
			break;

		if (pRow->GetIndex() % m_iIconViewRowsPerLine)
		{
			x += m_iIconWidthSpacing;

			CRect rcRow(x, y, x + m_iIconWidthSpacing, y + m_iIconHeightSpacing);

			pRow->Draw(pDC, rcRow, GetGridRectangle(), 0, mergeItems, 0, GetColumns()->GetCount());
		}
		else
		{
			x = rcClient.left;

			if (iLastMainRow != pRow->GetIndex())
			{
				y += m_iIconHeightSpacing;
				iLastMainRow = pRow->GetIndex();
			}

			CRect rcRow(x, y, x + m_iIconWidthSpacing, y + m_iIconHeightSpacing);

			pRow->Draw(pDC, rcRow, GetGridRectangle(), 0, mergeItems, 0, GetColumns()->GetCount());
		}

		CXTPGridRow* pScreenRow = NULL;
		if (IsVirtualMode())
		{
			pScreenRow = CreateRow();
			pScreenRow->InitRow(pRow);
			pScreenRow->SetExpanded(pRow->IsExpanded());
			pScreenRow->m_rcCollapse = pRow->m_rcCollapse;
		}
		else
		{
			pScreenRow = pRow;
			pScreenRow->InternalAddRef();
		}
		CXTPGridScreenRows* pScreenRows = GetRows()->GetScreenRows();
		pScreenRows->InsertAt(pScreenRows->GetSize(), pScreenRow);
	}
}

void CXTPGridControl::OnSelectionChanged()
{
	SetLastRqstEdit(-1, -1);

	// m_iLastRqstEditCol = -1;
	// m_iLastRqstEditRow = -1;

	SendNotifyMessage(XTP_NM_GRID_SELCHANGED);

	if (m_pSelectedRows)
		m_pSelectedRows->SetChanged(FALSE);
}

BOOL CXTPGridControl::OnFocusChanging(CXTPGridRow* pNewRow, CXTPGridColumn* pNewCol)
{
	XTP_NM_GRIDREQUESTEDIT nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.bCancel = FALSE;
	nm.pRow	= pNewRow ? pNewRow : GetFocusedRow();
	nm.pColumn = pNewCol ? pNewCol : GetFocusedColumn();
	nm.pItem   = nm.pRow && nm.pColumn
				   ? (nm.pRow->GetRecord() ? nm.pRow->GetRecord()->GetItem(nm.pColumn) : NULL)
				   : NULL;

	SendNotifyMessage(XTP_NM_GRID_FOCUS_CHANGING, (NMHDR*)&nm);

	return !nm.bCancel;
}

#ifdef _XTP_ACTIVEX
void CXTPGridControl::SetCustomDraw(long dwFlags)
{
	m_bCustomDrawItem			= dwFlags & xtpCustomDrawItem;
	m_bCustomDrawPreviewItem	= dwFlags & xtpCustomDrawPreviewItem;
	m_bCustomMeasureRow			= dwFlags & xtpCustomMeasureRow;
	m_bCustomBeforeDrawRow		= dwFlags & xtpCustomBeforeDrawRow;
	m_bCustomMeasurePreviewItem = dwFlags & xtpCustomMeasurePreviewItem;

	GetPaintManager()->SetFixedRowHeight(!m_bCustomMeasureRow);
}
#endif

void CXTPGridControl::GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
									 XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	XTP_NM_GRIDITEMMETRICS nmData;
	nmData.pDrawArgs	= pDrawArgs;
	nmData.pItemMetrics = pMetrics;

	SendNotifyMessage(XTP_NM_GRID_GETITEMMETRICS, (NMHDR*)&nmData);
}

BOOL CXTPGridControl::SetFocusedColumn(CXTPGridColumn* pColumn)
{
	if (m_pFocusedColumn != pColumn)
	{
		if (m_bFocusSubItems && pColumn)
		{
			if (!OnFocusChanging(NULL, pColumn))
				return FALSE;
		}

		m_pFocusedColumn = pColumn;

		if (m_pFocusedColumn && m_bFocusSubItems)
		{
			CRect rc(m_pFocusedColumn->GetRect());

			if (rc.right >= m_pSectionBody->GetRect().Width())
			{
				SetScrollOffsetH(m_nScrollOffsetH
								 + min(rc.left, rc.right - m_pSectionBody->m_rcSection.Width()));
			}
			else
			{
				int nFreezeColumnWidth  = 0;
				int nFreezeColumnsCount = GetFreezeColumnsCount();
				if (nFreezeColumnsCount > 0)
				{
					for (int i = 0; (i < m_pColumns->GetCount()) && (nFreezeColumnsCount > 0); i++)
					{
						CXTPGridColumn* pColumnCheck = m_pColumns->GetAt(i);

						if (pColumnCheck == m_pFocusedColumn)
						{
							nFreezeColumnWidth = 0;
							break;
						}

						if (pColumnCheck && pColumnCheck->IsVisible())
						{
							nFreezeColumnsCount--;
							nFreezeColumnWidth = pColumnCheck->GetRect().right;
						}
					}
				}

				if (rc.left - nFreezeColumnWidth <= 0 && m_nScrollOffsetH != 0)
				{
					SetScrollOffsetH(m_nScrollOffsetH + rc.left - nFreezeColumnWidth);
				}
			}
		}

		if (m_bFocusSubItems)
			OnSelectionChanged();
	}

	return TRUE;
}

BOOL CXTPGridControl::SetFocusedRow(CXTPGridRow* pRow, BOOL bShiftKey, BOOL bControlKey)
{
	return SetFocusedRow(TRUE, pRow, bShiftKey, bControlKey);
}

BOOL CXTPGridControl::SetFocusedRow(CXTPGridRow* pRow, BOOL bControlKey)
{
	return SetFocusedRow(TRUE, pRow, FALSE, bControlKey);
}

int CXTPGridControl::_GetFocusedRowType() const
{
	int nFocusedRowType = -1;

	if (GetRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowType = xtpRowTypeBody;
	}
	if (GetHeaderRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowType = xtpRowTypeHeader;
	}
	if (GetFooterRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowType = xtpRowTypeFooter;
	}

	return nFocusedRowType;
}

int CXTPGridControl::_GetFocusedRowIndex() const
{
	int nFocusedRowIndex = -1;

	if (GetRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowIndex = GetRows()->GetFocusedRowIndex();
	}
	if (GetHeaderRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowIndex = GetHeaderRows()->GetFocusedRowIndex();
	}
	if (GetFooterRows()->GetFocusedRowIndex() != -1)
	{
		nFocusedRowIndex = GetFooterRows()->GetFocusedRowIndex();
	}

	return nFocusedRowIndex;
}

BOOL CXTPGridControl::_SetFocusedRow(CXTPGridRow* pRow)
{
	//////////////////////////////////////////////////////////////////////////
	// Current focused row
	//////////////////////////////////////////////////////////////////////////
	int nFocusedRowType  = _GetFocusedRowType();
	int nFocusedRowIndex = _GetFocusedRowIndex();

	//////////////////////////////////////////////////////////////////////////
	// New focused row
	//////////////////////////////////////////////////////////////////////////
	int nRowType  = -1;
	int nRowIndex = -1;

	if (NULL != pRow)
	{
		nRowType  = pRow->GetType();
		nRowIndex = pRow->GetIndex();
	}

	//////////////////////////////////////////////////////////////////////////
	// Notify OnFocusChanging
	//////////////////////////////////////////////////////////////////////////
	BOOL bApply = TRUE;

	if (nFocusedRowType != nRowType || nFocusedRowIndex != nRowIndex)
	{
		if (NULL != pRow)
		{
			pRow->InternalAddRef();

			bApply = OnFocusChanging(pRow, NULL);

			if (1 == pRow->m_dwRef)
			{
				// Removed
				bApply = FALSE;
			}

			pRow->InternalRelease();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Set new focused row
	//////////////////////////////////////////////////////////////////////////

	GetRows()->SetFocusedRowIndex(-1);
	GetFooterRows()->SetFocusedRowIndex(-1);
	GetHeaderRows()->SetFocusedRowIndex(-1);

	if (NULL != pRow && bApply)
	{
		switch (pRow->GetType())
		{
			case xtpRowTypeBody: GetRows()->SetFocusedRowIndex(pRow->GetIndex()); break;
			case xtpRowTypeHeader: GetHeaderRows()->SetFocusedRowIndex(pRow->GetIndex()); break;
			case xtpRowTypeFooter: GetFooterRows()->SetFocusedRowIndex(pRow->GetIndex()); break;
		}
	}

	return bApply;
}

BOOL CXTPGridControl::_SetSelectedRow(CXTPGridRow* pRow, int nFocusedRowIndex, BOOL bShiftKey,
									  BOOL bControlKey)
{
	BOOL bEnableSelection = FALSE;

	if (NULL != pRow)
	{
		bEnableSelection = pRow->GetSection()->IsSelectionEnabled();
	}

	if (bEnableSelection)
	{
		if (m_bBlockSelection)
		{
			if (bShiftKey && nFocusedRowIndex != -1)
			{
				if (m_pSelectedRows->m_nRowType == pRow->GetType())
				{
					int nSelRow = pRow->GetIndex();
					m_pSelectedRows->SelectBlock(nFocusedRowIndex, nSelRow, bControlKey);
				}
			}
			else if (!bControlKey)
			{
				m_pSelectedRows->Select(pRow);
			}
		}
		else
		{
			m_pSelectedRows->Select(pRow);
		}
	}

	return TRUE;
}

BOOL CXTPGridControl::SetFocusedRow(BOOL bEnsureVisible, CXTPGridRow* pRow, BOOL bShiftKey,
									BOOL bControlKey)
{
	int nFocusedRowIndex = _GetFocusedRowIndex(); // Current focused row

	EditItem(NULL);
	BeginUpdate();

	_SetFocusedRow(pRow); // Null pointer clears focus
	_SetSelectedRow(pRow, nFocusedRowIndex, bShiftKey,
					bControlKey); // Null pointer clears selection

	if (bEnsureVisible && (NULL != pRow))
	{
		EnsureVisible(pRow); // Null pointer not allowed
	}
	EndUpdate();

	if (m_pSelectedRows->IsChanged())
	{
		OnSelectionChanged();
	}

	return TRUE;
}

void CXTPGridControl::SetScrollOffsetH(int nOffset)
{
	if (nOffset < 0)
	{
		nOffset = 0;
	}

	if (nOffset != m_nScrollOffsetH)
	{
		m_nScrollOffsetH = nOffset;
		AdjustScrollBarH(TRUE, SIF_POS); // Only update position

		// Send notification
		XTP_NM_GRIDSCROLL msg;
		msg.nSection  = m_pSectionScroll->GetIndex();
		msg.nPosition = nOffset;
		SendNotifyMessage(XTP_NM_GRID_HSCROLL, &msg);
	}
}

void CXTPGridControl::SetScrollOffsetV(int nOffset)
{
	if (nOffset < 0)
	{
		nOffset = 0;
	}

	if (nOffset != m_pSectionScroll->GetScrollOffsetV())
	{
		m_pSectionScroll->SetScrollOffsetV(nOffset);
		AdjustScrollBarV(TRUE, SIF_PAGE | SIF_POS); // Only update page and position

		// Send notification
		XTP_NM_GRIDSCROLL msg;
		msg.nSection  = m_pSectionScroll->GetIndex();
		msg.nPosition = nOffset;
		SendNotifyMessage(XTP_NM_GRID_VSCROLL, &msg);
	}
}

void CXTPGridControl::SetTopRow(int nIndex)
{
	if (nIndex < 0)
	{
		nIndex = 0;
	}

	if (nIndex != GetTopRowIndex())
	{
		if (IsIconView())
		{
			// Must get first row on the same line.
			while (nIndex % GetRowsPerLine())
			{
				nIndex--;
			}
		}

		m_pSectionScroll->SetScrollIndexV(nIndex);
	}
}

void CXTPGridControl::EnsureVisible(CXTPGridRow* pRow)
{
	if (NULL != pRow)
	{
		if (NULL == GetSafeHwnd())
		{
			m_nEnsureVisibleRowIdx = pRow->GetIndex();
		}
		else
		{
			CXTPGridSection* pSection = pRow->GetSection();
			ASSERT(NULL != pSection);

			if (NULL != pSection)
			{
				if (pSection->GetRect().IsRectNull())
				{
					m_nEnsureVisibleRowIdx = pRow->GetIndex();
				}
				else
				{
					CClientDC dc(this);
					pSection->EnsureVisible(&dc, pRow);
				}
			}
		}
	}
}

void CXTPGridControl::EnsureVisible(CXTPGridColumn* pCheckColumn)
{
	int nCheckIndex = pCheckColumn ? pCheckColumn->GetIndex() : -1;

	if (nCheckIndex == -1 || !pCheckColumn->m_bVisible || nCheckIndex >= m_pColumns->GetCount())
		return;

	if (m_pSectionBody->m_rcSection.Height() <= 0)
	{
		m_nEnsureVisibleRowIdx = nCheckIndex;
		return;
	}

	CRect rc(pCheckColumn->GetRect());

	if (rc.right >= m_pSectionBody->m_rcSection.Width())
	{
		SetScrollOffsetH(m_nScrollOffsetH
						 + min(rc.left, rc.right - m_pSectionBody->m_rcSection.Width()));
	}
	else
	{
		int nFreezeColumnWidth  = 0;
		int nFreezeColumnsCount = GetFreezeColumnsCount();
		if (nFreezeColumnsCount > 0)
		{
			for (int i = 0; (i < m_pColumns->GetCount()) && (nFreezeColumnsCount > 0); i++)
			{
				CXTPGridColumn* pColumn = m_pColumns->GetAt(i);

				if (pColumn == pCheckColumn)
				{
					nFreezeColumnWidth = 0;
					break;
				}

				if (pColumn && pColumn->IsVisible())
				{
					nFreezeColumnsCount--;
					nFreezeColumnWidth = pColumn->GetRect().right;
				}
			}
		}

		if (rc.left - nFreezeColumnWidth <= 0 && m_nScrollOffsetH != 0)
		{
			SetScrollOffsetH(m_nScrollOffsetH + rc.left - nFreezeColumnWidth);
		}
	}
}

BOOL CXTPGridControl::HitTest(CPoint pt, CXTPGridHitTestInfo* pInfo) const
{
	if (NULL == pInfo)
	{
		return FALSE;
	}

	pInfo->m_pColumn		= NULL;
	pInfo->m_pRow			= NULL;
	pInfo->m_pItem			= NULL;
	pInfo->m_pBlock			= NULL;
	pInfo->m_htCode			= xtpGridHitTestUnknown;
	pInfo->m_iTrackPosition = 0;

	if (m_rcHeaderArea.PtInRect(pt))
	{
		pInfo->m_htCode  = xtpGridHitTestHeader;
		pInfo->m_pColumn = GetGridHeader()->HitTest(pt);
	}
	else if (m_rcGroupByArea.PtInRect(pt))
	{
		pInfo->m_htCode  = xtpGridHitTestGroupBox;
		int nColumn		 = GetGridHeader()->FindGroupByColumn(pt, TRUE);
		pInfo->m_pColumn = GetColumns()->GetGroupsOrder()->GetAt(nColumn);
	}
	else if (m_pSections->GetRect().PtInRect(pt))
	{
		const CXTPGridSection* pSection = m_pSections->HitTest(pt);

		if (NULL != pSection)
		{
			switch (pSection->GetRowType())
			{
				case xtpRowTypeBody: pInfo->m_htCode = xtpGridHitTestBodyRows; break;

				case xtpRowTypeHeader: pInfo->m_htCode = xtpGridHitTestHeaderRows; break;

				case xtpRowTypeFooter: pInfo->m_htCode = xtpGridHitTestFooterRows; break;
			}
		}

		pInfo->m_pRow = HitTest(pt);

		if (NULL != pInfo->m_pRow)
		{
			pInfo->m_pItem = pInfo->m_pRow->HitTest(pt, NULL, &pInfo->m_pColumn);
		}
	}

	return TRUE;
}

CXTPGridRow* CXTPGridControl::HitTest(CPoint pt) const
{
	CXTPGridRow* pRow = NULL;

	CXTPGridSection* pSection = m_pSections->HitTest(pt);

	if (NULL != pSection)
	{
		pRow = pSection->GetRows()->GetScreenRows()->HitTest(pt);
	}

	return pRow;
}

int CXTPGridControl::GetGridAreaRows(int nStartRow, BOOL bMoveDown)
{
	int nDirection = bMoveDown ? +1 : -1;

	int top = m_pSectionBody->m_rcSection.top;

	CClientDC dc(this);
	int nHeaderWidth = GetGridHeader()->GetWidth();

	int i = nStartRow;

	if (IsIconView())
	{
		// Find the first row, to account for the height of the row we are in.
		while (i % GetRowsPerLine() != 0)
		{
			i--;
		}
	}

	for (; (i < m_pRows->GetCount() && i >= 0); i += nDirection)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(i);
		ASSERT(pRow);
		if (!pRow)
			continue;

		int rowHeight = pRow->GetHeight(&dc, nHeaderWidth);

		if (top + rowHeight > m_pSectionBody->m_rcSection.bottom)
			return bMoveDown ? max(0, i - nStartRow - 1) : max(0, nStartRow - i - 1);

		top += rowHeight;
	}

	// return bMoveDown ? m_pRows->GetCount() - nStartRow : nStartRow;
	return bMoveDown ? max(0, m_pRows->GetCount() - nStartRow - 1)
					 : nStartRow; //-this is better way!
}

void CXTPGridControl::AdjustLayout()
{
	/*
		------------------------------
		| Group by                   |
		------------------------------
		| Header                     |
		------------------------------
		| Sections                   |
		------------------------------
		| Footer                     |
		------------------------------
	*/

	if (NULL == GetSafeHwnd())
		return;

	CXTPClientRect rcClient(this);
	CClientDC dc(this);

	if (rcClient.IsRectEmpty())
	{
		// Avoid unnecessary layout calculations during creation
		return;
	}

	ASSERT(!m_bAdjustLayoutRunning);
	if (m_bAdjustLayoutRunning) // guard to prevent the recursion similar to OnSize function
	{
		return;
	}

	m_bAdjustLayoutRunning = TRUE;

	int nHeaderWidth = m_rcHeaderArea.Width();

	CXTPGridHeader* pHeader = GetGridHeader();

	int nHeightGroupBy  = 0;
	int nHeightHeader   = 0;
	int nHeightSections = 0;
	int nHeightFooter   = 0;

	if (nHeaderWidth != rcClient.Width() && pHeader)
	{
		pHeader->AdjustColumnsWidth(rcClient.Width());
	}

	if (NULL != pHeader && m_bGroupByEnabled)
	{
		nHeightGroupBy = pHeader->GetGroupByHeight();
	}

	if (m_bHeaderVisible)
	{
		nHeightHeader = GetPaintManager()->GetHeaderHeight(this, &dc);
	}

	if (m_bFooterVisible)
	{
		nHeightFooter = GetPaintManager()->GetFooterHeight(this, &dc);
	}

	// Group by rect
	m_rcGroupByArea.SetRect(0, 0, rcClient.Width(), nHeightGroupBy);

	// Header
	m_rcHeaderArea.SetRect(0, m_rcGroupByArea.bottom, rcClient.Width(),
						   m_rcGroupByArea.bottom + nHeightHeader);

	// Sections
	nHeightSections = rcClient.Height() - nHeightGroupBy - nHeightHeader - nHeightFooter;
	CRect rcSections(0, m_rcHeaderArea.bottom, rcClient.Width(),
					 m_rcHeaderArea.bottom + nHeightSections);

	m_pSections->AdjustLayout(&dc, rcSections);

	// Footer
	m_rcFooterArea.SetRect(0, rcClient.Height() - nHeightFooter, rcClient.Width(),
						   rcClient.Height());

	if (nHeaderWidth != m_rcHeaderArea.Width() && pHeader)
	{
		pHeader->AdjustColumnsWidth(m_rcHeaderArea.Width());
	}

	m_bAdjustLayoutRunning = FALSE;
}

CScrollBar* CXTPGridControl::GetScrollBarCtrl(int nBar) const
{
#ifdef _XTP_ACTIVEX
	if (GetParent())
		return GetParent()->GetScrollBarCtrl(nBar);
#else
	if (DYNAMIC_DOWNCAST(CView, GetParent()))
		return GetParent()->GetScrollBarCtrl(nBar);
#endif

	return NULL;
}

void CXTPGridControl::RedrawScrollBar(int nBar)
{
	CScrollBar* pScrollBar = GetScrollBarCtrl(nBar);

	if (NULL != pScrollBar)
	{
		SAFE_INVALIDATE(pScrollBar);
	}
	else
	{
		if (NULL != m_hWnd)
		{
			SendMessage(WM_NCPAINT);
		}
	}
}

void CXTPGridControl::GetParametersOfHScrollingBlockCount(int& nTotalCount, int& nScrollPage) const
{
	nTotalCount				   = 0;
	nScrollPage				   = 0;
	CXTPGridColumns* pColumns  = GetColumns();
	int nColumnCount		   = pColumns->GetCount();
	int nColumn				   = 0;
	int nFreezeWidth		   = 0;
	int nLastFreezeColumnIndex = -1;
	GetFreezeColumnsCount(nLastFreezeColumnIndex);

	if (nLastFreezeColumnIndex > -1)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nLastFreezeColumnIndex);
		if (NULL != pColumn)
			nFreezeWidth = pColumn->GetRect().right;
	}
	int nTotalWidth		= GetGridHeader()->GetWidth() - nFreezeWidth;
	int nScrollWidth	= m_pSectionBody->GetRect().Width() - nFreezeWidth;
	int nOutOfViewWidth = nTotalWidth - nScrollWidth;

	for (nColumn = nLastFreezeColumnIndex + 1; nColumn < nColumnCount; nColumn++)
	{
		const CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn->IsVisible())
			++nTotalCount;
	}

	if (nOutOfViewWidth > 0)
	{
		// calc nScrollPage from the end of columns to get correct  nLimit = si.nMax - si.nPage + 1
		// in OnHScroll()
		for (nColumn = nColumnCount - 1; nColumn > nLastFreezeColumnIndex; nColumn--)
		{
			const CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
			if (!pColumn->IsVisible())
				continue;

			nScrollWidth -= pColumn->GetWidth();
			if (nScrollWidth < 0)
				break;

			++nScrollPage;
		}
	}
	else
	{
		nScrollPage = nTotalCount; // disable scrollbar
	}

	if (nScrollPage < 1)
		nScrollPage = 1;
}

void CXTPGridControl::GetParametersOfHScrollingBlockSize(int& nTotalWidth, int& nScrollPage) const
{
	int nFreezeRight		   = 0;
	int nLastFreezeColumnIndex = -1;
	GetFreezeColumnsCount(nLastFreezeColumnIndex);

	if (nLastFreezeColumnIndex > -1)
	{
		CXTPGridColumn* pColumn = GetColumns()->GetAt(nLastFreezeColumnIndex);
		if (NULL != pColumn)
			nFreezeRight = pColumn->GetRect().right;
	}

	nTotalWidth = GetGridHeader()->GetWidth() - nFreezeRight - 1;
	nScrollPage = m_pSectionBody->GetRect().Width() - nFreezeRight;

	int nRightScrollWidth = nScrollPage;

	for (int nColumn = GetColumns()->GetCount() - 1; nColumn > nLastFreezeColumnIndex; nColumn--)
	{
		const CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		nRightScrollWidth -= pColumn->GetWidth();
		if (nRightScrollWidth < 0)
		{
			nTotalWidth += pColumn->GetWidth() + nRightScrollWidth; // add whitespace
			break;
		}
	}
}

void CXTPGridControl::UpdateScrollBarVisibility(int nBar)
{
	CScrollBar* pScrollBar = GetScrollBarCtrl(nBar);
	if (NULL != pScrollBar)
	{
		BOOL bVisible = FALSE;
		switch (nBar)
		{
			case SB_VERT: bVisible = m_bVScrollBarVisible; break;
			case SB_HORZ: bVisible = m_bHScrollBarVisible; break;
			default: ASSERT(!"Not supported scroll bar ID"); break;
		}

		BOOL bCurrentState = (0 != (pScrollBar->GetStyle() & WS_VISIBLE));
		if (bCurrentState != bVisible)
		{
			pScrollBar->ShowWindow(bVisible ? SW_SHOW : SW_HIDE);

			CXTPGridView* pGridView = DYNAMIC_DOWNCAST(CXTPGridView, GetParent());
			if (NULL != pGridView)
			{
				pGridView->UpdateScrollBars();
			}
		}
	}
}

void CXTPGridControl::AdjustScrollBarH(BOOL bUpdate, UINT nMask)
{
	if (bUpdate)
	{
		EditItem(NULL);
		BeginUpdate();
	}

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);

	// Get current values
	GetScrollInfo(SB_HORZ, &si, SIF_ALL);

	if (xtpGridScrollModeBlockCount == m_scrollModeH)
	{
		if ((SIF_RANGE & nMask) || (SIF_PAGE & nMask))
		{
			int nTotalCount, nScrollPage;
			GetParametersOfHScrollingBlockCount(nTotalCount, nScrollPage);

			if (SIF_RANGE & nMask)
				si.nMin = 0;
			if (SIF_RANGE & nMask)
				si.nMax = nTotalCount - 1;
			if (SIF_PAGE & nMask)
				si.nPage = XTPToUIntChecked(nScrollPage);
		}

		if (SIF_POS & nMask)
		{
			si.nPos			 = GetColumns()->GetAtOffset(m_nScrollOffsetH, FALSE);
			m_nScrollOffsetH = GetColumns()->GetOffset(si.nPos);
		}
	}

	if (xtpGridScrollModeBlockSize == m_scrollModeH)
	{
		if ((SIF_RANGE & nMask) || (SIF_PAGE & nMask))
		{
			int nTotalWidth, nScrollPage;
			GetParametersOfHScrollingBlockSize(nTotalWidth, nScrollPage);

			if (SIF_RANGE & nMask)
				si.nMin = 0;
			if (SIF_RANGE & nMask)
				si.nMax = nTotalWidth;
			if (SIF_PAGE & nMask)
				si.nPage = XTPToUIntChecked(nScrollPage);
		}

		if (SIF_POS & nMask)
		{
			int nColumn		 = GetColumns()->GetAtOffset(m_nScrollOffsetH, FALSE);
			m_nScrollOffsetH = GetColumns()->GetOffset(nColumn);
			si.nPos			 = m_nScrollOffsetH;
		}
	}

	if (xtpGridScrollModeSmooth == m_scrollModeH)
	{
		// Update requested values
		if (SIF_RANGE & nMask)
			si.nMin = 0;
		if (SIF_RANGE & nMask)
			si.nMax = GetGridHeader()->GetWidth() - XTP_DPI_X(1);
		if (SIF_PAGE & nMask)
			si.nPage = XTPToUIntChecked(m_pSectionBody->GetRect().Width());
		if (SIF_POS & nMask)
			si.nPos = m_nScrollOffsetH;
	}

	if (xtpGridScrollModeSmooth == m_scrollModeH || xtpGridScrollModeBlockSize == m_scrollModeH)
	{
		si.nPage = min(si.nPage, UINT(si.nMax) + 1);

		// Adjust scroll offset when increasing the section size
		if ((si.nPos > 0) && (si.nPos + int(si.nPage) - 1 > si.nMax))
		{
			m_nScrollOffsetH = XTPToIntChecked(si.nMax - si.nPage + 1);
			si.nPos			 = m_nScrollOffsetH;
		}
	}
	else if (xtpGridScrollModeBlockCount == m_scrollModeH)
	{
		si.nPage = min(si.nPage, UINT(si.nMax) + 1);

		// Adjust scroll offset when increasing the section size
		if ((si.nPos > 0) && (si.nPos + int(si.nPage) - 1 > si.nMax))
		{
			si.nPos = XTPToIntChecked(si.nMax - si.nPage + 1);   // offset in number of columns
			m_nScrollOffsetH = GetColumns()->GetOffset(si.nPos); // offset in pixels
		}
	}

	// Set new values
	si.fMask = nMask;
	SetScrollInfo(SB_HORZ, &si); // Note: Causes an OnSize message

	// si.nMax is -1 for no rows
	BOOL bEnabled = (-1 != si.nMax) && (UINT(si.nMax) >= UINT(si.nPage));

	CWnd::EnableScrollBarCtrl(SB_HORZ, bEnabled);
	::EnableScrollBar(m_hWnd, SB_HORZ,
					  UINT((bEnabled && IsWindowEnabled()) ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH));

	if (m_bHScrollBarVisible != bEnabled)
	{
		m_bHScrollBarVisible = bEnabled;
		UpdateScrollBarVisibility(SB_HORZ);
	}

	if (bUpdate)
	{
		EndUpdate();
	}
}

int CXTPGridControl::GetPageRowCount(CDC* pDC) const
{
	int nCount		= m_pRows->GetCount() - 1;
	int nLinesCount = nCount;

	int nHeight = m_pSectionBody->GetRect().Height();
	int nWidth  = m_pSectionBody->GetRect().Width();

	for (; nLinesCount >= 0; nLinesCount--)
	{
		CXTPGridRow* pRow = m_pRows->GetAt(nLinesCount);
		ASSERT(NULL != pRow);

		nHeight -= pRow->GetHeight(pDC, nWidth);

		if (nHeight < 0)
		{
			if (nLinesCount != nCount)
				nLinesCount += IsIconView() ? GetRowsPerLine()
											: 1; // Want the next row, in icon view, that is an
												 // addition of an entire row.

			break;
		}
	}

	nLinesCount = max(nLinesCount, 0);

	return nLinesCount;
}

void CXTPGridControl::AdjustScrollBarV(BOOL bUpdate, UINT nMask)
{
	CClientDC dc(this);

	if (bUpdate)
	{
		EditItem(NULL);
		BeginUpdate();
	}

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);

	// Get current values
	GetScrollInfo(SB_VERT, &si, SIF_ALL);

	if (xtpGridScrollModeBlockCount == m_scrollModeV || xtpGridScrollModeBlockSize == m_scrollModeV)
	{
		int nMax  = m_pSectionBody->GetRows()->GetCount() - 1;
		int nPage = GetPageRowCount(&dc);

		// Update requested values
		if (SIF_RANGE & nMask)
			si.nMin = 0;
		if (SIF_RANGE & nMask)
			si.nMax = nMax;
		if (SIF_PAGE & nMask)
			si.nPage = XTPToUIntChecked(nMax - nPage + 1);
		if (SIF_POS & nMask)
			si.nPos = GetTopRowIndex();

		si.nPage = min(si.nPage, UINT(si.nMax) + 1);

		// Adjust scroll offset when increasing the section size
		if ((si.nPos > 0) && (si.nPos + int(si.nPage) - 1 > si.nMax))
		{
			si.nPos = XTPToIntChecked(si.nMax - si.nPage + 1);
			m_pSectionScroll->SetScrollIndexV(si.nPos);
		}
	}

	if (xtpGridScrollModeSmooth == m_scrollModeV)
	{
		// Update requested values
		if (SIF_RANGE & nMask)
			si.nMin = 0;
		if (SIF_RANGE & nMask)
			si.nMax = m_pSectionScroll->GetRowsHeight(&dc) - 1;
		if (SIF_PAGE & nMask)
			si.nPage = XTPToUIntChecked(m_pSectionScroll->GetRect().Height());
		if (SIF_POS & nMask)
			si.nPos = m_pSectionScroll->GetScrollOffsetV();

		si.nPage = min(si.nPage, UINT(si.nMax) + 1);

		// Adjust scroll offset when increasing the section size
		if ((si.nPos > 0) && (si.nPos + int(si.nPage) - 1 > si.nMax))
		{
			si.nPos = XTPToIntChecked(si.nMax - si.nPage + 1);
			m_pSectionScroll->SetScrollOffsetV(si.nPos);
		}
	}

	// Set new values
	si.fMask = nMask;
	SetScrollInfo(SB_VERT, &si); // Note: Causes an OnSize message

	// si.nMax is -1 for no rows
	BOOL bEnabled = (-1 != si.nMax) && (UINT(si.nMax) >= UINT(si.nPage));

	CWnd::EnableScrollBarCtrl(SB_VERT, bEnabled);
	::EnableScrollBar(m_hWnd, SB_VERT,
					  UINT((bEnabled && IsWindowEnabled()) ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH));

	if (m_bVScrollBarVisible != bEnabled)
	{
		m_bVScrollBarVisible = bEnabled;
		UpdateScrollBarVisibility(SB_VERT);
	}

	if (bUpdate)
	{
		EndUpdate();
	}
}

void CXTPGridControl::UpdateScrollBarV()
{
	AdjustScrollBarV(TRUE, SIF_PAGE | SIF_POS); // Only update page and position

	// Send notification
	XTP_NM_GRIDSCROLL msg;
	msg.nSection = m_pSectionScroll->GetIndex();

	switch (m_scrollModeV)
	{
		case xtpGridScrollModeSmooth: msg.nPosition = m_pSectionScroll->GetScrollOffsetV(); break;
		case xtpGridScrollModeBlockCount:
		case xtpGridScrollModeBlockSize: msg.nPosition = m_pSectionScroll->GetScrollIndexV(); break;
	}
	SendNotifyMessage(XTP_NM_GRID_VSCROLL, &msg);
}

void CXTPGridControl::AdjustScrollBars()
{
	if (GetSafeHwnd() == NULL)
		return;

	EditItem(NULL);

	int nHeight = m_pSectionBody->GetRect().Height();

	if (nHeight <= 0)
		return;

	AdjustScrollBarH(FALSE, SIF_ALL);
	AdjustScrollBarV(FALSE, SIF_ALL);
}

void CXTPGridControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if (!m_bIconView)
	{
		SCROLLINFO si = { 0 };
		si.cbSize	 = sizeof(SCROLLINFO);
		GetScrollInfo(SB_VERT, &si, SIF_ALL);

		const int nLimit = XTPToIntChecked(si.nMax - si.nPage + 1); // GetScrollLimit

		if (xtpGridScrollModeSmooth == m_scrollModeV)
		{
			int nScrollOffsetV = m_pSectionScroll->GetScrollOffsetV();

			switch (nSBCode)
			{
				case SB_LINEUP: // 0
					nScrollOffsetV = max(nScrollOffsetV - m_nScrollStepV, 0);
					break;
				case SB_LINEDOWN: // 1
					nScrollOffsetV = min(nScrollOffsetV + m_nScrollStepV, nLimit);
					break;
				case SB_PAGEUP: // 2
					nScrollOffsetV = max(nScrollOffsetV - int(si.nPage), 0);
					break;
				case SB_PAGEDOWN: // 3
					nScrollOffsetV = min(nScrollOffsetV + int(si.nPage), nLimit);
					break;
				case SB_THUMBPOSITION: // 4
				case SB_THUMBTRACK:	// 5
					nScrollOffsetV = si.nTrackPos;
					break;
				case SB_TOP: // 6
					nScrollOffsetV = 0;
					break;
				case SB_BOTTOM: // 7
					nScrollOffsetV = nLimit;
					break;
				case SB_ENDSCROLL: // 8
					break;
			}

			if (-1 != nScrollOffsetV)
			{
				SetScrollOffsetV(nScrollOffsetV);
			}
		} // smooth

		if (xtpGridScrollModeBlockCount == m_scrollModeV
			|| xtpGridScrollModeBlockSize == m_scrollModeV)
		{
			int nTopRowPos = -1;
			switch (nSBCode)
			{
				case SB_LINEUP: // 0
					nTopRowPos = max(si.nPos - 1, 0);
					break;
				case SB_LINEDOWN: // 1
					nTopRowPos = min(si.nPos + 1, nLimit);
					break;
				case SB_PAGEUP: // 2
					nTopRowPos = max(si.nPos - max(1, GetGridAreaRows(si.nPos, FALSE)), 0);
					break;
				case SB_PAGEDOWN: // 3
					nTopRowPos = min(si.nPos + max(1, GetGridAreaRows(si.nPos, TRUE)), nLimit);
					break;
				case SB_THUMBPOSITION: // 4
				case SB_THUMBTRACK:	// 5
					nTopRowPos = si.nTrackPos;
					break;
				case SB_TOP: // 6
					nTopRowPos = 0;
					break;
				case SB_BOTTOM: // 7
					nTopRowPos = nLimit;
					break;
				case SB_ENDSCROLL: // 8
					break;
			}

			if (-1 != nTopRowPos)
			{
				SetTopRow(nTopRowPos);
			}
		}
	}
	else
	{
		int nCurPos = GetTopRowIndex();

		CXTPGridColumn* pColumn = GetColumns()->Find(m_iIconViewColumn);
		if (!pColumn)
		{
			CXTPGridControl::OnVScroll(nSBCode, nPos, pScrollBar);
			return;
		}

		// decide what to do for each diffrent scroll event
		switch (nSBCode)
		{
			case SB_TOP: nCurPos = 0; break;
			case SB_BOTTOM: nCurPos = GetScrollLimit(SB_VERT); break;
			case SB_LINEUP: nCurPos = max(nCurPos - m_iIconViewRowsPerLine, 0); break;
			case SB_LINEDOWN:
				nCurPos = min(nCurPos + m_iIconViewRowsPerLine, GetScrollLimit(SB_VERT));
				break;
			case SB_PAGEUP: nCurPos = max(nCurPos - GetGridAreaRows(nCurPos, FALSE), 0); break;
			case SB_PAGEDOWN:
				nCurPos = min(nCurPos + GetGridAreaRows(nCurPos, TRUE), GetScrollLimit(SB_VERT));
				break;
			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
			{
				SCROLLINFO si = { 0 };
				si.cbSize	 = sizeof(SCROLLINFO);
				si.fMask	  = SIF_TRACKPOS;

				if (!GetScrollInfo(SB_VERT, &si))
					return;
				nCurPos = si.nTrackPos;
			}
			break;
		}

		SetTopRow(nCurPos);
	}
}

void CXTPGridControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	int nCurPos = m_nScrollOffsetH;

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ, &si, SIF_ALL);

	const int nLimit = XTPToIntChecked(si.nMax - si.nPage + 1); // GetScrollLimit

	if (xtpGridScrollModeSmooth == m_scrollModeH)
	{
		switch (nSBCode)
		{
			case SB_LINEUP: // 0
				nCurPos = max(nCurPos - m_nScrollStepH, 0);
				break;
			case SB_LINEDOWN: // 1
				nCurPos = min(nCurPos + m_nScrollStepH, nLimit);
				break;
			case SB_PAGEUP: // 2
				nCurPos = max(nCurPos - m_pSectionBody->GetRect().Width(), 0);
				break;
			case SB_PAGEDOWN: // 3
				nCurPos = min(nCurPos + m_pSectionBody->m_rcSection.Width(), nLimit);
				break;
			case SB_THUMBPOSITION: // 4
			case SB_THUMBTRACK:	// 5
				nCurPos = si.nTrackPos;
				break;
			case SB_TOP: // 6
				nCurPos = 0;
				break;
			case SB_BOTTOM: // 7
				nCurPos = nLimit;
				break;
			case SB_ENDSCROLL: // 8
				break;
		}
	}

	if (xtpGridScrollModeBlockCount == m_scrollModeH)
	{
		CXTPGridColumn *pPrev = NULL, *pCurr = NULL, *pNext = NULL;
		int nScrollPos = 0, nScrollMax = 0;
		GetGridHeader()->GetFullColScrollInfo(pPrev, pCurr, pNext, nScrollPos, nScrollMax);

		switch (nSBCode)
		{
			case SB_LINEUP: // 0
			case SB_PAGEUP: // 2
			{
				if (NULL != pPrev)
				{
					nCurPos = max(nCurPos - pPrev->GetWidth(), 0);
				}
				else
					nCurPos = 0;
				break;
			}
			case SB_LINEDOWN: // 1
			case SB_PAGEDOWN: // 3
			{
				if (NULL != pCurr)
				{
					int nLimitOffset = GetColumns()->GetOffset(nLimit);
					nCurPos			 = min(nCurPos + pCurr->GetWidth(), nLimitOffset);
				}
				break;
			}
			case SB_THUMBPOSITION: // 4
			case SB_THUMBTRACK:	// 5
			{
				nCurPos = GetColumns()->GetOffset(si.nTrackPos);
				break;
			}
			case SB_TOP: // 6
				nCurPos = 0;
				break;
			case SB_BOTTOM: // 7
				nCurPos = GetColumns()->GetOffset(nLimit);
				break;
			case SB_ENDSCROLL: // 8
				break;
		}
	}

	if (xtpGridScrollModeBlockSize == m_scrollModeH)
	{
		CXTPGridColumn *pPrev = NULL, *pCurr = NULL, *pNext = NULL;
		int nScrollPos = 0, nScrollMax = 0;
		GetGridHeader()->GetFullColScrollInfo(pPrev, pCurr, pNext, nScrollPos, nScrollMax);

		switch (nSBCode)
		{
			case SB_LINEUP: // 0
			case SB_PAGEUP: // 2
			{
				if (NULL != pPrev)
				{
					nCurPos = max(nCurPos - pPrev->GetWidth(), 0);
				}
				break;
			}
			case SB_LINEDOWN: // 1
			case SB_PAGEDOWN: // 3
			{
				if (NULL != pCurr)
				{
					nCurPos = min(nCurPos + pCurr->GetWidth(), nLimit);
				}
				break;
			}
			case SB_THUMBPOSITION: // 4
			case SB_THUMBTRACK:	// 5
			{
				int nColumn = GetColumns()->GetAtOffset(si.nTrackPos, FALSE);
				nCurPos		= GetColumns()->GetOffset(nColumn);
				break;
			}
			case SB_TOP: // 6
				nCurPos = 0;
				break;
			case SB_BOTTOM: // 7
				nCurPos = nLimit;
				break;
			case SB_ENDSCROLL: // 8
				break;
		}
	}

	SetScrollOffsetH(nCurPos);
}

HBITMAP CXTPGridControl::GetWatermarkBitmap() const
{
	return m_hbmpWatermark;
}

BOOL CXTPGridControl::SetWatermarkBitmap(HBITMAP hBitmap, BYTE Transparency)
{
	m_WatermarkTransparency = Transparency;

	// remove old watermark bitmap if one is
	if (m_hbmpWatermark)
	{
		::DeleteObject(m_hbmpWatermark);
		m_hbmpWatermark = NULL;
	}

	if (hBitmap)
	{
		// add new watermark bitmap

		BITMAP bmSrc;
		if (!CBitmap::FromHandle(hBitmap)->GetBitmap(&bmSrc))
			return FALSE;

		BOOL bAlphaBitmap = bmSrc.bmBitsPixel == 32;

		// create an alpha bitmap if the bitmap supplied isn't an alpha one
		if (!bAlphaBitmap)
		{
			m_hbmpWatermark = ::CreateBitmap(bmSrc.bmWidth, bmSrc.bmHeight, bmSrc.bmPlanes, 32,
											 NULL);

			ASSERT(m_hbmpWatermark);
			if (!m_hbmpWatermark)
				return FALSE;

			CXTPCompatibleDC memWatermarkDC(NULL, m_hbmpWatermark);
			CXTPCompatibleDC memScrDC(NULL, hBitmap);

			memWatermarkDC.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight, &memScrDC, 0, 0, SRCCOPY);
		}
		else
		{
			m_hbmpWatermark = CXTPImageManagerIcon::CopyAlphaBitmap(hBitmap);
		}

		if (!m_hbmpWatermark)
			return FALSE;

		if (!CBitmap::FromHandle(m_hbmpWatermark)->GetBitmap(&m_bmWatermark))
		{
			::DeleteObject(m_hbmpWatermark);
			m_hbmpWatermark = NULL;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CXTPGridControl::SetWatermarkBitmap(LPCTSTR szPath, BYTE Transparency)
{
	m_WatermarkTransparency = Transparency;

	// if path is empty, remove watermark bitmap
	if (_tcslen(szPath) == 0)
	{
		::DeleteObject(m_hbmpWatermark);
		m_hbmpWatermark = NULL;
	}
	else
	{
		// remove old watermark bitmap if one is
		if (m_hbmpWatermark)
		{
			::DeleteObject(m_hbmpWatermark);
			m_hbmpWatermark = NULL;
		}
		// add new watermark bitmap
		BOOL bAlphaBitmap = FALSE;
		m_hbmpWatermark   = CXTPImageManagerIcon::LoadBitmapFromFile(szPath, &bAlphaBitmap);
		if (!m_hbmpWatermark)
			return FALSE;
		if (!CBitmap::FromHandle(m_hbmpWatermark)->GetBitmap(&m_bmWatermark))
		{
			::DeleteObject(m_hbmpWatermark);
			m_hbmpWatermark = NULL;
			return FALSE;
		}
		// create an alpha bitmap if the bitmap supplied isn't an alpha one
		if (!bAlphaBitmap)
		{
			CXTPCompatibleDC memWatermarkDC(NULL, m_hbmpWatermark);
			CXTPImageManagerIcon::DrawAlphaBitmap(&memWatermarkDC, m_hbmpWatermark, CPoint(0, 0),
												  CSize(m_bmWatermark.bmWidth,
														m_bmWatermark.bmHeight));
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridControl message handlers

void CXTPGridControl::Paint(CDC* pDC)
{
	OnDraw(pDC);

	// Update flag
	SetChanged(FALSE);

	if (!IsWindowEnabled() && GetPaintManager()->m_bGrayIfDisable)
	{
		CXTPClientRect rcClient(this);
		XTPImageManager()->DisableBitmap(*pDC, rcClient,
										 GetPaintManager()->m_clrDisableBitmapLightest,
										 GetPaintManager()->m_clrDisableBitmapDarkest);
	}
}

void CXTPGridControl::EnableDoubleBuffering(BOOL bEnable)
{
	m_bDoubleBuffering = bEnable;

	// Delete buffer
	if (!m_bDoubleBuffering && NULL != m_bmpCache.GetSafeHandle())
	{
		m_bmpCache.DeleteObject();
	}
}

void CXTPGridControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CXTPClientRect rcClient(this);

	// ensure visible row and column if first started
	if (m_nEnsureVisibleRowIdx >= 0)
	{
		CXTPGridRow* pRow = GetRows()->GetAt(m_nEnsureVisibleRowIdx);
		if (NULL != pRow)
		{
			EnsureVisible(pRow);
		}
		m_nEnsureVisibleRowIdx = -1;
	}

	if (m_nEnsureVisibleColumnIdx >= 0)
	{
		EnsureVisible(GetColumns()->GetAt(m_nEnsureVisibleColumnIdx));
		m_nEnsureVisibleColumnIdx = -1;
	}

	// start counting drawing time
#ifdef XTP_DEBUG
	LARGE_INTEGER iStartCount;
	QueryPerformanceCounter(&iStartCount);
#endif

	if (m_bDoubleBuffering)
	{
		// Update cached bitmap
		if (m_nLockUpdateCount == 0 && (IsChanged() || NULL == m_bmpCache.GetSafeHandle()))
		{
			m_bmpCache.DeleteObject();

			CDC memDC;

			if (memDC.CreateCompatibleDC(&dc))
			{
				if (m_bmpCache.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height()))
				{
					CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);
					Paint(&memDC);
					memDC.SelectObject(pOldBitmap);
				}
			}
		}
	}

	// Draw cached bitmap if available
	if (NULL != m_bmpCache.GetSafeHandle())
	{
		CXTPCompatibleDC memDC(&dc, &m_bmpCache);
		dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &memDC, 0, 0, SRCCOPY);
	}
	// Draw directly to device context
	else
	{
		Paint(&dc);
	}

	// count drawing time
#ifdef XTP_DEBUG
	LARGE_INTEGER iEndCount;
	QueryPerformanceCounter(&iEndCount);
	XTP_TRACE(_T("Draw counter ticks: %d\n"), iEndCount.LowPart - iStartCount.LowPart);
#endif
}

void CXTPGridControl::OnNcPaint()
{
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	CWindowDC dc(this);
	GetPaintManager()->DrawWindowFrame(&dc, rc);
}

void CXTPGridControl::DrawWatermark(CDC* pDC, CRect rcWatermark, CRect rcClient)
{
	if (m_hbmpWatermark)
	{
		CRect rcDst(rcWatermark);
		CRect rcSrc(0, 0, m_bmWatermark.bmWidth, m_bmWatermark.bmHeight);

		// no stretch
		if (!(GetWatermarkAlignment() & xtpGridWatermarkStretch))
		{
			if (rcSrc.Width() > rcDst.Width())
			{
				rcSrc.right = rcSrc.left + rcDst.Width();
			}
			else
			{
				rcDst.right = rcDst.left + rcSrc.Width();
			}

			if (rcSrc.Height() > rcDst.Height())
			{
				rcSrc.bottom = rcSrc.top + rcDst.Height();
			}
			else
			{
				rcDst.bottom = rcDst.top + rcSrc.Height();
			}
		}

		// enlarge only
		if ((GetWatermarkAlignment() & xtpGridWatermarkStretch)
			&& (GetWatermarkAlignment() & xtpGridWatermarkEnlargeOnly))
		{
			if (rcSrc.Width() > rcDst.Width())
			{
				rcSrc.right = rcSrc.left + rcDst.Width();
			}
			if (rcSrc.Height() > rcDst.Height())
			{
				rcSrc.bottom = rcSrc.top + rcDst.Height();
			}
		}

		// shrink only
		if ((GetWatermarkAlignment() & xtpGridWatermarkStretch)
			&& (GetWatermarkAlignment() & xtpGridWatermarkShrinkOnly))
		{
			if (rcSrc.Width() < rcDst.Width())
			{
				rcDst.right = rcDst.left + rcSrc.Width();
			}
			if (rcSrc.Height() < rcDst.Height())
			{
				rcDst.bottom = rcDst.top + rcSrc.Height();
			}
		}

		// preserve aspect ratio
		if ((GetWatermarkAlignment() & xtpGridWatermarkStretch)
			&& (GetWatermarkAlignment() & xtpGridWatermarkPreserveRatio))
		{
			if (rcDst.Width() > (rcDst.Height() * rcSrc.Width() / rcSrc.Height()))
			{
				rcDst.right = rcDst.left + rcDst.Height() * rcSrc.Width() / rcSrc.Height();
			}
			if (rcDst.Height() > (rcDst.Width() * rcSrc.Height() / rcSrc.Width()))
			{
				rcDst.bottom = rcDst.top + rcDst.Width() * rcSrc.Height() / rcSrc.Width();
			}
		}

		// horizontal
		switch (GetWatermarkAlignment() & xtpGridWatermarkHmask)
		{
			// center
			case xtpGridWatermarkCenter:
				rcDst.OffsetRect((rcWatermark.Width() - rcDst.Width()) / 2, 0);
				break;
			// right
			case xtpGridWatermarkRight:
				rcDst.OffsetRect(rcWatermark.Width() - rcDst.Width(), 0);
				break;
			// left
			default: break;
		}

		// vertical
		switch (GetWatermarkAlignment() & xtpGridWatermarkVmask)
		{
			// center
			case xtpGridWatermarkVCenter:
				rcDst.OffsetRect(0, (rcWatermark.Height() - rcDst.Height()) / 2);
				break;
			// bottom
			case xtpGridWatermarkBottom:
				rcDst.OffsetRect(0, rcWatermark.Height() - rcDst.Height());
				break;
			// top
			default: break;
		}

		CXTPCompatibleDC memWatermarkDC(pDC, m_hbmpWatermark);

		if (GetWatermarkAlignment() & xtpGridWatermarkStretch)
		{
			CBitmap bmpWatermark;
			bmpWatermark.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

			CXTPCompatibleDC dcWatermark(pDC, &bmpWatermark);

			dcWatermark.FillSolidRect(&rcClient, RGB(255, 255, 255));
			dcWatermark.SetStretchBltMode(HALFTONE);
			dcWatermark.StretchBlt(rcDst.left, rcDst.top, rcDst.Width(), rcDst.Height(),
								   &memWatermarkDC, 0, 0, rcSrc.Width(), rcSrc.Height(), SRCCOPY);

			XTPImageManager()->AlphaBlend2(*pDC, rcClient, dcWatermark, rcClient,
										   m_WatermarkTransparency);
		}
		else
		{
			XTPImageManager()->AlphaBlend2(*pDC, rcDst, memWatermarkDC, rcSrc,
										   m_WatermarkTransparency);
		}
	}
}

LRESULT CXTPGridControl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		if (m_bmpCache.GetSafeHandle() == 0)
			OnDraw(pDC);
		else
		{
			CXTPCompatibleDC memDC(pDC, &m_bmpCache);
			CXTPClientRect rc(this);
			pDC->BitBlt(0, 0, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
		}
	}

	return TRUE;
}

BOOL CXTPGridControl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	return TRUE; // Don't erase the background.
}

void CXTPGridControl::OnDraw(CDC* pDC)
{
	CXTPClientRect rcClient(this);

	pDC->SetBkMode(TRANSPARENT);

	if (GetExStyle() & WS_EX_RTLREADING)
	{
		pDC->SetTextAlign(TA_RTLREADING);
	}

	// Fill background
	pDC->FillSolidRect(rcClient, GetPaintManager()->GetControlBackColor(this));

	// Draw watermark
	DrawWatermark(pDC, m_pSections->GetRect(), rcClient);

	// Draw header
	CXTPGridHeader* pHeader = GetGridHeader();

	if (NULL != pHeader)
	{
		CRect rcHeader = m_rcHeaderArea;
		rcHeader.right = rcHeader.left + pHeader->GetWidth();
		pHeader->Draw(pDC, rcHeader, m_nScrollOffsetH);	// draw header
		pHeader->DrawGroupByControl(pDC, m_rcGroupByArea); // draw group by box
	}

	if (m_bIconView)
	{
		DrawIconView(pDC, m_pSectionBody->m_rcSection);
	}
	else
	{
		m_pSections->Draw(pDC);
	}

	CRect rcFooter = m_rcFooterArea;
	if (rcFooter.Height() > 0)
		pHeader->DrawFooter(pDC, rcFooter, m_nScrollOffsetH);

	BOOL bForce(FALSE);
	BOOL bDrawBetween = FALSE;
	int yForce(-1);
	if (m_bDragMode && GetPaintManager() && !IsIconView())
	{
		bForce = GetPaintManager()->m_bForceShowDropMarker;
		CPoint ptMouse;
		if (::GetCursorPos(&ptMouse))
		{
			ScreenToClient(&ptMouse);

			EnsureStartAutoVertScroll();

			if (m_dwDropMarkerFlags & xtpGridDropBetween)
			{
				CXTPGridRow* pHitRow = HitTest(ptMouse);
				if (pHitRow && pHitRow->GetRecord())
				{
					if (bForce || !(m_dwDropMarkerFlags & xtpGridDropSelect)
						|| (m_pSelectedRows->GetCount() == 0
							&& (ptMouse.y <= (pHitRow->GetRect().top + XTP_DPI_Y(4))
								|| ptMouse.y >= (pHitRow->GetRect().bottom - XTP_DPI_Y(4)))))
					{
						bDrawBetween = TRUE;
						BOOL bAbove  = TRUE;
						if (pHitRow->GetRect().CenterPoint().y < ptMouse.y)
							bAbove = FALSE;
						if (bAbove)
							yForce = pHitRow->GetRect().top - XTP_DPI_Y(1);
						else
							yForce = pHitRow->GetRect().bottom - XTP_DPI_Y(1);
					}
				}
			}
		}
	}

	if (bDrawBetween)
	{
		if (m_nDropPos != -1 && GetPaintManager()->m_bUseDropMarker)
		{
			DrawDropMarker(pDC);
		}
		else if (bForce && yForce > 0 && GetPaintManager()->m_bUseDropMarker)
		{
			DrawExtDropMarker(pDC, yForce);
		}
	}

	// update flag
	SetChanged(FALSE);
}

void CXTPGridControl::DrawDropMarker(CDC* pDC)
{
	if (m_nDropPos == 0 && GetRows()->GetCount() == 0)
	{
		m_nDropPos = m_pSectionBody->m_rcSection.top;
	}
	else if (m_nDropPos < m_pSectionBody->m_rcSection.top - XTP_DPI_Y(1)
			 || m_nDropPos > m_pSectionBody->m_rcSection.bottom)
	{
		return;
	}

	CRect rc(m_pSectionBody->m_rcSection.left, m_nDropPos, m_pSectionBody->m_rcSection.right,
			 m_nDropPos + 1);

	pDC->FillSolidRect(rc, GetPaintManager()->m_clrHotDivider);
	CXTPPenDC pen(*pDC, GetPaintManager()->m_clrHotDivider);
	CXTPBrushDC brush(*pDC, GetPaintManager()->m_clrHotDivider);

	int x = rc.left;
	int y = m_nDropPos;

	POINT ptsLeftArrow[] = { { x, y - XTP_DPI_Y(2) },
							 { x + XTP_DPI_X(2), y - XTP_DPI_Y(2) },
							 { x + XTP_DPI_X(2), y - XTP_DPI_Y(5) },
							 { x + XTP_DPI_X(7), y },
							 { x + XTP_DPI_X(2), y + XTP_DPI_Y(5) },
							 { x + XTP_DPI_X(2), y + XTP_DPI_Y(2) },
							 { x, y + XTP_DPI_Y(2) } };
	pDC->Polygon(ptsLeftArrow, 7);

	x = rc.right - XTP_DPI_X(1);

	POINT ptsRightArrow[] = { { x, y - XTP_DPI_Y(2) },
							  { x - XTP_DPI_X(2), y - XTP_DPI_Y(2) },
							  { x - XTP_DPI_X(2), y - XTP_DPI_Y(5) },
							  { x - XTP_DPI_X(7), y },
							  { x - XTP_DPI_X(2), y + XTP_DPI_Y(5) },
							  { x - XTP_DPI_X(2), y + XTP_DPI_Y(2) },
							  { x, y + XTP_DPI_Y(2) } };
	pDC->Polygon(ptsRightArrow, 7);
}

void CXTPGridControl::DrawExtDropMarker(CDC* pDC, int y)
{
	CRect rc(m_pSectionBody->m_rcSection.left, y, m_pSectionBody->m_rcSection.right,
			 y + XTP_DPI_Y(1));

	pDC->FillSolidRect(rc, GetPaintManager()->m_clrHotDivider);
	CXTPPenDC pen(*pDC, GetPaintManager()->m_clrHotDivider);
	CXTPBrushDC brush(*pDC, GetPaintManager()->m_clrHotDivider);

	int x = rc.left;

	POINT ptsLeftArrow[] = { { x, y - XTP_DPI_Y(2) },
							 { x + XTP_DPI_X(2), y - XTP_DPI_Y(2) },
							 { x + XTP_DPI_X(2), y - XTP_DPI_Y(5) },
							 { x + XTP_DPI_X(7), y },
							 { x + XTP_DPI_X(2), y + XTP_DPI_Y(5) },
							 { x + XTP_DPI_X(2), y + XTP_DPI_Y(2) },
							 { x, y + XTP_DPI_Y(2) } };
	pDC->Polygon(ptsLeftArrow, 7);

	x = rc.right - XTP_DPI_X(1);

	POINT ptsRightArrow[] = { { x, y - XTP_DPI_Y(2) },
							  { x - XTP_DPI_X(2), y - XTP_DPI_Y(2) },
							  { x - XTP_DPI_X(2), y - XTP_DPI_Y(5) },
							  { x - XTP_DPI_X(7), y },
							  { x - XTP_DPI_X(2), y + XTP_DPI_Y(5) },
							  { x - XTP_DPI_X(2), y + XTP_DPI_Y(2) },
							  { x, y + XTP_DPI_Y(2) } };
	pDC->Polygon(ptsRightArrow, 7);
}

BOOL CXTPGridControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
							 CCreateContext* pContext)
{
	return CXTPGridControl::Create(XTPGRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID,
								   pContext);
}

BOOL CXTPGridControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
							 const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	m_bCreated = TRUE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CXTPGridControl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	// DDX_Control subclasses the window and OnCreate and OnSize will not be called
	if (!m_bCreated)
	{
		AdjustLayout();
		AdjustScrollBars();
	}
}

int CXTPGridControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (0 != CWnd::OnCreate(lpCreateStruct))
	{
		// CWnd creation failed
		return -1;
	}

	return 0;
}

void CXTPGridControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	BeginUpdate();

	EditItem(NULL);
	AdjustLayout();
	AdjustScrollBars();

	EndUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// Left button
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::OnButton(UINT nFlags, CPoint point, CXTPGridBehaviorRowMouseButton* pBehavior)
{
	UNREFERENCED_PARAMETER(nFlags);

	XTPGridInputState inputState;

	// Get input states
	inputState.ptMouse	 = point;
	inputState.bKeyControl = (::GetKeyState(VK_CONTROL) < 0) || m_bMultiSelectionMode;
	inputState.bKeyShift   = (::GetKeyState(VK_SHIFT) < 0);
	inputState.nRow		   = -1;
	inputState.bSelected   = FALSE;

	// Click arguments
	XTP_GRIDRECORDITEM_CLICKARGS clickArgs;
	clickArgs.pControl = this;
	clickArgs.ptClient = point;
	clickArgs.pColumn  = NULL;
	clickArgs.pRow	 = NULL;
	clickArgs.pItem	= NULL;

	// Find clicked row
	clickArgs.pRow = HitTest(point);

	CXTPSmartPtrInternalT<CCmdTarget> ptrRowLock(clickArgs.pRow, TRUE);

	if (NULL != clickArgs.pRow)
	{
		inputState.nRow		 = clickArgs.pRow->GetIndex();
		inputState.bSelected = clickArgs.pRow->IsSelected();

		// Find clicked item and column
		clickArgs.pItem = clickArgs.pRow->HitTest(point, &clickArgs.rcItem, &clickArgs.pColumn);
	}

	// Store state
	if (xtpGridMouseEventButtonDown == pBehavior->m_event)
	{
		m_mouseDownState = inputState;
	}

	CXTPGridBehaviorRowModifier* pModifier = NULL;

	if (m_mouseDownState.bKeyShift)
		pModifier = pBehavior->Shift; // Shift
	else if (m_mouseDownState.bKeyControl)
		pModifier = pBehavior->Control; // Control
	else
		pModifier = pBehavior->None; // No modifier

	// Current focused row index
	const int nFocusedRowIndex = _GetFocusedRowIndex();

	if (xtpGridMouseEventButtonDown == pBehavior->m_event)
	{
		SetFocus();

		// If an item is currently edited, this need to be ended first.
		// EditItem(NULL) sends a XTP_NM_GRID_VALUECHANGED message
		// and the user may modify the rows.
		EditItem(NULL);
	}

	// Notify header
	CXTPGridHeader* pHeader = GetGridHeader();
	if (NULL != pHeader)
	{
		if (xtpGridMouseButtonLeft == pBehavior->m_button)
		{
			if (xtpGridMouseEventButtonDown == pBehavior->m_event)
			{
				pHeader->OnLButtonDown(point);
			}
			if (xtpGridMouseEventButtonUp == pBehavior->m_event)
			{
				pHeader->OnLButtonUp(nFlags, point);
			}
		}
	}

	if (NULL != clickArgs.pRow)
	{
		BOOL bHandled = FALSE;

		// Notify row
		if (xtpGridMouseButtonLeft == pBehavior->m_button)
		{
			if (xtpGridMouseEventButtonDown == pBehavior->m_event)
			{
				bHandled = clickArgs.pRow->OnLButtonDown(&clickArgs);
			}
			if (xtpGridMouseEventButtonUp == pBehavior->m_event)
			{
				bHandled = clickArgs.pRow->OnLButtonUp(&clickArgs);
			}
		}

		// Notify inplace control
		if (NULL != clickArgs.pItem)
		{
		}

		if (!bHandled && clickArgs.pRow->GetSection()->IsAllowAccess())
		{
			BeginUpdate();

			// Begin drag
			if (pBehavior->bDragBegin)
			{
				m_ptDrag	   = point;
				m_bPrepareDrag = TRUE;
			}

			// End drag
			if (pBehavior->bDragEnd)
			{
				// Stop dragging
				m_bPrepareDrag = FALSE;

				// Stop auto scrolling
				EnsureStopAutoVertScroll();
			}

			BOOL bKeepSelection		  = FALSE;
			BOOL bKeepFocus			  = FALSE;
			BOOL bDeferredDeselection = FALSE;
			BOOL bFocused			  = TRUE;

			const BOOL bSelected = clickArgs.pRow->IsSelected();

			if (bSelected)
			{
				if (pModifier->bKeepSelection)
				{
					bKeepSelection = TRUE;
				}

				if (pModifier->bKeepFocus)
				{
					bKeepFocus = TRUE;
				}
			}

			// Focus column
			if (pBehavior->bFocusColumn && m_bFocusSubItems)
			{
				if (clickArgs.pItem && clickArgs.pItem->IsFocusable())
				{
					bFocused = SetFocusedColumn(clickArgs.pColumn);
				}
				else
				{
					bFocused = SetFocusedColumn(NULL);
				}
			}

			// Focus row
			if ((pModifier->bFocusRow || pModifier->bFocusRowTemporarily) && !bKeepFocus)
			{
				bFocused = _SetFocusedRow(clickArgs.pRow);
			}

			// Deferred deselection
			if (m_mouseDownState.bKeyControl && m_mouseDownState.bSelected
				&& m_mouseDownState.nRow == clickArgs.pRow->GetIndex()
				&& xtpGridMouseEventButtonUp == pBehavior->m_event)
			{
				bDeferredDeselection = TRUE;
			}

			// Select row(s)
			if ((pModifier->bSelectRow && !bKeepSelection) || bDeferredDeselection)
			{
				if (pModifier->bMultipleSelection)
				{
					_SetSelectedRow(clickArgs.pRow, nFocusedRowIndex, inputState.bKeyShift,
									inputState.bKeyControl);

					if (IsSelectionEnabled() && IsMultipleSelection() && !inputState.bKeyShift
						&& inputState.bKeyControl)
					{
						m_pSelectedRows->Invert(clickArgs.pRow);
					}
				}
				else
				{
					_SetSelectedRow(clickArgs.pRow, nFocusedRowIndex, FALSE, FALSE);
				}
			}

			// Ensure visible
			if (pBehavior->bEnsureVisible)
			{
				EnsureVisible(clickArgs.pRow);
			}

			// First update selected row and after that perform click action.
			EndUpdate();

			// Row click
			if (pBehavior->bClick)
			{
				clickArgs.pRow->OnClick(point);

				if (pBehavior->bCheckSelectedRows && clickArgs.pItem)
				{
					SetSelectionState(clickArgs.pItem->GetIndex(),
									  clickArgs.pItem->GetCheckedState());
				}
			}

		} // !bHandled
	}
	else if (m_bFastDeselectMode)
	{
		if (!inputState.bKeyControl && !inputState.bKeyShift)
		{
			m_pSelectedRows->Clear();
		}
	}

	// Context menu
	if (pBehavior->bContextMenu)
	{
		if (NULL != m_pSections->HitTest(point))
		{
			CXTPGridColumn* pColumn   = NULL;
			CXTPGridRecordItem* pItem = NULL;

			if (NULL != clickArgs.pRow)
			{
				// clickArgs.pRow->HitTest(point, NULL, &pColumn);
				clickArgs.pRow->OnContextMenu(point);
			}
			else
			{
				ClientToScreen(&point);
				SendMessageToParent(clickArgs.pRow, pItem, pColumn, NM_RCLICK, &point);
			}
		}
	}

	// Restore focus
	if (pModifier->bFocusRowTemporarily)
	{
		CXTPGridRow* pRow = NULL;

		pRow = GetRows()->GetAt(nFocusedRowIndex);
		BeginUpdate();
		_SetFocusedRow(pRow);
		EndUpdate();
	}

	if (m_pSelectedRows->IsChanged())
	{
		// TRACE(_T("OnSelectionChanged\n"));
		OnSelectionChanged();
	}
}

void CXTPGridControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	m_dwLastMouseMessage = WM_LBUTTONDOWN;
	OnButton(nFlags, point, m_pBehavior->Row->MouseDown->Left);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPGridControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TRACE(_T("CXTPGridControl::OnLButtonUp\n"));
	CWnd::OnLButtonUp(nFlags, point);

	/*
		The Windows window manager sends an unwanted WM_LBUTTONDOWN message if
		the window is maximized with a double-click on the caption bar.

		Window messages for double-click:
		- WM_LBUTTONDOWN
		- WM_LBUTTONUP
		- WM_LBUTTONDBLCLK
		- WM_LBUTTONUP
	*/
	if (WM_LBUTTONDOWN == m_dwLastMouseMessage || WM_LBUTTONDBLCLK == m_dwLastMouseMessage
		|| xtpGridMouseNothing != GetMouseMode())
	{
		m_dwLastMouseMessage = WM_LBUTTONUP;
		OnButton(nFlags, point, m_pBehavior->Row->MouseUp->Left);

#ifdef _XTP_ACTIVEX
		((COleControl*)GetParent())->FireMouseUp(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif
	}
}

void CXTPGridControl::OnLButtonDblClk(UINT nFlags, CPoint ptDblClick)
{
	CWnd::OnLButtonDblClk(nFlags, ptDblClick);
	m_dwLastMouseMessage = WM_LBUTTONDBLCLK;

	EditItem(NULL);

	// Information for delay editing
	EnsureStopDelayEditTimer();
	SetLastRqstEdit(-1, -1);

	// columns processing
	CXTPGridHeader* pHeader = GetGridHeader();
	if (NULL != pHeader)
	{
		pHeader->OnLButtonDblClk(ptDblClick);
	}

	// rows processing
	CXTPGridRow* pRow = HitTest(ptDblClick);
	if (NULL != pRow)
	{
		pRow->OnDblClick(ptDblClick);
	}
	else // just notify parent
	{
		SendMessageToParent(NULL, NULL, NULL, NM_DBLCLK, &ptDblClick);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Right button
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

	OnButton(nFlags, point, m_pBehavior->Row->MouseDown->Right);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPGridControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

	if (!IsWindow(m_hWnd)) // Can be destroyed in WM_CONTEXTMENU
		return;

	OnButton(nFlags, point, m_pBehavior->Row->MouseUp->Right);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

LRESULT CXTPGridControl::OnNcHitTest(CPoint point)
{
	LRESULT ht = CWnd::OnNcHitTest(point);

	if (ht != HTCLIENT)
		return ht;

	DWORD dwStyle = GetStyle();

	if ((dwStyle & (WS_VSCROLL | WS_HSCROLL)) == 0)
		return ht;

	CXTPWindowRect rcWindow(this);

	if (dwStyle & WS_VSCROLL)
	{
		if (IsLayoutRTL() || !!(GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LEFTSCROLLBAR))
		{
			CRect rc(rcWindow.left, rcWindow.top, GetSystemMetrics(SM_CXVSCROLL),
					 rcWindow.bottom - (dwStyle & WS_HSCROLL ? GetSystemMetrics(SM_CYHSCROLL) : 0));

			if (rc.PtInRect(point))
				return HTVSCROLL;
		}
		else
		{
			CRect rc(rcWindow.right - GetSystemMetrics(SM_CXVSCROLL), rcWindow.top, rcWindow.right,
					 rcWindow.bottom - (dwStyle & WS_HSCROLL ? GetSystemMetrics(SM_CYHSCROLL) : 0));

			if (rc.PtInRect(point))
				return HTVSCROLL;
		}
	}

	if (dwStyle & WS_HSCROLL)
	{
		CRect rc(rcWindow.left, rcWindow.bottom - GetSystemMetrics(SM_CYHSCROLL),
				 rcWindow.right - (dwStyle & WS_VSCROLL ? GetSystemMetrics(SM_CXVSCROLL) : 0),
				 rcWindow.bottom);

		if (rc.PtInRect(point))
			return HTHSCROLL;
	}

	return ht;
}

void CXTPGridControl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	UNREFERENCED_PARAMETER(pWnd);

	if (GetMouseMode() != xtpGridMouseNothing)
		return;

	CPoint ptClient = pos;
	ScreenToClient(&ptClient);

	// call context menu handler for grid header if clicked inside
	if (m_rcHeaderArea.PtInRect(ptClient) || m_rcGroupByArea.PtInRect(ptClient))
	{
		CXTPGridHeader* pHeader = GetGridHeader();
		if (pHeader)
			pHeader->OnContextMenu(ptClient);
		return;
	}

	// Context menu key
	if (pos == CPoint(-1, -1))
	{
		CXTPGridRow* pFocusedRow = GetFocusedRow();
		if (pFocusedRow)
		{
			// Menu position below row
			ptClient = CPoint(pFocusedRow->GetRect().left, pFocusedRow->GetRect().bottom);

			pFocusedRow->OnContextMenu(ptClient);
		}
		else
		{
			pos = m_pSectionBody->m_rcSection.TopLeft();
			ClientToScreen(&pos);
			SendMessageToParent(NULL, NULL, NULL, NM_RCLICK, &pos);
		}
	}
}

CXTPGridColumn* CXTPGridControl::GetNextFocusableColumn(CXTPGridRow* pRow, int nColumnIndex,
														int nDirection)
{
	if (!pRow->GetRecord())
		return NULL;

	for (;;)
	{
		CXTPGridColumn* pColumn = GetGridHeader()->GetNextVisibleColumn(nColumnIndex, nDirection);
		if (!pColumn)
			return NULL;

		CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(pColumn);

		if (pItem && pItem->IsFocusable())
			return pColumn;

		nColumnIndex = pColumn->GetIndex();
	}
}

BOOL CXTPGridControl::OnPreviewKeyDown(UINT& rnChar, UINT nRepCnt, UINT nFlags)
{
	XTP_NM_GRIDPREVIEWKEYDOWN nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	nmParams.nChar   = rnChar;
	nmParams.nRepCnt = nRepCnt;
	nmParams.nFlags  = nFlags;
	nmParams.bCancel = FALSE;

	SendNotifyMessage(XTP_NM_GRID_PREVIEWKEYDOWN, (NMHDR*)&nmParams);
	rnChar = nmParams.nChar;

	return !nmParams.bCancel;
}

void CXTPGridControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	BOOL bControlKeyOnly = (::GetKeyState(VK_CONTROL) < 0);
	BOOL bControlKey	 = (::GetKeyState(VK_CONTROL) < 0) || m_bMultiSelectionMode;
	BOOL bShiftKey		 = (::GetKeyState(VK_SHIFT) < 0);

	CXTPGridRow* pFocusedRow = GetFocusedRow();

	CXTPDrawHelpers::KeyToLayout(this, nChar);
	int nRows	 = GetRows()->GetCount();
	int nHeadRows = GetHeaderRows()->GetCount();
	int nFootRows = GetFooterRows()->GetCount();

	// Increase reference Count for pFocusedRow to be sure it won't be destroyed inside function.
	CXTPSmartPtrInternalT<CCmdTarget> ptrRowLock(pFocusedRow, TRUE);

	if (nRows > 0 || nHeadRows > 0 || nFootRows > 0)
		switch (nChar)
		{
			case VK_F5: Recalc(bShiftKey); break;

			case VK_ADD:
			{
				if (bControlKeyOnly)
				{
					if (!IsLockExpand())
					{
						ExpandAll();
					}
				}
				else if (pFocusedRow && pFocusedRow->HasChildren() && !pFocusedRow->IsExpanded()
						 && !pFocusedRow->IsLockExpand())
				{
					pFocusedRow->SetExpanded(TRUE);
				}
			}
			break;

			case VK_SUBTRACT:
			{
				if (bControlKeyOnly)
				{
					if (!IsLockExpand())
					{
						CollapseAll();
					}
				}
				else if (pFocusedRow && pFocusedRow->HasChildren() && pFocusedRow->IsExpanded()
						 && !pFocusedRow->IsLockExpand())
				{
					pFocusedRow->SetExpanded(FALSE);
				}
			}
			break;

			case VK_LEFT:

				if (m_bIconView)
				{
					GetNavigator()->MoveLeft(bShiftKey, bControlKey);
				}
				else
				{
					/**
						[0] if FocusSubItems we need check first if we can move one column left. if
					   not - "fall down" for cases [1][2][3] [1] If a row without children has the
					   focus, focus shall be put on the parent row [2] If a row with expanded
					   children has the focus, children shall be collapsed [3] If a row with
					   collapsed children has the focus, focus shall be put on the parent row
					*/

					if (pFocusedRow)
					{
						if (m_bFocusSubItems && m_pFocusedColumn)
						{
							CXTPGridColumn* pColumn = GetNextFocusableColumn(
								pFocusedRow, m_pFocusedColumn->GetIndex(), -1);
							if (pColumn)
							{
								SetFocusedColumn(pColumn);
								SetFocusedRow(GetFocusedRow(), bControlKey);
								break;
							}
						}

						/* [1], Row without children */
						if (!pFocusedRow->HasChildren())
						{
							CXTPGridRow* pParent = pFocusedRow->GetParentRow();
							if (NULL != pParent)
							{
								SetFocusedRow(pParent, bControlKey);
							}
							else
							{
								GetNavigator()->MoveUp(bShiftKey, bControlKey);
							}
						}

						/* [2][3] Row with children */
						else // if (pFocusedRow->HasChildren())
						{
							/* [2] Row with expanded children */
							if (pFocusedRow->IsExpanded())
							{
								if (!pFocusedRow->IsLockExpand())
								{
									pFocusedRow->SetExpanded(FALSE);
								}
								else
								{
									GetNavigator()->MoveUp(bShiftKey, bControlKey);
								}
							}
							/* [3] Row with collapsed children */
							else
							{
								CXTPGridRow* pParent = pFocusedRow->GetParentRow();

								// if we don't have parent just go up, like Outlook.
								if (NULL != pParent)
								{
									SetFocusedRow(pParent, bControlKey);
								}
								else
								{
									GetNavigator()->MoveUp(bShiftKey, bControlKey);
								}
							}
						}
					}
				}
				break;

			case VK_RIGHT:
				if (m_bIconView)
				{
					GetNavigator()->MoveRight(bShiftKey, bControlKey);
					break;
				}
				else
				{
					/**
						[0] if FocusSubItems we need check first if we can move one column right. if
					   not - "fall down" for cases [1][2][3] [1] If a row with expanded children has
					   the focus, focus shall be put on first children [2] If a row with collapsed
					   children has the focus, children shall be expanded [3] If a row with no
					   children has the focus, we go down like Outlook
					*/

					if (pFocusedRow)
					{
						if (m_bFocusSubItems && m_pFocusedColumn)
						{
							CXTPGridColumn* pColumn = GetNextFocusableColumn(
								pFocusedRow, m_pFocusedColumn->GetIndex(), +1);
							if (pColumn)
							{
								SetFocusedColumn(pColumn);
								SetFocusedRow(GetFocusedRow(), bControlKey);
								break;
							}
						}

						if (pFocusedRow->HasChildren())
						{
							/* [1] Row with expanded children */
							if (pFocusedRow->IsExpanded())
							{
								CXTPGridRows* pChildRows = pFocusedRow->GetChilds();
								CXTPGridRow* pChildRow   = pChildRows->GetAt(0);
								SetFocusedRow(pChildRow, bControlKey);
							}
							/* [2] Row with collapsed children */
							else
							{
								if (!pFocusedRow->IsLockExpand())
								{
									pFocusedRow->SetExpanded(TRUE, bControlKeyOnly);
								}
								else
								{
									GetNavigator()->MoveDown(bShiftKey, bControlKey);
								}
							}
						}
						else
						{
							GetNavigator()->MoveDown(bShiftKey, bControlKey);
						}
					}
				}
				break;

			case VK_DOWN: GetNavigator()->MoveDown(bShiftKey, bControlKey); break;

			case VK_UP: GetNavigator()->MoveUp(bShiftKey, bControlKey); break;

			case VK_HOME: GetNavigator()->MoveFirstRow(bShiftKey, FALSE); break;

			case VK_END: GetNavigator()->MoveLastRow(bShiftKey, FALSE); break;

			case VK_NEXT: GetNavigator()->MovePageDown(bShiftKey, FALSE); break;

			case VK_PRIOR: GetNavigator()->MovePageUp(bShiftKey, FALSE); break;

			case VK_RETURN:
				if (pFocusedRow && pFocusedRow->HasChildren() && !pFocusedRow->IsLockExpand())
				{
					pFocusedRow->SetExpanded(!pFocusedRow->IsExpanded());
				}
				break;

			case VK_ESCAPE:
				if (GetMouseMode() != xtpGridMouseNothing)
					GetGridHeader()->CancelMouseMode();
				break;

			case VK_F2: GetNavigator()->BeginEdit(); break;

			case VK_SPACE:
				if (IsSelectionEnabled() && bControlKey && pFocusedRow)
				{
					if (pFocusedRow->IsSelected())
						m_pSelectedRows->Remove(pFocusedRow);
					else
						m_pSelectedRows->Add(pFocusedRow);

					OnSelectionChanged(); // set Selection flag m_bChanged = FALSE;
					RedrawControl();
				}
				break;

			case 'A':
				if (IsSelectionEnabled() && !bShiftKey && bControlKeyOnly && IsMultipleSelection())
				{
					if (IsVirtualMode())
					{
						GetNavigator()->MoveToRow(0);
						GetNavigator()->MoveToRow(GetRecords()->GetCount() - 1, TRUE);
					}
					else
					{
						if (m_pSelectedRows->m_nRowType == xtpRowTypeBody
							&& GetRecords()->GetCount()
								   == m_pSelectedRows->GetCount()) // all rows were selected before
						{
							break;
						}

						_SelectRows(GetRecords());
					}
					RedrawControl();
				}
				break;
		}

	NMKEY nmgv;
	nmgv.nVKey  = nChar;
	nmgv.uFlags = nFlags;
	SendNotifyMessage(NM_KEYDOWN, (NMHDR*)&nmgv);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, TRUE, nChar)
#endif
}

// ICON_VIEW_MODE RELATED <<
void CXTPGridControl::SetNavigator(CXTPGridNavigator* pNavigator)
{
	if (pNavigator)
	{
		CMDTARGET_RELEASE(m_pNavigator);
		m_pNavigator = pNavigator;
	}
}

void CXTPGridControl::SetIconViewToDefaults()
{
	m_bIconView			   = FALSE;
	m_iIconWidth		   = 0;
	m_iIconHeight		   = 0;
	m_iIconWidthSpacing	= 0;
	m_iIconHeightSpacing   = 0;
	m_iIconViewColumn	  = -1;
	m_iIconViewRowsPerLine = 0;
	m_bUseIconColumnForNum = FALSE;
	m_iColumnForNum		   = -1;
	m_bPrevFocusSubItems   = FALSE;
	m_bPrevHeaderAutoSize  = FALSE;
	m_iIconPropNum		   = 0;
	m_iIconNum			   = 0;
	m_pPrevVisible		   = NULL;
	m_pPrevGroupsOrder	 = NULL;
	m_nPrevTreeIndent	  = 0;

	m_bIconColumnIndexNotValid = FALSE;

	m_bPrevHeaderRows = FALSE;
	m_bPrevFooterRows = FALSE;
	m_bPrevHeaderShow = FALSE;
	m_bPrevFooterShow = FALSE;

	m_bRClickDrag = FALSE;
}

void CXTPGridControl::SetIconColumn(CXTPGridColumn* pColumn)
{
	ASSERT(pColumn);
	m_iIconViewColumn = pColumn ? pColumn->GetItemIndex() : -1;
}

void CXTPGridControl::CreateIconColumn(BOOL bUseColumnForNum, int nWidth)
{
	if (GetColumns())
	{
		if (!m_bUseIconColumnForNum)
			m_bUseIconColumnForNum = bUseColumnForNum;

		if (m_iIconViewColumn == -1)
		{
			int iCnt = GetColumns()->GetCount();
			if (GetRecords())
			{
				int iCnt0 = iCnt;
				if (GetRecords()->GetAt(0))
					iCnt = max(iCnt, GetRecords()->GetAt(0)->GetItemCount());
				if (iCnt > iCnt0)
					for (int iIc = 0; iIc < iCnt - iCnt0; iIc++)
						AddColumn(new CXTPGridColumn(iIc, _T(""), 0, FALSE, XTP_GRID_NOICON, FALSE,
													 FALSE));
			}

			CXTPGridColumn* pIconCol = AddColumn(new CXTPGridColumn(iCnt, _T(""), 0));
			pIconCol->SetVisible(FALSE);

			if (m_bUseIconColumnForNum)
			{
				pIconCol->SetCaption(_T("#"));
				pIconCol->SetVisible(TRUE);
				pIconCol->SetHeaderAlignment(DT_CENTER);
				pIconCol->SetWidth(nWidth);

				if (GetColumns()->GetGroupsOrder()->GetCount() > 0 && nWidth < XTP_DPI_X(30))
					nWidth = XTP_DPI_X(30);

				GetPaintManager()->m_RecordNumberWidth = nWidth;

				pIconCol->AllowRemove(FALSE);
				pIconCol->EnableResize(FALSE);
				// pIconCol->SetAutoSize(FALSE);
				pIconCol->SetAllowDrag(FALSE);
				pIconCol->SetEditable(FALSE);
				pIconCol->SetGroupable(FALSE);
				pIconCol->SetSortable(FALSE);
			}
			m_iIconViewColumn = pIconCol->GetIndex();
			m_iColumnForNum   = m_iIconViewColumn;
		}
	}
}

void CXTPGridControl::AssignIconViewPropNumAndIconNum(int nCol, int nIcon, BOOL bUseColumnForNum,
													  int nWidth)
{
	BOOL bAlreadyUsed = m_bUseIconColumnForNum;

	m_bIconColumnIndexNotValid = TRUE;

	CreateIconColumn(bUseColumnForNum, nWidth);

	m_bIconColumnIndexNotValid = FALSE;

	if (!bAlreadyUsed && bUseColumnForNum)
	{
		m_bUseIconColumnForNum = bUseColumnForNum;

		if (m_iIconViewColumn > -1)
		{
			CXTPGridColumn* pIconCol = GetColumns()->GetAt(m_iIconViewColumn);
			if (m_bUseIconColumnForNum)
			{
				pIconCol->SetCaption(_T("#"));
				pIconCol->SetVisible(TRUE);
				pIconCol->SetHeaderAlignment(DT_CENTER);
				pIconCol->SetWidth(nWidth);

				if (GetColumns()->GetGroupsOrder()->GetCount() > 0 && nWidth < XTP_DPI_X(30))
					nWidth = XTP_DPI_X(30);

				GetPaintManager()->m_RecordNumberWidth = nWidth;

				pIconCol->AllowRemove(FALSE);
				pIconCol->EnableResize(FALSE);
				pIconCol->SetEditable(FALSE);
				pIconCol->SetGroupable(FALSE);
				pIconCol->SetSortable(FALSE);
			}
		}
	}
	if (nCol > -1)
		m_iIconPropNum = nCol;
	if (nIcon > -1)
		m_iIconNum = nIcon;

	if (!bAlreadyUsed && m_iColumnForNum > -1 && bUseColumnForNum)
		GetColumns()->ChangeColumnOrder(0, m_iColumnForNum);
}
// ICON_VIEW_MODE RELATED >>

void CXTPGridControl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, TRUE, nChar)
#endif
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPGridControl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_bWasShiftKey = (GetKeyState(VK_SHIFT) < 0);
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, FALSE, nChar)
#endif
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPGridControl::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, FALSE, nChar)
#endif
	CWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPGridControl::OnChar(UINT nChar, UINT nRepCntr, UINT nFlags)
{
	NMCHAR nmgv;
	ZeroMemory(&nmgv, sizeof(NMCHAR));
	nmgv.ch = nChar;
	SendNotifyMessage(NM_CHAR, (NMHDR*)&nmgv);

	CXTPGridRow* pFocusedRow = GetFocusedRow();

	if (pFocusedRow && (nChar == VK_TAB))
	{
		if (m_bFocusSubItems)
		{
			CXTPSmartPtrInternalT<CCmdTarget> ptrRowLock(pFocusedRow, TRUE);
			EditItem(NULL);

			BOOL bBack = (GetKeyState(VK_SHIFT) < 0);
			GetNavigator()->MoveLeftRight(bBack);

			if (IsAllowEdit() && m_bEditOnClick && !m_bTrapTabKey)
			{
				GetNavigator()->BeginEdit();
			}
		}
		return;
	}

	if (m_pFocusedColumn && pFocusedRow && pFocusedRow->GetRecord() && (nChar != VK_RETURN)
		&& (nChar != VK_ESCAPE))
	{
		XTP_GRIDRECORDITEM_ARGS itemArgs(this, pFocusedRow, m_pFocusedColumn);

		if (itemArgs.pItem && itemArgs.pItem->OnChar(&itemArgs, nChar))
			return;
	}

	CWnd::OnChar(nChar, nRepCntr, nFlags);
}

void CXTPGridControl::OnCaptureChanged(CWnd* pWnd)
{
	if (CWnd::GetCapture() != this && GetMouseMode() != xtpGridMouseNothing)
		GetGridHeader()->CancelMouseMode();

	CWnd::OnCaptureChanged(pWnd);
}

void CXTPGridControl::OnEnable(BOOL bEnable)
{
	UNREFERENCED_PARAMETER(bEnable);

	AdjustScrollBars();
}

UINT CXTPGridControl::GetMouseScrollLines()
{
	UINT nScrollLines = 3; // default value

	if (XTPSystemVersion()->IsWin95())
	{
		HKEY hKey;
		if (ERROR_SUCCESS
			== ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE,
							  &hKey))
		{
			TCHAR szData[128];
			DWORD dwKeyDataType;
			DWORD dwDataBufSize = sizeof(szData);

			if (ERROR_SUCCESS
				== ::RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
									 (LPBYTE)&szData, &dwDataBufSize))
			{
				nScrollLines = _tcstoul(szData, NULL, 10);
			}
			::RegCloseKey(hKey);
		}
	}
	// win98 or greater
	else
	{
		::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);
	}

	return nScrollLines;
}

BOOL CXTPGridControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (!m_bVScrollBarVisible)
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);

	UINT uiMsg;
	int nScrollsCount = 0;
	// calculate what should be sent
	if (m_nRowsPerWheel == -1)
	{
		// A m_nRowsPerWheel value less than 0 indicates that the mouse wheel scrolls whole pages,
		// not just lines.
		int nPagesScrolled = zDelta / 120;
		uiMsg			   = UINT(nPagesScrolled > 0 ? SB_PAGEUP : SB_PAGEDOWN);
		nScrollsCount	  = nPagesScrolled > 0 ? nPagesScrolled : -nPagesScrolled;
	}
	else
	{
		int nRowsScrolled = m_nRowsPerWheel * zDelta / 120;
		uiMsg			  = UINT(nRowsScrolled > 0 ? SB_LINEUP : SB_LINEDOWN);
		nScrollsCount	 = nRowsScrolled > 0 ? nRowsScrolled : -nRowsScrolled;
	}

	BeginUpdate();

	// send scroll messages
	for (int i = 0; i < nScrollsCount; i++)
	{
		OnVScroll(uiMsg, 0, NULL);
	}

	EndUpdate();

	UpdateWindow();

	return TRUE;
}

void CXTPGridControl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPGridControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	CXTPGridHeader* pHeader   = GetGridHeader();
	CXTPGridColumn* pColumn   = NULL;
	CXTPGridRow* pRow		  = NULL;
	CXTPGridRecordItem* pItem = NULL;

	if (NULL != pHeader)
	{
		pHeader->OnMouseMove(nFlags, point);
	}

	CXTPGridRow* pHotExpandButtonRow = NULL;

	CXTPGridHyperlink* pHotHyperlink = NULL;

	if (GetMouseMode() == xtpGridMouseNothing)
	{
		pRow = HitTest(point);

		if (NULL != pRow)
		{
			pRow->OnMouseMove(nFlags, point);

			// Hyperlink hit test
			pItem = pRow->HitTest(point);

			if (NULL != pItem)
			{
				pColumn = pItem->GetColumn();

				int nHyperlink = pItem->HitTestHyperlink(point);

				if (nHyperlink >= 0)
				{
					pHotHyperlink = pItem->GetHyperlinks()->GetAt(nHyperlink);
				}
			}

			if (m_bShowTooltips && nFlags == 0)
			{
				pRow->ShowToolTip(point, m_pTip);
			}
		}
		else
		{
			m_pTip->m_pItem		= NULL;
			m_pTip->m_nRowIndex = -1;
		}

		if (pRow && pRow->IsGroupRow() && m_pPaintManager->m_columnStyle == xtpGridColumnResource)
		{
			if (pRow->m_rcCollapse.PtInRect(point))
			{
				pHotExpandButtonRow = pRow;
			}
		}

		if (m_pHotRow != pRow)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, GetSafeHwnd(), 0 };
			_TrackMouseEvent(&tme);

			m_pHotRow = pRow;
		}

		// If mouse moved some since down...
		if (m_bPrepareDrag
			&& (labs(point.x - m_ptDrag.x) > XTP_DPI_X(GetDragSensitivityX())
				|| labs(point.y - m_ptDrag.y) > XTP_DPI_Y(GetDragSensitivityY())))
		{
			// Prevent duplicate
			m_bPrepareDrag = FALSE;

			// Begin a drag operation
			OnBeginDrag(m_ptDrag, nFlags);
		}
	}

	if (m_pHotHyperlink != pHotHyperlink)
	{
		m_pHotHyperlink = pHotHyperlink;
		RedrawControl();
	}

	if (m_pHotExpandButtonRow != pHotExpandButtonRow)
	{
		m_pHotExpandButtonRow = pHotExpandButtonRow;
		RedrawControl();
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseMove(AxButtonState(), AxShiftState(), point.x, point.y);
#else
	SendMessageToParent(pRow, pItem, pColumn, XTP_NM_GRID_MOUSEMOVE, &point);
#endif
}

class CXTPGridDropSource : public COleDropSource
{
public:
	CXTPGridDropSource(CXTPGridControl* pControl)
		: m_pControl(pControl)
	{
	}

	virtual SCODE GiveFeedback(DROPEFFECT dropEffect)
	{
		SCODE sCode = NOERROR;

		XTP_NM_GRIDGIVEFEEDBACK giveFeedback;
		giveFeedback.dropEffect   = dropEffect;
		giveFeedback.bDragStarted = m_bDragStarted;

		LRESULT lResult = m_pControl->SendNotifyMessage(XTP_NM_GRID_GIVEFEEDBACK, &giveFeedback);

		if (0 != lResult)
		{
			sCode = S_OK;
		}
		else
		{
			sCode = COleDropSource::GiveFeedback(dropEffect);
		}

		return sCode;
	}

protected:
	CXTPGridControl* m_pControl;
};

void CXTPGridControl::OnBeginDrag(CPoint point)
{
	OnBeginDrag(point, 0);
}

void CXTPGridControl::OnBeginDrag(CPoint point, UINT nFlags)
{
	if (SendMessageToParent(NULL, NULL, NULL, LVN_BEGINDRAG, &point))
		return;
	// R-DD <<
	if ((nFlags & MK_RBUTTON) && SendMessageToParent(NULL, NULL, NULL, LVN_BEGINRDRAG, &point))
		return;
	// R-DD >>
	if (m_cfGrid == NULL)
		return;

	CXTPGridSelectedRows* pSelectedRows = GetSelectedRows();
	if (!pSelectedRows)
		return;

	if ((m_dwDragDropFlags & xtpGridAllowDrag) == 0)
		return;

	if (pSelectedRows->m_nRowType == xtpRowTypeHeader
		|| pSelectedRows->m_nRowType == xtpRowTypeFooter)
		return;

	int nCount = pSelectedRows->GetCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		CXTPGridRow* pRow = pSelectedRows->GetAt(i);
		if (pRow && pRow->IsGroupRow())
		{
			pRow->SetExpanded(TRUE);
			pRow->SelectChilds();
		}
	}

	int nRowsCount = pSelectedRows->GetCount();
	if (nRowsCount < 1)
		return;

	// minimize memory reallocs to improve performance
	UINT nAveRecordsSize = 1500; // bytes
	UINT nGrowBytes		 = ((nRowsCount * nAveRecordsSize) / 4096 + 1) * 4096;

	UINT nAllocFlags = GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT;

	CSharedFile fileRecords(nAllocFlags, nGrowBytes);

	BOOL bSucceed = FALSE;

	//------------------------------------------------------------------------
	const int cErrTextSize = 1024;
	TCHAR szErrText[cErrTextSize + 1];

	CXTPGridRecords* pDragRecords = new CXTPGridRecords(TRUE);

	if (!_GetSelectedRows(pDragRecords))
	{
		CMDTARGET_RELEASE(pDragRecords);
		return;
	}

	pDragRecords->CleanOfRedundant();

	try
	{
		CArchive ar(&fileRecords, CArchive::store);
		CXTPPropExchangeArchive px(ar);

		bSucceed = _WriteRecordsData(&px, pDragRecords);

		ar.Close(); // perform Flush() and detach from file
	}
	catch (CArchiveException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPGridControl::OnBeginDrag() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch (CFileException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPGridControl::OnBeginDrag() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch (...)
	{
		TRACE(_T("EXCEPTION: CXTPGridControl::OnBeginDrag() - Unhandled Exception!\n"));
	}

	if (!bSucceed)
	{
		CMDTARGET_RELEASE(pDragRecords);
		return;
	}

	HGLOBAL hGlobal = fileRecords.Detach();

	m_bDragMode		= TRUE;
	m_bInternalDrag = TRUE;

	DROPEFFECT dropEffectMask =
		static_cast<DROPEFFECT>((m_dwDragDropFlags & xtpGridAllowDragCopy) ? DROPEFFECT_COPY : 0)
		+ ((m_dwDragDropFlags & xtpGridAllowDragMove) ? DROPEFFECT_MOVE : 0);

	COleDataSource* pds = new COleDataSource();
	XTP_NM_GRIDDRAGDROP nmData;
	ZeroMemory(&nmData, sizeof(nmData));
	nmData.pRecords	= pDragRecords;
	nmData.pDataSource = pds; // Data Source - issue 22675

	if (SendNotifyMessage(XTP_NM_GRID_BEGINDRAG, (NMHDR*)&nmData) == -1)
	{
		CMDTARGET_RELEASE(pDragRecords);
		CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag)
		m_bDragMode		= FALSE;
		m_bInternalDrag = FALSE;

		pds->InternalRelease();
		return;
	}

	pds->CacheGlobalData(m_cfGrid, hGlobal);

	if (!(m_dwDragDropFlags & xtpGridDontDropAsText))
	{
		// Get As Text
		CString strDropText = _GetSelectedRowsVisibleColsText();
		HGLOBAL hDropText   = XTPAllocStrInGlobalMem(strDropText);
		if (hDropText)
			pds->CacheGlobalData(XTP_CF_TEXT_T, hDropText);
	}

	CXTPGridDropSource dropSource(this);

	DROPEFFECT dropEffect = pds->DoDragDrop(dropEffectMask, NULL, &dropSource);
	m_bDragMode			  = FALSE;
	m_bInternalDrag		  = FALSE;

	if ((m_dwDropMarkerFlags & xtpGridDropSelect) && m_pSelectedRowsBeforeDrag)
	{
		// Must remember the selected items before drag.
		m_pSelectedRows->Clear();
		for (int i = 0; i < m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks.GetSize(); i++)
			m_pSelectedRows->m_arrSelectedBlocks.Add(
				m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks[i]);

		CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag);
	}

	nmData.dropEffect = dropEffect;
	SendNotifyMessage(XTP_NM_GRID_DRAGDROP_COMPLETED, (NMHDR*)&nmData);

	if ((dropEffect == DROPEFFECT_MOVE) && (dropEffectMask & DROPEFFECT_MOVE))
	{
		Cut();
	}

	CMDTARGET_RELEASE(pDragRecords);

	EnsureStopAutoVertScroll();

	pds->InternalRelease();
}

DROPEFFECT CXTPGridControl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point,
									   int nState)
{
	DROPEFFECT dropEffect		  = DROPEFFECT_MOVE;
	BOOL bAbove					  = TRUE;
	CXTPGridRecord* pTargetRecord = NULL;
	int nTargetRow				  = -1;

	if (nState == 0)
	{
		// Must get the selected rows before the selection changes.
		if (m_dwDropMarkerFlags & xtpGridDropSelect)
		{
			// Must remember the selected items before drag.
			m_pSelectedRowsBeforeDrag = new CXTPGridSelectedRows(this);
			for (int i = 0; i < m_pSelectedRows->m_arrSelectedBlocks.GetSize(); i++)
			{
				m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks.Add(
					m_pSelectedRows->m_arrSelectedBlocks[i]);
			}
		}
	}

	if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
	{
		dropEffect = DROPEFFECT_COPY;
	}

	if (!m_nOLEDropMode)
	{
		if (m_cfGrid == NULL)
			dropEffect = DROPEFFECT_NONE;

		if ((m_dwDragDropFlags & xtpGridAllowDrop) == 0)
			dropEffect = DROPEFFECT_NONE;

		XTP_NM_GRIDDRAGDROP nmData;
		nmData.dropEffect  = dropEffect;
		nmData.pDataObject = pDataObject;
		nmData.pt		   = point;
		// R-DD <<
		nmData.dwKeyState = dwKeyState;
		// R-DD >>

		int iValid = (int)SendNotifyMessage(XTP_NM_GRID_HASVALIDDROPTYPE, (NMHDR*)&nmData);

		m_nOLEDropAbove = nmData.bAbove;

		if ((dropEffect != DROPEFFECT_NONE)
			&& (!pDataObject || (!pDataObject->IsDataAvailable(m_cfGrid) && iValid != 1)))
			dropEffect = DROPEFFECT_NONE;
	}

	int nDropPos = m_nDropPos;
	m_nDropPos   = -1;

	if (dropEffect != DROPEFFECT_NONE)
	{
		EnsureStartAutoVertScroll();
		//      DoAutoVertScrollIfNeed(m_ptDrag, point);

		m_bNoNeedSortedDragDrop = (GetColumns()->GetSortOrder()->GetCount() == 0
								   && GetColumns()->GetGroupsOrder()->GetCount() == 0);

		if (m_bNoNeedSortedDragDrop || m_bSortedDragDrop)
		{
			CXTPGridRow* pRow = HitTest(point);

			if (pRow && pRow->GetRecord())
			{
				if (pRow->GetRect().CenterPoint().y < point.y)
					bAbove = FALSE;
				if (bAbove)
					m_nDropPos = pRow->GetRect().top == 0 ? 0 : pRow->GetRect().top - XTP_DPI_Y(1);
				// m_nDropPos = pRow->GetRect().top - 1;
				else
					m_nDropPos = pRow->GetRect().bottom - XTP_DPI_Y(1);
				pTargetRecord = pRow->GetRecord();
				nTargetRow	= pRow->GetIndex();

				if (((m_dwDropMarkerFlags & xtpGridDropSelect)
					 && (m_dwDropMarkerFlags & xtpGridDropBetween)
					 && point.y > (pRow->GetRect().top + XTP_DPI_Y(3))
					 && point.y < (pRow->GetRect().bottom - XTP_DPI_Y(3)))
					|| ((m_dwDropMarkerFlags & xtpGridDropSelect) && point.y > pRow->GetRect().top
						&& point.y < pRow->GetRect().bottom))
				{
					XTP_NM_GRIDDRAGDROP nmData;
					nmData.dropEffect	= dropEffect;
					nmData.pDataObject   = pDataObject;
					nmData.pTargetRecord = pTargetRecord;
					nmData.nTargetRow	= nTargetRow;
					nmData.pt			 = point;
					// R-DD <<
					nmData.dwKeyState = dwKeyState;
					// R-DD >>

					int iValid = (int)SendNotifyMessage(XTP_NM_GRID_VALIDDROPTARGET,
														(NMHDR*)&nmData);
					if (iValid == 1)
						m_pSelectedRows->Select(pRow);
					else
						m_pSelectedRows->Clear();

					m_nOLEDropAbove = nmData.bAbove;
				}
				else if (m_dwDropMarkerFlags & xtpGridDropSelect)
				{
					m_pSelectedRows->Clear();
				}
			}
			else
			{
				if ((m_nDropPos > m_pSectionBody->m_rcSection.top - XTP_DPI_Y(1)
					 || m_nDropPos < m_pSectionBody->m_rcSection.bottom - XTP_DPI_Y(1)))
					m_nDropPos = point.y;

				if (GetRows()->GetCount() == 0)
					m_nDropPos = 0;

				if (m_dwDropMarkerFlags & xtpGridDropSelect)
					m_pSelectedRows->Clear();
			}

			if (m_nDropPos == 0 && GetRows()->GetCount() == 0)
				m_nDropPos = m_pSectionBody->m_rcSection.top;
			else if ((m_nDropPos < m_pSectionBody->m_rcSection.top - XTP_DPI_Y(1)
					  || m_nDropPos > m_pSectionBody->m_rcSection.bottom - XTP_DPI_Y(1)))
				dropEffect = DROPEFFECT_NONE;
		}
	}

#ifdef _XTP_ACTIVEX
	if (m_nOLEDropMode)
	{
		CXTPOleDataObject::FireOleDragOverEvent((COleControl*)GetParent(), pDataObject, dropEffect,
												point);

		if (dropEffect == DROPEFFECT_NONE)
			m_nDropPos = -1;
	}
#endif

	if (nState == 0)
	{
		// entering, get grid records
		CMDTARGET_RELEASE(m_pDropRecords);

		CFile* pFile = m_cfGrid ? pDataObject->GetFileData(m_cfGrid) : NULL;
		if (pFile)
		{
			m_pDropRecords = new CXTPGridRecords();

			const int cErrTextSize = 1024;
			TCHAR szErrText[cErrTextSize + 1];

			try
			{
				CArchive ar(pFile, CArchive::load);
				CXTPPropExchangeArchive px(ar);

				if (!_ReadRecordsFromData(&px, *m_pDropRecords))
				{
					m_pDropRecords->RemoveAll();
				}
				ar.Close(); // detach from file
			}
			catch (CArchiveException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::OnDragOver() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (CFileException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::OnDragOver() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (...)
			{
				TRACE(_T("EXCEPTION: CXTPGridControl::OnDragOver() - Unhandled Exception!\n"));
			}

			delete pFile;
		}

		m_bDragMode = TRUE;
	}
	else if (nState == 1)
	{
		// leaving, release drop records
		CMDTARGET_RELEASE(m_pDropRecords);

		if ((m_dwDropMarkerFlags & xtpGridDropSelect) && m_pSelectedRowsBeforeDrag)
		{
			// Must remember the selected items before drag.
			m_pSelectedRows->Clear();
			for (int i = 0; i < m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks.GetSize(); i++)
				m_pSelectedRows->m_arrSelectedBlocks.Add(
					m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks[i]);

			CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag);
		}

		m_bDragMode = FALSE;
	}

	COleDataSource* pds = new COleDataSource();

	XTP_NM_GRIDDRAGDROP nmData;
	nmData.pRecords		 = m_pDropRecords;
	nmData.pTargetRecord = pTargetRecord;
	nmData.nTargetRow	= nTargetRow;
	nmData.bAbove		 = bAbove;
	nmData.dropEffect	= dropEffect;
	nmData.pt			 = point;
	nmData.nState		 = nState;

	nmData.pDataSource = pds; // Data Source - issue 22675
	nmData.pDataObject = pDataObject;
	// R-DD <<
	nmData.dwKeyState = dwKeyState;
	// R-DD >>

	m_nOLEDropAbove = nmData.bAbove;

	SendNotifyMessage(XTP_NM_GRID_DRAGOVER, (NMHDR*)&nmData);

	m_nOLEDropAbove = nmData.bAbove;

	if (nState != 1 && nmData.dropEffect == DROPEFFECT_NONE
		&& (m_dwDropMarkerFlags & xtpGridDropSelect))
		m_pSelectedRows->Clear();

	if (m_nDropPos != nDropPos)
		RedrawControl();

	pds->InternalRelease();

	return nmData.dropEffect;
}

BOOL CXTPGridControl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CMDTARGET_RELEASE(m_pDropRecords);

	EnsureStopAutoVertScroll();

	m_bDragMode = FALSE;

	if (m_nDropPos == -1)
		return FALSE;

	CUpdateContext updateContext(this);
	int nDropPos = m_nDropPos;
	m_nDropPos   = -1;

	if (dropEffect != DROPEFFECT_COPY && dropEffect != DROPEFFECT_MOVE)
		return FALSE;

#ifdef _XTP_ACTIVEX
	BOOL bSkipOLEDrop = (m_dwDragDropFlags & xtpGridAllowDrop) && !IsVirtualMode() && pDataObject
						&& pDataObject->IsDataAvailable(m_cfGrid);

	if (m_nOLEDropMode && pDataObject && !bSkipOLEDrop)
	{
		CXTPOleDataObject::FireOleDropEvent((COleControl*)GetParent(), pDataObject, dropEffect,
											point);

		return dropEffect != DROPEFFECT_NONE;
	}
#endif

	// if (IsVirtualMode()) return FALSE;
	int nTargetRow = -1;
	if ((m_dwDropMarkerFlags & xtpGridDropSelect) && m_pSelectedRows->GetCount() == 1)
		nTargetRow = m_pSelectedRows->GetAt(0)->GetIndex();

	XTP_NM_GRIDDRAGDROP nmData;
	nmData.dropEffect  = dropEffect;
	nmData.pDataObject = pDataObject;
	nmData.pt		   = point;

	int iValid		= (int)SendNotifyMessage(XTP_NM_GRID_HASVALIDDROPTYPE, (NMHDR*)&nmData);
	m_nOLEDropAbove = nmData.bAbove;
	if (iValid != -1 && nmData.dropEffect == DROPEFFECT_NONE)
	{
		// Drop handled or not allowed.
		if ((m_dwDropMarkerFlags & xtpGridDropSelect) && m_pSelectedRowsBeforeDrag)
		{
			// Must remember the selected items before drag.
			m_pSelectedRows->Clear();
			for (int i = 0; i < m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks.GetSize(); i++)
				m_pSelectedRows->m_arrSelectedBlocks.Add(
					m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks[i]);

			CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag);
		}
		return FALSE;
	}

	if ((m_dwDropMarkerFlags & xtpGridDropSelect) && m_pSelectedRowsBeforeDrag)
	{
		// Must remember the selected items before drag.
		m_pSelectedRows->Clear();
		for (int i = 0; i < m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks.GetSize(); i++)
			m_pSelectedRows->m_arrSelectedBlocks.Add(
				m_pSelectedRowsBeforeDrag->m_arrSelectedBlocks[i]);

		CMDTARGET_RELEASE(m_pSelectedRowsBeforeDrag);
	}

	if (!pDataObject || (!pDataObject->IsDataAvailable(m_cfGrid) && iValid != 1))
		return FALSE;

	if ((m_dwDragDropFlags & xtpGridAllowDrop) == 0)
		return FALSE;

	int nInsert = GetRecords()->GetCount();

	BOOL bAbove						= TRUE;
	CXTPGridRecord* pTargetRecord   = NULL;
	CXTPGridRecords* pTargetRecords = GetRecords();
	CXTPGridRecords* pDropRecords   = NULL;
	CXTPGridRow* pRow				= HitTest(point);
	if (pRow)
	{
		if (pRow->GetRect().CenterPoint().y < point.y)
			bAbove = FALSE;

		pTargetRecord = pRow->GetRecord();
		if (pTargetRecord)
		{
			nInsert		   = pTargetRecord->GetIndex();
			pTargetRecords = pTargetRecord->GetRecords();

			if (!bAbove)
			{
				if (pTargetRecord->HasChildren() && pTargetRecord->IsExpanded())
				{
					pTargetRecords = pTargetRecord->GetChilds();
					nInsert		   = 0;
				}
				else
					nInsert++;
			}
		}
	}

	m_bInternalMove = FALSE;

	if (m_bInternalDrag && dropEffect == DROPEFFECT_MOVE)
	{
		m_bInternalMove = TRUE;

		if (nDropPos == -1)
			return FALSE;

		BOOL bRedraw = FALSE;
		if (!m_bNoNeedSortedDragDrop && m_bSortedDragDrop
			&& GetColumns()->GetGroupsOrder()->GetCount() == 0)
		{
			ReleaseSorted();
			bRedraw = TRUE;
		}

		pDropRecords = new CXTPGridRecords(TRUE);
		if (!_GetSelectedRows(pDropRecords) || pDropRecords->GetCount() == GetRows()->GetCount())
		{
			CMDTARGET_RELEASE(pDropRecords);
			return FALSE;
		}

		if (bRedraw)
		{
			Populate();
			RedrawControl();
		}

		pDropRecords->CleanOfRedundant();

		nmData.pRecords		 = pDropRecords;
		nmData.pTargetRecord = pTargetRecord;
		nmData.nTargetRow	= nTargetRow;
		nmData.bAbove		 = bAbove;
		nmData.dropEffect	= dropEffect;
		nmData.pDataObject   = pDataObject;
		// nmData.bReturnValue = TRUE; //James proposal (wrong!)
		nmData.bReturnValue = FALSE;

		m_nOLEDropAbove = nmData.bAbove;
		CMDTARGET_ADDREF(pTargetRecord);

		if (SendNotifyMessage(XTP_NM_GRID_DROP, (NMHDR*)&nmData) != -1)
		{
			m_nOLEDropAbove = nmData.bAbove;

			if (!IsVirtualMode())
			{
				if (m_bUnrestrictedDragDrop)
					pDropRecords->PrepareToDropping(pTargetRecord, pTargetRecords, TRUE);

				if (pDropRecords->GetCount() == 0)
				{
					CMDTARGET_RELEASE(pDropRecords);
					return FALSE;
				}

				if (pTargetRecords)
					pTargetRecords->Move(nInsert, pDropRecords);
				Populate();
				_SelectRows(pDropRecords);

				RedrawControl();
			}

			SendNotifyMessage(XTP_NM_GRID_RECORDS_DROPPED, (NMHDR*)&nmData);
		}

		CMDTARGET_RELEASE(pDropRecords);
		CMDTARGET_RELEASE(pTargetRecord);

		return nmData.bReturnValue;
	}

	if (pDataObject->IsDataAvailable(m_cfGrid))
	{
		CFile* pFile = pDataObject->GetFileData(m_cfGrid);
		if (pFile)
		{
			pDropRecords = new CXTPGridRecords();

			const int cErrTextSize = 1024;
			TCHAR szErrText[cErrTextSize + 1];

			try
			{
				CArchive ar(pFile, CArchive::load);
				CXTPPropExchangeArchive px(ar);

				if (!_ReadRecordsFromData(&px, *pDropRecords))
				{
					pDropRecords->RemoveAll();
				}
				ar.Close(); // detach from file
			}
			catch (CArchiveException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::OnDrop() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (CFileException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::OnDrop() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (...)
			{
				TRACE(_T("EXCEPTION: CXTPGridControl::OnDrop() - Unhandled Exception!\n"));
			}

			delete pFile;
		}
	}

	nmData.pRecords		 = pDropRecords;
	nmData.pTargetRecord = pTargetRecord;
	nmData.nTargetRow	= nTargetRow;
	nmData.bAbove		 = bAbove;
	nmData.dropEffect	= dropEffect;
	nmData.pDataObject   = pDataObject;
	nmData.bReturnValue  = TRUE;

	m_nOLEDropAbove = nmData.bAbove;
	CMDTARGET_ADDREF(pTargetRecord);

	if (SendNotifyMessage(XTP_NM_GRID_DROP, (NMHDR*)&nmData) == -1)
	{
		CMDTARGET_RELEASE(pDropRecords);
		CMDTARGET_RELEASE(pTargetRecord);
		return nmData.bReturnValue;
	}

	m_nOLEDropAbove = nmData.bAbove;

	if (!IsVirtualMode() && pDropRecords && pTargetRecords) // DROPEFFECT_COPY
	{
		pDropRecords->CleanOfRedundant();

		// Add and Populate records
		int nRecordsCount = pDropRecords->GetCount();
		if (nRecordsCount > 0)
		{
			// Add
			for (int i = 0; i < nRecordsCount; i++)
			{
				CXTPGridRecord* pRecord = pDropRecords->GetAt(i);
				if (pRecord)
				{
					CMDTARGET_ADDREF(pRecord);
					pTargetRecords->InsertAt(nInsert, pRecord);
					nInsert++;
				}
			}

			Populate();
			_SelectRows(pDropRecords);

			RedrawControl();

			SendNotifyMessage(XTP_NM_GRID_RECORDS_DROPPED, (NMHDR*)&nmData);
		}
	}
	else
	{
		SendNotifyMessage(XTP_NM_GRID_RECORDS_DROPPED, (NMHDR*)&nmData);
	}

	CMDTARGET_RELEASE(pDropRecords);
	CMDTARGET_RELEASE(pTargetRecord);

	return nmData.bReturnValue;
}

void CXTPGridControl::ReleaseSorted()
{
	int nSortCnt		= GetColumns()->GetSortOrder()->GetCount();
	CXTPGridRows* pRows = GetRows();
	if (nSortCnt > 0 && pRows)
	{
		GetColumns()->GetSortOrder()->Clear();

		CXTPGridRecord* pDropRec = NULL;
		int N					 = pRows->GetCount();
		for (int J = 0; J < N; J++)
		{
			pDropRec = pRows->GetAt(J)->GetRecord();
			if (pDropRec)
			{
				CXTPGridRecords* pDropRecs = new CXTPGridRecords(TRUE);
				pDropRecs->Add(pDropRec);
				GetRecords()->Move(J, pDropRecs);
				delete pDropRecs;
			}
		}
		m_pSectionBody->m_UaSorted.RemoveAll();
	}
	if (GetColumns()->GetGroupsOrder()->GetCount() > 0)
		GetColumns()->GetGroupsOrder()->Clear();

	m_bNoNeedSortedDragDrop = TRUE;
	m_bSortedDragDrop		= TRUE;
}

void CXTPGridControl::OnTimer(UINT_PTR uTimerID)
{
	if (m_uAutoScrollTimerID == uTimerID)
	{
		CPoint ptMouse;
		if (::GetCursorPos(&ptMouse))
		{
			ScreenToClient(&ptMouse);
			DoAutoVertScrollIfNeed(m_ptDrag, ptMouse);
		}
	}

	if (m_uRqstEditTimer == uTimerID)
	{
		m_iLastRqstEditRow = -1;
		m_iLastRqstEditCol = -1;
	}

	// Information for delay editing.
	if (m_uiDelayEditTimer == uTimerID)
	{
		EnsureStopDelayEditTimer();

		ASSERT(!IsEditMode());
		if (m_iLastRqstEditRow != -1 && m_iLastRqstEditCol != -1
			&& m_iLastRqstEditRow < GetRows()->GetCount() && IsEditOnDelayClick())
		{
			XTP_GRIDRECORDITEM_ARGS args(this, GetRows()->GetAt(m_iLastRqstEditRow),
										 GetColumns()->Find(m_iLastRqstEditCol));
			EditItem(&args);
			StartLastRqstTimer();
		}
	}

	CWnd::OnTimer(uTimerID);
}

void CXTPGridControl::EnsureStopAutoVertScroll()
{
	if (m_uAutoScrollTimerID)
	{
		KillTimer(m_uAutoScrollTimerID);
		m_uAutoScrollTimerID = 0;
	}
}

void CXTPGridControl::EnsureStartAutoVertScroll()
{
	if (0 == m_uAutoScrollTimerID)
	{
		m_uAutoScrollTimerID = SetTimer(XTP_GRID_AUTO_SCROLL_TIMER_ID,
										m_nAutoVScrollTimerResolution, NULL);
	}
}

void CXTPGridControl::SetAutoVScrollTimerResolution(UINT nNewTimerResolution)
{
	if (nNewTimerResolution == m_nAutoVScrollTimerResolution)
		return;

	BOOL bStartTimer = (m_uAutoScrollTimerID != 0);

	EnsureStopAutoVertScroll();
	m_nAutoVScrollTimerResolution = nNewTimerResolution;

	if (bStartTimer)
		EnsureStartAutoVertScroll();
}

void CXTPGridControl::DoAutoVertScrollIfNeed(CPoint ptClick, CPoint pt)
{
	enum XTPGridControlDirection
	{
		xtpGridControlDirectionNone = 0,
		xtpGridControlDirectionUp   = -1,
		xtpGridControlDirectionDown = 1
	};

	CXTPGridScreenRows* pScreenRows = GetRows()->GetScreenRows();

	if (0 == pScreenRows->GetSize())
	{
		EnsureStopAutoVertScroll();
		return;
	}

	CXTPGridRow* pRowUp   = pScreenRows->GetAt(0);
	CXTPGridRow* pRowDown = pScreenRows->GetAt(pScreenRows->GetSize() - 1);

	if (NULL == pRowUp || NULL == pRowDown)
	{
		EnsureStopAutoVertScroll();
		return;
	}

	XTPGridControlDirection nDirection = xtpGridControlDirectionNone;

	CRect rcUp   = m_pSectionBody->m_rcSection;
	CRect rcDown = m_pSectionBody->m_rcSection;
	rcUp.bottom  = m_pSectionBody->m_rcSection.top + XTP_DPI_Y(20); // while dragging item
	rcDown.top   = m_pSectionBody->m_rcSection.bottom - XTP_DPI_Y(20);

	if (rcUp.PtInRect(pt))
	{
		nDirection = xtpGridControlDirectionUp;
	}
	if (rcDown.PtInRect(pt))
	{
		nDirection = xtpGridControlDirectionDown;
	}

	if (xtpGridControlDirectionNone == nDirection
		|| m_bDragMode
			   && ((xtpGridControlDirectionDown == nDirection && pt.y - ptClick.y < XTP_DPI_Y(3))
				   || (xtpGridControlDirectionUp == nDirection && ptClick.y - pt.y < XTP_DPI_Y(3))))
	{
		EnsureStopAutoVertScroll();
		return;
	}

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	GetScrollInfo(SB_VERT, &si, SIF_ALL);

	const int nScrollFactor = 2; // Factor to speed up smooth scrolling

	if (xtpGridControlDirectionUp == nDirection && si.nPos > 0)
	{
		m_nScrollStepV *= nScrollFactor;
		OnVScroll(SB_LINEUP, 0, NULL);
		m_nScrollStepV /= nScrollFactor;
		EnsureStartAutoVertScroll();
	}
	else if (xtpGridControlDirectionDown == nDirection && si.nPos + int(si.nPage) - 1 < si.nMax)
	{
		m_nScrollStepV *= nScrollFactor;
		OnVScroll(SB_LINEDOWN, 0, NULL);
		m_nScrollStepV /= nScrollFactor;
		EnsureStartAutoVertScroll();
	}
	else
	{
		EnsureStopAutoVertScroll();
	}
}

BOOL CXTPGridControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		switch (GetMouseMode())
		{
			case xtpGridMouseOverColumnDivide:
				SetCursor(GetGridHeader()->m_hResizeCursor);
				return TRUE;

			case xtpGridMouseOverRowDivide:
				SetCursor(GetGridHeader()->m_hResizeVCursor);
				return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CXTPGridControl::SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR) const
{
	LRESULT lResult = 0;

	if (IsWindow(m_hWnd))
	{
		NMHDR nmhdr;

		if (pNMHDR == NULL)
		{
			pNMHDR = &nmhdr;
		}

		pNMHDR->hwndFrom = GetSafeHwnd();
		pNMHDR->idFrom   = XTPToULongPtr(GetDlgCtrlID());
		pNMHDR->code	 = nMessage;

		CWnd* pOwner = GetOwner();

		if (pOwner && IsWindow(pOwner->m_hWnd))
		{
			lResult = pOwner->SendMessage(WM_NOTIFY, pNMHDR->idFrom, LPARAM(pNMHDR));
		}
	}

	return lResult;
}

LRESULT CXTPGridControl::SendMessageToParent(CXTPGridRow* pRow, CXTPGridRecordItem* pItem,
											 CXTPGridColumn* pColumn, UINT nMessage, CPoint* pPoint,
											 int nHyperlink) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	XTP_NM_GRIDRECORDITEM nmgv;
	nmgv.pItem		= pItem;
	nmgv.pColumn	= pColumn;
	nmgv.pRow		= pRow;
	nmgv.nHyperlink = nHyperlink;
	nmgv.pt.x		= 0;
	nmgv.pt.y		= 0;
	if (pPoint)
		nmgv.pt = *pPoint;

	return SendNotifyMessage(nMessage, (NMHDR*)&nmgv);
}

void CXTPGridControl::DoPropExchange(CXTPPropExchange* pPX)
{
	/*
		 9.8.1 (_XTP_SCHEMA_98)
		10.1.0 (_XTP_SCHEMA_1000)
		10.2.0 (_XTP_SCHEMA_1000)
		10.3.0 (_XTP_SCHEMA_1030)
		10.3.1 (_XTP_SCHEMA_1030)
		10.4.0 (_XTP_SCHEMA_1040)
		10.4.1 (_XTP_SCHEMA_1041)
		10.4.2 (_XTP_SCHEMA_1041)
		- [Columns]
		- ShowGroupBox
		- FreezeColumnsCount
		- [Header]

	  11.1.0 (_XTP_SCHEMA_1100)
	  11.1.2 (_XTP_SCHEMA_1100)
	  12.0.1 (_XTP_SCHEMA_1200)
	  12.1.0 (_XTP_SCHEMA_1210)
	  13.0.0 (_XTP_SCHEMA_1210)
		- [Columns]
		- ShowGroupBox
		- FreezeColumnsCount
		- FullColumnScrolling       >= _XTP_SCHEMA_1100
		- HScrollStep               >= _XTP_SCHEMA_1100
		- [Header]

	  13.1.0 (_XTP_SCHEMA_1210)
		- [Columns]
		- ShowGroupBox
		- FreezeColumnsCount
		- FullColumnScrolling       >= _XTP_SCHEMA_1100
		- HScrollStep               >= _XTP_SCHEMA_1100
		- [Header]
		- ShowIconView

	  13.2.0 (_XTP_SCHEMA_1310)
		- [Columns]
		- ShowGroupBox
		- FullColumnScrolling       >= _XTP_SCHEMA_1100
		- HScrollStep               >= _XTP_SCHEMA_1100
		- FreezeColumnsCount        !! Order changed !!
		- FreezeColumnsAbs          >= _XTP_SCHEMA_1310
		- MovePivot                 >= _XTP_SCHEMA_1310
		- ForcePagination           >= _XTP_SCHEMA_1310
		- StrictBestFit             >= _XTP_SCHEMA_1310
		- SelectionExcludeGroupRows >= _XTP_SCHEMA_1310
		- DesktopTrackerMode        >= _XTP_SCHEMA_1310
		- [Header]
		- ShowIconView
	*/

	// ICON_VIEW_MODE RELATED <<
	BeginUpdate();

	BOOL bCurIconView		= IsIconView();
	BOOL bSetBackToIconView = bCurIconView;
	if (bSetBackToIconView)
		SetIconView(FALSE);

	// ICON_VIEW_MODE RELATED >>
	TRY
	{
		pPX->ExchangeSchemaSafe();

		CXTPPropExchangeSection secColumns(pPX->GetSection(_T("Columns")));
		m_pColumns->DoPropExchange(&secColumns);

		PX_Bool(pPX, _T("ShowGroupBox"), m_bGroupByEnabled, FALSE);

		if (pPX->GetSchema() >= _XTP_SCHEMA_1100)
		{
			BOOL bFullColumnScrolling = IsFullColumnScrolling();
			PX_Bool(pPX, _T("FullColumnScrolling"), bFullColumnScrolling, FALSE);
			PX_Int(pPX, _T("HScrollStep"), m_nScrollStepH, XTP_GRID_HSCROLL_STEP);

			if (pPX->IsLoading())
			{
				SetFullColumnScrolling(bFullColumnScrolling);
			}
		}

		if (pPX->GetSchema() >= _XTP_SCHEMA_1310)
		{
			// The order of "FreezeColumnsCount" has been changed with 13.2.0
			int nFreezeColumnsCount = GetFreezeColumnsCount();
			BOOL bFreezeColumnsAbs  = FALSE;
			PX_Int(pPX, _T("FreezeColumnsCount"), nFreezeColumnsCount, 0);
			PX_Bool(pPX, _T("FreezeColumnsAbs"), bFreezeColumnsAbs, FALSE); // Removed
			PX_Bool(pPX, _T("MovePivot"), m_bMovePivot, FALSE);
			PX_Bool(pPX, _T("ForcePagination"), m_bForcePagination, FALSE);
			PX_Bool(pPX, _T("StrictBestFit"), m_bStrictBestFit, FALSE);
			PX_Bool(pPX, _T("SelectionExcludeGroupRows"), m_bSelectionExcludeGroupRows, TRUE);
			PX_Bool(pPX, _T("DesktopTrackerMode"), m_bDesktopTrackerMode, FALSE);

			if (pPX->IsLoading())
			{
				SetFreezeColumnsCount(nFreezeColumnsCount);
			}
		}

		CXTPPropExchangeSection secHeader(pPX->GetSection(_T("Header")));
		GetGridHeader()->DoPropExchange(&secHeader);

		if (pPX->IsLoading())
		{
			GetGridHeader()->OnColumnsChanged(xtpGridColumnOrderChanged | xtpGridColumnPropExchange,
											  NULL);
			Populate();
		}
	}
	CATCH(CArchiveException, e)
	{
		ASSERT(FALSE);
	}
	END_CATCH
	// ICON_VIEW_MODE RELATED <<
	TRY
	{
		pPX->ExchangeSchemaSafe();

		PX_Bool(pPX, _T("ShowIconView"), bCurIconView);

		if (pPX->IsLoading() && bCurIconView)
			SetIconView(bCurIconView);
	}
	CATCH(CArchiveException, e)
	{
	}
	END_CATCH

	if (pPX->IsStoring() && bSetBackToIconView)
		SetIconView(TRUE);

	EndUpdate();
	// ICON_VIEW_MODE RELATED >>
}

// ICON_VIEW_MODE RELATED <<
void CXTPGridControl::SetIconView(BOOL bIconView /* = TRUE */)
{
	if (m_iIconViewColumn == -1) // No Icon Column Set
	{
		m_bIconView = FALSE;
		return;
	}

	if (m_bIconView == bIconView)
		return;

	CXTPGridColumn* pColumn = NULL;

	if (bIconView)
	{
		m_pNavigator = m_pNavigatorIcon;

		if (!m_pPrevGroupsOrder)
			m_pPrevGroupsOrder = new CXTPGridColumnOrder(GetColumns());

		if (!m_pPrevVisible)
			m_pPrevVisible = new CXTPGridColumnOrder(GetColumns());

		if (m_pPrevGroupsOrder)
		{
			m_pPrevGroupsOrder->Clear();

			CXTPGridColumnOrder* pCurrentColumnOrder = GetColumns()->GetGroupsOrder();
			if (pCurrentColumnOrder->GetCount())
			{
				for (int i = 0; i < GetColumns()->GetCount(); i++)
				{
					pColumn = pCurrentColumnOrder->GetAt(i);
					if (pColumn)
					{
						m_pPrevGroupsOrder->Add(pColumn);
						pColumn->SetVisible(FALSE);
					}
				}
				pCurrentColumnOrder->Clear();
				Populate();
			}
		}
		if (m_pPrevVisible)
		{
			m_pPrevVisible->Clear();

			for (int i = 0; i < GetColumns()->GetCount(); i++)
			{
				pColumn = GetColumns()->GetAt(i);
				if (pColumn && pColumn->IsVisible())
				{
					m_pPrevVisible->Add(pColumn);
					pColumn->SetVisible(FALSE);
				}
			}
		}

		m_iIconWidth		 = GetSystemMetrics(SM_CXICON);
		m_iIconHeight		 = GetSystemMetrics(SM_CYICON);
		m_iIconWidthSpacing  = GetSystemMetrics(SM_CXICONSPACING) + XTP_DPI_X(5);
		m_iIconHeightSpacing = GetSystemMetrics(SM_CYICONSPACING);

		m_iIconHeightSpacing = max(m_iIconHeightSpacing,
								   (((CXTPGridPaintManager*)GetPaintManager())->m_nRowHeight << 1)
									   + m_iIconHeight);

		pColumn = GetColumns()->Find(m_iIconViewColumn);
		if (pColumn)
		{
			pColumn->SetVisible(TRUE);

			if (pColumn->IsResizable())
				GetPaintManager()->m_RecordNumberWidth = pColumn->GetWidth();
			pColumn->SetWidth(m_iIconWidth);
			pColumn->SetEditable(TRUE);
		}

		m_bPrevHeaderShow = IsHeaderVisible();
		m_bPrevFooterShow = IsFooterVisible();

		ShowHeader(FALSE);
		ShowFooter(FALSE);

		m_bPrevFocusSubItems  = IsFocusSubItems();
		m_bPrevHeaderAutoSize = GetGridHeader()->IsAutoColumnSizing();
		// This is not needed since we only have one column and it screws up the selection.
		FocusSubItems(FALSE);
		GetGridHeader()->SetAutoColumnSizing(TRUE);

		m_bPrevHeaderRows = IsHeaderRowsVisible();
		m_bPrevFooterRows = IsFooterRowsVisible();

		ShowHeaderRows(FALSE);
		ShowFooterRows(FALSE);

		// Need this so the scrollbar is correct at the beginning.
		m_iIconViewRowsPerLine			 = GetNumRowsOnLine(GetGridHeader()->GetWidth());
		m_nPrevTreeIndent				 = GetPaintManager()->m_nTreeIndent;
		GetPaintManager()->m_nTreeIndent = 0;
		m_bPrevPreviewMode				 = IsPreviewMode();
		GetPaintManager()->EnablePreviewMode(FALSE);
	}
	else
	{
		m_pNavigator = m_pNavigatorGrid;

		GetPaintManager()->m_nTreeIndent = m_nPrevTreeIndent;
		GetGridHeader()->SetAutoColumnSizing(m_bPrevHeaderAutoSize);

		pColumn = GetColumns()->Find(m_iIconViewColumn);
		if (pColumn)
		{
			if (m_bUseIconColumnForNum)
			{
				if (m_iColumnForNum > -1)
				{
					if (m_pPrevGroupsOrder && m_pPrevGroupsOrder->GetCount() > 0
						&& GetPaintManager()->m_RecordNumberWidth < XTP_DPI_X(30))
						GetPaintManager()->m_RecordNumberWidth = XTP_DPI_X(30);

					pColumn->SetWidth(GetPaintManager()->m_RecordNumberWidth);
					pColumn->SetMinWidth(GetPaintManager()->m_RecordNumberWidth);
					pColumn->SetVisible(TRUE);
				}
				else
					pColumn->SetVisible(FALSE);

				pColumn->SetHeaderAlignment(DT_CENTER);
				pColumn->SetEditable(FALSE);
			}
			else
			{
				pColumn->SetVisible(FALSE);
				pColumn->SetWidth(0);
			}
		}
		if (m_pPrevVisible)
		{
			for (int i = 0; i < m_pPrevVisible->GetCount(); i++)
			{
				pColumn = m_pPrevVisible->GetAt(i);
				if (pColumn)
				{
					pColumn->SetVisible(TRUE);
					if (pColumn->GetWidth() == 0)
						pColumn->SetWidth(pColumn->GetMinWidth());
				}
			}
		}

		if (m_pPrevGroupsOrder)
		{
			for (int i = 0; i < m_pPrevGroupsOrder->GetCount(); i++)
			{
				pColumn = m_pPrevGroupsOrder->GetAt(i);
				if (pColumn)
				{
					GetColumns()->GetGroupsOrder()->Add(pColumn);
					pColumn->SetVisible(TRUE);
				}
			}
			Populate();
		}

		ShowHeader(m_bPrevHeaderShow);
		ShowFooter(m_bPrevFooterShow);

		if (m_bPrevFocusSubItems)
			FocusSubItems(m_bPrevFocusSubItems);

		ShowHeaderRows(m_bPrevHeaderRows);
		ShowFooterRows(m_bPrevFooterRows);

		GetPaintManager()->EnablePreviewMode(m_bPrevPreviewMode);
	}

	BOOL bOldFirst(TRUE);

	if (bOldFirst)
	{
		if (GetRows()->GetAt(0))
			GetRows()->GetAt(0)->SetSelected(FALSE);
	}

	m_bIconView = bIconView;

	AdjustScrollBars();
}

int CXTPGridControl::GetNumRowsOnLine(int iTotalWidth) const
{
	return max(iTotalWidth / m_iIconWidthSpacing, 1);
}

// ICON_VIEW_MODE RELATED >>

void CXTPGridControl::SerializeState(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}

void CXTPGridControl::CollapseAll()
{
	BeginUpdate();

	m_pSectionBody->CollapseAll();
	AdjustScrollBars();

	EndUpdate();
}

void CXTPGridControl::ExpandAll(BOOL bRecursive)
{
	BeginUpdate();

	m_pSectionBody->ExpandAll(bRecursive);
	AdjustScrollBars();

	EndUpdate();
}

void CXTPGridControl::SetMouseMode(XTPGridMouseMode nMode)
{
	XTP_TRACE(_T("SetMouseMode: Switching from %d to %d\n"), GetMouseMode(), nMode);
	m_mouseMode = nMode;
}

void CXTPGridControl::RelayToolTipEvent(UINT message)
{
	if (m_pTip->GetSafeHwnd() && m_pTip->IsWindowVisible())
	{
		CPoint pt;
		GetCursorPos(&pt);

		if (!m_pTip->GetHoverRect().PtInRect(pt))
		{
			m_pTip->SetTooltipText(NULL);
			m_pTip->Activate(FALSE, FALSE);
		}

		switch (message)
		{
			case WM_MOUSEWHEEL:
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_MOUSELEAVE: m_pTip->Activate(FALSE, FALSE);
		}
	}
}

BOOL CXTPGridControl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	m_bRelayWndMsg = FALSE;

	if (m_pTip->GetSafeHwnd() && m_pTip->IsWindowVisible() && !m_bRelayWndMsg)
	{
		m_bRelayWndMsg = TRUE;
		RelayToolTipEvent(message);
		m_bRelayWndMsg = FALSE;
	}

	if (m_pToolTipContext && m_bShowTooltips)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	if (GetMarkupContext())
	{
		CPoint ptMouse(0);
		GetCursorPos(&ptMouse);
		ScreenToClient(&ptMouse);

		m_bRelayWndMsg = FALSE;

		CXTPGridRow* pRow = HitTest(ptMouse);
		if (pRow)
		{
			CXTPGridRecordItem* pItem = pRow->HitTest(ptMouse);
			if (pItem && pItem->GetMarkupUIElement())
			{
				m_bRelayWndMsg = TRUE;
				if (XTPMarkupRelayMessage(pItem->GetMarkupUIElement(), message, wParam, lParam,
										  pResult))
					return TRUE;
			}
			// 13.2.2 <<
			// enable markup mouse move for groups
			else if (pRow->IsGroupRow())
			{
				m_bRelayWndMsg = TRUE;
				if (XTPMarkupRelayMessage(((CXTPGridGroupRow*)pRow)->m_pMarkupUIElement, message,
										  wParam, lParam, pResult))
					return TRUE;
			}
			// 13.2.2 >>
		}

		if (!m_bRelayWndMsg)
		{
			if (XTPMarkupRelayMessage(GetMarkupContext(), message, wParam, lParam, pResult))
				return TRUE;
		}
	}
	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPGridControl::OnSysColorChange()
{
	CWnd::OnSysColorChange();
	m_pPaintManager->RefreshMetrics();

	RedrawControl();
}

UINT CXTPGridControl::OnGetDlgCode()
{
	return (UINT(m_bFocusSubItems ? DLGC_WANTTAB : 0) | DLGC_WANTARROWS | DLGC_WANTCHARS);
}

void CXTPGridControl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	RedrawControl();

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, this, TRUE, pOldWnd)
#endif
}

void CXTPGridControl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	EnsureStopAutoVertScroll();
	RedrawControl();

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, this, FALSE, NULL)
#endif
}

int CXTPGridControl::GetHeaderIndent() const
{
	return GetIndent(GetGridHeader()->m_nIndentLevel);
}

CXTPGridRow* CXTPGridControl::CreateRow()
{
	return new CXTPGridRow_Batch();
}

CXTPGridGroupRow* CXTPGridControl::CreateGroupRow()
{
	return new CXTPGridGroupRow_Batch();
}

CXTPGridRow* CXTPGridControl::CreateHeaderFooterRow()
{
	return new CXTPHeapObjectT<CXTPGridRow, CXTPGridAllocatorDefault>;
}

void CXTPGridControl::ShowIconWhenEditing(BOOL bShow)
{
	m_bShowIconWhenEditing = bShow;
}

BOOL CXTPGridControl::IsShowIconWhenEditing()
{
	return m_bShowIconWhenEditing;
}

void CXTPGridControl::EditItem(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	CXTPGridRecordItem* pItem = pItemArgs ? pItemArgs->pItem : NULL;

	if (NULL == m_pActiveItem && NULL == pItem)
	{
		// Nothing do to
		return;
	}

	if (m_pActiveItem != NULL)
	{
		m_pActiveItem->OnCancelEdit(this, TRUE);
		m_pActiveItem = NULL;

		if (!m_bFocusSubItems)
			SetFocusedColumn(NULL);
	}

	CMDTARGET_RELEASE(m_ptrVirtualEditingRow);

	if (pItem && pItemArgs && pItemArgs->pRow)
	{
		if (!HasFocus())
			SetFocus();

		if (!IsVirtualMode())
		{
			AdjustScrollBars();
			RedrawControl();
			UpdateWindow();
		}

		if (IsVirtualMode())
		{
			int nRowIndex			  = pItemArgs->pRow->GetIndex();
			CXTPGridSection* pSection = pItemArgs->pRow->GetSection();
			ASSERT(pSection != NULL);

			// EnsureVisible(pItemArgs->pRow);

			RedrawControl();
			UpdateWindow();
			pItemArgs->pRow = NULL; // RedrawControl delete and re-create new screen rows

			BOOL bMapped = FALSE;

			if (pSection)
			{
				CXTPGridScreenRows* pScreenRows = pSection->GetRows()->GetScreenRows();
				for (int i = 0; i < pScreenRows->GetSize(); i++)
				{
					CXTPGridRow* pRow = pScreenRows->GetAt(i);
					if (pRow->GetIndex() == nRowIndex)
					{
						pItemArgs->pRow = pRow;
						bMapped			= TRUE;

						ASSERT(m_ptrVirtualEditingRow == NULL);

						m_ptrVirtualEditingRow = pRow;
						CMDTARGET_ADDREF(m_ptrVirtualEditingRow);
						break;
					}
				}
			}
			ASSERT(bMapped);
			if (!bMapped)
				return;
		}
		else if (GetFocusedRow() != pItemArgs->pRow)
		{
			BOOL bControlKey = (::GetKeyState(VK_CONTROL) < 0) || m_bMultiSelectionMode;
			SetFocusedRow(pItemArgs->pRow, bControlKey);
			UpdateWindow();
		}

		pItemArgs->rcItem = pItemArgs->pRow->GetItemRect(pItem, TRUE);
		pItemArgs->rcItem.left += (pItemArgs->rcItem.left == 0 ? GetHeaderIndent() : 0);

		if (m_bShowIconWhenEditing && pItemArgs->pColumn)
		{
			XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS();
			XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
			drawArgs.pControl   = this;
			drawArgs.pColumn	= pItemArgs->pColumn;
			drawArgs.rcItem		= pItemArgs->pColumn->GetRect();
			drawArgs.nTextAlign = pItem->GetAlignment() == -1 ? pItemArgs->pColumn->GetAlignment()
															  : pItem->GetAlignment();
			drawArgs.nTextAlign = drawArgs.nTextAlign == -1 ? 0 : drawArgs.nTextAlign;

			drawArgs.pRow  = pItemArgs->pRow;
			drawArgs.pItem = pItemArgs->pItem;

			pItemArgs->pRow->GetItemMetrics(&drawArgs, pMetrics);

			int nIcon = pMetrics->nItemIcon;
			nIcon	 = (nIcon != XTP_GRID_NOICON) ? nIcon : pItem->GetIconIndex();
			if (nIcon != XTP_GRID_NOICON && (drawArgs.nTextAlign & xtpColumnIconCenter) == 0)
			{
				if ((drawArgs.nTextAlign & xtpColumnIconRight) != 0)
					pItemArgs->rcItem.right -= GetPaintManager()->DrawBitmap(NULL, this,
																			 pItemArgs->rcItem,
																			 nIcon);
				else
					pItemArgs->rcItem.left += GetPaintManager()->DrawBitmap(NULL, this,
																			pItemArgs->rcItem,
																			nIcon);
			}

			CMDTARGET_RELEASE(pMetrics);
		}

		if (pItem->GetHasCheckbox() && pItem->GetCanEditCheckboxItem())
			pItemArgs->rcItem.left += XTP_DPI_X(15);

		if (SetFocusedColumn(pItemArgs->pColumn))
		{
			m_pActiveItem = pItem;
			pItem->OnBeginEdit(pItemArgs);

			if (!IsEditMode()) // Need to actually cancel the edit - old version (10) did not have
							   // this call
				EditItem(NULL);
		}

		// Fix selection
		if (GetInplaceEdit()->GetSafeHwnd() && GetInplaceEdit()->GetItem() == pItem)
		{
			CXTPGridRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pItemArgs->pColumn);
			if (pEditOptions && pEditOptions->m_bSelectTextOnEdit)
			{
				GetInplaceEdit()->SetSel(0, -1);
			}
			else
			{
				CString str;
				GetInplaceEdit()->GetWindowText(str);

				if (pEditOptions && pEditOptions->m_bScrollTextOnEdit)
				{
					GetInplaceEdit()->SetSel(str.GetLength(), str.GetLength());
				}
			}
		}
	}

	RedrawControl();
}

BOOL CXTPGridControl::HasFocus() const
{
	const CWnd* pFocusWnd = GetFocus();
	if (!pFocusWnd)
		return FALSE;

	return (pFocusWnd->GetSafeHwnd() == m_hWnd) || (pFocusWnd->GetParent()->GetSafeHwnd() == m_hWnd)
		   || (pFocusWnd->GetOwner()->GetSafeHwnd() == m_hWnd);
}

void CXTPGridControl::ReleaseItem(int nIndex)
{
	CXTPGridRecords* pRecords = GetRecords();

	int i(0);
	if (IsVirtualMode() && pRecords->GetCount())
	{
		CXTPGridRecord* pRecord = pRecords->GetAt(0);

		pRecord->m_arrItems[nIndex]->InternalRelease();
		pRecord->m_arrItems.RemoveAt(nIndex);
	}
	else
	{
		for (i = 0; i < pRecords->GetCount(); i++)
		{
			CXTPGridRecord* pRecord = pRecords->GetAt(i);

			pRecord->m_arrItems[nIndex]->InternalRelease();
			pRecord->m_arrItems.RemoveAt(nIndex);
		}
	}

	CXTPGridColumns* pColumns		= GetColumns();
	CXTPGridColumn* pColumnToRemove = NULL;
	int nColumnsCount				= pColumns->GetCount();
	for (i = 0; i < nColumnsCount; i++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(i);

		if (pColumn->m_nItemIndex > nIndex)
			pColumn->m_nItemIndex--;
		else if (pColumn->m_nItemIndex == nIndex)
			pColumnToRemove = pColumn;
	}

	if (pColumnToRemove)
		pColumns->Remove(pColumnToRemove);
}

void CXTPGridControl::SetVirtualMode(CXTPGridRecord* pVirtualRecord, int nCount, int nFields)
{
	CXTPGridRecords* pRecords = GetRecords();

	if (pRecords)
	{
		if (pVirtualRecord)
		{
			int nItems = pVirtualRecord->GetItemCount();
			if (nItems == 0 && nFields > 0)
				for (int i = 0; i < nFields; i++)
#ifdef _XTP_ACTIVEX
					pVirtualRecord->AddItem(new CXTPGridRecordItemText());
#else
					pVirtualRecord->AddItem(new CXTPGridRecordItem());
#endif
		}
		else
		{
			pVirtualRecord = new CXTPGridRecord();
			if (nFields > 0)
				for (int i = 0; i < nFields; i++)
#ifdef _XTP_ACTIVEX
					pVirtualRecord->AddItem(new CXTPGridRecordItemText());
#else
					pVirtualRecord->AddItem(new CXTPGridRecordItem());
#endif
		}
		if (m_iIconViewColumn > -1)
		{
			int nItems					  = pVirtualRecord->GetItemCount();
			CXTPGridRecordItem* pLastItem = pVirtualRecord->GetItem(nItems - 1);
			if (!pLastItem->IsKindOf(RUNTIME_CLASS(CXTPGridRecordItemIcon)))
				pVirtualRecord->AddItem(new CXTPGridRecordItemIcon());
		}

		pRecords->SetVirtualMode(pVirtualRecord, nCount);
	}
}

BOOL CXTPGridControl::IsVirtualMode() const
{
	CXTPGridRecords* pRecords = GetRecords();
	return pRecords ? pRecords->IsVirtualMode() : FALSE;
}

CXTPGridRow* CXTPGridControl::GetFocusedRow() const
{
	return m_pSections->GetFocusedRow();
}

void CXTPGridControl::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CWnd::OnStyleChanged(nStyleType, lpStyleStruct);

	RedrawControl();
}

CXTPToolTipContext* CXTPGridControl::GetToolTipContext() const
{
	return m_pToolTipContext;
}

INT_PTR CXTPGridControl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	nHit = GetGridHeader()->OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CXTPGridRow* pRow = HitTest(point);
	if (pRow)
		nHit = pRow->OnToolHitTest(point, pTI);

	return nHit;
}

void CXTPGridControl::SetLayoutRTL(BOOL bRightToLeft)
{
	if (!XTPSystemVersion()->IsLayoutRTLSupported())
		return;

	if (!m_hWnd)
		return;

	if (bRightToLeft)
	{
		ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}
	else
	{
		ModifyStyleEx(WS_EX_LAYOUTRTL, 0);
	}

	GetImageManager()->DrawReverted(bRightToLeft);

	GetInplaceEdit()->DestroyWindow();
	AdjustLayout();

	if (GetParent())
	{
		CRect rc;
		GetParent()->GetClientRect(&rc);
		GetParent()->SetWindowPos(NULL, 0, 0, rc.Width() + XTP_DPI_X(1), rc.Height(),
								  SWP_NOMOVE | SWP_NOZORDER);
	}
}

BOOL CXTPGridControl::IsLayoutRTL()
{
	if (!XTPSystemVersion()->IsLayoutRTLSupported())
		return FALSE;

	if (!m_hWnd)
		return FALSE;

	return !!(GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
}

//////////////////////////////////////////////////////////////////////////
// Clipboard operations

BOOL CXTPGridControl::CanCopy()
{
	// Check whether are there any selected rows to be copied
	CXTPGridSelectedRows* pSelRows = GetSelectedRows();

	if ((pSelRows != NULL) && (pSelRows->GetCount() > 0))
		return TRUE;

	return FALSE;
}

BOOL CXTPGridControl::CanCut()
{
	// Old VirtualMode
	if (IsVirtualMode())
		return FALSE;

	return CanCopy();
}

BOOL CXTPGridControl::CanPaste()
{
	CLIPFORMAT uCF_Records = (CLIPFORMAT)::RegisterClipboardFormat(XTPGRIDCTRL_CF_RECORDS);

	BOOL bCan = FALSE;
	COleDataObject odj;
	if (odj.AttachClipboard())
	{
		bCan = odj.IsDataAvailable(CF_TEXT) || odj.IsDataAvailable(CF_UNICODETEXT)
			   || odj.IsDataAvailable(uCF_Records);
	}
	return bCan;
}

void CXTPGridControl::Cut()
{
	CWaitCursor _WC;

	Copy();

	if (IsVirtualMode())
		return;

	// Delete selected rows
	CXTPInternalCollectionT<CXTPGridRow> arSelectedRows;
	_GetSelectedRows(NULL, &arSelectedRows);
	int nSelRowsCount = (int)arSelectedRows.GetSize();
	int nFirstSelRow  = INT_MAX;
	for (int i = nSelRowsCount - 1; i >= 0; i--)
	{
		CXTPGridRow* pRow = arSelectedRows.GetAt(i, FALSE);
		if (pRow)
		{
			nFirstSelRow = min(nFirstSelRow, pRow->GetIndex());
			VERIFY(RemoveRowEx(pRow, FALSE));
		}

		arSelectedRows.RemoveAt(i);
	}

	if (GetSelectedRows())
	{
		GetSelectedRows()->Clear();

		if (nFirstSelRow != INT_MAX)
		{
			GetRows()->m_nFocusedRow = min(nFirstSelRow, GetRows()->GetCount() - 1);

			if (GetFocusedRow())
			{
				SetFocusedRow(GetFocusedRow());
				GetSelectedRows()->Add(GetFocusedRow());
			}
		}
	}

	AdjustScrollBars();
	RedrawControl();
}

// We support text format with \t dividers for record items and
// \r\n dividers for records (simple tab-separated text).
// Such format is also supported by Excel and some other applications.
void CXTPGridControl::Copy()
{
	CWaitCursor _WC;

	int nRowsCount = 1;
	if (GetSelectedRows())
		nRowsCount = GetSelectedRows()->GetCount();

	// minimize memory reallocs to improve performance
	UINT nAveRecordsSize = 1500; // bytes
	UINT nGrowBytes		 = ((nRowsCount * nAveRecordsSize) / 4096 + 1) * 4096;

	UINT nAllocFlags = GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT;

	CSharedFile fileRecords(nAllocFlags, nGrowBytes);

	BOOL bIsRecordsData = FALSE;

	//------------------------------------------------------------------------
	const int cErrTextSize = 1024;
	TCHAR szErrText[cErrTextSize + 1];

	try
	{
		CArchive ar(&fileRecords, CArchive::store);
		CXTPPropExchangeArchive px(ar);

		bIsRecordsData = _WriteSelectedRowsData(&px);

		ar.Close(); // perform Flush() and detach from file
	}
	catch (CArchiveException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPGridControl::Copy() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch (CFileException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPGridControl::Copy() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch (...)
	{
		TRACE(_T("EXCEPTION: CXTPGridControl::Copy() - Unhandled Exception!\n"));
	}

	CString strClipText = _GetSelectedRowsVisibleColsText();

	CLIPFORMAT uCF_Records = (CLIPFORMAT)::RegisterClipboardFormat(XTPGRIDCTRL_CF_RECORDS);

	// Put prepared text into the clipboard
	if (OpenClipboard())
	{
		::EmptyClipboard();

		// 1 - Text
		HGLOBAL hText = XTPAllocStrInGlobalMem(strClipText);
		if (hText != NULL)
		{
			::SetClipboardData(XTP_CF_TEXT_T, hText);
		}

		// 2 - Blob data
		if (bIsRecordsData)
		{
			HGLOBAL hData = fileRecords.Detach();
			::GlobalUnlock(hData); // unlock data
			::SetClipboardData(uCF_Records, hData);
		}

		::CloseClipboard();
	}
}

void CXTPGridControl::Paste()
{
	CWaitCursor _WC;

	CLIPFORMAT uCF_Records = (CLIPFORMAT)::RegisterClipboardFormat(XTPGRIDCTRL_CF_RECORDS);

	CXTPGridRecords arRecords;
	BOOL bTryPasteFromText = TRUE;

	// Retrieve text from the clipboard
	if (!OpenClipboard())
		return;

	if (::IsClipboardFormatAvailable(uCF_Records))
	{
		HGLOBAL hPasteData = ::GetClipboardData(uCF_Records);

		if (hPasteData)
		{
			bTryPasteFromText = FALSE;

			const int cErrTextSize = 1024;
			TCHAR szErrText[cErrTextSize + 1];

			CSharedFile fileSahred;
			fileSahred.SetHandle(hPasteData, FALSE);
			CArchive ar(&fileSahred, CArchive::load);

			try
			{
				CXTPPropExchangeArchive px(ar);

				if (!_ReadRecordsFromData(&px, arRecords))
				{
					arRecords.RemoveAll();
				}
			}
			catch (CArchiveException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::Paste() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (CFileException* pE)
			{
				if (pE->GetErrorMessage(szErrText, cErrTextSize))
				{
					TRACE(_T("EXCEPTION: CXTPGridControl::Paste() - %s\n"), szErrText);
				}
				pE->Delete();
			}
			catch (...)
			{
				TRACE(_T("EXCEPTION: CXTPGridControl::Paste() - Unhandled Exception!\n"));
			}

			//*********
			ar.Close();					// detach from file
			fileSahred.Detach();		// detach from data
			::GlobalUnlock(hPasteData); // unlock data
		}
	}

	UINT uCF_TText = sizeof(TCHAR) == 2 ? CF_UNICODETEXT : CF_TEXT;

	if (bTryPasteFromText && ::IsClipboardFormatAvailable(uCF_TText))
	{
		// Try to get text data from the clipboard
		HGLOBAL hglbPaste = ::GetClipboardData(uCF_TText);

		// Import Text data into the control
		if (hglbPaste != NULL)
		{
			TCHAR* lpszClipboard = (TCHAR*)GlobalLock(hglbPaste);

			if (!_ReadRecordsFromText(lpszClipboard, arRecords))
			{
				arRecords.RemoveAll();
			}
			::GlobalUnlock(hglbPaste);
		}
	}

	::CloseClipboard();

	if (IsVirtualMode())
		return;
	CUpdateContext updateContext(this);

	// Add and Populate records
	int nRecordsCount = arRecords.GetCount();
	if (nRecordsCount > 0)
	{
		// Add
		for (int i = 0; i < nRecordsCount; i++)
		{
			CXTPGridRecord* pRecord = arRecords.GetAt(i);
			if (pRecord)
			{
				CMDTARGET_ADDREF(pRecord);
				AddRecord(pRecord);
			}
		}

		Populate();

		// Select added records
		_SelectRows(&arRecords);
	}
}

// We support text format with \t dividers for record items and
// \r\n dividers for records (simple tab-separated text).
// Such format is also supported by Excel and some other applications.
CString CXTPGridControl::_GetSelectedRowsVisibleColsText()
{
	CString strSelText;

	CXTPGridColumns* pColumns = GetColumns();
	if (NULL == pColumns)
		return _T("");

	const int nColumnCount = pColumns->GetVisibleColumnsCount();

	// Iterate over the selected rows and prepare corresponding records text
	CXTPGridSelectedRows* pSelectedRows = GetSelectedRows();
	if ((pSelectedRows != NULL) && (pSelectedRows->GetCount() > 0))
	{
		POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();
		while (pos)
		{
			CXTPGridRow* pRow = pSelectedRows->GetNextSelectedRow(pos);
			if (NULL == pRow)
				break;

			CXTPGridRecord* pRecord = pRow->GetRecord();
			if (NULL == pRecord)
				continue;

			CStringArray arStrings;
			for (int nCol = 0; nCol < nColumnCount; nCol++)
			{
				CXTPGridColumn* pColumn   = pColumns->GetVisibleAt(nCol);
				CXTPGridRecordItem* pItem = pRecord->GetItem(pColumn);
				if (NULL == pItem)
					continue;

				arStrings.Add(pItem->GetCaption(pColumn));
			}

			BOOL bCanceled = OnBeforeCopyToText(pRecord, arStrings);
			if (bCanceled)
				continue;

			strSelText += XTPStrMake(arStrings, _T("\t"));
			strSelText += _T("\r\n");
		}
	}

	return strSelText;
}

BOOL CXTPGridControl::_ReadRecordsFromText(LPCTSTR pcszText, CXTPGridRecords& rarRecords)
{
	//////////////////////////////////////////////////////////////////////////
	// Insert retrieved text into the control

	CStringArray arRecordsStrings;
	XTPStrSplit(pcszText, _T("\r\n"), arRecordsStrings);

	int nCount = (int)arRecordsStrings.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CString strRecord = arRecordsStrings[i];
		if (strRecord.IsEmpty())
			continue;

		CXTPGridRecord* pNewRec = _CreateRecodFromText(strRecord);
		// Case: pNewRec = NULL - paste was handled by the user

		if (pNewRec)
			rarRecords.Add(pNewRec);
	}

	return TRUE;
}

CXTPGridRecord* CXTPGridControl::_CreateRecodFromText(LPCTSTR pcszRecord)
{
	if (!GetColumns())
	{
		ASSERT(FALSE);
		return NULL;
	}

	CStringArray arStrings;

	// Read each field from the initial string and set visible field one by one
	XTPStrSplit(pcszRecord, _T("\t"), arStrings);

	CXTPGridRecord* pRecord = NULL;
	BOOL bHandled			= OnBeforePasteFromText(arStrings, &pRecord);
	if (bHandled)
		return pRecord;

#if _MFC_VER >= 0x0600 // Not supported by Visual Studio 5.0

	if (!pRecord)
	{
		pRecord = new CXTPGridRecord();
		if (!pRecord)
		{
			return NULL;
		}

		int nDataCount = (int)arStrings.GetSize();

		// Fill record with all items
		int nCount = GetColumns()->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			COleVariant varItem(_T(""));

			CXTPGridRecordItem* pItem = new CXTPGridRecordItemVariant(varItem);
			if (!pItem)
			{
				CMDTARGET_RELEASE(pRecord);
				return NULL;
			}
			pRecord->AddItem(pItem);
		}

		// Iterate all visible columns and set text for each next
		const int nColumnCount = GetColumns()->GetVisibleColumnsCount();
		for (int nCol = 0; nCol < nColumnCount; nCol++)
		{
			COleVariant varItem(nCol < nDataCount ? (LPCTSTR)arStrings[nCol] : _T(""));

			CXTPGridColumn* pColumn   = GetColumns()->GetVisibleAt(nCol);
			CXTPGridRecordItem* pItem = pRecord->GetItem(pColumn);
			ASSERT(pItem);
			if (NULL == pItem)
				continue;

			CXTPGridRecordItemVariant* pItemVar = DYNAMIC_DOWNCAST(CXTPGridRecordItemVariant,
																   pItem);
			ASSERT(pItemVar);

			if (pItemVar)
			{
				pItemVar->m_oleValue = varItem;
			}
		}
	}
#endif

	return pRecord;
}

BOOL CXTPGridControl::_WriteRecordsData(CXTPPropExchange* pPX, CXTPGridRecords* pRecords)
{
	if (!pRecords)
		return FALSE;

	long nSchema = XTP_GRID_CB_RECORDS_DATA_VER;

	PX_Long(pPX, _T("Version"), (long&)nSchema);
	pPX->ExchangeLocale();

	CXTPPropExchangeSection secRecords(pPX->GetSection(_T("GridRecords")));

	int nRecordsCount = (int)pRecords->GetCount();
	CXTPPropExchangeEnumeratorPtr pEnumRecords(secRecords->GetEnumerator(_T("Record")));
	POSITION posRecord = pEnumRecords->GetPosition((DWORD)nRecordsCount);

	for (int i = 0; i < nRecordsCount; i++)
	{
		CXTPPropExchangeSection secRecord(pEnumRecords->GetNext(posRecord));

		CXTPGridRecord* pRecord = pRecords->GetAt(i);
		PX_Object(&secRecord, pRecord, RUNTIME_CLASS(CXTPGridRecord));
	}

	return TRUE;
}

void CXTPGridControl::_SelectChilds(CXTPGridRecords* pRecords)
{
	ASSERT(NULL != pRecords);

	CXTPGridRows* pRows			   = GetRows();
	CXTPGridSelectedRows* pSelRows = GetSelectedRows();

	for (int nRecord = 0; nRecord < pRecords->GetCount(); nRecord++)
	{
		CXTPGridRecord* pRecord = pRecords->GetAt(nRecord);
		ASSERT(NULL != pRecord);

		if (NULL != pRecord)
		{
			CXTPGridRow* pRow = pRows->Find(pRecord);

			if (NULL != pRow)
			{
				pSelRows->Add(pRow);

				// Recursion
				if (pRecord->HasChildren())
				{
					CXTPGridRecords* pChilds = pRecord->GetChilds();
					if (NULL != pChilds)
					{
						_SelectChilds(pChilds);
					}
				}
			}
		}
	}
}

void CXTPGridControl::_SelectRows(CXTPGridRecords* pRecords)
{
	CXTPGridSelectedRows* pSelRows = GetSelectedRows();

	pSelRows->Clear();

	_SelectChilds(pRecords);

	if (pSelRows->IsChanged())
	{
		OnSelectionChanged();
	}
}

BOOL CXTPGridControl::_GetSelectedRows(CXTPGridRecords* pRecords,
									   CXTPInternalCollectionT<CXTPGridRow>* pRows)
{
	ASSERT(!pRecords || pRecords->m_bArray == TRUE);

	CXTPGridSelectedRows* pSelectedRows = GetSelectedRows();
	if (!pSelectedRows || !pSelectedRows->GetCount() || (!pRecords && !pRows))
		return FALSE;

	POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();
	while (pos)
	{
		CXTPGridRow* pRow		= pSelectedRows->GetNextSelectedRow(pos);
		CXTPGridRecord* pRecord = pRow ? pRow->GetRecord() : NULL;
		if (NULL == pRow)
			break;

		if (pRecord && pRecords)
			pRecords->Add(pRecord);

		if (pRow && pRows)
			pRows->AddPtr(pRow, TRUE);
	}

	return TRUE;
}

BOOL CXTPGridControl::_WriteSelectedRowsData(CXTPPropExchange* pPX)
{
	if (!pPX || !pPX->IsStoring())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CXTPGridRecords* pSelectedRecords = new CXTPGridRecords(TRUE);

	if (!_GetSelectedRows(pSelectedRecords) || !_WriteRecordsData(pPX, pSelectedRecords))
	{
		CMDTARGET_RELEASE(pSelectedRecords);
		return FALSE;
	}

	CMDTARGET_RELEASE(pSelectedRecords);
	return TRUE;
}

BOOL CXTPGridControl::_ReadRecordsFromData(CXTPPropExchange* pPX, CXTPGridRecords& rarRecords)
{
	rarRecords.RemoveAll();

	if (!pPX || !pPX->IsLoading())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CXTPPropExchangeSection secRecords(pPX->GetSection(_T("GridRecords")));

	long nSchema = 0;

	PX_Long(pPX, _T("Version"), (long&)nSchema);
	pPX->ExchangeLocale();

	if (nSchema != XTP_GRID_CB_RECORDS_DATA_VER)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CXTPPropExchangeEnumeratorPtr pEnumRecords(secRecords->GetEnumerator(_T("Record")));
	POSITION posRecord = pEnumRecords->GetPosition();

	while (posRecord)
	{
		CXTPPropExchangeSection secRecord(pEnumRecords->GetNext(posRecord));

		CXTPGridRecord* pRecord = NULL;
		PX_Object(&secRecord, pRecord, RUNTIME_CLASS(CXTPGridRecord));

		if (!pRecord)
			AfxThrowArchiveException(CArchiveException::badClass);

		CXTPGridRecord* pRecord2 = pRecord;
		BOOL bCanceled			 = OnBeforePaste(&pRecord2);

		if (bCanceled || pRecord2 != pRecord)
			CMDTARGET_RELEASE(pRecord);

		if (bCanceled)
			continue;

		rarRecords.Add(pRecord2);
	}

	return TRUE;
}

BOOL CXTPGridControl::OnBeforeCopyToText(CXTPGridRecord* pRecord, CStringArray& rarStrings)
{
	XTP_NM_GRID_BEFORE_COPYPASTE nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	CXTPGridRecord* pRecordTmp = pRecord;
	nmParams.ppRecord		   = &pRecordTmp;
	nmParams.parStrings		   = &rarStrings;

	LRESULT lResult = SendNotifyMessage(XTP_NM_GRID_BEFORE_COPY_TOTEXT, (NMHDR*)&nmParams);

	return lResult != 0;
}

BOOL CXTPGridControl::OnBeforePasteFromText(CStringArray& arStrings, CXTPGridRecord** ppRecord)
{
	XTP_NM_GRID_BEFORE_COPYPASTE nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	nmParams.parStrings = &arStrings;
	nmParams.ppRecord   = ppRecord;

	LRESULT lResult = SendNotifyMessage(XTP_NM_GRID_BEFORE_PASTE_FROMTEXT, (NMHDR*)&nmParams);

	return lResult != 0;
}

BOOL CXTPGridControl::OnBeforePaste(CXTPGridRecord** ppRecord)
{
	XTP_NM_GRID_BEFORE_COPYPASTE nmParams;
	::ZeroMemory(&nmParams, sizeof(nmParams));

	nmParams.ppRecord = ppRecord;

	LRESULT lResult = SendNotifyMessage(XTP_NM_GRID_BEFORE_PASTE, (NMHDR*)&nmParams);

	return lResult != 0;
}

int CXTPGridControl::OnGetColumnDataBestFitWidth(CXTPGridColumn* pColumn)
{
	CXTPGridPaintManager* pPaintManager = GetPaintManager();
	if (!pColumn || !pPaintManager)
	{
		ASSERT(FALSE);
		return 0;
	}
	int nDataWidth  = 0;
	int nDataWidth0 = 0;
	int nDataWidth1 = 0;

	CXTPClientRect rcClient(this);
	CBitmap bmp;
	{
		CClientDC dcClient(this);
		bmp.CreateCompatibleBitmap(&dcClient, rcClient.Width(), rcClient.Height());
	}

	CXTPCompatibleDC dc(NULL, &bmp);

	CXTPFontDC autoFont(&dc, pPaintManager->GetTextFont()); // to reset selected font on exit

	if (pColumn->GetBestFitMode() == xtpColumnBestFitModeVisibleData
		|| (pColumn->GetBestFitMode() == xtpColumnBestFitModeAllData && IsVirtualMode())
		/* && pColumn->m_nMaxItemWidth == 0*/)
	{
		// calculate width for visible rows only
		int nVisibleRows = GetGridAreaRows(GetTopRowIndex(), TRUE);

		nDataWidth = OnGetItemsCaptionMaxWidth(&dc, GetRows(), pColumn, GetTopRowIndex(),
											   nVisibleRows);
	}
	else if (pColumn->GetBestFitMode() == xtpColumnBestFitModeAllData && !IsVirtualMode())
		nDataWidth = OnGetItemsCaptionMaxWidth(&dc, GetRows(), pColumn);
	else
		return 0;

	if (m_pSectionHeader->IsVisible() && GetHeaderRows()->GetCount() > 0)
		nDataWidth0 = OnGetItemsCaptionMaxWidth(&dc, GetHeaderRows(), pColumn);

	if (m_pSectionFooter->IsVisible() && GetFooterRows()->GetCount() > 0)
		nDataWidth1 = OnGetItemsCaptionMaxWidth(&dc, GetFooterRows(), pColumn);

	nDataWidth = max(nDataWidth, max(nDataWidth0, nDataWidth1));

	return nDataWidth;
}

int CXTPGridControl::OnGetItemsCaptionMaxWidth(CDC* pDC, CXTPGridRows* pRows,
											   CXTPGridColumn* pColumn, int nStartRow,
											   int nRowsCount)
{
	CXTPGridPaintManager* pPaintManager = GetPaintManager();
	if (!pDC || !pRows || !pColumn || !pPaintManager)
	{
		ASSERT(FALSE);
		return 0;
	}

	XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
	drawArgs.pDC	  = pDC;
	drawArgs.pControl = this;
	drawArgs.pColumn  = pColumn;
	drawArgs.rcItem   = pColumn->GetRect();

	XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS();

	int nGroupIndent = 0;
	if (GetColumns()->GetVisibleAt(0) == pColumn)
		nGroupIndent = GetHeaderIndent();

	CSize sizeBitmap(0, 0);
	int nMaxWidth  = 0;
	int nItemIndex = pColumn->GetItemIndex();

	int nEndRow = pRows->GetCount();
	if (nRowsCount > 0)
		nEndRow = min(pRows->GetCount(), nStartRow + nRowsCount);

	for (int i = max(0, nStartRow); i < nEndRow; i++)
	{
		CXTPGridRow* pRow = pRows->GetAt(i);
		if (!pRow)
			continue;

		if (pRow && pRow->IsGroupRow())
			continue;

		CXTPGridRecord* pRec	  = pRow ? pRow->GetRecord() : NULL;
		CXTPGridRecordItem* pItem = pRec ? pRec->GetItem(nItemIndex) : NULL;

		if (!pItem)
			continue;

		// 1. Calculate Text
		drawArgs.pRow  = pRow;
		drawArgs.pItem = pItem;

		pMetrics->strText = pItem->GetCaption(pColumn);

		pRow->GetItemMetrics(&drawArgs, pMetrics);

		CXTPFontDC fontDC(pDC, pMetrics->pFont);

		int nWidth = 0;

		if (pItem->GetMarkupUIElement())
		{
			nWidth = XTPMarkupMeasureElement(pItem->GetMarkupUIElement(), INT_MAX, 0).cx
					 + XTP_DPI_X(4);
		}
		else
		{
			CString sFirst(pMetrics->strText);
			CString sLast;
			int jPos = sFirst.Find(_T('\n'), 0);
			while (jPos > -1)
			{
				sLast  = sFirst.Mid(jPos + 1);
				sFirst = sFirst.Left(jPos);
				nWidth = max(nWidth, pDC->GetTextExtent(sFirst).cx + XTP_DPI_X(7));
				sFirst = sLast;
				jPos   = sFirst.Find(_T('\n'), 0);
			}
			if (!sFirst.IsEmpty())
				nWidth = max(nWidth, pDC->GetTextExtent(sFirst).cx + XTP_DPI_X(7));
		}

		// 2. Calculate Tree Indent
		if (pColumn->IsTreeColumn())
		{
			int nTreeDepth = pRow->GetTreeDepth() - pRow->GetGroupLevel();
			if (nTreeDepth > 0)
				nTreeDepth++;
			nWidth += GetIndent(nTreeDepth);

			if (sizeBitmap.cx == 0 && sizeBitmap.cy == 0)
			{
				CRect rcBmp(0, 0, XTP_DPI_X(100), XTP_DPI_Y(100));
				sizeBitmap = pPaintManager->DrawCollapsedBitmap(NULL, pRow, rcBmp);
			}

			nWidth += sizeBitmap.cx + XTP_DPI_X(2);
		}

		// 2.1 Calculate Group Indent
		nWidth += nGroupIndent;

		// 3. Calculate item Icon
		int nIcon = pMetrics->nItemIcon;
		nIcon	 = (nIcon != XTP_GRID_NOICON) ? nIcon : pItem->GetIconIndex();
		if (nIcon != XTP_GRID_NOICON)
		{
			CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(XTPToUInt(nIcon), 0);
			if (pIcon)
				nWidth += pIcon->GetWidth() + XTP_DPI_X(4);
		}

		if (pItem->GetHasCheckbox())
		{
			nWidth += XTP_DPI_X(15);
		}

		// 4. Calculate item Controls
		if (pItem->m_pItemControls)
		{
			int nControlsCount = pItem->m_pItemControls->GetSize();
			for (int k = 0; k < nControlsCount; k++)
			{
				CXTPGridRecordItemControl* pCtrl = pItem->m_pItemControls->GetAt(k);
				int nControlWidth				 = 0;
				if (pCtrl)
					nControlWidth = pCtrl->GetSize().cx;
				if (nControlWidth < 0)
					nControlWidth = XTP_DPI_X(20);

				nWidth += nControlWidth;
			}
		}

		nMaxWidth = max(nMaxWidth, nWidth);
	}

	CMDTARGET_RELEASE(pMetrics);

	return nMaxWidth;
}

BOOL CXTPGridControl::IsEditMode()
{
	CXTPGridInplaceEdit* pEdit = GetInplaceEdit();

	BOOL bEditMode = pEdit && pEdit->GetSafeHwnd() && pEdit->IsWindowVisible();
	bEditMode |= GetInplaceList() && GetInplaceList()->GetSafeHwnd()
				 && GetInplaceList()->IsWindowVisible();

	return bEditMode;
}

int CXTPGridControl::GetRowsHeight(CXTPGridRows* pRows, int nTotalWidth, int nMaxHeight)
{
	int nRowsHeight = 0;
	pRows->ClearMergeHeight();
	CWindowDC dc(this);

	for (int i = 0; i < pRows->GetCount(); ++i)
	{
		nRowsHeight += GetPaintManager()->GetRowHeight(&dc, pRows->GetAt(i), nTotalWidth);

		if (nMaxHeight >= 0 && nRowsHeight > nMaxHeight)
			return nRowsHeight;
	}

	return nRowsHeight;
}

void CXTPGridControl::DrawDefaultGrid(CDC* pDC, CRect rcClient, int nRowHeight, int nLeftOffset)
{
	if (nRowHeight <= 0)
		return;

	int nFreezeCols = GetFreezeColumnsCount();
	CRect rcClipBox = GetGridRectangle();

	CRect rcRow;
	rcRow = rcClient;
	rcRow.left -= nLeftOffset;
	rcRow.right -= nLeftOffset;
	rcRow.bottom = rcClient.top + nRowHeight;

	int nIndentWidth					= GetHeaderIndent();
	CXTPGridPaintManager* pPaintManager = GetPaintManager();

	CXTPGridColumns arrVisibleColumns(this);
	GetColumns()->GetVisibleColumns(arrVisibleColumns);
	int nVisColCount = arrVisibleColumns.GetCount();
	nFreezeCols		 = min(nFreezeCols, nVisColCount);

	// fill the empty space
	while (rcRow.top < rcClient.bottom)
	{
		CRect rcItem(rcRow.left, rcRow.top, rcRow.right, rcRow.bottom);

		int xMinCol_0 = rcRow.left + nIndentWidth;

		for (int nColumn = nVisColCount - 1; nColumn >= 0; nColumn--)
		{
			BOOL bFreezeCol = nColumn < nFreezeCols;
			int nColIdx		= bFreezeCol ? nFreezeCols - 1 - nColumn : nColumn;

			CXTPGridColumn* pColumn = arrVisibleColumns.GetAt(nColIdx);
			ASSERT(pColumn && pColumn->IsVisible());

			if (pColumn)
			{
				rcItem.left = pColumn->GetRect().left;
				if (nColIdx == 0)
					rcItem.left = max(xMinCol_0, rcItem.left);

				rcItem.right = pColumn->GetRect().right;

				if (!CRect().IntersectRect(rcClipBox, rcItem))
					continue;

				if (bFreezeCol)
				{
					CRect rcFreeze(rcItem);
					rcFreeze.top += XTP_DPI_Y(1);
					pDC->FillSolidRect(rcFreeze, pPaintManager->GetControlBackColor(this));
				}

				CRect rcGridItem(rcItem);
				rcGridItem.left--;

				pPaintManager->DrawGrid(pDC, xtpGridOrientationHorizontal, rcGridItem);
				pPaintManager->DrawGrid(pDC, xtpGridOrientationVertical, rcGridItem);

				if (nColIdx == nFreezeCols - 1)
					pPaintManager->DrawFreezeColsDivider(pDC, rcGridItem, this);
			}
		}

		if (nIndentWidth > 0) // draw indent column
		{
			CRect rcIndent(rcRow);
			if (nFreezeCols > 0)
			{
				rcIndent.left += nLeftOffset;
			}
			rcIndent.right = rcIndent.left + nIndentWidth;

			pPaintManager->FillIndent(pDC, rcIndent);
		}

		rcRow.top += nRowHeight;
		rcRow.bottom += nRowHeight;
	}
}

BOOL CXTPGridControl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (!OnPreviewKeyDown((UINT&)pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)))
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CXTPGridControl::OnConstraintSelecting(CXTPGridRow* pRow, CXTPGridRecordItem* pItem,
											CXTPGridColumn* pColumn,
											CXTPGridRecordItemConstraint* pConstraint)
{
	XTP_NM_GRIDCONSTRAINTSELECTING nmConstraint;
	::ZeroMemory(&nmConstraint, sizeof(nmConstraint));

	nmConstraint.pRow		 = pRow;
	nmConstraint.pColumn	 = pColumn;
	nmConstraint.pItem		 = pItem;
	nmConstraint.pConstraint = pConstraint;

	LRESULT lResult = SendNotifyMessage(XTP_NM_GRID_CONSTRAINT_SELECTING, (NMHDR*)&nmConstraint);

	return lResult != 0;
}

const XTP_NM_GRIDTOOLTIPINFO& CXTPGridControl::OnGetToolTipInfo(CXTPGridRow* pRow,
																CXTPGridRecordItem* pItem,
																CString& rstrToolTipText)
{
	::ZeroMemory(m_pCachedToolTipInfo, sizeof(XTP_NM_GRIDTOOLTIPINFO));

	m_pCachedToolTipInfo->pRow	 = pRow;
	m_pCachedToolTipInfo->pItem	= pItem;
	m_pCachedToolTipInfo->pstrText = &rstrToolTipText;

	SendNotifyMessage(XTP_NM_GRID_GETTOOLTIPINFO, (NMHDR*)m_pCachedToolTipInfo);

	return *m_pCachedToolTipInfo;
}

CXTPGridDataManager* CXTPGridControl::GetDataManager()
{
	if (!m_pDataManager)
		m_pDataManager = new CXTPGridDataManager(this);

	return m_pDataManager;
}

CRect CXTPGridControl::GetElementRect(XTPGridElementRect nElement) const
{
	CRect rcElement;
	switch (nElement)
	{
		case xtpGridElementRectGridArea: rcElement = m_pSectionBody->m_rcSection; break;
		case xtpGridElementRectGroupByArea: rcElement = m_rcGroupByArea; break;
		case xtpGridElementRectHeaderArea: rcElement = m_rcHeaderArea; break;
		case xtpGridElementRectFooterArea: rcElement = m_rcFooterArea; break;
		case xtpGridElementRectHeaderRecordsArea: rcElement = m_pSectionHeader->m_rcSection; break;
		case xtpGridElementRectFooterRecordsArea: rcElement = m_pSectionFooter->m_rcSection; break;
		case xtpGridElementRectHeaderRecordsDividerArea:
			rcElement = m_pSectionHeader->GetDividerRect();
			break;
		case xtpGridElementRectFooterRecordsDividerArea:
			rcElement = m_pSectionFooter->GetDividerRect();
			break;
		default: ASSERT(!"Unexpected grid element specified"); break;
	}

	return rcElement;
}

CRect CXTPGridControl::GetElementRect(int nElement) const
{
	return GetElementRect(static_cast<XTPGridElementRect>(nElement));
}

CXTPMarkupContext* CXTPGridControl::GetMarkupContext() const
{
	return m_pMarkupContext;
}

void CXTPGridControl::EnableMarkup(BOOL bEnable)
{
	if (m_bMarkupEnabled == bEnable)
		return;

	m_bMarkupEnabled = bEnable;

	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd);
	}

	m_pSections->SetMarkupContext(m_pMarkupContext);

	// Reset Markup UI Elements
	CXTPGridColumns* pColumns = GetColumns();
	int nColumnsCount		  = pColumns->GetCount();
	for (int nColumn = 0; nColumn < nColumnsCount; nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn)
			pColumn->ResetMarkupUIElement();
	}

	for (int nSection = 0; nSection < m_pSections->GetSize(); nSection++)
	{
		CXTPGridSection* pSection = m_pSections->GetAt(nSection);
		if (pSection == NULL)
			continue;

		CXTPGridRows* pRows = pSection->GetRows();
		if (pRows)
		{
			for (int nRow = 0; nRow < pRows->GetCount(); nRow++)
			{
				CXTPGridRow* pRow = pRows->GetAt(nRow);
				if (pRow && pRow->IsGroupRow())
				{
					((CXTPGridGroupRow*)pRow)->ResetMarkupUIElement();
				}
			}
		}

		CXTPGridRecords* pRecords = pSection->GetRecords();
		if (pRecords)
		{
			for (int nRecord = 0; nRecord < pRecords->GetCount(); nRecord++)
			{
				CXTPGridRecord* pRecord = pRecords->GetAt(nRecord);
				if (pRecord == NULL)
					continue;

				int nItemsCount = pRecord->GetItemCount();
				for (int nItem = 0; nItem < nItemsCount; nItem++)
				{
					CXTPGridRecordItem* pRecordItem = pRecord->GetItem(nItem);
					if (pRecordItem)
						pRecordItem->ResetMarkupUIElement();
				}
			}
		}
	} // for (sections)

	AdjustScrollBars();
}

LRESULT CXTPGridControl::OnGetObject(WPARAM wParam, LPARAM lParam)
{
	if (((LONG)lParam) != OBJID_CLIENT)
		return (LRESULT)Default();

	return LresultFromProxyObject(wParam);
}

BEGIN_INTERFACE_MAP(CXTPGridControl, CWnd)
	INTERFACE_PART(CXTPGridControl, IID_IAccessible, ExternalAccessible)
	XTP_DECLARE_CMDTARGETPROVIDER_INTERFACE_PART(CXTPGridControl)
END_INTERFACE_MAP()

XTP_IMPLEMENT_CMDTARGETPROVIDER_INTERFACE(CXTPGridControl)

CCmdTarget* CXTPGridControl::GetAccessible()
{
	return this;
}

HRESULT CXTPGridControl::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	if (NULL == ppdispParent)
		return E_INVALIDARG;

	*ppdispParent = NULL;

	if (GetSafeHwnd())
	{
		return AccessibleObjectFromWindow(GetSafeHwnd(), OBJID_WINDOW, IID_IDispatch,
										  (void**)ppdispParent);
	}
	return E_FAIL;
}

HRESULT CXTPGridControl::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (NULL == pChildCount)
		return E_INVALIDARG;

	*pChildCount = 1 + GetRows()->GetCount();
	return S_OK;
}

HRESULT CXTPGridControl::GetAccessibleChild(VARIANT varChild, IDispatch* FAR* ppdispChild)
{
	if (NULL == ppdispChild)
		return E_INVALIDARG;

	*ppdispChild = NULL;
	int nChild   = GetChildIndex(&varChild);

	if (nChild <= 0)
	{
		return E_INVALIDARG;
	}

	if (nChild == 1)
	{
		*ppdispChild = GetGridHeader()->CreateAccProxy();
		return S_OK;
	}

	CXTPGridRow* pRow = GetRows()->GetAt(nChild - 2);
	if (!pRow)
	{
		return E_INVALIDARG;
	}

	*ppdispChild = pRow->CreateAccProxy();
	return S_OK;
}

HRESULT CXTPGridControl::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	if (NULL == pszName)
		return E_INVALIDARG;

	int nChild = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF || nChild == -1)
	{
		CString strCaption;
		GetWindowText(strCaption);
		if (strCaption.IsEmpty())
			strCaption = _T("Grid");
		*pszName = strCaption.AllocSysString();
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPGridControl::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	if (NULL == pvarRole)
		return E_INVALIDARG;

	pvarRole->vt = VT_EMPTY;
	int nChild   = GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		pvarRole->vt   = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_TABLE;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPGridControl::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	if (NULL == pvarState)
		return E_INVALIDARG;

	pvarState->vt   = VT_I4;
	pvarState->lVal = 0;
	int nChild		= GetChildIndex(&varChild);

	if (nChild == CHILDID_SELF)
	{
		pvarState->lVal = STATE_SYSTEM_FOCUSABLE;
	}

	return S_OK;
}

HRESULT CXTPGridControl::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth,
											long* pcyHeight, VARIANT varChild)
{
	if (NULL == pxLeft || NULL == pyTop || NULL == pcxWidth || NULL == pcyHeight)
		return E_INVALIDARG;

	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (!GetSafeHwnd())
		return S_OK;

	CRect rc;
	GetWindowRect(&rc);

	int nChild = GetChildIndex(&varChild);

	if (nChild == 1)
	{
		rc = m_rcHeaderArea;
		ClientToScreen(&rc);
	}

	if (nChild > 1)
	{
		CXTPGridRow* pRow = GetRows()->GetAt(nChild - 2);
		if (pRow)
		{
			rc = pRow->GetRect();
			ClientToScreen(&rc);
		}
	}

	*pxLeft	= rc.left;
	*pyTop	 = rc.top;
	*pcxWidth  = rc.Width();
	*pcyHeight = rc.Height();

	return S_OK;
}

HRESULT CXTPGridControl::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (NULL == pvarID)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(this).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt   = VT_I4;
	pvarID->lVal = 0;

	CPoint pt(xLeft, yTop);
	ScreenToClient(&pt);

	if (m_rcHeaderArea.PtInRect(pt))
	{
		pvarID->vt		 = VT_DISPATCH;
		pvarID->pdispVal = GetGridHeader()->CreateAccProxy();
		return S_OK;
	}

	CXTPGridRow* pRow = HitTest(pt);
	if (pRow)
	{
		pvarID->vt		 = VT_DISPATCH;
		pvarID->pdispVal = pRow->CreateAccProxy();
		return S_OK;
	}

	return S_FALSE;
}

void CXTPGridControl::EnsureStopLastRqstTimer()
{
	if (m_uRqstEditTimer)
	{
		KillTimer(m_uRqstEditTimer);

		m_uRqstEditTimer   = 0;
		m_iLastRqstEditRow = -1;
		m_iLastRqstEditCol = -1;
	}
}

// Information for delay editing.
void CXTPGridControl::EnsureStopDelayEditTimer()
{
	if (m_uiDelayEditTimer)
	{
		KillTimer(m_uiDelayEditTimer);

		m_uiDelayEditTimer = NULL;
	}
}

void CXTPGridControl::StartLastRqstTimer()
{
	EnsureStopLastRqstTimer();

	m_uRqstEditTimer = SetTimer(XTP_GRID_DELAY_CLICK_TIMER_ID, 3000,
								NULL); // After 3 seconds, ignore this click.
}

// Information for delay editing.
void CXTPGridControl::StartDelayEditTimer()
{
	EnsureStopDelayEditTimer();

	m_uiDelayEditTimer = SetTimer(XTP_GRID_DELAYEDIT_CLICK_TIMER_ID, m_uiDelayEditMaxTime,
								  NULL); // Begin edit after a full double click is possible.
}

void CXTPGridControl::SetSelectionState(int index, int state)
{
	CXTPGridSelectedRows* pSelectedRows = GetSelectedRows();
	if (!pSelectedRows)
		return;
	CXTPGridRecordItem* pItem = NULL;
	CXTPGridRecord* pRec	  = NULL;
	for (int i = 0; i < pSelectedRows->GetCount(); i++)
	{
		if (pSelectedRows->GetAt(i))
		{
			pRec = pSelectedRows->GetAt(i)->GetRecord();
			if (pRec)
			{
				if (index < pRec->GetItemCount())
				{
					pItem = pRec->GetItem(index);
					if (pItem)
					{
						pItem->SetChecked(state == 1);
						pItem->SetCheckedState(state);
					}
				}
			}
		}
	}
	RedrawControl();
}

void CXTPGridControl::Recalc(BOOL bAll)
{
	EditItem(NULL);

	if (!bAll && GetFocusedRow())
	{
		CXTPGridRecord* pRec = GetFocusedRow()->GetRecord();
		if (pRec)
		{
			int N					  = pRec->GetItemCount();
			CXTPGridRecordItem* pItem = NULL;
			for (int i = 0; i < N; i++)
			{
				pItem = pRec->GetItem(i);
				if (pItem)
					pItem->UpdateValue();
			}
			RedrawControl();
		}
	}
	else if (GetRecords())
	{
		int n = GetRecords()->GetCount();
		for (int j = 0; j < n; j++)
		{
			CXTPGridRecord* pRec = GetRecords()->GetAt(j);
			if (pRec)
			{
				int N					  = pRec->GetItemCount();
				CXTPGridRecordItem* pItem = NULL;
				for (int i = 0; i < N; i++)
				{
					pItem = pRec->GetItem(i);
					if (pItem)
						pItem->UpdateValue();
				}
				RedrawControl();
			}
		}
		int N = GetRows()->GetCount();
		for (int I = 0; I < N; I++)
		{
			CXTPGridRow* pTRow = GetRows()->GetAt(I);
			if (pTRow && pTRow->IsGroupRow())
			{
				CXTPGridGroupRow* pTgRow = (CXTPGridGroupRow*)pTRow;
				pTgRow->UpdateValue();
			}
		}
		RedrawControl();
	}
}

BOOL CXTPGridControl::IsShowRowNumber()
{
	if (m_iColumnForNum > -1)
	{
		if (m_bUseIconColumnForNum)
		{
			CXTPGridColumn* pIconCol = GetColumns()->Find(m_iColumnForNum);
			if (pIconCol)
				return pIconCol->IsVisible();
		}
	}
	return FALSE;
}

void CXTPGridControl::ShowRowNumber(BOOL bSet)
{
	if (bSet)
	{
		if (m_iColumnForNumPrev > -1)
			m_iColumnForNum = m_iColumnForNumPrev;
		if (m_iColumnForNum > -1)
		{
			if (m_bUseIconColumnForNum)
			{
				CXTPGridColumn* pIconCol = GetColumns()->Find(m_iColumnForNum);
				if (pIconCol)
					pIconCol->SetVisible(TRUE);
			}
		}
	}
	else
	{
		m_iColumnForNumPrev = m_iColumnForNum;
		if (m_iColumnForNum > -1)
		{
			if (m_bUseIconColumnForNum && !m_bIconView)
			{
				CXTPGridColumn* pIconCol = GetColumns()->Find(m_iColumnForNum);
				if (pIconCol)
					pIconCol->SetVisible(FALSE);
			}
		}
		m_iColumnForNum = -1;
	}
	RedrawControl();
}

int CXTPGridControl::GetFreezeColumnsCount() const
{
	int nLastIndex = -1;
	return GetFreezeColumnsCount(nLastIndex);
}

int CXTPGridControl::GetFreezeColumnsCount(int& nLastIndex) const
{
	int nCount = 0;
	nLastIndex = -1;

	const CXTPGridColumns* pColumns = GetColumns();
	int nColumnsCount				= pColumns->GetCount();

	for (int nColumn = 0; nColumn < nColumnsCount; nColumn++)
	{
		const CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		ASSERT(NULL != pColumn);

		if (pColumn->IsVisible())
		{
			if (pColumn->IsFrozen())
			{
				nLastIndex = nColumn;
				++nCount;
			}
			else
			{
				break;
			}
		}
	}

	return nCount;
}

int CXTPGridControl::GetFreezeColumnsIndex() const
{
	int nIndex						= -1;
	const CXTPGridColumns* pColumns = GetColumns();
	int nColumnsCount				= pColumns->GetCount();

	for (int nColumn = 0; nColumn < nColumnsCount; nColumn++)
	{
		const CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		ASSERT(NULL != pColumn);

		if (pColumn->IsVisible())
		{
			if (pColumn->IsFrozen())
			{
				nIndex = nColumn;
			}
			else
				break;
		}
	}

	return nIndex;
}

void CXTPGridControl::SetFreezeColumnsCount(int nCount, BOOL bReserved)
{
	UNREFERENCED_PARAMETER(bReserved);

	for (int nColumn = 0; nColumn < GetColumns()->GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = GetColumns()->GetAt(nColumn);

		if (NULL != pColumn)
		{
			pColumn->SetFrozen(nColumn < nCount);
		}
	}

	RedrawControl();
}

void CXTPGridControl::SetDisableReorderColumnsCount(int nCount)
{
	if (nCount >= 0 && nCount < m_pColumns->GetCount())
		m_nDisableReorderColumnsCount = nCount;
	else
		m_nDisableReorderColumnsCount = 0;
}

CXTPGridRecordItem* CXTPGridControl::GetFocusedRecordItem() const
{
	CXTPGridColumn* pCol = GetFocusedColumn();
	CXTPGridRow* pRow	= GetFocusedRow();

	if (pCol && pRow && pRow->GetRecord())
		return pRow->GetRecord()->GetItem(pCol);

	return NULL;
}

BOOL CXTPGridControl::SetCellText(int row, int col, CString sText)
{
	CXTPGridRow* pRow = GetRows()->GetAt(row);
	if (pRow && pRow->GetRecord())
	{
		CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(col);
		if (pItem)
		{
			if (pItem->IsKindOf(RUNTIME_CLASS(CXTPGridRecordItemText)))
				((CXTPGridRecordItemText*)pItem)->SetValue(sText);
			pItem->SetCaption(sText);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXTPGridControl::SetCellFormula(int row, int col, CString sFormula)
{
	CXTPGridRow* pRow = GetRows()->GetAt(row);
	if (pRow && pRow->GetRecord())
	{
		CXTPGridRecordItem* pItem = pRow->GetRecord()->GetItem(col);
		if (pItem)
		{
			pItem->SetFormula(sFormula); // e.g. "SUM(R0*C2:R1*C5)"
			pItem->SetFormatString(_T("%d"));
			pItem->UpdateValue();
			pItem->SetEditable(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CXTPGridControl::UnselectGroupRows()
{
	if (IsSkipGroupsFocusEnabled() && !IsVirtualMode())
	{
		for (int II = 0; II < GetRows()->GetCount(); II++)
		{
			CXTPGridRow* pRow = GetRows()->GetAt(II);
			if (pRow)
			{
				if (pRow->IsGroupRow())
					pRow->SetSelected(FALSE);
			}
		}
	}
}

BOOL CXTPGridControl::IsFullColumnScrolling() const
{
	if (GetGridHeader() && GetGridHeader()->IsAutoColumnSizing())
	{
		return FALSE;
	}
	else
	{
		return (xtpGridScrollModeBlock == m_scrollModeH);
	}
}

//----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CXTPPromptDlg, CDialog)

CXTPPromptDlg::CXTPPromptDlg(CWnd* pParent)
	: CDialog()
{
	UNREFERENCED_PARAMETER(pParent);
}

CXTPPromptDlg::~CXTPPromptDlg()
{
}

INT_PTR CXTPPromptDlg::DoModal()
{
	BYTE DlgTempl[] = { 0X01, 0X00, 0XFF, 0XFF, 0X00, 0X00, 0X00, 0X00, 0X80, 0X00, 0X00, 0X00,
						0X48, 0X00, 0XC8, 0X80, 0X02, 0X00, 0X00, 0X00, 0X00, 0X00, 0X06, 0X01,
						0X19, 0X00, 0X00, 0X00, 0X00, 0X00, 0X41, 0X00, 0X73, 0X00, 0X73, 0X00,
						0X69, 0X00, 0X67, 0X00, 0X6E, 0X00, 0X20, 0X00, 0X4D, 0X00, 0X61, 0X00,
						0X72, 0X00, 0X6B, 0X00, 0X65, 0X00, 0X72, 0X00, 0X20, 0X00, 0X4E, 0X00,
						0X61, 0X00, 0X6D, 0X00, 0X65, 0X00, 0X00, 0X00, 0X08, 0X00, 0X90, 0X01,
						0X00, 0X01, 0X4D, 0X00, 0X53, 0X00, 0X20, 0X00, 0X53, 0X00, 0X68, 0X00,
						0X65, 0X00, 0X6C, 0X00, 0X6C, 0X00, 0X20, 0X00, 0X44, 0X00, 0X6C, 0X00,
						0X67, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
						0X80, 0X10, 0X81, 0X50, 0X02, 0X00, 0X02, 0X00, 0XCA, 0X00, 0X15, 0X00,
						0XF1, 0X03, 0X00, 0X00, 0XFF, 0XFF, 0X81, 0X00, 0X00, 0X00, 0X00, 0X00,
						0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X00, 0X01, 0X50,
						0XD0, 0X00, 0X04, 0X00, 0X32, 0X00, 0X0E, 0X00, 0X01, 0X00, 0X00, 0X00,
						0XFF, 0XFF, 0X80, 0X00, 0X4F, 0X00, 0X4B, 0X00, 0X00, 0X00, 0X00, 0X00 };
	InitModalIndirect((LPCDLGTEMPLATE)&DlgTempl, NULL);
	return CDialog::DoModal();
}

void CXTPPromptDlg::OnOK()
{
	if (GetDlgItem(1009))
		GetDlgItem(1009)->GetWindowText(m_sName);
	CDialog::OnOK();
}

BOOL CXTPPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (GetDlgItem(1009))
		GetDlgItem(1009)->SetWindowText(m_sName);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

XTPGridScrollMode CXTPGridControl::GetScrollMode(XTPGridOrientation orientation) const
{
	XTPGridScrollMode scrollMode;

	switch (orientation)
	{
		case xtpGridOrientationHorizontal: scrollMode = m_scrollModeH; break;
		case xtpGridOrientationVertical: scrollMode = m_scrollModeV; break;
		default: scrollMode = xtpGridScrollModeNone; break;
	}

	return scrollMode;
}

void CXTPGridControl::SetScrollMode(XTPGridOrientation orientation, XTPGridScrollMode scrollMode)
{
	switch (orientation)
	{
		case xtpGridOrientationHorizontal: m_scrollModeH = scrollMode; break;
		case xtpGridOrientationVertical: m_scrollModeV = scrollMode; break;

		case xtpGridOrientationAll:
			m_scrollModeH = scrollMode;
			m_scrollModeV = scrollMode;
			break;
	}

	AdjustScrollBars();
}

void CXTPGridControl::SetGridStyle(BOOL bVertical, XTPGridGridStyle gridStyle)
{
	m_pPaintManager->SetGridStyle(bVertical, gridStyle);
	AdjustScrollBars();
}

XTPGridGridStyle CXTPGridControl::GetGridStyle(BOOL bVertical) const
{
	return m_pPaintManager->GetGridStyle(bVertical);
}

void CXTPGridControl::SetGridStyle(XTPGridOrientation orientation, XTPGridGridStyle gridStyle)
{
	switch (orientation)
	{
		case xtpGridOrientationHorizontal: SetGridStyle(FALSE, gridStyle); break;
		case xtpGridOrientationVertical: SetGridStyle(TRUE, gridStyle); break;
		case xtpGridOrientationAll:
			m_pPaintManager->SetGridStyle(FALSE, gridStyle);
			m_pPaintManager->SetGridStyle(TRUE, gridStyle);
			AdjustScrollBars();
			break;
		default: ASSERT(!"Unknown orientation specified"); break;
	}
}

XTPGridGridStyle CXTPGridControl::GetGridStyle(XTPGridOrientation orientation) const
{
	switch (orientation)
	{
		case xtpGridOrientationHorizontal: return GetGridStyle(FALSE);
		case xtpGridOrientationVertical: return GetGridStyle(TRUE);
		default:
			ASSERT(!"Orientation must be one of: xtpGridOrientationVertical, "
					"xtpGridOrientationHorizontal");
			return xtpGridLineStyleNone;
	}
}

COLORREF CXTPGridControl::SetGridColor(COLORREF clrGridLine)
{
	return m_pPaintManager->SetGridColor(clrGridLine);
}

void CXTPGridControl::EnablePreviewMode(BOOL bIsPreviewMode)
{
	m_pPaintManager->EnablePreviewMode(bIsPreviewMode);
}

BOOL CXTPGridControl::IsPreviewMode() const
{
	return m_pPaintManager->IsPreviewMode();
}

void CXTPGridControl::ShadeGroupHeadings(BOOL bEnable)
{
	if (m_pPaintManager)
		m_pPaintManager->m_bShadeGroupHeadings = bEnable;
	AdjustScrollBars();
}

BOOL CXTPGridControl::IsShadeGroupHeadingsEnabled() const
{
	return m_pPaintManager ? m_pPaintManager->m_bShadeGroupHeadings : FALSE;
}

void CXTPGridControl::SetGroupRowsBold(BOOL bBold)
{
	if (m_pPaintManager)
		m_pPaintManager->m_bGroupRowTextBold = bBold;
}

BOOL CXTPGridControl::IsGroupRowsBold() const
{
	return m_pPaintManager ? m_pPaintManager->m_bGroupRowTextBold : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////

void CXTPGridControl::InitialSelectionEnable(BOOL bEnable)
{
	m_bInitialSelectionEnable = bEnable;

	if (!m_bInitialSelectionEnable)
		m_pSelectedRows->Clear();
}

CXTPGridSections* CXTPGridControl::GetSections() const
{
	return m_pSections;
}

void CXTPGridControl::SetTheme(XTPGridPaintTheme paintTheme, BOOL bEnableMetrics /*=FALSE*/)
{
	m_themeCurrent  = paintTheme;
	m_bThemeMetrics = bEnableMetrics;

	switch (m_themeCurrent)
	{
		case xtpGridThemeOfficeXP: SetPaintManager(new CXTPGridThemeOfficeXP()); break;
		case xtpGridThemeOffice2003: SetPaintManager(new CXTPGridThemeOffice2003()); break;
		case xtpGridThemeOffice2003Luna: SetPaintManager(new CXTPGridThemeOffice2003Luna()); break;
		case xtpGridThemeOffice2013: SetPaintManager(new CXTPGridThemeOffice2013()); break;
		case xtpGridThemeVisualStudio2012Light:
			SetPaintManager(new CXTPGridThemeVisualStudio2012Light());
			break;
		case xtpGridThemeVisualStudio2012Dark:
			SetPaintManager(new CXTPGridThemeVisualStudio2012Dark());
			break;
		case xtpGridThemeVisualStudio2012Blue:
			SetPaintManager(new CXTPGridThemeVisualStudio2012Blue());
			break;
		case xtpGridThemeVisualStudio2015:
			SetPaintManager(new CXTPGridThemeVisualStudio2015());
			break;
		case xtpGridThemeVisualStudio2017:
			SetPaintManager(new CXTPGridThemeVisualStudio2017());
			break;
		case xtpGridThemeVisualStudio2019:
			SetPaintManager(new CXTPGridThemeVisualStudio2019());
			break;
		case xtpGridThemeVisualStudio2022:
			SetPaintManager(new CXTPGridThemeVisualStudio2022());
			break;
		case xtpGridThemeNativeWindows10:
			SetPaintManager(new CXTPGridThemeNativeWindows10());
			break;
		case xtpGridThemeExplorer: SetPaintManager(new CXTPGridThemeExplorer()); break;
		case xtpGridThemeResource: SetPaintManager(new CXTPGridThemeResource()); break;
		default: SetPaintManager(new CXTPGridThemeDefault()); break;
	}
}

BOOL CXTPGridControl::IsCellMergingAllowed() const
{
	return GetPaintManager() && GetPaintManager()->m_bAllowMergeCells;
}

void CXTPGridControl::SetDragSensitivityX(int nDragSensitivityX)
{
	if (XTP_ASSERT_CHECK(1 <= nDragSensitivityX && nDragSensitivityX <= 10000))
	{
		m_nDragSensitivityX = nDragSensitivityX;
	}
}

void CXTPGridControl::SetDragSensitivityY(int nDragSensitivityY)
{
	if (XTP_ASSERT_CHECK(1 <= nDragSensitivityY && nDragSensitivityY <= 10000))
	{
		m_nDragSensitivityY = nDragSensitivityY;
	}
}
