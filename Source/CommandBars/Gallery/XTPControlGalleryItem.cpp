// XTPControlGalleryItem.cpp : implementation file.
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
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/ScrollBar/XTPScrollBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/Gallery/XTPControlGalleryItem.h"
#include "CommandBars/Gallery/XTPControlGalleryItems.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryItem

IMPLEMENT_DYNAMIC(CXTPControlGalleryItem, CXTPCmdTarget);

CXTPControlGalleryItem::CXTPControlGalleryItem()
{
	m_pItems = NULL;

	m_nId	  = 0;
	m_nIndex   = -1;
	m_nImage   = -1;
	m_dwData   = 0;
	m_bLabel   = FALSE;
	m_bEnabled = TRUE;

	m_szItem = CSize(0, 0);

	m_nImageWidth = ICON_SMALL;

	m_pMarkupUIElement = NULL;

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPControlGalleryItem::~CXTPControlGalleryItem()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPControlGalleryItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled == bEnabled)
		return;

	m_bEnabled = bEnabled;

	if (m_pItems && m_pItems->GetAction())
		m_pItems->GetAction()->RedrawControls();
}

void CXTPControlGalleryItem::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (m_pItems && m_pItems->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(m_pItems->GetMarkupContext(), lpszCaption);
	}
}

BOOL CXTPControlGalleryItem::GetEnabled() const
{
	return m_bEnabled;
}

int CXTPControlGalleryItem::GetIndex() const
{
	return m_nIndex;
}

int AFX_CDECL CXTPControlGalleryItem::GetDefaultHeight()
{
	return (CXTPControlGallery::m_bAutoScaleDpi ? XTP_DPI_Y(19) : 19);
}

CSize CXTPControlGalleryItem::GetSize()
{
	if (m_bLabel)
	{
		return CSize(0,
					 m_szItem.cy == 0 ? CXTPControlGalleryItem::GetDefaultHeight() : m_szItem.cy);
	}

	return CSize(m_szItem.cx == 0 ? m_pItems->m_szItem.cx : m_szItem.cx,
				 m_szItem.cy == 0 ? m_pItems->m_szItem.cy : m_szItem.cy);
}

void CXTPControlGalleryItem::SetSize(CSize szItem)
{
	if (CXTPControlGallery::m_bAutoScaleDpi)
	{
		szItem = XTP_DPI(CSize(szItem.cx, szItem.cy));
	}

	m_szItem = szItem;
}

void CXTPControlGalleryItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem,
								  BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	CXTPControlGalleryPaintManager* pGalleryPaintManager = pGallery->GetGalleryPaintManager();

	if (IsLabel())
	{
		pGalleryPaintManager->DrawLabel(pDC, this, rcItem);
	}
	else
	{
		pGalleryPaintManager->DrawItem(pDC, rcItem, bEnabled, bSelected, bPressed, bChecked, this,
									   pGallery);
	}
}

CXTPImageManagerIcon* CXTPControlGalleryItem::GetImage()
{
	if (!m_pItems)
		return NULL;

	return m_pItems->GetImageManager()->GetImage(XTPToUInt(GetImageIndex() >= 0 ? GetImageIndex()
																				: m_nId),
												 m_nImageWidth);
}

int CXTPControlGalleryItem::GetImageIndex()
{
	return m_nImage;
}

void CXTPControlGalleryItem::SetImageIndex(int nImageIndex)
{
	m_nImage = nImageIndex;
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlGalleryItem, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Id", 1, m_nId, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "ImageIndex", 2, OleGetImageIndex, OleSetImageIndex,
						VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Caption", 3, m_strCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "ToolTip", 4, m_strToolTip, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Index", 5, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "Tag", 6, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Label", 7, OleGetLabel, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "ItemWidth", 8, OleGetItemWidth, OleSetItemWidth,
						VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "ItemHeight", 9, OleGetItemHeight, OleSetItemHeight,
						VT_I4)
	DISP_PROPERTY_ID(CXTPControlGalleryItem, "DescriptionText", 10, m_strDescriptionText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlGalleryItem, "Enabled", 11, GetEnabled, SetEnabled, VT_BOOL)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlGalleryItem, CXTPCmdTarget)
	INTERFACE_PART(CXTPControlGalleryItem, XTPDIID_ICommandBarGalleryItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlGalleryItem, XTPDIID_ICommandBarGalleryItem)

int CXTPControlGalleryItem::OleGetImageIndex()
{
	return GetImageIndex();
}

void CXTPControlGalleryItem::OleSetImageIndex(int nImageIndex)
{
	SetImageIndex(nImageIndex);
}

BOOL CXTPControlGalleryItem::OleGetLabel()
{
	return IsLabel();
}

int CXTPControlGalleryItem::OleGetItemWidth()
{
	return m_szItem.cx == 0 ? m_pItems->m_szItem.cx : m_szItem.cx;
}

void CXTPControlGalleryItem::OleSetItemWidth(int cx)
{
	m_szItem.cx = cx;
}

int CXTPControlGalleryItem::OleGetItemHeight()
{
	return m_szItem.cy == 0 ? m_pItems->m_szItem.cy : m_szItem.cy;
}

void CXTPControlGalleryItem::OleSetItemHeight(int cy)
{
	m_szItem.cy = cy;
}

#endif
