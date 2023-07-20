// XTPRibbonSystemButton.cpp : implementation file
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
#include "Common/XTPIntel80Helpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/XTPResourceManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPRecentFileList.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/Resource.h"
#include "Ribbon/XTPRibbonSystemButton.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_ICON_PINNED 1000
#define ID_ICON_UNPINNED 1001

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemButton, CXTPControlPopup)

CXTPRibbonControlSystemButton::CXTPRibbonControlSystemButton()
{
	SetFlags(xtpFlagNoMovable | xtpFlagManualUpdate);
	m_bShowShadow = FALSE;

	m_bCloseOnDblClick = TRUE;

	EnableAutomation();
}

BOOL CXTPRibbonControlSystemButton::IsSimpleButton() const
{
	return GetStyle() != xtpButtonAutomatic;
}

CString CXTPRibbonControlSystemButton::GetCaption() const
{
	CString strCaption = CXTPControlPopup::GetCaption();

	if (GetParent()->GetCommandBars()->IsAllCaps())
	{
		strCaption.MakeUpper();
	}

	return strCaption;
}

CSize CXTPRibbonControlSystemButton::GetSize(CDC* pDC)
{
	CSize sz = CXTPControlPopup::GetSize(pDC);

	if (IsSimpleButton())
	{
		sz.cx = max(sz.cx, XTP_DPI_X(56));
		sz.cy = max(sz.cy, XTP_DPI_Y(23));
	}

	return sz;
}

void CXTPRibbonControlSystemButton::Draw(CDC* pDC)
{
	((CXTPRibbonBar*)GetParent())
		->GetRibbonPaintManager()
		->DrawRibbonFrameSystemButton(pDC, this, GetRect());
}

BOOL CXTPRibbonControlSystemButton::OnLButtonDblClk(CPoint /*point*/)
{
	GetParent()->GetCommandBars()->ClosePopups();

	CWnd* pSite = GetParent()->GetSite();

	if ((!IsSimpleButton() && (pSite->GetStyle() & WS_CHILD) == 0) && m_bCloseOnDblClick)
	{
		pSite->SendMessage(WM_SYSCOMMAND, SC_CLOSE | HTSYSMENU, 0);
	}
	else
	{
		OnExecute();
	}

	return TRUE;
}

void CXTPRibbonControlSystemButton::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetParent());
	if (!pRibbonBar)
	{
		CXTPControlPopup::AdjustExcludeRect(rc, bVertical);
		return;
	}

	if (IsSimpleButton())
		return;

	if (pRibbonBar->IsCaptionVisible() && pRibbonBar->IsTabsVisible()
		&& DYNAMIC_DOWNCAST(CXTPRibbonSystemPopupBar, GetCommandBar()))
	{
		rc.bottom -= XTP_DPI_Y(18);
		return;
	}

	CXTPControlPopup::AdjustExcludeRect(rc, bVertical);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonSystemPopupBar, CXTPPopupBar)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPRibbonSystemPopupBar, CXTPPopupBar)
	ON_WM_NCHITTEST_EX()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPRibbonSystemPopupBar::CXTPRibbonSystemPopupBar()
{
	if (GetPaintManager()->GetCurrentTheme() >= xtpThemeOffice2013)
		m_rcBorders = XTP_DPI(CRect(2, 2, 2, 2));
	else
		m_rcBorders = XTP_DPI(CRect(6, 18, 6, 29));
	SetShowGripper(FALSE);
}

CRect CXTPRibbonSystemPopupBar::GetBorders()
{
	if (GetPaintManager()->GetCurrentTheme() >= xtpThemeOffice2013)
		m_rcBorders = XTP_DPI(CRect(2, 2, 2, 2));
	else
		m_rcBorders = XTP_DPI(CRect(6, 18, 6, 29));

	return m_rcBorders;
}

void CXTPRibbonSystemPopupBar::FillCommandBarEntry(CDC* pDC)
{
	CXTPClientRect rc(this);
	CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();

	pPaintManager->FillSystemPopupBarEntry(pDC, this);
}

LRESULT CXTPRibbonSystemPopupBar::OnNcHitTest(CPoint point)
{
	if (!DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton, m_pControlPopup))
		return CXTPPopupBar::OnNcHitTest(point);

	CRect rcPopup = m_pControlPopup->GetRect();
	m_pControlPopup->GetParent()->ClientToScreen(rcPopup);
	if (rcPopup.PtInRect(point))
		return HTTRANSPARENT;

	return CXTPPopupBar::OnNcHitTest(point);
}

CSize CXTPRibbonSystemPopupBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CArray<CXTPControl*, CXTPControl*> arrButtons;
	int i;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (pControl && pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonControlSystemPopupBarButton)))
		{
			arrButtons.Add(pControl);
			pControl->SetHideFlag(xtpHideDockingPosition, TRUE);
		}
	}

	CSize sz = CXTPPopupBar::CalcDynamicLayout(nLength, dwMode);

	if (arrButtons.GetSize() == 0)
		return sz;

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	int nRight = sz.cx - m_rcBorders.right + XTP_DPI_X(1);

	for (i = (int)arrButtons.GetSize() - 1; i >= 0; i--)
	{
		CXTPControl* pControl = arrButtons[i];
		pControl->SetHideFlag(xtpHideDockingPosition, FALSE);

		CSize szControl = pControl->GetSize(&dc);
		pControl->SetRect(CRect(nRight - szControl.cx, sz.cy - m_rcBorders.bottom + XTP_DPI_Y(4),
								nRight, sz.cy - XTP_DPI_Y(3)));
		nRight -= szControl.cx + XTP_DPI_X(6);
	}

	return sz;
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarButton

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarButton, CXTPControlButton)

CXTPRibbonControlSystemPopupBarButton::CXTPRibbonControlSystemPopupBarButton()
{
}

BOOL CXTPRibbonControlSystemPopupBarButton::IsSystemPopupButton() const
{
	return m_pParent && m_pParent->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar));
}

BOOL CXTPRibbonControlSystemPopupBarButton::IsTransparent() const
{
	if (IsSystemPopupButton())
		return TRUE;

	return CXTPControlButton::IsTransparent();
}

void CXTPRibbonControlSystemPopupBarButton::Draw(CDC* pDC)
{
	if (IsSystemPopupButton())
	{
		CXTPRibbonPaintManager* pPaintManager = GetPaintManager()->GetRibbonPaintManager();
		pPaintManager->DrawSystemPopupBarButton(pDC, this);
	}
	else
	{
		CXTPControlButton::Draw(pDC);
	}
}

CSize CXTPRibbonControlSystemPopupBarButton::GetSize(CDC* pDC)
{
	if (IsSystemPopupButton())
		return GetPaintManager()->DrawControlToolBarParent(pDC, this, FALSE);

	return CXTPControlButton::GetSize(pDC);
}

CSize CXTPRibbonControlSystemPopupBarButton::GetButtonSize() const
{
	return XTP_DPI(CSize(22, 22));
}

CSize CXTPRibbonControlSystemPopupBarButton::GetIconSize() const
{
	return CSize(16, 16);
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlRecentFileList
IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListItem, CXTPControlButton)

CXTPRibbonControlSystemPopupBarListItem::CXTPRibbonControlSystemPopupBarListItem()
{
	m_nWidth		 = XTP_DPI_X(300);
	m_nHeight		 = XTP_DPI_Y(21);
	m_bAlignShortcut = FALSE;
}

CSize CXTPRibbonControlSystemPopupBarListItem::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight);
}

void CXTPRibbonControlSystemPopupBarListItem::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = (CXTPPaintManager*)GetPaintManager();

	pPaintManager->DrawControlEntry(pDC, this);

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode(TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + XTP_DPI_X(7), rc.top, rc.right, rc.bottom);
	CString strText(GetCaption());

	if (m_nIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = GetImage();
		if (pIcon)
		{
			CSize sz = CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), this,
														   rc.Size());
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - sz.cy) / 2));
			rcText.left += sz.cx + XTP_DPI_X(4);
		}
	}

	if (m_bAlignShortcut)
	{
		if (strText.GetLength() > 2 && strText[0] == _T('&') && strText[2] == _T(' '))
		{
			pDC->DrawText(strText.Left(2), &rcText, DT_SINGLELINE | DT_VCENTER);
			strText.Delete(0, 3);
		}

		rcText.left += pDC->GetTextExtent(_T("0"), 1).cx + XTP_DPI_X(7);
	}

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarPinableListItem

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarPinableListItem,
					  CXTPRibbonControlSystemPopupBarListItem)

#define PIN_WIDTH XTP_DPI_X(28)

CXTPRibbonControlSystemPopupBarPinableListItem::CXTPRibbonControlSystemPopupBarPinableListItem()
{
	m_pIcons = new CXTPImageManager();

	m_pIcons->SetVectorIcon(XTPResourceManager()->GetResourceHandle(), _T("RT_XAML"),
							XTP_XAML_ICON_PIN_HORZ, ID_ICON_PINNED,
							XTPToUIntChecked(XTP_DPI_X(16)));
	m_pIcons->SetVectorIcon(XTPResourceManager()->GetResourceHandle(), _T("RT_XAML"),
							XTP_XAML_ICON_PIN_GREEN, ID_ICON_UNPINNED,
							XTPToUIntChecked(XTP_DPI_X(16)));
}

