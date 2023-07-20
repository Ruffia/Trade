// XTPExcelTabCtrlTheme.cpp: implementation of the CXTPExcelTabCtrlTheme class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Scroll/XTPScrollBar.h"
#include "Controls/Tab/XTPExcelTabCtrl.h"
#include "Controls/Tab/XTPExcelTabCtrlTheme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlTheme class
//

CXTPExcelTabCtrlTheme::CXTPExcelTabCtrlTheme()
{
}

CXTPExcelTabCtrlTheme::~CXTPExcelTabCtrlTheme()
{
}

void CXTPExcelTabCtrlTheme::RefreshMetrics(CXTPExcelTabCtrl* pTab)
{
	UNREFERENCED_PARAMETER(pTab);

	m_clr3DFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clr3DFacePushed.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clr3DFaceHilite.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrBtnText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrBtnTextPushed.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrBtnTextHilite.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrBtnTextGray.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	m_clrWindow.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrWindowText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clr3DHilight.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
	m_clr3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrBorder3DHilite.SetStandardValue(GetXtremeColor(COLOR_3DHILIGHT));
	m_clrBorder3DShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
}

COLORREF CXTPExcelTabCtrlTheme::GetTabBackColor(CXTPTcbItem* pTcbItem) const
{
	return (pTcbItem->crTabBack == COLORREF_NULL) ? m_clr3DFace.operator COLORREF()
												  : pTcbItem->crTabBack;
}

COLORREF CXTPExcelTabCtrlTheme::GetTabTextColor(CXTPTcbItem* pTcbItem) const
{
	return (pTcbItem->crTabText == COLORREF_NULL) ? m_clrBtnText.operator COLORREF()
												  : pTcbItem->crTabText;
}

COLORREF CXTPExcelTabCtrlTheme::GetSelTabBackColor(CXTPTcbItem* pTcbItem) const
{
	return (pTcbItem->crTabSelBack == COLORREF_NULL) ? m_clrWindow.operator COLORREF()
													 : pTcbItem->crTabSelBack;
}

COLORREF CXTPExcelTabCtrlTheme::GetSelTabTextColor(CXTPTcbItem* pTcbItem) const
{
	return (pTcbItem->crTabSelText == COLORREF_NULL) ? m_clrWindowText.operator COLORREF()
													 : pTcbItem->crTabSelText;
}

//////////////////////////////////////////////////////////////////////////
//
//     Height ************************
//          \*| |                  |  *
//          *\| |xxxxxxxxxxxxxxxxxx|   *
//         *  | |         |        |    *
//        *---+-+         |        +-----*
//           |  \       Text
//         Base  \
//                Margin
//
//      Base = Height / 2
//      Margin = Base / 2
//

