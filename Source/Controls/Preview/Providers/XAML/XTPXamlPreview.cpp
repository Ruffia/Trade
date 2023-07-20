// XTPXamlPreview.cpp : implementation of the CXTPXamlPreview class.
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
#include <atlbase.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/Xaml/XTPXamlPreview.h"
#include "Controls/Resource.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupContext.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <comdef.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPXamlPreview::CContext : public CXTPMarkupContext
{
};

CXTPXamlPreview::CXTPXamlPreview(IXTPPreviewHost& host, LPCTSTR pFilePath)
	: m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_pMarkupRoot(NULL)
	, m_hwnd(NULL)
	, m_host(host)
{
	CComPtr<IStream> stream;
	if (FAILED(XTPCreateReadOnlyFileStream(pFilePath, &stream)))
	{
		TRACE(_T("Unable to create a stream on file\n"));
		return;
	}

	m_bLoadFinished = TRUE;

	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);
	if (NULL == m_pMarkupContext)
	{
		TRACE(_T("Unable to create markup context\n"));
		return;
	}

	m_pMarkupRoot = XTPMarkupParseText(m_pMarkupContext, stream);
	if (NULL == m_pMarkupRoot)
	{
		TRACE(_T("Unable to parse markup code\n"));
		XTPMarkupReleaseContext(m_pMarkupContext);
		m_pMarkupContext = NULL;
		return;
	}

	m_bLoadSuccess = TRUE;
}

CXTPXamlPreview::CXTPXamlPreview(IXTPPreviewHost& host, IStream& stream, LPCTSTR pImpliedFileName)
	: m_bLoadFinished(TRUE)
	, m_bLoadSuccess(FALSE)
	, m_pMarkupRoot(NULL)
	, m_hwnd(NULL)
	, m_host(host)
{
	UNREFERENCED_PARAMETER(pImpliedFileName);

	m_pMarkupContext = XTPMarkupCreateContext(NULL, TRUE);
	if (NULL == m_pMarkupContext)
	{
		TRACE(_T("Unable to create markup context\n"));
		return;
	}

	m_pMarkupRoot = XTPMarkupParseText(m_pMarkupContext, &stream);
	if (NULL == m_pMarkupRoot)
	{
		TRACE(_T("Unable to parse markup code\n"));
		XTPMarkupReleaseContext(m_pMarkupContext);
		m_pMarkupContext = NULL;
		return;
	}

	m_bLoadSuccess = TRUE;
}

CXTPXamlPreview::~CXTPXamlPreview()
{
	if (NULL != m_pMarkupRoot)
		XTPMarkupReleaseElement(m_pMarkupRoot);

	if (NULL != m_pMarkupContext)
		delete static_cast<CContext*>(m_pMarkupContext);
}

BOOL CXTPXamlPreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPXamlPreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPXamlPreview::HasOwnNavigator() const
{
	return FALSE;
}

UINT CXTPXamlPreview::GetNumberOfPages() const
{
	return 0;
}

SIZE CXTPXamlPreview::GetPreferredPageSize() const
{
	CSize maxSize(INT_MAX, INT_MAX);

	if (::IsWindow(m_hwnd))
	{
		CXTPClientRect rc(m_hwnd);
		maxSize = rc.Size();
	}

	ASSERT(NULL != m_pMarkupRoot);
	return XTPMarkupMeasureElement(m_pMarkupRoot, maxSize.cx, maxSize.cy);
}

void CXTPXamlPreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPXamlPreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	ASSERT(NULL != m_pMarkupRoot);
	UNREFERENCED_PARAMETER(updateRect);

	if (!m_bLoadSuccess)
		return;

	if (0 == clientSize.cx || 0 == clientSize.cy)
		return;

	CXTPBufferDC memDC(dc, updateRect);

	COLORREF crBack = m_host.GetBackgroundColor();
	if (COLORREF_NULL == crBack)
	{
		crBack = GetXtremeColor(COLOR_WINDOW);
	}

	memDC.FillSolidRect(&updateRect, crBack);

	XTPMarkupRenderElement(m_pMarkupRoot, memDC, &updateRect);
}

BOOL CXTPXamlPreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	ASSERT(NULL != m_pMarkupRoot);

	if (NULL == m_hwnd)
	{
		m_hwnd = msg->hwnd;
		XTPMarkupAssignHandle(m_pMarkupContext, m_hwnd);
	}

	return XTPMarkupRelayMessage(m_pMarkupRoot, msg->message, msg->wParam, msg->lParam, result);
}

BOOL CXTPXamlPreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPXamlPreview::Activate()
{
	// Does not support activation
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
