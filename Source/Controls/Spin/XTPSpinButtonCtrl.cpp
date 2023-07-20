// XTPSpinButtonCtrl.cpp : implementation of the CXTPSpinButtonCtrl class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Defines.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/Spin/XTPSpinButtonCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UD_HITNOWHERE 0
#define UD_HITDOWN 1
#define UD_HITUP 2

IMPLEMENT_DYNAMIC(CXTPSpinButtonCtrl, CSpinButtonCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTPMaskEdit
/////////////////////////////////////////////////////////////////////////////

CXTPSpinButtonCtrl::CXTPSpinButtonCtrl()
{
	m_bUseVisualStyle = TRUE;
	m_nTheme		  = xtpControlThemeDefault;

	m_nHotButton	 = UD_HITNOWHERE;
	m_nPressedButton = UD_HITNOWHERE;

	m_bPreSubclassInit = TRUE;

	m_themeSpin = new CXTPWinThemeWrapper();
}

CXTPSpinButtonCtrl::~CXTPSpinButtonCtrl()
{
	SAFE_DELETE(m_themeSpin);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CXTPSpinButtonCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPSpinButtonCtrl::Init()
{
	RefreshMetrics();

	Invalidate(FALSE);
}

void CXTPSpinButtonCtrl::PreSubclassWindow()
{
	CSpinButtonCtrl::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPSpinButtonCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPSpinButtonCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CSpinButtonCtrl::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPSpinButtonCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPBufferDC dcMem(dc);
	OnDraw(&dcMem);
}

CXTPEdit* CXTPSpinButtonCtrl::GetEditBuddy() const
{
	HWND hWndBuddy = (HWND)::SendMessage(m_hWnd, UDM_GETBUDDY, 0, 0);
	if (!hWndBuddy)
		return FALSE;

	CXTPEdit* pEdit = DYNAMIC_DOWNCAST(CXTPEdit, CWnd::FromHandle(hWndBuddy));
	return pEdit;
}

BOOL CXTPSpinButtonCtrl::HasSharedBorder() const
{
	CXTPEdit* pEdit = GetEditBuddy();
	if (!pEdit)
		return FALSE;

	CXTPWindowRect rcEdit(pEdit);
	CXTPWindowRect rcSpin(this);

	CRect rcIntersect;
	if (!rcIntersect.IntersectRect(rcEdit, rcSpin))
		return FALSE;

	return rcIntersect.Width() == 2;
}

void CXTPSpinButtonCtrl::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	if (HasSharedBorder())
	{
		CXTPEdit* pEditBuddy = GetEditBuddy();
		pEditBuddy->DrawBuddyBorders(this, pDC, rc,
									 GetStyle() & UDS_ALIGNRIGHT ? UDS_ALIGNRIGHT : UDS_ALIGNLEFT);
	}

	if (m_bUseVisualStyle && m_themeSpin->IsAppThemeActive())
	{
		DrawVisualStyle(pDC);
	}
	else if (m_nTheme == xtpControlThemeResource)
	{
		DrawOffice2007(pDC);
	}
	else if (m_nTheme != xtpControlThemeDefault)
	{
		DrawFlat(pDC);
	}
	else
	{
		DrawStandard(pDC);
	}
}

BOOL CXTPSpinButtonCtrl::IsVertical() const
{
	return (GetStyle() & UDS_HORZ) == 0;
}

void CXTPSpinButtonCtrl::DrawStandard(CDC* pDC)
{
	BOOL bEnabled = IsWindowEnabled();

	if (IsVertical())
	{
		UINT uFlags = DFCS_SCROLLUP;
		if (m_nPressedButton == UD_HITUP)
			uFlags |= DFCS_PUSHED;
		if (!bEnabled)
			uFlags |= DFCS_INACTIVE;

		CRect rc = GetButtonRect(UD_HITUP);
		DrawFrameControl(pDC->GetSafeHdc(), &rc, DFC_SCROLL, uFlags);

		uFlags = DFCS_SCROLLDOWN;
		if (m_nPressedButton == UD_HITDOWN)
			uFlags |= DFCS_PUSHED;
		if (!bEnabled)
			uFlags |= DFCS_INACTIVE;

		rc = GetButtonRect(UD_HITDOWN);
		DrawFrameControl(pDC->GetSafeHdc(), &rc, DFC_SCROLL, uFlags);
	}
	else
	{
		UINT uFlags = DFCS_SCROLLLEFT;
		if (m_nPressedButton == UD_HITDOWN)
			uFlags |= DFCS_PUSHED;
		if (!bEnabled)
			uFlags |= DFCS_INACTIVE;

		CRect rc = GetButtonRect(UD_HITDOWN);
		DrawFrameControl(pDC->GetSafeHdc(), &rc, DFC_SCROLL, uFlags);

		uFlags = DFCS_SCROLLRIGHT;
		if (m_nPressedButton == UD_HITUP)
			uFlags |= DFCS_PUSHED;
		if (!bEnabled)
			uFlags |= DFCS_INACTIVE;

		rc = GetButtonRect(UD_HITUP);
		DrawFrameControl(pDC->GetSafeHdc(), &rc, DFC_SCROLL, uFlags);
	}
}

void CXTPSpinButtonCtrl::DrawArrowGlyph(CDC* pDC, CRect rcArrow, BOOL bHorz, BOOL bUpArrow,
										COLORREF clrArrow)
{
	ASSERT(pDC);

	int nHeight = min(rcArrow.Width(), rcArrow.Height());
	if (!(nHeight < XTP_DPI_Y(6)))
	{
		XTPPrimitiveSymbol nSymbol = bHorz ? (!bUpArrow ? xtpPrimitiveSymbolPlayLeft
														: xtpPrimitiveSymbolPlayRight)
										   : (bUpArrow ? xtpPrimitiveSymbolPlayUp
													   : xtpPrimitiveSymbolPlayDown);
		rcArrow.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rcArrow, clrArrow);
	}
}

