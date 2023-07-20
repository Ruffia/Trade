// XTPGridInplaceEdit.cpp
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPGdiObjects.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridInplaceControls.h"
#include "ItemTypes/XTPGridRecordItemText.h"
#include "GridControl/XTPGridRecordItemConstraint.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridColumn.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPGridInplaceEdit, CEdit)

CXTPGridInplaceEdit::CXTPGridInplaceEdit()
{
	m_pSelectedConstraint = NULL;
	m_clrText			  = 0;
	m_clrBack			  = RGB(255, 255, 255);
	m_bSetWindowText	  = FALSE;
}

CXTPGridInplaceEdit::~CXTPGridInplaceEdit()
{
	if (m_xtpBrushBack.GetSafeHandle())
		m_xtpBrushBack.DeleteObject();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridInplaceEdit, CXTPMaskEditT<CEdit>)
	//{{AFX_MSG_MAP(CXTPGridInplaceEdit)
	ON_WM_MOUSEACTIVATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPGridInplaceEdit::HideWindow()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
		SetItemArgs(0);
	}
}

void CXTPGridInplaceEdit::Create(XTP_GRIDRECORDITEM_ARGS* pItemArgs)
{
	SetItemArgs(pItemArgs);
	m_pSelectedConstraint = NULL;

	XTP_GRIDRECORDITEM_METRICS* pMetrics = new XTP_GRIDRECORDITEM_METRICS;

	pItemArgs->pRow->FillMetrics(pColumn, pItem, pMetrics);

	if (!pControl->IsVirtualMode())
	{
		CXTPGridRecordItemText* pTextItem = DYNAMIC_DOWNCAST(CXTPGridRecordItemText, pItem);

		if (pTextItem != NULL)
		{
			pMetrics->strText = pTextItem->GetValue();
		}
		else // not CXTPGridRecordItemText case!
		{
			pMetrics->strText = pItem->GetCaption(pColumn);
		}
	}

	CXTPGridRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pColumn);
	ASSERT(pEditOptions);
	if (!pEditOptions)
		return;
	CRect rect = pItemArgs->rcItem;

	rect.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(2));

	m_clrText = pMetrics->clrForeground;
	m_clrBack = pControl->GetPaintManager()->m_clrControlBack;

	if (m_xtpBrushBack.GetSafeHandle())
		m_xtpBrushBack.DeleteObject();

	m_xtpBrushBack.CreateSolidBrush(m_clrBack);

	m_strValue	 = pMetrics->strText;
	m_strText_prev = pMetrics->strText;

	DWORD dwEditStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | pEditOptions->m_dwEditStyle;

	// Remove alignment from style
	dwEditStyle &= ~(ES_LEFT | ES_CENTER | ES_RIGHT);

	if (pControl->GetPaintManager()->m_bUseEditTextAlignment)
	{
		if (pMetrics->nColumnAlignment & DT_RIGHT)
			dwEditStyle |= ES_RIGHT;
		else if (pMetrics->nColumnAlignment & DT_CENTER)
			dwEditStyle |= ES_CENTER;
	}

	if (m_hWnd)
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_READONLY;

		if ((GetStyle() & dwStyle) != (dwEditStyle & dwStyle))
			DestroyWindow();
	}

	if (!m_hWnd)
	{
		CEdit::Create(dwEditStyle, rect, pControl, 0);
	}

	if (pControl->GetExStyle() & WS_EX_RTLREADING)
	{
		ModifyStyleEx(0, WS_EX_RTLREADING);
	}

	SetLimitText(XTPToUIntChecked(pEditOptions->m_nMaxLength));

	// SetFocus();
	SetFont(pMetrics->pFont);
	SetWindowText(m_strValue);

	pMetrics->InternalRelease();

	SetMargins(XTPToUIntChecked(XTP_DPI_X(1)), XTPToUIntChecked(XTP_DPI_Y(1)));

	SetUseMask(pColumn->IsUseMask());
	if (pColumn->IsUseMask())
	{
		SetPromptChar(pColumn->GetPrompt(), FALSE);
		SetEditMask(pColumn->GetMask(), pColumn->GetLiteral(), pItem->GetCaption());
	}

	if (rect.right > rect.left)
	{
		SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(),
					 SWP_NOZORDER | SWP_SHOWWINDOW);
		// CEdit::SetRect(rect);
		SetFocus();
	}
	else
	{
		HideWindow();
	}
}

