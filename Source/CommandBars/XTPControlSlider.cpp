// XTPControlSlider.cpp : implementation of the CXTPControlSlider class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPToolTipContext.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlSlider.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CXTPControlSlider, CXTPControl)

CXTPControlSlider::CXTPControlSlider()
{
	m_nWidth				  = XTP_DPI_X(150);
	m_pSliderCtrl			  = new CXTPControlSliderCtrl;
	m_pSliderCtrl->m_pControl = this;

	m_nMax = 100;
	m_nMin = 0;
	m_nPos = 0;

	m_rcPadding.SetRect(XTP_DPI_X(2), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));

	m_dwSliderStyle = TBS_BOTH | TBS_NOTICKS;
}

CXTPControlSlider::~CXTPControlSlider()
{
	delete m_pSliderCtrl;
}

DWORD CXTPControlSlider::GetSliderStyle() const
{
	return m_dwSliderStyle;
}

void CXTPControlSlider::SetSliderStyle(DWORD dwStyle)
{
	m_dwSliderStyle = dwStyle;
}

void CXTPControlSlider::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControl::Copy(pControl, bRecursive);

		CXTPControlSlider* pControlSlider = DYNAMIC_DOWNCAST(CXTPControlSlider, pControl);
		if (!pControlSlider)
			return;

		SetRange(pControlSlider->m_nMin, pControlSlider->m_nMax);
		SetPos(pControlSlider->GetPos());
		m_dwSliderStyle = pControlSlider->m_dwSliderStyle;
	}
}

void CXTPControlSlider::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Int(pPX, _T("Min"), m_nMin, 0);
	PX_Int(pPX, _T("Max"), m_nMax, 100);
	PX_Int(pPX, _T("Pos"), m_nPos, 0);

	if (pPX->GetSchema() > _XTP_SCHEMA_1121)
	{
		PX_DWord(pPX, _T("SliderStyle"), m_dwSliderStyle, TBS_BOTH | TBS_NOTICKS);
	}
}

CSize CXTPControlSlider::GetSize(CDC* /*pDC*/)
{
	int nButtonHeight = GetPaintManager()->GetControlHeight();

	if (m_pSliderCtrl->GetSafeHwnd() && m_pSliderCtrl->GetStyle() & TBS_VERT)
	{
		return CSize(nButtonHeight, GetWidth());
	}
	return CSize(GetWidth(), nButtonHeight);
}

void CXTPControlSlider::Draw(CDC* /*pDC*/)
{
}

void CXTPControlSlider::SetRect(CRect rcControl)
{
	// If layout is changed always make repaint
	if (!IsLayoutChanged())
	{
		if (m_rcControl == rcControl && !(m_pSliderCtrl && !m_pSliderCtrl->GetSafeHwnd()))
		{
			if (!(m_pSliderCtrl && m_pSliderCtrl->GetSafeHwnd()
				  && m_pSliderCtrl->GetParent() != m_pParent))
				return;
		}
	}

	if (m_pSliderCtrl && m_pSliderCtrl->GetSafeHwnd() && m_pSliderCtrl->GetParent() != m_pParent)
	{
		m_pSliderCtrl->DestroyWindow();
	}

	m_rcControl = rcControl;

	if (m_pSliderCtrl)
	{
		if (!m_pSliderCtrl->GetSafeHwnd())
		{
			m_pSliderCtrl->Create(WS_CHILD | m_dwSliderStyle, CRect(0, 0, 0, 0), m_pParent, 0);
			m_pSliderCtrl->SetDlgCtrlID(0); // Prevent Visual Studio 2010 to assign non zero ID.

			m_pSliderCtrl->SetRange(m_nMin, m_nMax);
			m_pSliderCtrl->SetPos(m_nPos);
		}

		rcControl.DeflateRect(m_rcPadding);

		if (m_pParent->GetType() == xtpBarTypePopup && m_pParent->GetPosition() == xtpBarPopup)
		{
			CXTPPaintManager* pPaintManager = GetPaintManager();
			rcControl.left += pPaintManager->GetPopupBarGripperWidth(GetParent())
							  + pPaintManager->m_nPopupBarTextPadding - XTP_DPI_X(2);
		}

		m_pSliderCtrl->MoveWindow(rcControl);
		m_pSliderCtrl->SetWindowPos(0, 0, 0, 0, 0,
									UINT(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
										 | ((m_dwHideFlags != xtpNoHide) ? SWP_HIDEWINDOW
																		 : SWP_SHOWWINDOW)));

		m_pSliderCtrl->EnableWindow(m_bEnabled);
		m_pSliderCtrl->Invalidate(FALSE);
	}
}

void CXTPControlSlider::SetPos(int nPos)
{
	if (m_pSliderCtrl->GetSafeHwnd() && ::GetCapture() == m_pSliderCtrl->GetSafeHwnd())
		return;

	m_nPos = nPos;

	if (m_pSliderCtrl->GetSafeHwnd())
	{
		m_pSliderCtrl->SetPos(nPos);
	}
}

int CXTPControlSlider::GetPos()
{
	if (m_pSliderCtrl->GetSafeHwnd())
	{
		return m_nPos = m_pSliderCtrl->GetPos();
	}

	return m_nPos;
}