int CXTPExcelTabCtrlTheme::DrawTab(CDC* pDC, CXTPExcelTabCtrl* pTabCtrl, const CPoint& pt,
								   bool bSelected, CXTPTcbItem* pTcbItem)
{
	if (pDC == NULL || pTcbItem == NULL || !::IsWindow(pTabCtrl->GetSafeHwnd()))
		return -1;

	CXTPDCSaveState dcState(pDC);

	const int iHeight = pTabCtrl->GetTabHeight();
	const int iBase   = iHeight / 2;
	const int iWidth  = iHeight + iHeight / 2; // 2 bases + 2 margins

	const bool bBottom = (pTabCtrl->GetTabStyle() & FTS_XTP_BOTTOM) == FTS_XTP_BOTTOM;

	COLORREF crBack = GetTabBackColor(pTcbItem);
	COLORREF crFore = GetTabTextColor(pTcbItem);

	if (bSelected)
	{
		crBack = GetSelTabBackColor(pTcbItem);
		crFore = GetSelTabTextColor(pTcbItem);
	}

	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crFore);

	CXTPFontDC font(pDC, pTabCtrl->GetTabFont(bSelected), crFore);
	CXTPPenDC pen(*pDC, PS_SOLID, 1, m_clrBtnText);
	CXTPBrushDC brush(*pDC, crBack);

	const CSize szText = pDC->GetTextExtent(pTcbItem->szTabLabel);

	CRect rcText;
	rcText.left   = pt.x + iBase + iBase / 2;
	rcText.top	= pt.y + ((iHeight - szText.cy) / 2) - XTP_DPI_Y(1);
	rcText.right  = rcText.left + szText.cx;
	rcText.bottom = rcText.top + szText.cy;

	if (pTabCtrl->GetTabStyle() & FTS_XTP_SQUARETABS)
	{
		int y[] = { bBottom ? pt.y : pt.y + iHeight - XTP_DPI_Y(1),
					bBottom ? pt.y + iHeight - XTP_DPI_Y(1) : pt.y };
		int x[] = { rcText.left - iBase, rcText.right + iBase };

		pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER);

		if (bSelected)
		{
			// shadow line on top
			pen.Color(m_clr3DShadow);
			pDC->MoveTo(pt.x, y[0]);
			pDC->LineTo(pt.x + szText.cx + iWidth, y[0]);

			// fill tab and draw text.
			pDC->FillSolidRect(rcText.left - iBase, y[0], szText.cx + iBase * 2, y[1], m_clrWindow);
			pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER | DT_VCENTER);

			// tab borders
			pen.Color(m_clrBtnText);
			pDC->MoveTo(x[0], y[0]);
			pDC->LineTo(x[0], y[1]);
			pDC->MoveTo(x[1], y[0]);
			pDC->LineTo(x[1], y[1]);

			pen.Color(m_clr3DShadow);
			pDC->MoveTo(x[0], y[1]);
			pDC->LineTo(x[1] + XTP_DPI_X(1), y[1]);
		}
		else
		{
			// draw text.
			pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER | DT_VCENTER);

			// shadow line on top
			pen.Color(m_clr3DShadow);
			pDC->MoveTo(pt.x, y[0]);
			pDC->LineTo(pt.x + szText.cx + iWidth, y[0]);

			// bottom
			pDC->MoveTo(x[0], y[1]);
			pDC->LineTo(x[1], y[1]);

			// left / right tab border
			pen.Color(m_clrBtnText);
			pDC->MoveTo(x[0], y[0] + XTP_DPI_Y(1));
			pDC->LineTo(x[0], y[1]);
			pDC->MoveTo(x[1] - XTP_DPI_X(1), y[0] + XTP_DPI_Y(1));
			pDC->LineTo(x[1] - XTP_DPI_X(1), y[1]);

			// highlight
			pen.Color(m_clr3DHilight);
			pDC->MoveTo(x[0] + XTP_DPI_X(1), y[0] + XTP_DPI_Y(1));
			pDC->LineTo(x[0] + XTP_DPI_X(1), y[1]);
		}
	}
	else
	{
		POINT points[] = { { pt.x, pt.y + iHeight - XTP_DPI_Y(1) },
						   { pt.x + iBase - XTP_DPI_X(1), pt.y },
						   { pt.x + szText.cx + iWidth - iBase - XTP_DPI_X(1), pt.y },
						   { pt.x + szText.cx + iWidth - XTP_DPI_X(1),
							 pt.y + iHeight - XTP_DPI_Y(1) } };

		// swap vertical coordinates
		if (bBottom)
		{
			points[0].y = points[1].y;
			points[2].y = points[3].y;
			points[1].y = points[2].y;
			points[3].y = points[0].y;
		}

		pDC->Polygon(points, 4);
		pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER);
		pen.Color(m_clr3DShadow);

		if (bSelected)
		{
			// shadow line on bottom
			pDC->MoveTo(pt.x + iBase, points[1].y);
			pDC->LineTo(pt.x + iBase * 2 + szText.cx - XTP_DPI_X(1), points[1].y);

			// white line on top
			pen.Color(crBack);
			pDC->MoveTo(pt.x + XTP_DPI_X(1), points[0].y);
			pDC->LineTo(pt.x + szText.cx + iWidth - XTP_DPI_X(1), points[0].y);
		}
		else
		{
			// highlight line on left
			pen.Color(m_clr3DHilight);
			pDC->MoveTo(pt.x + XTP_DPI_X(1), points[0].y);
			pDC->LineTo(pt.x + XTP_DPI_X(1) + iBase, points[0].y + iHeight);

			// shadow line on top
			pen.Color(m_clr3DShadow);
			pDC->MoveTo(pt.x, points[0].y);
			pDC->LineTo(pt.x + szText.cx + iWidth, points[0].y);

			// shadow line on bottom
			pDC->MoveTo(pt.x + iBase, points[1].y);
			pDC->LineTo(pt.x + szText.cx + iHeight - XTP_DPI_X(1), points[1].y);
		}
	}

	return szText.cx + iWidth;
}

