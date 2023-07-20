// XTPTearOffFrame.cpp : implementation of the CXTPTearOffFrame class.
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

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/XTPDockingPaneDefines.h"
#	endif
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

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/Includes.h"
#		include "DockingPane/XTPDockingPaneManager.h"
#	endif
#endif

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
#include "CommandBars/TabClient/XTPTabClientSticker.h"

#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//*********************************************************
//
//		CXTPStickerBase implementation
//
//*********************************************************

IMPLEMENT_DYNAMIC(CXTPStickerBase, CWnd);

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPStickerBase, CWnd)
	//{{AFX_MSG_MAP(CXTPStickerBase)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPStickerBase::CXTPStickerBase()
{
	m_pTarget		 = NULL;
	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);
	ASSERT(m_pMarkupContext);
}

CXTPStickerBase::~CXTPStickerBase()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
}

BOOL CXTPStickerBase::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPStickerBase::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));
	OnDraw(dc);
}

void CXTPStickerBase::IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2)
{
	if (x1 < x2)
	{
		CRgn rgnExclude;
		rgnExclude.CreateRectRgn(x1, y, x2, y + 1);
		pRgn->CombineRgn(pRgn, &rgnExclude, RGN_OR);
		rgnExclude.DeleteObject();
	}
}

void CXTPStickerBase::RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc)
{
	CSize sz = rc.Size();

	pRgn->CreateRectRgn(0, 0, 0, 0);

	for (int y = 0; y < sz.cy; y++)
	{
		int nStart = 0, x = 0;
		BOOL bTransparent = TRUE;

		while (x < sz.cx)
		{
			BOOL bTransparentPixel = pDC->GetPixel(x, y) == 0;

			if (bTransparent && !bTransparentPixel)
			{
				nStart		 = x;
				bTransparent = FALSE;
			}
			else if (!bTransparent && bTransparentPixel)
			{
				IncludeRgnPart(pRgn, nStart, y, x);
				bTransparent = TRUE;
			}
			x++;
		}
		if (!bTransparent)
		{
			IncludeRgnPart(pRgn, nStart, y, x);
		}
	}
}

BOOL CXTPStickerBase::Create()
{
	ASSERT(::IsWindow(m_pTarget->GetSafeHwnd()));

	CPoint pt = GetTargetRect().CenterPoint();

	CRect rcWnd(pt.x - m_rcSticker.Width() / 2, pt.y - m_rcSticker.Height() / 2,
				pt.x + m_rcSticker.Width() / 2, pt.y + m_rcSticker.Height() / 2);

	BOOL res = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
						AfxRegisterWndClass(NULL, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0,
						WS_POPUP, CRect(0, 0, 0, 0), m_pTarget, 0);

	CRgn pRgn;
	CClientDC dcClient(this);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dcClient, rcWnd.Width(), rcWnd.Height());
	if (bmp.GetSafeHandle())
	{
		CXTPCompatibleDC dc(&dcClient, &bmp);
		OnDraw(dc);
		RegionFromBitmap(&pRgn, &dc, rcWnd);
	}

	HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
	::CombineRgn(hRgn, (HRGN)pRgn.GetSafeHandle(), NULL, RGN_COPY);

	SetWindowRgn(hRgn, FALSE);

	m_pTarget->ClientToScreen(&rcWnd);

	SetWindowPos(0, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),
				 SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);

	return res;
}

