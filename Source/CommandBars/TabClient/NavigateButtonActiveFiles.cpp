// NavigateButtonActiveFiles.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "TabManager/Includes.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlExt.h"

#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/TabClient/NavigateButtonActiveFiles.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

CXTPTabClientWnd::CNavigateButtonActiveFiles::CNavigateButtonActiveFiles(
	CXTPTabManager* pManager, CXTPTabClientWnd* pTabClientWnd)
	: CXTPTabManagerNavigateButton(pManager, xtpTabNavigateButtonActiveFiles,
								   xtpTabNavigateButtonNone)
{
	m_bHiddenTabs   = FALSE;
	m_pTabClientWnd = pTabClientWnd;
	XTPResourceManager()->LoadString(&m_strToolTip, XTP_IDS_TABNAVIGATEBUTTON_ACTIVEFILES);
}

void CXTPTabClientWnd::CNavigateButtonActiveFiles::DrawEntry(CDC* pDC, CRect rc, COLORREF clr,
															 BOOL bEnabled)
{
	XTPPrimitiveSymbol nSymbol = (m_bHiddenTabs ? xtpPrimitiveSymbolEjectDown
												: xtpPrimitiveSymbolPlayDown);
	XTPPrimitiveDrawer()->DrawSymbol(pDC, nSymbol, rc, clr, COLORREF_NULL, !bEnabled);
}

void CXTPTabClientWnd::CNavigateButtonActiveFiles::Reposition(CRect& rcNavigateButtons)
{
	CRect rc = m_pManager->GetAppearanceSet()->GetHeaderMargin();

	m_bHiddenTabs = m_pManager->GetItemsLength() > m_pManager->GetRectLength(rcNavigateButtons)
													   - rc.left - rc.right - GetSize().cx;

	CXTPTabManagerNavigateButton::Reposition(rcNavigateButtons);
}
void CXTPTabClientWnd::CNavigateButtonActiveFiles::PerformClick(HWND hWnd, CPoint /*pt*/)
{
	CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
	if (!pCommandBars || pCommandBars->IsCustomizeMode())
		return;

	m_bPressed = TRUE;
	m_pManager->RedrawControl(NULL, FALSE);

	CPoint pt(m_rcButton.left, m_rcButton.bottom);
	m_pTabClientWnd->WorkspaceToScreen(&pt);
	CRect rcExclude(pt.x, pt.y - m_rcButton.Height(), pt.x + m_rcButton.Width(), pt.y);

	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(pCommandBars);

	CXTPImageManager* pImageManager = new CXTPImageManager();
	pPopupBar->SetImageManager(pImageManager);

	UINT nIconId	= 0;
	HICON hIconLast = 0;

	for (int nIndex = 0; nIndex < m_pManager->GetItemCount(); nIndex++)
	{
		CXTPTabManagerItem* pItem = m_pManager->GetItem(nIndex);
		if (!pItem->GetHandle())
			continue;

		CString strCaption	= pItem->GetCaption();
		CXTPControl* pControl = pPopupBar->GetControls()->Add(xtpControlButton, nIndex + 1, _T(""),
															  nIndex, TRUE);

		HICON hIcon = m_pTabClientWnd->GetItemIcon(pItem);
		if (hIconLast != hIcon)
		{
			nIconId++;
			pImageManager->SetIcon(CXTPImageManagerIconHandle(hIcon, FALSE), nIconId);
			hIconLast = hIcon;
		}

		pControl->SetIconId(XTPToInt(nIconId));
		pControl->SetCaption(CXTPControlWindowList::ConstructCaption(strCaption, 0));
		pControl->SetDescription(_T(""));
		pControl->SetTooltip(m_pManager->GetItemTooltip(pItem));
	}

	int nItem = pCommandBars->TrackPopupMenu(pPopupBar,
											 TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, pt.x,
											 pt.y, m_pTabClientWnd, &rcExclude);
	pPopupBar->InternalRelease();

	m_bPressed = FALSE;
	m_pManager->PerformMouseMove(hWnd, CPoint(-1, -1));
	m_pManager->RedrawControl(NULL, FALSE);

	if (nItem > 0)
	{
		CXTPTabManagerItem* pItem = m_pManager->GetItem(nItem - 1);

		if (pItem)
		{
			m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
		}
	}
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
