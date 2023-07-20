// XTPTreeTheme.cpp: implementation of the CXTPTreeTheme class.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Tree/XTPTreeTheme.h"

#include "Controls/Tree/XTPTreeBase.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPTreeTheme::CXTPTreeTheme()
{
	m_crText				 = COLORREF_NULL;
	m_crTextDisabled		 = COLORREF_NULL;
	m_crTextSelected		 = COLORREF_NULL;
	m_crTextSelectedInactive = COLORREF_NULL;
	m_crTextSelectedDisabled = COLORREF_NULL;

	m_crBack				 = COLORREF_NULL;
	m_crBackDisabled		 = COLORREF_NULL;
	m_crBackSelected		 = COLORREF_NULL;
	m_crBackSelectedInactive = COLORREF_NULL;
	m_crBackSelectedDisabled = COLORREF_NULL;

	m_nTheme		 = xtpControlThemeDefault;
	m_bDarkTheme	 = FALSE;
	m_bDrawFocusRect = TRUE;
}

CXTPTreeTheme::~CXTPTreeTheme()
{
}

void CXTPTreeTheme::RefreshMetrics(CXTPTreeBase* pTreeBase)
{
	ASSERT(pTreeBase);
	if (!pTreeBase)
		return;

	m_crText				 = GetXtremeColor(COLOR_WINDOWTEXT);
	m_crTextDisabled		 = GetXtremeColor(COLOR_GRAYTEXT);
	m_crTextSelected		 = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crTextSelectedInactive = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crTextSelectedDisabled = GetXtremeColor(COLOR_GRAYTEXT);

	m_crBack				 = GetXtremeColor(COLOR_WINDOW);
	m_crBackDisabled		 = GetXtremeColor(COLOR_WINDOW);
	m_crBackSelected		 = GetXtremeColor(COLOR_HIGHLIGHT);
	m_crBackSelectedInactive = GetXtremeColor(COLOR_HIGHLIGHT);
	m_crBackSelectedDisabled = GetXtremeColor(COLOR_3DFACE);

	CTreeCtrl* pTreeCtrl = pTreeBase->GetSelfTreeCtrl();
	if (pTreeCtrl)
	{
		// if the tree is disabled return disabled back color.
		if (!pTreeCtrl->IsWindowEnabled())
		{
			m_crBack = GetXtremeColor(COLOR_3DFACE);
		}
		else
		{
#if _MSC_VER >= 1200 // MFC 6.0

			if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
			{
				// is there a user defined color available ?
				COLORREF crBack = pTreeCtrl->GetBkColor();
				if (crBack != COLORREF_NULL)
					m_crBack = crBack;
			}
#endif
		}

		// if the tree is disabled return disabled text color.
		if (!pTreeCtrl->IsWindowEnabled())
			m_crText = GetXtremeColor(COLOR_GRAYTEXT);
		else
		{
#if _MSC_VER >= 1200 // MFC 6.0
			if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
			{
				// is ther a user defined color available ?
				COLORREF crText = pTreeCtrl->GetTextColor();
				if (crText != COLORREF_NULL)
					m_crText = crText;
			}
#endif
		}
	}
}

COLORREF CXTPTreeTheme::GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle,
										 COLORREF crBackDef, CTreeCtrl* pTreeCtrl) const
{
	ASSERT(pTreeCtrl);

	// if the tree control is not valid return default color.
	if (!::IsWindow(pTreeCtrl->GetSafeHwnd()))
		return crBackDef;

	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED || uState & TVIS_FOCUSED)
	{
		// if the tree has focus and draw the text background hilite color.
		if (bTreeHasFocus || uState & TVIS_DROPHILITED)
		{
			if (pTreeCtrl->IsWindowEnabled())
			{
				return m_crBackSelected;
			}
			else
			{
				return m_crBackDisabled;
			}
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text background hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (pTreeCtrl->IsWindowEnabled())
			{
				return m_crBackSelectedInactive;
			}
			else
			{
				return m_crBackSelectedDisabled;
			}
		}
	}

	return crBackDef;
}

COLORREF CXTPTreeTheme::GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle,
										 COLORREF crTextDef, CTreeCtrl* pTreeCtrl) const
{
	ASSERT(pTreeCtrl);

	// if the tree control is not valid return default color.
	if (!::IsWindow(pTreeCtrl->GetSafeHwnd()))
		return crTextDef;

	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED)
	{
		// if the tree has focus and draw the text background hilite color.
		if (bTreeHasFocus || uState & TVIS_DROPHILITED)
		{
			if (pTreeCtrl->IsWindowEnabled())
			{
				return m_crTextSelected;
			}
			else
			{
				return m_crTextDisabled;
			}
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text background hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (pTreeCtrl->IsWindowEnabled())
			{
				return m_crTextSelectedInactive;
			}
			else
			{
				return m_crTextSelectedDisabled;
			}
		}
	}

	return crTextDef;
}

