// XTPGridColumn.cpp : implementation of the CXTPGridColumn class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridSubListControl.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridHeader.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPGridColumn, CXTPCmdTarget);

CXTPGridColumnDisplayOptions::CXTPGridColumnDisplayOptions()
{
	m_pColumn		= new CXTPGridColumnDisplaySettings();
	m_pFieldChooser = new CXTPGridColumnDisplaySettings();
	m_pGroupBy		= new CXTPGridColumnDisplaySettings();
}

CXTPGridColumnDisplayOptions::~CXTPGridColumnDisplayOptions()
{
	CMDTARGET_RELEASE(m_pColumn);
	CMDTARGET_RELEASE(m_pFieldChooser);
	CMDTARGET_RELEASE(m_pGroupBy);
}

CXTPGridColumnDisplaySettings* CXTPGridColumnDisplayOptions::Column() const
{
	return m_pColumn;
}

CXTPGridColumnDisplaySettings* CXTPGridColumnDisplayOptions::FieldChooser() const
{
	return m_pFieldChooser;
}

CXTPGridColumnDisplaySettings* CXTPGridColumnDisplayOptions::GroupBy() const
{
	return m_pGroupBy;
}

//////////////////////////////////////////////////////////////////////////
//
CXTPGridColumnDisplaySettings::CXTPGridColumnDisplaySettings()
	: m_bShowIcon(FALSE)
	, m_bShowText(FALSE)
{
}

BOOL CXTPGridColumnDisplaySettings::IsShowIcon() const
{
	return m_bShowIcon;
}

void CXTPGridColumnDisplaySettings::SetShowIcon(BOOL bShowIcon)
{
	m_bShowIcon = bShowIcon;
}

BOOL CXTPGridColumnDisplaySettings::IsShowText() const
{
	return m_bShowText;
}

void CXTPGridColumnDisplaySettings::SetShowText(BOOL bShowText)
{
	m_bShowText = bShowText;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridColumn

CXTPGridColumn::CXTPGridColumn(int nItemIndex, LPCTSTR strDisplayName, int nWidth, BOOL bAutoSize,
							   int nIconID, BOOL bSortable, BOOL bVisible)
{
	_initData(nItemIndex, strDisplayName, _T(""), nWidth, bAutoSize, nIconID, bSortable, bVisible);
}

CXTPGridColumn::CXTPGridColumn(int nItemIndex, LPCTSTR strDisplayName, LPCTSTR strInternalName,
							   int nWidth, BOOL bAutoSize, int nIconID, BOOL bSortable,
							   BOOL bVisible)
{
	_initData(nItemIndex, strDisplayName, strInternalName, nWidth, bAutoSize, nIconID, bSortable,
			  bVisible);
}

void CXTPGridColumn::_initData(int nItemIndex, LPCTSTR strDisplayName, LPCTSTR strInternalName,
							   int nWidth, BOOL bAutoSize, int nIconID, BOOL bSortable,
							   BOOL bVisible)
{
	m_nItemIndex	  = nItemIndex;
	m_strInternalName = strInternalName;
	m_strName		  = strDisplayName;

	m_nIconID   = nIconID;
	m_nMinWidth = 10;
	m_nMaxWidth = 0; // unrestricted

	m_bVisible			   = bVisible;
	m_bSortable			   = bSortable;
	m_bGroupable		   = TRUE;
	m_bFixed			   = FALSE;
	m_bFrozen			   = FALSE;
	m_bFilterable		   = TRUE;
	m_bAllowRemove		   = TRUE;
	m_bAllowDrag		   = TRUE;
	m_bAutoSize			   = bAutoSize;
	m_bAutoSortWhenGrouped = TRUE;

	m_nOldVisibleIndex = -1;

	m_bIsResizable = bAutoSize;
	m_rcColumn.SetRectEmpty();

	m_pColumns		  = NULL;
	m_bSortIncreasing = TRUE;

	m_nMaxItemWidth	= 0;
	m_nAlignment	   = DT_LEFT;
	m_nHeaderAlignment = -1;
	m_nFooterAlignment = -1;

	m_bDrawFooterDivider = TRUE;
	m_bDrawHeaderDivider = TRUE;

	m_bAutoNumbering	 = FALSE;
	m_nAutoNumberingBase = 1;

	m_nColumnStaticWidth = m_nColumnAutoWidth = nWidth;

	m_bShowInFieldChooser = TRUE;
	m_bEditable			  = TRUE;

	m_bDrawHeaderRowsVGrid = TRUE;
	m_bDrawFooterRowsVGrid = TRUE;

	m_nBestFitMode = xtpColumnBestFitModeVisibleData;

	m_pEditOptions = new CXTPGridRecordItemEditOptions();

	m_pDisplayOptions = new CXTPGridColumnDisplayOptions();
	m_pDisplayOptions->Column()->SetShowIcon(TRUE);
	m_pDisplayOptions->Column()->SetShowText(TRUE);
	m_pDisplayOptions->FieldChooser()->SetShowText(TRUE);
	m_pDisplayOptions->GroupBy()->SetShowText(TRUE);

	m_pMarkupUIElement = NULL;

	m_bPlusMinus	   = FALSE;
	m_bExpanded		   = FALSE;
	m_nNextVisualBlock = 0;

	m_chPrompt = _T('\0');
	m_bUseMask = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridColumn::~CXTPGridColumn()
{
	CMDTARGET_RELEASE(m_pEditOptions);
	CMDTARGET_RELEASE(m_pDisplayOptions);

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPGridColumn::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);
	if (m_strName == strCaption)
		return;

	m_strName = strCaption;
	if (m_pColumns)
		m_pColumns->GetGridHeader()->OnColumnsChanged(xtpGridColumnCaptionChanged, this);

	ResetMarkupUIElement();
}

void CXTPGridColumn::ResetMarkupUIElement()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);

	CXTPGridControl* pControl = m_pColumns ? m_pColumns->m_pControl : NULL;
	if (pControl && pControl->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(), m_strName);
	}
}

