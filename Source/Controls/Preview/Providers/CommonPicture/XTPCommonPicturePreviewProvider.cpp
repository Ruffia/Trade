// XTPCommonPicturePreviewProvider.cpp : implementation of the CXTPCommonPicturePreviewProvider
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

#include "stdafx.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/CommonPicture/XTPCommonPicturePreviewProvider.h"
#include "Controls/Preview/Providers/CommonPicture/XTPCommonPicturePreview.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

LPCTSTR CXTPCommonPicturePreviewProvider::GetDefaultFileAssociations() const
{
	return _T("bmp;png;tif;tiff;ico;jpeg;jpg;exif;wmf;emf;");
}

IXTPPreview* CXTPCommonPicturePreviewProvider::CreatePreview(LPCTSTR pFilePath, HWND hwndParent)
{
	UNREFERENCED_PARAMETER(hwndParent);

	IXTPPreview* pPreview = new CXTPCommonPicturePreview(pFilePath);

	if (!pPreview->IsReady())
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

IXTPPreview* CXTPCommonPicturePreviewProvider::CreatePreview(IStream& stream,
															 LPCTSTR pImpliedFileName,
															 HWND hwndParent)
{
	UNREFERENCED_PARAMETER(hwndParent);

	IXTPPreview* pPreview = new CXTPCommonPicturePreview(stream, pImpliedFileName);

	if (!pPreview->IsReady())
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
