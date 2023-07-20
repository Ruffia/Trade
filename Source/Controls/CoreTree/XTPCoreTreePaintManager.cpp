// XTPCoreTreePaintManager.cpp: implementation of the CXTPCoreTreePaintManager class.
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"

#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPResourceImage.h"

#include "Controls/CoreTree/XTPCoreTreePaintManager.h"
#include "Controls/CoreTree/XTPCoreTreeControl.h"
#include "Controls/CoreTree/XTPCoreTreeItem.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCoreTreePaintManager::CXTPCoreTreePaintManager()
{
	m_nItemHeight = NULL;

	m_themeButton = new CXTPWinThemeWrapper();
	m_themeTree   = new CXTPWinThemeWrapper();

	m_bShowButtons = TRUE;

	m_bEnableTheme = FALSE;
}

CXTPCoreTreePaintManager::~CXTPCoreTreePaintManager()
{
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeTree);
}

void CXTPCoreTreePaintManager::RefreshMetrics()
{
	m_clrBackground = GetSysColor(COLOR_WINDOW);
	m_clrText		= GetSysColor(COLOR_WINDOWTEXT);

	m_clrLabelBackground = RGB(240, 242, 245);
	m_clrLabelText		 = RGB(59, 59, 59);

	m_clrSelectedText		= GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrSelectedBackground = GetSysColor(COLOR_HIGHLIGHT);

	m_clrDisabledText = GetSysColor(COLOR_GRAYTEXT);

	if (m_bEnableTheme)
	{
		m_clrBackground = XTPIniColor(_T("Controls.Tree"), _T("Back"), GetSysColor(COLOR_WINDOW));
		m_clrText = XTPIniColor(_T("Controls.Tree"), _T("Text"), GetSysColor(COLOR_WINDOWTEXT));

		m_clrSelectedText		= XTPIniColor(_T("Controls.Tree"), _T("TextSelected"),
										  GetSysColor(COLOR_HIGHLIGHTTEXT));
		m_clrSelectedBackground = XTPIniColor(_T("Controls.Tree"), _T("BackSelected"),
											  GetSysColor(COLOR_HIGHLIGHT));

		m_clrLabelBackground = XTPIniColor(_T("Controls.Edit"), _T("Back"), RGB(240, 242, 245));
		m_clrLabelText		 = XTPIniColor(_T("Controls.Edit"), _T("Text"), RGB(59, 59, 59));

		m_clrDisabledText = XTPIniColor(_T("Controls.Tree"), _T("TextDisabled"),
										GetSysColor(COLOR_GRAYTEXT));
	}

	m_themeButton->OpenThemeData(NULL, L"BUTTON");
	m_themeTree->OpenThemeData(NULL, L"TREEVIEW");

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	SetFontIndirect(&lfIcon);
}

void CXTPCoreTreePaintManager::SetFontIndirect(LOGFONT* pLogFont)
{
	m_xtpFontItem.DeleteObject();
	m_xtpFontLabel.DeleteObject();

	pLogFont->lfWeight = FW_BOLD;
	m_xtpFontLabel.CreateFontIndirect(pLogFont);

	pLogFont->lfWeight = FW_NORMAL;
	m_xtpFontItem.CreateFontIndirect(pLogFont);

	{
		CWindowDC dc(NULL);
		CXTPFontDC font(&dc, &m_xtpFontItem);
		m_nItemHeight = dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(2);
		if (m_nItemHeight < XTP_DPI_Y(16))
			m_nItemHeight = XTP_DPI_Y(16);
	}
}

void CXTPCoreTreePaintManager::FillTreeControl(CDC* pDC, CXTPCoreTreeControl* pTreeControl)
{
	CXTPClientRect rc(pTreeControl);
	pDC->FillSolidRect(rc, m_clrBackground);
}

