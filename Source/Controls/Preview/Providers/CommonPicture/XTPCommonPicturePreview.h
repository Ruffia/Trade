// XTPCommonPicturePreview.h : interface for the CXTPCommonPicturePreview class.
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

#if !defined(__XTPCOMMONPICTUREPREVIEW_H__)
#	define __XTPCOMMONPICTUREPREVIEW_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace Gdiplus
{
class Image;
}

class _XTP_EXT_CLASS CXTPCommonPicturePreview : public IXTPPreview
{
public:
	explicit CXTPCommonPicturePreview(LPCTSTR pFilePath);
	explicit CXTPCommonPicturePreview(IStream& stream, LPCTSTR pImpliedFileName);
	~CXTPCommonPicturePreview();

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

	void GetFileType(LPCTSTR filePath);
	void ParseMultipageTiff();
	void SetProperRotation();

private:
	Gdiplus::Image* m_pPicture;
	BOOL m_bLoadFinished;
	BOOL m_bLoadSuccess;
	UINT m_nOrientation;
	UINT m_FrameCount;
	UINT m_iCurrentFrame;
	GUID m_DimensionID;
	int m_FileType;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPCOMMONPICTUREPREVIEW_H__)

//}}AFX_CODEJOCK_PRIVATE