void CXTPExcelTabCtrlTheme::DrawButtonBack(CDC* pDC, CRect& rect,
										   const CXTPExcelTabCtrlButtonState& state)
{
	if (state.m_bPressed && state.m_bHilight)
	{
		pDC->Draw3dRect(rect, m_clrBorder3DShadow, m_clrBorder3DHilite);
	}
	else
	{
		pDC->Draw3dRect(rect, m_clrBorder3DHilite, m_clrBorder3DShadow);
	}

	rect.DeflateRect(1, 1);
	pDC->FillSolidRect(rect, m_clr3DFace);
}

void CXTPExcelTabCtrlTheme::DrawButton(CDC* pDC, CXTPExcelTabCtrl* /*pTabCtrl*/,
									   const CXTPExcelTabCtrlButtonState& state)
{
	CRect rc(state.m_rect);
	if (rc.IsRectEmpty())
		return;

	DrawButtonBack(pDC, rc, state);

	COLORREF clrBtnText = m_clrBtnText;

	if (state.m_bEnabled)
	{
		if (state.m_bPressed)
		{
			clrBtnText = m_clrBtnTextPushed;
		}
		else if (state.m_bHilight)
		{
			clrBtnText = m_clrBtnTextHilite;
		}
	}
	else
	{
		clrBtnText = m_clrBtnTextGray;
	}

	XTPPrimitiveSymbol nSymbol = xtpPrimitiveSymbolInvalid;
	switch (state.m_IconType)
	{
		case xtpArrowIconLeft:
			nSymbol = xtpPrimitiveSymbolPlayLeft;
			rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
			break;
		case xtpArrowIconLeftHome: nSymbol = xtpPrimitiveSymbolInjectLeft; break;
		case xtpArrowIconRight:
			nSymbol = xtpPrimitiveSymbolPlayRight;
			rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
			break;
		case xtpArrowIconRightHome: nSymbol = xtpPrimitiveSymbolInjectRight; break;
	}

	CSize sz = rc.Size();
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rc, clrBtnText);
}

void CXTPExcelTabCtrlTheme::FillBackground(CDC* pDC, CRect& rect, BOOL bBottom)
{
	pDC->FillSolidRect(rect, m_clr3DFace);

	pDC->FillSolidRect(0, bBottom ? rect.top : rect.bottom - XTP_DPI_Y(1), rect.Width(),
					   XTP_DPI_Y(1), m_clr3DShadow);
}

void CXTPExcelTabCtrlTheme::FillTabsBackground(CDC* pDC, int cx, int cy)
{
	CRect rcTabs(0, 0, cx, cy);
	pDC->FillSolidRect(rcTabs, m_clr3DFace);
}

void CXTPExcelTabCtrlTheme::DrawGripper(CDC* pDC, CRect rect)
{
	pDC->Draw3dRect(rect, m_clr3DHilight, m_clr3DShadow);
	rect.InflateRect(-CX_BORDER, -CY_BORDER);

	// fill the middle
	pDC->FillSolidRect(rect, m_clr3DFace);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeOfficeXP class
//

CXTPExcelTabCtrlThemeOfficeXP::CXTPExcelTabCtrlThemeOfficeXP()
{
	m_nTheme = xtpControlThemeOfficeXP;
}

void CXTPExcelTabCtrlThemeOfficeXP::RefreshMetrics(CXTPExcelTabCtrl* pTab)
{
	CXTPExcelTabCtrlTheme::RefreshMetrics(pTab);

	m_clr3DFacePushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clr3DFaceHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_clrBtnText.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_TEXT));
	m_clrBtnTextPushed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_clrBtnTextHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrBtnTextGray.SetStandardValue(GetXtremeColor(XPCOLOR_GRAYTEXT));
	m_clrBorder3DHilite.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	m_clrBorder3DShadow.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
}

