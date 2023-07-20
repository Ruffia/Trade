// XTPGridRecordItem.cpp : implementation of the CXTPGridRecordItem class.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <locale.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

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
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridHyperlink.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRecordItemConstraint.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "GridControl/XTPGridRecordItemControls.h"
#include "GridControl/XTPGridBorder.h"
#include "GridControl/XTPGridControlIIDs.h"
#include "GridControl/XTPGridGroupRow.h"

#ifdef _XTP_ACTIVEX
#	include "Markup/XTPMarkupObject.h"
#	include "Markup/XTPMarkupInputElement.h"
#	include "Markup/XTPMarkupUIElement.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#	define IDC_HAND MAKEINTRESOURCE(32649)
#endif

//////////////////////////////////////////////////////////////////////////
// XTP_GRIDRECORDITEM_ARGS

XTP_GRIDRECORDITEM_ARGS::XTP_GRIDRECORDITEM_ARGS()
{
	pControl = NULL;
	pRow	 = NULL;
	pColumn  = NULL;
	pItem	= NULL;
	rcItem.SetRectEmpty();
}

XTP_GRIDRECORDITEM_ARGS::XTP_GRIDRECORDITEM_ARGS(CXTPGridControl* pControl, CXTPGridRow* pRow,
												 CXTPGridColumn* pColumn)
{
	ASSERT(pControl && pRow && pColumn);

	this->pControl = pControl;
	this->pRow	 = pRow;
	this->pColumn  = pColumn;
	this->pItem	= pRow->GetRecord()->GetItem(pColumn);
	this->rcItem   = pRow->GetItemRect(pItem);
}

XTP_GRIDRECORDITEM_ARGS::~XTP_GRIDRECORDITEM_ARGS()
{
}

void XTP_GRIDRECORDITEM_ARGS::AddRef()
{
	CMDTARGET_ADDREF(pRow);
	CMDTARGET_ADDREF(pColumn);
	CMDTARGET_ADDREF(pItem);
}

void XTP_GRIDRECORDITEM_ARGS::Release()
{
	if (pRow)
		pRow->InternalRelease();

	if (pColumn)
		pColumn->InternalRelease();

	if (pItem)
		pItem->InternalRelease();
}

const XTP_GRIDRECORDITEM_ARGS& XTP_GRIDRECORDITEM_ARGS::operator=(const XTP_GRIDRECORDITEM_ARGS& src)
{
	pControl = src.pControl;
	pRow	 = src.pRow;
	pColumn  = src.pColumn;
	pItem	= src.pItem;
	rcItem   = src.rcItem;

	return *this;
}

CXTPGridInplaceButtons::CXTPGridInplaceButtons()
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItem

IMPLEMENT_SERIAL(CXTPGridRecordItem, CXTPCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPGridRecordItem::CXTPGridRecordItem()
	: m_pMergeItem(NULL)
	, m_pControl(NULL)
	, m_pRecord(NULL)
	, m_pBorder(NULL)
	, m_checkState(xtpGridCheckStateUnchecked)
{
	static const CString g_strFormatStringDef = _T("%s");

	m_strFormatString = g_strFormatStringDef;
	m_strFormula.Empty();

	m_bEditable	= TRUE;
	m_pFontCaption = NULL;
	m_bBoldText	= FALSE;

	m_clrText		= XTP_GRID_COLOR_DEFAULT;
	m_clrBackground = XTP_GRID_COLOR_DEFAULT;

	m_nIconIndex = XTP_GRID_NOICON;

	m_dwData = 0;

	m_nSortPriority  = -1;
	m_nGroupPriority = -1;

	m_Alignment = (XTPGridColumnIconAlignment)(-1);

	m_bHasCheckbox			 = FALSE;
	m_bCanEditCheckboxItem   = FALSE;
	m_bTristateCheckbox		 = FALSE;
	m_pEditOptions			 = NULL;
	m_pItemControls			 = NULL;
	m_pFocusedItemControl	= NULL;
	m_bItemControlUnderMouse = FALSE;
	m_pItemControlHookWnd	= NULL;

	m_bFocusable = TRUE;

	m_pHyperlinks = NULL;

	m_pMarkupUIElement = NULL;

	m_rcGlyph.SetRectEmpty();
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecordItem::~CXTPGridRecordItem()
{
	if (m_pItemControlHookWnd)
	{
		m_pItemControlHookWnd->DestroyWindow();
		delete m_pItemControlHookWnd;
		m_pItemControlHookWnd = NULL;
	}

	CMDTARGET_RELEASE(m_pEditOptions);
	CMDTARGET_RELEASE(m_pItemControls);
	CMDTARGET_RELEASE(m_pHyperlinks);

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

double CXTPGridRecordItem::StringToDouble(CString strText)
{
	TCHAR* locale = _tsetlocale(LC_NUMERIC, NULL);

	if (locale == NULL || _tcscmp(locale, _T("C")) == 0) // Not set
	{
		strText.Replace(',', '.');
	}

	double res = atof(XTP_CT2CA(strText));

	return res;
}

void CXTPGridRecordItem::ParseBBCode(CString& strText)
{
	GetHyperlinks()->RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// Left [url=address]name[/url] Right
	//////////////////////////////////////////////////////////////////////////

	const CString sUrlTagOpen  = _T("[url");
	const CString sUrlTagClose = _T("[/url]");

	int nOpenTagStart = strText.Find(sUrlTagOpen);
	int nOpenTagEnd;

	int nCloseTagStart;
	int nCloseTagEnd;

	while (-1 != nOpenTagStart)
	{
		TCHAR ch = strText.GetAt(nOpenTagStart + sUrlTagOpen.GetLength());
		if (_T('=') != ch && _T(']') != ch)
		{
			break;
		}

		nOpenTagEnd = strText.Find(_T("]"), nOpenTagStart);
		if (-1 == nOpenTagEnd)
		{
			break;
		}

		CString sLeft = strText.Left(nOpenTagStart);

		nCloseTagStart = strText.Find(sUrlTagClose);
		if (-1 == nCloseTagStart)
		{
			break;
		}

		CString sName = strText.Mid(nOpenTagEnd + 1, nCloseTagStart - nOpenTagEnd - 1);

		nCloseTagEnd   = nCloseTagStart + sUrlTagClose.GetLength();
		CString sRight = strText.Right(strText.GetLength() - nCloseTagEnd);

		strText = sLeft + sName + sRight;

		GetHyperlinks()->Add(
			new CXTPGridHyperlink(nOpenTagStart, nCloseTagStart - nOpenTagEnd - 1));

		nOpenTagStart = strText.Find(sUrlTagOpen);
	}
}

void CXTPGridRecordItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (!m_strFormula.IsEmpty() && m_pRecord != NULL)
	{
		COLORREF clrSum	= RGB(255, 0, 0);
		COLORREF clrSubSum = RGB(0, 0, 255);
		if (m_pControl)
		{
			clrSum	= m_pControl->GetPaintManager()->m_clrFormulaSum;
			clrSubSum = m_pControl->GetPaintManager()->m_clrFormulaSubSum;
		}
		// CODE FOR RECALC
		double d(0.0);
		int m(0), M(0), n(0), N(0);
		CString s, t, u, v;
		CString sFormula(m_strFormula);
		int pos = sFormula.Find(_T("SUMSUB("));
		// pattern SUMSUB(R#C#:R#C#) or * instead of # - means all row or column
		if (pos > -1 && m_pRecord->GetChilds() != NULL)
		{
			int mMax = m_pRecord->GetChilds()->GetCount();
			int nMax = m_pRecord->GetItemCount();
			sFormula.Replace(_T(" "), _T(""));
			sFormula = sFormula.Mid(pos + 7);
			pos		 = sFormula.Find(_T(":"));
			if (pos > -1)
			{
				s   = sFormula.Left(pos);
				t   = sFormula.Mid(pos + 1);
				pos = s.Find(_T("C"));
				if (pos > -1)
				{
					u = s.Left(pos);
					s = s.Mid(pos + 1);
					u.Replace(_T("R"), _T(""));
					if (u == _T("*"))
						m = 0;
					else
						m = _ttoi(u);
					if (s == _T("*"))
						n = 0;
					else
						n = _ttoi(s);

					pos = t.Find(_T("C"));
					if (pos > -1)
					{
						u = t.Left(pos);
						t = t.Mid(pos + 1);
						u.Replace(_T("R"), _T(""));
						if (u == _T("*"))
							M = mMax;
						else
							M = _ttoi(u);
						t.Replace(_T(")"), _T(""));
						if (t == _T("*"))
							N = nMax;
						else
							N = _ttoi(t);

						CXTPGridRecord* pRec	  = NULL;
						CXTPGridRecordItem* pItem = NULL;
						if (mMax > 0)
						{
							for (int row = m; row < M; row++)
							{
								for (int col = n; col < N; col++)
								{
									if (row < mMax)
									{
										pRec = m_pRecord->GetChilds()->GetAt(row);
										if (pRec)
										{
											if (col < pRec->GetItemCount())
											{
												pItem = pRec->GetItem(col);
												if (pItem && pItem != this) // no circular ref!)
												{
													s = pItem->GetCaption(NULL);
													d += StringToDouble(s);
												}
											}
										}
									}
								}
							}
							CString sFmt = GetFormatString();
							if (sFmt.IsEmpty() || sFmt == _T("%s"))
								sFmt = _T("%f");
							else if (sFmt == _T("%d"))
								sFmt = _T("%.0f");

							strCaption.Format(sFmt, d);
							SetTextColor(clrSubSum);
						}
					}
				}
			}
		}
		sFormula = m_strFormula;
		pos		 = sFormula.Find(_T("SUM("));
		// pattern SUM(R#C#:R#C#) or * instead of # - means all row or column
		if (pos > -1 && m_pRecord->GetRecords() != NULL)
		{
			int mMax = m_pRecord->GetRecords()->GetCount();
			int nMax = m_pRecord->GetItemCount();
			sFormula.Replace(_T(" "), _T(""));
			sFormula = sFormula.Mid(pos + 4);
			pos		 = sFormula.Find(_T(":"));
			if (pos > -1)
			{
				s   = sFormula.Left(pos);
				t   = sFormula.Mid(pos + 1);
				pos = s.Find(_T("C"));
				if (pos > -1)
				{
					u = s.Left(pos);
					s = s.Mid(pos + 1);
					u.Replace(_T("R"), _T(""));
					if (u == _T("*"))
						m = 0;
					else
						m = _ttoi(u);
					if (s == _T("*"))
						n = 0;
					else
						n = _ttoi(s);

					pos = t.Find(_T("C"));
					if (pos > -1)
					{
						u = t.Left(pos);
						t = t.Mid(pos + 1);
						u.Replace(_T("R"), _T(""));
						if (u == _T("*"))
							M = mMax;
						else
							M = _ttoi(u);
						t.Replace(_T(")"), _T(""));
						if (t == _T("*"))
							N = nMax;
						else
							N = _ttoi(t);

						CXTPGridRecord* pRec	  = NULL;
						CXTPGridRecordItem* pItem = NULL;
						if (m_pRecord->GetRecords())
						{
							for (int row = m; row < M; row++)
							{
								for (int col = n; col < N; col++)
								{
									if (row < mMax)
									{
										pRec = m_pRecord->GetRecords()->GetAt(row);
										if (pRec)
										{
											if (col < pRec->GetItemCount())
											{
												pItem = pRec->GetItem(col);
												if (pItem && pItem != this) // no circular ref!
												{
													s = pItem->GetCaption(NULL);
													d += StringToDouble(s);
												}
											}
										}
									}
								}
							}
							CString sFmt = GetFormatString();
							if (sFmt.IsEmpty() || sFmt == _T("%s"))
								sFmt = _T("%f");
							else if (sFmt == _T("%d"))
								sFmt = _T("%.0f");

							strCaption.Format(sFmt, d);
							SetTextColor(clrSum);
						}
					}
				}
			}
		}
	}
	// else if (m_strFormula.IsEmpty())
	//{
	//  CString sFmt = GetFormatString();
	//  if (sFmt.Find(_T("%")) > -1)
	//  {
	//      if (sFmt.Find(_T("d")) > -1)
	//          strCaption.Format(sFmt, _ttoi(strCaption));
	//      else if (sFmt.Find(_T("f")) > -1 || sFmt.Find(_T("g")) > -1)
	//          strCaption.Format(sFmt, _tstof(strCaption));
	//      else
	//          strCaption.Format(sFmt, strCaption);
	//  }
	//}
	if (m_strCaption == strCaption)
		return;

	m_strCaption = strCaption;
	ResetMarkupUIElement();
}

void CXTPGridRecordItem::UpdateValue()
{
	if (!m_strFormula.IsEmpty())
		SetCaption(_T(""));
}

void CXTPGridRecordItem::ResetMarkupUIElement()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);

	CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;
	if (pMarkupContext)
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strCaption); // don't GetCaption()
}

