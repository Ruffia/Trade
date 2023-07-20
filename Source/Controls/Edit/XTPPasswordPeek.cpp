// XTPPasswordPeek.cpp : implementation of the CXTPPasswordPeek class.
//
// (c)1998-2021 Codejock Software, All Rights Reserved.
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

#include "Controls/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemMetrics.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/Edit/XTPEditTheme.h"
#include "Controls/Edit/XTPEdit.h"
#include "Controls/Edit/XTPMaskEdit.h"
#include "Controls/Edit/XTPPasswordPeek.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPasswordPeek
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTPPasswordPeek, CXTPEdit)

CXTPPasswordPeek::CXTPPasswordPeek()
	: m_nButtonWidth(XTP_DPI_X(20))
	, m_bIsButtonPressed(FALSE)
	, m_bIsButtonHighlighted(FALSE)
	, m_bIsButtonCaptured(FALSE)
	, m_sizeImage(XTP_DPI_X(16), XTP_DPI_Y(16))
	, m_Mode(PasswordPeekModeDisabled)
	, m_pImageManager(NULL)
{
	m_pImageManager = new CXTPImageManager();
}

CXTPPasswordPeek::~CXTPPasswordPeek()
{
	SAFE_DELETE(m_pImageManager);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPasswordPeek, CXTPEdit)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST_EX()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPasswordPeek::PeekButtonEnable(BOOL bEnable)
{
	m_Mode = bEnable ? PasswordPeekModeEnabled : PasswordPeekModeDisabled;

	SetPasswordChar(bEnable ? _T('*') : _T('\0'));

	InitIcons();

	OnChangeLayout();
}

BOOL CXTPPasswordPeek::IsPeekButtonEnabled()
{
	return m_Mode == PasswordPeekModeEnabled ? TRUE : FALSE;
}

void CXTPPasswordPeek::InitIcons()
{
	LPCTSTR type(_T("RT_XAML"));
	HMODULE hRes = XTPResourceManager()->GetResourceHandle();
	UINT cxSmall = XTPToUIntChecked(XTPSystemMetrics()->m_nSmallIconX);
	UINT cxLarge = XTPToUIntChecked(XTPSystemMetrics()->m_nIconX);

	m_pImageManager->RemoveAll();

	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_SMALL_NORMAL, 0, cxSmall);
	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_SMALL_HIGHLIGHTED, 1,
								   cxSmall);
	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_SMALL_PRESSED, 2,
								   cxSmall);
	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_LARGE_NORMAL, 0, cxLarge);
	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_LARGE_HIGHLIGHTED, 1,
								   cxLarge);
	m_pImageManager->SetVectorIcon(hRes, type, IDR_XAML_ICON_PASSWORDPEEK_LARGE_PRESSED, 2,
								   cxLarge);
}

void CXTPPasswordPeek::OnChangeLayout()
{
	m_nButtonWidth = max(XTP_DPI_X(20), m_sizeImage.cx + XTP_DPI_X(8));

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);

	if (m_Mode == PasswordPeekModeEnabled)
	{
		GetWindowRect(m_rectBtn);

		if (GetExStyle() & WS_EX_RIGHT)
			m_rectBtn.right = m_rectBtn.left + m_nButtonWidth;
		else
			m_rectBtn.left = m_rectBtn.right - m_nButtonWidth;

		ScreenToClient(&m_rectBtn);
	}
	else
	{
		m_rectBtn.SetRectEmpty();
	}
}

void CXTPPasswordPeek::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	CXTPEdit::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (m_Mode == PasswordPeekModeEnabled)
	{
		if (GetExStyle() & WS_EX_RIGHT)
			lpncsp->rgrc[0].left += m_nButtonWidth;
		else
			lpncsp->rgrc[0].right -= m_nButtonWidth;
	}
}

void CXTPPasswordPeek::OnNcPaint()
{
	CXTPEdit::OnNcPaint();

	if (m_Mode == PasswordPeekModeDisabled)
	{
		return;
	}

	CWindowDC dc(this);

	CRect rectWindow;
	GetWindowRect(rectWindow);

	m_rectBtn = rectWindow;

	if (GetExStyle() & WS_EX_RIGHT)
		m_rectBtn.right = m_rectBtn.left + m_nButtonWidth;
	else
		m_rectBtn.left = m_rectBtn.right - m_nButtonWidth;

	CRect rectClient;
	GetClientRect(rectClient);
	ClientToScreen(&rectClient);

	if (GetExStyle() & WS_EX_RIGHT)
		m_rectBtn.OffsetRect(rectClient.left - m_nButtonWidth - rectWindow.left, 0);
	else
		m_rectBtn.OffsetRect(rectClient.right + m_nButtonWidth - rectWindow.right, 0);

	m_rectBtn.top += rectClient.top - rectWindow.top;
	m_rectBtn.bottom -= rectWindow.bottom - rectClient.bottom;

	CRect rect = m_rectBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);

	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect(&rect);

	dc.SelectClipRgn(&rgnClip);

	OnDrawButton(&dc, rect, m_bIsButtonPressed, m_bIsButtonHighlighted);

	dc.SelectClipRgn(NULL);

	ScreenToClient(&m_rectBtn);
}

