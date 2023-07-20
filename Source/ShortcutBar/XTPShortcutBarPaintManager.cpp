// XTPShortcutBarPaintManager.cpp : implementation of the CXTPShortcutBarPaintManager class.
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

#include "ShortcutBar/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"

#include "ShortcutBar/XTPShortcutBarPaintManager.h"
#include "ShortcutBar/XTPShortcutBar.h"
#include "ShortcutBar/XTPShortcutBarPane.h"

#include "ShortcutBar/XTPShortcutBarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPShortcutBarPaintManager::CXTPShortcutBarPaintManager()
	: m_bNavigationButton(FALSE)
{
	m_bBoldItemCaptionFont  = FALSE;
	m_pExpandIcon			= NULL;
	m_bClearTypeTextQuality = FALSE;

	CXTPImageManagerIconHandle iconHandle;

	HMODULE hModule = AfxFindResourceHandle(MAKEINTRESOURCE(XTP_IDB_SHORTCUT_EXPANDBUTTON),
											_T("PNG"));
	if (!hModule)
		hModule = XTPResourceManager()->GetResourceHandle();

	iconHandle.Attach(CXTPImageManagerIcon::LoadBitmapFromResource(
						  hModule, MAKEINTRESOURCE(XTP_IDB_SHORTCUT_EXPANDBUTTON), NULL),
					  FALSE);

	ASSERT(!iconHandle.IsEmpty());

	if (!iconHandle.IsEmpty())
	{
		m_pExpandIcon = new CXTPImageManagerIcon(0, 16, 16);
		m_pExpandIcon->SetNormalIcon(iconHandle);
	}

	m_pImageManager = new CXTPImageManager();
	m_pImageManager->SetVectorIcon(XTPResourceManager()->GetResourceHandle(), _T("RT_XAML"),
								   IDR_XAML_ICON_NAVIGATION_BUTTON, IDR_NAVIGATION_BUTTON,
								   XTPToUInt(GetSystemMetrics(SM_CXSMICON)));

	m_nNormalIconWidth   = XTP_DPI_X(16);
	m_nExpandedIconWidth = XTP_DPI_X(24);
	m_bUseStandardFont   = TRUE;
	m_nPaneItemHeight	= 0;

	m_rcCaptionPadding	 = XTP_DPI(CRect(4, 0, 4, 0));
	m_rcItemCaptionPadding = XTP_DPI(CRect(7, 0, 7, 0));

	m_bShowBorder = TRUE;

	m_bDrawTextNoPrefix = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPShortcutBarPaintManager::~CXTPShortcutBarPaintManager()
{
	SAFE_DELETE(m_pExpandIcon);
	SAFE_DELETE(m_pImageManager);
}

void CXTPShortcutBarPaintManager::EnableOfficeFont(BOOL bEnable /*=TRUE*/)
{
	m_strOfficeFont.Empty();

	if (bEnable)
	{
		if (XTPDrawHelpers()->FontExists(_T("Segoe UI")))
		{
			m_strOfficeFont = _T("Segoe UI");
		}

		else if (XTPDrawHelpers()->FontExists(_T("Tahoma")))
		{
			m_strOfficeFont = _T("Tahoma");
		}
	}
}

void CXTPShortcutBarPaintManager::CreateFonts()
{
	// create pane item caption font.
	// ------------------------------

	LOGFONT lf;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));
	STRCPY_S(lf.lfFaceName, LF_FACESIZE,
			 m_strOfficeFont.IsEmpty() ? _T("Arial") : m_strOfficeFont.operator LPCTSTR());

	if (m_bClearTypeTextQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		lf.lfQuality = 5;
	}

	lf.lfWeight = m_bBoldItemCaptionFont ? FW_BOLD : FW_NORMAL;

	m_xtpFontPaneItemCaption.DeleteObject();
	m_xtpFontPaneItemCaption.CreateFontIndirect(&lf);

	// create standard font.
	// ------------------------------

	if (m_bUseStandardFont || !m_xtpFontText.GetSafeHandle())
	{
		lf.lfWeight = FW_BOLD;

		m_xtpFontText.DeleteObject();
		m_xtpFontText.CreateFontIndirect(&lf);
	}

	// create pane caption font.
	// ------------------------------

	lf.lfWeight = FW_BOLD;
	lf.lfHeight = XTP_DPI_Y(20);

	m_xtpFontPaneCaption.DeleteObject();
	m_xtpFontPaneCaption.CreateFontIndirect(&lf);

	// create vertical font.
	// ------------------------------

	lf.lfWeight		 = m_strOfficeFont.IsEmpty() ? FW_BOLD : FW_NORMAL;
	lf.lfOrientation = 900;
	lf.lfEscapement  = 900;
	lf.lfHeight		 = XTP_DPI_Y(21);

	if (m_strOfficeFont.IsEmpty())
	{
		lf.lfHeight = XTP_DPI_Y(18);
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, CXTPDrawHelpers::GetVerticalFontName(FALSE));
	}

	m_xtpFontVerticalText.DeleteObject();
	m_xtpFontVerticalText.CreateFontIndirect(&lf);
}

void CXTPShortcutBarPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);
	CreateFonts();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_xtpFontPaneItemCaption);
	m_nPaneItemHeight = max(XTP_DPI_Y(19), dc.GetTextExtent(_T(" "), 1).cy + XTP_DPI_Y(4));

	m_clrPushedText.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_clrSelectedText.SetStandardValue(GetXtremeColor(XPCOLOR_CHECKED_TEXT));
	m_clrHotText.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrNormalText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrDisabledText.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	m_clrPopupFrame.SetStandardValue(GetXtremeColor(COLOR_BTNSHADOW));
	m_clrPopupBackground.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	m_clrFlatBackground  = GetXtremeColor(COLOR_WINDOW);
	m_clrFlatTextColor   = GetXtremeColor(COLOR_WINDOWTEXT);
	m_clrAppSplitterFace = GetXtremeColor(COLOR_3DFACE);
}

void CXTPShortcutBarPaintManager::DrawPaneCaptionMinimizeButton(CDC* /*pDC*/,
																CXTPShortcutBarPane* /*pPane*/)
{
}

void CXTPShortcutBarPaintManager::DrawExpandButton(CDC* pDC, CRect rc)
{
	CPoint ptImage(rc.CenterPoint().x - XTP_DPI_X(8), rc.CenterPoint().y - XTP_DPI_Y(8));

	if (m_pExpandIcon)
	{
		CSize sz(16, 16);
		if (m_pExpandIcon->IsVectorIcon() || XTPDpiHelper()->IsDpiBitmapScalingEnabled())
			sz = XTP_DPI(sz);

		m_pExpandIcon->Draw(pDC, ptImage, sz);
	}
}

BOOL CXTPShortcutBarPaintManager::IsShortcutItemTruncated(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	if (!pItem->IsExpanded())
		return FALSE;

	int nTextWidth = pItem->GetItemRect().Width() - XTP_DPI_X(5);

	CXTPImageManagerIcon* pImage = pItem->GetImage(m_nExpandedIconWidth);
	if (pImage)
	{
		nTextWidth -= m_nExpandedIconWidth + XTP_DPI_X(5);
	}

	CRect rcText(0, 0, 0, 0);
	CXTPFontDC font(pDC, &m_xtpFontText);
	pDC->DrawText(pItem->GetCaption(), rcText,
				  UINT(DT_LEFT | DT_SINGLELINE | DT_CALCRECT
					   | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));

	return rcText.Width() > nTextWidth;
}

void CXTPShortcutBarPaintManager::DrawNavigationItem(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CRect rc = pItem->GetItemRect();

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(m_clrNormalText);
	FillNavigationItemEntry(pDC, pItem);

	CXTPFontDC font(pDC, &m_xtpFontVerticalText);

	CSize sz = pDC->GetTextExtent(pItem->GetCaption());

	CRect rcText(CPoint((rc.left + rc.right - sz.cy) / 2,
						min(rc.bottom - XTP_DPI_Y(3), (rc.top + rc.bottom + sz.cx) / 2)),
				 CSize(sz.cy, sz.cx));

	pDC->IntersectClipRect(rc);

	if (m_bNavigationButton)
	{
		CXTPImageManagerIcon* pIcon = m_pImageManager->GetImage(IDR_NAVIGATION_BUTTON,
																GetSystemMetrics(SM_CXSMICON));
		if (pIcon)
		{
			CSize szIcon(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
			CPoint ptCenter = rc.CenterPoint();
			ptCenter.Offset(-(szIcon.cx / 2), -(szIcon.cy / 2));
			pIcon->Draw(pDC, ptCenter, szIcon);
		}
	}
	else
	{
		pDC->DrawText(pItem->GetCaption(), rcText,
					  UINT(DT_SINGLELINE | DT_NOCLIP | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));
	}

	pDC->SelectClipRgn(NULL);
}

void CXTPShortcutBarPaintManager::DrawShortcutItem(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CRect rc = pItem->GetItemRect();

	CXTPShortcutBar* pBar = pItem->GetShortcutBar();

	pDC->SetTextColor(pItem->IsEnabled() ? m_clrNormalText : m_clrDisabledText);
	FillShortcutItemEntry(pDC, pItem);

	CSize szIcon(pItem->IsExpanded() ? m_nExpandedIconWidth : m_nNormalIconWidth, 0);
	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);

	BOOL bSelected = pBar->GetHotItem() == pItem
					 && (pBar->GetPressedItem() == NULL || pBar->GetPressedItem() == pItem),
		 bPressed = pBar->GetPressedItem() == pItem, bChecked = pItem->IsSelected();

	if (pItem->IsExpanded())
	{
		CRect rcText(rc);
		rcText.left = XTP_DPI_X(5);

		if (pImage)
		{
			CPoint ptImage(XTP_DPI_X(5), rc.CenterPoint().y - m_nExpandedIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked,
					  pItem->IsEnabled());

			rcText.left = XTP_DPI_X(5) * 2 + m_nExpandedIconWidth;
		}

		pDC->SetBkMode(TRANSPARENT);

		CXTPFontDC font(pDC, &m_xtpFontText);
		pDC->DrawText(pItem->GetCaption(), rcText,
					  UINT(DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS
						   | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));
	}
	else
	{
		if (pItem->IsItemExpandButton())
		{
			if (pBar->GetPressedItem() == pItem)
			{
				pDC->SetTextColor(m_clrPushedText);
			}
			else
			{
				pDC->SetTextColor(m_clrNormalText);
			}

			DrawExpandButton(pDC, rc);
		}

		else if (pImage)
		{
			CPoint ptImage(rc.CenterPoint().x - m_nNormalIconWidth / 2,
						   rc.CenterPoint().y - m_nNormalIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked,
					  pItem->IsEnabled());
		}
	}
}