void CXTPSpinButtonCtrl::DrawFlatButton(CDC* pDC, UINT nButton)
{
	CRect rc = GetButtonRect(XTPToInt(nButton));

	BOOL bEnabled = IsWindowEnabled();
	BOOL bPushed  = (nButton == m_nPressedButton);
	BOOL bHilite  = (nButton == m_nHotButton);

	COLORREF clrBack   = m_crBack;
	COLORREF clrBorder = m_crBorder;
	COLORREF clrArrow  = m_crArrowEnabled;

	if (!bEnabled)
	{
		clrBack   = m_crBackDisabled;
		clrBorder = m_crBorderDisabled;
		clrArrow  = m_crArrowDisabled;
	}
	else if (bPushed)
	{
		clrBack   = m_crBackPushed;
		clrBorder = m_crBorderPushed;
		clrArrow  = m_crArrowPushed;
	}
	else if (bHilite)
	{
		clrBack   = m_crBackHilite;
		clrBorder = m_crBorderHilite;
		clrArrow  = m_crArrowHilite;
	}

	if (m_nTheme != xtpControlThemeOffice2000)
	{
		pDC->FillSolidRect(rc, clrBack);
		pDC->Draw3dRect(rc, clrBorder, clrBorder);

		if (m_nTheme == xtpControlThemeFlat)
		{
			CRect rcBorder(rc);
			rcBorder.DeflateRect(1, 1);
			pDC->Draw3dRect(rcBorder, GetXtremeColor(COLOR_3DHIGHLIGHT),
							GetXtremeColor(COLOR_3DHIGHLIGHT));
		}
	}
	else
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_BTNFACE));

		if (GetEditBuddy())
		{
			rc.left += 1;
		}

		if (m_nPressedButton == nButton)
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
		}
		else
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}

		rc.InflateRect(2, 2);
		rc.OffsetRect(0, -1);
	}

	if (m_nPressedButton == nButton && m_nTheme == xtpControlThemeFlat)
		rc.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	DrawArrowGlyph(pDC, rc, !IsVertical(), nButton == UD_HITUP, clrArrow);
}