void CXTPStickerBase::InitSize()
{
	m_szIcon	 = CSize(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_delimeterX = XTP_DPI_X(4);
	m_delimeterY = XTP_DPI_Y(4);

	m_rcSticker.SetRect(0, 0, m_szIcon.cx * 3 + m_delimeterX * 6,
						m_szIcon.cy * 3 + m_delimeterY * 6);

	CPoint ptCenter = m_rcSticker.CenterPoint();

	m_rcBack.SetRect(ptCenter.x - (m_szIcon.cx / 2 + m_szIcon.cx + m_delimeterX * 2),
					 ptCenter.y - (m_szIcon.cy / 2 + m_szIcon.cy + m_delimeterY * 2),
					 ptCenter.x + (m_szIcon.cx / 2 + m_szIcon.cx + m_delimeterX * 2),
					 ptCenter.y + (m_szIcon.cy / 2 + m_szIcon.cy + m_delimeterY * 2));

	m_rcCenter.SetRect(ptCenter.x - m_szIcon.cx / 2, ptCenter.y - m_szIcon.cy / 2,
					   ptCenter.x + m_szIcon.cx / 2, ptCenter.y + m_szIcon.cy / 2);

	m_rcLeft = m_rcCenter;
	m_rcLeft.OffsetRect(-(m_szIcon.cx + m_delimeterX), 0);

	m_rcTop = m_rcCenter;
	m_rcTop.OffsetRect(0, -(m_szIcon.cy + m_delimeterY));

	m_rcRight = m_rcCenter;
	m_rcRight.OffsetRect(m_szIcon.cx + m_delimeterX, 0);

	m_rcBottom = m_rcCenter;
	m_rcBottom.OffsetRect(0, m_szIcon.cy + m_delimeterY);

	m_stickers = InitStickers();
}

void CXTPStickerBase::InitColors()
{
	COLORREF clrBackBorderNormal			= XTPIniColor(_T("TabClient.Stickers.Xaml"),
												  _T("BackBorderNormal"), RGB(99, 104, 113));
	COLORREF clrExternalBorderNormal		= XTPIniColor(_T("TabClient.Stickers.Xaml"),
													  _T("ExternalBorderNormal"), RGB(174, 178, 186));
	COLORREF clrInternalBorderNormal		= XTPIniColor(_T("TabClient.Stickers.Xaml"),
													  _T("InternalBorderNormal"), RGB(125, 139, 162));
	COLORREF clrExternalFillGradient1Normal = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														  _T("ExternalFillGradient1Normal"),
														  RGB(248, 251, 253));
	COLORREF clrExternalFillGradient2Normal = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														  _T("ExternalFillGradient2Normal"),
														  RGB(232, 235, 240));
	COLORREF clrInternalFillGradient1Normal = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														  _T("InternalFillGradient1Normal"),
														  RGB(254, 239, 193));
	COLORREF clrInternalFillGradient2Normal = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														  _T("InternalFillGradient2Normal"),
														  RGB(250, 216, 157));
	COLORREF clrArrowBorderNormal			= XTPIniColor(_T("TabClient.Stickers.Xaml"),
												  _T("ArrowBorderNormal"), RGB(187, 195, 207));
	COLORREF clrArrowFillNormal = XTPIniColor(_T("TabClient.Stickers.Xaml"), _T("ArrowFillNormal"),
											  RGB(124, 138, 161));
	COLORREF clrBackBorderSelected			  = XTPIniColor(_T("TabClient.Stickers.Xaml"),
													_T("BackBorderSelected"), RGB(99, 104, 113));
	COLORREF clrExternalBorderSelected		  = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														_T("ExternalBorderSelected"),
														RGB(138, 145, 156));
	COLORREF clrInternalBorderSelected		  = XTPIniColor(_T("TabClient.Stickers.Xaml"),
														_T("InternalBorderSelected"),
														RGB(68, 88, 121));
	COLORREF clrExternalFillGradient1Selected = XTPIniColor(_T("TabClient.Stickers.Xaml"),
															_T("ExternalFillGradient1Selected"),
															RGB(245, 248, 251));
	COLORREF clrExternalFillGradient2Selected = XTPIniColor(_T("TabClient.Stickers.Xaml"),
															_T("ExternalFillGradient2Selected"),
															RGB(222, 226, 233));
	COLORREF clrInternalFillGradient1Selected = XTPIniColor(_T("TabClient.Stickers.Xaml"),
															_T("InternalFillGradient1Selected"),
															RGB(253, 231, 165));
	COLORREF clrInternalFillGradient2Selected = XTPIniColor(_T("TabClient.Stickers.Xaml"),
															_T("InternalFillGradient2Selected"),
															RGB(247, 198, 113));
	COLORREF clrArrowBorderSelected			  = XTPIniColor(_T("TabClient.Stickers.Xaml"),
													_T("ArrowBorderSelected"), RGB(86, 142, 192));
	COLORREF clrArrowFillSelected			  = XTPIniColor(_T("TabClient.Stickers.Xaml"),
												_T("ArrowFillSelected"), RGB(41, 101, 159));

	CXTPMarkupStaticExtension* pExtensionRoot = m_pMarkupContext->GetExtensionRoot();
	if (pExtensionRoot)
	{
		CXTPMarkupStaticExtension* pTheme  = new CXTPMarkupStaticExtension(L"Theme");
		CXTPMarkupStaticExtension* pColors = new CXTPMarkupStaticExtension(L"Colors");

		pColors->Extend(
			new CXTPMarkupStaticExtensionBrushProperty(L"BackBorderNormal", clrBackBorderNormal));
		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"ExternalBorderNormal",
																   clrExternalBorderNormal));
		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"InternalBorderNormal",
																   clrInternalBorderNormal));
		pColors->Extend(new CXTPMarkupStaticExtensionColorProperty(L"ExternalFillGradientANormal",
																   clrExternalFillGradient1Normal));
		pColors->Extend(new CXTPMarkupStaticExtensionColorProperty(L"ExternalFillGradientBNormal",
																   clrExternalFillGradient2Normal));
		pColors->Extend(new CXTPMarkupStaticExtensionColorProperty(L"InternalFillGradientANormal",
																   clrInternalFillGradient1Normal));
		pColors->Extend(new CXTPMarkupStaticExtensionColorProperty(L"InternalFillGradientBNormal",
																   clrInternalFillGradient2Normal));
		pColors->Extend(
			new CXTPMarkupStaticExtensionBrushProperty(L"ArrowBorderNormal", clrArrowBorderNormal));
		pColors->Extend(
			new CXTPMarkupStaticExtensionBrushProperty(L"ArrowFillNormal", clrArrowFillNormal));

		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"BackBorderSelected",
																   clrBackBorderSelected));
		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"ExternalBorderSelected",
																   clrExternalBorderSelected));
		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"InternalBorderSelected",
																   clrInternalBorderSelected));
		pColors->Extend(
			new CXTPMarkupStaticExtensionColorProperty(L"ExternalFillGradientASelected",
													   clrExternalFillGradient1Selected));
		pColors->Extend(
			new CXTPMarkupStaticExtensionColorProperty(L"ExternalFillGradientBSelected",
													   clrExternalFillGradient2Selected));
		pColors->Extend(
			new CXTPMarkupStaticExtensionColorProperty(L"InternalFillGradientASelected",
													   clrInternalFillGradient1Selected));
		pColors->Extend(
			new CXTPMarkupStaticExtensionColorProperty(L"InternalFillGradientBSelected",
													   clrInternalFillGradient2Selected));
		pColors->Extend(new CXTPMarkupStaticExtensionBrushProperty(L"ArrowBorderSelected",
																   clrArrowBorderSelected));
		pColors->Extend(
			new CXTPMarkupStaticExtensionBrushProperty(L"ArrowFillSelected", clrArrowFillSelected));

		pTheme->Extend(pColors);
		pExtensionRoot->Extend(pTheme);
	}

	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.BackBorderNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalBorderNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalBorderNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalFillGradientANormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalFillGradientBNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalFillGradientANormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalFillGradientBNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ArrowBorderNormal"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ArrowFillNormal"));

	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.BackBorderSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalBorderSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalBorderSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalFillGradientASelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ExternalFillGradientBSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalFillGradientASelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.InternalFillGradientBSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ArrowBorderSelected"));
	ASSERT(pExtensionRoot->FindExtension(L"Theme.Colors.ArrowFillSelected"));
}

