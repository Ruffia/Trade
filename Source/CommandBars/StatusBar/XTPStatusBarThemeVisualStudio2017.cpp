// XTPStatusBarThemeVisualStudio2017.cpp
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/XTPStatusBarPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h"
#include "CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2017.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPStatusBarThemeVisualStudio2017::CXTPStatusBarThemeVisualStudio2017(
	CXTPPaintManager* pPaintManager)
	: CXTPStatusBarThemeOffice2013(pPaintManager)
	, m_bAutomaticStateColor(TRUE)
{
	XTPGetApplication()->Subscribe(this);
}

CXTPStatusBarThemeVisualStudio2017::~CXTPStatusBarThemeVisualStudio2017()
{
	XTPGetApplication()->Unsubscribe(this);
}

void CXTPStatusBarThemeVisualStudio2017::RefreshMetrics()
{
	CXTPStatusBarThemeOffice2013::RefreshMetrics();

	m_clrGripperLite.SetStandardValue(
		XTPColorManager()->LightColor(m_clrBackground, GetXtremeColor(COLOR_WINDOW), 50));
	m_clrGripperDark.SetStandardValue(
		XTPColorManager()->LightColor(m_clrBackground, GetXtremeColor(COLOR_WINDOWTEXT), 75));

	UpdateStateColor();
}

void CXTPStatusBarThemeVisualStudio2017::SetStateColor(COLORREF clrState)
{
	if (clrState == COLORREF_NULL)
	{
		m_clrTextGray.SetDefaultValue();
		m_clrBackground.SetDefaultValue();
		m_clrButtonBack.SetDefaultValue();
		m_clrGripperLite.SetDefaultValue();
		m_clrGripperDark.SetDefaultValue();
		m_clrButtonBackPushed.SetDefaultValue();
		m_clrButtonBackHighlight.SetDefaultValue();
		m_clrButtonBackChecked.SetDefaultValue();

		m_bAutomaticStateColor = TRUE;
	}
	else
	{
		m_clrTextGray	= clrState;
		m_clrBackground  = clrState;
		m_clrButtonBack  = clrState;
		m_clrGripperLite = XTPColorManager()->LightColor(clrState, GetXtremeColor(COLOR_WINDOW),
														 50);
		m_clrGripperDark = XTPColorManager()->LightColor(clrState, GetXtremeColor(COLOR_WINDOWTEXT),
														 75);
		m_clrButtonBackPushed	= m_clrGripperDark;
		m_clrButtonBackHighlight = m_clrGripperLite;
		m_clrButtonBackChecked   = m_clrGripperDark;

		m_bAutomaticStateColor = FALSE;
	}
}

COLORREF CXTPStatusBarThemeVisualStudio2017::GetStateColor() const
{
	return (m_clrTextGray == m_clrBackground && m_clrTextGray == m_clrButtonBack
					&& m_clrTextGray == m_clrGripperLite && m_clrTextGray == m_clrGripperDark
					&& m_clrTextGray == m_clrButtonBackPushed
					&& m_clrTextGray == m_clrButtonBackHighlight
					&& m_clrTextGray == m_clrButtonBackChecked
				? m_clrTextGray.operator COLORREF()
				: COLORREF_NULL);
}

void CXTPStatusBarThemeVisualStudio2017::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	CPoint pt(rcClient.BottomRight());
	pt.x -= XTP_DPI_X(6);
	pt.y -= XTP_DPI_Y(6);

	COLORREF clr = m_clrGripperDark;
	int diffX = XTP_DPI_X(1), diffY = XTP_DPI_Y(1);
	CRect rcDot; // CRect(a, b, a + diffX, b + diffY);

	for (int i = 1; i >= 0; --i)
	{
		for (int a = 0, c = 4; a < 4; ++a, --c)
		{
			rcDot.left  = pt.x - a * XTP_DPI_X(2);
			rcDot.right = rcDot.left + diffX;
			for (int b = 0; b < c; ++b)
			{
				rcDot.top	= pt.y - b * XTP_DPI_Y(2);
				rcDot.bottom = rcDot.top + diffY;
				pDC->FillSolidRect(rcDot, clr);
			}
		}
		clr = m_clrGripperLite;
		pt.x += diffX;
		pt.y += diffY;
	}
}

void CXTPStatusBarThemeVisualStudio2017::OnAmbientPropertyChanged(
	CXTPApplication* pApplication, XTPApplicationAmbientProperty nProperty)
{
	UNREFERENCED_PARAMETER(pApplication);

	if (xtpApplicationStateColor != nProperty)
		return;

	UpdateStateColor();
}

void CXTPStatusBarThemeVisualStudio2017::UpdateStateColor()
{
	if (!m_bAutomaticStateColor)
		return;

	COleVariant color = XTPGetApplication()->GetAmbientProperty(xtpApplicationStateColor);
	color.ChangeType(VT_I4);
	SetStateColor(static_cast<COLORREF>(color.lVal));

	m_bAutomaticStateColor = TRUE;
}