void CXTPSpinButtonCtrl::DrawFlat(CDC* pDC)
{
	if (m_nHotButton == UD_HITUP)
	{
		DrawFlatButton(pDC, UD_HITDOWN);
		DrawFlatButton(pDC, UD_HITUP);
	}
	else
	{
		DrawFlatButton(pDC, UD_HITUP);
		DrawFlatButton(pDC, UD_HITDOWN);
	}

	if (m_nTheme == xtpControlThemeOffice2000)
	{
		CRect rc = GetButtonRect(UD_HITDOWN);
		rc.top -= 1;
		rc.bottom = rc.top + 1;
		pDC->FillSolidRect(rc, ::GetSysColor(COLOR_BTNFACE));
	}
}

CRect GetCenterRect(CRect rc, CSize sz, int xOffset = 0, int yOffset = 0)
{
	return CRect(CPoint((rc.left + rc.right - sz.cx) / 2 + xOffset,
						(rc.top + rc.bottom - sz.cy) / 2 + yOffset),
				 sz);
}

void CXTPSpinButtonCtrl::DrawOffice2007(CDC* pDC)
{
	BOOL bEnabled = IsWindowEnabled();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (IsVertical())
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONSPINARROWSVERTICAL"));
		if (!pImage)
			return;

		CRect rcTop	= GetButtonRect(UD_HITUP);
		CRect rcBottom = GetButtonRect(UD_HITDOWN);

		if (rcTop.bottom + 1 == rcBottom.top)
		{
			CRect rcBackSrc(pImage->GetSource(1, 10));
			rcBackSrc.top = rcBackSrc.bottom - 1;
			pImage->DrawImage(pDC, CRect(rcTop.left, rcTop.bottom, rcTop.right, rcBottom.top),
							  rcBackSrc, CRect(2, 0, 2, 0), 0xFF00FF);
		}

		// Background
		{
			int nState = !bEnabled
							 ? 4
							 : m_nPressedButton == UD_HITUP ? 3 : m_nHotButton == UD_HITUP ? 2 : 1;
			pImage->DrawImage(pDC, rcTop, pImage->GetSource(nState, 10), CRect(2, 2, 2, 2),
							  0xFF00FF);

			nState = !bEnabled
						 ? 9
						 : m_nPressedButton == UD_HITDOWN ? 8 : m_nHotButton == UD_HITDOWN ? 7 : 6;
			pImage->DrawImage(pDC, rcBottom, pImage->GetSource(nState, 10), CRect(2, 2, 2, 2),
							  0xFF00FF);
		}

		// Glyphs
		{
			pImage = rcTop.Width() > XTP_DPI_X(12) && rcTop.Height() > XTP_DPI_Y(12)
						 ? pImages->LoadFile(_T("CONTROLGALLERYSCROLLARROWGLYPHS"))
						 : pImages->LoadFile(_T("TOOLBARBUTTONSPINARROWGLYPHS"));

			if (!pImage)
				return;

			int nState = !bEnabled
							 ? 3
							 : m_nPressedButton == UD_HITUP ? 2 : m_nHotButton == UD_HITUP ? 1 : 0;
			CRect rcSrc = pImage->GetSource(nState, 16);
			pImage->DrawImage(pDC, GetCenterRect(rcTop, rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0),
							  0xFF00FF);

			nState = 4
					 + (!bEnabled
							? 3
							: m_nPressedButton == UD_HITUP ? 2 : m_nHotButton == UD_HITUP ? 1 : 0);
			rcSrc = pImage->GetSource(nState, 16);
			pImage->DrawImage(pDC, GetCenterRect(rcBottom, rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0),
							  0xFF00FF);
		}
	}
	else
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONSPINARROWSHORIZONTAL"));
		if (!pImage)
			return;

		CRect rcTop	= GetButtonRect(UD_HITUP);
		CRect rcBottom = GetButtonRect(UD_HITDOWN);

		if (rcBottom.right + 1 == rcTop.left)
		{
			CRect rcBackSrc(pImage->GetSource(1, 10));
			rcBackSrc.left = rcBackSrc.right - 1;
			pImage->DrawImage(pDC, CRect(rcBottom.right, rcTop.top, rcTop.left, rcTop.bottom),
							  rcBackSrc, CRect(0, 2, 0, 2), 0xFF00FF);
		}

		// Background
		{
			int nState = 5
						 + (!bEnabled
								? 4
								: m_nPressedButton == UD_HITUP ? 3
															   : m_nHotButton == UD_HITUP ? 2 : 1);
			pImage->DrawImage(pDC, rcTop, pImage->GetSource(nState, 10), CRect(2, 2, 2, 2),
							  0xFF00FF);

			nState = !bEnabled
						 ? 4
						 : m_nPressedButton == UD_HITDOWN ? 3 : m_nHotButton == UD_HITDOWN ? 2 : 1;
			pImage->DrawImage(pDC, rcBottom, pImage->GetSource(nState, 10), CRect(2, 2, 2, 2),
							  0xFF00FF);
		}

		// Glyphs
		{
			pImage = rcTop.Width() > XTP_DPI_X(12) && rcTop.Height() > XTP_DPI_Y(12)
						 ? pImages->LoadFile(_T("CONTROLGALLERYSCROLLARROWGLYPHS"))
						 : pImages->LoadFile(_T("TOOLBARBUTTONSPINARROWGLYPHS"));

			if (!pImage)
				return;

			int nState = 12
						 + (!bEnabled
								? 3
								: m_nPressedButton == UD_HITUP ? 2
															   : m_nHotButton == UD_HITUP ? 1 : 0);
			CRect rcSrc = pImage->GetSource(nState, 16);
			pImage->DrawImage(pDC, GetCenterRect(rcTop, rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0),
							  0xFF00FF);

			nState = 8
					 + (!bEnabled
							? 3
							: m_nPressedButton == UD_HITUP ? 2 : m_nHotButton == UD_HITUP ? 1 : 0);
			rcSrc = pImage->GetSource(nState, 16);
			pImage->DrawImage(pDC, GetCenterRect(rcBottom, rcSrc.Size()), rcSrc, CRect(0, 0, 0, 0),
							  0xFF00FF);
		}
	}
}

