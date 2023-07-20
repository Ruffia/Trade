// XTPColorPicker.cpp : implementation of the CXTPColorPicker class.
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

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Defines.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPButtonTheme.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Popup/XTPColorPopup.h"
#include "Controls/Button/XTPColorPicker.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_XTP_SHOWPOPUP = (WM_XTP_CONTROLS_BASE + 29);

/////////////////////////////////////////////////////////////////////////////
// DDX Routines

_XTP_EXT_CLASS void AFXAPI DDX_XTPColorPicker(CDataExchange* pDX, int nIDC, COLORREF& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT(hWndCtrl != NULL);

	CXTPColorPicker* pColorPicker = (CXTPColorPicker*)CWnd::FromHandle(hWndCtrl);
	if (pDX->m_bSaveAndValidate)
	{
		value = pColorPicker->GetColor();
	}
	else
	{
		pColorPicker->SetColor(value);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPicker

CXTPColorPicker::CXTPColorPicker()
	: m_bShowText(TRUE)
	, m_bColorText(FALSE)
	, m_bPopupActive(FALSE)
	, m_dwPopup(CPS_XTP_EXTENDED | CPS_XTP_MORECOLORS | CPS_XTP_RECENTCOLORS | CPS_XTP_COLORBORDERS)
	, m_clrSelected(CLR_DEFAULT)
	, m_clrDefault(CLR_DEFAULT)
	, m_bCaptions(TRUE)
{
	m_nCols			  = 8;
	m_pExtendedColors = 0;
	m_nExtendedColors = 0;
}

CXTPColorPicker::~CXTPColorPicker()
{
}

IMPLEMENT_DYNAMIC(CXTPColorPicker, CXTPButton)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPColorPicker, CXTPButton)
	//{{AFX_MSG_MAP(CXTPColorPicker)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_XTP_SELCHANGE, OnXtpSelChange)
	ON_MESSAGE(CPN_XTP_DROPDOWN, OnXtpDropDown)
	ON_MESSAGE(CPN_XTP_CLOSEUP, OnXtpCloseUp)
	ON_MESSAGE(CPN_XTP_SELENDOK, OnXtpSelEndOK)
	ON_MESSAGE(CPN_XTP_SELENDCANCEL, OnXtpSelEndCancel)
	ON_MESSAGE(CPN_XTP_SELNOFILL, OnXtpSelNoFill)
	ON_MESSAGE_VOID(WM_XTP_SHOWPOPUP, OnXtpShowPopup)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPColorPicker::DrawColorPicker(CDC* pDC)
{
	CXTPDCSaveState dcState(pDC);

	// draw the color box.
	CXTPClientRect rcItem(this);
	rcItem.DeflateRect(XTP_DPI_X(4), XTP_DPI_Y(4), XTP_DPI_X(20), XTP_DPI_Y(4));

	BOOL bSelected = (GetChecked() || IsPushed());
	if (bSelected && (m_pTheme->m_bOffsetHiliteText || m_pTheme->IsVisualThemeUsed(this)))
	{
		rcItem.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	}

	BOOL bEnabled = IsWindowEnabled();

	COLORREF clrFill = GetColor();
	COLORREF clrText = bEnabled ? m_pTheme->m_crText.operator COLORREF()
								: ::GetSysColor(COLOR_GRAYTEXT);

	if (clrFill != COLORREF_NULL && m_bColorText == FALSE)
	{
		if (bEnabled)
		{
			// create a pen for the rectangle border color.
			CXTPPenDC dcPen(pDC->m_hDC, CXTPDrawHelpers::AdjustBrightness(clrFill, -25));

			// create a pen for the rectangle background color.
			CXTPBrushDC dcBrush(pDC->m_hDC, clrFill);

			pDC->Rectangle(&rcItem);
		}
		else
		{
			pDC->Draw3dRect(&rcItem, clrText, clrText);
		}
	}

	// Draw arrow
	CXTPClientRect rcArrow(this);
	rcArrow.DeflateRect(GetBorderGap(), GetBorderGap());
	rcArrow.left = rcArrow.right - XTP_DPI_X(12);
	if (bSelected && (m_pTheme->m_bOffsetHiliteText || m_pTheme->IsVisualThemeUsed(this)))
	{
		rcArrow.OffsetRect(XTP_DPI_X(1), XTP_DPI_Y(1));
	}

	XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcArrow, clrText);

	// Draw text
	CString strText = GetButtonText();
	if (!strText.IsEmpty() && m_bShowText)
	{
		pDC->SetTextColor(m_bColorText ? clrFill : bEnabled ? GetContrastColor() : clrText);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strText, rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void CXTPColorPicker::DoDraw(CDC* pDC)
{
	m_pTheme->DrawButtonBackground(pDC, this);

	DrawColorPicker(pDC);

	if (::GetFocus() == m_hWnd)
	{
		m_pTheme->DrawFocusRect(pDC, this);
	}
}

BOOL CXTPColorPicker::IsPushed()
{
	return m_bPopupActive;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPicker message handlers

LRESULT CXTPColorPicker::OnXtpSelChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		// Notify of selection change.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_SELCHANGE),
							(LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnXtpDropDown(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup activation.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_DROPDOWN),
							(LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnXtpCloseUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_bPopupActive = FALSE;

	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_CLOSEUP),
							(LPARAM)m_hWnd);
	}
	ReleaseCapture();
	SetFocus();
	Invalidate();
	return 0;
}

LRESULT CXTPColorPicker::OnXtpSelEndOK(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);

		// Notify of popup close up.
		// NB: doing so may destroy the picker so lets copy off
		// the window handle/control ID for safety
		HWND hWndSender = m_hWnd;
		int nControlID  = GetDlgCtrlID();
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID, CPN_XTP_CLOSEUP),
							(LPARAM)hWndSender);

		// Notify of selection made.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID, CPN_XTP_SELENDOK),
							(LPARAM)hWndSender);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnXtpSelEndCancel(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_CLOSEUP),
							(LPARAM)m_hWnd);

		// Notify of cancel.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_SELENDCANCEL),
							(LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnXtpSelNoFill(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);

		// Notify of no fill press.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_XTP_SELNOFILL),
							(LPARAM)m_hWnd);
	}

	return 0;
}