void CXTPStickerBase::SetTarget(CWnd* pTarget)
{
	m_pTarget = pTarget;
}

CWnd* CXTPStickerBase::GetTarget() const
{
	return m_pTarget;
}

//*********************************************************
//
//		CXTPTabClientSticker implementation
//
//*********************************************************

IMPLEMENT_DYNCREATE(CXTPTabClientSticker, CXTPStickerBase)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPTabClientSticker, CXTPStickerBase)
	//{{AFX_MSG_MAP(CXTPTabClientSticker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPTabClientSticker::CXTPTabClientSticker()
{
	m_nTargetWorkspace = -1;

	InitSize();
	InitColors();
	InitIcons();
}

CXTPTabClientSticker::CXTPTabClientSticker(CXTPTabClientWnd* pTabClientWnd, int nTabWorkspace)
{
	SetTarget(pTabClientWnd);
	m_nTargetWorkspace = nTabWorkspace;

	InitSize();
	InitColors();
	InitIcons();
}

CXTPTabClientSticker::~CXTPTabClientSticker()
{
}

void CXTPTabClientSticker::InitIcons()
{
	HMODULE hModule = XTPResourceImages()->GetHandle();
	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	if (resource.LoadResource(hModule, _T("XAML_STYLE"), _T("STYLE")))
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		if (hZip)
		{
			m_partBackNormal.LoadPart(hZip, _T("Stickers/back.3x3.xaml"), m_pMarkupContext);
			m_partLeftNormal.LoadPart(hZip, _T("Stickers/tab.left.xaml"), m_pMarkupContext);
			m_partTopNormal.LoadPart(hZip, _T("Stickers/tab.top.xaml"), m_pMarkupContext);
			m_partRightNormal.LoadPart(hZip, _T("Stickers/tab.right.xaml"), m_pMarkupContext);
			m_partBottomNormal.LoadPart(hZip, _T("Stickers/tab.bottom.xaml"), m_pMarkupContext);
			m_partCenterNormal.LoadPart(hZip, _T("Stickers/center.xaml"), m_pMarkupContext);
			m_partBackSelected.LoadPart(hZip, _T("Stickers/back.3x3.selected.xaml"),
										m_pMarkupContext);
			m_partLeftSelected.LoadPart(hZip, _T("Stickers/tab.left.selected.xaml"),
										m_pMarkupContext);
			m_partTopSelected.LoadPart(hZip, _T("Stickers/tab.top.selected.xaml"),
									   m_pMarkupContext);
			m_partRightSelected.LoadPart(hZip, _T("Stickers/tab.right.selected.xaml"),
										 m_pMarkupContext);
			m_partBottomSelected.LoadPart(hZip, _T("Stickers/tab.bottom.selected.xaml"),
										  m_pMarkupContext);
			m_partCenterSelected.LoadPart(hZip, _T("Stickers/center.selected.xaml"),
										  m_pMarkupContext);

			CloseZip(hZip);
		}
	}
}

