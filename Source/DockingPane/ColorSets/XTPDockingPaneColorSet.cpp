// XTPDockingPaneColorSet.cpp
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
#include "Common/XTPColorManager.h"
#include "Common/XTPTheme.h"

#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneCaptionButton.h"
#include "DockingPane/ColorSets/XTPDockingPaneColorSet.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPDockingPaneColorSet::~CXTPDockingPaneColorSet()
{
}

CXTPColorSetCaptionButton&
	CXTPDockingPaneColorSet::GetButtonColorSet(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton)
	{
		if (pButton->IsPressed())
			return m_csCaptionButtonPressed;

		if (pButton->IsSelected())
			return m_csCaptionButtonHot;
	}

	return m_csCaptionButtonNormal;
}
