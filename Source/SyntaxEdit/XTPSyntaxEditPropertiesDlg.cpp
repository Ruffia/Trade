// XTPSyntaxEditPropertiesDlg.cpp : implementation file
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyntaxEdit/Resource.h"

// common includes
#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"

// syntax editor includes
#include "SyntaxEdit/XTPSyntaxEditColorSampleText.h"
#include "SyntaxEdit/XTPSyntaxEditColorComboBox.h"
#include "SyntaxEdit/XTPSyntaxEditPropertiesPage.h"
#include "SyntaxEdit/XTPSyntaxEditPropertiesDlg.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditPropertiesDlg

IMPLEMENT_DYNAMIC(CXTPSyntaxEditPropertiesDlg, CPropertySheet)

CXTPSyntaxEditPropertiesDlg::CXTPSyntaxEditPropertiesDlg(CXTPSyntaxEditView* pEditView /*=NULL*/)
	: CPropertySheet(XTPResourceManager()->LoadString(XTP_IDD_EDIT_OPTIONS), (CWnd*)pEditView)
	, m_Page1(pEditView)
	, m_Page2(pEditView)
	, m_Page3(pEditView)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CXTPSyntaxEditPropertiesDlg::~CXTPSyntaxEditPropertiesDlg()
{
}

BEGIN_MESSAGE_MAP(CXTPSyntaxEditPropertiesDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CXTPSyntaxEditPropertiesDlg)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditPropertiesDlg message handlers