CXTPRibbonControlSystemPopupBarPinableListItem::~CXTPRibbonControlSystemPopupBarPinableListItem()
{
	SAFE_DELETE(m_pIcons);
}

void CXTPRibbonControlSystemPopupBarPinableListItem::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = (CXTPPaintManager*)GetPaintManager();

	if (m_bSelected == TRUE_SPLITDROPDOWN)
	{
		CRect rcCheck(GetRect());
		rcCheck.left = rcCheck.right - PIN_WIDTH;

		pPaintManager->DrawRectangle(pDC, rcCheck, GetSelected(), GetPressed(), GetEnabled(), FALSE,
									 FALSE, GetParent()->GetType(), GetParent()->GetPosition());
	}
	else
	{
		pPaintManager->DrawRectangle(pDC, GetRect(), GetSelected(), GetPressed(), GetEnabled(),
									 FALSE, FALSE, GetParent()->GetType(),
									 GetParent()->GetPosition());
	}

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode(TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + XTP_DPI_X(7), rc.top, rc.right - PIN_WIDTH, rc.bottom);
	CString strText(GetCaption());

	if (m_nIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = GetImage();
		if (pIcon)
		{
			CSize sz = CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), this,
														   rc.Size());
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - sz.cy) / 2));
			rcText.left += sz.cx + XTP_DPI_X(4);
		}
	}

	if (m_bAlignShortcut)
	{
		if (strText.GetLength() > 2 && strText[0] == _T('&') && strText[2] == _T(' '))
		{
			pDC->DrawText(strText.Left(2), &rcText, DT_SINGLELINE | DT_VCENTER);
			strText.Delete(0, 3);
		}

		rcText.left += pDC->GetTextExtent(_T("0"), 1).cx + XTP_DPI_X(7);
	}

	pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

	CRect rcCheck(rc.right - PIN_WIDTH, rc.top, rc.right, rc.bottom);

	CXTPImageManagerIcon* pIcon = m_pIcons->GetImage(
		UINT(GetChecked() ? ID_ICON_PINNED : ID_ICON_UNPINNED));
	ASSERT_VALID(pIcon);

	CSize sz;
	if (pIcon->IsVectorIcon())
		sz = CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
	else
		CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), this, rc.Size());

	pIcon->Draw(pDC,
				CPoint(rcCheck.CenterPoint().x - XTP_DPI_X(8),
					   rcCheck.CenterPoint().y - XTP_DPI_Y(8)),
				sz);
}

void CXTPRibbonControlSystemPopupBarPinableListItem::OnMouseMove(CPoint point)
{
	CRect rcCheck(GetRect());
	rcCheck.left = rcCheck.right - PIN_WIDTH;

	if (m_bSelected && rcCheck.PtInRect(point) && m_bSelected != TRUE_SPLITDROPDOWN)
	{
		m_bSelected = TRUE_SPLITDROPDOWN;
		RedrawParent(FALSE);
		return;
	}
	else if (m_bSelected == TRUE_SPLITDROPDOWN && !rcCheck.PtInRect(point)
			 && GetRect().PtInRect(point))
	{
		m_bSelected = TRUE;
		RedrawParent(FALSE);
		return;
	}

	CXTPRibbonControlSystemPopupBarListItem::OnMouseMove(point);
}

BOOL CXTPRibbonControlSystemPopupBarPinableListItem::OnSetSelected(int bSelected)
{
	if (!CXTPRibbonControlSystemPopupBarListItem::OnSetSelected(bSelected))
		return FALSE;

	if (bSelected && !IsKeyboardSelected(bSelected))
	{
		CRect rcCheck(GetRect());
		rcCheck.left = rcCheck.right - PIN_WIDTH;

		CPoint point;
		GetCursorPos(&point);
		GetParent()->ScreenToClient(&point);

		if (rcCheck.PtInRect(point))
			m_bSelected = TRUE_SPLITDROPDOWN;
	}

	return TRUE;
}

void CXTPRibbonControlSystemPopupBarPinableListItem::OnLButtonUp(CPoint point)
{
	CRect rcCheck(GetRect());
	rcCheck.left = rcCheck.right - PIN_WIDTH;

	if (rcCheck.PtInRect(point))
	{
		SetChecked(!GetChecked());
		return;
	}

	CXTPRibbonControlSystemPopupBarListItem::OnLButtonUp(point);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControlSystemPopupBarListCaption

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemPopupBarListCaption, CXTPControl)