void CXTPGridRecordItem::DoMouseButtonClick()
{
	mouse_event(DWORD(GetSystemMetrics(SM_SWAPBUTTON) ? MOUSEEVENTF_RIGHTDOWN
													  : MOUSEEVENTF_LEFTDOWN),
				0, 0, 0, 0);
	mouse_event(DWORD(GetSystemMetrics(SM_SWAPBUTTON) ? MOUSEEVENTF_RIGHTUP : MOUSEEVENTF_LEFTUP),
				0, 0, 0, 0);
}

CString CXTPGridRecordItem::GetCaption()
{
	if (IsChildOfMerge())
	{
		return GetMergeItem()->GetCaption(GetColumn());
	}
	return GetCaption(GetColumn());
};

CString CXTPGridRecordItem::GetCaption(CXTPGridColumn* pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);

	if (IsChildOfMerge())
	{
		return GetMergeItem()->m_strCaption;
	}
	return m_strCaption;
};

void CXTPGridRecordItem::GetCaptionRect(XTP_GRIDRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	m_pControl = pDrawArgs->pControl;

	CXTPGridPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	if (GetIconIndex() != XTP_GRID_NOICON)
	{
		int nIconAlignment = GetAlignment();
		if (nIconAlignment == -1)
		{
			CXTPGridColumn* pColumn = GetColumn();
			if (pColumn)
				nIconAlignment = pColumn->GetAlignment();

			nIconAlignment = nIconAlignment == -1 ? 0 : nIconAlignment;
		}

		if ((nIconAlignment & xtpColumnIconCenter) == 0)
		{
			if ((nIconAlignment & xtpColumnIconRight) != 0)
				rcItem.right -= pPaintManager->DrawBitmap(NULL, pDrawArgs->pControl, rcItem,
														  GetIconIndex());
			else
				rcItem.left += pPaintManager->DrawBitmap(NULL, pDrawArgs->pControl, rcItem,
														 GetIconIndex());
		}
	}

	if (m_bHasCheckbox)
		rcItem.left += XTP_DPI_X(15);

	if (m_pItemControls && m_pItemControls->GetSize())
	{
		for (int i = 0; i < m_pItemControls->GetSize() && rcItem.Width() > 0; i++)
		{
			int nWidth = m_pItemControls->GetAt(i)->GetSize().cx;
			nWidth	 = nWidth < 0 ? rcItem.Width() : nWidth;
			if (m_pItemControls->GetAt(i)->GetAlignment() == xtpItemControlRight)
				rcItem.right -= nWidth;
			else
				rcItem.left += nWidth;
		}
	}

	// Adjust for new lock icon
	BOOL bShowLock = !IsEditable() && pPaintManager->m_bShowLockIcon;

	if (bShowLock && !pDrawArgs->pControl->IsIconView()) // to prevent reaction on always read-only
														 // IconColumn / NumberColumn
	{
		rcItem.left += XTP_DPI_X(18);
	}
}

void CXTPGridRecordItem::DrawCheckBox(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	BOOL bEditable = IsEditable()
					 && (pDrawArgs->pColumn == NULL || pDrawArgs->pColumn->IsEditable());

	CDC* pDC = pDrawArgs->pDC;

	XTPGridGlyph eState = xtpGridGlyphInvalid;

	if (bEditable && !IsChecked())
	{
		eState = xtpGridGlyphEnabledUnchecked;
	}
	else if (bEditable && IsChecked())
	{
		eState = xtpGridGlyphEnabledChecked;
	}
	else if (!bEditable && !IsChecked())
	{
		eState = xtpGridGlyphDisabledUnchecked;
	}
	else if (!bEditable && IsChecked())
	{
		eState = xtpGridGlyphDisabledChecked;
	}

	if (m_bTristateCheckbox && GetCheckedState() == xtpGridCheckStateIndeterminate)
	{
		eState = xtpGridGlyphEnabledTristate;
	}

	CXTPGridPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	int nIconAlign  = pDrawArgs->nTextAlign & xtpColumnIconMask;
	int nIconAlignV = nIconAlign & 0x0F000000;
	int nIconAlignH = nIconAlign & 0x00F00000;

	m_rcGlyph		= rcItem;
	CSize sizeGlyph = pPaintManager->DrawGlyph(NULL, m_rcGlyph, eState);

	CRect rcGlyph(m_rcGlyph);
	// horizontal alignment
	switch (nIconAlignH)
	{
		case xtpColumnIconRight:
			rcGlyph.left = rcItem.right - sizeGlyph.cx - XTP_DPI_X(2);
			// shift text box to the left
			rcItem.right -= (XTP_DPI_X(2) + sizeGlyph.cx);
			break;
		case xtpColumnIconCenter:
			rcGlyph.left  = rcItem.CenterPoint().x - sizeGlyph.cx / 2 - XTP_DPI_X(1);
			rcGlyph.right = rcGlyph.left + sizeGlyph.cx;
			break; // left text box as is - draw on image...
		case xtpColumnIconLeft:
		default:
			// shift text box to the right
			rcItem.left += XTP_DPI_X(2) + sizeGlyph.cx;
			rcGlyph.left += XTP_DPI_X(2);
			rcGlyph.right = rcItem.left - XTP_DPI_X(1);
			break;
	}
	// vertical alignment
	switch (nIconAlignV)
	{
		case xtpColumnIconTop: rcGlyph.bottom = rcItem.top + sizeGlyph.cy + XTP_DPI_Y(2); break;
		case xtpColumnIconBottom: rcGlyph.top = rcItem.bottom - sizeGlyph.cy - XTP_DPI_Y(2); break;
	}
	// draw the glyph
	pPaintManager->DrawGlyph(pDC, rcGlyph, eState);

	m_rcGlyph = rcGlyph; // to restrict click on Glyph area only
}

