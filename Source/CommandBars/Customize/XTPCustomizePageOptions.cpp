// XTPCustomizePageOptions.cpp : implementation of the CXTPCustomizeOptionsPage class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "TabManager/Includes.h"

#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Resize/XTPResizeDialog.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Scroll/XTPScrollBar.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlEdit.h"

#include "Controls/Dialog/XTPDialogBase.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "Controls/Dialog/XTPPropertyPageNavigator.h"
#include "CommandBars/Customize/XTPCustomizePageOptions.h"
#include "CommandBars/Customize/XTPCustomizeSheet.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGroupLine

CXTPGroupLine::CXTPGroupLine()
	: m_bEnableTheme(FALSE)
{
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeButton->OpenThemeData(0, L"BUTTON");
}

CXTPGroupLine::~CXTPGroupLine()
{
	SAFE_DELETE(m_themeButton);
}

BEGIN_MESSAGE_MAP(CXTPGroupLine, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CXTPGroupLine::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	COLORREF clrText = GetSysColor(COLOR_BTNTEXT);

	if (m_themeButton->IsAppThemeReady()
		&& FAILED(m_themeButton->GetThemeColor(BP_GROUPBOX, GBS_NORMAL, TMT_TEXTCOLOR, &clrText)))
	{
		clrText = GetSysColor(COLOR_BTNTEXT);
	}

	BOOL bRightToLeft = (0 != (GetExStyle() & WS_EX_RTLREADING));

	CString strText;
	GetWindowText(strText);

	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);
	dc.FillSolidRect(rc, m_bEnableTheme ? m_clrBack : GetSysColor(COLOR_WINDOW));

	CXTPFontDC font(&dc, GetFont());
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_bEnableTheme ? m_clrText : clrText);

	dc.DrawText(strText, rc,
				UINT(DT_SINGLELINE | DT_VCENTER
					 | (bRightToLeft ? DT_RIGHT | DT_RTLREADING : DT_LEFT)));

	CRect rcLine(0, rc.top, 0, rc.bottom);
	dc.DrawText(strText, rcLine, DT_SINGLELINE | DT_VCENTER | DT_CALCRECT);

	if (bRightToLeft)
		rcLine.SetRect(rc.left, rc.top, rc.right - +rcLine.Width() - XTP_DPI_X(6), rc.bottom);
	else
		rcLine.SetRect(rc.left + rcLine.Width() + XTP_DPI_X(6), rc.top, rc.right, rc.bottom);

	COLORREF clrLine = GetSysColor(COLOR_3DSHADOW);
	if (m_themeButton->IsAppThemeReady()
		&& SUCCEEDED(m_themeButton->GetThemeColor(BP_GROUPBOX, GBS_NORMAL, TMT_BORDERCOLORHINT,
												  &clrLine)))
	{
		dc.FillSolidRect(rcLine.left, rcLine.CenterPoint().y, rcLine.Width(), XTP_DPI_Y(1),
						 m_bEnableTheme ? m_clrLine : clrLine);
	}
	else
	{
		dc.FillSolidRect(rcLine.left, rcLine.CenterPoint().y, rcLine.Width(), XTP_DPI_Y(1),
						 GetSysColor(COLOR_3DSHADOW));
		dc.FillSolidRect(rcLine.left, rcLine.CenterPoint().y + XTP_DPI_Y(1), rcLine.Width(),
						 XTP_DPI_Y(1), GetSysColor(COLOR_3DHIGHLIGHT));
	}
}

void CXTPGroupLine::EnableTheme(BOOL bEnable)
{
	m_bEnableTheme = bEnable;
}

void CXTPGroupLine::SetBkColor(COLORREF clr)
{
	m_clrBack = clr;
}

