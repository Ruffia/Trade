// XTPGifPreview.cpp : implementation of the CXTPGifPreview class.
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

#include "Common/XTPCasting.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/GIF/XTPGifPreview.h"
#include "Controls/Preview/Providers/GIF/XTPGifPreviewProvider.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <comdef.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

CXTPGifPreview::CXTPGifPreview(HWND parent)
	: m_pPicture(NULL)
	, m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_gdiPlusToken(0L)
	, m_pHostWindow(NULL)
	, m_nTimerId(1)
	, m_bIsPlaying(FALSE)
	, m_FrameCount(0)
	, m_iCurrentFrame(0)
	, m_pDimensionIDs(NULL)
	, m_pDelayProperty(NULL)
{
	m_pHostWindow = CWnd::FromHandlePermanent(parent);
	ASSERT_VALID(m_pHostWindow);
}

CXTPGifPreview::~CXTPGifPreview()
{
	if (m_bIsPlaying && IsWindow(m_pHostWindow->GetSafeHwnd()))
	{
		m_pHostWindow->KillTimer(m_nTimerId);
		m_bIsPlaying = FALSE;
	}

	if (NULL != m_pDimensionIDs)
	{
		delete[] m_pDimensionIDs;
		m_pDimensionIDs = NULL;
	}

	if (NULL != m_pDelayProperty)
	{
		free(m_pDelayProperty);
		m_pDelayProperty = NULL;
	}

	if (NULL != m_pPicture)
	{
		delete m_pPicture;
		m_pPicture = NULL;
	}
}

BOOL CXTPGifPreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPGifPreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPGifPreview::HasOwnNavigator() const
{
	return FALSE;
}

UINT CXTPGifPreview::GetNumberOfPages() const
{
	return UINT(m_bLoadSuccess ? 1 : 0);
}

SIZE CXTPGifPreview::GetPreferredPageSize() const
{
	CSize size(0, 0);
	if (NULL != m_pPicture)
	{
		size.cx = XTPToInt(m_pPicture->GetWidth());
		size.cy = XTPToInt(m_pPicture->GetHeight());
	}

	return size;
}

void CXTPGifPreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPGifPreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	UNREFERENCED_PARAMETER(updateRect);

	if (!m_bLoadSuccess)
		return;

	if ((clientSize.cx == 0) || (clientSize.cy == 0) || (m_pPicture->GetWidth() == 0)
		|| (m_pPicture->GetHeight() == 0))
		return;

	int outX = XTPToInt(m_pPicture->GetWidth());
	int outY = XTPToInt(m_pPicture->GetHeight());
	if ((outX > clientSize.cx) || (outY > clientSize.cy))
	{
		double rX = ((double)m_pPicture->GetWidth()) / ((double)clientSize.cx);
		double rY = ((double)m_pPicture->GetHeight()) / ((double)clientSize.cy);
		double r  = rX;
		if (rY > rX)
		{
			r = rY;
		}

		rX   = ((double)m_pPicture->GetWidth()) / r + 0.5;
		outX = (int)rX;
		if (outX > clientSize.cx)
			outX = clientSize.cx;

		rY   = ((double)m_pPicture->GetHeight()) / r + 0.5;
		outY = (int)rY;
		if (outY > clientSize.cy)
			outY = clientSize.cy;
	}

	Gdiplus::Graphics hostGraphics(dc);
	int x = (clientSize.cx - outX) / 2;
	int y = (clientSize.cy - outY) / 2;
	hostGraphics.DrawImage(m_pPicture, x, y, outX, outY);

	if ((m_FrameCount == 1) || m_bIsPlaying)
		return;

	UINT delay   = ((PUINT)m_pDelayProperty->value)[m_iCurrentFrame] * 10;
	m_nTimerId   = m_pHostWindow->SetTimer(1, delay, NULL);
	m_bIsPlaying = TRUE;
}

BOOL CXTPGifPreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	if (msg->message == WM_TIMER && msg->wParam == m_nTimerId)
	{
		m_pHostWindow->KillTimer(m_nTimerId);
		if (m_bIsPlaying)
		{
			m_iCurrentFrame = (++m_iCurrentFrame) % m_FrameCount;

			GUID Guid = Gdiplus::FrameDimensionTime;
			m_pPicture->SelectActiveFrame(&Guid, m_iCurrentFrame);

			UINT delay = ((PUINT)m_pDelayProperty->value)[m_iCurrentFrame] * 10;
			m_nTimerId = m_pHostWindow->SetTimer(1, delay, NULL);

			m_pHostWindow->Invalidate(FALSE);
		}

		*result = 0;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPGifPreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPGifPreview::Activate()
{
	// Does not support activation
}

BOOL CXTPGifPreview::InitImage(LPCTSTR pFilePath)
{
	return InitImage(NULL, pFilePath);
}

BOOL CXTPGifPreview::InitImage(IStream* pStream, LPCTSTR pImpliedFileName)
{
	if (!IsWindow(m_pHostWindow->GetSafeHwnd()))
		return FALSE;

	m_pPicture = (NULL != pStream ? new Gdiplus::Image(_bstr_t(pImpliedFileName))
								  : new Gdiplus::Image(pStream));

	if (m_pPicture->GetLastStatus() != Gdiplus::Ok)
		return FALSE;

	m_bLoadFinished = TRUE;

	UINT count = m_pPicture->GetFrameDimensionsCount();

	if (0 == count)
	{
		m_FrameCount	= 1;
		m_iCurrentFrame = 0;
	}
	else
	{
		m_pDimensionIDs = new GUID[count];
		m_pPicture->GetFrameDimensionsList(m_pDimensionIDs, count);

		m_FrameCount = m_pPicture->GetFrameCount(&m_pDimensionIDs[0]);

		UINT propertyItemSize = m_pPicture->GetPropertyItemSize(PropertyTagFrameDelay);
		if (propertyItemSize == 0)
			return FALSE;

		m_pDelayProperty = reinterpret_cast<Gdiplus::PropertyItem*>(malloc(propertyItemSize));
		if (Gdiplus::Ok
			!= m_pPicture->GetPropertyItem(PropertyTagFrameDelay, propertyItemSize,
										   m_pDelayProperty))
		{
			return FALSE;
		}

		m_iCurrentFrame = 0;
		GUID Guid		= Gdiplus::FrameDimensionTime;
		m_pPicture->SelectActiveFrame(&Guid, m_iCurrentFrame);
	}

	m_bLoadSuccess = TRUE;
	return TRUE;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
