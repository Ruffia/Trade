// XTPCalendarEventColorDlg.cpp : implementation file
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
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Defines.h"
#include "Controls/Popup/XTPColorSelectorCtrl.h"
#include "Controls/Button/XTPButton.h"
#include "Controls/Button/XTPColorPicker.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarEventLabel.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarViewEvent.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarEventColorListBox.h"
#include "Calendar/XTPCalendarEventColorDlg.h"
#include "Calendar/XTPCalendarUtils.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorDlg dialog

IMPLEMENT_DYNAMIC(CXTPCalendarEventColorDlg, CDialog)

CXTPCalendarEventColorDlg::CXTPCalendarEventColorDlg(BOOL bCategories,
													 CXTPCalendarControl* pCalendarCtrl,
													 CWnd* pParent /*=NULL*/)
	: m_bCategories(bCategories)
	, m_pCalendarCtrl(pCalendarCtrl)
{
	ASSERT(m_pCalendarCtrl);

	//{{AFX_DATA_INIT(CXTPCalendarEventColorDlg)
	//}}AFX_DATA_INIT

	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(CXTPCalendarEventColorDlg::IDD),
					  pParent);

	CXTPCalendarViewEvent* pViewEvent = XTP_SAFE_GET2(pCalendarCtrl, GetActiveView(),
													  GetLastSelectedViewEvent(), NULL);

	if (pViewEvent && pViewEvent->IsShowingToolTip())
	{
		pViewEvent->HideToolTip();
	}

	CXTPCalendarEvent* pEvent = XTP_SAFE_GET1(pViewEvent, GetEvent(), NULL);

	m_pEvent.SetPtr(pEvent->CloneEvent(), TRUE);
}

void CXTPCalendarEventColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCalendarEventColorDlg)
	DDX_Control(pDX, XTP_IDC_EVENT_INFO, m_wndEventInfo);
	DDX_Control(pDX, XTP_IDC_EVENT_COLORPICKER, m_cpEvent);
	DDX_Control(pDX, XTP_IDC_EVENT_LISTBOX, m_lboxColors);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXTPCalendarEventColorDlg, CDialog)
	//{{AFX_MSG_MAP(CXTPCalendarEventColorDlg)
	ON_BN_CLICKED(XTP_IDC_EVENT_RENAME, OnBtnRename)
	ON_BN_CLICKED(XTP_IDC_EVENT_DELETE, OnBtnDelete)
	ON_BN_CLICKED(XTP_IDC_EVENT_NEW, OnBtnNew)
	ON_LBN_SELCHANGE(XTP_IDC_EVENT_LISTBOX, OnSelChangeListCategories)
	ON_BN_CLICKED(XTP_IDC_EVENT_OK, OnBtnOK)
	ON_BN_CLICKED(XTP_IDC_EVENT_RESET, OnEventReset)
	//}}AFX_MSG_MAP
	ON_CPN_XTP_SELENDOK(XTP_IDC_EVENT_COLORPICKER, OnSelEndOkColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventColorDlg message handlers

BOOL CXTPCalendarEventColorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CXTPCalendarData* pData = XTP_SAFE_GET1(m_pCalendarCtrl, GetDataProvider(), NULL);
	ASSERT(pData);

	if (!pData)
		return FALSE;

	m_lboxColors.Init(pData, m_pEvent, m_bCategories);

	m_cpEvent.ShowText(FALSE);
	m_cpEvent.ModifyCPStyle(0, CPS_XTP_SHOWEYEDROPPER);

	if (m_bCategories)
	{
		m_wndEventInfo.SetWindowText(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_CATEGORYINFO));
		SetWindowText(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_CATEGORYTITLE));
	}
	else
	{
		m_wndEventInfo.SetWindowText(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_LABELINFO));
		SetWindowText(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_LABELTITLE));
	}

	UpdateSelColor();

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPCalendarEventColorDlg::OnBtnRename()
{
	m_lboxColors.BeginEdit();
}

void CXTPCalendarEventColorDlg::OnBtnDelete()
{
	m_lboxColors.DeleteItem();
	UpdateSelColor();
}

void CXTPCalendarEventColorDlg::OnBtnNew()
{
	int nIndex = m_lboxColors.AddItem();

	if (nIndex != LB_ERR)
	{
		UpdateSelColor(nIndex);
	}
}

void CXTPCalendarEventColorDlg::UpdateSelColor(int nIndex /*=LB_ERR*/)
{
	if (nIndex == LB_ERR)
	{
		nIndex = m_lboxColors.GetCurSel();
	}
	else
	{
		m_lboxColors.SetCurSel(nIndex);
	}

	if (m_bCategories)
	{
		CXTPCalendarEventCategory* pCategory = m_lboxColors.GetCategory(nIndex);

		if (!pCategory)
			return;

		m_cpEvent.SetColor(pCategory->GetBackgroundColor());
		m_cpEvent.SetDefaultColor(pCategory->GetBackgroundColor());
	}
	else
	{
		CXTPCalendarEventLabel* pLabel = m_lboxColors.GetLabel(nIndex);

		if (!pLabel)
			return;

		m_cpEvent.SetColor(pLabel->m_clrColor);
		m_cpEvent.SetDefaultColor(pLabel->m_clrColor);
	}
}

void CXTPCalendarEventColorDlg::OnSelChangeListCategories()
{
	UpdateSelColor();
}

void CXTPCalendarEventColorDlg::OnSelEndOkColor()
{
	m_lboxColors.SetItemColor(m_cpEvent.GetColor());
}

void CXTPCalendarEventColorDlg::OnBtnOK()
{
	if (!m_pCalendarCtrl)
		return;

	CXTPCalendarData* pData = XTP_SAFE_GET1(m_pCalendarCtrl, GetDataProvider(), NULL);
	ASSERT(pData);

	if (!pData)
		return;

	if (m_bCategories)
	{
		m_lboxColors.UpdateCategories(pData);
	}
	else
	{
		m_lboxColors.UpdateLabels(pData);
	}

	CXTPCalendarControl::CUpdateContext updateContext(m_pCalendarCtrl,
													  xtpCalendarUpdateLayout
														  | xtpCalendarUpdateRedraw);

	XTP_SAFE_CALL1(pData, ChangeEvent(m_pEvent));

	CDialog::OnOK();
}

void CXTPCalendarEventColorDlg::OnEventReset()
{
	CXTPCalendarData* pData = XTP_SAFE_GET1(m_pCalendarCtrl, GetDataProvider(), NULL);
	ASSERT(pData);

	if (!pData)
		return;

	if (m_bCategories)
	{
		if (AfxMessageBox(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_RESTORE_CATEGORYLIST),
						  MB_YESNO | MB_ICONQUESTION)
			== IDYES)
		{
			pData->RestoreEventCategoryList();

			XTP_SAFE_CALL2(m_pCalendarCtrl, GetPaintManager(),
						   UpdateEventCategoriesDefaultColors());

			m_lboxColors.Init(pData, m_pEvent, TRUE);
			UpdateSelColor(0);
		}
	}
	else
	{
		if (AfxMessageBox(CXTPCalendarUtils::LoadString(XTP_IDS_CALENDAR_RESTORE_LABELLIST),
						  MB_YESNO | MB_ICONQUESTION)
			== IDYES)
		{
			pData->RestoreEventLabelList();

			XTP_SAFE_CALL2(m_pCalendarCtrl, GetPaintManager(), UpdateEventLabelsDefaultColors());

			m_lboxColors.Init(pData, m_pEvent, FALSE);
			UpdateSelColor(m_lboxColors.GetCurSel());
		}
	}
}
