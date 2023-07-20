// XTPTabColorSetOffice2007Access.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"

#include "TabManager/XTPTabManagerDefines.h"
#include "TabManager/XTPTabPaintManager.h"
#include "TabManager/XTPTabPaintManagerColorSet.h"
#include "TabManager/ColorSets/XTPTabColorSetDefault.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2003.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2007.h"
#include "TabManager/ColorSets/XTPTabColorSetOffice2007Access.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//

void CXTPTabColorSetOffice2007Access::RefreshMetrics()
{
	CXTPTabColorSetOffice2007::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_clrTextSelected.SetStandardValue(0);
		m_clrTextNormal.SetStandardValue(0);
		m_clrTextHighlight.SetStandardValue(0);

		m_clrFrameBorder.SetStandardValue(
			pImages->GetImageColor(_T("TabManager"), _T("AccessBorder")));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(m_clrFrameBorder);

		m_clrHeaderFace.SetStandardValue(
			pImages->GetImageColor(_T("TabManager"), _T("AccessHeader"), RGB(196, 218, 250)));
		m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);

		m_clrButtonText.SetStandardValue(pImages->GetImageColor(_T("Ribbon"), _T("TabNormalText")));
	}
}
