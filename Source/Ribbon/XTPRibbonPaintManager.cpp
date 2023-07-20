// XTPRibbonTheme.cpp: implementation of the CXTPRibbonPaintManager class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/Frame/XTPFramePaintManager.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonTabTheme.h"
#include "Ribbon/XTPRibbonTabColorSet.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

BOOL AFX_CDECL IsCompositeRect(CXTPCommandBar* pRibbonBar, const CRect& rc)
{
	if (!pRibbonBar || !pRibbonBar->IsRibbonBar() || !((CXTPRibbonBar*)pRibbonBar)->IsDwmEnabled())
		return FALSE;

	CRect rcCaption = ((CXTPRibbonBar*)pRibbonBar)->GetCaptionRect();

	if (((CXTPRibbonBar*)pRibbonBar)->GetRibbonPaintManager()->m_bFlatFrame)
		rcCaption.bottom += ((CXTPRibbonBar*)pRibbonBar)->GetTabsHeight();

	return CRect().IntersectRect(rcCaption, rc);
}

#ifdef _XTP_INCLUDE_RIBBON
BOOL CXTPControl::HasDwmCompositedRect() const
{
	if (!GetParent())
		return FALSE;

	if (IsCompositeRect(GetParent(), GetRect()))
		return TRUE;

	if (GetParent()->IsDwmEnabled())
		return TRUE;

	return FALSE;
}
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonPaintManager::CXTPRibbonPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	ASSERT(NULL != pPaintManager);

	m_bHotTrackingGroups = TRUE;

	m_pTabPaintManager = CreateTabPaintManager();
	m_pTabPaintManager->RefreshMetrics();

	m_hApplicationIcon		= 0;
	m_bAlphaApplicationIcon = FALSE;

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableTypeLib();
	EnableAutomation();
#endif
}

IMPLEMENT_DYNAMIC(CXTPRibbonPaintManager, CXTPCmdTarget)

CXTPRibbonPaintManager::~CXTPRibbonPaintManager()
{
	CMDTARGET_RELEASE(m_pTabPaintManager);
}

CXTPResourceImage* CXTPRibbonPaintManager::LoadImage(LPCTSTR lpszFileName)
{
	return GetImages()->LoadFile(lpszFileName);
}

CXTPResourceImages* CXTPRibbonPaintManager::GetImages() const
{
	return XTPResourceImages();
}

int CXTPRibbonPaintManager::GetFontHeight() const
{
	return m_pPaintManager->m_nFontHeight;
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonPaintManager

int CXTPRibbonPaintManager::GetEditHeight() const
{
	int nEditHeight = m_pPaintManager->GetEditHeight();
	return max(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT, nEditHeight);
}

void CXTPRibbonPaintManager::SetFontHeight(int nFontHeight)
{
	m_pPaintManager->SetFontHeight(nFontHeight);
}

void CXTPRibbonPaintManager::RefreshMetrics()
{
	CXTPNonClientMetrics ncm;

	int nBaseHeight = abs(ncm.lfMenuFont.lfHeight);
	int nFontHeight = GetFontHeight();

	if (XTP_COMMANDBAR_MIN_FONT_HEIGHT < nFontHeight)
		nBaseHeight = nFontHeight;

	if (nBaseHeight < XTP_COMMANDBAR_MIN_FONT_HEIGHT)
		nBaseHeight = XTP_COMMANDBAR_MIN_FONT_HEIGHT;

	LOGFONT lf;
	m_pPaintManager->GetRegularFont()->GetLogFont(&lf);
	m_fontGroupCaption.SetStandardFont(&lf);

	m_nTabsHeight = m_pPaintManager->GetTabsHeight();

	m_rcFileButtonMargin.SetRect(0, 0, 0, 0);

	m_nGroupCaptionHeight = XTP_DPI_Y(1) * 2; // top and bottom margin
	CDC screenDC;
	if (screenDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL))
	{
		CXTPFontDC fontDC(&screenDC, m_pPaintManager->GetRegularFont());
		m_nGroupCaptionHeight += screenDC.GetTextExtent(_T("#")).cy;
	}
	else
	{
		LOGFONT fntRegular;
		m_pPaintManager->GetRegularFont()->GetLogFont(&fntRegular);
		m_nGroupCaptionHeight += abs(fntRegular.lfHeight);
	}

	m_pTabPaintManager->RefreshMetrics();
	m_pTabPaintManager->SetFontIndirect(&lf, TRUE);
}

