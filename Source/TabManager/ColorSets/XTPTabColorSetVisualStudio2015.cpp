// XTPTabColorSetVisualStudio2015.cpp
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/XTPTabManagerNavigateButton.h"
#include "TabManager/XTPTabManagerNavigateButtons.h"
#include "TabManager/XTPTabManagerItem.h"
#include "TabManager/XTPTabManagerAtom.h"
#include "TabManager/XTPTabManager.h"
#include "TabManager/ColorSets/XTPTabColorSetVisualStudio2015.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPTabColorSetVisualStudio2015::CXTPTabColorSetVisualStudio2015()
	: CXTPTabPaintManagerColorSet()
{
}

void CXTPTabColorSetVisualStudio2015::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet::RefreshMetrics();

	m_clrButtonNormal	  = XTPIniColor(_T("TabManager"), _T("Button"), RGB(238, 238, 242));
	m_clrButtonPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonPressed"), RGB(14, 97, 152));
	m_clrButtonHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonHighlighted"),
										 RGB(28, 151, 234));
	m_clrButtonHighlightedClose = XTPIniColor(_T("TabManager"), _T("ButtonHighlightedClose"),
											  RGB(82, 176, 239));
	m_clrButtonSelected = XTPIniColor(_T("TabManager"), _T("ButtonSelected"), RGB(0, 122, 204));
	m_clrButtonSelectedPressed	 = XTPIniColor(_T("TabManager"), _T("ButtonSelectedPressed"),
											 RGB(14, 97, 152));
	m_clrButtonSelectedHighlighted = XTPIniColor(_T("TabManager"), _T("ButtonSelectedHighlighted"),
												 RGB(28, 151, 234));
	m_clrButtonBorderHot = XTPIniColor(_T("TabManager"), _T("ButtonBorderHot"), COLORREF_NULL);
	m_clrTextNormal		 = XTPIniColor(_T("TabManager"), _T("Text"), RGB(30, 30, 30));
	m_clrTextPressed	 = XTPIniColor(_T("TabManager"), _T("TextPressed"), RGB(255, 255, 255));
	m_clrTextHighlight   = XTPIniColor(_T("TabManager"), _T("TextHighlighted"), RGB(255, 255, 255));
	m_clrTextHighlightClose = XTPIniColor(_T("TabManager"), _T("TextHighlightedClose"),
										  RGB(255, 255, 255));
	m_clrTextSelected		= XTPIniColor(_T("TabManager"), _T("TextSelected"), RGB(255, 255, 255));
	m_clrTextSelectedPressed		 = XTPIniColor(_T("TabManager"), _T("TextSelectedPressed"),
										   RGB(255, 255, 255));
	m_clrTextSelectedHighlighted	 = XTPIniColor(_T("TabManager"), _T("TextSelectedHighlighted"),
											   RGB(0, 0, 0));
	m_clrActiveFilesGlyph			 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyph"),
										   RGB(113, 113, 113));
	m_clrActiveFilesGlyphPressed	 = XTPIniColor(_T("TabManager"), _T("ActiveFilesGlyphPressed"),
											   RGB(255, 255, 255));
	m_clrActiveFilesGlyphHighlighted = XTPIniColor(_T("TabManager"),
												   _T("ActiveFilesGlyphHighlighted"),
												   RGB(0, 122, 204));
	m_clrActiveFilesBack = XTPIniColor(_T("TabManager"), _T("ActiveFilesBack"), RGB(238, 238, 242));
	m_clrActiveFilesBackPressed		= XTPIniColor(_T("TabManager"), _T("ActiveFilesBackPressed"),
											  RGB(0, 122, 204));
	m_clrActiveFilesBackHighlighted = XTPIniColor(_T("TabManager"),
												  _T("ActiveFilesBackHighlighted"),
												  RGB(255, 255, 255));
	m_clrFrameBorder  = XTPIniColor(_T("TabManager"), _T("FrameBorder"), RGB(238, 238, 242));
	m_clrButtonBorder = XTPIniColor(_T("TabManager"), _T("FrameBorder"), RGB(238, 238, 242));
	m_clrAutoHideFace = XTPIniColor(_T("TabManager"), _T("AutoHideFace"), RGB(238, 238, 242));
	m_clrHeaderFace   = XTPIniColor(_T("TabManager"), _T("HeaderFace"), RGB(238, 238, 242));
	m_csStateButton.clrFace.clrLight   = m_clrButtonSelected;
	m_csStateButton.clrFace.clrDark	= m_clrButtonSelected;
	m_csStateButton.clrBorder.clrDark  = m_clrFrameBorder;
	m_csStateButton.clrBorder.clrLight = m_clrFrameBorder;

	m_clrTabClientBack   = XTPIniColor(_T("TabClient"), _T("Back"), RGB(53, 73, 106));
	m_clrTabClientBorder = XTPIniColor(_T("TabClient"), _T("Border"), RGB(53, 73, 106));
	m_clrPaneFace		 = XTPIniColor(_T("TabManager"), _T("PaneFace"), RGB(214, 219, 233));

	m_clrActiveClientBack = XTPIniColor(_T("TabManager.Active"), _T("ClientBack"), RGB(255, 0, 0));
	m_clrInactiveClientBack = XTPIniColor(_T("TabManager.Inactive"), _T("ClientBack"),
										  RGB(255, 255, 0));

	m_NavigationButton.Normal.clrBack   = XTPIniColor(_T("TabManager.NavigationButton.Normal"),
													  _T("Back"), RGB(41, 57, 86));
	m_NavigationButton.Normal.clrBorder = XTPIniColor(_T("TabManager.NavigationButton.Normal"),
													  _T("Border"), RGB(41, 57, 86));
	m_NavigationButton.Normal.clrText   = XTPIniColor(_T("TabManager.NavigationButton.Normal"),
													  _T("Text"), RGB(206, 212, 221));
	m_NavigationButton.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.NavigationButton.Highlighted"), _T("Back"), RGB(255, 252, 244));
	m_NavigationButton.Highlighted.clrBorder = XTPIniColor(
		_T("TabManager.NavigationButton.Highlighted"), _T("Border"), RGB(229, 195, 101));
	m_NavigationButton.Highlighted.clrText =
		XTPIniColor(_T("TabManager.NavigationButton.Highlighted"), _T("Text"), RGB(0, 0, 0));
	m_NavigationButton.Pressed.clrBack   = XTPIniColor(_T("TabManager.NavigationButton.Pressed"),
													   _T("Back"), RGB(255, 232, 166));
	m_NavigationButton.Pressed.clrBorder = XTPIniColor(_T("TabManager.NavigationButton.Pressed"),
													   _T("Border"), RGB(229, 195, 101));
	m_NavigationButton.Pressed.clrText   = XTPIniColor(_T("TabManager.NavigationButton.Pressed"),
													   _T("Text"), RGB(0, 0, 0));

	m_ActiveTabButton.Normal.clrBack   = XTPIniColor(_T("TabManager.Active.TabButton.Normal"),
													 _T("Back"), RGB(54, 78, 111));
	m_ActiveTabButton.Normal.clrBorder = XTPIniColor(_T("TabManager.Active.TabButton.Normal"),
													 _T("Border"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.rcBorder = XTP_DPI(XTPIniRect(_T("TabManager.Active.TabButton.Normal"),
														   _T("BorderMargin"), CRect(0, 0, 0, 0)));
	m_ActiveTabButton.Normal.clrText  = XTPIniColor(_T("TabManager.Active.TabButton.Normal"),
													_T("Text"), RGB(255, 255, 255));
	m_ActiveTabButton.Normal.Navigate.Normal.clrBack = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonNormalBack"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Normal.clrBorder = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonNormalBorder"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Normal.clrText = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonNormalText"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonHighlightedBack"),
					RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonHighlightedBorder"),
					RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonHighlightedText"),
					RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Pressed.clrBack = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonPressedBack"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Pressed.clrBorder = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonPressedBorder"), RGB(0, 0, 0));
	m_ActiveTabButton.Normal.Navigate.Pressed.clrText = XTPIniColor(
		_T("TabManager.Active.TabButton.Normal"), _T("NavigateButtonPressedText"), RGB(0, 0, 0));
	m_ActiveTabButton.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("Back"), RGB(91, 113, 153));
	m_ActiveTabButton.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("Border"), RGB(0, 0, 0));
	m_ActiveTabButton.Highlighted.rcBorder = XTP_DPI(
		XTPIniRect(_T("TabManager.Active.TabButton.Highlighted"), _T("BorderMargin"),
				   CRect(0, 0, 0, 0)));
	m_ActiveTabButton.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("Text"), RGB(255, 255, 255));
	m_ActiveTabButton.Highlighted.Navigate.Normal.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("NavigateButtonNormalBack"),
					RGB(91, 113, 153));
	m_ActiveTabButton.Highlighted.Navigate.Normal.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("NavigateButtonNormalBorder"),
					RGB(91, 113, 153));
	m_ActiveTabButton.Highlighted.Navigate.Normal.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("NavigateButtonNormalText"),
					RGB(206, 212, 221));
	m_ActiveTabButton.Highlighted.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedBack"), RGB(255, 252, 244));
	m_ActiveTabButton.Highlighted.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedBorder"), RGB(229, 195, 101));
	m_ActiveTabButton.Highlighted.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedText"), RGB(0, 0, 0));
	m_ActiveTabButton.Highlighted.Navigate.Pressed.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("NavigateButtonPressedBack"),
					RGB(253, 244, 191));
	m_ActiveTabButton.Highlighted.Navigate.Pressed.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"),
					_T("NavigateButtonPressedBorder"), RGB(229, 195, 101));
	m_ActiveTabButton.Highlighted.Navigate.Pressed.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Highlighted"), _T("NavigateButtonPressedText"),
					RGB(0, 0, 0));
	m_ActiveTabButton.Selected.clrBack   = XTPIniColor(_T("TabManager.Active.TabButton.Selected"),
													   _T("Back"), RGB(255, 242, 157));
	m_ActiveTabButton.Selected.clrBorder = XTPIniColor(_T("TabManager.Active.TabButton.Selected"),
													   _T("Border"), RGB(0, 0, 0));
	m_ActiveTabButton.Selected.rcBorder  = XTP_DPI(
		 XTPIniRect(_T("TabManager.Active.TabButton.Selected"), _T("BorderMargin"),
					CRect(0, 0, 0, 0)));
	m_ActiveTabButton.Selected.clrText = XTPIniColor(_T("TabManager.Active.TabButton.Selected"),
													 _T("Text"), RGB(0, 0, 0));
	m_ActiveTabButton.Selected.Navigate.Normal.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonNormalBack"),
					RGB(255, 242, 157));
	m_ActiveTabButton.Selected.Navigate.Normal.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonNormalBorder"),
					RGB(255, 242, 157));
	m_ActiveTabButton.Selected.Navigate.Normal.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonNormalText"),
					RGB(117, 99, 61));
	m_ActiveTabButton.Selected.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonHighlightedBack"),
					RGB(255, 252, 244));
	m_ActiveTabButton.Selected.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"),
					_T("NavigateButtonHighlightedBorder"), RGB(229, 195, 101));
	m_ActiveTabButton.Selected.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonHighlightedText"),
					RGB(0, 0, 0));
	m_ActiveTabButton.Selected.Navigate.Pressed.clrBack =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonPressedBack"),
					RGB(253, 244, 191));
	m_ActiveTabButton.Selected.Navigate.Pressed.clrBorder =
		XTPIniColor(_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonPressedBorder"),
					RGB(229, 195, 101));
	m_ActiveTabButton.Selected.Navigate.Pressed.clrText = XTPIniColor(
		_T("TabManager.Active.TabButton.Selected"), _T("NavigateButtonPressedText"), RGB(0, 0, 0));

	m_InactiveTabButton.Normal.clrBack   = XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"),
													   _T("Back"), RGB(54, 78, 111));
	m_InactiveTabButton.Normal.clrBorder = XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"),
													   _T("Border"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.rcBorder  = XTP_DPI(
		 XTPIniRect(_T("TabManager.Inactive.TabButton.Normal"), _T("BorderMargin"),
					CRect(0, 0, 0, 0)));
	m_InactiveTabButton.Normal.clrText = XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"),
													 _T("Text"), RGB(255, 255, 255));
	m_InactiveTabButton.Normal.Navigate.Normal.clrBack = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonNormalBack"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Normal.clrBorder = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonNormalBorder"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Normal.clrText = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonNormalText"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonHighlightedBack"),
					RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"),
					_T("NavigateButtonHighlightedBorder"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonHighlightedText"),
					RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Pressed.clrBack = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonPressedBack"), RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Pressed.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonPressedBorder"),
					RGB(0, 0, 0));
	m_InactiveTabButton.Normal.Navigate.Pressed.clrText = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Normal"), _T("NavigateButtonPressedText"), RGB(0, 0, 0));
	m_InactiveTabButton.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"), _T("Back"), RGB(91, 113, 153));
	m_InactiveTabButton.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"), _T("Border"), RGB(0, 0, 0));
	m_InactiveTabButton.Highlighted.rcBorder = XTP_DPI(
		XTPIniRect(_T("TabManager.Inactive.TabButton.Highlighted"), _T("BorderMargin"),
				   CRect(0, 0, 0, 0)));
	m_InactiveTabButton.Highlighted.clrText = XTPIniColor(
		_T("TabManager.Inactive.TabButton.Highlighted"), _T("Text"), RGB(255, 255, 255));
	m_InactiveTabButton.Highlighted.Navigate.Normal.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"), _T("NavigateButtonNormalBack"),
					RGB(91, 113, 153));
	m_InactiveTabButton.Highlighted.Navigate.Normal.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonNormalBorder"), RGB(91, 113, 153));
	m_InactiveTabButton.Highlighted.Navigate.Normal.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"), _T("NavigateButtonNormalText"),
					RGB(206, 212, 221));
	m_InactiveTabButton.Highlighted.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedBack"), RGB(255, 252, 244));
	m_InactiveTabButton.Highlighted.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedBorder"), RGB(229, 195, 101));
	m_InactiveTabButton.Highlighted.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonHighlightedText"), RGB(0, 0, 0));
	m_InactiveTabButton.Highlighted.Navigate.Pressed.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonPressedBack"), RGB(253, 244, 191));
	m_InactiveTabButton.Highlighted.Navigate.Pressed.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonPressedBorder"), RGB(229, 195, 101));
	m_InactiveTabButton.Highlighted.Navigate.Pressed.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Highlighted"),
					_T("NavigateButtonPressedText"), RGB(0, 0, 0));
	m_InactiveTabButton.Selected.clrBack = XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"),
													   _T("Back"), RGB(77, 96, 130));
	m_InactiveTabButton.Selected.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("Border"), RGB(0, 0, 0));
	m_InactiveTabButton.Selected.rcBorder = XTP_DPI(
		XTPIniRect(_T("TabManager.Inactive.TabButton.Selected"), _T("BorderMargin"),
				   CRect(0, 0, 0, 0)));
	m_InactiveTabButton.Selected.clrText = XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"),
													   _T("Text"), RGB(255, 255, 255));
	m_InactiveTabButton.Selected.Navigate.Normal.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonNormalBack"),
					RGB(77, 96, 130));
	m_InactiveTabButton.Selected.Navigate.Normal.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonNormalBorder"),
					RGB(77, 96, 130));
	m_InactiveTabButton.Selected.Navigate.Normal.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonNormalText"),
					RGB(206, 212, 221));
	m_InactiveTabButton.Selected.Navigate.Highlighted.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"),
					_T("NavigateButtonHighlightedBack"), RGB(255, 252, 244));
	m_InactiveTabButton.Selected.Navigate.Highlighted.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"),
					_T("NavigateButtonHighlightedBorder"), RGB(229, 195, 101));
	m_InactiveTabButton.Selected.Navigate.Highlighted.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"),
					_T("NavigateButtonHighlightedText"), RGB(0, 0, 0));
	m_InactiveTabButton.Selected.Navigate.Pressed.clrBack =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonPressedBack"),
					RGB(253, 244, 191));
	m_InactiveTabButton.Selected.Navigate.Pressed.clrBorder =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonPressedBorder"),
					RGB(229, 195, 101));
	m_InactiveTabButton.Selected.Navigate.Pressed.clrText =
		XTPIniColor(_T("TabManager.Inactive.TabButton.Selected"), _T("NavigateButtonPressedText"),
					RGB(0, 0, 0));
}