int CXTPTabClientSticker::InitStickers()
{
	int value = xtpTabClientStickerNone;

	CXTPTabClientWnd* pTarget = GetTargetTabClientWnd();
	if (!pTarget)
		return value;

	if (!pTarget->IsGroupingEnabled())
	{
		value |= xtpTabClientStickerCenter;
		return value;
	}

	int count = pTarget->GetWorkspaceCount();
	if (count == 0)
	{
		value |= xtpTabClientStickerCenter;
	}
	else if (count == 1)
	{
		value |= xtpTabClientStickerCenter;
		value |= xtpTabClientStickerLeft;
		value |= xtpTabClientStickerTop;
		value |= xtpTabClientStickerRight;
		value |= xtpTabClientStickerBottom;
	}
	else if (count > 1 && pTarget->m_bHorizSplitting)
	{
		value |= xtpTabClientStickerCenter;
		value |= xtpTabClientStickerTop;
		value |= xtpTabClientStickerBottom;
	}
	else if (count > 1 && !pTarget->m_bHorizSplitting)
	{
		value |= xtpTabClientStickerCenter;
		value |= xtpTabClientStickerLeft;
		value |= xtpTabClientStickerRight;
	}

	return value;
}

CXTPTabClientWnd* CXTPTabClientSticker::GetTargetTabClientWnd() const
{
	CXTPTabClientWnd* pTarget = DYNAMIC_DOWNCAST(CXTPTabClientWnd, m_pTarget);

	if (::IsWindow(pTarget->GetSafeHwnd()))
		return pTarget;

	return NULL;
}