CXTPRibbonControlSystemPopupBarListCaption::CXTPRibbonControlSystemPopupBarListCaption()
{
	SetFlags(xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable | xtpFlagWrapRow);
	m_nWidth  = XTP_DPI_X(300);
	m_nHeight = XTP_DPI_Y(27);
}

CSize CXTPRibbonControlSystemPopupBarListCaption::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight);
}

void CXTPRibbonControlSystemPopupBarListCaption::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPFontDC dont(pDC, pPaintManager->GetRegularBoldFont());

	COLORREF clrText = pPaintManager->GetControlTextColor(this);

	pDC->SetTextColor(clrText);
	pDC->SetBkMode(TRANSPARENT);

	CRect rc(GetRect());
	CRect rcText(rc.left + XTP_DPI_X(7), rc.top + XTP_DPI_Y(4), rc.right, rc.bottom - XTP_DPI_Y(5));

	pDC->DrawText(GetCaption(), &rcText, DT_SINGLELINE | DT_TOP);

	pPaintManager->HorizontalLine(
		pDC, rc.left, rc.bottom - XTP_DPI_Y(6), rc.right,
		pPaintManager->GetRibbonPaintManager()->m_clrRecentFileListEdgeShadow);
	pPaintManager->HorizontalLine(
		pDC, rc.left, rc.bottom - XTP_DPI_Y(5), rc.right,
		pPaintManager->GetRibbonPaintManager()->m_clrRecentFileListEdgeHighLight);
}

IMPLEMENT_XTP_CONTROL(CXTPRibbonControlSystemRecentFileList,
					  CXTPRibbonControlSystemPopupBarListCaption)

CXTPRibbonControlSystemRecentFileList::CXTPRibbonControlSystemRecentFileList()
{
}

CRecentFileList* CXTPRibbonControlSystemRecentFileList::GetRecentFileList()
{
#ifndef _XTP_COMMANDBARS_ACTIVEX
	_XTP_USES_PROTECTED_ACCESS(CXTPRibbonControlSystemRecentFileList, CWinApp, CRecentFileList*,
							   m_pRecentFileList)

	return _XTP_PROTECTED_ACCESS(CWinApp, AfxGetApp(), m_pRecentFileList);
#else
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	return pCommandBars->m_pRecentFileList;
#endif
}

int CXTPRibbonControlSystemRecentFileList::GetFirstMruID()
{
#ifdef _XTP_COMMANDBARS_ACTIVEX
	return GetID();
#else
	return ID_FILE_MRU_FILE1;
#endif
}

UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);

CString CXTPRibbonControlSystemRecentFileList::ConstructCaption(const CString& lpszTitle,
																int nIndex)
{
	CString strTemp;

	// double up any '&' characters so they are not underlined
	LPCTSTR lpszSrc = lpszTitle;
	LPTSTR lpszDest = strTemp.GetBuffer(lpszTitle.GetLength() * 2);

	while (*lpszSrc != 0)
	{
		if (*lpszSrc == '&')
			*lpszDest++ = '&';

		if (_istlead(XTPToUInt(*lpszSrc)))
			*lpszDest++ = *lpszSrc++;
		*lpszDest++ = *lpszSrc++;
	}

	*lpszDest = 0;
	strTemp.ReleaseBuffer();

	CString strTitle;

	if (nIndex == 0)
		return strTemp;

	if (nIndex < 10)
	{
		strTitle.Format(_T("&%i %s"), nIndex, (LPCTSTR)strTemp);
	}
	else
	{
		strTitle = strTemp;
	}

	return strTitle;
}

class CXTPRibbonControlSystemRecentFileList::CControlFileItem
	: public CXTPRibbonControlSystemPopupBarListItem
{
public:
	CControlFileItem()
	{
		m_bAlignShortcut = TRUE;
	}
};

class CXTPRibbonControlSystemRecentFileList::CControlPinableFileItem
	: public CXTPRibbonControlSystemPopupBarPinableListItem
{
public:
	CControlPinableFileItem(CXTPRecentFileList* pFileList, int iMRU)
	{
		m_bAlignShortcut = TRUE;
		m_iMRU			 = iMRU;
		m_pFileList		 = pFileList;
	}

	void SetChecked(BOOL bChecked)
	{
		CXTPControl::SetChecked(bChecked);

		m_pFileList->GetItem(m_iMRU)->SetPinned(bChecked);
	}

protected:
	int m_iMRU;
	CXTPRecentFileList* m_pFileList;
};

