// XTPCalendarViewPart.cpp : implementation file
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
#include "Common/XTPVC80Helpers.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarPaintManager.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarTimeLineView.h"
#include "Calendar/XTPCalendarTimeLineViewTimeScale.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPCalendarViewPartBrushValue

CXTPCalendarViewPartBrushValue::~CXTPCalendarViewPartBrushValue()
{
	DeleteStandardValue();
	DeleteCustomValue();
}

void CXTPCalendarViewPartBrushValue::DeleteStandardValue()
{
	if (m_bAutoDelete_Standard && NULL != m_pValueStandard)
	{
		m_pValueStandard->DeleteObject();
	}

	CXTPCalendarViewPartCustomizableValueT<CBrush>::DeleteStandardValue();
}

void CXTPCalendarViewPartBrushValue::DeleteCustomValue()
{
	if (m_bAutoDelete_Custom && NULL != m_pValueCustom)
	{
		m_pValueCustom->DeleteObject();
	}

	CXTPCalendarViewPartCustomizableValueT<CBrush>::DeleteCustomValue();
}

///////////////////////////////////////////////////////////////////////////////
// CXTPCalendarViewPartFontValue

CXTPCalendarViewPartFontValue::CXTPCalendarViewPartFontValue()
{
}

BOOL CXTPCalendarViewPartFontValue::IsDefaultValue()
{
	return (m_xtpFontCustomValue.GetSafeHandle() == NULL)
		   && (m_xtpFontStandardValue.GetSafeHandle() == NULL);
}

void CXTPCalendarViewPartFontValue::SetStandardValue(LOGFONT* pLogFont)
{
	m_xtpFontStandardValue.DeleteObject();
	m_xtpFontStandardValue.CreateFontIndirect(pLogFont);
}

void CXTPCalendarViewPartFontValue::SetDefaultValue()
{
	m_xtpFontCustomValue.DeleteObject();
}

CXTPCalendarViewPartFontValue::operator CFont&()
{
	return (m_xtpFontCustomValue.GetSafeHandle() == 0) ? m_xtpFontStandardValue
													   : m_xtpFontCustomValue;
}

CXTPCalendarViewPartFontValue::operator CFont*()
{
	return (m_xtpFontCustomValue.GetSafeHandle() == 0) ? &m_xtpFontStandardValue
													   : &m_xtpFontCustomValue;
}

const CXTPCalendarViewPartFontValue& CXTPCalendarViewPartFontValue::operator=(LOGFONT* pLogFont)
{
	m_xtpFontCustomValue.DeleteObject();
	if (pLogFont)
		m_xtpFontCustomValue.CreateFontIndirect(pLogFont);
	return *this;
}