CRect CXTPTabClientSticker::GetTargetRect()
{
	CXTPTabClientWnd* pTarget = GetTargetTabClientWnd();
	ASSERT(pTarget);

	CRect rcWorkspace;
	if (m_nTargetWorkspace == -1)
		pTarget->GetClientRect(&rcWorkspace);
	else
		rcWorkspace = pTarget->GetWorkspace(m_nTargetWorkspace)->GetControlRect();

	return rcWorkspace;
}

void CXTPTabClientSticker::OnDraw(CDC& dc)
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

	BOOL bSelectedAnySticker = FALSE;

	if (m_stickers & xtpTabClientStickerCenter)
	{
		BOOL bCenter = m_rcCenter.PtInRect(pt);
		if (bCenter)
		{
			bSelectedAnySticker = TRUE;
			m_partCenterSelected.RenderMarkup(&dc, m_rcCenter);
		}
		else
			m_partCenterNormal.RenderMarkup(&dc, m_rcCenter);
	}

	if (m_stickers & xtpTabClientStickerLeft)
	{
		BOOL bLeft = m_rcLeft.PtInRect(pt);
		if (bLeft)
		{
			bSelectedAnySticker = TRUE;
			m_partLeftSelected.RenderMarkup(&dc, m_rcLeft);
		}
		else
			m_partLeftNormal.RenderMarkup(&dc, m_rcLeft);
	}

	if (m_stickers & xtpTabClientStickerTop)
	{
		BOOL bTop = m_rcTop.PtInRect(pt);
		if (bTop)
		{
			bSelectedAnySticker = TRUE;
			m_partTopSelected.RenderMarkup(&dc, m_rcTop);
		}
		else
			m_partTopNormal.RenderMarkup(&dc, m_rcTop);
	}

	if (m_stickers & xtpTabClientStickerRight)
	{
		BOOL bRight = m_rcRight.PtInRect(pt);
		if (bRight)
		{
			bSelectedAnySticker = TRUE;
			m_partRightSelected.RenderMarkup(&dc, m_rcRight);
		}
		else
			m_partRightNormal.RenderMarkup(&dc, m_rcRight);
	}

	if (m_stickers & xtpTabClientStickerBottom)
	{
		BOOL bBottom = m_rcBottom.PtInRect(pt);
		if (bBottom)
		{
			bSelectedAnySticker = TRUE;
			m_partBottomSelected.RenderMarkup(&dc, m_rcBottom);
		}
		else
			m_partBottomNormal.RenderMarkup(&dc, m_rcBottom);
	}

	CSize szBack		 = XTP_DPI_SIZE(CSize(112, 112));
	CPoint ptStickerBack = m_rcSticker.CenterPoint();
	ptStickerBack.x -= szBack.cx / 2;
	ptStickerBack.y -= szBack.cy / 2;

	if (bSelectedAnySticker)
		m_partBackSelected.RenderMarkup(&dc, m_rcBack);
	else
		m_partBackNormal.RenderMarkup(&dc, m_rcBack);
}