int CXTPShortcutBarPaintManager::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_xtpFontPaneCaption);

		CXTPClientRect rc(pPane);
		rc.bottom = rc.top + XTP_DPI_Y(24);
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));

		CRect rcText(rc);
		rcText.DeflateRect(m_rcCaptionPadding);
		pDC->DrawText(pPane->GetCaption(), rcText,
					  UINT(DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS
						   | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));
	}

	return XTP_DPI_Y(24);
}

int CXTPShortcutBarPaintManager::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem,
													 BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_xtpFontPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

		CRect rcText(rc);
		rcText.DeflateRect(m_rcItemCaptionPadding);
		pDC->DrawText(pItem->GetCaption(), rcText,
					  UINT(DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS
						   | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0)));
	}

	return m_nPaneItemHeight;
}

void CXTPShortcutBarPaintManager::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XTPToUIntChecked(nBrush)));
	pDC->Draw3dRect(rc, GetXtremeColor(XTPToUIntChecked(nPen)),
					GetXtremeColor(XTPToUIntChecked(nPen)));
}
void CXTPShortcutBarPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(XTPToUIntChecked(nPen)));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}

void CXTPShortcutBarPaintManager::FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	FillShortcutItemEntry(pDC, pItem);
}

void CXTPShortcutBarPaintManager::FillPopupBack(CDC* pDC, CRect rc, CSize szGap)
{
	if (pDC)
	{
		pDC->FillSolidRect(rc, m_clrPopupBackground);
		pDC->Draw3dRect(rc, m_clrPopupFrame, m_clrPopupFrame);
		pDC->Draw3dRect(szGap.cx - XTP_DPI_X(1), szGap.cy - XTP_DPI_Y(1),
						rc.Width() - szGap.cx * 2 + XTP_DPI_X(2),
						rc.Height() - szGap.cy * 2 + XTP_DPI_Y(2), m_clrPopupFrame,
						m_clrPopupFrame);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPShortcutBarPaintManager, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "NormalIconWidth", 1, m_nNormalIconWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "ExpandedIconWidth", 2, m_nExpandedIconWidth,
					 VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPShortcutBarPaintManager, "ClearTypeTextQuality", 4,
							m_bClearTypeTextQuality, OleRefreshMetrics, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "Font", DISPID_FONT, OleGetFont, OleSetFont,
						VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "PaneBackgroundColor", 5,
						OleGetFlatBackgroundColor, SetNotSupported, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "PaneTextColor", 6, OleGetFlatTextColor,
						SetNotSupported, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "SplitterBackgroundColor", 7,
						OleGetSplitterColor, SetNotSupported, VT_COLOR)

	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "DrawTextNoPrefix", 8, m_bDrawTextNoPrefix,
					 VT_BOOL)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPShortcutBarPaintManager, XTPDIID_IShortcutBarPaintManager)

BEGIN_INTERFACE_MAP(CXTPShortcutBarPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPShortcutBarPaintManager, XTPDIID_IShortcutBarPaintManager, Dispatch)
END_INTERFACE_MAP()

void CXTPShortcutBarPaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPShortcutBarPaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_bUseStandardFont = FALSE;
		m_xtpFontText.DeleteObject();
		m_xtpFontText.CreateFontIndirect(&lf);
	}
	else
	{
		m_bUseStandardFont = TRUE;
	}
	RefreshMetrics();
}

LPFONTDISP CXTPShortcutBarPaintManager::OleGetFont()
{
	return AxCreateOleFont(&m_xtpFontText, this,
						   (LPFNFONTCHANGED)&CXTPShortcutBarPaintManager::OleSetFont);
}

OLE_COLOR CXTPShortcutBarPaintManager::OleGetFlatBackgroundColor()
{
	return m_clrFlatBackground;
}

OLE_COLOR CXTPShortcutBarPaintManager::OleGetFlatTextColor()
{
	return m_clrFlatTextColor;
}

OLE_COLOR CXTPShortcutBarPaintManager::OleGetSplitterColor()
{
	return m_clrAppSplitterFace;
}

#endif
