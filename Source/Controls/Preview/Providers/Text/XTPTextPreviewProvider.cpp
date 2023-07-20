// XTPTextPreviewProvider.cpp : implementation of the CXTPTextPreviewProvider class.
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
#include "Controls/Preview/Providers/Text/XTPTextPreviewProvider.h"
#include "Controls/Preview/Providers/Text/XTPTextPreview.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

LPCTSTR CXTPTextPreviewProvider::GetDefaultFileAssociations() const
{
	return _T("txt;");
}

IXTPPreview* CXTPTextPreviewProvider::CreatePreview(LPCTSTR pFilePath, HWND hwndParent)
{
	CXTPTextPreview* pPreview = new CXTPTextPreview();

	if (!pPreview->CreateRichEditCtrl(hwndParent))
	{
		delete pPreview;
		return NULL;
	}

	if (!pPreview->LoadFile(pFilePath))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

IXTPPreview* CXTPTextPreviewProvider::CreatePreview(IStream& stream, LPCTSTR pImpliedFileName,
													HWND hwndParent)
{
	UNREFERENCED_PARAMETER(pImpliedFileName);

	CXTPTextPreview* pPreview = new CXTPTextPreview();

	if (!pPreview->CreateRichEditCtrl(hwndParent))
	{
		delete pPreview;
		return NULL;
	}

	if (!pPreview->LoadFile(stream))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
