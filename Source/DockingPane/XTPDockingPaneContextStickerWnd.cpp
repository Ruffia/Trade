// XTPDockingPaneContextStickerWnd.cpp : implementation of the CXTPDockingPaneContextStickerWnd
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

#include "StdAfx.h"

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
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
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "DockingPane/Resource.h"
#include "DockingPane/XTPDockingPaneDefines.h"
#include "DockingPane/XTPDockingPaneBase.h"
#include "DockingPane/XTPDockingPaneContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextStickerWnd

CXTPDockingPaneContextStickerWnd::CXTPDockingPaneContextStickerWnd(CXTPDockingPaneContext* pContext)
	: m_pContext(pContext)
	, STICKERS_BACK(0)
	, STICKERS_CENTER(1)
	, STICKERS_LEFT(2)
	, STICKERS_TOP(3)
	, STICKERS_RIGHT(4)
	, STICKERS_BOTTOM(5)
{
	m_typeSticker = m_selectedSticker = xtpPaneStickerNone;

	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);
	ASSERT(m_pMarkupContext);

	m_partBackNormal		= new CXTPMarkupThemePart();
	m_partBackSelected		= new CXTPMarkupThemePart();
	m_partSmallBackNormal   = new CXTPMarkupThemePart();
	m_partSmallBackSelected = new CXTPMarkupThemePart();
	m_partLeftNormal		= new CXTPMarkupThemePart();
	m_partLeftSelected		= new CXTPMarkupThemePart();
	m_partTopNormal			= new CXTPMarkupThemePart();
	m_partTopSelected		= new CXTPMarkupThemePart();
	m_partRightNormal		= new CXTPMarkupThemePart();
	m_partRightSelected		= new CXTPMarkupThemePart();
	m_partBottomNormal		= new CXTPMarkupThemePart();
	m_partBottomSelected	= new CXTPMarkupThemePart();
	m_partCenterNormal		= new CXTPMarkupThemePart();
	m_partCenterSelected	= new CXTPMarkupThemePart();

	m_pImgManager = new CXTPImageManager();
}

CXTPDockingPaneContextStickerWnd::~CXTPDockingPaneContextStickerWnd()
{
	SAFE_DELETE(m_pImgManager);

	SAFE_DELETE(m_partBackNormal);
	SAFE_DELETE(m_partBackSelected);
	SAFE_DELETE(m_partSmallBackNormal);
	SAFE_DELETE(m_partSmallBackSelected);
	SAFE_DELETE(m_partLeftNormal);
	SAFE_DELETE(m_partLeftSelected);
	SAFE_DELETE(m_partTopNormal);
	SAFE_DELETE(m_partTopSelected);
	SAFE_DELETE(m_partRightNormal);
	SAFE_DELETE(m_partRightSelected);
	SAFE_DELETE(m_partBottomNormal);
	SAFE_DELETE(m_partBottomSelected);
	SAFE_DELETE(m_partCenterNormal);
	SAFE_DELETE(m_partCenterSelected);

	XTPMarkupReleaseContext(m_pMarkupContext);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPDockingPaneContextStickerWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextStickerWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextStickerWnd message handlers

void CXTPDockingPaneContextStickerWnd::Init()
{
	XTPDockingContextStickerStyle nStyle = m_pContext->GetStickerStyle();

	switch (nStyle)
	{
		case xtpPaneStickerStyleVisualStudio2005Beta:
		case xtpPaneStickerStyleVisualStudio2005:
			InitSize_VC2005();
			InitIcons_VC2005();
			break;

		case xtpPaneStickerStyleVisualStudio2008:
		case xtpPaneStickerStyleVisualStudio2010:
			InitSize_VC2008();
			InitIcons_VC2008();
			break;

		case xtpPaneStickerStyleVisualStudio2012Light:
		case xtpPaneStickerStyleVisualStudio2012Dark:
		case xtpPaneStickerStyleVisualStudio2015Light:
		case xtpPaneStickerStyleVisualStudio2015Blue:
		case xtpPaneStickerStyleVisualStudio2015Dark:
		case xtpPaneStickerStyleVisualStudio2017Blue:
		case xtpPaneStickerStyleVisualStudio2017BlueExtra:
		case xtpPaneStickerStyleVisualStudio2017Dark:
		case xtpPaneStickerStyleVisualStudio2017Light:
		case xtpPaneStickerStyleVisualStudio2019Blue:
		case xtpPaneStickerStyleVisualStudio2019BlueExtra:
		case xtpPaneStickerStyleVisualStudio2019Dark:
		case xtpPaneStickerStyleVisualStudio2019Light:
		case xtpPaneStickerStyleVisualStudio2022Blue:
		case xtpPaneStickerStyleVisualStudio2022BlueExtra:
		case xtpPaneStickerStyleVisualStudio2022Dark:
		case xtpPaneStickerStyleVisualStudio2022Light:

		case xtpPaneStickerStyleNativeWindows10Light:
		case xtpPaneStickerStyleNativeWindows10Dark:
		default:
			InitSize();
			InitColors();
			InitIcons();
			break;
	}
}

BOOL CXTPDockingPaneContextStickerWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockingPaneContextStickerWnd::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));
	OnDraw(&dc);
}

