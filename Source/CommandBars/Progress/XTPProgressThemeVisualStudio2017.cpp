// XTPProgressThemeVisualStudio2017.cpp : implementation of the CXTPProgressThemeVisualStudio2017
// class.
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
#include "Common/XTPColorManager.h"

#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/Progress/XTPProgressThemeOffice2013.h"
#include "CommandBars/Progress/XTPProgressThemeVisualStudio2017.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPProgressThemeVisualStudio2017

CXTPProgressThemeVisualStudio2017::CXTPProgressThemeVisualStudio2017(CXTPPaintManager* pPaintManager)
	: CXTPProgressThemeOffice2013(pPaintManager)
{
}

CXTPProgressThemeVisualStudio2017::~CXTPProgressThemeVisualStudio2017()
{
}