void CXTPGridColumn::SetFooterText(LPCTSTR strFooter)
{
	m_strFooterText = strFooter;
	if (m_pColumns)
	{
		m_pColumns->GetGridHeader()->GetControl()->AdjustLayout();
		m_pColumns->GetGridHeader()->GetControl()->AdjustScrollBars();
	}
}

void CXTPGridColumn::SetFooterFont(CFont* pFont)
{
	m_xtpFontFooter.DeleteObject(); // set default

	if (m_pColumns)
	{
		CFont* pFontDefault =
			&m_pColumns->GetGridHeader()->GetControl()->GetPaintManager()->m_xtpFontCaption;

		if (pFont && pFont->m_hObject && pFont->m_hObject != pFontDefault->m_hObject)
		{
			LOGFONT lfFooter;
			pFont->GetLogFont(&lfFooter);

			VERIFY(m_xtpFontFooter.CreateFontIndirect(&lfFooter));
		}

		m_pColumns->GetGridHeader()->GetControl()->AdjustLayout();
		m_pColumns->GetGridHeader()->GetControl()->AdjustScrollBars();
	}
}

void CXTPGridColumn::SetDrawFooterDivider(BOOL bSet)
{
	m_bDrawFooterDivider = bSet;

	if (m_pColumns)
		m_pColumns->GetGridHeader()->GetControl()->RedrawControl();
}

void CXTPGridColumn::SetDrawHeaderDivider(BOOL bSet)
{
	m_bDrawHeaderDivider = bSet;

	if (m_pColumns)
		m_pColumns->GetGridHeader()->GetControl()->RedrawControl();
}

CFont* CXTPGridColumn::GetFooterFont()
{
	if (m_xtpFontFooter.m_hObject != NULL)
		return &m_xtpFontFooter;

	if (m_pColumns)
		return &m_pColumns->GetGridHeader()->GetControl()->GetPaintManager()->m_xtpFontCaption;

	return NULL;
}

int CXTPGridColumn::GetNormAlignment(int nAlignment) const
{
	if (!GetColumns()->GetGridHeader()->GetPaintManager()->m_bRevertAlignment)
		return nAlignment;

	return nAlignment & DT_RIGHT ? (nAlignment - DT_RIGHT) : (nAlignment + DT_RIGHT);
}

int CXTPGridColumn::GetAlignment() const
{
	return GetNormAlignment(m_nAlignment);
}