XTPTabClientStickerPart CXTPTabClientSticker::HitTest(POINT pt) const
{
	ScreenToClient(&pt);

	if (m_stickers & xtpTabClientStickerCenter && m_rcCenter.PtInRect(pt))
		return xtpTabClientStickerCenter;
	else if (m_stickers & xtpTabClientStickerLeft && m_rcLeft.PtInRect(pt))
		return xtpTabClientStickerLeft;
	else if (m_stickers & xtpTabClientStickerTop && m_rcTop.PtInRect(pt))
		return xtpTabClientStickerTop;
	else if (m_stickers & xtpTabClientStickerRight && m_rcRight.PtInRect(pt))
		return xtpTabClientStickerRight;
	else if (m_stickers & xtpTabClientStickerBottom && m_rcBottom.PtInRect(pt))
		return xtpTabClientStickerBottom;
	else
		return xtpTabClientStickerNone;
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
//*********************************************************
//
//		CXTPDockPaneSticker implementation
//
//*********************************************************

IMPLEMENT_DYNCREATE(CXTPDockPaneSticker, CXTPStickerBase)

#		include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPDockPaneSticker, CXTPStickerBase)
	//{{AFX_MSG_MAP(CXTPDockPaneSticker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#		include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPDockPaneSticker::CXTPDockPaneSticker()
{
	InitSize();
	InitColors();
	InitIcons();
}

CXTPDockPaneSticker::CXTPDockPaneSticker(CXTPDockingPaneTabbedContainer* pDockPane)
{
	SetTarget(pDockPane);

	InitSize();
	InitColors();
	InitIcons();
}

CXTPDockPaneSticker::~CXTPDockPaneSticker()
{
}

void CXTPDockPaneSticker::InitIcons()
{
	HMODULE hModule = XTPResourceImages()->GetHandle();
	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	if (resource.LoadResource(hModule, _T("XAML_STYLE"), _T("STYLE")))
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		if (hZip)
		{
			m_partBackNormal.LoadPart(hZip, _T("Stickers/back.3x3.xaml"), m_pMarkupContext);
			m_partLeftNormal.LoadPart(hZip, _T("Stickers/dock.left.xaml"), m_pMarkupContext);
			m_partTopNormal.LoadPart(hZip, _T("Stickers/dock.top.xaml"), m_pMarkupContext);
			m_partRightNormal.LoadPart(hZip, _T("Stickers/dock.right.xaml"), m_pMarkupContext);
			m_partBottomNormal.LoadPart(hZip, _T("Stickers/dock.bottom.xaml"), m_pMarkupContext);
			m_partCenterNormal.LoadPart(hZip, _T("Stickers/center.xaml"), m_pMarkupContext);
			m_partBackSelected.LoadPart(hZip, _T("Stickers/back.3x3.selected.xaml"),
										m_pMarkupContext);
			m_partLeftSelected.LoadPart(hZip, _T("Stickers/dock.left.selected.xaml"),
										m_pMarkupContext);
			m_partTopSelected.LoadPart(hZip, _T("Stickers/dock.top.selected.xaml"),
									   m_pMarkupContext);
			m_partRightSelected.LoadPart(hZip, _T("Stickers/dock.right.selected.xaml"),
										 m_pMarkupContext);
			m_partBottomSelected.LoadPart(hZip, _T("Stickers/dock.bottom.selected.xaml"),
										  m_pMarkupContext);
			m_partCenterSelected.LoadPart(hZip, _T("Stickers/center.selected.xaml"),
										  m_pMarkupContext);

			CloseZip(hZip);
		}
	}
}

int CXTPDockPaneSticker::InitStickers()
{
	int value = xtpPaneStickerNone;

	CXTPDockingPaneTabbedContainer* pTarget = GetTargetDockPaneWnd();
	if (!pTarget)
		return value;

	value |= xtpPaneStickerCenter;
	value |= xtpPaneStickerLeft;
	value |= xtpPaneStickerTop;
	value |= xtpPaneStickerRight;
	value |= xtpPaneStickerBottom;

	return value;
}

CXTPDockingPaneTabbedContainer* CXTPDockPaneSticker::GetTargetDockPaneWnd() const
{
	CXTPDockingPaneTabbedContainer* pTarget = DYNAMIC_DOWNCAST(CXTPDockingPaneTabbedContainer,
															   m_pTarget);
	if (NULL != pTarget && ::IsWindow(pTarget->GetSafeHwnd()))
		return pTarget;

	return NULL;
}

