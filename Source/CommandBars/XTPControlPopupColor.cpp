// XTPControlPopupColor.cpp : implementation of the CXTPControlPopupColor class.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPHookManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/Extensions/XTPMarkupStaticExtension.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlPopupColor.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_TO_RGBQUAD(clr) (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

#define XTP_SELECTOR_SIZE XTP_DPI_X(18)
#define XTP_SELECTOR_MARGIN XTP_DPI_X(2)

#define XTP_SELECTOR_WIDTH (XTP_SELECTOR_SIZE * 8 + XTP_SELECTOR_MARGIN * 2)
#define XTP_SELECTOR_HEIGHT (XTP_SELECTOR_SIZE * 5 + XTP_SELECTOR_MARGIN * 2)

IMPLEMENT_XTP_CONTROL(CXTPControlPopupColor, CXTPControlPopup)

CXTPControlPopupColor::CXTPControlPopupColor()
{
	m_controlType = xtpControlSplitButtonPopup;
	m_clr		  = (COLORREF)-1;
	EnableAutomation();
}

CXTPControlPopupColor::~CXTPControlPopupColor()
{
}

void CXTPControlPopupColor::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControlPopup::Copy(pControl, bRecursive);
		m_clr = ((CXTPControlPopupColor*)pControl)->m_clr;
	}
}

void CXTPControlPopupColor::RedrawIcon(CXTPImageManagerIcon* pImage,
									   CXTPImageManagerIconHandle& hHandle)
{
	CXTPImageManagerIconHandle& hIcon = pImage->GetIcon();

	if (hIcon.IsEmpty())
		return;

	hHandle.CopyHandle(hIcon);

	if (!hHandle.IsAlpha())
	{
		ICONINFO info;

		if (GetIconInfo(hHandle.GetIcon(), &info))
		{
			{
				CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(info.hbmColor));
				CXTPCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				::GetObject(info.hbmColor, sizeof(BITMAP), &bmp);
				int nHeight = int((double)bmp.bmHeight / 5);

				CRect rc(0, bmp.bmHeight - nHeight, bmp.bmWidth, bmp.bmHeight);
				dc.FillSolidRect(rc, m_clr);
				dcMask.FillSolidRect(rc, 1);
			}

			hHandle.Attach(CreateIconIndirect(&info), FALSE);

			::DeleteObject(info.hbmMask);
			::DeleteObject(info.hbmColor);
		}
	}
	else if (hHandle.PreMultiply())
	{
		LPDWORD pBits = (LPDWORD)hHandle.PreMultiply();
		CSize szIcon  = hHandle.GetExtent();

		DWORD dwColor = 0xFF000000 | CLR_TO_RGBQUAD(m_clr);
		int nCount	= (szIcon.cy / 5) * szIcon.cx;

		for (int i = 0; i < nCount; i++)
		{
			*pBits++ = dwColor;
		}
	}
}

void CXTPControlPopupColor::RedrawVectorIcon(CXTPImageManagerVectorImageHandle pIcon, UINT nWidth,
											 XTPImageState state)
{
	CXTPMarkupContext* pMarkupContext = pIcon.GetMarkupContext();
	if (NULL == pMarkupContext)
		return;

	CXTPMarkupStaticExtension* pExtensionRoot = pMarkupContext->GetExtensionRoot();
	if (NULL == pExtensionRoot)
		return;

	CXTPMarkupStaticExtensionBrushProperty* pBrush =
		static_cast<CXTPMarkupStaticExtensionBrushProperty*>(
			pExtensionRoot->FindExtension(L"Color"));
	if (NULL == pBrush)
		return;

	pBrush->SetValue(m_clr);

	pIcon = CXTPImageManagerVectorImageHandle::CreateMarkupHandle(pMarkupContext, TRUE,
																  pIcon.GetDataStream());
	if (NULL == pIcon)
		return;

	VERIFY(GetImageManager()->SetVectorIcon(pIcon, XTPToUInt(GetIconId()), nWidth, state));
}

