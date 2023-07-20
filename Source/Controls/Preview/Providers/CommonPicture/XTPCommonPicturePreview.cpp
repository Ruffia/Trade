// XTPCommonPicturePreview.cpp : implementation of the CXTPCommonPicturePreview class.
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
#include "Controls/Preview/Providers/CommonPicture/XTPCommonPicturePreview.h"
#include "Controls/Resource.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <comdef.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#define XTP_FILETYPE_PICTURE 0
#define XTP_FILETYPE_TIFF 1
#define XTP_FILETYPE_METAFILE 2

CXTPCommonPicturePreview::CXTPCommonPicturePreview(LPCTSTR pFilePath)
	: m_pPicture(NULL)
	, m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_nOrientation(0)
	, m_FrameCount(1)
	, m_iCurrentFrame(0)
	, m_DimensionID(GUID_NULL)
	, m_FileType(XTP_FILETYPE_PICTURE)
{
	m_pPicture = new Gdiplus::Image(_bstr_t(pFilePath));

	if (m_pPicture->GetLastStatus() == Gdiplus::Ok)
	{
		m_bLoadFinished = TRUE;
		m_bLoadSuccess  = TRUE;
	}

	SetProperRotation();

	GetFileType(pFilePath);
	if (m_FileType == XTP_FILETYPE_TIFF)
		ParseMultipageTiff();
}

CXTPCommonPicturePreview::CXTPCommonPicturePreview(IStream& stream, LPCTSTR pImpliedFileName)
	: m_pPicture(NULL)
	, m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_nOrientation(0)
	, m_FrameCount(1)
	, m_iCurrentFrame(0)
	, m_DimensionID(GUID_NULL)
	, m_FileType(XTP_FILETYPE_PICTURE)
{
	m_pPicture = new Gdiplus::Image(&stream);

	if (m_pPicture->GetLastStatus() == Gdiplus::Ok)
	{
		m_bLoadFinished = TRUE;
		m_bLoadSuccess  = TRUE;
	}

	SetProperRotation();

	GetFileType(pImpliedFileName);
	if (m_FileType == XTP_FILETYPE_TIFF)
		ParseMultipageTiff();
}

CXTPCommonPicturePreview::~CXTPCommonPicturePreview()
{
	if (NULL != m_pPicture)
	{
		delete m_pPicture;
		m_pPicture = NULL;
	}
}

BOOL CXTPCommonPicturePreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPCommonPicturePreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPCommonPicturePreview::HasOwnNavigator() const
{
	return FALSE;
}

UINT CXTPCommonPicturePreview::GetNumberOfPages() const
{
	return (m_bLoadSuccess ? m_FrameCount : 0);
}

SIZE CXTPCommonPicturePreview::GetPreferredPageSize() const
{
	CSize size(0, 0);
	if (NULL != m_pPicture)
	{
		size.cx = XTPToInt(m_pPicture->GetWidth());
		size.cy = XTPToInt(m_pPicture->GetHeight());
	}

	return size;
}

void CXTPCommonPicturePreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPCommonPicturePreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	UNREFERENCED_PARAMETER(updateRect);

	if (!m_bLoadSuccess)
		return;

	if ((clientSize.cx == 0) || (clientSize.cy == 0) || (m_pPicture->GetWidth() == 0)
		|| (m_pPicture->GetHeight() == 0))
		return;

	int w = XTPToInt(m_pPicture->GetWidth());
	int h = XTPToInt(m_pPicture->GetHeight());
	if ((w > clientSize.cx) || (h > clientSize.cy))
	{
		double rX = ((double)w) / ((double)clientSize.cx);
		double rY = ((double)h) / ((double)clientSize.cy);
		double r  = rX;
		if (rY > rX)
			r = rY;

		rX = ((double)m_pPicture->GetWidth()) / r + 0.5;
		w  = (int)rX;
		if (w > clientSize.cx)
			w = clientSize.cx;

		rY = ((double)m_pPicture->GetHeight()) / r + 0.5;
		h  = (int)rY;
		if (h > clientSize.cy)
			h = clientSize.cy;
	}

	Gdiplus::Graphics hostGraphics(dc);
	int x = 0;
	int y = 0;
	if (m_FileType != XTP_FILETYPE_METAFILE)
	{
		x = (clientSize.cx - w) / 2;
		y = (clientSize.cy - h) / 2;
	}

	hostGraphics.DrawImage(m_pPicture, x, y, w, h);
}