void CXTPDockingPaneContextStickerWnd::OnDraw(CDC* pDC)
{
	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	XTPDockingContextStickerStyle nStyle = m_pContext->GetStickerStyle();

	CRect rcIcon = CRect(m_delimeterX, m_delimeterY, m_delimeterX + m_szIcon.cx,
						 m_delimeterY + m_szIcon.cy);

	if (nStyle == xtpPaneStickerStyleVisualStudio2005Beta
		|| nStyle == xtpPaneStickerStyleVisualStudio2005
		|| nStyle == xtpPaneStickerStyleVisualStudio2008
		|| nStyle == xtpPaneStickerStyleVisualStudio2010)
	{
		if (bClient)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_BACK,
																  m_rcBack_3x3.Width());

			if (m_selectedSticker == xtpPaneStickerCenter)
				pIcon->Draw(pDC, m_rcBack_3x3.TopLeft(), xtpImageHot, m_rcBack_3x3.Size());
			else
				pIcon->Draw(pDC, m_rcBack_3x3.TopLeft(), m_rcBack_3x3.Size());
		}
		else
		{
			//
		}

		if (m_typeSticker & xtpPaneStickerCenter)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_CENTER, m_szIcon.cx);
			if (m_selectedSticker == xtpPaneStickerCenter)
				pIcon->Draw(pDC, m_rcCenter.TopLeft(), xtpImageHot, m_rcCenter.Size());
			else
				pIcon->Draw(pDC, m_rcCenter.TopLeft(), m_rcCenter.Size());
		}

		if (m_typeSticker & xtpPaneStickerTop)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_TOP, m_szIcon.cx);

			if (m_selectedSticker == xtpPaneStickerTop)
				pIcon->Draw(pDC, bClient ? m_rcTop.TopLeft() : rcIcon.TopLeft(), xtpImageHot,
							bClient ? m_rcTop.Size() : rcIcon.Size());
			else
				pIcon->Draw(pDC, bClient ? m_rcTop.TopLeft() : rcIcon.TopLeft(),
							bClient ? m_rcTop.Size() : rcIcon.Size());
		}
		if (m_typeSticker & xtpPaneStickerLeft)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_LEFT, m_szIcon.cx);
			if (m_selectedSticker == xtpPaneStickerLeft)
				pIcon->Draw(pDC, bClient ? m_rcLeft.TopLeft() : rcIcon.TopLeft(), xtpImageHot,
							bClient ? m_rcLeft.Size() : rcIcon.Size());
			else
				pIcon->Draw(pDC, bClient ? m_rcLeft.TopLeft() : rcIcon.TopLeft(),
							bClient ? m_rcLeft.Size() : rcIcon.Size());
		}
		if (m_typeSticker & xtpPaneStickerBottom)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_BOTTOM, m_szIcon.cx);

			if (m_selectedSticker == xtpPaneStickerBottom)
				pIcon->Draw(pDC, bClient ? m_rcBottom.TopLeft() : rcIcon.TopLeft(), xtpImageHot,
							bClient ? m_rcBottom.Size() : rcIcon.Size());
			else
				pIcon->Draw(pDC, bClient ? m_rcBottom.TopLeft() : rcIcon.TopLeft(),
							bClient ? m_rcBottom.Size() : rcIcon.Size());
		}
		if (m_typeSticker & xtpPaneStickerRight)
		{
			CXTPImageManagerIcon* pIcon = m_pImgManager->GetImage(STICKERS_RIGHT, m_szIcon.cx);
			if (m_selectedSticker == xtpPaneStickerRight)
				pIcon->Draw(pDC, bClient ? m_rcRight.TopLeft() : rcIcon.TopLeft(), xtpImageHot,
							bClient ? m_rcRight.Size() : rcIcon.Size());
			else
				pIcon->Draw(pDC, bClient ? m_rcRight.TopLeft() : rcIcon.TopLeft(),
							bClient ? m_rcRight.Size() : rcIcon.Size());
		}
	}
	else
	{
		if (bClient)
		{
			if (m_selectedSticker == xtpPaneStickerCenter)
				m_partBackSelected->RenderMarkup(pDC, m_rcBack_3x3);
			else
				m_partBackNormal->RenderMarkup(pDC, m_rcBack_3x3);
		}
		else
		{
			if (m_selectedSticker == xtpPaneStickerCenter)
				m_partSmallBackSelected->RenderMarkup(pDC, m_rcBack_1x1);
			else
				m_partSmallBackNormal->RenderMarkup(pDC, m_rcBack_1x1);
		}

		if (m_typeSticker & xtpPaneStickerCenter)
		{
			if (m_selectedSticker == xtpPaneStickerCenter)
				m_partCenterSelected->RenderMarkup(pDC, m_rcCenter);
			else
				m_partCenterNormal->RenderMarkup(pDC, m_rcCenter);
		}

		if (m_typeSticker & xtpPaneStickerTop)
		{
			if (m_selectedSticker == xtpPaneStickerTop)
				m_partTopSelected->RenderMarkup(pDC, bClient ? m_rcTop : rcIcon);
			else
				m_partTopNormal->RenderMarkup(pDC, bClient ? m_rcTop : rcIcon);
		}
		if (m_typeSticker & xtpPaneStickerLeft)
		{
			if (m_selectedSticker == xtpPaneStickerLeft)
				m_partLeftSelected->RenderMarkup(pDC, bClient ? m_rcLeft : rcIcon);
			else
				m_partLeftNormal->RenderMarkup(pDC, bClient ? m_rcLeft : rcIcon);
		}
		if (m_typeSticker & xtpPaneStickerBottom)
		{
			if (m_selectedSticker == xtpPaneStickerBottom)
				m_partBottomSelected->RenderMarkup(pDC, bClient ? m_rcBottom : rcIcon);
			else
				m_partBottomNormal->RenderMarkup(pDC, bClient ? m_rcBottom : rcIcon);
		}
		if (m_typeSticker & xtpPaneStickerRight)
		{
			if (m_selectedSticker == xtpPaneStickerRight)
				m_partRightSelected->RenderMarkup(pDC, bClient ? m_rcRight : rcIcon);
			else
				m_partRightNormal->RenderMarkup(pDC, bClient ? m_rcRight : rcIcon);
		}
	}
}