CRect CXTPSpinButtonCtrl::GetButtonRect(int nButton)
{
	CRect rc;
	GetClientRect(&rc);

	if (HasSharedBorder())
	{
		CXTPEdit* pEditBuddy = GetEditBuddy();
		pEditBuddy->AdjustBuddyRect(this, rc,
									GetStyle() & UDS_ALIGNRIGHT ? UDS_ALIGNRIGHT : UDS_ALIGNLEFT);
	}

	BOOL bOverride = (m_nTheme == xtpControlThemeUltraFlat || m_nTheme == xtpControlThemeOfficeXP
					  || m_nTheme == xtpControlThemeOffice2003
					  || m_nTheme == xtpControlThemeOffice2013 || m_nTheme == xtpControlThemeFlat
					  || m_nTheme == xtpControlThemeVisualStudio2012Light
					  || m_nTheme == xtpControlThemeVisualStudio2012Dark
					  || m_nTheme == xtpControlThemeVisualStudio2015
					  || m_nTheme == xtpControlThemeVisualStudio2017
					  || m_nTheme == xtpControlThemeVisualStudio2019
					  || m_nTheme == xtpControlThemeVisualStudio2022);

	if (bOverride && m_bUseVisualStyle && m_themeSpin->IsAppThemeActive())
		bOverride = FALSE;

	if (!IsVertical())
	{
		int nWidth = (rc.Width() + (bOverride ? 1 : 0)) / 2;
		if (nButton == UD_HITDOWN)
			rc.right = rc.left + nWidth;
		else
			rc.left = bOverride ? rc.left + nWidth - 1 : rc.right - nWidth;
	}
	else
	{
		int nHeight = (rc.Height() + (bOverride ? 1 : 0)) / 2;
		if (nButton == UD_HITUP)
			rc.bottom = rc.top + nHeight;
		else
			rc.top = bOverride ? rc.top + nHeight - 1 : rc.bottom - nHeight;
	}

	return rc;
}