void CXTPExcelTabCtrlThemeOfficeXP::DrawButtonBack(CDC* pDC, CRect& rect,
												   const CXTPExcelTabCtrlButtonState& state)
{
	if (state.m_bEnabled && state.m_bHilight)
	{
		if (state.m_bPressed)
		{
			pDC->FillSolidRect(rect, m_clr3DFacePushed);
			pDC->Draw3dRect(rect, m_clrBorder3DHilite, m_clrBorder3DHilite);
		}
		else
		{
			pDC->FillSolidRect(rect, m_clr3DFaceHilite);
			pDC->Draw3dRect(rect, m_clrBorder3DHilite, m_clrBorder3DHilite);
		}
	}
	else
	{
		pDC->FillSolidRect(rect, m_clr3DFace);
		pDC->Draw3dRect(rect, m_clr3DFace, m_clr3DFace);
	}

	rect.DeflateRect(1, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeOffice2003 class
//

CXTPExcelTabCtrlThemeOffice2003::CXTPExcelTabCtrlThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

void CXTPExcelTabCtrlThemeOffice2003::RefreshMetrics(CXTPExcelTabCtrl* pTab)
{
	CXTPExcelTabCtrlThemeOfficeXP::RefreshMetrics(pTab);

	if (XTPColorManager()->GetCurrentSystemTheme() != xtpSystemThemeUnknown)
	{
		m_clr3DFacePushed.SetStandardValue(GetMSO2003Color(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clr3DFaceHilite.SetStandardValue(GetMSO2003Color(XPCOLOR_HIGHLIGHT));
		m_clrBtnTextPushed.SetStandardValue(GetMSO2003Color(XPCOLOR_PUSHED_TEXT));
		m_clrBtnTextHilite.SetStandardValue(GetMSO2003Color(XPCOLOR_HIGHLIGHT_TEXT));
		m_clrBorder3DShadow.SetStandardValue(GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER));
		m_clrBorder3DHilite.SetStandardValue(GetMSO2003Color(XPCOLOR_HIGHLIGHT_BORDER));
	}
}

void CXTPExcelTabCtrlThemeOffice2003::DrawButtonBack(CDC* pDC, CRect& rect,
													 const CXTPExcelTabCtrlButtonState& state)
{
	if (state.m_bEnabled && state.m_bHilight)
	{
		if (state.m_bPressed)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rect, XTPColorManager()->grcLunaPushed, FALSE);
		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, &rect, XTPColorManager()->grcLunaSelected, FALSE);
		}

		pDC->Draw3dRect(&rect, m_clrBorder3DHilite, m_clrBorder3DHilite);
	}
	else
	{
		pDC->FillSolidRect(rect, m_clr3DFace);
		pDC->Draw3dRect(rect, m_clr3DFace, m_clr3DFace);
	}

	rect.DeflateRect(1, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeOffice2013 class
//

CXTPExcelTabCtrlThemeOffice2013::CXTPExcelTabCtrlThemeOffice2013()
{
	m_nTheme = xtpControlThemeOffice2013;
}

void CXTPExcelTabCtrlThemeOffice2013::RefreshMetrics(CXTPExcelTabCtrl* pTab)
{
	UNREFERENCED_PARAMETER(pTab);

	COLORREF crTabFaceNormal   = XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TabFaceNormal"),
											 RGB(245, 245, 245));
	COLORREF crTabFaceSelected = XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TabFaceSelected"),
											 RGB(255, 255, 255));
	COLORREF crTabBorder	   = XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TabBorder"),
										 RGB(212, 212, 212));
	COLORREF crTextNormal	  = XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TextNormal"),
										 RGB(152, 152, 152));
	COLORREF crTextSelected	= XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TextSelected"),
											 RGB(43, 87, 154));
	COLORREF crTextDisabled	= XTPIniColor(_T("Controls.ExcelTabCtrl"), _T("TextDisabled"),
											 RGB(212, 212, 212));

	m_clr3DFace.SetStandardValue(crTabFaceNormal);
	m_clr3DFacePushed.SetStandardValue(crTabFaceNormal);
	m_clr3DFaceHilite.SetStandardValue(crTabFaceNormal);
	m_clrBtnText.SetStandardValue(crTextNormal);
	m_clrBtnTextPushed.SetStandardValue(crTextNormal);
	m_clrBtnTextHilite.SetStandardValue(crTextNormal);
	m_clrBtnTextGray.SetStandardValue(crTextDisabled);
	m_clrWindow.SetStandardValue(crTabFaceSelected);
	m_clrWindowText.SetStandardValue(crTextSelected);
	m_clr3DHilight.SetStandardValue(crTabFaceNormal);
	m_clr3DShadow.SetStandardValue(crTabBorder);
	m_clrBorder3DHilite.SetStandardValue(crTabFaceNormal);
	m_clrBorder3DShadow.SetStandardValue(crTabBorder);
}