void CXTPCoreTreePaintManager::DrawButton(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem)
{
	rcItem.right = rcItem.left + m_nItemHeight;

	CPoint pt = rcItem.CenterPoint();

	CRect rcSign(pt.x - XTP_DPI_X(4), pt.y - XTP_DPI_Y(4), pt.x + XTP_DPI_X(5),
				 pt.y + XTP_DPI_Y(5));

	if (m_themeTree->IsAppThemeActive())
	{
		m_themeTree->DrawThemeBackground(pDC->GetSafeHdc(), TVP_GLYPH,
										 pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign,
										 0);
	}
	else
	{
		pDC->Draw3dRect(rcSign, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNSHADOW));

		pDC->FillSolidRect(pt.x - XTP_DPI_X(2), pt.y, XTP_DPI_X(5), XTP_DPI_Y(1), m_clrText);

		if (!pItem->IsExpanded())
		{
			pDC->FillSolidRect(pt.x, pt.y - XTP_DPI_Y(2), XTP_DPI_X(1), XTP_DPI_Y(5), m_clrText);
		}
	}
}

void CXTPCoreTreePaintManager::DrawCheckbox(CDC* pDC, CXTPCoreTreeItem* pItem, CRect rcItem)
{
	rcItem.right  = rcItem.left + m_nItemHeight;
	BOOL bChecked = pItem->IsChecked();
	BOOL bEnabled = pItem->IsEnabled();

	CPoint pt(rcItem.CenterPoint());
	CRect rcCheckMark(pt.x - XTP_DPI_X(7), pt.y - XTP_DPI_Y(7), pt.x + XTP_DPI_X(6),
					  pt.y + XTP_DPI_Y(6));

	if (m_themeButton->IsAppThemeActive())
	{
		m_themeButton->DrawThemeBackground(
			pDC->GetSafeHdc(), BP_CHECKBOX,
			!bEnabled ? (bChecked ? CBS_CHECKEDDISABLED : CBS_UNCHECKEDDISABLED)
					  : (bChecked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL),
			rcCheckMark, 0);
	}
	else
	{
		pDC->DrawFrameControl(rcCheckMark, DFC_BUTTON,
							  UINT(DFCS_BUTTONCHECK | (!bEnabled ? DFCS_INACTIVE : 0)
								   | (bChecked ? DFCS_CHECKED : 0)));
	}
}

void CXTPCoreTreePaintManager::DrawDropRect(CDC* pDC, CXTPCoreTreeItem* pItem)
{
	CRect rcItem(pItem->GetRect());

	CXTPCoreTreeControl* pTreeControl = pItem->GetTreeControl();

	if (pTreeControl->GetDropPosition() == xtpCoreTreeDropBefore)
	{
		pDC->FillSolidRect(rcItem.left, rcItem.top - XTP_DPI_Y(1), rcItem.Width(), XTP_DPI_Y(2), 0);
	}
	else if (pTreeControl->GetDropPosition() == xtpCoreTreeDropAfter)
	{
		pDC->FillSolidRect(rcItem.left, rcItem.bottom - XTP_DPI_Y(1), rcItem.Width(), XTP_DPI_Y(2),
						   0);
	}
	else if (pTreeControl->GetDropPosition() == xtpCoreTreeDropChild)
	{
		int nIndent = GetChildrenIndent(pItem);

		pDC->FillSolidRect(rcItem.left + nIndent, rcItem.bottom - XTP_DPI_Y(1),
						   rcItem.Width() - nIndent, XTP_DPI_Y(2), 0);
	}
}