void CXTPPasswordPeek::OnDrawButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot)
{
	CXTPEditTheme* pTheme = GetTheme();
	pDC->FillSolidRect(rect, pTheme->m_clrBack);

	CPoint ptStart(rect.CenterPoint());
	ptStart.Offset(-m_sizeImage.cx / 2, -m_sizeImage.cy / 2);

	CXTPImageManagerIcon* pIcon = m_pImageManager->GetImage(
		UINT(bIsButtonPressed ? 2 : (bIsButtonHot ? 1 : 0)), m_sizeImage.cx);
	XTP_SAFE_CALL1(pIcon, Draw(pDC, ptStart, m_sizeImage));
}

LRESULT CXTPPasswordPeek::OnNcHitTest(CPoint point)
{
	CPoint ptClient = point;
	ScreenToClient(&ptClient);

	if (m_Mode == PasswordPeekModeEnabled && m_rectBtn.PtInRect(ptClient))
	{
		return HTCAPTION;
	}

	return CXTPEdit::OnNcHitTest(point);
}

void CXTPPasswordPeek::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if (!m_bIsButtonCaptured)
	{
		CPoint ptClient = point;
		ScreenToClient(&ptClient);

		if (m_rectBtn.PtInRect(ptClient))
		{
			SetCapture();
			m_bIsButtonHighlighted = TRUE;

			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}

	CXTPEdit::OnNcMouseMove(nHitTest, point);
}

void CXTPPasswordPeek::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_Mode == PasswordPeekModeEnabled && m_rectBtn.PtInRect(point))
	{
		return;
	}

	CXTPEdit::OnRButtonDown(nFlags, point);
}

void CXTPPasswordPeek::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_Mode == PasswordPeekModeEnabled && m_rectBtn.PtInRect(point))
	{
		return;
	}

	CXTPEdit::OnRButtonUp(nFlags, point);
}

void CXTPPasswordPeek::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Mode == PasswordPeekModeEnabled && m_rectBtn.PtInRect(point))
	{
		SetFocus();

		m_bIsButtonPressed  = TRUE;
		m_bIsButtonCaptured = TRUE;

		SetCapture();

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		SetPasswordChar(0);

		return;
	}

	CXTPEdit::OnLButtonDown(nFlags, point);
}

void CXTPPasswordPeek::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsButtonCaptured)
	{
		ReleaseCapture();

		m_bIsButtonPressed	 = FALSE;
		m_bIsButtonCaptured	= FALSE;
		m_bIsButtonHighlighted = FALSE;

		RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if (m_rectBtn.PtInRect(point))
		{
			SetPasswordChar(_T('*'));
		}

		return;
	}

	CXTPEdit::OnLButtonUp(nFlags, point);
}

void CXTPPasswordPeek::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsButtonCaptured)
	{
		BOOL bIsButtonPressed = m_rectBtn.PtInRect(point);
		if (bIsButtonPressed != m_bIsButtonPressed)
		{
			m_bIsButtonPressed = bIsButtonPressed;
			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

			if (m_rectBtn.PtInRect(point))
				SetPasswordChar(0);
			else
				SetPasswordChar(_T('*'));
		}

		return;
	}

	if (m_bIsButtonHighlighted)
	{
		if (!m_rectBtn.PtInRect(point))
		{
			m_bIsButtonHighlighted = FALSE;
			ReleaseCapture();

			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}

	CXTPEdit::OnMouseMove(nFlags, point);
}

void CXTPPasswordPeek::OnCancelMode()
{
	CXTPEdit::OnCancelMode();

	if (IsWindowEnabled())
	{
		ReleaseCapture();
	}

	m_bIsButtonPressed	 = FALSE;
	m_bIsButtonCaptured	= FALSE;
	m_bIsButtonHighlighted = FALSE;

	RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
}
