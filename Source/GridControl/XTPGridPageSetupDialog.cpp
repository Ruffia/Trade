// XTPGridPageSetupDialog.cpp : implementation of the CXTPGridPageSetupDialog class.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/Resource.h"
#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPageSetupDialog.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CXTPGridPageSetupDialog, CPageSetupDialog)
	ON_BN_CLICKED(XTP_IDC_HEADER_FORMAT_BTN, OnBnClickedHeaderFormat)
	ON_BN_CLICKED(XTP_IDC_FOOTER_FORMAT_BTN, OnBnClickedFooterFormat)
END_MESSAGE_MAP()

CXTPGridPageSetupDialog::CXTPGridPageSetupDialog(CXTPGridViewPrintOptions* pOptions, DWORD dwFlags,
												 CWnd* pParentWnd)
	: CPageSetupDialog(dwFlags, pParentWnd)
{
	ASSERT(pOptions);
	m_pOptions = pOptions;

	if (m_pOptions)
	{
		BOOL bIsInches  = m_pOptions->IsMarginsMeasureInches();
		DWORD dwMeasure = DWORD(bIsInches ? PSD_INTHOUSANDTHSOFINCHES
										  : PSD_INHUNDREDTHSOFMILLIMETERS);

		m_psd.Flags &= ~PSD_INWININIINTLMEASURE;
		m_psd.Flags |= dwMeasure;
	}

	m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATEHANDLE;
	m_psd.hPageSetupTemplate = (HGLOBAL)XTPResourceManager()->LoadDialogTemplate(IDD);
	ASSERT(m_psd.hPageSetupTemplate);

	if (m_pOptions)
		m_psd.rtMargin = m_pOptions->m_rcMargins;

	m_nIDHelp = CXTPGridPageSetupDialog::IDD;
}

CXTPGridPageSetupDialog::~CXTPGridPageSetupDialog()
{
}

#ifndef rad1
#	define rad1 0x0420
#	define rad2 0x0421

#	define grp4 0x0433
#endif

BOOL CXTPGridPageSetupDialog::OnInitDialog()
{
	CPageSetupDialog::OnInitDialog();

	ASSERT(m_pOptions);

	VERIFY(m_ctrlHeaderFormat.SubclassDlgItem(XTP_IDC_HEADER_FORMAT, this));
	VERIFY(m_ctrlFooterFormat.SubclassDlgItem(XTP_IDC_FOOTER_FORMAT, this));

	VERIFY(m_ctrlHeaderFormatBtn.SubclassDlgItem(XTP_IDC_HEADER_FORMAT_BTN, this));
	VERIFY(m_ctrlFooterFormatBtn.SubclassDlgItem(XTP_IDC_FOOTER_FORMAT_BTN, this));

	if (m_pOptions && m_pOptions->GetPageHeader())
		m_ctrlHeaderFormat.SetWindowText(m_pOptions->GetPageHeader()->m_strFormatString);

	if (m_pOptions && m_pOptions->GetPageFooter())
		m_ctrlFooterFormat.SetWindowText(m_pOptions->GetPageFooter()->m_strFormatString);

	if (GetDlgItem(rad1))
		GetDlgItem(rad1)->EnableWindow(TRUE); // 1056 Portrait
	if (GetDlgItem(rad2))
		GetDlgItem(rad2)->EnableWindow(TRUE); // 1057 Landscape

	BOOL bIsInches = m_pOptions ? m_pOptions->IsMarginsMeasureInches() : FALSE;

	UINT uStrID = UINT(bIsInches ? XTP_IDS_GRID_MARGINS_INCH : XTP_IDS_GRID_MARGINS_MM);
	CString strCaption;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strCaption, uStrID));

	if (!strCaption.IsEmpty() && GetDlgItem(grp4))
		GetDlgItem(grp4)->SetWindowText(strCaption);

	return FALSE;
}

void CXTPGridPageSetupDialog::OnOK()
{
	if (m_pOptions && m_pOptions->GetPageHeader())
		m_ctrlHeaderFormat.GetWindowText(m_pOptions->GetPageHeader()->m_strFormatString);

	if (m_pOptions && m_pOptions->GetPageFooter())
		m_ctrlFooterFormat.GetWindowText(m_pOptions->GetPageFooter()->m_strFormatString);

	if (m_pOptions)
		m_pOptions->m_rcMargins = m_psd.rtMargin;

	CPageSetupDialog::OnOK();
}

void CXTPGridPageSetupDialog::OnBnClickedHeaderFormat()
{
	CXTPPrintPageHeaderFooter::DoInsertHFFormatSpecifierViaMenu(this, &m_ctrlHeaderFormat,
																&m_ctrlHeaderFormatBtn);
}

void CXTPGridPageSetupDialog::OnBnClickedFooterFormat()
{
	CXTPPrintPageHeaderFooter::DoInsertHFFormatSpecifierViaMenu(this, &m_ctrlFooterFormat,
																&m_ctrlFooterFormatBtn);
}

/////////////////////////////////////////////////////////////////////////////
// class CXTPGridViewPrintOptions

IMPLEMENT_DYNAMIC(CXTPGridViewPrintOptions, CXTPPrintOptions)
CXTPGridViewPrintOptions::CXTPGridViewPrintOptions()
{
	m_bRepeatHeaderRows = FALSE;
	m_bRepeatFooterRows = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

LCID CXTPGridViewPrintOptions::GetActiveLCID()
{
	return CXTPGridControlLocale::GetActiveLCID();
}

void CXTPGridViewPrintOptions::Set(const CXTPGridViewPrintOptions* pSrc)
{
	if (pSrc)
	{
		CXTPPrintOptions::Set(pSrc);

		m_bRepeatHeaderRows = pSrc->m_bRepeatHeaderRows;
		m_bRepeatFooterRows = pSrc->m_bRepeatFooterRows;
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridViewPrintOptions, CXTPPrintOptions)
	INTERFACE_PART(CXTPGridViewPrintOptions, XTPDIID_GridPrintOptions, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridViewPrintOptions, XTPDIID_GridPrintOptions)

BEGIN_DISPATCH_MAP(CXTPGridViewPrintOptions, CXTPPrintOptions)
	DISP_PROPERTY_ID(CXTPGridViewPrintOptions, "RepeatHeaderRows", 1, m_bRepeatHeaderRows, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridViewPrintOptions, "RepeatFooterRows", 2, m_bRepeatFooterRows, VT_BOOL)
END_DISPATCH_MAP()

#endif