int CXTPGridColumn::GetHeaderAlignment() const
{
	if (m_nHeaderAlignment != -1)
		return m_nHeaderAlignment;

	if (GetColumns()->GetGridHeader()->GetPaintManager()->m_bUseColumnTextAlignment)
		return GetAlignment();

	return GetNormAlignment(DT_LEFT);
}

int CXTPGridColumn::GetFooterAlignment() const
{
	if (m_nFooterAlignment != -1)
		return m_nFooterAlignment;

	if (GetColumns()->GetGridHeader()->GetPaintManager()->m_bUseColumnTextAlignment)
		return GetAlignment();

	return GetNormAlignment(DT_LEFT);
}

void CXTPGridColumn::SetFooterAlignment(int nAlignment)
{
	m_nFooterAlignment = nAlignment;

	if (m_pColumns)
	{
		m_pColumns->GetGridHeader()->GetControl()->AdjustLayout();
		m_pColumns->GetGridHeader()->GetControl()->AdjustScrollBars();
	}
}

int CXTPGridColumn::GetWidth() const
{
	return m_nColumnStaticWidth + (!m_bIsResizable ? GetIndent() : 0);
}

CRect CXTPGridColumn::GetRect() const
{
	return m_rcColumn;
}

int CXTPGridColumn::SetWidth(int nNewWidth)
{
	int nOldWidth		 = m_nColumnStaticWidth;
	m_nColumnStaticWidth = m_nColumnAutoWidth = nNewWidth;

	if (m_pColumns)
		m_pColumns->GetGridHeader()->OnColumnsChanged(xtpGridColumnWidthChanged, this);

	return nOldWidth;
}

BOOL CXTPGridColumn::IsSortedIncreasing() const
{
	return m_bSortIncreasing;
}
BOOL CXTPGridColumn::IsSortedDecreasing() const
{
	return !m_bSortIncreasing;
}

BOOL CXTPGridColumn::IsSorted() const
{
	if (m_pColumns)
		return (m_pColumns->GetSortOrder()->IndexOf(this) != -1);

	return FALSE;
}

void CXTPGridColumn::SetTreeColumn(BOOL bIsTreeColumn)
{
	if (m_pColumns == NULL)
		return;

	if (bIsTreeColumn)
	{
		m_pColumns->m_pTreeColumn = this;
	}
	else if (IsTreeColumn())
	{
		m_pColumns->m_pTreeColumn = NULL;
	}
}

BOOL CXTPGridColumn::IsTreeColumn() const
{
	if (m_pColumns)
		return m_pColumns->m_pTreeColumn == this;

	return FALSE;
}

BOOL CXTPGridColumn::IsVisible() const
{
	return m_bVisible;
}

void CXTPGridColumn::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("SortIncreasing"), m_bSortIncreasing, TRUE);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Int(pPX, _T("Alignment"), m_nAlignment, DT_LEFT);
	PX_DPI_X_Int(pPX, _T("StaticWidth"), m_nColumnStaticWidth, 0);
	PX_DPI_X_Int(pPX, _T("AutoWidth"), m_nColumnAutoWidth, 0);

	if (pPX->GetSchema() > _XTP_SCHEMA_1041)
	{
		PX_Int(pPX, _T("HeaderAlignment"), m_nHeaderAlignment, -1);
		PX_Int(pPX, _T("FooterAlignment"), m_nFooterAlignment, -1);
	}
	if (pPX->GetSchema() >= _XTP_SCHEMA_1310)
	{
		PX_Bool(pPX, _T("PlusMinus"), m_bPlusMinus, FALSE);
		PX_Int(pPX, _T("NextVisualBlock"), m_nNextVisualBlock, 0);
		PX_Int(pPX, _T("OldVisibleIndex"), m_nOldVisibleIndex, -1);
		PX_Bool(pPX, _T("Expanded"), m_bExpanded, TRUE);
	}
}

BOOL CXTPGridColumn::HasSortTriangle() const
{
	if (m_pColumns == NULL)
		return FALSE;

	if (m_pColumns->GetSortOrder()->IndexOf(this) != -1)
		return TRUE;

	if (m_pColumns->GetGroupsOrder()->IndexOf(this) != -1 && m_bAutoSortWhenGrouped)
		return TRUE;

	return FALSE;
}