void CXTPGroupLine::SetTextColor(COLORREF clr)
{
	m_clrText = clr;
	m_clrLine = clr;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeOptionsPage property page

CXTPCustomizeOptionsPage::CXTPCustomizeOptionsPage(CXTPCustomizeSheet* pSheet)
	: CXTPPropertyPage(CXTPCustomizeOptionsPage::IDD)
{
	CXTPCommandBars* pCommandBars	= pSheet->GetCommandBars();
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();

	m_nAnimationType = pOptions->animationType;

	//{{AFX_DATA_INIT(CXTPCustomizeOptionsPage)
	//}}AFX_DATA_INIT
	m_pSheet = pSheet;

	// RevertResourceContext();
}

CXTPCustomizeOptionsPage::~CXTPCustomizeOptionsPage()
{
}

CXTPCommandBars* CXTPCustomizeOptionsPage::GetCommandBars() const
{
	ASSERT(m_pSheet && m_pSheet->GetCommandBars());
	return m_pSheet ? m_pSheet->GetCommandBars() : NULL;
}

void CXTPCustomizeOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, XTP_IDC_COMBO_ANIMATION, m_comboAnimationType);
	DDX_CBIndex(pDX, XTP_IDC_COMBO_ANIMATION, m_nAnimationType);

	//{{AFX_DATA_MAP(CXTPCustomizeOptionsPage)
	DDX_Control(pDX, XTP_IDC_CHECK_FULL_MENUS, m_btnAlwaysShowFullMenus);
	DDX_Control(pDX, XTP_IDC_CHECK_AFTERDELAY, m_btnShowFullAfterDelay);
	DDX_Control(pDX, XTP_IDC_CHECK_LARGEICONS, m_btnLargeIcons);
	DDX_Control(pDX, XTP_IDC_CHECK_SCREENTTIPS, m_btnToolBarScreenTips);
	DDX_Control(pDX, XTP_IDC_CHECK_SHORTCUTS, m_btnToolBarAccelTips);
	DDX_Control(pDX, XTP_IDC_BUTTON_RESETDATA, m_btnResetData);
	//}}AFX_DATA_MAP
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPCustomizeOptionsPage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPCustomizeOptionsPage)
	ON_BN_CLICKED(XTP_IDC_CHECK_FULL_MENUS, OnCheckFullMenus)
	ON_BN_CLICKED(XTP_IDC_CHECK_AFTERDELAY, OnCheckAfterdelay)
	ON_BN_CLICKED(XTP_IDC_CHECK_LARGEICONS, OnCheckLargeicons)
	ON_BN_CLICKED(XTP_IDC_CHECK_SCREENTTIPS, OnCheckScreenttips)
	ON_BN_CLICKED(XTP_IDC_CHECK_SHORTCUTS, OnCheckShortcuts)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RESETDATA, OnResetData)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_ANIMATION, OnAnimationChanged)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeOptionsPage message handlers

void CXTPCustomizeOptionsPage::AddComboString(UINT nIDResource)
{
	CString str;
	XTPResourceManager()->LoadString(&str, nIDResource);

	m_comboAnimationType.AddString(str);
}

BOOL CXTPCustomizeOptionsPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	CXTPCommandBarsOptions* pOptions = m_pSheet->GetCommandBars()->GetCommandBarsOptions();
	if (pOptions)
	{
		m_btnLargeIcons.SetCheck(pOptions->bLargeIcons ? TRUE : FALSE);
		m_btnShowFullAfterDelay.SetCheck(pOptions->bShowFullAfterDelay ? TRUE : FALSE);
		m_btnToolBarScreenTips.SetCheck(pOptions->bToolBarScreenTips ? TRUE : FALSE);
		m_btnToolBarAccelTips.SetCheck(pOptions->bToolBarAccelTips ? TRUE : FALSE);
		m_btnAlwaysShowFullMenus.SetCheck(pOptions->bAlwaysShowFullMenus ? TRUE : FALSE);

		m_btnShowFullAfterDelay.EnableWindow(!pOptions->bAlwaysShowFullMenus);
		m_btnToolBarAccelTips.EnableWindow(pOptions->bToolBarScreenTips);
	}
	else
	{
		ASSERT(FALSE);
	}

	if (GetDlgItem(XTP_IDC_GROUP_PERSONALIZED))
		m_wndPersonalizedGroup.SubclassDlgItem(XTP_IDC_GROUP_PERSONALIZED, this);

	if (GetDlgItem(XTP_IDC_GROUP_OTHER))
		m_wndOtherGroup.SubclassDlgItem(XTP_IDC_GROUP_OTHER, this);

	// Add strings to animation combo box.
	AddComboString(XTP_IDS_ANIMATION_SYSDEF);
	AddComboString(XTP_IDS_ANIMATION_RANDOM);
	AddComboString(XTP_IDS_ANIMATION_UNFOLD);
	AddComboString(XTP_IDS_ANIMATION_SLIDE);
	AddComboString(XTP_IDS_ANIMATION_FADE);
	AddComboString(XTP_IDS_ANIMATION_NONE);

	// Ensure combo selection is made.
	UpdateData(FALSE);

	if (GetDlgItem(XTP_IDC_GROUP_PERSONALIZED))
		SetResize(XTP_IDC_GROUP_PERSONALIZED, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	if (GetDlgItem(XTP_IDC_GROUP_OTHER))
		SetResize(XTP_IDC_GROUP_OTHER, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

#ifdef _XTP_INCLUDE_CONTROLS
	XTPControlTheme controlTheme = m_pSheet->GetTheme();

	m_btnResetData.SetTheme(controlTheme);
	m_btnAlwaysShowFullMenus.SetTheme(controlTheme);
	m_btnShowFullAfterDelay.SetTheme(controlTheme);
	m_btnLargeIcons.SetTheme(controlTheme);
	m_btnToolBarScreenTips.SetTheme(controlTheme);
	m_btnToolBarAccelTips.SetTheme(controlTheme);
	m_comboAnimationType.SetTheme(controlTheme);

	m_clrText = XTPIniColor(_T("CommandBars.Toolbar"), _T("ButtonText"),
							GetXtremeColor(COLOR_WINDOWTEXT));

	if (controlTheme == xtpControlThemeResource || controlTheme == xtpControlThemeOffice2013
		|| controlTheme == xtpControlThemeOffice2016
		|| controlTheme == xtpControlThemeVisualStudio2015
		|| controlTheme == xtpControlThemeVisualStudio2017
		|| controlTheme == xtpControlThemeVisualStudio2019
		|| controlTheme == xtpControlThemeVisualStudio2022
		|| controlTheme == xtpControlThemeNativeWindows10)
	{
		m_wndPersonalizedGroup.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);
		m_wndPersonalizedGroup.SetTextColor(m_clrText);
		m_wndPersonalizedGroup.SetBkColor(m_pSheet->GetNavigator()->GetPageColor());

		m_wndOtherGroup.EnableTheme(controlTheme != xtpControlThemeDefault ? TRUE : FALSE);
		m_wndOtherGroup.SetTextColor(m_clrText);
		m_wndOtherGroup.SetBkColor(m_pSheet->GetNavigator()->GetPageColor());
	}

#endif

	return TRUE;
}