void CXTPGridRecordItem::OnDrawCaption(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
									   XTP_GRIDRECORDITEM_METRICS* pMetrics)
{
	ASSERT(pDrawArgs->pItem == this);

	CXTPGridPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();
	BOOL bForceMarkUp(FALSE);
	if (pPaintManager)
		bForceMarkUp = pPaintManager->m_bForceDynamicMarkupForCell;
	// THIS CONDITION - IsVirtualMode() [Rev 7687 Oct 2008] PREVENTS MARKUP in Non-Virtual mode
	// if (pDrawArgs->pControl->IsVirtualMode() && pDrawArgs->pControl->GetMarkupContext())
	// if (pDrawArgs->pControl->GetMarkupContext()
	//  && !m_pMarkupUIElement) - rev 9113
	// if (pDrawArgs->pControl->IsVirtualMode()
	//  && pDrawArgs->pControl->GetMarkupContext() && !m_pMarkupUIElement) - rev 9114
	// FIXED BACK! Only for virtual mode! Developer have to set Caption of Item to have markup
	// FIXED AGAIN WITH SPECIAL Flag to use
	if ((pDrawArgs->pControl->IsVirtualMode() || bForceMarkUp)
		&& pDrawArgs->pControl->GetMarkupContext())
	{
		CXTPMarkupUIElement* pMarkupUIElement =
			XTPMarkupParseText(pDrawArgs->pControl->GetMarkupContext(), pMetrics->strText);
		if (pMarkupUIElement)
		{
			CRect rcItem = pDrawArgs->rcItem;
			rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);

			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement),
									(HFONT)pMetrics->pFont->GetSafeHandle(),
									pMetrics->clrForeground);
			XTPMarkupMeasureElement(pMarkupUIElement, rcItem.Width(), INT_MAX);
			XTPMarkupRenderElement(pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
			XTPMarkupReleaseElement(pMarkupUIElement);
			return;
		}
	}

	if (m_pMarkupUIElement)
	{
		CRect rcItem = pDrawArgs->rcItem;
		rcItem.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(1), XTP_DPI_X(2), 0);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
								(HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);
		XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width(), INT_MAX);
		XTPMarkupRenderElement(m_pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
	}
	else
	{
		if (pPaintManager)
		{
			pPaintManager->DrawItemCaption(pDrawArgs, pMetrics);
		}
	}
}

void CXTPGridRecordItem::OnDrawControls(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pItem == this);
	if (!m_pItemControls)
		return;
	for (int i = 0; i < m_pItemControls->GetSize() && rcItem.Width(); i++)
	{
		pDrawArgs->pControl->GetPaintManager()->DrawItemControl(pDrawArgs,
																m_pItemControls->GetAt(i), rcItem);
	}
}

int CXTPGridRecordItem::Draw(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return 0;
	else
		m_pControl = pDrawArgs->pControl;

	CDC* pDC = pDrawArgs->pDC;

	CRect& rcItem						= pDrawArgs->rcItem;
	CXTPGridPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS();
	pMetrics->strText					 = GetCaption(pDrawArgs->pColumn);
	pDrawArgs->pRow->GetItemMetrics(pDrawArgs, pMetrics);

	ASSERT(pMetrics->pFont);
	ASSERT(pMetrics->clrForeground != XTP_GRID_COLOR_DEFAULT);

	if (pMetrics->clrBackground != XTP_GRID_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcItem, pMetrics->clrBackground);
	}

	int nItemGlyphs = rcItem.left;

	// draw tree inside item rect (see also HitTest function)
	if (pDrawArgs->pColumn && pDrawArgs->pColumn->IsTreeColumn())
	{
		int nTreeDepth = pDrawArgs->pRow->GetTreeDepth() - pDrawArgs->pRow->GetGroupLevel();
		if (nTreeDepth > 0)
			nTreeDepth++;

		BOOL bHasChildren = TRUE;
		if (pDrawArgs->pControl->IsVirtualMode())
		{
			bHasChildren = (pMetrics->nVirtRowFlags & xtpVirtRowHasChildren) > 0;
			if (!bHasChildren)
				pDrawArgs->pRow->m_rcCollapse = CRect(NULL);

			if (pDrawArgs->pColumn == pDrawArgs->pControl->GetColumns()->GetIndentColumn())
				pDrawArgs->nTextAlign = DT_LEFT; // to make compact tree presentation
		}
		else
		{
			bHasChildren = pDrawArgs->pRow->HasChildren();
		}
		rcItem.left += pDrawArgs->pControl->GetIndent(nTreeDepth);

		if (pPaintManager->m_nBaseParentRecordIndent != 0)
		{
			if ((pDrawArgs->pRow->GetParentRow() != NULL
					 ? pDrawArgs->pRow->GetParentRow()->IsGroupRow()
					 : FALSE)
				&& pDrawArgs->pRow->HasChildren() && (nTreeDepth == 0))
				rcItem.left += pPaintManager->m_nBaseParentRecordIndent;

			if ((pDrawArgs->pRow->GetParentRow() != NULL
					 ? !pDrawArgs->pRow->GetParentRow()->IsGroupRow()
					 : FALSE)
				&& (nTreeDepth > 0)
				&& pDrawArgs->pControl->GetColumns()->GetGroupsOrder()->GetCount() > 0)
				rcItem.left += pPaintManager->m_nBaseParentRecordIndent;
		}

		// EXTRA TREE INDENT
		if (pPaintManager->m_bBaseTreeIndent)
		{
			rcItem.left += pPaintManager->m_nTreeIndent;
		}

		CRect rcBitmap(rcItem);
		CSize sizeBitmap = pPaintManager->DrawCollapsedBitmap(NULL, pDrawArgs->pRow, rcBitmap);

		pPaintManager->DrawTreeStructure(pDrawArgs, pMetrics, rcItem, sizeBitmap);

		int nTreeTextIndent = 0;

		if (bHasChildren)
		{
			sizeBitmap = pPaintManager->DrawCollapsedBitmap(pDC, pDrawArgs->pRow, rcBitmap);

			if (!pDC->IsPrinting())
			{
				pDrawArgs->pRow->SetCollapseRect(rcBitmap);
			}

			nTreeTextIndent = pPaintManager->m_nTreeTextIndentRowParent;
		}
		else
		{
			nTreeTextIndent = pPaintManager->m_nTreeTextIndentRowChildren;
		}

		rcItem.left = rcBitmap.left;
		rcItem.left += sizeBitmap.cx;
		rcItem.left += nTreeTextIndent;
	}

	pDC->SetTextColor(pMetrics->clrForeground);

	CFont* pOldFont = (CFont*)pDC->SelectObject(pMetrics->pFont);

	if (pMetrics->clrBackground != XTP_GRID_COLOR_DEFAULT)
	{
		pDC->SetBkColor(pMetrics->clrBackground);
	}
	else
	{
		pDC->SetBkColor(pPaintManager->m_clrControlBack);
	}

	// Draw checkbox
	if (m_bHasCheckbox)
	{
		DrawCheckBox(pDrawArgs, rcItem);
	}

	BOOL bShowLock = !IsEditable() && pDrawArgs->pControl->GetPaintManager()->m_bShowLockIcon;
	// Do the draw bitmap pDC, rcItem, GetIconIndex()
	if (bShowLock && !pDrawArgs->pControl->IsIconView()) // to prevent reaction on always read-only
														 // IconColumn / NumberColumn
	{
		rcItem.left++;

		if (pDrawArgs->pControl->GetPaintManager()->m_bUseShellIcon) // variant without resources
		{
			pPaintManager->DrawShellIcon(pDC, rcItem, xtpGridShellIconLock);
		}
		else // variant with resources
		{
			pPaintManager->DrawGlyph(pDC, rcItem, xtpGridGlyphItemLocked);
		}

		rcItem.left += XTP_DPI_X(17);
	}
	else if (pMetrics->nItemIcon != XTP_GRID_NOICON || GetIconIndex() != XTP_GRID_NOICON)
	{
		pPaintManager->DrawItemBitmap(pDrawArgs, rcItem,
									  pMetrics->nItemIcon != XTP_GRID_NOICON ? pMetrics->nItemIcon
																			 : GetIconIndex());
	}
	else
	{
		CXTPGridRecordItemEditOptions* pEditOptions = GetEditOptions(pDrawArgs->pColumn);
		if (pEditOptions && pEditOptions->arrInplaceButtons.GetSize() > 0 && IsAllowEdit(pDrawArgs))
		{
			CXTPGridInplaceButton* pButton = pEditOptions->GetButton(0);
			if (pButton && pButton->IsInsideCellButton())
			{
				CRect rc(rcItem);
				rc.left = rc.right - XTP_DPI_X(17); // like CXTPGridInplaceButton::m_nWidth

				if (pPaintManager->IsFixedInplaceButtonHeight())
					rc.bottom =
						min(rc.bottom,
							rc.top + XTP_DPI_Y(19)); // like CXTPGridInplaceButton::m_nFixedHeight

				BOOL bShift(FALSE);
				// to keep focused frame not touched
				rc.top++;
				rc.bottom -= XTP_DPI_Y(2);
				rc.left--;
				if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON)
					rc.right--;

				if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON)
					bShift = pPaintManager->DrawInplaceComboButton(pDC, &rc);
				else if (pButton->GetID() == XTP_ID_GRID_EXPANDBUTTON)
					bShift = pPaintManager->DrawInplaceExpandButton(pDC, &rc);
				else if (pButton->GetID() == XTP_ID_GRID_SPINBUTTON)
					bShift = pPaintManager->DrawInplaceSpinButton(pDC, &rc);

				if (bShift)
					pDrawArgs->rcItem.right -= XTP_DPI_X(17);
			}
		}
	}
	nItemGlyphs = (rcItem.left - nItemGlyphs);

	OnDrawControls(pDrawArgs, rcItem);

	if (rcItem.Width() > XTP_DPI_X(2))
	{
		OnDrawCaption(pDrawArgs, pMetrics);
	}

	int nItemTextWidth = nItemGlyphs + XTP_DPI_Y(7);
	if (m_pMarkupUIElement)
	{
		nItemTextWidth +=
			XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width() - XTP_DPI_X(4), INT_MAX).cx;
	}
	else
	{
		CString sFirst(pMetrics->strText);
		CString sLast;
		int jPos = sFirst.Find(_T('\n'), 0);
		int wd(0);
		while (jPos > -1)
		{
			sLast  = sFirst.Mid(jPos + 1);
			sFirst = sFirst.Left(jPos - 1);
			wd	 = max(wd, pDC->GetTextExtent(sFirst).cx + XTP_DPI_X(7));
			sFirst = sLast;
			jPos   = sFirst.Find(_T('\n'), 0);
		}

		if (!sFirst.IsEmpty())
		{
			nItemTextWidth = max(wd, pDC->GetTextExtent(sFirst).cx + XTP_DPI_X(7));
		}
	}
	pDC->SelectObject(pOldFont);

	pMetrics->InternalRelease();

	return nItemTextWidth;
}