void CXTPControlPopupColor::SetColor(COLORREF clr)
{
	if (clr != m_clr)
	{
		m_clr = clr;

		CXTPImageManagerIconSet* pIconSet = GetImageManager()->GetIconSet(XTPToUInt(GetIconId()));
		if (pIconSet)
		{
			pIconSet->RefreshAll();

			CXTPImageManagerIconSet::CIconSetMap* pMap = pIconSet->GetIcons();

			POSITION pos = pMap->GetStartPosition();
			UINT nWidth;
			CXTPImageManagerIcon* pImage;
			while (pos != NULL)
			{
				pMap->GetNextAssoc(pos, nWidth, pImage);

				if (pImage->IsRasterIcon())
				{
					CXTPImageManagerIconHandle hIcon;
					RedrawIcon(pImage, hIcon);

					if (!hIcon.IsEmpty())
					{
						pImage->SetIcon(hIcon);
					}
				}

				if (pImage->IsVectorIcon())
				{
					CXTPImageManagerVectorImageHandle ImageNormal = pImage->GetVectorIcon(
						xtpImageNormal);
					RedrawVectorIcon(ImageNormal, nWidth, xtpImageNormal);

					// CXTPImageManagerVectorImageHandle ImageDisabled =
					// pImage->GetVectorIcon(xtpImageDisabled); RedrawVectorIcon(ImageDisabled,
					// nWidth, xtpImageDisabled);

					CXTPImageManagerVectorImageHandle ImageHot = pImage->GetVectorIcon(xtpImageHot);
					RedrawVectorIcon(ImageHot, nWidth, xtpImageHot);

					CXTPImageManagerVectorImageHandle ImageChecked = pImage->GetVectorIcon(
						xtpImageChecked);
					RedrawVectorIcon(ImageChecked, nWidth, xtpImageChecked);

					CXTPImageManagerVectorImageHandle ImagePressed = pImage->GetVectorIcon(
						xtpImagePressed);
					RedrawVectorIcon(ImagePressed, nWidth, xtpImagePressed);
				}
			}

			RedrawParent(FALSE);
		}
	}
}

COLORREF CXTPControlPopupColor::GetColor() const
{
	return m_clr;
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CXTPControlButtonColor, CXTPControlButton)

CXTPControlButtonColor::CXTPControlButtonColor()
{
	SetStyle(xtpButtonCaption);
	m_clr = COLORREF_NULL;
}

void CXTPControlButtonColor::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControlButton::Copy(pControl, bRecursive);
		m_clr = ((CXTPControlButtonColor*)pControl)->m_clr;
	}
}

void CXTPControlButtonColor::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControlButton::DoPropExchange(pPX);

	if (pPX->GetSchema() > _XTP_SCHEMA_1041)
	{
		PX_DWord(pPX, _T("Color"), m_clr, COLORREF_NULL);
	}
}

CSize CXTPControlButtonColor::GetSize(CDC* /*pDC*/)
{
	return CSize(XTP_SELECTOR_WIDTH, max(XTP_DPI_Y(23), GetPaintManager()->GetControlHeight()));
}

