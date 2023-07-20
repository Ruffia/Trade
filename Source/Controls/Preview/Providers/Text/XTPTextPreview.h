// XTPTextPreview.h : interface for the CXTPTextPreview class.
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

#if !defined(__XTPTEXTPREVIEW_H__)
#	define __XTPTEXTPREVIEW_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPTextPreviewProvider;

class _XTP_EXT_CLASS CXTPTextPreview : public IXTPPreview
{
public:
	CXTPTextPreview();
	~CXTPTextPreview();

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

	BOOL CreateRichEditCtrl(HWND parent);
	BOOL LoadFile(LPCTSTR pFilePath);
	BOOL LoadFile(IStream& stream);

	static DWORD CALLBACK StreamInFile(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

private:
	friend class CXTPTextPreviewProvider;
	BOOL m_bLoadFinished;
	BOOL m_bLoadSuccess;
	BOOL m_bCreated;
	CRichEditCtrl m_RichEdit;
	CFile m_inFile;
	IStream* m_pStream;
	ULARGE_INTEGER m_ulSize;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPTEXTPREVIEW_H__)

//}}AFX_CODEJOCK_PRIVATE
