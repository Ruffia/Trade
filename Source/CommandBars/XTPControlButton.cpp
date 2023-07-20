// XTPControlButton.cpp : implementation of the CXTPControlButton class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPControlButton

IMPLEMENT_XTP_CONTROL(CXTPControlButton, CXTPControl)

CXTPControlButton::CXTPControlButton()
{
	EnableAutomation();

	m_controlType   = xtpControlButton;
	m_buttonStyleEx = xtpButtonAutomatic;
}

CXTPControlButton::~CXTPControlButton()
{
}

void CXTPControlButton::OnLButtonUp(CPoint /*point*/)
{
	if (GetEnabled()
		&& (m_pParent->GetType() == xtpBarTypePopup || m_pParent->GetPosition() == xtpBarPopup))
	{
		OnExecute();
	}
}

void CXTPControlButton::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!GetEnabled())
		return;

	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
		return;
	}

	if (!bKeyboard)
	{
		if (m_pParent->GetType() != xtpBarTypePopup || m_nExecuteOnPressInterval > 0)
			ClickToolBarButton();
	}
	else
	{
		OnExecute();
	}
}

void CXTPControlButton::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		ASSERT_KINDOF(CXTPControlButton, pControl);

		CXTPControl::Copy(pControl, bRecursive);

		CXTPControlButton* pControlButton = DYNAMIC_DOWNCAST(CXTPControlButton, pControl);
		if (pControlButton)
		{
			m_buttonStyleEx = pControlButton->m_buttonStyleEx;
		}
	}
}

void CXTPControlButton::SetStyleEx(XTPButtonStyle buttonStyle)
{
	if (m_buttonStyleEx != buttonStyle)
	{
		m_buttonStyleEx = buttonStyle;
		DelayLayoutParent();
	}
}

XTPButtonStyle CXTPControlButton::GetStyleEx() const
{
	return m_buttonStyleEx;
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlButton, CXTPControl)
	DISP_PROPERTY_EX_ID(CXTPControl, "StyleEx", 220, OleGetStyleEx, OleSetStyleEx, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlButton, CXTPControl)
	INTERFACE_PART(CXTPControlButton, XTPDIID_ICommandBarButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlButton, XTPDIID_ICommandBarButton)

XTPButtonStyle CXTPControlButton::OleGetStyleEx()
{
	return GetStyleEx();
}

void CXTPControlButton::OleSetStyleEx(XTPButtonStyle nStyle)
{
	SetStyleEx(nStyle);
}

#endif
