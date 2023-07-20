// XTPPreviewCtrl.h : interface for the CXTPPreviewCtrl class.
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
#if !defined(__XTPPREVIEWCTRL_H__)
#	define __XTPPREVIEWCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPreviewHostWindow;
class CXTPPreviewNavigatorWindow;

//===========================================================================
// Summary:
//     Implements the preview control that can display visual preview data provided
//     by the best matching preview provider registered to the control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPreviewCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTPPreviewCtrl)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles control construction.
	//-----------------------------------------------------------------------
	CXTPPreviewCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles control destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPPreviewCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a preview control window object.
	// Parameters:
	//     dwExStyle  - Bitwise combination (OR) of extended window styles; otherwise
	//                  NULL for the default extended window style.
	//     dwStyle    - Bitwise combination (OR) of window styles; otherwise NULL for
	//                  the default window style.
	//     x          - The initial horizontal distance of the window from the left side
	//                  of the screen or the parent window.
	//     y          - The initial vertical distance of the window from the top of the
	//                  screen or the parent window.
	//     nWidth     - The width, in pixels, of the window.
	//     nHeight    - The height, in pixels, of the window.
	//     hwndParent - For a child window, the handle to the parent window; otherwise,
	//                  the handle of the owner window if the window has an owner.
	//     nIDorHMenu - For a child window, the window ID; otherwise, the ID of a menu
	//                  for the window.
	//     lpParam    - Pointer to user data that is passed to the CWnd::OnCreate method
	//                  in the lpCreateParams field.
	//     rect       - The size and location of the window relative to the screen or
	//                  the parent window.
	//     pParentWnd - For a child window, pointer to the parent window; otherwise, a
	//                  pointer to the owner window if the window has an owner.
	//     nID        - For a child window, the window ID; otherwise, the ID of a menu
	//                  for the window.
	// Returns:
	//     TRUE if the method was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext = NULL);

	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight,
						  HWND hWndParent, HMENU nIDorHMenu,
						  LPVOID lpParam = NULL); // <COMBINE CXTPPreviewCtrl::Create@DWORD@const
												  // RECT&@CWnd*@UINT@CCreateContext*>

	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						  UINT nID,
						  LPVOID lpParam = NULL); // <COMBINE CXTPPreviewCtrl::Create@DWORD@const
												  // RECT&@CWnd*@UINT@CCreateContext*>

	using CWnd::Create;
	using CWnd::CreateEx;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a path to a file to preview.
	// Parameters:
	//     lpFilePath        - File path to preview.
	//     lpImpliedFileName - An implied file name associated with stream data. Its extension
	//     determines the preview provider type used for stream data.
	//     pStream           - Preview data source stream. If NULL then lpImpliedFileName must be a
	//     full path.
	//-----------------------------------------------------------------------
	void Preview(LPCTSTR lpFilePath);
	void Preview(IStream* pStream,
				 LPCTSTR lpImpliedFileName); // <combine CXTPPreviewCtrl::Preview@LPCTSTR>

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new preview provider object. The object will be destroyed by the
	//     control upon control destruction.
	// Parameters:
	//     provider - A new provider object pointer.
	//     associations - A string that specifies a list of custom supported file
	//                    extensions that will be used in addition to the default
	//                    list of supported extensions provided by the provider
	//                    (e.g. "doc;docx;xls;xlsx"), or NULL to use only provider
	//                    supported extensions.
	// Remarks:
	//     Every time a new preview request is issued, the providers get checked
	//     in the order of addition. The first provider that is able to create
	//     a preview object for a new preview target is selected.
	// See also:
	//     IXTPPreviewProvider::GetDefaultFileAssociations, IXTPPreviewProvider::CreatePreview
	//-----------------------------------------------------------------------
	void AddProvider(IXTPPreviewProvider* provider, LPCTSTR associations = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds all known providers with their default associations in the following order:
	//       1. CXTPSystemPreviewProvider
	//       2. CXTPCommonPicturePreviewProvider
	//       3. CXTPGifPreviewProvider
	//       4. CXTPMsgPreviewProvider
	//       5. CXTPTextPreviewProvider
	//       6. CXTPXamlPreviewProvider
	// See also:
	//     AddProvider
	//-----------------------------------------------------------------------
	void AddDefaultProviders();

	//-----------------------------------------------------------------------
	// Summary:
	//     Customizes the message displayed when a preview is being loaded.
	// Parameters:
	//     message - Custom message. Every occurence of the '%' symbol in the string
	//               will be replaced with the preview target path/name.
	// See also:
	//     SetFailedMessage
	//-----------------------------------------------------------------------
	void SetLoadingMessage(LPCTSTR message);

	//-----------------------------------------------------------------------
	// Summary:
	//     Customizes the message displayed when a preview cannot be obtained.
	// Parameters:
	//     message - Custom message. Every occurence of the '%' symbol in the string
	//               will be replaced with the preview target path/name.
	// See also:
	//     SetLoadingMessage
	//-----------------------------------------------------------------------
	void SetFailedMessage(LPCTSTR message);

	//-----------------------------------------------------------------------
	// Summary:
	//     Provides access to the background color value used for filling the
	//     preview area.
	// Parameters:
	//     cr - New color value.
	// Returns:
	//     Current color value.
	// See also:
	//     GetForegroundColor
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor() const;
	void SetBackgroundColor(COLORREF cr); // <COMBINE CXTPPreviewCtrl::GetBackgroundColor@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Provides access to the foreground color value used for text messages
	//     in the preview area.
	// Parameters:
	//     cr - New color value.
	// Returns:
	//     Current color value.
	// See also:
	//     GetBackgroundColor
	//-----------------------------------------------------------------------
	COLORREF GetForegroundColor() const;
	void SetForegroundColor(COLORREF cr); // <COMBINE CXTPPreviewCtrl::GetForegroundColor@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets/sets an automatic preview activation on loading. By default this behavior is not
	//     enabled because it is undefined how some system provided preview handlers will behave on
	//     activation. If the behavior is enabled then every time when a new preview is loaded its
	//     provider tries actiate, set focus, or do whatever else is deemed to be an activate state
	//     of a preview for that particular provider. A provider may not support this behavior at
	//     all if it makes no sense for provided preview, for example a static picture.
	// Parameters:
	//     bEnable - TRUE to enable the behavior, FALSE to disable.
	// Returns:
	//     The current state of the behavior.
	//-----------------------------------------------------------------------
	void EnableAutoPreviewActivate(BOOL bEnable = TRUE);
	BOOL IsAutoPreviewActivateEnabled() const; // <combine
											   // CXTPPreviewCtrl::EnableAutoPreviewActivate>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_CODEJOCK_PRIVATE

private:
	void SetCompositionWnd(HWND hCompositionWnd);
	BOOL RegisterWindowClass();
	BOOL IsExtensionInString(const CString& strAssociation, LPCTSTR lpExt) const;
	int CalcNavigatorHeight();
	void CreateHostAndNavigator();
	void ChangeChildrenPosition(WINDOWPOS* lpwndpos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnFirst();
	afx_msg void OnLast();
	afx_msg void OnPrev();
	afx_msg void OnNext();

private:
	CPtrArray m_providers;
	CStringArray m_additionalAssociations;
	CString m_strLoadingMessage;
	CString m_strFailedMessage;
	BOOL m_bChildrenCreated;
	BOOL m_bShowNavigator;
	CXTPPreviewHostWindow* m_pHostWindow;
	CXTPPreviewNavigatorWindow* m_pNavigatorWindow;
	CRect m_rPrevPlace;
	int m_iNavHeight;
	BOOL m_bSubclassed;
	HWND m_hCompositionWnd;
	COLORREF m_crBack;
	COLORREF m_crFore;
	BOOL m_bAutoActivate;
};

AFX_INLINE void CXTPPreviewCtrl::SetCompositionWnd(HWND hCompositionWnd)
{
	m_hCompositionWnd = hCompositionWnd;
}

AFX_INLINE COLORREF CXTPPreviewCtrl::GetBackgroundColor() const
{
	return m_crBack;
}

AFX_INLINE void CXTPPreviewCtrl::SetBackgroundColor(COLORREF cr)
{
	m_crBack = cr;
}

AFX_INLINE COLORREF CXTPPreviewCtrl::GetForegroundColor() const
{
	return m_crFore;
}

AFX_INLINE void CXTPPreviewCtrl::SetForegroundColor(COLORREF cr)
{
	m_crFore = cr;
}

AFX_INLINE void CXTPPreviewCtrl::EnableAutoPreviewActivate(BOOL bEnable /*= TRUE*/)
{
	m_bAutoActivate = bEnable;
}

AFX_INLINE BOOL CXTPPreviewCtrl::IsAutoPreviewActivateEnabled() const
{
	return m_bAutoActivate;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPPREVIEWCTRL_H__)
