// XTPStatusBarPaneProgress.cpp
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
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/Controls/Progress/XTPProgressBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlProgress.h"
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPaneProgress.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarProgressPane

IMPLEMENT_DYNAMIC(CXTPStatusBarProgressPane, CXTPStatusBarPane)

CXTPStatusBarProgressPane::CXTPStatusBarProgressPane()
{
	m_nStyle |= SBPS_NOBORDERS;
	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);
	m_cxText = XTP_DPI_X(100);
}

CXTPStatusBarProgressPane::~CXTPStatusBarProgressPane()
{
}

CXTPProgressPaintManager* CXTPStatusBarProgressPane::GetProgressPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetProgressPaintManager();
}

void CXTPStatusBarProgressPane::RedrawProgress()
{
	Redraw();
}

CRect CXTPStatusBarProgressPane::GetProgressRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(m_rcPadding);

	int nHeight = GetProgressPaintManager()->m_cyProgress;
	if (rcItem.Height() > nHeight)
	{
		rcItem.top	= (rcItem.top + rcItem.bottom - nHeight) / 2;
		rcItem.bottom = rcItem.top + nHeight;
	}

	return rcItem;
}

void CXTPStatusBarProgressPane::OnDraw(CDC* pDC, CRect /*rcItem*/)
{
	CXTPProgressPaintManager* pPaintManager = GetProgressPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawProgress(pDC, this);
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarProgressPane, CXTPStatusBarPane)
	DISP_PROPERTY_ID(CXTPStatusBarProgressPane, "Min", 120, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPStatusBarProgressPane, "Max", 121, m_nMax, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarProgressPane, "Value", 122, OleGetPos, OleSetPos, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarProgressPane, "RedrawProgress", 334, OleRedrawProgress, VT_EMPTY,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPStatusBarProgressPane, CXTPStatusBarPane)
	INTERFACE_PART(CXTPStatusBarProgressPane, XTPDIID_StatusBarProgressPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarProgressPane, XTPDIID_StatusBarProgressPane)

int CXTPStatusBarProgressPane::OleGetPos()
{
	return GetPos();
}

void CXTPStatusBarProgressPane::OleSetPos(int nPos)
{
	SetPos(nPos);
}

void CXTPStatusBarProgressPane::OleRedrawProgress()
{
	if (this && m_pStatusBar && m_pStatusBar->GetSafeHwnd() && IsVisible())
	{
		m_pStatusBar->InvalidateRect(GetRect());
		UpdateWindow(m_pStatusBar->GetSafeHwnd());
	}
}

#endif