BOOL CXTPCommonPicturePreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	UNREFERENCED_PARAMETER(result);

	WORD lowPart = LOWORD(msg->wParam);
	WORD hiPart  = HIWORD(msg->wParam);
	if ((msg->message == WM_COMMAND) && (m_FrameCount > 1) && (hiPart == BN_CLICKED))
	{
		if (lowPart == XTP_IDC_NAVIGATOR_FIRST_BTN)
		{
			m_iCurrentFrame = 0;
		}
		else if (lowPart == XTP_IDC_NAVIGATOR_LAST_BTN)
		{
			m_iCurrentFrame = m_FrameCount - 1;
		}
		else if (lowPart == XTP_IDC_NAVIGATOR_PREV_BTN)
		{
			if (0 == m_iCurrentFrame--)
				m_iCurrentFrame = 0;
		}
		else if (lowPart == XTP_IDC_NAVIGATOR_NEXT_BTN)
		{
			m_iCurrentFrame++;
			if (m_iCurrentFrame >= m_FrameCount)
				m_iCurrentFrame = m_FrameCount - 1;
		}
		else
		{
			return FALSE;
		}

		m_pPicture->SelectActiveFrame(&m_DimensionID, m_iCurrentFrame);

		CWnd* pHost = CWnd::FromHandle(msg->hwnd);
		ASSERT_VALID(pHost);

		pHost->Invalidate();
		pHost->UpdateWindow();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCommonPicturePreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPCommonPicturePreview::Activate()
{
	// Does not support activation
}

void CXTPCommonPicturePreview::GetFileType(LPCTSTR filePath)
{
	CString sFilePath(filePath);

	m_FileType = XTP_FILETYPE_PICTURE;
	int pos	= sFilePath.ReverseFind(_TCHAR('.'));

	if (pos < 0)
		return;

	pos++;

	CString ext = sFilePath.Right(sFilePath.GetLength() - pos);
	if ((ext.CompareNoCase(_T("tif")) == 0) || (ext.CompareNoCase(_T("tiff")) == 0))
	{
		m_FileType = XTP_FILETYPE_TIFF;
	}
	else if ((ext.CompareNoCase(_T("emf")) == 0) || (ext.CompareNoCase(_T("wmf")) == 0))
	{
		m_FileType = XTP_FILETYPE_METAFILE;
	}
}

void CXTPCommonPicturePreview::ParseMultipageTiff()
{
	UINT count = m_pPicture->GetFrameDimensionsCount();

	if (count == 0)
		return;

	if (Gdiplus::Ok != m_pPicture->GetFrameDimensionsList(&m_DimensionID, 1))
		return;

	m_FrameCount = m_pPicture->GetFrameCount(&m_DimensionID);

	if (m_FrameCount <= 1)
		return;

	m_pPicture->SelectActiveFrame(&m_DimensionID, m_iCurrentFrame);
}

void CXTPCommonPicturePreview::SetProperRotation()
{
	UINT cb = m_pPicture->GetPropertyItemSize(PropertyTagOrientation);
	if (0 == cb)
		return;

	Gdiplus::PropertyItem* pProp = reinterpret_cast<Gdiplus::PropertyItem*>(malloc(cb));
	if (NULL == pProp)
		return;

	if (Gdiplus::Ok != m_pPicture->GetPropertyItem(PropertyTagOrientation, cb, pProp))
	{
		free(pProp);
		return;
	}

	if (PropertyTagTypeShort != pProp->type)
	{
		free(pProp);
		return;
	}

	UINT nOrientation = *reinterpret_cast<WORD*>(pProp->value);
	free(pProp);

	Gdiplus::RotateFlipType nType = Gdiplus::RotateNoneFlipNone;
	switch (nOrientation)
	{
		// 1 - The 0th row is at the top of the visual image, and the 0th column is
		// the visual left side. No rotation is required.
		case 1: break;

		// 2 - The 0th row is at the visual top of the image, and the 0th column is
		// the visual right side.
		case 2: nType = Gdiplus::RotateNoneFlipX; break;

		// 3 - The 0th row is at the visual bottom of the image, and the 0th column is
		// the visual right side.
		case 3: nType = Gdiplus::RotateNoneFlipXY; break;

		// 4 - The 0th row is at the visual bottom of the image, and the 0th column is
		// the visual left side.
		case 4: nType = Gdiplus::RotateNoneFlipY; break;

		// 5 - The 0th row is the visual left side of the image, and the 0th column is
		// the visual top.
		case 5: nType = Gdiplus::Rotate90FlipX; break;

		// 6 - The 0th row is the visual right side of the image, and the 0th column
		// is the visual top.
		case 6: nType = Gdiplus::Rotate90FlipNone; break;

		// 7 - The 0th row is the visual right side of the image, and the 0th column
		// is the visual bottom.
		case 7: nType = Gdiplus::Rotate270FlipX; break;

		// 8 - The 0th row is the visual left side of the image, and the 0th column is
		// the visual bottom.
		case 8: nType = Gdiplus::Rotate270FlipNone; break;
	}

	if (Gdiplus::RotateNoneFlipNone != nType)
		m_pPicture->RotateFlip(nType);
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