COLORREF CXTPTabColorSetVisualStudio2015::GetItemColor(CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT(pItem->GetTabManager());

	COLORREF clr = COLORREF_NULL;

	if (pItem->GetTabManager()->IsActive())
	{
		if (pItem->IsSelected())
			clr = m_ActiveTabButton.Selected.clrBack;
		else if (pItem->IsHighlighted())
			clr = m_ActiveTabButton.Highlighted.clrBack;
		else
			clr = m_ActiveTabButton.Normal.clrBack;
	}
	else
	{
		if (pItem->IsSelected())
			clr = m_InactiveTabButton.Selected.clrBack;
		else if (pItem->IsHighlighted())
			clr = m_InactiveTabButton.Highlighted.clrBack;
		else
			clr = m_InactiveTabButton.Normal.clrBack;
	}

	return clr;
}

COLORREF CXTPTabColorSetVisualStudio2015::GetItemBorderColor(CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT(pItem->GetTabManager());

	COLORREF clr = COLORREF_NULL;

	if (pItem->GetTabManager()->IsActive())
	{
		if (pItem->IsSelected())
			clr = m_ActiveTabButton.Selected.clrBorder;
		else if (pItem->IsHighlighted())
			clr = m_ActiveTabButton.Highlighted.clrBorder;
		else
			clr = m_ActiveTabButton.Normal.clrBorder;
	}
	else
	{
		if (pItem->IsSelected())
			clr = m_InactiveTabButton.Selected.clrBorder;
		else if (pItem->IsHighlighted())
			clr = m_InactiveTabButton.Highlighted.clrBorder;
		else
			clr = m_InactiveTabButton.Normal.clrBorder;
	}

	return clr;
}