void CXTPRibbonControlSystemRecentFileList::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	CRecentFileList* pRecentFileList = GetRecentFileList();

	if (!pRecentFileList)
		return;

	ASSERT(pRecentFileList->m_arrNames != NULL);
	if (!pRecentFileList->m_arrNames)
		return;

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= GetFirstMruID()
			&& pControl->GetID() <= GetFirstMruID() + pRecentFileList->m_nSize)
		{
			m_pControls->Remove(pControl);
		}
		else
			break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		SetEnabled(TRUE);
		return;
	}

	CString strName;
	BOOL bExtendedRecentFileList = pRecentFileList->m_strOriginal == _T("PinableRecentFileList");
	BOOL bPinable = bExtendedRecentFileList && ((CXTPRecentFileList*)pRecentFileList)->m_bPinable;

	for (int iMRU = 0; iMRU < pRecentFileList->m_nSize; iMRU++)
	{
		if (pRecentFileList->m_arrNames[iMRU].IsEmpty())
			break;

		// copy file name only since directories are same
		AfxGetFileTitle(pRecentFileList->m_arrNames[iMRU], strName.GetBuffer(_MAX_PATH), _MAX_PATH);
		strName.ReleaseBuffer();

#ifdef _XTP_COMMANDBARS_ACTIVEX
		int nId = GetFirstMruID();
#else
		int nId = iMRU + GetFirstMruID();
#endif

		CXTPRecentFileListItem* pItem = bExtendedRecentFileList
											? ((CXTPRecentFileList*)pRecentFileList)->GetItem(iMRU)
											: NULL;
		if (pItem)
		{
			if (!pItem->GetCaption().IsEmpty())
				strName = pItem->GetCaption();
		}

		CXTPControl* pControl = m_pControls->Add(
			!bPinable
				? (CXTPControl*)new CControlFileItem()
				: (CXTPControl*)new CControlPinableFileItem((CXTPRecentFileList*)pRecentFileList,
															iMRU),
			nId, _T(""), m_nIndex + iMRU + 1, TRUE);

		if (pItem && bPinable)
		{
			pControl->SetChecked(pItem->IsPinned());
		}

		if (pItem)
		{
			pControl->SetIconId(pItem->GetIconId());
		}

		pControl->SetCaption(ConstructCaption(strName, iMRU + 1));
		pControl->SetFlags(xtpFlagManualUpdate | xtpFlagShowPopupBarTip);
		pControl->SetTooltip(pRecentFileList->m_arrNames[iMRU]);
		pControl->SetParameter(pRecentFileList->m_arrNames[iMRU]);
		pControl->SetDescription(NULL);
	}
}

BOOL CXTPRibbonControlSystemRecentFileList::DoCustomDragOver(CXTPCommandBar* pCommandBar,
															 CPoint /*point*/,
															 DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonSystemPopupBarPage

IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonSystemPopupBarPage, CXTPPopupBar)

CXTPRibbonSystemPopupBarPage::CXTPRibbonSystemPopupBarPage()
{
}

CSize CXTPRibbonSystemPopupBarPage::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	CSize sz = CXTPPopupBar::CalcDynamicLayout(nLength, dwMode);

	if (!m_pControlPopup->GetParent()->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar)))
		return sz;

	CXTPRibbonSystemPopupBar* pParent = ((CXTPRibbonSystemPopupBar*)m_pControlPopup->GetParent());

	int nHeight = CXTPClientRect(pParent).Height()
				  - (pParent->GetBorders().top + pParent->GetBorders().bottom) - XTP_DPI_Y(2);

	m_nMaxHeight = nHeight;

	if (nHeight > sz.cy)
		sz.cy = nHeight;

	return sz;
}

void CXTPRibbonSystemPopupBarPage::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	if (!m_pControlPopup->GetParent()->IsKindOf(RUNTIME_CLASS(CXTPRibbonSystemPopupBar)))
	{
		CXTPPopupBar::AdjustExcludeRect(rc, bVertical);
		return;
	}

	rc.top = ((CXTPRibbonSystemPopupBar*)m_pControlPopup->GetParent())->GetBorders().top
			 + XTP_DPI_Y(1);
	rc.right += XTP_DPI_X(1);
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonControlSystemButton, CXTPControlPopup)
	DISP_PROPERTY_ID(CXTPRibbonControlSystemButton, "CloseOnDblClick", 200, m_bCloseOnDblClick,
					 VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPRibbonControlSystemButton, CXTPControlPopup)
	INTERFACE_PART(CXTPRibbonControlSystemButton, XTPDIID_RibbonBarSystemButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonControlSystemButton, XTPDIID_RibbonBarSystemButton)

#endif