void CXTPCoreTreePaintManager::DrawItem(CDC* pDC, CXTPCoreTreeItem* pItem)
{
	if (pItem->IsLabel())
	{
		CXTPFontDC fnt(pDC, &m_xtpFontLabel);
		CRect rc(pItem->GetRect());

		pDC->FillSolidRect(rc, m_clrLabelBackground);
		pDC->FillSolidRect(rc.left, rc.bottom - XTP_DPI_Y(1), rc.Width(), XTP_DPI_Y(1),
						   RGB(226, 228, 231));

		pDC->SetTextColor(m_clrLabelText);

		CRect rcText(rc);
		rcText.left += 2;

		if (pItem->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupUIElement()),
									m_xtpFontLabel, pDC->GetTextColor());
			XTPMarkupRenderElement(pItem->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
		}
		else
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}
	}
	else
	{
		CXTPFontDC fnt(pDC, &m_xtpFontItem);

		CRect rcItem(pItem->GetRect());

		if (m_bShowButtons)
		{
			if (pItem->HasChildren())
			{
				DrawButton(pDC, pItem, rcItem);
			}
			rcItem.left += m_nItemHeight;
		}

		if (pItem->IsSelected())
		{
			if (::GetFocus() == pItem->GetTreeControl()->GetSafeHwnd())
			{
				pDC->FillSolidRect(rcItem, m_clrSelectedBackground);
				pDC->SetTextColor(m_clrSelectedText);
			}
			else
			{
				pDC->FillSolidRect(rcItem, GetSysColor(COLOR_BTNFACE));
				pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
			}
		}
		else
		{
			pDC->SetTextColor(m_clrText);
		}

		if (!pItem->IsEnabled())
		{
			pDC->SetTextColor(m_clrDisabledText);
		}

		if (pItem->IsCheckboxVisible())
		{
			DrawCheckbox(pDC, pItem, rcItem);
			rcItem.left += m_nItemHeight;
		}

		if (pItem->GetIconId() != -1)
		{
			CRect rcIcon;

			int nIconHight = XTP_DPI_Y(16);
			rcIcon.left	= rcItem.left + XTP_DPI_X(2);
			rcIcon.right   = rcIcon.left + XTP_DPI_X(16);
			rcIcon.top	 = (rcItem.top + rcItem.bottom - nIconHight) / 2;
			rcIcon.bottom  = rcIcon.top + nIconHight;

			CXTPImageManagerIcon* pIcon = pItem->GetImage();
			if (NULL != pIcon)
			{
				CSize size = rcIcon.Size();
				if (pIcon->IsRasterIcon() && !pItem->GetTreeControl()->IsDpiBitmapScalingEnabled())
				{
					CSize iconSize = pIcon->GetExtent();
					if (iconSize.cx < size.cx && iconSize.cy < size.cy)
						size = iconSize;
				}

				CPoint ptCorner = rcIcon.CenterPoint();
				ptCorner.x -= size.cx / 2;
				ptCorner.y -= size.cy / 2;

				BOOL bRTL = XTPDrawHelpers()->IsContextRTL(pDC);
				if (bRTL)
				{
					if (pIcon->IsVectorIcon())
					{
						XTPDrawHelpers()->SetContextRTL(pDC, !bRTL);
						ptCorner.x = rcItem.right - (ptCorner.x + size.cx);
					}
				}

				pIcon->Draw(pDC, ptCorner, pItem->IsEnabled() ? xtpImageNormal : xtpImageDisabled,
							size);

				XTPDrawHelpers()->SetContextRTL(pDC, XTPToDWORD(bRTL));
			}

			rcItem.left += XTP_DPI_X(20);
		}

		CRect rcText(rcItem);
		rcText.left += XTP_DPI_X(2);

		if (pItem->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupUIElement()),
									m_xtpFontItem, pDC->GetTextColor());
			XTPMarkupRenderElement(pItem->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
		}
		else
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}
	}
}

int CXTPCoreTreePaintManager::GetChildrenIndent(CXTPCoreTreeItem* pItem)
{
	if (pItem->IsCheckboxVisible())
		return 2 * m_nItemHeight;

	return m_nItemHeight;
}

BOOL CXTPCoreTreePaintManager::IsAppThemed() const
{
	return m_themeTree->IsAppThemed();
}

void CXTPCoreTreePaintManager::EnableTheme(BOOL bEnable)
{
	m_bEnableTheme = bEnable;

	RefreshMetrics();
}