XTPDockingPaneStickerType CXTPDockingPaneContextStickerWnd::HitTest(CPoint pt)
{
	CXTPClientRect rc(this);
	ScreenToClient(&pt);

	if (!rc.PtInRect(pt))
		return xtpPaneStickerNone;

	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	if (bClient)
	{
		if (m_typeSticker & xtpPaneStickerCenter && m_rcCenter.PtInRect(pt))
			return xtpPaneStickerCenter;
		else if (m_typeSticker & xtpPaneStickerLeft && m_rcLeft.PtInRect(pt))
			return xtpPaneStickerLeft;
		else if (m_typeSticker & xtpPaneStickerTop && m_rcTop.PtInRect(pt))
			return xtpPaneStickerTop;
		else if (m_typeSticker & xtpPaneStickerRight && m_rcRight.PtInRect(pt))
			return xtpPaneStickerRight;
		else if (m_typeSticker & xtpPaneStickerBottom && m_rcBottom.PtInRect(pt))
			return xtpPaneStickerBottom;
	}
	else
	{
		CRect rcIcon = CRect(m_delimeterX, m_delimeterY, m_delimeterX + m_szIcon.cx,
							 m_delimeterY + m_szIcon.cy);

		if (m_typeSticker & xtpPaneStickerCenter && rcIcon.PtInRect(pt))
			return xtpPaneStickerCenter;
		else if (m_typeSticker & xtpPaneStickerLeft && rcIcon.PtInRect(pt))
			return xtpPaneStickerLeft;
		else if (m_typeSticker & xtpPaneStickerTop && rcIcon.PtInRect(pt))
			return xtpPaneStickerTop;
		else if (m_typeSticker & xtpPaneStickerRight && rcIcon.PtInRect(pt))
			return xtpPaneStickerRight;
		else if (m_typeSticker & xtpPaneStickerBottom && rcIcon.PtInRect(pt))
			return xtpPaneStickerBottom;
	}

	return xtpPaneStickerNone;
}