int CXTPGridColumn::GetCaptionWidth(CDC* pDC)
{
	int nTextWidth = pDC->GetTextExtent(m_strName).cx;
	if (m_pMarkupUIElement)
	{
		// CSize szText = XTPMarkupMeasureElement(m_pMarkupUIElement, m_rcColumn.Width(), INT_MAX);
		CSize szText = XTPMarkupMeasureElement(m_pMarkupUIElement, nTextWidth, INT_MAX);
		nTextWidth   = szText.cx;
	}
	return nTextWidth;
}

BOOL CXTPGridColumn::IsDragging() const
{
	return m_pColumns && m_pColumns->GetGridHeader()
		   && m_pColumns->GetGridHeader()->m_pDragColumn == this;
}

int CXTPGridColumn::GetIndent() const
{
	if (m_pColumns == NULL || !m_pColumns->GetGridHeader())
		return 0;

	if (m_pColumns->GetVisibleAt(0) == this)
		return GetControl()->GetHeaderIndent();

	return 0;
}

int CXTPGridColumn::GetMinWidth() const
{
	return ((m_bIsResizable || m_bAutoSize) ? m_nMinWidth : (m_nColumnStaticWidth) + GetIndent());
}

int CXTPGridColumn::GetMaxWidth() const
{
	return m_nMaxWidth;
}

void CXTPGridColumn::SetVisible(BOOL bVisible)
{
	int IconViewColumn(0);
	if (GetControl())
	{
		IconViewColumn = GetControl()->m_iIconViewColumn;
		if (IconViewColumn > 0 && IconViewColumn == GetIndex())
		{
			if (!bVisible)
				SetWidth(0);
		}
	}
	if (bVisible != m_bVisible)
	{
		if (m_bVisible)
			m_nOldVisibleIndex = GetVisibleIndex();

		m_bVisible = bVisible;
		if (m_pColumns)
			m_pColumns->GetGridHeader()->OnColumnsChanged(xtpGridColumnOrderChanged
															  | (m_bVisible ? xtpGridColumnShown
																			: xtpGridColumnHidden),
														  this);
	}
}

int CXTPGridColumn::GetOldVisibleIndex() const
{
	return m_nOldVisibleIndex;
}

int CXTPGridColumn::GetItemIndex() const
{
	return m_nItemIndex;
}

void CXTPGridColumn::SetItemIndex(int nItemIndex)
{
	m_nItemIndex = nItemIndex;
}

int CXTPGridColumn::GetIndex() const
{
	if (m_pColumns == NULL)
		return -1;

	return m_pColumns->IndexOf(this);
}

int CXTPGridColumn::GetVisibleIndex() const
{
	if (m_pColumns)
	{
		int nIndex = -1;
		for (int i = 0; i < m_pColumns->GetCount(); i++)
		{
			if (m_pColumns->GetAt(i)->IsVisible())
			{
				nIndex++;
				if (m_pColumns->GetAt(i) == this)
					return nIndex;
			}
		}
	}

	return -1;
}

CXTPGridControl* CXTPGridColumn::GetControl() const
{
	if (m_pColumns)
		return m_pColumns->GetGridHeader()->GetControl();

	return NULL;
}

void CXTPGridColumn::SetShowInFieldChooser(BOOL bShow)
{
	if (bShow != m_bShowInFieldChooser)
	{
		m_bShowInFieldChooser = bShow;

		CXTPGridSubListControl* pSubList = m_pColumns
											   ? m_pColumns->GetGridHeader()->GetSubListCtrl()
											   : NULL;
		if (pSubList)
		{
			pSubList->UpdateList();
		}
	}
}

BOOL CXTPGridColumn::IsHotTracking() const
{
	if (m_pColumns)
		return m_pColumns->GetGridHeader()->GetHotTrackingColumn() == this;

	return FALSE;
}

BOOL CXTPGridColumn::IsShowInFieldChooser() const
{
	return m_bShowInFieldChooser;
}

