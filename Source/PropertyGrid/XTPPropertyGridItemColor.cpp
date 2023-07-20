// XTPPropertyGridItemColor.cpp : implementation of the CXTPPropertyGridItemColor class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridItemColor.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"
#include "PropertyGrid/XTPPropertyGridIIDs.h"

#ifdef _XTP_INCLUDE_CONTROLS
#	include "Controls/Util/XTPControlTheme.h"
#	include "Controls/Dialog/XTPColorDialog.h"
#	include "Controls/Popup/XTPColorSelectorCtrl.h"
#	include "Controls/Popup/XTPColorPopup.h"
#	include "Controls/Defines.h"
#endif

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemColor

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemColor, CXTPPropertyGridItem)

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(LPCTSTR strCaption, COLORREF clr,
													 COLORREF* pBindColor)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags		  = xtpPropertyGridItemHasEdit | xtpPropertyGridItemHasExpandButton;
	m_strDefaultValue = m_strValue;
	m_colorEditor	 = xtpPropertyGridItemColorExtendedDialog;
}

CXTPPropertyGridItemColor::CXTPPropertyGridItemColor(UINT nID, COLORREF clr, COLORREF* pBindColor)
	: CXTPPropertyGridItem(nID)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags		  = xtpPropertyGridItemHasEdit | xtpPropertyGridItemHasExpandButton;
	m_strDefaultValue = m_strValue;
	m_colorEditor	 = xtpPropertyGridItemColorExtendedDialog;
}