CFont* CXTPCalendarViewPartFontValue::operator->()
{
	return (m_xtpFontCustomValue.GetSafeHandle() == 0) ? &m_xtpFontStandardValue
													   : &m_xtpFontCustomValue;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarViewPart

CXTPCalendarViewPart::CXTPCalendarViewPart(CXTPCalendarViewPart* pParentPart)
	: m_pParentPart(pParentPart)
{
	m_pPaintManager = NULL;
}

CXTPCalendarViewPart::~CXTPCalendarViewPart()
{
}

CXTPCalendarControl* CXTPCalendarViewPart::GetCalendarControl() const
{
	return XTP_SAFE_GET1(m_pPaintManager, m_pControl, NULL);
}

void CXTPCalendarViewPart::RefreshMetrics()
{
}

COLORREF CXTPCalendarViewPart::GetBackgroundColor()
{
	return m_pPaintManager->PartGetBackColor(this);
}

COLORREF CXTPCalendarViewPart::GetTextColor()
{
	return m_pPaintManager->PartGetTextColor(this);
}

CFont& CXTPCalendarViewPart::GetTextFont()
{
	return m_pPaintManager->PartGetTextFont(this, m_fntText);
}

void CXTPCalendarViewPart::DrawText(CDC* pDC, const CString& str, LPRECT lpRect, UINT nFormat,
									COLORREF clrText /*= COLORREF_NULL*/, CFont* pFont /*= NULL*/)
{
	CXTPDCSaveState dcState(pDC);

	if (!pFont->GetSafeHandle())
	{
		pFont = &GetTextFont();
	}
	pDC->SelectObject(pFont);

	if (clrText == COLORREF_NULL)
	{
		clrText = GetTextColor();
	}
	pDC->SetTextColor(clrText);

	nFormat |= DT_NOPREFIX;
	pDC->DrawText(str, lpRect, nFormat);
}

void CXTPCalendarViewPart::DrawLine_CenterLR(CDC* pDC, const CString& str, LPRECT lpRect,
											 UINT nFormat)
{
	CFont* pOldFont = pDC->SelectObject(&GetTextFont());
	pDC->SetTextColor(GetTextColor());
	nFormat |= DT_NOPREFIX | DT_SINGLELINE;

	UINT nLeftRight = nFormat & (DT_LEFT | DT_RIGHT);
	nFormat &= ~(DT_CENTER | DT_LEFT | DT_RIGHT);

	CSize sz = pDC->GetTextExtent(str);
	if (sz.cx < labs(lpRect->right - lpRect->left))
	{
		nFormat |= DT_CENTER;
	}
	else
	{
		nFormat |= nLeftRight ? nLeftRight : DT_LEFT;
	}

	pDC->DrawText(str, lpRect, nFormat);

	pDC->SelectObject(pOldFont);
}

void CXTPCalendarViewPart::DrawLineEx(CDC* pDC, const CString& str, LPRECT lpRect,
									  UINT nFormatNormal, UINT nFormatSmall)
{
	CFont* pOldFont = pDC->SelectObject(&GetTextFont());
	pDC->SetTextColor(GetTextColor());
	UINT nFormat = 0;

	CSize sz = pDC->GetTextExtent(str);
	if (sz.cx < labs(lpRect->right - lpRect->left))
	{
		nFormat = nFormatNormal | DT_NOPREFIX | DT_SINGLELINE;
	}
	else
	{
		nFormat = nFormatSmall | DT_NOPREFIX | DT_SINGLELINE;
	}

	pDC->DrawText(str, lpRect, nFormat);

	pDC->SelectObject(pOldFont);
}

CSize CXTPCalendarViewPart::GetTextExtent(CDC* pDC, const CString& str)
{
	CFont* pOldFont = pDC->SelectObject(&GetTextFont());
	CSize sz		= pDC->GetTextExtent(str);
	pDC->SelectObject(pOldFont);
	return sz;
}

void CXTPCalendarViewPart::SetBackgroundColor(COLORREF clr)
{
	m_clrBackground = clr;
}

void CXTPCalendarViewPart::SetTextColor(COLORREF clr)
{
	m_clrTextColor = clr;
}

void CXTPCalendarViewPart::SetTextFont(LOGFONT* pLogFont)
{
	m_fntText = pLogFont;
}

BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pcszPropName, LOGFONT& rLogFont)
{
	CXTPPropExchangeSection secLF(pPX->GetSection(pcszPropName));

	if (pPX->IsLoading())
	{
		::ZeroMemory(&rLogFont, sizeof(rLogFont));
	}
	else
	{
		secLF->EmptySection();
	}

	long nVersion = 1;
	PX_Long(&secLF, _T("Version"), nVersion, 1);

	if (nVersion != 1 && pPX->IsLoading())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	PX_Long(&secLF, _T("Height"), rLogFont.lfHeight, 0);
	PX_Long(&secLF, _T("Width"), rLogFont.lfWidth, 0);
	PX_Long(&secLF, _T("Escapement"), rLogFont.lfEscapement, 0);
	PX_Long(&secLF, _T("Orientation"), rLogFont.lfOrientation, 0);
	PX_Long(&secLF, _T("Weight"), rLogFont.lfWeight, 0);

	PX_Byte(&secLF, _T("Italic"), rLogFont.lfItalic, 0);
	PX_Byte(&secLF, _T("Underline"), rLogFont.lfUnderline, 0);
	PX_Byte(&secLF, _T("StrikeOut"), rLogFont.lfStrikeOut, 0);
	PX_Byte(&secLF, _T("CharSet"), rLogFont.lfCharSet, 0);
	PX_Byte(&secLF, _T("OutPrecision"), rLogFont.lfOutPrecision, 0);
	PX_Byte(&secLF, _T("ClipPrecision"), rLogFont.lfClipPrecision, 0);
	PX_Byte(&secLF, _T("Quality"), rLogFont.lfQuality, 0);
	PX_Byte(&secLF, _T("PitchAndFamily"), rLogFont.lfPitchAndFamily, 0);

	CString strFaceName;
	if (pPX->IsStoring())
	{
		strFaceName = rLogFont.lfFaceName;
		PX_String(&secLF, _T("FaceName"), strFaceName);
	}
	else
	{
		PX_String(&secLF, _T("FaceName"), strFaceName);
		STRCPY_S(rLogFont.lfFaceName, LF_FACESIZE, strFaceName);
	}

	return TRUE;
}

