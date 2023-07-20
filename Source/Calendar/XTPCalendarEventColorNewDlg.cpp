// XTPCalendarEventColorNewDlg.cpp : implementation file
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

#include "Calendar/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Defines.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPColorPicker.h"

#include "Calendar/XTPCalendarEventColorNewDlg.h"
#include "Calendar/XTPCalendarUtils.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorNewDlg dialog

IMPLEMENT_DYNAMIC(CXTPCalendarEventColorNewDlg, CDialog)

CXTPCalendarEventColorNewDlg::CXTPCalendarEventColorNewDlg(BOOL bCategories,
														   CWnd* pParent /*=NULL*/)
	: m_bCategories(bCategories)
{
	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(CXTPCalendarEventColorNewDlg::IDD),
					  pParent);

	//{{AFX_DATA_INIT(CXTPCalendarEventColorNewDlg)
	m_strName = _T("");
	m_crColor = COLORREF_NULL;
	//}}AFX_DATA_INIT
}

void CXTPCalendarEventColorNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCalendarEventColorNewDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, XTP_IDC_EVENT_COLORPICKER, m_cpEventColor);
	DDX_XTPColorPicker(pDX, XTP_IDC_EVENT_COLORPICKER, m_crColor);
	DDX_Text(pDX, XTP_IDC_EVENT_NAME, m_strName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXTPCalendarEventColorNewDlg, CDialog)
	//{{AFX_MSG_MAP(CXTPCalendarEventColorNewDlg)
	ON_EN_CHANGE(XTP_IDC_EVENT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
	ON_CPN_XTP_SELENDOK(XTP_IDC_EVENT_COLORPICKER, OnSelEndOkColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorNewDlg message handlers

void CXTPCalendarEventColorNewDlg::OnSelEndOkColor()
{
	UpdateData();
	EnableOkButton();
}

void CXTPCalendarEventColorNewDlg::OnChangeEditName()
{
	UpdateData();
	EnableOkButton();
}

BOOL CXTPCalendarEventColorNewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_bCategories)
	{
		SetWindowText(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_NEWLABELTITLE));
	}

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPCalendarEventColorNewDlg::EnableOkButton()
{
	m_btnOk.EnableWindow(!m_strName.IsEmpty() && m_crColor != COLORREF_NULL);
}
