// XTPMsgPreviewProvider.cpp : implementation of the CXTPMsgPreviewProvider class.
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
#include "Controls/Preview/Providers/MSG/XTPMsgPreviewProvider.h"
#include "Controls/Preview/Providers/MSG/XTPMsgPreview.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

LPCTSTR CXTPMsgPreviewProvider::GetDefaultFileAssociations() const
{
	return _T("msg;");
}

IXTPPreview* CXTPMsgPreviewProvider::CreatePreview(LPCTSTR pFilePath, HWND hwndParent)
{
	CXTPMsgPreview* pPreview = new CXTPMsgPreview();

	if (!pPreview->CreateMsgDialog(hwndParent))
	{
		delete pPreview;
		return NULL;
	}

	if (!pPreview->LoadMsg(pFilePath))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

IXTPPreview* CXTPMsgPreviewProvider::CreatePreview(IStream& stream, LPCTSTR pImpliedFileName,
												   HWND hwndParent)
{
	CXTPMsgPreview* pPreview = new CXTPMsgPreview();

	if (!pPreview->CreateMsgDialog(hwndParent))
	{
		delete pPreview;
		return NULL;
	}

	if (!pPreview->LoadMsg(&stream, pImpliedFileName))
	{
		delete pPreview;
		return NULL;
	}

	return pPreview;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
