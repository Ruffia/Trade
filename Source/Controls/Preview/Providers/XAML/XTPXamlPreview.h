// XTPXamlPreview.h : interface for the CXTPXamlPreview class.
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

#if !defined(__XTPXAMLPREVIEW_H__)
#	define __XTPXAMLPREVIEW_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace Gdiplus
{
class Image;
}

class CXTPMarkupContext;
class CXTPMarkupUIElement;

class _XTP_EXT_CLASS CXTPXamlPreview : public IXTPPreview
{
	class CContext;

public:
	CXTPXamlPreview(IXTPPreviewHost& host, LPCTSTR pFilePath);
	CXTPXamlPreview(IXTPPreviewHost& host, IStream& stream, LPCTSTR pImpliedFileName);
	~CXTPXamlPreview();

private:
	virtual BOOL IsReady() const;
	virtual BOOL IsAvailable() const;
	virtual BOOL HasOwnNavigator() const;
	virtual UINT GetNumberOfPages() const;
	virtual SIZE GetPreferredPageSize() const;
	virtual void GoToPage(UINT page);
	virtual void Draw(HDC dc, SIZE clientSize, RECT updateRect);
	virtual BOOL OnHostWndMsg(LPMSG msg, LRESULT* result);
	virtual BOOL SupportsCompositedParent() const;
	virtual void Activate();

private:
	BOOL m_bLoadFinished;
	BOOL m_bLoadSuccess;
	CXTPMarkupContext* m_pMarkupContext;
	CXTPMarkupUIElement* m_pMarkupRoot;
	HWND m_hwnd;
	IXTPPreviewHost& m_host;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPXAMLPREVIEW_H__)

//}}AFX_CODEJOCK_PRIVATE