CXTPPropertyGridItemColor::~CXTPPropertyGridItemColor()
{
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL CXTPPropertyGridItemColor::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	COLORREF clr = dc.GetTextColor();
	CRect rcSample(rcValue.left - XTP_DPI_X(2), rcValue.top + XTP_DPI_Y(1),
				   rcValue.left + XTP_DPI_X(18), rcValue.bottom - XTP_DPI_Y(1));
	CXTPPenDC pen(dc, clr);
	CXTPBrushDC brush(dc, m_clrValue);
	dc.Rectangle(rcSample);

	CRect rcText(rcValue);
	rcText.left += XTP_DPI_X(25);

	dc.DrawText(m_strValue, rcText, DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

CRect CXTPPropertyGridItemColor::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.left += XTP_DPI_X(25);
	return rcValue;
}

COLORREF AFX_CDECL CXTPPropertyGridItemColor::StringToRGB(LPCTSTR str)
{
	int nRed   = NextNumber(str);
	int nGreen = NextNumber(str);
	int nBlue  = NextNumber(str);

	return RGB(__min(nRed, 255), __min(nGreen, 255), __min(nBlue, 255));
}

CString AFX_CDECL CXTPPropertyGridItemColor::RGBToString(COLORREF clr)
{
	CString str;
	str.Format(_T("%i; %i; %i"), GetRValue(clr), GetGValue(clr), GetBValue(clr));
	return str;
}

void CXTPPropertyGridItemColor::SetValue(CString strValue)
{
	SetColor(StringToRGB(strValue));
}

void CXTPPropertyGridItemColor::SetColor(COLORREF clr)
{
	m_clrValue = clr;

	if (m_pBindColor)
	{
		*m_pBindColor = clr;
	}

	CXTPPropertyGridItem::SetValue(RGBToString(clr));
}

void CXTPPropertyGridItemColor::BindToColor(COLORREF* pBindColor)
{
	m_pBindColor = pBindColor;
	if (m_pBindColor)
	{
		*m_pBindColor = m_clrValue;
	}
}

void CXTPPropertyGridItemColor::OnBeforeInsert()
{
	if (m_pBindColor && *m_pBindColor != m_clrValue)
	{
		SetColor(*m_pBindColor);
	}
}

void CXTPPropertyGridItemColor::SetEditorStyle(XTPPropertyGridItemColorEditor editor)
{
	m_colorEditor = editor;

	if (m_colorEditor == xtpPropertyGridItemColorPopup)
		SetFlags(xtpPropertyGridItemHasEdit | xtpPropertyGridItemHasComboButton);
	else
		SetFlags(xtpPropertyGridItemHasEdit | xtpPropertyGridItemHasExpandButton);
}

XTPPropertyGridItemColorEditor CXTPPropertyGridItemColor::GetEditorStyle()
{
	return m_colorEditor;
}

void CXTPPropertyGridItemColor::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if (!OnRequestEdit())
		return;

	CWnd* pGrid = m_pGrid;

#ifdef _XTP_INCLUDE_CONTROLS
	if (m_colorEditor == xtpPropertyGridItemColorPopup)
	{
		class CPropertyGridItemColorColorPopup : public CXTPColorPopup
		{
		public:
			CPropertyGridItemColorColorPopup()
				: CXTPColorPopup(TRUE)
			{
				m_pItem = NULL;
			}
			~CPropertyGridItemColorColorPopup()
			{
				CMDTARGET_RELEASE(m_pItem);
			}

			BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
			{
				if (message == CPN_XTP_SELENDOK)
				{
					CString strValue = m_pItem->RGBToString((COLORREF)wParam);
					if (m_pItem->OnAfterEdit(strValue))
					{
						m_pItem->OnValueChanged(strValue);
					}
					*pResult = 0;
					return TRUE;
				}
				if (message == CPN_XTP_SELENDCANCEL)
				{
					m_pItem->OnCancelEdit();
				}
				return CXTPColorPopup::OnWndMsg(message, wParam, lParam, pResult);
			}

			CXTPPropertyGridItemColor* m_pItem;
		};

		CPropertyGridItemColorColorPopup* pColorPopup = new CPropertyGridItemColorColorPopup();
		pColorPopup->SetTheme(xtpControlThemeOfficeXP);

		CRect rcItem = GetItemRect();
		m_pGrid->ClientToScreen(&rcItem);

		COLORREF clrDefault = StringToRGB(m_strDefaultValue);
		pColorPopup->Create(rcItem, m_pGrid,
							CPS_XTP_RIGHTALIGN | CPS_XTP_EXTENDED | CPS_XTP_MORECOLORS
								| CPS_XTP_RECENTCOLORS | CPS_XTP_SHOWEYEDROPPER
								| CPS_XTP_SHOWHEXVALUE,
							GetColor(), clrDefault);
		pColorPopup->SetOwner(m_pGrid);
		pColorPopup->SetFocus();
		pColorPopup->AddListener(pColorPopup->GetSafeHwnd());
		pColorPopup->m_pItem = this;

		InternalAddRef();
	}
	else if (m_colorEditor == xtpPropertyGridItemColorExtendedDialog)
	{
		InternalAddRef();

		CXTPColorDialog dlg(m_clrValue, m_clrValue,
							CPS_XTP_SHOW3DSELECTION | CPS_XTP_SHOWEYEDROPPER, m_pGrid);

		if (dlg.DoModal() == IDOK)
		{
			CString strValue = RGBToString(dlg.GetColor());

			if (OnAfterEdit(strValue))
			{
				OnValueChanged(strValue);
				SAFE_INVALIDATE(pGrid);
			}
		}
		else
		{
			OnCancelEdit();
		}

		InternalRelease();
	}
	else
#endif
	{
		InternalAddRef();

		CColorDialog dlg(m_clrValue, 0, m_pGrid);

		if (dlg.DoModal() == IDOK)
		{
			CString strValue = RGBToString(dlg.GetColor());

			if (OnAfterEdit(strValue))
			{
				OnValueChanged(strValue);
				SAFE_INVALIDATE(pGrid);
			}
		}
		else
		{
			OnCancelEdit();
		}

		InternalRelease();
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPropertyGridItemColor, CXTPPropertyGridItem)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemColor, "EditorStyle", 50, GetEditorStyle,
						SetEditorStyle, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridItemColor, CXTPPropertyGridItem)
	INTERFACE_PART(CXTPPropertyGridItemColor, XTPDIID_IPropertyGridItemColor, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemColor, XTPDIID_IPropertyGridItemColor)

COLORREF TranslateColor(OLE_COLOR clrColor)
{
	COLORREF cr = RGB(0x00, 0x00, 0x00);
	::OleTranslateColor(clrColor, NULL, &cr);
	return cr;
}

#	define CLR_TO_RGBQUAD(clr) (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

void CXTPPropertyGridItemColor::OleSetValue(const VARIANT* varValue)
{
	LONG nColor = VariantToLong(varValue);

	if ((nColor & 0xFF000000) == 0xFF000000)
	{
		nColor = CLR_TO_RGBQUAD(nColor & 0xFFFFFF);
	}

	SetColor(::TranslateColor(nColor));
}

const VARIANT CXTPPropertyGridItemColor::OleGetValue()
{
	return CComVariant((long)m_clrValue);
}

void CXTPPropertyGridItemColor::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_I4, m_clrValue);
	}
}

#endif
