// XTPTabClientDraggingContents.cpp : implementation of the CXTPTabClientDraggingContents class.
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

#include "CommandBars/Resource.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPResource.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"
#include "Common/Hook/XTPWinEventHook.h"
#include "Common/FrameShadow/XTPFrameShadowManager.h"

#include "Markup/XTPMarkupObject.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Edit/XTPEdit.h"
#endif

#include "TabManager/Includes.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2010.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2012.h"
#include "TabManager/Themes/XTPTabThemeOffice2013.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2015.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2017.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2019.h"
#include "TabManager/Themes/XTPTabThemeVisualStudio2022.h"
#include "TabManager/Themes/XTPTabThemeNativeWindows10.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPShortcutManager.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPFrameWnd.h"
#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/Frame/XTPFrameHook.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/SingleWorkspace.h"
#include "CommandBars/TabClient/TabClientDropTarget.h"
#include "CommandBars/TabClient/NavigateButtonActiveFiles.h"
#include "CommandBars/TabClient/XTPTearOffFrame.h"
#include "CommandBars/TabClient/XTPTabClientDraggingContents.h"

#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

IMPLEMENT_DYNCREATE(CXTPTabClientDraggingContents, CWnd)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTabClientDraggingContents, CWnd)
	//{{AFX_MSG_MAP(CXTPTabClientDraggingContents)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPTabClientDraggingContents::CXTPTabClientDraggingContents()
{
	m_pTarget   = NULL;
	m_Width		= 0;
	m_Height	= 0;
	m_imgHeight = 0;
	m_imgWidth  = 0;
	m_borderX   = XTP_DPI_X(1);
	m_borderY   = XTP_DPI_Y(1);
}

CXTPTabClientDraggingContents::CXTPTabClientDraggingContents(CXTPTabClientWnd* pTabClientWnd)
{
	m_pTarget   = pTabClientWnd;
	m_Width		= 0;
	m_Height	= 0;
	m_imgHeight = 0;
	m_imgWidth  = 0;
	m_borderX   = XTP_DPI_X(1);
	m_borderY   = XTP_DPI_Y(1);
}

CXTPTabClientDraggingContents::~CXTPTabClientDraggingContents()
{
}

BOOL CXTPTabClientDraggingContents::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTPTabClientDraggingContents::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));

	dc.FillSolidRect(0, 0, m_Width, m_Height,
					 m_pTarget->GetPaintManager()->GetColorSet()->m_clrHeaderFace);

	CDC hDCMem;
	hDCMem.CreateCompatibleDC(&dc);
	hDCMem.SelectObject(&m_bitmap);
	dc.BitBlt(m_borderX, m_borderY, m_imgWidth, m_imgHeight, &hDCMem, 0, 0, SRCCOPY);
}

BOOL CXTPTabClientDraggingContents::Create()
{
	CRect rcControl = m_pTarget->GetSelectedItem()->GetTabManager()->GetControlRect();
	CRect rcHeader  = m_pTarget->GetSelectedItem()->GetTabManager()->GetHeaderRect();
	// CRect rcClient  = m_pTarget->GetSelectedItem()->GetTabManager()->GetClientRect();
	m_imgHeight = rcControl.Height();
	m_imgWidth  = rcControl.Width();

	CRect rcMargin = m_pTarget->GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin;
	CRect rcTab	= m_pTarget->GetSelectedItem()->GetRect();
	CDC* pDC	   = m_pTarget->GetDC();

	CDC hDCMem;
	if (hDCMem.CreateCompatibleDC(pDC))
	{
		if (m_bitmap.CreateCompatibleBitmap(pDC, m_imgWidth, m_imgHeight))
		{
			HBITMAP hBmpOld = (HBITMAP)hDCMem.SelectObject(m_bitmap);

			hDCMem.BitBlt(0, 0, rcControl.Width(), rcControl.Height(), pDC, rcControl.left,
						  rcControl.top, SRCCOPY);

			COLORREF clr = m_pTarget->GetPaintManager()->GetColorSet()->m_clrHeaderFace;
			switch (m_pTarget->GetPaintManager()->GetPosition())
			{
				case xtpTabPositionTop:
					hDCMem.FillSolidRect(0, 0, rcControl.Width(), rcHeader.Height(), clr);
					hDCMem.BitBlt(rcMargin.left, rcMargin.top, rcTab.Width(), rcTab.Height(), pDC,
								  rcTab.left, rcTab.top, SRCCOPY);
					break;

				case xtpTabPositionBottom:
					hDCMem.FillSolidRect(0, rcControl.Height() - rcHeader.Height(),
										 rcControl.Width(), rcHeader.Height(), clr);
					hDCMem.BitBlt(rcMargin.left, rcControl.Height() - rcTab.Height() - rcMargin.top,
								  rcTab.Width(), rcTab.Height(), pDC, rcTab.left, rcTab.top,
								  SRCCOPY);
					break;

				case xtpTabPositionLeft:
					hDCMem.FillSolidRect(0, 0, rcHeader.Width(), rcControl.Height(), clr);
					hDCMem.BitBlt(rcMargin.left, rcMargin.top, rcTab.Width(), rcTab.Height(), pDC,
								  rcTab.left, rcTab.top, SRCCOPY);
					break;

				case xtpTabPositionRight:
					hDCMem.FillSolidRect(rcControl.Width() - rcHeader.Width(), 0, rcHeader.Width(),
										 rcControl.Height(), clr);
					hDCMem.BitBlt(rcControl.Width() - rcHeader.Width(), rcMargin.top, rcTab.Width(),
								  rcTab.Height(), pDC, rcTab.left, rcTab.top, SRCCOPY);
					break;

				default: break;
			}

			hDCMem.SelectObject(hBmpOld);
		}
	}

	m_Width  = rcControl.Width() + 2 * m_borderX;
	m_Height = rcControl.Height() + 2 * m_borderY;

	BOOL res = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
						AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL,
						WS_POPUP, 0, 0, m_Width, m_Height, NULL, NULL, NULL);

	ShowWindow(SW_SHOW);

	return res;
}