void CXTPDockingPaneContextStickerWnd::InitSize_VC2005()
{
	m_szIcon	 = CSize(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_delimeterX = XTP_DPI_X(4);
	m_delimeterY = XTP_DPI_Y(4);

	CPoint ptCenter;
	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	if (bClient)
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx * 3 + m_delimeterX * 6,
							m_szIcon.cy * 3 + m_delimeterY * 6);

		ptCenter = m_rcSticker.CenterPoint();

		m_rcBack_3x3.SetRect(ptCenter.x - XTP_DPI_X(40), ptCenter.y - XTP_DPI_Y(40),
							 ptCenter.x + XTP_DPI_X(40), ptCenter.y + XTP_DPI_Y(40));
	}
	else
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx + m_delimeterX * 2, m_szIcon.cy + m_delimeterY * 2);

		ptCenter = m_rcSticker.CenterPoint();

		// m_rcBack_1x1
	}

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
}

void CXTPDockingPaneContextStickerWnd::InitIcons_VC2005()
{
	UINT szIcon = XTPToUIntChecked(m_szIcon.cx);
	UINT szBack = XTPToUIntChecked(m_rcBack_3x3.Width());

	HMODULE hRes = XTPResourceManager()->GetResourceHandle();
	CString type = _T("RT_XAML");

	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_BACK,
								 STICKERS_BACK, szBack);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_CENTER,
								 STICKERS_CENTER, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_LEFT,
								 STICKERS_LEFT, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_TOP, STICKERS_TOP,
								 szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_RIGHT,
								 STICKERS_RIGHT, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_BOTTOM,
								 STICKERS_BOTTOM, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_CENTER_SELECTED,
								 STICKERS_CENTER, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_LEFT_SELECTED,
								 STICKERS_LEFT, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_TOP_SELECTED,
								 STICKERS_TOP, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_RIGHT_SELECTED,
								 STICKERS_RIGHT, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2005_BOTTOM_SELECTED,
								 STICKERS_BOTTOM, szIcon, xtpImageHot);
}

void CXTPDockingPaneContextStickerWnd::InitSize_VC2008()
{
	m_szIcon	 = CSize(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_delimeterX = XTP_DPI_X(4);
	m_delimeterY = XTP_DPI_Y(4);

	CPoint ptCenter;
	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	if (bClient)
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx * 3 + m_delimeterX * 6,
							m_szIcon.cy * 3 + m_delimeterY * 6);

		ptCenter = m_rcSticker.CenterPoint();

		m_rcBack_3x3.SetRect(ptCenter.x - XTP_DPI_X(40), ptCenter.y - XTP_DPI_Y(40),
							 ptCenter.x + XTP_DPI_X(40), ptCenter.y + XTP_DPI_Y(40));
	}
	else
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx + m_delimeterX * 2, m_szIcon.cy + m_delimeterY * 2);

		ptCenter = m_rcSticker.CenterPoint();

		// m_rcBack_1x1
	}

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
}

void CXTPDockingPaneContextStickerWnd::InitIcons_VC2008()
{
	UINT szIcon = XTPToUIntChecked(m_szIcon.cx);
	UINT szBack = XTPToUIntChecked(m_rcBack_3x3.Width());

	HMODULE hRes = XTPResourceManager()->GetResourceHandle();
	CString type = _T("RT_XAML");

	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_BACK,
								 STICKERS_BACK, szBack);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_CENTER,
								 STICKERS_CENTER, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_LEFT,
								 STICKERS_LEFT, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_TOP, STICKERS_TOP,
								 szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_RIGHT,
								 STICKERS_RIGHT, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_BOTTOM,
								 STICKERS_BOTTOM, szIcon);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_CENTER_SELECTED,
								 STICKERS_CENTER, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_LEFT_SELECTED,
								 STICKERS_LEFT, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_TOP_SELECTED,
								 STICKERS_TOP, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_RIGHT_SELECTED,
								 STICKERS_RIGHT, szIcon, xtpImageHot);
	m_pImgManager->SetVectorIcon(hRes, type, XTP_XAML_DOCKINGPANE_STICKERS_VC2008_BOTTOM_SELECTED,
								 STICKERS_BOTTOM, szIcon, xtpImageHot);
}