int CXTPExcelTabCtrlThemeOffice2013::DrawTab(CDC* pDC, CXTPExcelTabCtrl* pTabCtrl, const CPoint& pt,
											 bool bSelected, CXTPTcbItem* pTcbItem)
{
	if (pDC == NULL || pTcbItem == NULL || !::IsWindow(pTabCtrl->GetSafeHwnd()))
		return -1;

	CXTPDCSaveState dcState(pDC);

	const int iHeight   = pTabCtrl->GetTabHeight();
	const int iBase		= iHeight / 2;
	const int iWidth	= iHeight + iHeight / 2; // 2 bases + 2 margins
	const int iBorderLR = XTP_DPI_X(1);			 // selected item LR border
	const int iBorderTB = XTP_DPI_Y(2);			 // selected item top/bottom border

	const bool bBottom = (pTabCtrl->GetTabStyle() & FTS_XTP_BOTTOM) == FTS_XTP_BOTTOM;

	COLORREF crBack = bSelected ? GetSelTabBackColor(pTcbItem) : GetTabBackColor(pTcbItem);
	COLORREF crFore = bSelected ? GetSelTabTextColor(pTcbItem) : GetTabTextColor(pTcbItem);

	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crFore);

	CXTPFontDC font(pDC, pTabCtrl->GetTabFont(bSelected), crFore);
	CXTPPenDC pen(*pDC, PS_SOLID, 1, m_clr3DShadow);
	CXTPBrushDC brush(*pDC, crBack);

	int iGap = XTP_DPI_Y(1);

	if (pTabCtrl->GetTabHeight() > ::GetSystemMetrics(SM_CYHSCROLL))
		iGap += XTP_DPI_Y(2);

	const CSize szText = pDC->GetTextExtent(pTcbItem->szTabLabel);

	CRect rcText;
	rcText.left  = pt.x + iBase + iBase / 2;
	rcText.right = rcText.left + szText.cx;
	const int dy = (iHeight - (iGap + iBorderTB + szText.cy)) / 2;
	if (bBottom)
	{
		rcText.top	= pt.y + dy;
		rcText.bottom = rcText.top + szText.cy;
	}
	else
	{
		rcText.bottom = iHeight + pt.y - dy;
		rcText.top	= rcText.bottom - szText.cy;
	}

	if (pTabCtrl->GetTabStyle() & FTS_XTP_SQUARETABS)
	{
		CRect rc;
		rc.left   = rcText.left - iBase;
		rc.right  = rcText.right + iBase;
		rc.top	= pt.y;
		rc.bottom = pt.y + iHeight;

		if (bSelected)
		{
			// draw top or bottom border
			{
				CRect rcTB(rc);
				rcTB.left -= iBase;
				rcTB.right += iBase;
				rcTB.DeflateRect(0, bBottom ? 0 : rcTB.Height() - XTP_DPI_Y(1), 0,
								 bBottom ? rcTB.Height() - XTP_DPI_Y(1) : 0);

				pDC->FillSolidRect(rcTB, m_clr3DShadow);
			}

			// fill tab
			rc.DeflateRect(0, bBottom ? 0 : iGap, 0, bBottom ? iGap : 0);
			pDC->FillSolidRect(rc, m_clr3DShadow);

			// draw left and right borders
			{
				CRect rcLR(rc);
				rcLR.DeflateRect(iBorderLR, 0, iBorderLR, 0);
				pDC->FillSolidRect(rcLR, crBack);
			}

			// draw text
			pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER | DT_VCENTER);

			// draw top or bottom border
			{
				CRect rcTB(rc);
				rcTB.DeflateRect(0, bBottom ? rcTB.Height() - iBorderTB : 0, 0,
								 bBottom ? 0 : rcTB.Height() - iBorderTB);

				pDC->FillSolidRect(rcTB, crFore);
			}
		}
		else
		{
			// draw text.
			pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER | DT_VCENTER);

			{
				CRect rcTB(rc);
				rcTB.left -= iBase;
				rcTB.right += iBase;
				rcTB.DeflateRect(0, bBottom ? 0 : rcTB.Height() - XTP_DPI_Y(1), 0,
								 bBottom ? rcTB.Height() - XTP_DPI_Y(1) : 0);

				pDC->FillSolidRect(rcTB, m_clr3DShadow);
			}

			// draw left border
			pDC->MoveTo(rc.left, rc.top + XTP_DPI_Y(4));
			pDC->LineTo(rc.left, rc.bottom - XTP_DPI_Y(4));
		}
	}
	else
	{
		POINT points[] = { { pt.x, pt.y + iHeight - iGap },
						   { pt.x + iBase - XTP_DPI_X(1), pt.y },
						   { pt.x + szText.cx + iWidth - iBase - XTP_DPI_X(1), pt.y },
						   { pt.x + szText.cx + iWidth - XTP_DPI_X(1), pt.y + iHeight - iGap } };

		// swap vertical coordinates
		if (bBottom)
		{
			points[0].y = points[1].y;
			points[2].y = points[3].y;
			points[1].y = points[2].y;
			points[3].y = points[0].y;
		}

		pDC->Polygon(points, 4);
		pDC->DrawText(pTcbItem->szTabLabel, rcText, DT_CENTER);

		if (bSelected)
		{
			// bottom line
			pen.Color(crFore);
			pDC->MoveTo(pt.x + iBase, points[1].y);
			pDC->LineTo(pt.x + iBase * 2 + szText.cx, points[1].y);
			pDC->MoveTo(pt.x + iBase, points[1].y - XTP_DPI_Y(1));
			pDC->LineTo(pt.x + iBase * 2 + szText.cx, points[1].y - XTP_DPI_Y(1));

			// top line.
			pen.Color(crBack);
			pDC->MoveTo(pt.x + XTP_DPI_X(1), points[0].y);
			pDC->LineTo(pt.x + szText.cx + iWidth - XTP_DPI_X(1), points[0].y);
		}
		else
		{
			// border line on top
			pen.Color(m_clr3DShadow);
			pDC->MoveTo(pt.x, points[0].y);
			pDC->LineTo(pt.x + szText.cx + iWidth, points[0].y);

			// border line on bottom
			pDC->MoveTo(pt.x + iBase, points[1].y);
			pDC->LineTo(pt.x + szText.cx + iHeight - XTP_DPI_X(1), points[1].y);
		}
	}

	return szText.cx + iWidth;
}