CRect CXTPTabColorSetVisualStudio2015::GetItemBorderRect(CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT(pItem->GetTabManager());

	CRect rc;

	if (pItem->GetTabManager()->IsActive())
	{
		if (pItem->IsSelected())
			rc = m_ActiveTabButton.Selected.rcBorder;
		else if (pItem->IsHighlighted())
			rc = m_ActiveTabButton.Highlighted.rcBorder;
		else
			rc = m_ActiveTabButton.Normal.rcBorder;
	}
	else
	{
		if (pItem->IsSelected())
			rc = m_InactiveTabButton.Selected.rcBorder;
		else if (pItem->IsHighlighted())
			rc = m_InactiveTabButton.Highlighted.rcBorder;
		else
			rc = m_InactiveTabButton.Normal.rcBorder;
	}

	return rc;
}

void CXTPTabColorSetVisualStudio2015::SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pItem);
	ASSERT(pItem->GetTabManager());

	COLORREF clrText = COLORREF_NULL;

	if (pItem->GetTabManager()->IsActive())
	{
		if (pItem->IsSelected())
			clrText = m_ActiveTabButton.Selected.clrText;
		else if (pItem->IsHighlighted())
			clrText = m_ActiveTabButton.Highlighted.clrText;
		else
			clrText = m_ActiveTabButton.Normal.clrText;
	}
	else
	{
		if (pItem->IsSelected())
			clrText = m_InactiveTabButton.Selected.clrText;
		else if (pItem->IsHighlighted())
			clrText = m_InactiveTabButton.Highlighted.clrText;
		else
			clrText = m_InactiveTabButton.Normal.clrText;
	}

	pDC->SetTextColor(clrText);
}