void CXTPDockingPaneContextStickerWnd::InitSize()
{
	m_szIcon	 = CSize(::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_delimeterX = XTP_DPI_X(4);
	m_delimeterY = XTP_DPI_Y(4);

	CPoint ptCenter;
	BOOL bClient = (m_typeSticker & xtpPaneStickerClient) == xtpPaneStickerClient;
	if (bClient)
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx * 3 + m_delimeterX * 6,
							m_szIcon.cy * 3 + m_delimeterY * 6);

		ptCenter = m_rcSticker.CenterPoint();

		m_rcBack_3x3.SetRect(ptCenter.x - (m_szIcon.cx / 2 + m_szIcon.cx + m_delimeterX * 2),
							 ptCenter.y - (m_szIcon.cy / 2 + m_szIcon.cy + m_delimeterY * 2),
							 ptCenter.x + (m_szIcon.cx / 2 + m_szIcon.cx + m_delimeterX * 2),
							 ptCenter.y + (m_szIcon.cy / 2 + m_szIcon.cy + m_delimeterY * 2));
	}
	else
	{
		m_rcSticker.SetRect(0, 0, m_szIcon.cx + m_delimeterX * 2, m_szIcon.cy + m_delimeterY * 2);

		ptCenter = m_rcSticker.CenterPoint();

		m_rcBack_1x1.SetRect(ptCenter.x - (m_szIcon.cx / 2 + m_delimeterX),
							 ptCenter.y - (m_szIcon.cy / 2 + m_delimeterY),
							 ptCenter.x + (m_szIcon.cx / 2 + m_delimeterX),
							 ptCenter.y + (m_szIcon.cy / 2 + m_delimeterY));
	}

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
}

void CXTPDockingPaneContextStickerWnd::InitColors()
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

void CXTPDockingPaneContextStickerWnd::InitIcons()
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
			m_partBackNormal->LoadPart(hZip, _T("Stickers/back.3x3.xaml"), m_pMarkupContext);
			m_partSmallBackNormal->LoadPart(hZip, _T("Stickers/back.1x1.xaml"), m_pMarkupContext);
			m_partCenterNormal->LoadPart(hZip, _T("Stickers/center.xaml"), m_pMarkupContext);
			m_partLeftNormal->LoadPart(hZip, _T("Stickers/dock.left.xaml"), m_pMarkupContext);
			m_partTopNormal->LoadPart(hZip, _T("Stickers/dock.top.xaml"), m_pMarkupContext);
			m_partRightNormal->LoadPart(hZip, _T("Stickers/dock.right.xaml"), m_pMarkupContext);
			m_partBottomNormal->LoadPart(hZip, _T("Stickers/dock.bottom.xaml"), m_pMarkupContext);

			m_partBackSelected->LoadPart(hZip, _T("Stickers/back.3x3.selected.xaml"),
										 m_pMarkupContext);
			m_partSmallBackSelected->LoadPart(hZip, _T("Stickers/back.1x1.selected.xaml"),
											  m_pMarkupContext);
			m_partCenterSelected->LoadPart(hZip, _T("Stickers/center.selected.xaml"),
										   m_pMarkupContext);
			m_partLeftSelected->LoadPart(hZip, _T("Stickers/dock.left.selected.xaml"),
										 m_pMarkupContext);
			m_partTopSelected->LoadPart(hZip, _T("Stickers/dock.top.selected.xaml"),
										m_pMarkupContext);
			m_partRightSelected->LoadPart(hZip, _T("Stickers/dock.right.selected.xaml"),
										  m_pMarkupContext);
			m_partBottomSelected->LoadPart(hZip, _T("Stickers/dock.bottom.selected.xaml"),
										   m_pMarkupContext);

			CloseZip(hZip);
		}
	}
}