CRect CXTPDockPaneSticker::GetTargetRect()
{
	CXTPDockingPaneTabbedContainer* pTarget = GetTargetDockPaneWnd();

	if (!pTarget)
		return CRect();

	CRect rc;
	CWnd* pTargetWnd = (CWnd*)pTarget;
	pTargetWnd->GetClientRect(&rc);

	return rc;
}

void CXTPDockPaneSticker::OnDraw(CDC& dc)
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

	BOOL bSelectedAnySticker = FALSE;

	if (m_stickers & xtpPaneStickerCenter)
	{
		BOOL bCenter = m_rcCenter.PtInRect(pt);
		if (bCenter)
		{
			bSelectedAnySticker = TRUE;
			m_partCenterSelected.RenderMarkup(&dc, m_rcCenter);
		}
		else
			m_partCenterNormal.RenderMarkup(&dc, m_rcCenter);
	}

	if (m_stickers & xtpPaneStickerLeft)
	{
		BOOL bLeft = m_rcLeft.PtInRect(pt);
		if (bLeft)
		{
			bSelectedAnySticker = TRUE;
			m_partLeftSelected.RenderMarkup(&dc, m_rcLeft);
		}
		else
			m_partLeftNormal.RenderMarkup(&dc, m_rcLeft);
	}

	if (m_stickers & xtpPaneStickerTop)
	{
		BOOL bTop = m_rcTop.PtInRect(pt);
		if (bTop)
		{
			bSelectedAnySticker = TRUE;
			m_partTopSelected.RenderMarkup(&dc, m_rcTop);
		}
		else
			m_partTopNormal.RenderMarkup(&dc, m_rcTop);
	}

	if (m_stickers & xtpPaneStickerRight)
	{
		BOOL bRight = m_rcRight.PtInRect(pt);
		if (bRight)
		{
			bSelectedAnySticker = TRUE;
			m_partRightSelected.RenderMarkup(&dc, m_rcRight);
		}
		else
			m_partRightNormal.RenderMarkup(&dc, m_rcRight);
	}

	if (m_stickers & xtpPaneStickerBottom)
	{
		BOOL bBottom = m_rcBottom.PtInRect(pt);
		if (bBottom)
		{
			bSelectedAnySticker = TRUE;
			m_partBottomSelected.RenderMarkup(&dc, m_rcBottom);
		}
		else
			m_partBottomNormal.RenderMarkup(&dc, m_rcBottom);
	}

	CSize szBack		 = XTP_DPI_SIZE(CSize(112, 112));
	CPoint ptStickerBack = m_rcSticker.CenterPoint();
	ptStickerBack.x -= szBack.cx / 2;
	ptStickerBack.y -= szBack.cy / 2;

	if (bSelectedAnySticker)
		m_partBackSelected.RenderMarkup(&dc, m_rcBack);
	else
		m_partBackNormal.RenderMarkup(&dc, m_rcBack);
}

XTPDockingPaneStickerType CXTPDockPaneSticker::HitTest(POINT pt) const
{
	ScreenToClient(&pt);

	if (m_stickers & xtpPaneStickerCenter && m_rcCenter.PtInRect(pt))
		return xtpPaneStickerCenter;
	else if (m_stickers & xtpPaneStickerLeft && m_rcLeft.PtInRect(pt))
		return xtpPaneStickerLeft;
	else if (m_stickers & xtpPaneStickerTop && m_rcTop.PtInRect(pt))
		return xtpPaneStickerTop;
	else if (m_stickers & xtpPaneStickerRight && m_rcRight.PtInRect(pt))
		return xtpPaneStickerRight;
	else if (m_stickers & xtpPaneStickerBottom && m_rcBottom.PtInRect(pt))
		return xtpPaneStickerBottom;
	else
		return xtpPaneStickerNone;
}
#	endif
#endif