COLORREF CXTPTabColorSetVisualStudio2015::GetClientColor(CXTPTabManager* pTabManager)
{
	ASSERT(pTabManager);
	return pTabManager->IsActive() ? m_clrActiveClientBack : m_clrInactiveClientBack;
}

COLORREF CXTPTabColorSetVisualStudio2015::FillPropertyButton(CDC* pDC, CRect rcItem,
															 CXTPTabManagerItem* pItem)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pItem);

	COLORREF clrItem   = GetItemColor(pItem);
	COLORREF clrBorder = GetItemBorderColor(pItem);
	CRect rcBorder	 = GetItemBorderRect(pItem);

	CRect rc(rcItem);
	pDC->FillSolidRect(rc, clrBorder);

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
			rc.left += rcBorder.left;
			rc.top += rcBorder.top;
			rc.right -= rcBorder.right;
			rc.bottom -= rcBorder.bottom;
			break;

		case xtpTabPositionBottom:
			rc.left += rcBorder.left;
			rc.top += rcBorder.bottom;
			rc.right -= rcBorder.right;
			rc.bottom -= rcBorder.top;
			break;

		case xtpTabPositionLeft:
			rc.left += rcBorder.top;
			rc.top += rcBorder.right;
			rc.right -= rcBorder.bottom;
			rc.bottom -= rcBorder.left;
			break;

		case xtpTabPositionRight:
			rc.left += rcBorder.bottom;
			rc.top += rcBorder.left;
			rc.right -= rcBorder.top;
			rc.bottom -= rcBorder.right;
			break;
	}

	pDC->FillSolidRect(rc, clrItem);

	return clrItem;
}

