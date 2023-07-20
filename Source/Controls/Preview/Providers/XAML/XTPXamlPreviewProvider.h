// XTPXamlPreviewProvider.h : interface for the CXTPXamlPreviewProvider class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPXAMLPREVIEWPROVIDER_H__)
#	define __XTPXAMLPREVIEWPROVIDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//=======================================================================
// Summary:
//     Provides a preview handler for XAML files that uses Markup components for
//     rendering file contents.
// See Also:
//     CXTPSystemPreviewProvider, CXTPCommonPicturePreviewProvider, CXTPGifPreviewProvider,
//     CXTPMsgPreviewProvider, CXTPTextPreviewProvider
//=======================================================================
class _XTP_EXT_CLASS CXTPXamlPreviewProvider : public CXTPPreviewProviderBase
{
private:
	virtual LPCTSTR GetDefaultFileAssociations() const;
	virtual IXTPPreview* CreatePreview(LPCTSTR pFilePath, HWND hwndParent);
	virtual IXTPPreview* CreatePreview(IStream& stream, LPCTSTR pImpliedFileName, HWND hwndParent);
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPXAMLPREVIEWPROVIDER_H__)