void CXTPGridInplaceEdit::OnSetFocus(CWnd* pOldWnd)
{
	if (pItem && pColumn && pItem->GetEditOptions(pColumn)->m_bConstraintEdit)
	{
		return;
	}

	CXTPMaskEditT<CEdit>::OnSetFocus(pOldWnd);
}

void CXTPGridInplaceEdit::SetWindowText(LPCTSTR lpszString)
{
	m_bSetWindowText = TRUE;

	CWnd::SetWindowText(lpszString);
	m_strText_prev = lpszString;

	m_bSetWindowText = FALSE;
}

void CXTPGridInplaceEdit::SetFont(CFont* pFont)
{
	ASSERT_VALID(pFont);

	m_xtpFontEdit.DeleteObject();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_xtpFontEdit.CreateFontIndirect(&lf);

	CEdit::SetFont(&m_xtpFontEdit);
}

void CXTPGridInplaceEdit::OnEnKillfocus()
{
	if (pControl && pItem)
	{
		pControl->RedrawControl();
		pItem->OnValidateEdit((XTP_GRIDRECORDITEM_ARGS*)this);
		HideWindow();
	}
}

void CXTPGridInplaceEdit::OnEnChange()
{
	if (m_bSetWindowText || !pControl || !pItem)
		return;

	CString strValue, strValNew;
	GetWindowText(strValue);
	strValNew = strValue;

	// recover previous selection by diff between old and new text - compare from the start and from
	// the end
	int kO = strValue.GetLength();
	int kP = m_strText_prev.GetLength();
	int K  = min(kO, kP);
	int kOld(0);
	int kOldEnd(kP);
	if (m_strText_prev != strValue)
	{
		for (int j = 0; j < K; j++)
		{
			if (m_strText_prev[j] != strValue[j])
			{
				kOld = j;
				break;
			}
		}
		for (int jB = 1; jB <= K; jB++)
		{
			if (m_strText_prev[kP - jB] != strValue[kO - jB])
			{
				kOldEnd = kP + 1 - jB;
				break;
			}
		}
	}
	BOOL bCommit = pItem->OnEditChanging((XTP_GRIDRECORDITEM_ARGS*)this, strValNew);

	if (!bCommit || strValNew != strValue)
	{
		int nSelStart = 0, nSelEnd = 0;
		GetSel(nSelStart, nSelEnd);

		int kN = strValNew.GetLength();
		int kk = min(kO, kN);
		if (strValNew != strValue) // it means that bCommit is TRUE
		{
			SetWindowText(strValNew);
			int k(kk - 1);
			for (int j = 0; j < kk; j++)
			{
				if (strValNew[j] != strValue[j])
				{
					k = j;
					break;
				}
			}
			SetSel(k, k);
		}
		else // it means that bCommit is FALSE - need rollback
		{
			SetWindowText(m_strText_prev);

			SetSel(kOld, kOldEnd);
			// Cursor position before 1st OnEditChanging - handler - modified symbol
			if (nSelStart == kO && nSelEnd == kO && kP < kO)
				SetSel(nSelStart, nSelEnd);

			if (nSelStart == 1 && nSelEnd == 1 && kP < kO)
				SetSel(0, 0);
		}
	}
	else
	{
		m_strText_prev = strValue;
		// Normal (default) cursor positioning
	}
}

int CXTPGridInplaceEdit::OnMouseActivate(CWnd*, UINT,
										 UINT) //(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;
}