BOOL AFX_CDECL PX_Font(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
					   CXTPCalendarThemeFontValue& refFont)
{
	CFont* pFontStandard = refFont.GetStandardValue();
	CFont* pFontCustom   = refFont.IsCustomValue() ? refFont.GetValue() : NULL;

	LOGFONT lfStandard0, lfCustom0;

	if (!pFontStandard || !pFontStandard->GetLogFont(&lfStandard0))
	{
		::ZeroMemory(&lfStandard0, sizeof(lfStandard0));
	}

	if (!pFontCustom || !pFontCustom->GetLogFont(&lfCustom0))
	{
		::ZeroMemory(&lfCustom0, sizeof(lfCustom0));
	}

	//-------------------------------------------------------------------
	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
	{
		secData->EmptySection();

		PX_Font(&secData, _T("Standard"), lfStandard0);
		PX_Font(&secData, _T("Custom"), lfCustom0);
	}
	else
	{
		ASSERT(pPX->IsLoading());

		LOGFONT lfStandard, lfCustom;

		PX_Font(&secData, _T("Standard"), lfStandard);
		PX_Font(&secData, _T("Custom"), lfCustom);

		if (memcmp(&lfStandard, &lfStandard0, sizeof(lfStandard)))
		{
			if ((lfStandard.lfHeight || lfStandard.lfWidth || lfStandard.lfFaceName[0]))
				refFont.SetStandardValue(&lfStandard);
			else
				refFont.SetStandardValue((CFont*)NULL);
		}

		if (memcmp(&lfCustom, &lfCustom0, sizeof(lfCustom)))
		{
			if (lfCustom.lfHeight || lfCustom.lfWidth || lfCustom.lfFaceName[0])
				refFont.SetCustomValue(&lfCustom);
			else
				refFont.SetDefaultValue();
		}
	}
	return TRUE;
}

BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pcszPropName, COLORREF& refColor)
{
	ASSERT(pPX && pcszPropName);
	if (!pPX || !pcszPropName)
		return FALSE;

	CString strColor;

	// save/load color as hex number
	if (pPX->IsStoring())
	{
		if (refColor == COLORREF_NULL)
			strColor = _T("-1");
		else
			strColor.Format(_T("%06X"), refColor);

		return PX_String(pPX, pcszPropName, strColor);
	}
	else
	{
		PX_String(pPX, pcszPropName, strColor, _T("-1"));

		strColor.Remove(_T(' '));

		if (strColor == _T("-1"))
		{
			refColor = COLORREF_NULL;
		}
		else
		{
			TCHAR* pEnd = NULL;
			refColor	= (COLORREF)_tcstol(strColor, &pEnd, 16);
		}
	}

	return TRUE;
}

BOOL AFX_CDECL PX_Color(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
						CXTPPaintManagerColor& refColor)
{
	class CXTPPaintManagerColor_accesser : public CXTPPaintManagerColor
	{
	public:
		void doPX(CXTPPropExchange* pPX, LPCTSTR pcszPropName)
		{
			CXTPPropExchangeSection secCLR(pPX->GetSection(pcszPropName));

			if (pPX->IsStoring())
				secCLR->EmptySection();

			PX_Color(&secCLR, _T("Standard"), m_clrStandardValue);
			PX_Color(&secCLR, _T("Custom"), m_clrCustomValue);
		}
	};

	CXTPPaintManagerColor_accesser* pClrEx = (CXTPPaintManagerColor_accesser*)&refColor;
	pClrEx->doPX(pPX, pcszPropName);

	return TRUE;
}