void CXTPCustomizeOptionsPage::OnCheckFullMenus()
{
	BOOL state = m_btnAlwaysShowFullMenus.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	GetCommandBars()->GetCommandBarsOptions()->bAlwaysShowFullMenus = state;

	m_btnShowFullAfterDelay.EnableWindow(!state);
}

void CXTPCustomizeOptionsPage::OnAnimationChanged()
{
	UpdateData();
	if (m_nAnimationType != CB_ERR)
	{
		GetCommandBars()->GetCommandBarsOptions()->animationType = (XTPAnimationType)
			m_nAnimationType;
	}
}

void CXTPCustomizeOptionsPage::OnCheckAfterdelay()
{
	BOOL state = m_btnShowFullAfterDelay.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	GetCommandBars()->GetCommandBarsOptions()->bShowFullAfterDelay = state;
}

void CXTPCustomizeOptionsPage::OnCheckLargeicons()
{
	BOOL state = m_btnLargeIcons.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	GetCommandBars()->GetCommandBarsOptions()->bLargeIcons = state;
	GetCommandBars()->RedrawCommandBars();
}

void CXTPCustomizeOptionsPage::OnCheckScreenttips()
{
	BOOL state = m_btnToolBarScreenTips.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	GetCommandBars()->GetCommandBarsOptions()->bToolBarScreenTips = state;

	m_btnToolBarAccelTips.EnableWindow(state);
}

void CXTPCustomizeOptionsPage::OnCheckShortcuts()
{
	BOOL state = m_btnToolBarAccelTips.GetCheck() == BST_CHECKED ? TRUE : FALSE;
	GetCommandBars()->GetCommandBarsOptions()->bToolBarAccelTips = state;
}

void CXTPCustomizeOptionsPage::OnResetData()
{
	CString strPrompt;
	CXTPResourceManager::AssertValid(
		XTPResourceManager()->LoadString(&strPrompt, XTP_IDS_CONFIRM_RESETDATA));

	if (m_pSheet->MessageBox(strPrompt, MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2) == IDYES)
	{
		GetCommandBars()->ResetUsageData();
	}
}

HBRUSH CXTPCustomizeOptionsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush = CXTPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

#ifdef _XTP_INCLUDE_CONTROLS
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (m_pSheet->GetTheme() != xtpControlThemeDefault)
			::SetTextColor(pDC->GetSafeHdc(), m_clrText);
	}
#endif

	return hBrush;
}
