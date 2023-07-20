// XTPMarkupStatic.cpp : implementation file
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

#ifdef _XTP_INCLUDE_MARKUP

#	include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#	include "Common/XTPTypeId.h"
#	include "Common/XTPCasting.h"

#	include "Common/XTPFramework.h"
#	include "Common/XTPSystemHelpers.h"
#	include "Common/XTPSynchro.h"
#	include "Common/XTPApplication.h"
#	include "Common/XTPSingleton.h"
#	include "Common/XTPGdiObjects.h"
#	include "Common/XTPDrawHelpers.h"
#	include "Common/XTPColorManager.h"
#	include "Common/Base/Types/XTPSize.h"

#	include "Markup/XTPMarkupContext.h"
#	include "Markup/XTPMarkupObject.h"
#	include "Markup/XTPMarkupInputElement.h"
#	include "Markup/XTPMarkupUIElement.h"
#	include "Markup/XTPMarkupDrawingContext.h"

#	include "Controls/Static/XTPMarkupStatic.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	ifdef _DEBUG
#		define new DEBUG_NEW
#		undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupStatic

CXTPMarkupStatic::CXTPMarkupStatic()
{
	m_pUIElement	 = NULL;
	m_pMarkupContext = new CXTPMarkupContext();
	m_bIgnoreRTL	 = FALSE;
}

CXTPMarkupStatic::~CXTPMarkupStatic()
{
	MARKUP_RELEASE(m_pUIElement);
	MARKUP_RELEASE(m_pMarkupContext);
}

BEGIN_MESSAGE_MAP(CXTPMarkupStatic, CStatic)
	//{{AFX_MSG_MAP(CXTPMarkupStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupStatic message handlers

void CXTPMarkupStatic::SetMarkupTextEx(LPCSTR lpszMarkup, UINT nCodePage /*= CP_UTF8*/)
{
	MARKUP_RELEASE(m_pUIElement);

	if (lpszMarkup && strchr(lpszMarkup, '%') != NULL)
	{
		CString strCaption;
		GetWindowText(strCaption);

#	ifndef _UNICODE
		UNREFERENCED_PARAMETER(nCodePage);

		CString strMarkup;
		strMarkup.Format(lpszMarkup, (LPCTSTR)strCaption);
		m_pUIElement = m_pMarkupContext->Parse(strMarkup);

#	else /*!_UNICODE*/
		CArray<char, char&> strCaptionA;
		strCaptionA.SetSize(strCaption.GetLength() * 2 + 1);
		int cch = WideCharToMultiByte(nCodePage, 0, strCaption, strCaption.GetLength(),
									  strCaptionA.GetData(),
									  static_cast<int>(strCaptionA.GetSize()), NULL, NULL);
		if (0 < cch)
		{
			strCaptionA[cch] = '\0';

			CArray<char, char&> strMarkup;
			strMarkup.SetSize(XTPToIntPtrChecked(strCaptionA.GetSize() + strlen(lpszMarkup) + 1));
			wsprintfA(strMarkup.GetData(), strCaptionA.GetData());
			m_pUIElement = m_pMarkupContext->Parse(strMarkup.GetData());
		}

#	endif /*_UNICODE*/
	}
	else
	{
		m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);
	}

	if (m_hWnd)
		Invalidate(FALSE);
}

void CXTPMarkupStatic::SetMarkupTextEx(LPCWSTR lpszMarkup, UINT nCodePage /*= CP_UTF8*/)
{
	MARKUP_RELEASE(m_pUIElement);

	if (lpszMarkup && wcschr(lpszMarkup, L'%') != NULL)
	{
		CString strCaption;
		GetWindowText(strCaption);
#	ifdef _UNICODE
		UNREFERENCED_PARAMETER(nCodePage);

		CString strMarkup;
		strMarkup.Format(lpszMarkup, (LPCWSTR)strCaption);
		m_pUIElement = m_pMarkupContext->Parse(strMarkup);

#	else /*_UNICODE*/
		CArray<wchar_t, wchar_t&> strCaptionW;
		strCaptionW.SetSize(strCaption.GetLength() + 1);
		int cch = MultiByteToWideChar(nCodePage, 0, strCaption, strCaption.GetLength(),
									  strCaptionW.GetData(),
									  static_cast<int>(strCaptionW.GetSize()));
		if (0 < cch)
		{
			strCaptionW[cch] = L'\0';

			CArray<wchar_t, wchar_t&> strMarkup;
			strMarkup.SetSize(XTPToIntPtr(strCaptionW.GetSize() + wcslen(lpszMarkup) + 1));
			wsprintfW(strMarkup.GetData(), strCaptionW.GetData());
			m_pUIElement = m_pMarkupContext->Parse(strMarkup.GetData());
		}

#	endif /*!_UNICODE*/
	}
	else
	{
		m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);
	}

	if (m_hWnd)
		Invalidate(FALSE);
}

void CXTPMarkupStatic::SetMarkupText(LPCSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);

	if (m_hWnd)
		Invalidate(FALSE);
}

void CXTPMarkupStatic::SetMarkupText(LPCWSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);

	if (m_hWnd)
		Invalidate(FALSE);
}

void CXTPMarkupStatic::OnPaint()
{
	CPaintDC dcPaint(this);

	BOOL bRTL = XTPDrawHelpers()->IsContextRTL(dcPaint);
	if (bRTL && m_bIgnoreRTL)
		XTPDrawHelpers()->SetContextRTL(dcPaint, !bRTL);

	CXTPBufferDC dcBuffer(dcPaint);

	CXTPClientRect rc(this);

	HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC,
													 (WPARAM)dcBuffer.GetSafeHdc(), (LPARAM)m_hWnd);
	if (hBrush)
	{
		::FillRect(dcBuffer.GetSafeHdc(), rc, hBrush);
	}
	else
	{
		dcBuffer.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}

	if (m_pUIElement)
	{
		CXTPMarkupDrawingContext dc(dcBuffer);

		m_pUIElement->Measure(&dc, rc.Size());

		m_pUIElement->Arrange(rc);

		m_pUIElement->Render(&dc);
	}
}

BOOL CXTPMarkupStatic::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (!m_pMarkupContext->IsDpiAware())
	{
		m_pMarkupContext->SetDpiAware(TRUE, *this);
	}

	// To Handle Hyperlinks:
	if (m_pUIElement)
	{
		m_pMarkupContext->m_hContextWnd = m_hWnd;
		if (m_pMarkupContext->OnWndMsg(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	return CStatic::OnWndMsg(message, wParam, lParam, pResult);
}

#endif
