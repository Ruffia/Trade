
// XTPGifPreview.h : interface for the CXTPGifPreview class.
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

#if !defined(__XTPGIFPREVIEW_H__)
#	define __XTPGIFPREVIEW_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace Gdiplus
{
class Image;
class PropertyItem;
} // namespace Gdiplus

class _XTP_EXT_CLASS CXTPGifPreview : public IXTPPreview
{
public:
	explicit CXTPGifPreview(HWND parent);
	~CXTPGifPreview();

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

	BOOL InitImage(LPCTSTR pFilePath);
	BOOL InitImage(IStream* pStream, LPCTSTR pImpliedFileName);

private:
	friend class CXTPGifPreviewProvider;
	Gdiplus::Image* m_pPicture;
	BOOL m_bLoadFinished;
	BOOL m_bLoadSuccess;
	ULONG_PTR m_gdiPlusToken;
	CWnd* m_pHostWindow;
	UINT_PTR m_nTimerId;
	BOOL m_bIsPlaying;
	UINT m_FrameCount;
	UINT m_iCurrentFrame;
	GUID* m_pDimensionIDs;
	Gdiplus::PropertyItem* m_pDelayProperty;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPGIFPREVIEW_H__)

//}}AFX_CODEJOCK_PRIVATE