BOOL AFX_CDECL PX_GrColor(CXTPPropExchange* pPX, LPCTSTR psczPropName,
						  CXTPPaintManagerColorGradient& refGrColor)
{
	CXTPPropExchangeSection secData(pPX->GetSection(psczPropName));

	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("LightColor"), refGrColor.clrLight);
	PX_Color(&secData, _T("DarkColor"), refGrColor.clrDark);
	PX_Float(&secData, _T("GradientFactor"), refGrColor.fGradientFactor, 0.5);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPCalendarThemeFontValue

CXTPCalendarThemeFontValue::CXTPCalendarThemeFontValue()
{
}

CXTPCalendarThemeFontValue::~CXTPCalendarThemeFontValue()
{
	DeleteStandardValue();
	DeleteCustomValue();
}

void CXTPCalendarThemeFontValue::SetStandardValue(LOGFONT* pLogFont)
{
	CFont* pFont = new CFont();
	if (pFont)
	{
		VERIFY(pFont->CreateFontIndirect(pLogFont));
	}

	TBase::SetStandardValue(pFont, TRUE);
}

void CXTPCalendarThemeFontValue::SetCustomValue(LOGFONT* pLogFont)
{
	CFont* pFont = new CFont();
	if (pFont)
	{
		VERIFY(pFont->CreateFontIndirect(pLogFont));
	}

	TBase::SetCustomValue(pFont, TRUE);
}

void CXTPCalendarThemeFontValue::SetStandardValue(CFont* pFont)
{
	if (!pFont)
	{
		TBase::SetStandardValue(pFont, FALSE);
		return;
	}

	LOGFONT lfFont;
	int nRes = pFont->GetLogFont(&lfFont);
	ASSERT(nRes);
	if (nRes)
	{
		SetStandardValue(&lfFont);
	}
}

void CXTPCalendarThemeFontValue::SetCustomValue(CFont* pFont)
{
	if (!pFont)
	{
		TBase::SetStandardValue(pFont, FALSE);
		return;
	}

	LOGFONT lfFont;
	int nRes = pFont->GetLogFont(&lfFont);
	ASSERT(nRes);
	if (nRes)
	{
		SetCustomValue(&lfFont);
	}
}

const CXTPCalendarThemeFontValue& CXTPCalendarThemeFontValue::operator=(LOGFONT& rLogFont)
{
	SetCustomValue(&rLogFont);
	return *this;
}

void CXTPCalendarThemeFontValue::CopySettings(const CXTPCalendarThemeFontValue& refSrc)
{
	SetStandardValue(refSrc.GetValue());
}

void CXTPCalendarThemeFontValue::Serialize(CArchive& ar)
{
	LOGFONT lfStandard0, lfCustom0;

	const UINT cnLogFont_size = sizeof(LOGFONT);

	if (!m_pValueStandard || !m_pValueStandard->GetLogFont(&lfStandard0))
	{
		::ZeroMemory(&lfStandard0, cnLogFont_size);
	}

	if (!m_pValueCustom || !m_pValueCustom->GetLogFont(&lfCustom0))
	{
		::ZeroMemory(&lfCustom0, cnLogFont_size);
	}

	if (ar.IsStoring())
	{
		ar.Write((void*)&lfStandard0, cnLogFont_size);
		ar.Write((void*)&lfCustom0, cnLogFont_size);
	}
	else
	{
		ASSERT(ar.IsLoading());

		LOGFONT lfStandard, lfCustom;

		ar.Read((void*)&lfStandard, cnLogFont_size);
		ar.Read((void*)&lfCustom, cnLogFont_size);

		if (memcmp(&lfStandard, &lfStandard0, cnLogFont_size))
		{
			if ((lfStandard.lfHeight || lfStandard.lfWidth || lfStandard.lfFaceName[0]))
				SetStandardValue(&lfStandard);
			else
				SetStandardValue((CFont*)NULL);
		}

		if (memcmp(&lfCustom, &lfCustom0, cnLogFont_size))
		{
			if (lfCustom.lfHeight || lfCustom.lfWidth || lfCustom.lfFaceName[0])
				SetCustomValue(&lfCustom);
			else
				SetDefaultValue();
		}
	}
}