void CXTPSpinButtonCtrl::DrawVisualStyle(CDC* pDC)
{
	BOOL bEnabled = IsWindowEnabled();

	if (!IsVertical())
	{
		int nState = DNHZS_NORMAL;
		if (!bEnabled)
			nState = DNHZS_DISABLED;
		else if (m_nPressedButton == UD_HITDOWN && m_nHotButton == UD_HITDOWN)
			nState = DNHZS_PRESSED;
		else if ((m_nHotButton == UD_HITDOWN || m_nPressedButton == UD_HITDOWN)
				 && (m_nPressedButton != UD_HITUP))
			nState = DNHZS_HOT;

		CRect rc = GetButtonRect(UD_HITDOWN);
		m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_DOWNHORZ, nState, &rc, NULL);

		nState = UPHZS_NORMAL;
		if (!bEnabled)
			nState = UPHZS_DISABLED;
		else if (m_nPressedButton == UD_HITUP && m_nHotButton == UD_HITUP)
			nState = UPHZS_PRESSED;
		else if ((m_nHotButton == UD_HITUP || m_nPressedButton == UD_HITUP)
				 && (m_nPressedButton != UD_HITDOWN))
			nState = UPHZS_HOT;

		rc = GetButtonRect(UD_HITUP);
		m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_UPHORZ, nState, &rc, NULL);
	}
	else
	{
		int nState = UPS_NORMAL;
		if (!bEnabled)
			nState = UPS_DISABLED;
		else if (m_nPressedButton == UD_HITUP && m_nHotButton == UD_HITUP)
			nState = UPS_PRESSED;
		else if ((m_nHotButton == UD_HITUP || m_nPressedButton == UD_HITUP)
				 && (m_nPressedButton != UD_HITDOWN))
			nState = UPS_HOT;

		CRect rc = GetButtonRect(UD_HITUP);
		m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_UP, nState, &rc, NULL);

		nState = DNS_NORMAL;
		if (!bEnabled)
			nState = DNS_DISABLED;
		else if (m_nPressedButton == UD_HITDOWN && m_nHotButton == UD_HITDOWN)
			nState = DNS_PRESSED;
		else if ((m_nHotButton == UD_HITDOWN || m_nPressedButton == UD_HITDOWN)
				 && (m_nPressedButton != UD_HITUP))
			nState = DNS_HOT;

		rc = GetButtonRect(UD_HITDOWN);
		m_themeSpin->DrawThemeBackground(pDC->GetSafeHdc(), SPNP_DOWN, nState, &rc, NULL);
	}
}

BOOL CXTPSpinButtonCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPSpinButtonCtrl::OnSize(UINT nType, int cx, int cy)
{
	CSpinButtonCtrl::OnSize(nType, cx, cy);

	Invalidate(FALSE);
}