CXTPTabPaintManager* CXTPRibbonPaintManager::CreateTabPaintManager()
{
	CXTPTabPaintManager* pTabPaintManager = new CXTPTabPaintManager();
	pTabPaintManager->SetAppearanceSet(new CXTPRibbonTabTheme(this));
	pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);

	pTabPaintManager->SetColorSet(new CXTPRibbonTabColorSet);
	pTabPaintManager->m_bFillBackground   = FALSE;
	pTabPaintManager->m_bHotTracking	  = TRUE;
	pTabPaintManager->m_clientFrame		  = xtpTabFrameSingleLine;
	pTabPaintManager->m_bDrawTextNoPrefix = FALSE;
	pTabPaintManager->m_rcButtonMargin.SetRect(0, XTP_DPI_Y(2), 0, XTP_DPI_Y(2));
	pTabPaintManager->m_rcButtonTextPadding.SetRect(XTP_DPI_X(4), 0, XTP_DPI_X(4), 0);
	pTabPaintManager->m_nButtonExtraLength   = XTP_DPI_X(21);
	pTabPaintManager->m_bDrawTextEndEllipsis = FALSE;
	pTabPaintManager->m_bDrawFocusRect		 = FALSE;
	pTabPaintManager->EnableToolTips(xtpTabToolTipShrinkedOnly);
	pTabPaintManager->m_bClearTypeTextQuality = m_pPaintManager->m_bClearTypeTextQuality;
	pTabPaintManager->m_bClipHeader			  = FALSE;

	return pTabPaintManager;
}

void CXTPRibbonPaintManager::FillTabPopupToolBar(CDC* pDC, CXTPPopupToolBar* pPopupToolBar)
{
	CXTPClientRect rcRibbonClient(pPopupToolBar);
	pDC->FillSolidRect(rcRibbonClient, m_clrRibbonFace);
}

#define XTP_DTT_GLOWSIZE (1UL << 11)   // iGlowSize has been specified
#define XTP_DTT_COMPOSITED (1UL << 13) // Draws text with antialiased alpha (needs a DIB section)

void CXTPRibbonPaintManager::DrawDwmCaptionText(CDC* pDC, CRect rcCaptionText,
												CString strWindowText, CWnd* pSite, BOOL bActive)
{
	if ((pSite->GetStyle() & WS_MAXIMIZE) && !XTPSystemVersion()->IsWin7OrGreater())
	{
		pDC->SetTextColor(0xFFFFFF);
		pDC->DrawText(strWindowText, rcCaptionText,
					  DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE | DT_NOPREFIX);
		return;
	}

	CXTPWinThemeWrapper wrapper(FALSE);
	wrapper.OpenThemeData(NULL, L"WINDOW");

	if (!wrapper.IsAppThemeActive())
	{
		pDC->SetTextColor(0);
		pDC->DrawText(strWindowText, rcCaptionText,
					  DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_LEFT);
		return;
	}

	CRect rcBuffer(rcCaptionText);
	rcBuffer.InflateRect(5, 0);

	HDC hDC			= 0;
	HPAINTBUFFER pb = wrapper.BeginBufferedPaint(pDC->GetSafeHdc(), rcBuffer, XTP_BPBF_TOPDOWNDIB,
												 0, &hDC);

	if (hDC != NULL)
	{
		::BitBlt(hDC, rcBuffer.left, rcBuffer.top, rcBuffer.Width(), rcBuffer.Height(),
				 pDC->GetSafeHdc(), rcBuffer.left, rcBuffer.top, SRCCOPY);
		::SetTextColor(hDC, 0);
		XTP_UX_DTTOPTS op;
		op.dwSize	= sizeof(op);
		op.dwFlags   = XTP_DTT_COMPOSITED | XTP_DTT_GLOWSIZE;
		op.iGlowSize = XTP_DPI_X(8);
		CXTPFontDC font(CDC::FromHandle(hDC),
						&m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption);

		wrapper.DrawThemeTextEx(hDC, pSite->GetStyle() & WS_MAXIMIZE ? WP_MAXCAPTION : WP_CAPTION,
								bActive ? FS_ACTIVE : FS_INACTIVE, XTP_CT2CW(strWindowText), -1,
								DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE
									| DT_NOPREFIX,
								rcCaptionText, &op);
	}

	wrapper.EndBufferedPaint(pb, TRUE);
}