void CXTPExcelTabCtrlThemeOffice2013::DrawButtonBack(CDC* pDC, CRect& rect,
													 const CXTPExcelTabCtrlButtonState& state)
{
	UNREFERENCED_PARAMETER(state);
	pDC->FillSolidRect(rect, m_clr3DFace);
	rect.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));
}

void CXTPExcelTabCtrlThemeOffice2013::DrawButton(CDC* pDC, CXTPExcelTabCtrl* /*pTabCtrl*/,
												 const CXTPExcelTabCtrlButtonState& state)
{
	CRect rc(state.m_rect);
	if (rc.IsRectEmpty())
		return;

	DrawButtonBack(pDC, rc, state);

	XTPPrimitiveSymbol nSymbol = xtpPrimitiveSymbolInvalid;
	switch (state.m_IconType)
	{
		case xtpArrowIconLeft:
			nSymbol = xtpPrimitiveSymbolPlayLeft;
			rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
			break;
		case xtpArrowIconLeftHome: nSymbol = xtpPrimitiveSymbolInjectLeft; break;
		case xtpArrowIconRight:
			nSymbol = xtpPrimitiveSymbolPlayRight;
			rc.InflateRect(XTP_DPI_X(1), XTP_DPI_Y(1), XTP_DPI_X(1), XTP_DPI_Y(1));
			break;
		case xtpArrowIconRightHome: nSymbol = xtpPrimitiveSymbolInjectRight; break;
	}

	CSize sz = rc.Size();
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rc,
									 state.m_bEnabled ? m_clrWindowText : m_clrBtnTextGray);
}