CXTPGridRecordItemControl* CXTPGridRecordItem::HitTest(CPoint ptPoint)
{
	if (!(m_pItemControls && m_pItemControls->GetSize()))
		return NULL;

	for (int i = 0; i < m_pItemControls->GetSize(); i++)
	{
		CRect rcControl = m_pItemControls->GetAt(i)->GetRect();
		if (rcControl.PtInRect(ptPoint))
			return m_pItemControls->GetAt(i);
	}

	return NULL;
}

BOOL CXTPGridRecordItem::OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	CXTPGridRecordItemControl* pItemControl = HitTest(pClickArgs->ptClient);
	if (!pItemControl)
		return FALSE;

	pItemControl->OnLButtonDown(pClickArgs);

	// create item control window
	if (m_pItemControlHookWnd == NULL)
	{
		m_pItemControlHookWnd = new CXTPGridRecordItemControlHookWnd(pClickArgs);
		m_pItemControlHookWnd->Create(AfxRegisterWndClass(0), _T(""), WS_CHILD,
									  pItemControl->GetRect(), pClickArgs->pControl, 0);
		m_pItemControlHookWnd->SetCapture();
	}
	// redraw control
	pClickArgs->pControl->RedrawControl();
	return TRUE;
}

BOOL CXTPGridRecordItem::OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	//>>attempt for 23588 issue case
	CXTPSmartPtrInternalT<CXTPGridRecordItem> xSafety(this, TRUE);
	//>>attempt for 23588 issue case

	if (m_pItemControlHookWnd)
	{
		ReleaseCapture();

		pClickArgs->pControl->ScreenToClient(&pClickArgs->ptClient);

		if (m_pFocusedItemControl)
			m_pFocusedItemControl->OnLButtonUp(pClickArgs);

		pClickArgs->pControl->RedrawControl();

		// destroy item control window
		m_pItemControlHookWnd->DestroyWindow();

		delete m_pItemControlHookWnd;
		m_pItemControlHookWnd = NULL;

		return TRUE;
	}
	CXTPGridRecordItemControl* pItemControl = HitTest(pClickArgs->ptClient);
	if (!pItemControl)
		return FALSE;
	pItemControl->OnLButtonUp(pClickArgs);
	pClickArgs->pControl->RedrawControl();
	return TRUE;
}

void CXTPGridRecordItem::OnClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonUp(pClickArgs)) // old version (10) did not have this call
		return;

	InternalAddRef();
	CMDTARGET_ADDREF(pClickArgs->pColumn);
	CMDTARGET_ADDREF(pClickArgs->pControl);

	CXTPGridRow* pRow = pClickArgs->pRow;
	CMDTARGET_ADDREF(pRow);

	BOOL bCheckBoxClicked	 = FALSE;
	CXTPGridControl* pControl = pClickArgs->pControl;

	if (IsEditable() && m_bHasCheckbox
		&& (pClickArgs->pColumn == NULL || pClickArgs->pColumn->IsEditable())
		&& OnRequestEdit(pClickArgs))
	{
		// adjust vertical coordinates if virtual mode
		if (pControl->IsVirtualMode())
		{
			m_rcGlyph.top	= pClickArgs->rcItem.top;
			m_rcGlyph.bottom = pClickArgs->rcItem.bottom;
		}
		// check whether we're clicking glyph area
		if (m_rcGlyph.PtInRect(pClickArgs->ptClient)
			&& m_rcGlyph.PtInRect(pControl->m_mouseDownState.ptMouse))
		{
			if (pControl->IsAutoCheckItems())
			{
				if (!m_bTristateCheckbox)
				{
					SetChecked(!IsChecked());
				}
				else
				{
					SetCheckedState((GetCheckedState() + 1) % xtpGridCheckStateCount);
				}
			}
			pControl->RedrawControl();

			pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn,
										  XTP_NM_GRID_CHECKED, &pClickArgs->ptClient);
			bCheckBoxClicked = TRUE;
		}
	}

	if (!bCheckBoxClicked && IsAllowEdit(pClickArgs))
	{
		if (pControl->IsEditOnDelayClick())
		{
			if (pControl->GetLastRqstEditCol() != pClickArgs->pColumn->GetItemIndex()
				|| pControl->GetLastRqstEditRow() != pClickArgs->pRow->GetIndex())
			{
				pControl->EnsureStopLastRqstTimer();
				pControl->SetLastRqstEdit(pClickArgs->pRow->GetIndex(),
										  pClickArgs->pColumn->GetItemIndex());
			}
			else if (!pControl->GetDelayEditTimer())
			{
				// Start the timer for this request.
				pControl->StartDelayEditTimer();
			}
		}
		if (pControl->IsEditOnClick())
		{
			pControl->EditItem(pClickArgs);

			CPoint pt(pClickArgs->ptClient);
			pControl->ClientToScreen(&pt);

			CXTPGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPGridInplaceEdit,
														  CWnd::FromHandle(WindowFromPoint(pt)));
			if (pEdit && pEdit->GetItem() == this)
			{
				CXTPGridRecordItemEditOptions* pEditOptions = GetEditOptions(pClickArgs->pColumn);

				if (pEditOptions->m_bSelectTextOnEdit)
					pEdit->SetSel(0, -1);
				else
					DoMouseButtonClick();
			}
		}
	}

	pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_CLICK,
											  &pClickArgs->ptClient);

	// Determine Hyperlink Click
	int nHyperlink = HitTestHyperlink(pClickArgs->ptClient);
	int mHyperlink = HitTestHyperlink(pControl->m_mouseDownState.ptMouse);
	if (nHyperlink >= 0 && mHyperlink >= 0)
		pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn,
												  XTP_NM_GRID_HYPERLINK, &pClickArgs->ptClient,
												  nHyperlink);

	CMDTARGET_RELEASE(pRow);
	CMDTARGET_RELEASE(pClickArgs->pColumn);
	CMDTARGET_RELEASE(pClickArgs->pControl);
	InternalRelease();
}

