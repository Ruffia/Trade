// XTPCommandBarScrollBarCtrl.cpp : implementation of the CXTPCommandBarScrollBarCtrl class.
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/ScrollBar/XTPCommandBarScrollBarCtrl.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarScrollBarCtrl

CXTPCommandBarScrollBarCtrl::CXTPCommandBarScrollBarCtrl()
{
	CXTPPaintManager::GetInstanceAddRef();

	m_pCommandBars = 0;
}

CXTPCommandBarScrollBarCtrl::~CXTPCommandBarScrollBarCtrl()
{
	CXTPPaintManager::ReleaseInstance();
}

BEGIN_MESSAGE_MAP(CXTPCommandBarScrollBarCtrl, CXTPScrollBarCtrl)
	//{{AFX_MSG_MAP(CXTPStatusBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPScrollBarPaintManager* CXTPCommandBarScrollBarCtrl::GetScrollBarPaintManager() const
{
	CXTPScrollBarPaintManager* pScrollBarPaintManager = NULL;

	if (NULL != m_pCommandBars)
	{
		pScrollBarPaintManager = m_pCommandBars->GetPaintManager()->GetScrollBarPaintManager();
	}
	else
	{
		pScrollBarPaintManager = XTPPaintManager()->GetScrollBarPaintManager();
	}

	return pScrollBarPaintManager;
}