UINT CXTPGridInplaceEdit::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPGridInplaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!pControl)
		return;

	if (nChar == VK_TAB)
	{
		return;
	}

	if (nChar == VK_ESCAPE)
	{
		pControl->EditItem(NULL);
		return;
	}

	if (nChar == VK_RETURN)
	{
		if ((GetStyle() & ES_WANTRETURN) == 0)
		{
			pControl->EditItem(NULL);
		}
		else
		{
			CXTPMaskEditT<CEdit>::OnChar(nChar, nRepCnt, nFlags);
		}
		return;
	}

	if (pItem && pColumn && pItem->GetEditOptions(pColumn)->m_bConstraintEdit)
	{
		CXTPGridRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pColumn);

		CXTPGridRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		int nCount									= pConstraints->GetCount();
		if (nCount > 0)
		{
			CString str, strActual;
			GetWindowText(str);
			strActual = str;

			CXTPGridRecordItemConstraint* pConstraint = NULL;

			if (NULL != m_pSelectedConstraint)
			{
				pConstraint = m_pSelectedConstraint;
			}
			else
			{
				pConstraint = pEditOptions->FindConstraint(str);
			}

			int nIndexStart, nIndex;
			nIndexStart = nIndex = (pConstraint == NULL ? nCount - 1 : pConstraint->GetIndex());

			CString strSeach((TCHAR)nChar);

			do
			{
				nIndex = nIndex < nCount - 1 ? nIndex + 1 : 0;

				pConstraint = pConstraints->GetAt(nIndex);
				str			= pConstraint->m_strConstraint;

				if (strSeach.CompareNoCase(str.Left(1)) == 0)
				{
					m_pSelectedConstraint = pConstraint;
					SetWindowText(str);
					SetSel(0, -1);

					if (strActual.CompareNoCase(str) != 0)
						((CXTPGridControl*)pControl)
							->OnConstraintSelecting(pRow, pItem, pColumn, pConstraint);

					return;
				}

			} while (nIndex != nIndexStart);

			return;
		}
	}
	if (nChar == 1 && nFlags == 30) // Ctrl+A case
	{
		SetSel(0, -1);
	}
	CXTPMaskEditT<CEdit>::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPGridInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPGridControl* _pControl = pControl;
	if (!_pControl)
		return;

	if (nChar == VK_TAB)
	{
		_pControl->SetFocus();
		_pControl->SendMessage(WM_CHAR, nChar);
		return;
	}
	if (nChar == VK_ESCAPE)
	{
		SetWindowText(m_strValue);
		return;
	}
	else if (nChar == VK_F5)
	{
		_pControl->Recalc(TRUE);
		return;
	}
	else if (nChar == VK_RETURN)
	{
		if ((GetStyle() & ES_WANTRETURN) == 0)
			return;

		if (GetKeyState(VK_CONTROL) < 0) // ES_WANTRETURN is set and Ctrl+Enter is pressed
		{
			pControl->EditItem(NULL);
			return;
		}
	}
	else if (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_PRIOR || nChar == VK_NEXT)
	{
		if (pItem && pColumn && pItem->GetEditOptions(pColumn)->m_bConstraintEdit)
		{
			CXTPGridRecordItemConstraint* pConstraint;
			CXTPGridRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pColumn);
			CXTPGridRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

			int nCount = pConstraints->GetCount();
			if (nCount > 1)
			{
				CString strActual, str;
				GetWindowText(strActual);

				int nIndex = 0; // the first item

				if (nChar == VK_NEXT)
				{
					nIndex = nCount - 1; // the last item
				}
				else if (nChar != VK_PRIOR)
				{
					// look for the actually selected item
					for (int i = 0; i < nCount; i++)
					{
						pConstraint = pConstraints->GetAt(i);

						if (strActual.CompareNoCase(pConstraint->m_strConstraint) == 0)
						{
							if (nChar == VK_UP)
								nIndex = max(0, i - 1);
							else if (nChar == VK_DOWN)
								nIndex = min(nCount - 1, i + 1);

							break;
						}
					}
				}

				pConstraint			  = pConstraints->GetAt(max(0, min(nIndex, nCount - 1)));
				str					  = pConstraint->m_strConstraint;
				m_pSelectedConstraint = pConstraint;

				// set the default font, because user could change the font (for ex. to striked one)
				SetFont(pControl->GetPaintManager()->GetTextFont());
				SetWindowText(str);
				SetSel(0, -1);

				if (strActual.CompareNoCase(str) != 0)
					((CXTPGridControl*)pControl)
						->OnConstraintSelecting(pRow, pItem, pColumn, pConstraint);

				return;
			}
		}
	}

	CXTPMaskEditT<CEdit>::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPGridInplaceEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_UP || nChar == VK_DOWN) && pControl)
	{
		if (pControl->GetInplaceButtons()->GetSize() > 0)
		{
			CXTPGridInplaceButton* pButton = pControl->GetInplaceButtons()->GetAt(0);

			if (pButton->GetItem() == pItem)
			{
				pItem->OnInplaceButtonDown(pButton);
			}
		}
	}

	CXTPMaskEditT<CEdit>::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPGridInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pControl)
	{
		if (!pControl->OnPreviewKeyDown((UINT&)pMsg->wParam, LOWORD(pMsg->lParam),
										HIWORD(pMsg->lParam)))
			return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;

	return CXTPMaskEditT<CEdit>::PreTranslateMessage(pMsg);
}

void CXTPGridInplaceEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (pRow && pItem)
	{
		MapWindowPoints(pControl, &point, 1);
		pRow->OnDblClick(point);
	}

	if (pItem)
	{
		CXTPMaskEditT<CEdit>::OnLButtonDblClk(nFlags, point);
	}
}

HBRUSH CXTPGridInplaceEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);

	return (HBRUSH)m_xtpBrushBack;
}
