// XTPComboBoxThemeDefault.cpp: implementation of the CXTPComboBoxThemeDefault class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Combo/XTPComboBoxTheme.h"
#include "Controls/Combo/Themes/XTPComboBoxThemeDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeDefault::CXTPComboBoxThemeDefault()
{
}

CXTPComboBoxThemeDefault::~CXTPComboBoxThemeDefault()
{
}

void CXTPComboBoxThemeDefault::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);
}

BOOL CXTPComboBoxThemeDefault::DrawComboBox(CDC* pDC, CXTPComboBox* pComboBox)
{
	if (!m_themeComboBox->IsAppThemeReady())
	{
		CRect rc	  = pComboBox->GetComboBoxRect();
		HBRUSH hBrush = pComboBox->GetClientBrush(pDC);

		pDC->SelectClipRgn(NULL);

		BOOL bPressed = (GetKeyState(VK_LBUTTON) < 0)
						&& pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
		BOOL bSelected = pComboBox->IsHighlighted() || pComboBox->IsFocused()
						 || (::GetCapture() == pComboBox->GetSafeHwnd());
		BOOL bEnabled	= pComboBox->IsWindowEnabled();
		BOOL bRightAlign = (0 != (pComboBox->GetExStyle() & WS_EX_RIGHT));

		int nState = !bEnabled ? CBXS_DISABLED
							   : bPressed ? CBXS_PRESSED : bSelected ? CBXS_HOT : CBXS_NORMAL;

		int nThumbWidth = GetSystemMetrics(SM_CXVSCROLL);

		if (bRightAlign)
			FillRect(pDC->GetSafeHdc(),
					 CRect(rc.left, rc.top, rc.left + XTP_DPI_X(3) + nThumbWidth, rc.bottom),
					 hBrush);
		else
			FillRect(pDC->GetSafeHdc(),
					 CRect(rc.right - nThumbWidth - XTP_DPI_X(3), rc.top, rc.right, rc.bottom),
					 hBrush);

		CRect rectClient(rc.left + XTP_DPI_X(2), rc.top + XTP_DPI_Y(2),
						 rc.right - nThumbWidth - XTP_DPI_X(2), rc.bottom - XTP_DPI_Y(2));
		if (bRightAlign)
			rectClient.OffsetRect(nThumbWidth, 0);

		pDC->ExcludeClipRect(rectClient);

		m_themeComboBox->DrawThemeBackground(pDC->GetSafeHdc(), 0, nState, rc, NULL);

		CRect rcDropDownButton(rc);
		rcDropDownButton.DeflateRect(1, 1);
		if (bRightAlign)
			rcDropDownButton.right = rcDropDownButton.left + nThumbWidth;
		else
			rcDropDownButton.left = rcDropDownButton.right - nThumbWidth;

		m_themeComboBox->DrawThemeBackground(pDC->GetSafeHdc(), CP_DROPDOWNBUTTON, nState,
											 rcDropDownButton, NULL);

		return TRUE;
	}

	return FALSE; // call DefWindowProc
}