void CXTPSpinButtonCtrl::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();

	m_themeSpin->CloseThemeData();

	if (m_bUseVisualStyle)
	{
		m_themeSpin->OpenThemeData(NULL, L"SPIN");
	}

	m_crArrowEnabled  = GetXtremeColor(COLOR_WINDOWTEXT);
	m_crArrowHilite   = m_crArrowEnabled;
	m_crArrowPushed   = m_crArrowEnabled;
	m_crArrowDisabled = GetXtremeColor(COLOR_3DSHADOW);

	switch (m_nTheme)
	{
		case xtpControlThemeFlat:
		{
			m_crBack		   = GetXtremeColor(COLOR_BTNFACE);
			m_crBackHilite	 = m_crBack;
			m_crBackPushed	 = m_crBack;
			m_crBackDisabled   = m_crBack;
			m_crBorder		   = GetXtremeColor(COLOR_WINDOWFRAME);
			m_crBorderHilite   = m_crBorder;
			m_crBorderPushed   = m_crBorder;
			m_crBorderDisabled = m_crBorder;
		}
		break;

		case xtpControlThemeUltraFlat:
		{
			m_crBack		   = GetXtremeColor(COLOR_3DFACE);
			m_crBackHilite	 = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE),
														   GetXtremeColor(COLOR_WINDOW), 0x1E);
			m_crBackPushed	 = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DSHADOW),
														   GetXtremeColor(COLOR_3DFACE), 0x32);
			m_crBackDisabled   = m_crBack;
			m_crBorder		   = GetXtremeColor(COLOR_3DSHADOW);
			m_crBorderHilite   = m_crBorder;
			m_crBorderPushed   = m_crBorder;
			m_crBorderDisabled = m_crBorder;
		}
		break;

		case xtpControlThemeOfficeXP:
		case xtpControlThemeOffice2003:
		{
			m_crBackPushed   = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
			m_crBackHilite   = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			m_crBorderHilite = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);

			XTPCurrentSystemTheme systemTheme = m_nTheme == xtpControlThemeOffice2003
													? XTPColorManager()->GetCurrentSystemTheme()
													: xtpSystemThemeUnknown;
			switch (systemTheme)
			{
				case xtpSystemThemeBlue:
				case xtpSystemThemeRoyale:
				case xtpSystemThemeAero:
					m_crBack	   = RGB(169, 199, 240);
					m_crBackHilite = RGB(0, 0, 128);
					m_crBorder	 = RGB(127, 157, 185);
					break;

				case xtpSystemThemeOlive:
					m_crBack	   = RGB(197, 212, 159);
					m_crBackHilite = RGB(63, 93, 56);
					m_crBorder	 = RGB(164, 185, 127);
					break;

				case xtpSystemThemeSilver:
					m_crBack	   = RGB(192, 192, 211);
					m_crBackHilite = RGB(75, 75, 11);
					m_crBorder	 = RGB(165, 172, 178);
					break;
			}

			if (systemTheme != xtpSystemThemeUnknown)
			{
				m_crBackHilite = RGB(255, 238, 194);
				m_crBackPushed = RGB(254, 128, 62);
			}

			m_crBackDisabled   = m_crBack;
			m_crBorderPushed   = m_crBorderHilite;
			m_crBorderDisabled = m_crBorder;
		}
		break;

		case xtpControlThemeVisualStudio2012Light:

			m_crBack		   = RGB(238, 238, 242);
			m_crBackHilite	 = RGB(201, 222, 245);
			m_crBackPushed	 = RGB(0, 122, 204);
			m_crBackDisabled   = RGB(238, 238, 242);
			m_crBorder		   = RGB(204, 206, 219);
			m_crBorderHilite   = RGB(0, 122, 204);
			m_crBorderPushed   = RGB(0, 122, 204);
			m_crBorderDisabled = RGB(204, 206, 219);
			m_crArrowEnabled   = RGB(113, 113, 113);
			m_crArrowHilite	= RGB(30, 30, 30);
			m_crArrowPushed	= RGB(255, 255, 255);
			m_crArrowDisabled  = RGB(204, 206, 219);
			break;

		case xtpControlThemeVisualStudio2012Dark:

			m_crBack		   = RGB(45, 45, 48);
			m_crBackHilite	 = RGB(31, 31, 32);
			m_crBackPushed	 = RGB(0, 122, 204);
			m_crBackDisabled   = RGB(45, 45, 48);
			m_crBorder		   = RGB(67, 67, 70);
			m_crBorderHilite   = RGB(0, 122, 204);
			m_crBorderPushed   = RGB(0, 122, 204);
			m_crBorderDisabled = RGB(67, 67, 70);
			m_crArrowEnabled   = RGB(153, 153, 153);
			m_crArrowHilite	= RGB(76, 140, 212);
			m_crArrowPushed	= RGB(255, 255, 255);
			m_crArrowDisabled  = RGB(67, 67, 70);
			break;

		case xtpControlThemeOffice2013:
		case xtpControlThemeVisualStudio2015:
		case xtpControlThemeVisualStudio2017:
		case xtpControlThemeVisualStudio2019:
		case xtpControlThemeVisualStudio2022:
		case xtpControlThemeNativeWindows10:

			// Defaults are set to OFFICE2013WORD_INI colors.

			m_crBack	   = XTPIniColor(_T("Controls.SpinButton"), _T("Back"), RGB(253, 253, 253));
			m_crBackHilite = XTPIniColor(_T("Controls.SpinButton"), _T("BackHilite"),
										 RGB(215, 225, 242));
			m_crBackPushed = XTPIniColor(_T("Controls.SpinButton"), _T("BackPushed"),
										 RGB(163, 189, 227));
			m_crBackDisabled = XTPIniColor(_T("Controls.SpinButton"), _T("BackDisabled"),
										   RGB(253, 253, 253));
			m_crBorder = XTPIniColor(_T("Controls.SpinButton"), _T("Border"), RGB(171, 171, 171));
			m_crBorderHilite   = XTPIniColor(_T("Controls.SpinButton"), _T("BorderHilite"),
											 RGB(163, 189, 227));
			m_crBorderPushed   = XTPIniColor(_T("Controls.SpinButton"), _T("BorderPushed"),
											 RGB(43, 87, 154));
			m_crBorderDisabled = XTPIniColor(_T("Controls.SpinButton"), _T("BorderDisabled"),
											 RGB(225, 225, 225));
			m_crArrowEnabled   = XTPIniColor(_T("Controls.SpinButton"), _T("Arrow"),
											 RGB(119, 119, 119));
			m_crArrowHilite	= XTPIniColor(_T("Controls.SpinButton"), _T("ArrowHilite"),
											 RGB(68, 68, 68));
			m_crArrowPushed	= XTPIniColor(_T("Controls.SpinButton"), _T("ArrowPushed"),
											 RGB(68, 68, 68));
			m_crArrowDisabled  = XTPIniColor(_T("Controls.SpinButton"), _T("ArrowDisabled"),
											 RGB(198, 198, 198));
			break;

		default:

			m_crBack		   = GetXtremeColor(COLOR_3DFACE);
			m_crBackHilite	 = m_crBack;
			m_crBackPushed	 = m_crBack;
			m_crBackDisabled   = m_crBack;
			m_crBorder		   = GetXtremeColor(COLOR_3DSHADOW);
			m_crBorderHilite   = m_crBorder;
			m_crBorderPushed   = m_crBorder;
			m_crBorderDisabled = m_crBorder;
			break;
	}
}

