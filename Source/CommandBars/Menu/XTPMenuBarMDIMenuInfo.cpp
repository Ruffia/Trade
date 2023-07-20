// XTPMenuBarMDIMenuInfo.cpp : implementation of the CXTPMenuBarMDIMenuInfo class.
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
#include "Common/XTPIntel80Helpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPControls.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPMenuBarMDIMenuInfo::CXTPMenuBarMDIMenuInfo(CXTPCommandBars* pCommandBars, UINT nIDResource)
{
	ASSERT(pCommandBars);
	ASSERT(nIDResource != 0);

	m_pControls = new CXTPControls;
	m_pControls->SetCommandBars(pCommandBars);

	m_nIDResource = nIDResource;
	m_bChanged	= FALSE;

#ifndef _XTP_COMMANDBARS_ACTIVEX
	m_hIcon = AfxGetApp()->LoadIcon(nIDResource);

#	pragma warning(disable : 4510)
#	pragma warning(disable : 4610)

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos)
	{
		CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(pos);

		_XTP_USES_PROTECTED_ACCESS_ARGS(CXTPMenuBarMDIMenuInfo, CDocTemplate,
										(UINT nIDResource, CRuntimeClass * pDocClass,
										 CRuntimeClass * pFrameClass, CRuntimeClass * pViewClass),
										(nIDResource, pDocClass, pFrameClass, pViewClass), UINT,
										m_nIDResource);
		UINT nID = _XTP_PROTECTED_ACCESS(CDocTemplate, pDocTemplate, m_nIDResource);

		if (nIDResource == nID)
		{
			pDocTemplate->GetDocString(m_strTitle, CDocTemplate::docName);
			pDocTemplate->GetDocString(m_strDescription, CDocTemplate::regFileTypeName);
			break;
		}
	}
#else
	m_hIcon = NULL;

	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMenuBarMDIMenuInfo::~CXTPMenuBarMDIMenuInfo()
{
	m_pControls->InternalRelease();
}
