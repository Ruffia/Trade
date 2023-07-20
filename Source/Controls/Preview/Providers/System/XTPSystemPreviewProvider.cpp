// XTPSystemPreviewProvider.cpp : implementation of the CXTPSystemPreviewProvider class.
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
#include "Controls/Preview/Providers/System/XTPSystemPreview.h"
#include "Controls/Preview/Providers/System/XTPSystemPreviewProvider.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

LPCTSTR CXTPSystemPreviewProvider::GetDefaultFileAssociations() const
{
	return NULL;
}

IXTPPreview* CXTPSystemPreviewProvider::CreatePreview(LPCTSTR pFilePath, HWND hwndParent)
{
	CXTPSystemPreview* pSystemPreview = new CXTPSystemPreview();

	if (!pSystemPreview->LoadFile(pFilePath, hwndParent))
	{
		delete pSystemPreview;
		return NULL;
	}

	return pSystemPreview;
}

IXTPPreview* CXTPSystemPreviewProvider::CreatePreview(IStream& stream, LPCTSTR pImpliedFileName,
													  HWND hwndParent)
{
	CXTPSystemPreview* pSystemPreview = new CXTPSystemPreview();

	if (!pSystemPreview->LoadFile(&stream, pImpliedFileName, hwndParent))
	{
		delete pSystemPreview;
		return NULL;
	}

	return pSystemPreview;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