UINT CXTPSpinButtonCtrl::HitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);

	if (!rc.PtInRect(pt))
		return UD_HITNOWHERE;

	if (GetStyle() & UDS_HORZ)
	{
		// Horizontal placement
		if (pt.x < (rc.right / 2))
		{
			return UD_HITDOWN;
		}
		else if (pt.x > (rc.right / 2))
		{
			return UD_HITUP;
		}
	}
	else
	{
		if (pt.y > (rc.bottom / 2))
		{
			return UD_HITDOWN;
		}
		else if (pt.y < (rc.bottom / 2))
		{
			return UD_HITUP;
		}
	}

	return UD_HITNOWHERE;
}

void CXTPSpinButtonCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	UINT uHot = HitTest(point);

	if (uHot != m_nHotButton)
	{
		m_nHotButton = uHot;
		Invalidate(FALSE);

		if (m_nHotButton != UD_HITNOWHERE)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT };
			_TrackMouseEvent(&tme);
		}
	}

	CSpinButtonCtrl::OnMouseMove(nFlags, point);
}

void CXTPSpinButtonCtrl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPSpinButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CSpinButtonCtrl::OnLButtonDown(nFlags, point);

	UINT uHot = HitTest(point);

	if (uHot != HTNOWHERE)
	{
		m_nPressedButton = uHot;
		Invalidate(FALSE);
	}
}

void CXTPSpinButtonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_nPressedButton != HTNOWHERE)
	{
		m_nPressedButton = HTNOWHERE;
		Invalidate(FALSE);
	}

	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}

void CXTPSpinButtonCtrl::SetUseVisualStyle(BOOL bUseVisualStyle /* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();

	if (m_hWnd)
		Invalidate(FALSE);
}

void CXTPSpinButtonCtrl::SetTheme(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	m_bUseVisualStyle = (m_nTheme == xtpControlThemeDefault
						 || m_nTheme == xtpControlThemeNativeWinXP);

	RefreshMetrics();

	if (m_hWnd)
		Invalidate(FALSE);
}

LRESULT CXTPSpinButtonCtrl::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}