void CXTPControlButtonColor::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPFontDC font(pDC, pPaintManager->GetRegularFont());

	pPaintManager->DrawControlEntry(pDC, this);
	pDC->SetTextColor(pPaintManager->GetControlTextColor(this));

	CRect rcText = GetRect();
	pPaintManager->DrawControlText(pDC, this, &rcText, TRUE, FALSE, TRUE, FALSE);

	if (m_clr != COLORREF_NULL)
	{
		CRect rcButton(CPoint(m_rcControl.left + XTP_DPI_X(2),
							  m_rcControl.CenterPoint().y - XTP_SELECTOR_SIZE / 2),
					   CSize(XTP_SELECTOR_SIZE, XTP_SELECTOR_SIZE));
		rcButton.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));

		if (GetEnabled())
			pDC->FillSolidRect(rcButton, m_clr);

		pDC->Draw3dRect(rcButton, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPControlButtonColor::SetColor(COLORREF clr)
{
	if (clr != m_clr)
	{
		m_clr = clr;
		RedrawParent(FALSE);
	}
}

COLORREF CXTPControlButtonColor::GetColor() const
{
	return m_clr;
}

struct CXTPControlColorSelector::CColorInfo
{
	CColorInfo(COLORREF _clrButton, UINT _toolTipID)
	{
		clrButton = _clrButton;
		toolTipID = _toolTipID;
	}

	COLORREF clrButton;
	UINT toolTipID;
	CString strTip;
};

static CCriticalSection gExtendedColorsAccess;
static HINSTANCE ghExtendedColorsResourceInstance			  = NULL;
static CXTPControlColorSelector::CColorInfo gExtendedColors[] = {
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x00, 0x00), XTP_IDS_CLR_BLACK),
	CXTPControlColorSelector::CColorInfo(RGB(0x99, 0x33, 0x00), XTP_IDS_CLR_BROWN),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0x33, 0x00), XTP_IDS_CLR_OLIVE_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x33, 0x00), XTP_IDS_CLR_DARK_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x33, 0x66), XTP_IDS_CLR_DARK_TEAL),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x00, 0x80), XTP_IDS_CLR_DARK_BLUE),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0x33, 0x99), XTP_IDS_CLR_INDIGO),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0x33, 0x33), XTP_IDS_CLR_GRAY80),
	CXTPControlColorSelector::CColorInfo(RGB(0x80, 0x00, 0x00), XTP_IDS_CLR_DARK_RED),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0x66, 0x00), XTP_IDS_CLR_ORANGE),
	CXTPControlColorSelector::CColorInfo(RGB(0x80, 0x80, 0x00), XTP_IDS_CLR_DARK_YELLOW),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x80, 0x00), XTP_IDS_CLR_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x80, 0x80), XTP_IDS_CLR_TEAL),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0x00, 0xFF), XTP_IDS_CLR_BLUE),
	CXTPControlColorSelector::CColorInfo(RGB(0x66, 0x66, 0x99), XTP_IDS_CLR_BLUEGRAY),
	CXTPControlColorSelector::CColorInfo(RGB(0x80, 0x80, 0x80), XTP_IDS_CLR_GRAY50),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0x00, 0x00), XTP_IDS_CLR_RED),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0x99, 0x00), XTP_IDS_CLR_LIGHTORANGE),
	CXTPControlColorSelector::CColorInfo(RGB(0x99, 0xCC, 0x00), XTP_IDS_CLR_LIME),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0x99, 0x66), XTP_IDS_CLR_SEA_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0xCC, 0xCC), XTP_IDS_CLR_AQUA),
	CXTPControlColorSelector::CColorInfo(RGB(0x33, 0x66, 0xFF), XTP_IDS_CLR_LIGHT_BLUE),
	CXTPControlColorSelector::CColorInfo(RGB(0x80, 0x00, 0x80), XTP_IDS_CLR_VIOLET),
	CXTPControlColorSelector::CColorInfo(RGB(0x99, 0x99, 0x99), XTP_IDS_CLR_GRAY40),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0x00, 0xFF), XTP_IDS_CLR_PINK),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0xCC, 0x00), XTP_IDS_CLR_GOLD),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0xFF, 0x00), XTP_IDS_CLR_YELLOW),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0xFF, 0x00), XTP_IDS_CLR_BRIGHT_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0xFF, 0xFF), XTP_IDS_CLR_TURQUOISE),
	CXTPControlColorSelector::CColorInfo(RGB(0x00, 0xCC, 0xFF), XTP_IDS_CLR_SKY_BLUE),
	CXTPControlColorSelector::CColorInfo(RGB(0x99, 0x33, 0x66), XTP_IDS_CLR_PLUM),
	CXTPControlColorSelector::CColorInfo(RGB(0xC0, 0xC0, 0xC0), XTP_IDS_CLR_GRAY25),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0x99, 0xCC), XTP_IDS_CLR_ROSE),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0xCC, 0x99), XTP_IDS_CLR_TAN),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0xFF, 0x99), XTP_IDS_CLR_LIGHT_YELLOW),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC, 0xFF, 0xCC), XTP_IDS_CLR_LIGHT_GREEN),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC, 0xFF, 0xFF), XTP_IDS_CLR_LIGHT_TURQUOISE),
	CXTPControlColorSelector::CColorInfo(RGB(0x99, 0xCC, 0xFF), XTP_IDS_CLR_PALE_BLUE),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC, 0x99, 0xFF), XTP_IDS_CLR_LAVENDER),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF, 0xFF, 0xFF), XTP_IDS_CLR_WHITE),
};