void CXTPCalendarThemeFontValue::DeleteStandardValue()
{
	if (m_bAutoDelete_Standard && NULL != m_pValueStandard)
	{
		m_pValueStandard->DeleteObject();
	}

	CXTPCalendarViewPartCustomizableValueT<CFont>::DeleteStandardValue();
}

void CXTPCalendarThemeFontValue::DeleteCustomValue()
{
	if (m_bAutoDelete_Custom && NULL != m_pValueCustom)
	{
		m_pValueCustom->DeleteObject();
	}

	CXTPCalendarViewPartCustomizableValueT<CFont>::DeleteCustomValue();
}

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarThemeStringValue

CXTPCalendarThemeStringValue::CXTPCalendarThemeStringValue()
{
	m_bAutoDestroy_Standard = m_bAutoDestroy_Custom = TRUE;
}

const CXTPCalendarThemeStringValue& CXTPCalendarThemeStringValue::operator=(LPCTSTR pcszValue)
{
	SetCustomValue(pcszValue);
	return *this;
}

void CXTPCalendarThemeStringValue::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName)
{
	if (!pPX || !pcszPropName)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Bool(&secData, _T("IsStandardSet"), m_bIsStandardSet);
	PX_Bool(&secData, _T("IsCustomSet"), m_bIsCustomSet);

	DoPX_Value(&secData, _T("Standard"), m_ValueStandard, TRUE);
	DoPX_Value(&secData, _T("Custom"), m_ValueCustom, FALSE);
}

void CXTPCalendarThemeStringValue::DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											  CString& rXValue, BOOL bStandard)
{
	if (pPX->IsStoring())
	{
		if (bStandard && !m_bIsStandardSet || !bStandard && !m_bIsCustomSet)
		{
			rXValue.Empty();
		}
	}
	PX_String(pPX, pcszPropName, rXValue);
}

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarThemeIntValue

CXTPCalendarThemeIntValue::CXTPCalendarThemeIntValue()
{
	m_ValueStandard = m_ValueCustom = 0;
}

void CXTPCalendarThemeIntValue::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName)
{
	if (!pPX || !pcszPropName)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Bool(&secData, _T("IsStandardSet"), m_bIsStandardSet);
	PX_Bool(&secData, _T("IsCustomSet"), m_bIsCustomSet);

	DoPX_Value(&secData, _T("Standard"), m_ValueStandard, TRUE);
	DoPX_Value(&secData, _T("Custom"), m_ValueCustom, FALSE);
}

void CXTPCalendarThemeIntValue::DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
										   int& rXValue, BOOL bStandard)
{
	if (pPX->IsStoring())
	{
		if (bStandard && !m_bIsStandardSet || !bStandard && !m_bIsCustomSet)
		{
			rXValue = 0;
		}
	}
	PX_Int(pPX, pcszPropName, rXValue);
}

const CXTPCalendarThemeIntValue& CXTPCalendarThemeIntValue::operator=(int nValue)
{
	SetCustomValue(nValue);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarThemeBOOLValue

CXTPCalendarThemeBOOLValue::CXTPCalendarThemeBOOLValue()
{
	m_ValueStandard = m_ValueCustom = FALSE;
}

void CXTPCalendarThemeBOOLValue::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName)
{
	if (!pPX || !pcszPropName)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Bool(&secData, _T("IsStandardSet"), m_bIsStandardSet);
	PX_Bool(&secData, _T("IsCustomSet"), m_bIsCustomSet);

	DoPX_Value(&secData, _T("Standard"), m_ValueStandard, TRUE);
	DoPX_Value(&secData, _T("Custom"), m_ValueCustom, FALSE);
}

void CXTPCalendarThemeBOOLValue::DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											BOOL& rXValue, BOOL bStandard)
{
	if (pPX->IsStoring())
	{
		if (bStandard && !m_bIsStandardSet || !bStandard && !m_bIsCustomSet)
		{
			rXValue = FALSE;
		}
	}
	PX_Bool(pPX, pcszPropName, rXValue);
}