void CXTPRibbonPaintManager::DrawRibbonFramePart(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pRibbonBar);
}

void CXTPRibbonPaintManager::DrawRibbonGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* pRibbonGroup)
{
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRibbonGroup);

	CXTPRibbonBar* pRibbonBar = pRibbonGroup->GetRibbonBar();
	FillGroupPopupToolBar(pDC, pRibbonGroup, pRibbonBar);
	CRect rcGroup = pRibbonGroup->GetRect();
	pRibbonGroup->Draw(pDC, rcGroup);
}

void CXTPRibbonPaintManager::DrawRibbonApplicationIcon(CDC* pDC, CXTPRibbonBar* pRibbonBar,
													   CRect rc, HICON hIcon)
{
	if (pRibbonBar->IsDwmEnabled())
	{
		if (m_hApplicationIcon != hIcon)
		{
			m_hApplicationIcon		= hIcon;
			m_bAlphaApplicationIcon = CXTPImageManagerIcon::IsAlphaIconHandle(hIcon);
		}

		if (m_bAlphaApplicationIcon)
		{
			DrawIconEx(pDC->GetSafeHdc(), rc.left, rc.top, hIcon, rc.Width(), rc.Height(), 0, NULL,
					   DI_NORMAL);
		}
		else
		{
			CXTPImageManagerIcon::DrawIconComposited(pDC, rc.TopLeft(), rc.Size(), hIcon);
		}
	}
	else
	{
		DrawIconEx(pDC->GetSafeHdc(), rc.left, rc.top, hIcon, rc.Width(), rc.Height(), 0, NULL,
				   DI_NORMAL);
	}
}

void CXTPRibbonPaintManager::DrawQuickAccessControl(CDC* pDC, CXTPRibbonBar* pRibbon,
													CXTPControl* pControl)
{
	UNREFERENCED_PARAMETER(pRibbon);
	ASSERT_VALID(pControl);
	pControl->Draw(pDC);
}

//////////////////////////////////////////////////////////////////////////
//

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonPaintManager, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPRibbonPaintManager, "CaptionFont", DISPID_FONT, OleGetCaptionFont,
						OleSetCaptionFont, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPRibbonPaintManager, "HotTrackingGroups", 100, m_bHotTrackingGroups,
					 VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPRibbonPaintManager, "WindowCaptionFont", 101, OleGetTitleFont,
						OleSetTitleFont, VT_DISPATCH)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonPaintManager, XTPDIID_RibbonPaintManager)

BEGIN_INTERFACE_MAP(CXTPRibbonPaintManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPRibbonPaintManager, XTPDIID_RibbonPaintManager, Dispatch)
END_INTERFACE_MAP()

LPFONTDISP CXTPRibbonPaintManager::OleGetCaptionFont()
{
	return AxCreateOleFont(&m_fontGroupCaption, this,
						   (LPFNFONTCHANGED)&CXTPRibbonPaintManager::OleSetCaptionFont);
}

void CXTPRibbonPaintManager::OleSetCaptionFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_fontGroupCaption.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_fontGroupCaption.SetCustomFont(&lf);
	}
	else
	{
		m_fontGroupCaption.SetCustomFont(LPLOGFONT(NULL));
		m_pPaintManager->RefreshMetrics();
	}
}

LPFONTDISP CXTPRibbonPaintManager::OleGetTitleFont()
{
	return AxCreateOleFont(&m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption, this,
						   (LPFNFONTCHANGED)&CXTPRibbonPaintManager::OleSetTitleFont);
}

void CXTPRibbonPaintManager::OleSetTitleFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption.SetCustomFont(&lf);
	}
	else
	{
		m_pPaintManager->GetFramePaintManager()->m_fntFrameCaption.SetCustomFont(LPLOGFONT(NULL));
		m_pPaintManager->RefreshMetrics();
	}
}

#endif