COLORREF CXTPTreeTheme::GetTreeBackColor() const
{
	return m_crBack;
}

COLORREF CXTPTreeTheme::GetTreeTextColor() const
{
	return m_crText;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeVisualStudio2012Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeVisualStudio2012Theme::CXTPTreeVisualStudio2012Theme(BOOL bLight /*=TRUE*/)
{
	m_bDarkTheme = !bLight;
	m_nTheme	 = m_bDarkTheme ? xtpControlThemeVisualStudio2012Dark
							: xtpControlThemeVisualStudio2012Light;
	m_bDrawFocusRect = FALSE;
}

void CXTPTreeVisualStudio2012Theme::RefreshMetrics(CXTPTreeBase* pTreeBase)
{
	UNREFERENCED_PARAMETER(pTreeBase);

	m_crText				 = m_bDarkTheme ? RGB(241, 241, 241) : RGB(30, 30, 30);
	m_crTextDisabled		 = GetXtremeColor(COLOR_GRAYTEXT);
	m_crTextSelected		 = RGB(255, 255, 255);
	m_crTextSelectedInactive = m_crText;
	m_crTextSelectedDisabled = m_crTextDisabled;

	m_crBack				 = m_bDarkTheme ? RGB(45, 45, 48) : RGB(246, 246, 246);
	m_crBackDisabled		 = m_crBack;
	m_crBackSelected		 = RGB(51, 153, 255);
	m_crBackSelectedInactive = m_bDarkTheme ? RGB(63, 63, 70) : RGB(204, 206, 219);
	m_crBackSelectedDisabled = m_crBackDisabled;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeOffice2013Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeOffice2013Theme::CXTPTreeOffice2013Theme()
{
	m_bDarkTheme	 = FALSE;
	m_nTheme		 = xtpControlThemeOffice2013;
	m_bDrawFocusRect = FALSE;
}

void CXTPTreeOffice2013Theme::RefreshMetrics(CXTPTreeBase* pTreeBase)
{
	CXTPTreeTheme::RefreshMetrics(pTreeBase);

	m_bDarkTheme	 = XTPIniInt(_T("Controls.Tree"), _T("DarkTheme"), m_bDarkTheme);
	m_crText		 = XTPIniColor(_T("Controls.Tree"), _T("Text"), m_crText);
	m_crTextDisabled = XTPIniColor(_T("Controls.Tree"), _T("TextDisabled"), m_crTextDisabled);
	m_crTextSelected = XTPIniColor(_T("Controls.Tree"), _T("TextSelected"), m_crTextSelected);
	m_crTextSelectedInactive = XTPIniColor(_T("Controls.Tree"), _T("TextSelectedInactive"),
										   m_crTextSelectedInactive);
	m_crTextSelectedDisabled = XTPIniColor(_T("Controls.Tree"), _T("TextSelectedDisabled"),
										   m_crTextSelectedDisabled);
	m_crBack				 = XTPIniColor(_T("Controls.Tree"), _T("Back"), m_crBack);
	m_crBackDisabled = XTPIniColor(_T("Controls.Tree"), _T("BackDisabled"), m_crBackDisabled);
	m_crBackSelected = XTPIniColor(_T("Controls.Tree"), _T("BackSelected"), m_crBackSelected);
	m_crBackSelectedInactive = XTPIniColor(_T("Controls.Tree"), _T("BackSelectedInactive"),
										   m_crBackSelectedInactive);
	m_crBackSelectedDisabled = XTPIniColor(_T("Controls.Tree"), _T("BackSelectedDisabled"),
										   m_crBackSelectedDisabled);
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeVisualStudio2015Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeVisualStudio2015Theme::CXTPTreeVisualStudio2015Theme()
{
	m_nTheme = xtpControlThemeVisualStudio2015;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeVisualStudio2017Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeVisualStudio2017Theme::CXTPTreeVisualStudio2017Theme()
{
	m_nTheme = xtpControlThemeVisualStudio2017;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeVisualStudio2019Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeVisualStudio2019Theme::CXTPTreeVisualStudio2019Theme()
{
	m_nTheme = xtpControlThemeVisualStudio2019;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeVisualStudio2022Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeVisualStudio2022Theme::CXTPTreeVisualStudio2022Theme()
{
	m_nTheme = xtpControlThemeVisualStudio2022;
}

//////////////////////////////////////////////////////////////////////
// CXTPTreeNativeWindows10Theme
//////////////////////////////////////////////////////////////////////

CXTPTreeNativeWindows10Theme::CXTPTreeNativeWindows10Theme()
{
	m_nTheme = xtpControlThemeNativeWindows10;
}