int CXTPGridRecordItem::HitTestHyperlink(CPoint ptClick)
{
	int nHitTest = -1;

	int nHyperlinks = GetHyperlinksCount();
	CXTPGridHyperlink* pHyperlink;

	for (int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	{
		pHyperlink = GetHyperlinkAt(nHyperlink);
		ASSERT(pHyperlink);
		if (pHyperlink && pHyperlink->m_rcHyperSpot.PtInRect(ptClick))
		{
			ASSERT(-1 == nHitTest);
			nHitTest = nHyperlink;
		}
	}
	return nHitTest;
}

void CXTPGridRecordItem::OnDblClick(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	if (!pClickArgs || !pClickArgs->pControl)
		return;

	if (OnLButtonDown(pClickArgs)) // old version (10) did not have this call
		return;

	CXTPGridControl* pControl = pClickArgs->pControl;

	if (IsAllowEdit(pClickArgs) && !pControl->IsEditOnClick() && pControl->IsEditOnDoubleClick()
		&& pControl->GetInplaceEdit()->GetItem() != this)
	{
		pControl->EditItem(pClickArgs);

		CPoint pt(pClickArgs->ptClient);
		pControl->ClientToScreen(&pt);

		CXTPGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPGridInplaceEdit,
													  CWnd::FromHandle(WindowFromPoint(pt)));
		if (pEdit && pEdit->GetItem() == this)
		{
			CXTPGridRecordItemEditOptions* pEditOptions = GetEditOptions(pClickArgs->pColumn);

			if (pEditOptions->m_bSelectTextOnEdit)
				pEdit->SetSel(0, -1);
			else
				DoMouseButtonClick();
		}
	}

	pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_DBLCLK,
								  &pClickArgs->ptClient, -1);
}

void CXTPGridRecordItem::OnMouseMove(UINT nFlags, CPoint point, CXTPGridControl* pControl)
{
	int nHyperlink = HitTestHyperlink(point);
	if (nHyperlink >= 0)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return;
	}

	CXTPGridRecordItemControl* pItemControl = HitTest(point);

	if (m_pItemControlHookWnd && m_pFocusedItemControl && (nFlags & MK_LBUTTON))
	{
		pControl->ScreenToClient(&point);
		if (m_bItemControlUnderMouse && !m_pFocusedItemControl->GetRect().PtInRect(point))
		{
			m_pFocusedItemControl->OnMouseLeave(nFlags, point);
			m_bItemControlUnderMouse = FALSE;
		}
		else if (!m_bItemControlUnderMouse && m_pFocusedItemControl->GetRect().PtInRect(point))
		{
			m_pFocusedItemControl->OnMouseEnter(nFlags, point);
			m_bItemControlUnderMouse = TRUE;
		}
		else
			m_pFocusedItemControl->OnMouseMove(nFlags, point);
		if (pControl != NULL && !pControl->IsEditMode())
			pControl->RedrawControl();
		return;
	}

	if (pItemControl)
	{
		if (pItemControl != m_pFocusedItemControl)
		{
			if (m_pFocusedItemControl)
			{
				m_pFocusedItemControl->OnMouseLeave(nFlags, point);
				m_bItemControlUnderMouse = FALSE;
			}
			pItemControl->OnMouseEnter(nFlags, point);
			m_bItemControlUnderMouse = TRUE;
		}
		pItemControl->OnMouseMove(nFlags, point);
		if (pControl != NULL && !pControl->IsEditMode())
			pControl->RedrawControl();
	}
	else
	{
		if (m_pFocusedItemControl)
		{
			m_pFocusedItemControl->OnMouseLeave(nFlags, point);
			m_bItemControlUnderMouse = FALSE;
			if (pControl != NULL && !pControl->IsEditMode())
				pControl->RedrawControl();
		}
	}
	m_pFocusedItemControl = pItemControl;
}

void CXTPGridRecordItem::SetFont(CFont* pFont)
{
	m_pFontCaption = pFont;
}

void CXTPGridRecordItem::SetBold(BOOL bBold)
{
	m_bBoldText = bBold;
}

void CXTPGridRecordItem::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}

COLORREF CXTPGridRecordItem::GetTextColor()
{
	return m_clrText;
}

void CXTPGridRecordItem::SetBackgroundColor(COLORREF clrBackground)
{
	m_clrBackground = clrBackground;
}

COLORREF CXTPGridRecordItem::GetBackgroundColor()
{
	return m_clrBackground;
}

int CXTPGridRecordItem::Compare(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem)
{
	if (!pItem)
		return 0;

	if (m_nSortPriority != -1 || pItem->m_nSortPriority != -1)
		return m_nSortPriority - pItem->m_nSortPriority;

	return m_pRecord->GetRecords()->Compare(GetCaption(pColumn), pItem->GetCaption(pColumn));
}

CFont* CXTPGridRecordItem::GetFont()
{
	return m_pFontCaption;
}

CString CXTPGridRecordItem::GetGroupCaption(CXTPGridColumn* pColumn)
{
	if (!m_strGroupCaption.IsEmpty())
		return m_strGroupCaption;

#ifndef _XTP_ACTIVEX
	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
	{
		CString str;
		if (str.LoadString(XTPToUInt(nID)))
			return str;
	}
#endif

	if (m_pMarkupUIElement || pColumn->GetMarkupUIElement())
		return GetCaption(pColumn);

	return GetColumnCaptionPrefix(pColumn) + GetCaption(pColumn);
}

COleVariant CXTPGridRecordItem::GetGroupCaptionValue(CXTPGridColumn* pColumn)
{
	CString strGroupCaption = GetGroupCaption(pColumn);
	return strGroupCaption.operator LPCTSTR();
}

CString CXTPGridRecordItem::GetColumnCaptionPrefix(CXTPGridColumn* pColumn) const
{
	ASSERT_VALID(pColumn);
	return pColumn->GetCaption() + _T(": ");
}

int CXTPGridRecordItem::CompareGroupCaption(CXTPGridColumn* pColumn, CXTPGridRecordItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT_VALID(pColumn);

	if (m_nGroupPriority != -1)
		return m_nGroupPriority - pItem->m_nGroupPriority;

	if (!m_strGroupCaption.IsEmpty())
		return m_pRecord->GetRecords()->Compare(m_strGroupCaption, pItem->m_strGroupCaption);

	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
		return nID - pItem->GetGroupCaptionID(pColumn);

	return Compare(pColumn, pItem);
}

int CXTPGridRecordItem::CompareGroupCaption(CXTPGridColumn* pColumn, CXTPGridGroupRow* pGroupRow)
{
	ASSERT_VALID(pGroupRow);

	CString strItemGroupCaption = GetGroupCaption(pColumn);
	CString strGroupRowCaption  = pGroupRow->GetCaption();
	return m_pRecord->GetRecords()->Compare(strItemGroupCaption, strGroupRowCaption);
}

int CXTPGridRecordItem::GetGroupCaptionID(CXTPGridColumn* /*pColumn*/)
{
	return m_nGroupPriority;
}

void CXTPGridRecordItem::GetItemMetrics(XTP_GRIDRECORDITEM_DRAWARGS* pDrawArgs,
										XTP_GRIDRECORDITEM_METRICS* pItemMetrics)
{
	if (m_clrBackground != XTP_GRID_COLOR_DEFAULT)
		pItemMetrics->clrBackground = m_clrBackground;

	if (m_clrText != XTP_GRID_COLOR_DEFAULT)
		pItemMetrics->clrForeground = m_clrText;

	if (m_pFontCaption != NULL)
		pItemMetrics->pFont = m_pFontCaption;
	else if (m_bBoldText)
		pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_xtpFontBoldText;

	if (m_Alignment != (XTPGridColumnIconAlignment)(-1))
		pItemMetrics->nColumnAlignment = m_Alignment;
}

int CXTPGridRecordItem::GetIndex() const
{
	return m_pRecord ? m_pRecord->IndexOf(this) : -1;
}

CXTPGridColumn* CXTPGridRecordItem::GetColumn()
{
	if (m_pControl)
	{
		int nItemIndex		   = GetIndex();
		CXTPGridColumns* pCols = m_pControl->GetColumns();
		if (pCols)
			return pCols->Find(nItemIndex);
	}

	return NULL;
}

BOOL CXTPGridRecordItem::IsEditable() const
{
	if (this == NULL)
		return FALSE;

	if (!m_pRecord)
		return FALSE;

	return m_pRecord ? m_pRecord->m_bEditable && m_bEditable && IsFocusable() : FALSE;
}