const CXTPCalendarThemeBOOLValue& CXTPCalendarThemeBOOLValue::operator=(BOOL bValue)
{
	SetCustomValue(bValue);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarThemeRectValue

CXTPCalendarThemeRectValue::CXTPCalendarThemeRectValue()
{
	m_ValueStandard = m_ValueCustom = CRect(0, 0, 0, 0);
}

void CXTPCalendarThemeRectValue::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszPropName)
{
	if (!pPX || !pcszPropName)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Bool(&secData, _T("IsStandardSet"), m_bIsStandardSet);
	PX_Bool(&secData, _T("IsCustomSet"), m_bIsCustomSet);

	DoPX_Value(&secData, _T("Standard"), m_ValueStandard, TRUE);
	DoPX_Value(&secData, _T("Custom"), m_ValueCustom, FALSE);
}

void CXTPCalendarThemeRectValue::DoPX_Value(CXTPPropExchange* pPX, LPCTSTR pcszPropName,
											CRect& rXValue, BOOL bStandard)
{
	const CRect crcZero(0, 0, 0, 0);
	if (pPX->IsStoring())
	{
		if (bStandard && !m_bIsStandardSet || !bStandard && !m_bIsCustomSet)
		{
			rXValue = crcZero;
		}
	}

	PX_Rect(pPX, pcszPropName, rXValue, crcZero);
}

const CXTPCalendarThemeRectValue& CXTPCalendarThemeRectValue::operator=(const CRect& rcValue)
{
	SetCustomValue((CRect&)rcValue);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// CXTPCalendarTimeLineViewTimeScalePart

CXTPCalendarTimeLineViewTimeScalePart::CXTPCalendarTimeLineViewTimeScalePart(
	CXTPCalendarViewPart* pParentPart)
	: CXTPCalendarViewPart(pParentPart)
{
	m_nHeaderHeight = 0;
}

void CXTPCalendarTimeLineViewTimeScalePart::RefreshMetrics()
{
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfHeight = -XTP_DPI_Y(16) * XTP_DPI_Y(11) / XTP_DPI_Y(8);
	m_fntScaleHeaderText.SetStandardValue(&lfIcon);
}

int CXTPCalendarTimeLineViewTimeScalePart::CalcHeigt(CDC* pDC)
{
	CXTPFontDC fnt(pDC, m_fntScaleHeaderText);
	CSize sizeText = pDC->GetTextExtent(_T("W1"));

	m_nHeaderHeight = sizeText.cy * XTP_DPI_Y(38) / XTP_DPI_Y(27);

	return m_nHeaderHeight + XTP_DPI_Y(30);
}

void CXTPCalendarTimeLineViewTimeScalePart::Draw(CDC* pDC, const CRect& rcRect,
												 CXTPCalendarTimeLineView* pView)
{
	if (!pView)
	{
		ASSERT(FALSE);
		return;
	}
	CRect rcHeader  = rcRect;
	rcHeader.bottom = rcHeader.top + m_nHeaderHeight;

	pDC->FillSolidRect(&rcHeader, pView->m_clrTimeScaleBackground);
	CRect rcBorder = rcHeader;
	rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);

	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);
}

CString CXTPCalendarTimeLineViewTimeScalePart::GetHeaderDateFormat(int nLabelInterval)
{
	// LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day
	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_ILDATE);
	CString strFormat = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE);
	CString strD = _T("dd"), strM = _T("M"), strY = _T("Y");

	if (strFormat.Find(_T("dd")) < 0)
		strD = _T("d");

	if (strFormat.Find(_T("MMMM")) >= 0)
		strM = _T("MMMM");
	else if (strFormat.Find(_T("MMM")) >= 0)
		strM = _T("MMM");
	else if (strFormat.Find(_T("MM")) >= 0)
		strM = _T("MM");

	if (strFormat.Find(_T("yyyy")) >= 0)
		strY = _T("yyyy");
	else if (strFormat.Find(_T("yy")) >= 0)
		strY = _T("yy");

	if (nLabelInterval == xtpDTScale_Day)
	{
		if (nDateOrdering == 0)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strM, (LPCTSTR)strD, (LPCTSTR)strY);
		else if (nDateOrdering == 1)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strD, (LPCTSTR)strM, (LPCTSTR)strY);
		else // if (nDateOrdering == 2)
			strFormat.Format(_T("%s %s %s"), (LPCTSTR)strY, (LPCTSTR)strM, (LPCTSTR)strD);
	}
	else if (nLabelInterval == xtpDTScale_Month)
	{
		if (nDateOrdering == 2)
			strFormat.Format(_T("%s %s"), (LPCTSTR)strY, (LPCTSTR)strM);
		else
			strFormat.Format(_T("%s %s"), (LPCTSTR)strM, (LPCTSTR)strY);
	}

	return strFormat;
}