IMPLEMENT_XTP_CONTROL(CXTPControlColorSelector, CXTPControl)

CXTPControlColorSelector::CXTPControlColorSelector()
{
	EnableAutomation();

	m_nSelected = -1;
	m_clr		= (COLORREF)-1;
	SetFlags(xtpFlagNoMovable);

	HINSTANCE hCurrentResourceInstance = XTPResourceManager()->GetResourceHandle();
	if (ghExtendedColorsResourceInstance != hCurrentResourceInstance)
	{
		CSingleLock lock(&gExtendedColorsAccess);
		if (ghExtendedColorsResourceInstance != hCurrentResourceInstance)
		{
			int nCount = GetItemCount();
			for (int i = 0; i < nCount; i++)
			{
				XTPResourceManager()->LoadString(&gExtendedColors[i].strTip,
												 gExtendedColors[i].toolTipID);
			}
			ghExtendedColorsResourceInstance = hCurrentResourceInstance;
		}
	}
	m_nPressed = -1;
}

CSize CXTPControlColorSelector::GetSize(CDC* /*pDC*/)
{
	return CSize(XTP_SELECTOR_WIDTH, XTP_SELECTOR_HEIGHT);
}

INT_PTR CXTPControlColorSelector::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int nIndex = HitTest(point);
	if (nIndex == -1)
		return 0;

	INT_PTR nHit = XTPToIntPtr(gExtendedColors[nIndex].toolTipID);

	CXTPToolTipContext::FillInToolInfo(pTI, GetParent()->GetSafeHwnd(), GetRect(nIndex), nHit,
									   gExtendedColors[nIndex].strTip);

	return nHit;
}

CRect CXTPControlColorSelector::GetRect(int nIndex) const
{
	int x = nIndex % 8, y = nIndex / 8;

	return CRect(CPoint(XTP_SELECTOR_MARGIN + m_rcControl.left + x * XTP_SELECTOR_SIZE,
						XTP_SELECTOR_MARGIN + m_rcControl.top + y * XTP_SELECTOR_SIZE),
				 CSize(XTP_SELECTOR_SIZE, XTP_SELECTOR_SIZE));
}

int CXTPControlColorSelector::HitTest(CPoint point) const
{
	int nCount = GetItemCount();

	if (!m_rcControl.PtInRect(point))
		return -1;

	for (int i = 0; i < nCount; i++)
	{
		if (GetRect(i).PtInRect(point))
			return i;
	}

	return -1;
}

