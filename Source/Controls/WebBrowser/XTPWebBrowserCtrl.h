// XTPWebBrowserCtrl.h : interface for the CXTPWebBrowserCtrl class.
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

#if !defined(__XTPWEBBROWSERCTRL_H__)
//{{AFX_CODEJOCK_PRIVATE
#	define __XTPWEBBROWSERCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#	include <atlbase.h>
#	include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE
class CXTPWebBrowserProvider;
//}}AFX_CODEJOCK_PRIVATE

//=======================================================================
// Summary:
//     Defines possible WebBrowser providers.
//=======================================================================
enum XTPWebBrowserProvider
{
	xtpWebBrowserShellExplorer, // Specifies the ShellExplorer WebBrowser provider, which is also
								// used by the Microsoft Internet Explorer browser.
	xtpWebBrowserWebView, // Specifies the WebView2 WebBrowser provider, which is also used by the
						  // Microsoft Edge browser.
	xtpWebBrowserLatest = xtpWebBrowserWebView // Specifies the latest supported WebBrowser
											   // provider.
};

//===========================================================================
// Summary:
//     Implements the WebBrowser control that is capable of hosting different supported Web
//     browsers. The class provides a narrow interface that gives access to the most frequently
//     needed functionality that is provided by each supported provider. The unified event handling
//     interface simplifies event processing.
//===========================================================================
class _XTP_EXT_CLASS CXTPWebBrowserCtrl
	: public CWnd
	, private IXTPWebBrowserHost
{
	DECLARE_DYNAMIC(CXTPWebBrowserCtrl)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles control construction.
	// Parameters:
	//     nProvider - Specifies the provider to initialize by default. It is not guaranteed that
	//                 the specified provider will be activated as at the point of construction it
	//                 is not yet known which of the providers are available and supported. In case
	//                 the specified provider is impossible to activate the ShellExplorer provider
	//                 will be activated by default.
	//-----------------------------------------------------------------------
	CXTPWebBrowserCtrl(XTPWebBrowserProvider nProvider = xtpWebBrowserLatest);

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles control destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPWebBrowserCtrl();

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
						  LPVOID lpParam = NULL); // <COMBINE CXTPWebBrowserCtrl::Create@DWORD@const
												  // RECT&@CWnd*@UINT@CCreateContext*>

	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
						  UINT nID,
						  LPVOID lpParam = NULL); // <COMBINE CXTPWebBrowserCtrl::Create@DWORD@const
												  // RECT&@CWnd*@UINT@CCreateContext*>

	using CWnd::Create;
	using CWnd::CreateEx;

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets custom properties that providers can use if provided.
	// Parameters:
	//      strName - Property name.
	//      pvtValue - Property value. The control will hold the ownership
	//                 of the allocated data. NULL pointer value will cause
	//                 removal of the previously set property value.
	// Returns:
	//      Current property value if it is found or NULL otherwise.
	// Remarks:
	//      Custom properties are held by the control object and don't belong
	//      to the current provider set, however they can be accessed by any provider.
	//      Currently supported properties include:
	//       - "UDF" - The string path value that indicates the UserDefaultFolder
	//                 for WebView2 provider.
	//       - "OverrideKeyPath" - The string registry key path value relative to
	//                             HKEY_CURRENT_USER that speicifies a registry key
	//                             to override default Internet Explorer settings.
	//       - "OptionKeyPath" - The string registry key path value relative to
	//                           HKEY_CURRENT_USER that speicifies a registry key
	//                           to use as a default location of Internet Explorer
	//                           settings.
	//       In order to apply custom properties to a provider that is already created
	//       the CXTPWebBrowserCtrl::ReCreateProvider method must be called.
	// See Also:
	//      CXTPWebBrowserCtrl::ReCreateProvider
	//-----------------------------------------------------------------------
	virtual const COleVariant* GetProperty(const CString& strName) const;
	void SetProperty(const CString& strName,
					 COleVariant* pvtValue); // <combine CXTPWebBrowserCtrl::GetProperty@const
											 // CString&>

	//-----------------------------------------------------------------------
	// Summary:
	//      Re-creates the current provider, i.e. the actual Web browser instance
	//      will be destroyed and re-created. The external object will be automatically
	//      bound if assigned, custom properties won't be affected, and the previously
	//      navigated location will be restored.
	// Returns:
	//      TRUE if a provider is re-created successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL ReCreateProvider();

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the active WebBrowser provider type.
	// Returns:
	//      The type of of the active WebBrowser provider.
	//-----------------------------------------------------------------------
	XTPWebBrowserProvider GetProviderType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets the active WebBrowser provider type.
	// Returns:
	//      TRUE if the WebBrowser provider has been changed.
	// Remarks:
	//      All previously set provider specific options will get reset.
	//-----------------------------------------------------------------------
	BOOL SetProviderType(XTPWebBrowserProvider nProvider);

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the active WebBrowser provider object or NULL if no provider has been activated
	//      yet or provider activation hasn't been completed.
	// Returns:
	//      A pointer to the base WebBrowser provider implementation. The pointer can be cast to the
	//      actual WebBrowser provider implementation if extended access is required.
	//-----------------------------------------------------------------------
	CXTPWebBrowserProvider* GetProvider() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the native WebBrowser provider object or NULL if no provider is activated or the
	//      current provider does not provide access to its core object via IUnknown.
	// Returns:
	//      Obtains the native WebBrowser provider object.
	// Remarks:
	//      The ShellExplorer provider will return IWebBrowserApp object.
	//      The WebView provider will return ICoreWebView2Controller object.
	//-----------------------------------------------------------------------
	IUnknown* GetBrowserObject() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets the event sink interface for handling WebBrowser events.
	// Parameters:
	//      pEventSink - The new event sink interface pointer or NULL to disable event handling.
	// Returns:
	//      The current even sink interface pointer.
	// Remarks:
	//      The control does not manage even sink object lifetime.
	//-----------------------------------------------------------------------
	IXTPWebBrowserEventSink* GetEventSink() const;
	void SetEventSink(IXTPWebBrowserEventSink* pEventSink); // <combines
															// CXTPWebBrowserCtrl::GetEventSink>

	//-----------------------------------------------------------------------
	// Summary:
	//      Performs navigation to the specified URL.
	// Parameters:
	//      pUrl - The URL value to navigate to.
	// Returns:
	//      S_OK if navigation is succeeded or an error code otherwise.
	// Remarks:
	//      If a provider is no yet activated the navigation will be postponed until a provider is
	//      activated.
	//-----------------------------------------------------------------------
	HRESULT Navigate(LPCTSTR pUrl);

	//-----------------------------------------------------------------------
	// Summary:
	//      Executes a script by the active WebBrowser provider.
	// Parameters:
	//      pText     - Script code text.
	//      pvtResult - The optional pointer to the variant object that is supposed to receive the
	//                  result of the executed script.
	// Returns:
	//      S_OK if script execution or its attempt are successful or an error code otherwise.
	// Remarks:
	//      The language of the script code is provider specific. Currently each provider supports
	//      JavaScript by default.
	//-----------------------------------------------------------------------
	HRESULT ExecuteScript(LPCWSTR pText, CComVariant* pvtResult = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets the object to be used by the "window.external" propery in the script execution
	//      context.
	// Parameters:
	//      pExternal - The new external object to be used.
	// Returns:
	//      The current external object.
	// Remarks:
	//      The WebBrowser control holds a reference to the specified external object until a new
	//      object is set or the control is destructed.
	//-----------------------------------------------------------------------
	virtual IDispatch* GetExternal() const;
	void SetExternal(IDispatch* pExternal); // <combine CXTPWebBrowserCtrl::GetExternal>

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_CODEJOCK_PRIVATE

private:
	// IXTPWebBrowserHost overrides

	virtual IUnknown* GetHostUnknown() const;
	virtual CWnd& GetHostWindow() const;

	virtual void ShellExplorer_OnStatusTextChange(LPCTSTR lpszText);
	virtual void ShellExplorer_OnProgressChange(long nProgress, long nProgressMax);
	virtual void ShellExplorer_OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void ShellExplorer_OnDownloadBegin();
	virtual void ShellExplorer_OnDownloadComplete();
	virtual void ShellExplorer_OnTitleChange(LPCTSTR lpszText);
	virtual void ShellExplorer_OnPropertyChange(LPCTSTR lpszProperty);
	virtual void ShellExplorer_OnNewWindow2(LPDISPATCH* ppDisp, BOOL* pbCancel);
	virtual void ShellExplorer_OnQuit();
	virtual void ShellExplorer_OnVisible(BOOL bVisible);
	virtual void ShellExplorer_OnToolBar(BOOL bToolBar);
	virtual void ShellExplorer_OnMenuBar(BOOL bMenuBar);
	virtual void ShellExplorer_OnStatusBar(BOOL bStatusBar);
	virtual void ShellExplorer_OnFullScreen(BOOL bFullScreen);
	virtual void ShellExplorer_OnTheaterMode(BOOL bTheaterMode);
	virtual void ShellExplorer_OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* pvtURL);
	virtual void ShellExplorer_OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* pvtURL,
												 VARIANT* pvtFlags, VARIANT* pvtTargetFrameName,
												 VARIANT* pvtPostData, VARIANT* pvtHeaders,
												 BOOL* pbCancel);
	virtual void ShellExplorer_OnDocumentComplete(LPDISPATCH pDisp, VARIANT* pvtURL);
	virtual void ShellExplorer_OnNavigateError(LPDISPATCH pDisp, VARIANT* pvtURL,
											   VARIANT* pvtTargetFrameName, VARIANT* pvtStatusCode,
											   BOOL* pbCancel);
	virtual void ShellExplorer_OnWindowSetResizable(BOOL bResizable);
	virtual void ShellExplorer_OnWindowSetLeft(long nLeft);
	virtual void ShellExplorer_OnWindowSetTop(long nTop);
	virtual void ShellExplorer_OnWindowSetWidth(long nWidth);
	virtual void ShellExplorer_OnWindowSetHeight(long nHeight);
	virtual void ShellExplorer_OnWindowClosing(BOOL bIsChildWindow, BOOL FAR* pbCancel);
	virtual void ShellExplorer_OnSetSecureLockIcon(long nSecureLockIcon);

	virtual HRESULT WebView2_NavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	virtual HRESULT WebView2_ContentLoading(ICoreWebView2ContentLoadingEventArgs* args);
	virtual HRESULT WebView2_SourceChanged(ICoreWebView2SourceChangedEventArgs* args);
	virtual HRESULT WebView2_HistoryChanged(IUnknown* args);
	virtual HRESULT WebView2_NavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	virtual HRESULT WebView2_FrameNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	virtual HRESULT
		WebView2_FrameNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	virtual HRESULT WebView2_ScriptDialogOpening(ICoreWebView2ScriptDialogOpeningEventArgs* args);
	virtual HRESULT WebView2_PermissionRequested(ICoreWebView2PermissionRequestedEventArgs* args);
	virtual HRESULT WebView2_ProcessFailed(ICoreWebView2ProcessFailedEventArgs* args);
	virtual HRESULT WebView2_NewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args);
	virtual HRESULT WebView2_DocumentTitleChanged(IUnknown* args);
	virtual HRESULT WebView2_ContainsFullScreenElementChanged(IUnknown* args);
	virtual HRESULT WebView2_WebResourceRequested(ICoreWebView2WebResourceRequestedEventArgs* args);
	virtual HRESULT WebView2_WindowCloseRequested(IUnknown* args);