void CXTPColorPicker::SetColor(COLORREF clr)
{
	if (clr != GetColor())
	{
		m_clrSelected = clr;
		if (m_hWnd)
			RedrawWindow();
	}
}

COLORREF CXTPColorPicker::GetContrastColor() const
{
	COLORREF cr		 = GetColor();
	double Intensity = ((299 * GetRValue(cr)) + (587 * GetGValue(cr)) + (114 * GetBValue(cr)))
					   / 1000.0;
	return Intensity > 128 ? RGB(0x00, 0x00, 0x00) : RGB(0xff, 0xff, 0xff);
}

void CXTPColorPicker::ShowPopupWindow()
{
	m_bPopupActive = TRUE;

	// Make sure that we have input focus.
	if (GetFocus() != this)
	{
		SetFocus();
	}

	// Post a message instead of displaying right away - this will take care
	// of asynchronous focusing issues
	PostMessage(WM_XTP_SHOWPOPUP);
}

BOOL CXTPColorPicker::SetThemeColors(XTP_PICK_BUTTON* pColors /*=NULL*/, int nCount /*=10*/,
									 LPCTSTR lpszTitle /*=NULL*/, UINT* pStringIDs /*=NULL*/,
									 int ppShadeMatrix[6][10] /*=NULL*/)
{
	CXTPColorSelectorCtrl csc;

	if (csc.SetThemeColors(pColors, nCount, lpszTitle, pStringIDs, ppShadeMatrix))
	{
		m_arThemeColors.RemoveAll();
		m_arThemeColors.Copy(csc.GetThemeColors());

		// SetThemeColors is always assumed to be a single row, so
		// the size of the array should equal the column count.
		m_nCols			= nCount;
		m_strTitleTheme = csc.GetThemeTitle();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPColorPicker::SetThemeColors(XTP_PICK_BUTTON* pColors, int nCount, LPCTSTR lpszTitle,
									 LPCTSTR* ppszStrings, int ppShadeMatrix[6][10])
{
	CXTPColorSelectorCtrl csc;

	if (csc.SetThemeColors(pColors, nCount, lpszTitle, ppszStrings, ppShadeMatrix))
	{
		m_arThemeColors.RemoveAll();
		m_arThemeColors.Copy(csc.GetThemeColors());

		// SetThemeColors is always assumed to be a single row, so
		// the size of the array should equal the column count.
		m_nCols			= nCount;
		m_strTitleTheme = csc.GetThemeTitle();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPColorPicker::SetStandardColors(XTP_PICK_BUTTON* pColors, int nCount, int nCols /*=10*/,
										LPCTSTR lpszTitle /*=NULL*/)
{
	CXTPColorSelectorCtrl csc;

	if (csc.SetStandardColors(pColors, nCount, nCols, lpszTitle))
	{
		m_arStandardColors.RemoveAll();
		m_arStandardColors.Copy(csc.GetStandardColors());
		m_nCols			   = nCols;
		m_strTitleStandard = csc.GetStandardTitle();
		return TRUE;
	}

	return FALSE;
}

void CXTPColorPicker::OnXtpShowPopup()
{
	// Get the size of the picker button.
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	// Create the color popup window.
	CXTPColorPopup* pColorPopup = new CXTPColorPopup(TRUE);
	pColorPopup->ShowCaptions(m_bCaptions);

	if (m_arThemeColors.GetSize())
		pColorPopup->SetThemeColors(m_arThemeColors, m_nCols, m_strTitleTheme);

	if (m_arStandardColors.GetSize())
		pColorPopup->SetStandardColors(m_arStandardColors, m_nCols, m_strTitleStandard);

	if (m_pExtendedColors || !m_strTitleCustom.IsEmpty())
		pColorPopup->SetColors(m_pExtendedColors, m_nExtendedColors, m_nCols, m_strTitleCustom);

	switch (m_pTheme->GetThemeStyle())
	{
		case xtpControlThemeOfficeXP:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeOfficeXP());
			break;
		case xtpControlThemeOffice2003:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeOffice2003());
			break;
		case xtpControlThemeOffice2013:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeOffice2013());
			break;
		case xtpControlThemeVisualStudio2015:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeVisualStudio2015());
			break;
		case xtpControlThemeVisualStudio2017:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeVisualStudio2017());
			break;
		case xtpControlThemeVisualStudio2019:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeVisualStudio2019());
			break;
		case xtpControlThemeVisualStudio2022:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeVisualStudio2022());
			break;
		case xtpControlThemeNativeWindows10:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeNativeWindows10());
			break;
		case xtpControlThemeResource:
			pColorPopup->SetTheme(new CXTPColorSelectorCtrlThemeResource());
			break;
		case xtpControlThemeFlat:
		case xtpControlThemeUltraFlat:
		case xtpControlThemeOffice2000:
		case xtpControlThemeNativeWinXP:
		case xtpControlThemeVisualStudio2005:
		case xtpControlThemeVisualStudio2008:
		case xtpControlThemeVisualStudio2010:
		case xtpControlThemeCustom:
		case xtpControlThemeVisualStudio2012:
		case xtpControlThemeVisualStudio2012Light:
		case xtpControlThemeVisualStudio2012Dark:
		case xtpControlThemeDefault:
		default: pColorPopup->SetTheme(new CXTPColorSelectorCtrlTheme()); break;
	}

	if (pColorPopup->Create(rcWindow, this, m_dwPopup, GetColor(), GetDefaultColor()))
	{
		pColorPopup->SetFocus();
	}
}

BOOL CXTPColorPicker::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (!m_bPopupActive)
	{
		if (message == WM_LBUTTONDOWN)
		{
			ShowPopupWindow();
			// in this case the message is not "swallowed" so the button will
			// get it and display itself in a recessed state
		}
		else if (message == WM_KEYDOWN
				 && (wParam == VK_RETURN || wParam == VK_SPACE || wParam == VK_DOWN))
		{
			ShowPopupWindow();
			return TRUE; // swallow message
		}
	}

	return CXTPButton::OnWndMsg(message, wParam, lParam, pResult);
}