void CXTPExcelTabCtrlThemeOffice2013::FillBackground(CDC* pDC, CRect& rect, BOOL bBottom)
{
	pDC->FillSolidRect(rect, m_clr3DFace);
	pDC->FillSolidRect(rect.left, bBottom ? rect.top : rect.bottom - XTP_DPI_Y(1), rect.Width(),
					   XTP_DPI_Y(1), m_clrBorder3DShadow);
}

void CXTPExcelTabCtrlThemeOffice2013::FillTabsBackground(CDC* pDC, int cx, int cy)
{
	CRect rcTabs(0, 0, cx, cy);
	pDC->FillSolidRect(rcTabs, m_clr3DFace);
}

void CXTPExcelTabCtrlThemeOffice2013::DrawGripper(CDC* pDC, CRect rect)
{
	int y = rect.top + rect.Height() / 2;
	int x = rect.left;

	rect.DeflateRect(-XTP_DPI_X(4), XTP_DPI_Y(1), 0, XTP_DPI_Y(1));
	pDC->FillSolidRect(rect, m_clr3DFace);

	CRect rGripper(x, y, x + XTP_DPI_X(2), y + XTP_DPI_Y(2));

	rGripper.OffsetRect(0, -XTP_DPI_Y(4));
	pDC->FillSolidRect(rGripper, m_clrBorder3DShadow);

	rGripper.OffsetRect(0, XTP_DPI_Y(4));
	pDC->FillSolidRect(rGripper, m_clrBorder3DShadow);

	rGripper.OffsetRect(0, XTP_DPI_Y(4));
	pDC->FillSolidRect(rGripper, m_clrBorder3DShadow);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeVisualStudio2015 class
//

CXTPExcelTabCtrlThemeVisualStudio2015::CXTPExcelTabCtrlThemeVisualStudio2015()
{
	m_nTheme = xtpControlThemeVisualStudio2015;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeVisualStudio2017 class
//

CXTPExcelTabCtrlThemeVisualStudio2017::CXTPExcelTabCtrlThemeVisualStudio2017()
{
	m_nTheme = xtpControlThemeVisualStudio2017;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeVisualStudio2019 class
//

CXTPExcelTabCtrlThemeVisualStudio2019::CXTPExcelTabCtrlThemeVisualStudio2019()
{
	m_nTheme = xtpControlThemeVisualStudio2019;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeVisualStudio2022 class
//

CXTPExcelTabCtrlThemeVisualStudio2022::CXTPExcelTabCtrlThemeVisualStudio2022()
{
	m_nTheme = xtpControlThemeVisualStudio2022;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPExcelTabCtrlThemeNativeWindows10 class
//

CXTPExcelTabCtrlThemeNativeWindows10::CXTPExcelTabCtrlThemeNativeWindows10()
{
	m_nTheme = xtpControlThemeNativeWindows10;
}