void CXTPTabColorSetVisualStudio2015::FillNavigateButton(CDC* pDC,
														 CXTPTabManagerNavigateButton* pButton,
														 CRect& rc, NAVIGATE_BUTTON_COLOR clr)
{
	ASSERT_VALID(pDC);
	ASSERT(pButton);

	CRect rcBack(rc);
	rcBack.DeflateRect(XTP_DPI_X(1), XTP_DPI_Y(1));

	pDC->FillSolidRect(rc, clr.clrBorder);
	pDC->FillSolidRect(rcBack, clr.clrBack);
	pButton->DrawEntry(pDC, rc, clr.clrText, pButton->IsEnabled());
}

void CXTPTabColorSetVisualStudio2015::FillNavigateButton(CDC* pDC,
														 CXTPTabManagerNavigateButton* pButton,
														 CRect& rc)
{
	ASSERT_VALID(pDC);
	ASSERT(pButton);

	BOOL navigatePressed   = pButton->IsPressed();
	BOOL navigateHilighted = pButton->IsHighlighted();

	if (pButton->IsOwnerNull())
	{
		if (navigatePressed)
			FillNavigateButton(pDC, pButton, rc, m_NavigationButton.Pressed);
		else if (navigateHilighted)
			FillNavigateButton(pDC, pButton, rc, m_NavigationButton.Highlighted);
		else
			FillNavigateButton(pDC, pButton, rc, m_NavigationButton.Normal);
	}
	else
	{
		BOOL tabGroupActive = pButton->IsOwnerActive();
		BOOL tabSelected	= pButton->IsOwnerSelected();
		BOOL tabHilighted   = pButton->IsOwnerHighlighted();

		if (tabGroupActive)
		{
			if (tabSelected && tabHilighted)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Normal);
			}
			else if (tabSelected)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Selected.Navigate.Normal);
			}
			else if (tabHilighted)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Highlighted.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Highlighted.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_ActiveTabButton.Highlighted.Navigate.Normal);
			}
			else
			{
				//
			}
		}
		else
		{
			if (tabSelected && tabHilighted)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Normal);
			}
			else if (tabSelected)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Selected.Navigate.Normal);
			}
			else if (tabHilighted)
			{
				if (navigatePressed)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Highlighted.Navigate.Pressed);
				else if (navigateHilighted)
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Highlighted.Navigate.Highlighted);
				else
					FillNavigateButton(pDC, pButton, rc,
									   m_InactiveTabButton.Highlighted.Navigate.Normal);
			}
			else
			{
				//
			}
		}
	}
}