int CXTPGridColumn::GetPrintWidth(int nTotalWidth) const
{
	if (m_pColumns == NULL)
		return 0;

	CXTPGridColumns* pColumns = m_pColumns;

	int nColumnsWidth				= 0;
	CXTPGridColumn* pLastAutoColumn = NULL;

	for (int nColumn = 0; nColumn < pColumns->GetCount(); nColumn++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		if (pColumn->IsAutoSize())
		{
			pLastAutoColumn = pColumn;
			nColumnsWidth += pColumn->GetWidth();
		}
		else
		{
			nTotalWidth -= pColumn->GetWidth();
		}
	}

	for (int i = 0; i < pColumns->GetCount(); i++)
	{
		CXTPGridColumn* pColumn = pColumns->GetAt(i);
		if (!pColumn->IsVisible())
			continue;

		int nWidth = pColumn->GetWidth();

		if (pColumn->IsAutoSize())
		{
			if (pColumn == pLastAutoColumn)
			{
				nWidth = max(nTotalWidth, pColumn->GetMinWidth());
			}
			else
			{
				nColumnsWidth = max(XTP_DPI_X(1), nColumnsWidth);

				nWidth = max(int(pColumn->GetWidth() * nTotalWidth / nColumnsWidth),
							 pColumn->GetMinWidth());

				nTotalWidth -= nWidth;
				nColumnsWidth -= pColumn->GetWidth();
			}
		}

		if (pColumn == this)
			return nWidth;
	}

	return 0;
}

int CXTPGridColumn::GetBestFitWidth()
{
	CXTPGridControl* pControl			= GetControl();
	CXTPGridPaintManager* pPaintManager = pControl->GetPaintManager();
	int nBestColumnWidth				= 0;

	CClientDC dc(pControl);
	CXTPFontDC font(&dc, &pPaintManager->m_xtpFontCaption);
	nBestColumnWidth = XTP_DPI_X(6) + GetCaptionWidth(&dc);

	if (GetIconID() != XTP_GRID_NOICON)
		nBestColumnWidth += pPaintManager->DrawBitmap(NULL, pControl, GetRect(), GetIconID())
							+ XTP_DPI_X(2);

	if (HasSortTriangle() && pPaintManager->m_DrawSortTriangleStyle != xtpGridDrawSortTriangleNever)
		nBestColumnWidth += XTP_DPI_X(27);

	int nDataWidth = pControl->OnGetColumnDataBestFitWidth(this);

	nBestColumnWidth = max(nBestColumnWidth, nDataWidth);
	nBestColumnWidth = max(nBestColumnWidth, m_nMaxItemWidth);

	return nBestColumnWidth;
}

int CXTPGridColumn::SetMinWidth(int nMinWidth)
{
	int OldMinWidth = m_nMinWidth;
	m_nMinWidth		= nMinWidth;

	return OldMinWidth;
}

int CXTPGridColumn::SetMaxWidth(int nMaxWidth)
{
	int OldMaxWidth = m_nMaxWidth;
	m_nMaxWidth		= nMaxWidth;

	return OldMaxWidth;
}

BOOL CXTPGridColumn::SetFilterable(BOOL bFiltrable)
{
	BOOL bOldFiltrable = m_bFilterable;
	m_bFilterable	  = bFiltrable;

	return bOldFiltrable;
}

void CXTPGridColumn::EnsureVisible()
{
	CXTPGridControl* pControl = GetControl();
	if (pControl)
		pControl->EnsureVisible(this);
}

int CXTPGridColumn::SetAlignment(int nAlignment)
{
	int nOld	 = m_nAlignment;
	m_nAlignment = nAlignment;

	return nOld;
}

void CXTPGridColumn::SetAllowDrag(BOOL bAllowDrag)
{
	// if (!m_bPlusMinus)
	m_bAllowDrag = bAllowDrag;
}

void CXTPGridColumn::SetPlusMinus(BOOL bPlusMinus)
{
	m_bPlusMinus = bPlusMinus;

	// if (m_bPlusMinus)
	//  m_bAllowDrag = FALSE;
}

void CXTPGridColumn::SetNextVisualBlock(int nNextVisualBlock)
{
	m_nNextVisualBlock = nNextVisualBlock;

	// if (m_pColumns == NULL)
	//	return;
	//
	// int i = GetIndex();
	// int n = m_pColumns->GetCount();
	// int k = 0;
	// for (int j = i + 1; j < n; j++)
	//{
	//  CXTPGridColumn* pCol = m_pColumns->GetAt(j);
	//  BOOL b = pCol->IsVisible();
	//  int l = pCol->GetVisibleIndex();

	//  if (pCol != this && b && k < m_nNextVisualBlock)
	//  {
	//      CString s = pCol->GetCaption();
	//      pCol->SetAllowDrag(FALSE);
	//      k++;
	//  }
	//}
	// m_nNextVisualBlock = k;
}

CXTPGridColumns* CXTPGridColumn::GetColumns() const
{
	return m_pColumns;
}

void CXTPGridColumn::SetExpanded(BOOL bExpanded)
{
	if (m_pColumns == NULL)
		return;

	m_bExpanded			 = bExpanded;
	int nCnt			 = m_pColumns->GetCount();
	int iN				 = GetNextVisualBlock();
	int iVs				 = GetVisibleIndex();
	CXTPGridColumn* pCol = NULL;
	for (int iC = 0; iC < nCnt; iC++)
	{
		pCol = m_pColumns->GetAt(nCnt - iC - 1);
		if (pCol)
		{
			if (bExpanded)
			{
				int iVc = pCol->GetVisibleIndex();
				if (iVc == -1)
					continue;
				if (iN > 0 && iVc > iVs && iVc <= iVs + iN)
					pCol->SetVisible(FALSE);
				else if (iN < 0 && iVc < iVs && iVc >= iVs + iN)
					pCol->SetVisible(FALSE);
			}
			else
			{
				int iVc = pCol->GetOldVisibleIndex();
				if (iVc == -1)
					continue;
				if (iN > 0 && iVc > iVs && iVc <= iVs + iN)
					pCol->SetVisible(TRUE);
				// else if (iN < 0 && iVc <= iVs + 1 && iVc >= iVs + iN)
				else if (iN < 0 && iVc <= iVs + 2 && iVc >= iVs + iN)
					pCol->SetVisible(TRUE);
			}
		}
	}
}

void CXTPGridColumn::SetMask(LPCTSTR strMask, LPCTSTR strLiteral, TCHAR chPrompt /*= _T('_')*/)
{
	m_strMask	= strMask;
	m_strLiteral = strLiteral;
	m_chPrompt   = chPrompt;
	m_bUseMask   = !m_strMask.IsEmpty();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridColumn, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "Caption", 1, OleGetCaption, SetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "Visible", 2, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "Alignment", 3, m_nAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "TreeColumn", 4, IsTreeColumn, SetTreeColumn, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "SortAscending", 5, m_bSortIncreasing, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "Sortable", 6, m_bSortable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "AllowDrag", 7, m_bAllowDrag, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "Index", 8, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "ItemIndex", 9, m_nItemIndex, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "Resizable", 10, m_bIsResizable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "AutoSize", 11, m_bAutoSize, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "Width", 12, GetWidth, SetWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "Icon", 13, m_nIconID, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "Groupable", 14, m_bGroupable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "ShowInFieldChooser", 15, IsShowInFieldChooser,
						SetShowInFieldChooser, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "Editable", 16, m_bEditable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "EditOptions", 17, OleGetEditOptions, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridColumn, "Move", 18, OleMove, VT_EMPTY, VTS_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "AllowRemove", 19, m_bAllowRemove, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "Tag", 20, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_ID(CXTPGridColumn, "Tooltip", 21, m_strTooltip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "FooterText", 22, OleGetFooterText, SetFooterText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridColumn, "HeaderAlignment", 23, m_nHeaderAlignment, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "FooterAlignment", 24, m_nFooterAlignment, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "AutoSortWhenGrouped", 25, m_bAutoSortWhenGrouped, VT_BOOL)
	DISP_FUNCTION_ID(CXTPGridColumn, "BestFit", 26, OleBestFit, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPGridColumn, "FooterFont", 27, OleGetFooterFont, OleSetFooterFont,
						VT_FONT)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "DrawFooterDivider", 28, GetDrawFooterDivider,
						SetDrawFooterDivider, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "DrawHeaderRowsVGrid", 29, GetDrawHeaderRowsVGrid,
						SetDrawHeaderRowsVGrid, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "DrawFooterRowsVGrid", 30, GetDrawFooterRowsVGrid,
						SetDrawFooterRowsVGrid, VT_BOOL)
	DISP_FUNCTION_ID(CXTPGridColumn, "EnsureVisible", 31, EnsureVisible, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_ID(CXTPGridColumn, "BestFitMode", 32, m_nBestFitMode, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "DrawHeaderDivider", 33, GetDrawHeaderDivider,
						SetDrawHeaderDivider, VT_BOOL)

	DISP_PROPERTY_ID(CXTPGridColumn, "Filterable", 34, m_bFilterable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "MinimumWidth", 35, m_nMinWidth, VT_I4)

	DISP_PROPERTY_ID(CXTPGridColumn, "PlusMinus", 36, m_bPlusMinus, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "Expanded", 37, m_bExpanded, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridColumn, "NextVisualBlock", 38, m_nNextVisualBlock, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "VisibleIndex", 39, GetVisibleIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridColumn, "OldVisibleIndex", 40, GetOldVisibleIndex, SetNotSupported,
						VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "MaximumWidth", 41, m_nMaxWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPGridColumn, "Key", 42, m_strInternalName, VT_BSTR)

	DISP_FUNCTION_ID(CXTPGridColumn, "SetMask", 43, OleSetMask, VT_EMPTY,
					 VTS_BSTR VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridColumn, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridColumn, XTPDIID_IGridColumn, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridColumn, XTPDIID_IGridColumn)

CXTPGridColumn* AFX_CDECL CXTPGridColumn::FromDispatch(LPDISPATCH pDisp)
{
	CXTPGridColumn* pInst = NULL;
	if (NULL != pDisp)
	{
		pInst = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPGridColumn,
											 CXTPGridColumn::FromIDispatchSafe(pDisp));
		if (NULL == pInst)
		{
			AfxThrowOleException(E_INVALIDARG);
		}
	}

	return pInst;
}

BSTR CXTPGridColumn::OleGetCaption()
{
	return m_strName.AllocSysString();
}

LPDISPATCH CXTPGridColumn::OleGetEditOptions()
{
	return m_pEditOptions->GetIDispatch(TRUE);
}

void CXTPGridColumn::OleMove(long nIndex)
{
	if (m_pColumns == NULL)
		return;

	m_pColumns->ChangeColumnOrder(nIndex, m_pColumns->IndexOf(this));
	m_pColumns->GetGridHeader()->OnColumnsChanged(xtpGridColumnOrderChanged | xtpGridColumnMoved,
												  this);
}

BSTR CXTPGridColumn::OleGetFooterText()
{
	return GetFooterText().AllocSysString();
}

void CXTPGridColumn::OleBestFit()
{
	if (m_pColumns && m_pColumns->m_pControl && m_pColumns->m_pControl->GetGridHeader())
	{
		m_pColumns->m_pControl->GetGridHeader()->BestFit(this);
	}
	else
	{
		ASSERT(FALSE);
	}
}

LPFONTDISP CXTPGridColumn::OleGetFooterFont()
{
	return AxCreateOleFont(GetFooterFont(), this,
						   (LPFNFONTCHANGED)&CXTPGridColumn::OleSetFooterFont);
}

void CXTPGridColumn::OleSetFooterFont(LPFONTDISP pFontDisp)
{
	LOGFONT lfNew, lfDef;
	if (!AxGetLogFontFromDispatch(&lfNew, pFontDisp))
		return;

	m_xtpFontFooter.DeleteObject(); // use default

	if (m_pColumns)
	{
		CFont* pFontDefault =
			&m_pColumns->GetGridHeader()->GetControl()->GetPaintManager()->m_xtpFontCaption;
		pFontDefault->GetLogFont(&lfDef);

		if (memcmp(&lfNew, &lfDef, sizeof(LOGFONT)) != 0)
		{
			VERIFY(m_xtpFontFooter.CreateFontIndirect(&lfNew));
		}

		m_pColumns->GetGridHeader()->GetControl()->AdjustLayout();
		m_pColumns->GetGridHeader()->GetControl()->AdjustScrollBars();
	}
}

void CXTPGridColumn::OleSetMask(LPCTSTR strMask, LPCTSTR strLiteral, LPCTSTR chPrompt)
{
	SetMask(strMask, strLiteral, chPrompt[0]);
}

#endif