BOOL CXTPGridRecordItem::IsFocusable() const
{
	if (this == NULL)
		return FALSE;

	return m_bFocusable;
}

BOOL CXTPGridRecordItem::GetCanEditCheckboxItem() const
{
	if (this == NULL)
		return FALSE;

	return m_bCanEditCheckboxItem;
}

void CXTPGridRecordItem::SetCanEditCheckboxItem(BOOL bSet)
{
	m_bCanEditCheckboxItem = bSet;
}

void CXTPGridRecordItem::OnBeginEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(pItemArgs);
	if (!pItemArgs)
		return;

	if (IsEditable() && OnRequestEdit(pItemArgs))
	{
		CXTPGridControl* pControl  = pItemArgs->pControl;
		CXTPGridInplaceEdit* pEdit = pControl->GetInplaceEdit();
		CRect rcCaption(pItemArgs->rcItem);
		GetCaptionRect(pItemArgs, rcCaption);

		CXTPGridRecordItemEditOptions* pEditOptions = GetEditOptions(pItemArgs->pColumn);
		ASSERT(pEditOptions);

		CXTPGridInplaceButtons* pInpaceButtons = &pEditOptions->arrInplaceButtons;

		CRect rcButtons(pItemArgs->rcItem);
		CXTPClientRect rcControl(pItemArgs->pControl);

		int i;
		// calculate right-aligned item controls width
		int nControlsWidthRight = 0;
		if (m_pItemControls && m_pItemControls->GetSize() > 0)
		{
			for (i = 0; m_pItemControls && i < m_pItemControls->GetSize(); i++)
			{
				CXTPGridRecordItemControl* pItemControl = m_pItemControls->GetAt(i);

				if (pItemControl->GetAlignment() == xtpItemControlRight)
					nControlsWidthRight += pItemControl->GetRect().Width();
				else
					pItemArgs->rcItem.left += pItemControl->GetRect().Width();
			}
		}
		// calculate inplace buttons widths
		int nButtonsWidthIn = 0, nButtonsWidthOut = 0;
		for (i = 0; i < pInpaceButtons->GetSize(); i++)
		{
			CXTPGridInplaceButton* pButton = pInpaceButtons->GetAt(i);
			if (pButton->IsInsideCellButton())
				nButtonsWidthIn += pButton->GetWidth();
			else
				nButtonsWidthOut += pButton->GetWidth();
		}

		// adjust buttons rect
		if (rcButtons.right + nButtonsWidthOut > rcControl.right)
			rcButtons.right = rcControl.right - nButtonsWidthOut;

		rcButtons.left = min(rcButtons.right, pItemArgs->rcItem.right - nControlsWidthRight);

		// create buttons
		for (i = 0; i < pInpaceButtons->GetSize(); i++)
		{
			CXTPGridInplaceButton* pButton = pInpaceButtons->GetAt(i);
			pButton->Create(pItemArgs, rcButtons);

			pControl->GetInplaceButtons()->Add(pButton);
		}

		// adjust item rect
		pItemArgs->rcItem.right = rcButtons.left;

		if (IsAllowEdit(pItemArgs))
		{
			if (m_bHasCheckbox && !m_bCanEditCheckboxItem)
				return;
			else
				pEdit->Create(pItemArgs);
		}

		// Expand if expansion is enabled and button type is expandable.
		if (0 < pInpaceButtons->GetSize() && pEditOptions->m_bExpandOnSelect)
		{
			CXTPGridInplaceButton* pButton = pInpaceButtons->GetAt(0);
			if (XTP_ID_GRID_EXPANDBUTTON == pButton->GetID()
				|| XTP_ID_GRID_COMBOBUTTON == pButton->GetID())
			{
				OnInplaceButtonDown(pButton);
				return;
			}
		}

		XTP_NM_GRIDRECORDITEM nm;
		::ZeroMemory(&nm, sizeof(nm));

		nm.pItem   = this;
		nm.pRow	= pItemArgs->pRow;
		nm.pColumn = pItemArgs->pColumn;

		pControl->SendNotifyMessage(XTP_NM_GRID_BEGINEDIT, (NMHDR*)&nm);
	}
}

void CXTPGridRecordItem::OnCancelEdit(CXTPGridControl* pControl, BOOL bApply)
{
	if (NULL == pControl)
		return;

	CXTPGridInplaceButtons* pInpaceButtons = pControl->GetInplaceButtons();

	for (int i = 0; i < pInpaceButtons->GetSize(); i++)
	{
		CXTPGridInplaceButton* pButton = pInpaceButtons->GetAt(i);
		if (pButton->GetSafeHwnd())
			pButton->DestroyWindow();

		if (pButton)
			pButton->SetItemArgs(0);
	}

	pInpaceButtons->RemoveAll();

	CXTPGridInplaceList* pInpaceList = pControl->GetInplaceList();
	if (pInpaceList->GetSafeHwnd())
		pInpaceList->DestroyWindow();

	CXTPGridInplaceEdit* pEdit = pControl->GetInplaceEdit();
	if (pEdit == NULL)
		return;

	if (pEdit->GetSafeHwnd())
	{
		if (bApply && pEdit->GetItem() == this)
			OnValidateEdit((XTP_GRIDRECORDITEM_ARGS*)pEdit);

		pEdit->HideWindow();
		pEdit->SetItemArgs(0);
		pEdit->DestroyWindow();
	}
}

void CXTPGridRecordItem::OnConstraintChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs,
											 CXTPGridRecordItemConstraint* pConstraint)
{
	OnEditChanged(pItemArgs, pConstraint->m_strConstraint);
}

BOOL CXTPGridRecordItem::OnEditChanging(XTP_GRIDRECORDITEM_ARGS* pItemArgs, CString& rstrNewText)
{
	UNREFERENCED_PARAMETER(pItemArgs);
	UNREFERENCED_PARAMETER(rstrNewText);
	return TRUE;
}

void CXTPGridRecordItem::OnEditChanged(XTP_GRIDRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	if (pItemArgs)
	{
		int r				 = pItemArgs->pRow->GetIndex();
		int i				 = pItemArgs->pItem->GetIndex();
		CXTPGridRecord* pRec = pItemArgs->pRow->GetRecord();
		if (pRec)
			pRec->UpdateRecordField(r, i, szText);
	}
}

void CXTPGridRecordItem::OnValidateEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(pItemArgs);
	if (!pItemArgs)
		return;

	CXTPGridControl* pControl	= pItemArgs->pControl;
	CXTPGridInplaceEdit* pEdit   = pControl->GetInplaceEdit();
	XTP_GRIDRECORDITEM_ARGS args = *pItemArgs;

	if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
	{
		args.AddRef();

		BOOL bRedraw = FALSE;
		CMDTARGET_RELEASE(pEdit->pItem); // pEdit->pItem = NULL;

		if (GetEditOptions(args.pColumn)->m_bConstraintEdit)
		{
			if (pEdit->m_pSelectedConstraint)
			{
				OnConstraintChanged(&args, pEdit->m_pSelectedConstraint);
				bRedraw = TRUE;
			}
		}
		else
		{
			CString strValue;
			pEdit->GetWindowText(strValue);

			CString sOldValue = GetCaption(args.pColumn);
			if (sOldValue != strValue || pControl->IsVirtualMode())
			{
				OnEditChanged(&args, strValue);
				bRedraw = TRUE;
			}
			// else if (GetCaption(args.pColumn) != strValue) //<<>>
			//{
			//  OnEditChanged(&args, strValue);
			//  bRedraw = TRUE;
			//}
		}
		if (bRedraw)
		{
			pControl->RedrawControl();
			pControl->SendMessageToParent(args.pRow, this, args.pColumn, XTP_NM_GRID_VALUECHANGED,
										  0);
		}
		else
		{
			OnEditCanceled(pItemArgs);
			pControl->SendMessageToParent(args.pRow, this, args.pColumn, XTP_NM_GRID_EDIT_CANCELED,
										  0);
		}

		pEdit->SetItemArgs(0);

		args.Release();
	}
}

BOOL CXTPGridRecordItem::IsAllowEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	BOOL bIsAllowEdit = pItemArgs->pControl->IsAllowEdit();

	if (m_pRecord)
	{
		// Header record
		if (m_pRecord->m_pRecords == pItemArgs->pControl->GetHeaderRecords())
		{
			bIsAllowEdit = pItemArgs->pControl->IsHeaderRowsAllowEdit();
		}
		// Footer record
		else if (m_pRecord->m_pRecords == pItemArgs->pControl->GetFooterRecords())
		{
			bIsAllowEdit = pItemArgs->pControl->IsFooterRowsAllowEdit();
		}
	}

	bIsAllowEdit = bIsAllowEdit && IsEditable();

	if (pItemArgs->pColumn)
	{
		bIsAllowEdit = bIsAllowEdit && pItemArgs->pColumn->IsEditable();
		if (NULL != pItemArgs->pColumn->GetEditOptions())
		{
			bIsAllowEdit = bIsAllowEdit && pItemArgs->pColumn->GetEditOptions()->m_bAllowEdit;
		}
	}

	if (m_pEditOptions)
	{
		bIsAllowEdit = bIsAllowEdit && m_pEditOptions->m_bAllowEdit;
	}

	return bIsAllowEdit;
}

BOOL CXTPGridRecordItem::OnRequestEdit(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	CXTPGridControl* pControl = pItemArgs->pControl;

	XTP_NM_GRIDREQUESTEDIT nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.bCancel = FALSE;
	nm.pItem   = this;
	nm.pRow	= pItemArgs->pRow;
	nm.pColumn = pItemArgs->pColumn;

	pControl->SendNotifyMessage(XTP_NM_GRID_REQUESTEDIT, (NMHDR*)&nm);

	return !nm.bCancel;
}

BOOL CXTPGridRecordItem::OnChar(XTP_GRIDRECORDITEM_ARGS* pItemArgs, UINT nChar)
{
	CXTPGridControl* pControl = pItemArgs->pControl;

	if ((nChar == VK_SPACE) && IsEditable() && m_bHasCheckbox
		&& (pItemArgs->pColumn == NULL || pItemArgs->pColumn->IsEditable())
		&& OnRequestEdit(pItemArgs))
	{
		if (pControl->IsAutoCheckItems())
		{
			if (!m_bTristateCheckbox)
				SetChecked(!IsChecked());
			else
				SetCheckedState((GetCheckedState() + 1) % 3);
		}
		pControl->RedrawControl();

		pControl->SendMessageToParent(pItemArgs->pRow, this, pItemArgs->pColumn,
									  XTP_NM_GRID_CHECKED, NULL);
		return TRUE;
	}

	if (IsAllowEdit(pItemArgs))
	{
		// ^^ causes CXTPGridRecordItem::OnChar do be called twice
		BOOL bSameItem = pItemArgs->pItem == pControl->GetActiveItem();

		if (!bSameItem)
		{
			pControl->EditItem(pItemArgs);
		}

		CXTPGridInplaceEdit* pEdit = pControl->GetInplaceEdit();

		if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
		{
			if (!bSameItem)
			{
				pEdit->SetFocus();
				pEdit->SetSel(0, -1);
			}
			if (VK_TAB != nChar)
			{
				pEdit->SendMessage(WM_CHAR, nChar);
			}
		}
		return TRUE;
	}
	return FALSE;
}

CXTPGridRecordItemEditOptions* CXTPGridRecordItem::GetEditOptions(CXTPGridColumn* pColumn)
{
	if (m_pEditOptions)
		return m_pEditOptions;

	if (pColumn)
		return pColumn->GetEditOptions();

	return m_pEditOptions = new CXTPGridRecordItemEditOptions();
}

CXTPGridRecordItemControls* CXTPGridRecordItem::GetItemControls()
{
	if (!m_pItemControls)
		m_pItemControls = new CXTPGridRecordItemControls(this);
	return m_pItemControls;
}

void CXTPGridRecordItem::OnInplaceButtonDown(CXTPGridInplaceButton* pButton)
{
	CXTPGridControl* pControl = pButton->pControl;

	XTP_NM_GRIDINPLACEBUTTON nm;
	::ZeroMemory(&nm, sizeof(nm));

	nm.pButton = pButton;
	nm.pItem   = this;

	if (pControl->SendNotifyMessage(XTP_NM_GRID_INPLACEBUTTONDOWN, (NMHDR*)&nm) == TRUE)
		return;

	if (pButton->GetID() == XTP_ID_GRID_COMBOBUTTON)
	{
		CXTPGridInplaceList* pList = pControl->GetInplaceList();

		XTP_GRIDRECORDITEM_ARGS itemArgs = *pButton;
		if (!itemArgs.pColumn && !itemArgs.pControl && !itemArgs.pItem && !itemArgs.pRow)
			return;
		ASSERT(itemArgs.pItem == this);

		CXTPWindowRect rcButton(pButton);
		pControl->ScreenToClient(&rcButton);
		itemArgs.rcItem.right  = rcButton.right;
		itemArgs.rcItem.bottom = rcButton.bottom;

		CXTPGridRecordItemEditOptions* pEditOptions = GetEditOptions(itemArgs.pColumn);
		pList->SetDropListItemNumber(pButton->GetDropListItemNumber());
		if (pEditOptions->GetConstraints()->GetCount() > 0)
			pList->Create(&itemArgs, pEditOptions->GetConstraints());
	}
	else if (pButton->GetID() == XTP_ID_GRID_SPINBUTTON)
	{
		XTP_GRIDRECORDITEM_ARGS itemArgs = *pButton;
		ASSERT(itemArgs.pItem == this);
		CXTPGridInplaceEdit* pEdit = pControl->GetInplaceEdit();
		if (itemArgs.pRow && itemArgs.pColumn && pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
		{
			CString strValue;
			pEdit->GetWindowText(strValue);

			int nValue = _ttoi(strValue);

			int nValue2 = nValue + pButton->m_nSpinIncrement;
			nValue2		= max(nValue2, pButton->m_nSpinMin);
			nValue2		= min(nValue2, pButton->m_nSpinMax);

			if (nValue2 != nValue)
			{
				strValue.Format(_T("%d"), nValue2);

				BOOL bCommit = OnEditChanging(&itemArgs, strValue);
				if (bCommit)
					pEdit->SetWindowText(strValue);

				pControl->RedrawControl();
				// pControl->SendMessageToParent(itemArgs.pRow, this, itemArgs.pColumn,
				// XTP_NM_GRID_VALUECHANGED, 0);
			}
		}
	}
}

CXTPGridHyperlinks* CXTPGridRecordItem::GetHyperlinks()
{
	if (!m_pHyperlinks)
		m_pHyperlinks = new CXTPGridHyperlinks();

	return m_pHyperlinks;
}

void CXTPGridRecordItem::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_DWord(pPX, _T("TextColor"), (DWORD&)m_clrText, XTP_GRID_COLOR_DEFAULT);
	PX_DWord(pPX, _T("BackgroundColor"), (DWORD&)m_clrBackground, XTP_GRID_COLOR_DEFAULT);

	PX_Bool(pPX, _T("BoldText"), m_bBoldText, FALSE);

	PX_String(pPX, _T("Format"), m_strFormatString, _T("%s"));

	PX_Bool(pPX, _T("Editable"), m_bEditable, TRUE);

	PX_Int(pPX, _T("IconIndex"), m_nIconIndex, XTP_GRID_NOICON);

	PX_Int(pPX, _T("SortPriority"), m_nSortPriority, -1);
	PX_Int(pPX, _T("GroupPriority"), m_nGroupPriority, -1);

	PX_String(pPX, _T("GroupCaption"), m_strGroupCaption, _T(""));

	PX_Bool(pPX, _T("Focusable"), m_bFocusable, TRUE);

	PX_Int(pPX, _T("Checked"), m_checkState, 0);
	PX_Bool(pPX, _T("HasCheckbox"), m_bHasCheckbox, FALSE);
	PX_Bool(pPX, _T("TristateCheckbox"), m_bTristateCheckbox, FALSE);

	ULONGLONG ullData = m_dwData;
	PX_UI8(pPX, _T("Data"), ullData, 0);
	m_dwData = (DWORD_PTR)ullData;

	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));

	if (pPX->GetSchema() > _XTP_SCHEMA_98)
	{
		PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	}

#ifdef _XTP_ACTIVEX
	if (pPX->GetSchema() > _XTP_SCHEMA_1040)
	{
		COleVariant varDefault(_T(""));
		PX_Variant(pPX, _T("Tag"), m_oleTag, varDefault);
	}
#endif

	// Hyperlinks
	CXTPPropExchangeSection secHyperlinks(pPX->GetSection(_T("Hyperlinks")));

	CXTPGridHyperlinks arHyperlinks;

	if (pPX->IsStoring() && m_pHyperlinks)
	{
		arHyperlinks.CopyFrom(m_pHyperlinks);
	}

	arHyperlinks.DoPropExchange(&secHyperlinks);

	if (pPX->IsLoading())
	{
		if (arHyperlinks.GetSize())
		{
			if (GetHyperlinks())
				GetHyperlinks()->CopyFrom(&arHyperlinks);
		}
		else if (m_pHyperlinks)
		{
			m_pHyperlinks->RemoveAll();
		}
	}

	// Item controls
	CXTPGridRecordItemControls arItemControls;
	if (pPX->IsStoring() && m_pItemControls)
		arItemControls.CopyFrom(m_pItemControls);

	if (pPX->GetSchema() >= _XTP_SCHEMA_1200)
	{
		CXTPPropExchangeSection secItemControls(pPX->GetSection(_T("ItemControls")));
		arItemControls.DoPropExchange(&secItemControls);
	}
	if (pPX->IsLoading())
	{
		if (arItemControls.GetSize())
		{
			if (GetItemControls())
				GetItemControls()->CopyFrom(&arItemControls);
		}
		else if (m_pItemControls)
		{
			m_pItemControls->RemoveAll();
		}
	}

	// Update Markup data
	if (pPX->IsLoading())
	{
		ResetMarkupUIElement();
	}
}

INT_PTR CXTPGridRecordItem::OnToolHitTest(CPoint /*point*/, TOOLINFO* /*pTI*/)
{
	return -1;
}

void CXTPGridRecordItem::Merge(CXTPGridRecordItem* pMergeItem)
{
	m_pMergeItem = pMergeItem;
}

CXTPGridRecordItem* CXTPGridRecordItem::GetMergeItem() const
{
	if (m_pControl == NULL || !m_pControl->IsCellMergingAllowed() || m_pControl->IsVirtualMode())
		return NULL;

	return m_pMergeItem;
}

BOOL CXTPGridRecordItem::IsMerged() const
{
	return (NULL != GetMergeItem());
}

BOOL CXTPGridRecordItem::IsChildOfMerge() const
{
	return IsMerged() && GetMergeItem() != this;
}

int CXTPGridRecordItem::AddHyperlink(CXTPGridHyperlink* pHyperlink)
{
	return GetHyperlinks() ? (int)GetHyperlinks()->Add(pHyperlink) : -1;
}

int CXTPGridRecordItem::GetHyperlinksCount() const
{
	return m_pHyperlinks ? (int)m_pHyperlinks->GetSize() : 0;
}

CXTPGridHyperlink* CXTPGridRecordItem::GetHyperlinkAt(int nHyperlink) const
{
	return m_pHyperlinks ? m_pHyperlinks->GetAt(nHyperlink) : NULL;
}

void CXTPGridRecordItem::RemoveHyperlinkAt(int nHyperlink)
{
	if (m_pHyperlinks)
		m_pHyperlinks->RemoveAt(nHyperlink);
}

BOOL CXTPGridRecordItem::CreateBorder()
{
	if (NULL == m_pBorder)
	{
		m_pBorder = new CXTPGridBorder();
	}

	return (NULL != m_pBorder);
}

BOOL CXTPGridRecordItem::DestroyBorder()
{
	if (NULL != m_pBorder)
	{
		delete m_pBorder;
		m_pBorder = NULL;
	}

	return (NULL == m_pBorder);
}

CXTPGridBorder* CXTPGridRecordItem::GetBorder() const
{
	return m_pBorder;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecordItem, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Bold", 1, m_bBoldText, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Format", 2, OleGetFormat, OleSetFormat, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "ForeColor", 4, m_clrText, VT_COLOR)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "BackColor", 5, m_clrBackground, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Index", 6, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "HasCheckbox", 7, m_bHasCheckbox, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "CanEditCheckboxItem", 33, m_bCanEditCheckboxItem, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Checked", 8, m_checkState, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Icon", 9, m_nIconIndex, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Caption", 10, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "GroupCaption", 11, OleGetGroupCaption,
						OleSetGroupCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "SortPriority", 12, m_nSortPriority, VT_I4)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "GroupPriority", 13, m_nGroupPriority, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Record", 14, OleGetRecord, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Focusable", 15, m_bFocusable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Editable", 16, m_bEditable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "EditOptions", 17, OleGetEditOptions, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridRecordItem, "CreateEditOptions", 18, OleCreateEditOptions, VT_EMPTY,
					 VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Tooltip", 19, OleGetTooltip, OleSetTooltip, VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Tag", 20, m_oleTag, VT_VARIANT)
	DISP_FUNCTION_ID(CXTPGridRecordItem, "Hyperlinks", 21, OleGetHyperlinks, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "Alignment", 22, m_Alignment, VT_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItem, "ItemControls", 23, OleGetItemControls, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecordItem, "MarkupUIElement", 27, OleGetMarkupUIElement, VT_DISPATCH,
					 VTS_NONE)
	DISP_PROPERTY_ID(CXTPGridRecordItem, "TristateCheckbox", 24, m_bTristateCheckbox, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "CheckboxState", 25, GetCheckedState, SetCheckedState,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItem, "Formula", 26, OleGetFormula, OleSetFormula, VT_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItem, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItem, XTPDIID_IGridRecordItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItem, XTPDIID_IGridRecordItem)

LPDISPATCH CXTPGridRecordItem::OleGetRecord()
{
	return m_pRecord->GetIDispatch(TRUE);
}
LPDISPATCH CXTPGridRecordItem::OleGetEditOptions()
{
	return m_pEditOptions ? m_pEditOptions->GetIDispatch(TRUE) : NULL;
}
void CXTPGridRecordItem::OleCreateEditOptions()
{
	if (m_pEditOptions == NULL)
		m_pEditOptions = new CXTPGridRecordItemEditOptions();
}

LPDISPATCH CXTPGridRecordItem::OleGetHyperlinks()
{
	return GetHyperlinks() ? GetHyperlinks()->GetIDispatch(TRUE) : NULL;
}

BSTR CXTPGridRecordItem::OleGetFormat()
{
	return m_strFormatString.AllocSysString();
}

void CXTPGridRecordItem::OleSetFormat(LPCTSTR pcszFormat)
{
	m_strFormatString = pcszFormat;
}

BSTR CXTPGridRecordItem::OleGetFormula()
{
	return m_strFormula.AllocSysString();
}

void CXTPGridRecordItem::OleSetFormula(LPCTSTR pcszFormula)
{
	m_strFormula = pcszFormula;
}

BSTR CXTPGridRecordItem::OleGetCaption()
{
	CString sCaption = GetCaption();
	return sCaption.AllocSysString();
}

void CXTPGridRecordItem::OleSetCaption(LPCTSTR pcszCaption)
{
	SetCaption(pcszCaption);
}

BSTR CXTPGridRecordItem::OleGetGroupCaption()
{
	return m_strGroupCaption.AllocSysString();
}

void CXTPGridRecordItem::OleSetGroupCaption(LPCTSTR pcszGroupCaption)
{
	m_strGroupCaption = pcszGroupCaption;
}

BSTR CXTPGridRecordItem::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}

void CXTPGridRecordItem::OleSetTooltip(LPCTSTR pcszTooltip)
{
	SetTooltip(pcszTooltip);
}

LPDISPATCH CXTPGridRecordItem::OleGetItemControls()
{
	CXTPGridRecordItemControls* pItemControls = GetItemControls();
	return pItemControls ? pItemControls->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPGridRecordItem::OleGetMarkupUIElement()
{
	CXTPMarkupUIElement* pMarkupUIElement = GetMarkupUIElement();
	return pMarkupUIElement ? pMarkupUIElement->GetIDispatch(TRUE) : NULL;
}

//////////////////////////////////////////////////////////////////////////
// XTP_GRIDRECORDITEM_METRICS

BEGIN_DISPATCH_MAP(XTP_GRIDRECORDITEM_METRICS, CXTPCmdTarget)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "BackColor", 1, clrBackground, VT_COLOR)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "ForeColor", 2, clrForeground, VT_COLOR)
	DISP_PROPERTY_EX_ID(XTP_GRIDRECORDITEM_METRICS, "Font", 3, OleGetFont, OleSetFont, VT_FONT)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "Text", 4, strText, VT_BSTR)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "GroupRowIcon", 5, nGroupRowIcon, VT_I4)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "GroupRowIconAlignment", 6, nGroupRowIconAlignment,
					 VT_I4)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "ColumnAlignment", 7, nColumnAlignment, VT_I4)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "Icon", 8, nItemIcon, VT_I4)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "VirtualRowLevel", 9, nVirtRowLevel, VT_I4)
	DISP_PROPERTY_ID(XTP_GRIDRECORDITEM_METRICS, "VirtualRowFlags", 10, nVirtRowFlags, VT_I4)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(XTP_GRIDRECORDITEM_METRICS, CXTPCmdTarget)
	INTERFACE_PART(XTP_GRIDRECORDITEM_METRICS, XTPDIID_IGridRecordItemMetrics, Dispatch)
END_INTERFACE_MAP()

LPFONTDISP XTP_GRIDRECORDITEM_METRICS::OleGetFont()
{
	return AxCreateOleFont(m_xtpFontMetrics.GetSafeHandle() ? &m_xtpFontMetrics : pFont, this,
						   (LPFNFONTCHANGED)&XTP_GRIDRECORDITEM_METRICS::OleSetFont);
}

void XTP_GRIDRECORDITEM_METRICS::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_xtpFontMetrics.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_xtpFontMetrics.CreateFontIndirect(&lf);
		pFont = &m_xtpFontMetrics;
	}
}

#endif
