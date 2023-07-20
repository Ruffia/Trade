// XTPGifPreviewProvider.cpp : implementation of the CXTPGifPreviewProvider class.
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
#include "Controls/Preview/Providers/GIF/XTPGifPreviewProvider.h"
#include "Controls/Preview/Providers/GIF/XTPGifPreview.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

LPCTSTR CXTPGifPreviewProvider::GetDefaultFileAssociations() const
{
	return _T("gif;");
}

IXTPPreview* CXTPGifPreviewProvider::CreatePreview(LPCTSTR pFilePath, HWND hwndParent)
{
	CXTPGifPreview* pPreview = new CXTPGifPreview(hwndParent);

	if (!pPreview->InitImage(pFilePath))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

IXTPPreview* CXTPGifPreviewProvider::CreatePreview(IStream& stream, LPCTSTR pImpliedFileName,
												   HWND hwndParent)
{
	CXTPGifPreview* pPreview = new CXTPGifPreview(hwndParent);

	if (!pPreview->InitImage(&stream, pImpliedFileName))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