private:
	BOOL RegisterWindowClass();
	BOOL InitializeProvider();
	void ReleaseProvider();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

private:
	static const UINT m_WM_INITIALIZE;
	XTPWebBrowserProvider m_nProvider;
	CXTPWebBrowserProvider* m_pProvider;
	BOOL m_bSubclassed;
	CString m_strPendlingUrl;
	IXTPWebBrowserEventSink* m_pEventSink;
	IDispatch* m_pExternal;
	CMap<CString, LPCTSTR, COleVariant*, COleVariant*> m_mapProps;
};

AFX_INLINE CXTPWebBrowserProvider* CXTPWebBrowserCtrl::GetProvider() const
{
	return const_cast<CXTPWebBrowserCtrl*>(this)->m_pProvider;
}

AFX_INLINE XTPWebBrowserProvider CXTPWebBrowserCtrl::GetProviderType() const
{
	return m_nProvider;
}

AFX_INLINE void CXTPWebBrowserCtrl::SetEventSink(IXTPWebBrowserEventSink* pEventSink)
{
	m_pEventSink = pEventSink;
}

AFX_INLINE IXTPWebBrowserEventSink* CXTPWebBrowserCtrl::GetEventSink() const
{
	return m_pEventSink;
}

AFX_INLINE IDispatch* CXTPWebBrowserCtrl::GetExternal() const
{
	return m_pExternal;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPWEBBROWSERCTRL_H__)
