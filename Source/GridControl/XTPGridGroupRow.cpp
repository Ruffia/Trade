// XTPGridGroupRow.cpp : implementation of the CXTPGridGroupRow class.
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
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridGroupRow.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridGroupItem

IMPLEMENT_DYNAMIC(CXTPGridGroupRow, CXTPGridRow);

CXTPGridGroupRow::CXTPGridGroupRow()
	: m_pMarkupUIElement(NULL)
	, m_lpCurrencyFmt(NULL)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPGridGroupRow::~CXTPGridGroupRow()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPGridGroupRow::SetCaption(LPCTSTR lpszCaption)
{
	CString strGroupCaption(lpszCaption);

#ifdef _DEBUG
	if (strGroupCaption == _T("x"))
	{
		TRACE(_T("WARNING: Using 'x' as a caption value to re-calculate formula value has been ")
			  _T("deprecated. Use UpdateValue method instead.\n"));
	}
#endif

	if (m_strFormula.IsEmpty())
	{
		if (m_strGroupText == strGroupCaption)
			return;

		m_strGroupText = strGroupCaption;
		if (m_strGroupLabel.IsEmpty())
			m_strGroupLabel = m_strGroupText;
		m_vtCaptionValue = COleVariant();

		ResetMarkupUIElement();
	}
	else
	{
		CString strCaption(strGroupCaption);
		double d(0.0);
		int m(0), M(0), n(0), N(0);
		CString s, t, u, v;
		CString sFormula(m_strFormula);
		sFormula.Replace(_T("(C"), _T("(R*C"));
		sFormula.Replace(_T(":C"), _T("(:R*C"));
		int pos = sFormula.Find(_T("SUMSUB("));
		int nextPos(-1);
		// Simplified format for group row formulas - SUMSUB(C#:C#) as R# ignored and dynamically
		// recreated For prev ver formulas - clean R# parts For multiple formulas case can be
		// SUMSUB(R#Ci1:R#Cl1) SUMSUB(R#Ci2:R#Cl2) ... SUMSUB(R#Cim:R#Clm) ROW range ignored as used
		// dynamic based on group row childs Space-separated string split between spaces and Instead
		// "if (pos > -1)" can use loop "while (pos > -1)" if (pos > -1)
		while (pos > -1)
		{
			CString sNextFormula, strCapt, strCol;
			if (sFormula.GetLength() > pos + 7)
				nextPos = sFormula.Find(_T("SUMSUB("), pos + 7);
			if (nextPos > -1)
				sNextFormula = sFormula.Mid(nextPos);

			if (GetChilds(FALSE) != NULL)
			{
				int mMax = m_pControl->GetRows()->GetCount();
				int nMax = m_pControl->GetColumns()->GetCount();
				// CODE FOR RECALC - pattern SUMSUB(R#C#:R#C#) or * instead of # - means all row or
				// column ROW range ignored as used dynamic based on group row childs
				sFormula.Replace(_T(" "), _T(""));
				if (sFormula.GetLength() > pos + 7)
					sFormula = sFormula.Mid(pos + 7);
				pos = sFormula.Find(_T(":"));
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

							if (n == N - 1)
								d = 0;
							CalculateByChilds(this, n, N, d);

							CString sFmt = GetFormatString();
							if (m_lpCurrencyFmt)
							{
								strCapt = sFmt + GetCurrencyString(d);
							}
							else
							{
								if (sFmt.IsEmpty() || sFmt == _T("%s"))
									sFmt = _T("%f");
								else if (sFmt == _T("%d"))
									sFmt = _T("%.0f");

								strCapt.Format(sFmt, d);
							}

							strCol.Format(_T(" [%d]"), n);
							strCapt = strCol + strCapt;
						}
					}
				}
			}
			if (nextPos > -1)
			{
				sFormula = sNextFormula;
				pos		 = sFormula.Find(_T("SUMSUB("));
				sNextFormula.Empty();
			}
			strCaption += strCapt;
		}
		if (m_strGroupText == strCaption)
			return;

		m_strGroupText   = m_strGroupLabel + strCaption;
		m_vtCaptionValue = COleVariant();

		ResetMarkupUIElement();
	}
}

void CXTPGridGroupRow::UpdateValue()
{
	if (!m_strFormula.IsEmpty())
		SetCaption(_T(""));
}

void CXTPGridGroupRow::SetCaption(LPCTSTR lpszCaption, COleVariant vtValue)
{
	SetCaption(lpszCaption);
	m_vtCaptionValue = vtValue;
}

void CXTPGridGroupRow::ResetMarkupUIElement()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (m_pControl && (m_pControl->GetMarkupContext()))
		m_pMarkupUIElement = XTPMarkupParseText(m_pControl->GetMarkupContext(), m_strGroupText);
}

BOOL CXTPGridGroupRow::CalculateByChilds(CXTPGridRow* pPassedRow, int col_start, int col_end,
										 double& dPassedValue)
{
	BOOL bRet = FALSE;

	CXTPGridRow* pRow		  = NULL;
	CXTPGridRecord* pRec	  = NULL;
	CXTPGridRecordItem* pItem = NULL;

	for (int row = 0; row < pPassedRow->GetChilds(FALSE)->GetCount(); row++)
	{
		pRow = pPassedRow->GetChilds(FALSE)->GetAt(row);
		if (pRow && !pRow->IsGroupRow())
		{
			pRec = pRow->GetRecord();
			if (pRec)
			{
				bRet = TRUE;
				for (int col = col_start; col < col_end; col++)
				{
					if (col < pRec->GetItemCount())
					{
						pItem = pRec->GetItem(col);
						if (pItem)
						{
							CString s = pItem->GetCaption(NULL);
							dPassedValue += pItem->StringToDouble(s);
						}
					}
				}
			}
		}
		else if (pRow && pRow->IsGroupRow())
		{
			bRet = CalculateByChilds(pRow, col_start, col_end, dPassedValue);
		}
	}
	return bRet;
}

CString CXTPGridGroupRow::GetCaption() const
{
	return m_strGroupText;
}

COleVariant CXTPGridGroupRow::GetCaptionValue() const
{
	return m_vtCaptionValue;
}

void CXTPGridGroupRow::Draw(CDC* pDC, CRect rcRow, CRect rcClip, int nLeftOffset,
							CXTPGridRecordMergeItems& /*mergeItems*/, int nColumnFrom,
							int nColumnTo)
{
	UNREFERENCED_PARAMETER(rcClip);
	UNREFERENCED_PARAMETER(nColumnFrom);
	UNREFERENCED_PARAMETER(nColumnTo);

	CRgn rgnClip;
	if (!pDC->IsPrinting())
	{
		rgnClip.CreateRectRgnIndirect(&rcClip);
		pDC->SelectClipRgn(&rgnClip, RGN_COPY);
	}

	CXTPGridPaintManager* pPaintManager = m_pControl->GetPaintManager();

	pDC->SetBkMode(TRANSPARENT);

	m_rcRow = rcRow;
	if (0 == GetControl()->GetFreezeColumnsCount())
	{
		m_rcRow.left -= nLeftOffset;
		m_rcRow.right -= nLeftOffset;
	}

	XTP_GRIDRECORDITEM_DRAWARGS drawArgs;
	drawArgs.pDC		= pDC;
	drawArgs.nTextAlign = DT_LEFT;
	drawArgs.pControl   = m_pControl;
	drawArgs.pColumn	= NULL;
	drawArgs.pRow		= this;
	drawArgs.pItem		= NULL;
	drawArgs.rcItem		= m_rcRow;

	XTP_GRIDRECORDITEM_METRICS* pDrawMetrics = new XTP_GRIDRECORDITEM_METRICS;
	pDrawMetrics->strText					 = GetCaption();

	pPaintManager->FillGroupRowMetrics(this, pDrawMetrics, pDC->IsPrinting());

	ASSERT(m_pControl);
	if (m_pControl)
		m_pControl->GetItemMetrics(&drawArgs, pDrawMetrics);

	pPaintManager->DrawGroupRow(pDC, this, m_rcRow, pDrawMetrics);

	pDrawMetrics->InternalRelease();

	if (!pDC->IsPrinting())
	{
		pDC->SelectClipRgn(NULL, RGN_COPY);
		rgnClip.DeleteObject();
	}
}

void CXTPGridGroupRow::OnClick(CPoint ptClicked)
{
	// expand/collapse on single click at the collapse bitmap
	if (m_rcCollapse.PtInRect(m_pControl->m_mouseDownState.ptMouse)
		&& m_rcCollapse.PtInRect(ptClicked) && !IsLockExpand())
	{
		BOOL bExpanded = IsExpanded();

		SetExpanded(!bExpanded);

		const BOOL bKeyControl = (::GetKeyState(VK_CONTROL) < 0)
								 || m_pControl->IsMultiSelectionMode();
		const BOOL bKeyShift = (::GetKeyState(VK_SHIFT) < 0);

		if ((bKeyControl || bKeyShift) && bExpanded && m_pControl->m_bSelectionExcludeGroupRows)
		{
			m_pControl->UnselectGroupRows();
		}
	}
}

void CXTPGridGroupRow::OnDblClick(CPoint ptClicked)
{
	// do not expand if double clicked on the collapse bitmap - different action (one level only)
	if (!m_rcCollapse.PtInRect(ptClicked) && !IsLockExpand())
	{
		SetExpanded(!IsExpanded());
	}
	// process parent
	CXTPGridRow::OnDblClick(ptClicked);
}

INT_PTR CXTPGridGroupRow::OnToolHitTest(CPoint point, TOOLINFO* pToolInfo)
{
	UNREFERENCED_PARAMETER(point);

	INT_PTR nHit   = (INT_PTR)this;
	CString strTip = GetTooltip();

	if (strTip.IsEmpty())
		return -1;

	// Clean markup
	CString s(strTip), u, v;
	int j = s.Find(_T("<"));
	int k = s.Find(_T(">"));
	// Prevent from infinite loop when strTip contain a ">" and then a "<" character
	while (j > -1 && k > -1 && j < k)
	{
		u	  = s.Left(j);
		v	  = s.Mid(k + 1);
		s	  = u + v;
		strTip = s;
		j	  = s.Find(_T("<"));
		k	  = s.Find(_T(">"));
	}

	CXTPToolTipContext::FillInToolInfo(pToolInfo, m_pControl->GetSafeHwnd(), m_rcRow, nHit, strTip);

	return nHit;
}

CString CXTPGridGroupRow::GetCurrencyString(double value)
{
	if (!m_lpCurrencyFmt)
		return _T("");

	CString in, out;
	in.Format(_T("%02f"), value);

	int len = GetCurrencyFormat(LOCALE_USER_DEFAULT, 0, in, m_lpCurrencyFmt, 0, 0);

	LPTSTR lpBuffer = out.GetBufferSetLength(len);
	GetCurrencyFormat(LOCALE_USER_DEFAULT, 0, in, m_lpCurrencyFmt, lpBuffer, len);
	out.ReleaseBuffer();

	return out;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridGroupRow, CXTPGridRow)
	DISP_PROPERTY_EX_ID(CXTPGridGroupRow, "GroupCaption", 100, OleGetCaption, OleSetCaption,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPGridGroupRow, "GroupFormula", 102, OleGetFormula, OleSetFormula,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPGridGroupRow, "GroupFormat", 103, OleGetFormat, OleSetFormat, VT_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridGroupRow, CXTPGridRow)
	INTERFACE_PART(CXTPGridGroupRow, XTPDIID_IGridGroupRow, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridGroupRow, XTPDIID_IGridGroupRow)

BSTR CXTPGridGroupRow::OleGetCaption()
{
	return m_strGroupText.AllocSysString();
}

void CXTPGridGroupRow::OleSetCaption(LPCTSTR pcszCaption)
{
	SetCaption(pcszCaption);
}

BSTR CXTPGridGroupRow::OleGetFormat()
{
	return m_strFormat.AllocSysString();
}

void CXTPGridGroupRow::OleSetFormat(LPCTSTR pcszFormat)
{
	m_strFormat = CString(pcszFormat);
}

BSTR CXTPGridGroupRow::OleGetFormula()
{
	return m_strFormula.AllocSysString();
}

void CXTPGridGroupRow::OleSetFormula(LPCTSTR pcszFormula)
{
	m_strFormula = CString(pcszFormula);
}

#endif