void CXTPCalendarTimeLineViewTimeScalePart::DrawHeader(CDC* pDC, const CRect& rcRect,
													   CXTPCalendarTimeLineView* pView,
													   int nLabelInterval)
{
	if (!XTP_SAFE_GET2(pView, GetTimeScale(), GetTimeScaleParams(), NULL))
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTimeLineViewTimeScale* pTimeScale = pView->GetTimeScale();
	CXTPCalendarTLV_TimeScaleParams* pTSParams	= pTimeScale->GetTimeScaleParams();

	CRect rcHeader  = rcRect;
	rcHeader.bottom = rcHeader.top + m_nHeaderHeight;

	CRect rcBorder = rcHeader;
	rcBorder.top   = rcBorder.bottom - XTP_DPI_Y(1);

	pDC->FillSolidRect(&rcHeader, pView->m_clrTimeScaleBackground);
	pDC->FillSolidRect(&rcBorder, pView->m_clrTimeScaleBorder);

	pDC->SetBkMode(TRANSPARENT);

	COleDateTime dtStart = pTimeScale->GetStartViewDate();

	double dTimePerPixel = (double)pTSParams->GetTimePerPixel();

	CXTPFontDC autoFont(pDC, m_fntScaleHeaderText, pView->m_clrTimeScaleText);

	CString strFormat = GetHeaderDateFormat(nLabelInterval);
	COleDateTimeSpan spDay(1, 0, 0, 0);

	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStart);
	for (int nP = 0; nP < 100; nP++)
	{
		COleDateTime dtDayNext = dtDay;
		if (nLabelInterval == xtpDTScale_Month)
		{
			CXTPCalendarUtils::ShiftDate_Month(dtDayNext, 1);
		}
		else
		{
			ASSERT(nLabelInterval == xtpDTScale_Day);
			dtDayNext = dtDay + spDay;
		}

		double dDay_tshift = double(dtDay - dtStart);
		int nDay_xshift	= int(dDay_tshift / dTimePerPixel);

		if (nDay_xshift > rcHeader.Width())
			break;

		double dNextDay_tshift = double(dtDayNext - dtStart);
		int nNextDay_xshift	= int(dNextDay_tshift / dTimePerPixel);

		CRect rcDayLabel = rcHeader;
		rcDayLabel.left += XTP_DPI_X(5);
		rcDayLabel.right = rcHeader.left + nNextDay_xshift - XTP_DPI_X(15);

		if (nDay_xshift > 0)
			rcDayLabel.left += nDay_xshift;

		CString strLabel = CXTPCalendarUtils::GetDateFormat(dtDay, strFormat);
		CSize szLabel	= pDC->GetTextExtent(strLabel);

		UINT nFormat = DT_VCENTER | DT_SINGLELINE;

		CPoint ptTR(rcDayLabel.left + szLabel.cx + XTP_DPI_X(4), rcDayLabel.bottom - XTP_DPI_Y(17));

		if (szLabel.cx >= rcDayLabel.Width())
		{
			nFormat |= DT_RIGHT;
			rcDayLabel.left = rcHeader.left;
			ptTR.x			= rcDayLabel.right;
		}

		pDC->DrawText(strLabel, &rcDayLabel, nFormat);

		CRect rcTrangleRect(ptTR.x, ptTR.y, ptTR.x + XTP_DPI_X(17), ptTR.y + XTP_DPI_Y(17));
		XTPPrimitiveDrawer()->DrawSymbol(pDC, xtpPrimitiveSymbolPlayDown, rcTrangleRect,
										 pView->m_clrTimeScaleText);

		dtDay = dtDayNext;
	}
}