void CXTPControlColorSelector::Draw(CDC* pDC)
{
	int nCount = GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CRect rcButton = GetRect(i);

		if (i == m_nSelected || gExtendedColors[i].clrButton == m_clr)
		{
			BOOL bPressed = GetPressed() && i == m_nSelected;
			GetPaintManager()->DrawRectangle(pDC, rcButton, i == m_nSelected, bPressed,
											 GetEnabled(), gExtendedColors[i].clrButton == m_clr,
											 FALSE, xtpBarTypeNormal, xtpBarPopup);
		}

		rcButton.DeflateRect(XTP_DPI_X(3), XTP_DPI_Y(3));
		if (GetEnabled())
			pDC->FillSolidRect(rcButton, gExtendedColors[i].clrButton);

		pDC->Draw3dRect(rcButton, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPControlColorSelector::SetSelectedItem(int nItem)
{
	if (m_nSelected != nItem)
	{
		m_nSelected = nItem;
		RedrawParent();
	}
}

void CXTPControlColorSelector::OnMouseMove(CPoint point)
{
	int nHit = HitTest(point);
	if (nHit != -1)
	{
		SetSelectedItem(nHit);
	}
}

BOOL CXTPControlColorSelector::OnSetSelected(int bSelected)
{
	if (!bSelected)
		m_nSelected = -1;

	if (IsKeyboardSelected(bSelected) && !m_bSelected)
	{
		SetSelectedItem(bSelected == TRUE_KEYBOARD_PREV ? GetItemCount() - 1 : 0);
	}

	return CXTPControl::OnSetSelected(bSelected);
}

BOOL CXTPControlColorSelector::IsFocused() const
{
	return GetSelected();
}

int CXTPControlColorSelector::GetItemCount() const
{
	return sizeof(gExtendedColors) / sizeof(gExtendedColors[0]);
}

BOOL CXTPControlColorSelector::OnHookKeyDown(UINT nChar, LPARAM /*lParam*/)
{
	ASSERT(IsFocused());

	if (!IsFocused())
		return FALSE;

	CXTPDrawHelpers::KeyToLayout(GetParent(), nChar);

	switch (nChar)
	{
		case VK_HOME: SetSelectedItem(0); return TRUE;

		case VK_END: SetSelectedItem(GetItemCount() - 1); return TRUE;

		case VK_LEFT:
			if (m_nSelected <= 0)
				return FALSE;

			SetSelectedItem(m_nSelected - 1);
			return TRUE;

		case VK_RIGHT:
			if (m_nSelected >= GetItemCount() - 1)
				return FALSE;

			SetSelectedItem(m_nSelected + 1);
			return TRUE;

		case VK_UP:
			if (m_nSelected < 8)
				return FALSE;

			SetSelectedItem(m_nSelected - 8);
			return TRUE;

		case VK_DOWN:
			if (m_nSelected >= GetItemCount() - 8)
				return FALSE;

			SetSelectedItem(m_nSelected + 8);
			return TRUE;
	}

	return FALSE;
}

void CXTPControlColorSelector::OnExecute()
{
	if (m_nPressed != -1)
	{
		m_clr = gExtendedColors[m_nPressed].clrButton;
	}
	CXTPControl::OnExecute();
}

COLORREF CXTPControlColorSelector::GetColor() const
{
	return m_clr;
}

void CXTPControlColorSelector::SetColor(COLORREF clr)
{
	if (clr != m_clr)
	{
		m_clr = clr;
		RedrawParent();
	}
}

void CXTPControlColorSelector::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!GetEnabled())
		return;

	int nHit = bKeyboard ? m_nSelected : HitTest(pt);
	if (nHit == -1)
		return;

	m_nPressed = nHit;

	if (!bKeyboard)
	{
		ClickToolBarButton();
	}
	else
	{
		OnExecute();
	}
	m_nPressed = -1;
}

void CXTPControlColorSelector::OnLButtonUp(CPoint point)
{
	if (GetEnabled() && m_pParent->GetPosition() == xtpBarPopup)
	{
		OnClick(TRUE, point);
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlPopupColor, CXTPControlPopup)
	DISP_PROPERTY_EX_ID(CXTPControlPopupColor, "Color", 200, GetColor, SetColor, VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlPopupColor, CXTPControlPopup)
	INTERFACE_PART(CXTPControlPopupColor, XTPDIID_CommandBarPopupColor, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlPopupColor, XTPDIID_CommandBarPopupColor)

//////////////////////////////////////////////////////////////////////////
BEGIN_DISPATCH_MAP(CXTPControlColorSelector, CXTPControl)
	DISP_PROPERTY_EX_ID(CXTPControlColorSelector, "Color", 200, GetColor, SetColor, VT_COLOR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlColorSelector, CXTPControl)
	INTERFACE_PART(CXTPControlColorSelector, XTPDIID_CommandBarControlColorSelector, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlColorSelector, XTPDIID_CommandBarControlColorSelector)

#endif