void CXTPControlSlider::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;

	if (m_pSliderCtrl->GetSafeHwnd())
	{
		m_pSliderCtrl->SetRange(nMin, nMax);
	}
}

BOOL CXTPControlSlider::HasFocus() const
{
	return ::GetCapture() == m_pSliderCtrl->GetSafeHwnd();
}

void CXTPControlSlider::OnEnabledChanged()
{
	if (m_pSliderCtrl && m_pSliderCtrl->GetSafeHwnd())
		m_pSliderCtrl->EnableWindow(GetEnabled());
}

void CXTPControlSlider::SetParent(CXTPCommandBar* pParent)
{
	CXTPControl::SetParent(pParent);

	if (!pParent && m_pSliderCtrl && m_pSliderCtrl->GetSafeHwnd())
	{
		m_pSliderCtrl->DestroyWindow();
	}
}

void CXTPControlSlider::SetHideFlags(DWORD dwFlags)
{
	if (m_dwHideFlags != dwFlags)
	{
		m_dwHideFlags = dwFlags;

		if (m_pSliderCtrl && m_pSliderCtrl->GetSafeHwnd())
			m_pSliderCtrl->SetWindowPos(0, 0, 0, 0, 0,
										UINT(SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
											 | ((m_dwHideFlags != xtpNoHide) ? SWP_HIDEWINDOW
																			 : SWP_SHOWWINDOW)));
		DelayLayoutParent();
	}
}

void CXTPControlSlider::OnCalcDynamicSize(DWORD dwMode)
{
	if (m_pSliderCtrl->GetSafeHwnd())
	{
		if (dwMode & LM_VERTDOCK)
			m_pSliderCtrl->ModifyStyle(0, TBS_VERT);
		else
			m_pSliderCtrl->ModifyStyle(TBS_VERT, 0);
	}
}

void CXTPControlSlider::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (IsCustomizeMode() && !bKeyboard)
	{
		CustomizeStartDrag(pt);
	}
}

void CXTPControlSlider::OnScroll(UINT nSBCode, UINT /*nPos*/)
{
	if (nSBCode != SB_ENDSCROLL)
	{
		m_nPos = GetPos();

		NotifySite(CBN_XTP_EXECUTE);
	}
}

void CXTPControlSlider::OnThemeChanged()
{
	if (m_pSliderCtrl->GetSafeHwnd())
		m_pSliderCtrl->Invalidate(FALSE);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPControlSliderCtrl, CSliderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()

	ON_WM_HSCROLL_REFLECT()
	ON_WM_VSCROLL_REFLECT()

	ON_MESSAGE(XTP_TTM_WINDOWFROMPOINT, OnWindowFromPoint)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPControlSliderCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

HBRUSH CXTPControlSliderCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	ASSERT_VALID(pDC);

	if (pDC->m_hAttribDC == NULL)
		pDC->m_hAttribDC = pDC->m_hDC;

	CXTPWindowRect rcOrig(this);
	GetParent()->ScreenToClient(&rcOrig);

	CPoint pt = pDC->SetViewportOrg(-rcOrig.TopLeft());
	((CXTPCommandBar*)GetParent())->DrawCommandBar(pDC, rcOrig);
	pDC->SetViewportOrg(pt);

	SendMessage(TBM_CLEARSEL, 0, 0); // minor hacking. don't ask me why
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CXTPControlSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		if (m_pControl->GetParent()->IsCustomizable()
			&& ((m_pControl->GetFlags() & xtpFlagNoMovable) == 0))
			m_pControl->OnClick();
	}
	else
		CSliderCtrl::OnLButtonDown(nFlags, point);
}

void CXTPControlSliderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		ClientToScreen(&point);
		m_pControl->GetParent()->ScreenToClient(&point);

		m_pControl->GetParent()->OnRButtonDown(nFlags, point);
		return;
	}

	CSliderCtrl::OnRButtonDown(nFlags, point);
}

void CXTPControlSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CSliderCtrl::OnMouseMove(nFlags, point);

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);
}

void CXTPControlSliderCtrl::HScroll(UINT nSBCode, UINT nPos)
{
	m_pControl->OnScroll(nSBCode, nPos);
}

void CXTPControlSliderCtrl::VScroll(UINT nSBCode, UINT nPos)
{
	m_pControl->OnScroll(nSBCode, nPos);
}

BOOL CXTPControlSliderCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPCommandBar* pCommandBar = m_pControl->GetParent();

	if (pCommandBar)
	{
		pCommandBar->FilterToolTipMessage(message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

LRESULT CXTPControlSliderCtrl::OnWindowFromPoint(WPARAM, LPARAM)
{
	return 1;
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlSlider, CXTPControl)
	DISP_PROPERTY_EX_ID(CXTPControlSlider, "Pos", 200, GetPos, SetPos, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlSlider, "Min", 201, m_nMin, OleRangeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlSlider, "Max", 202, m_nMax, OleRangeChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlSlider, CXTPControl)
	INTERFACE_PART(CXTPControlSlider, XTPDIID_CommandBarSlider, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlSlider, XTPDIID_CommandBarSlider)

void CXTPControlSlider::OleRangeChanged()
{
	SetRange(m_nMin, m_nMax);
}

#endif
