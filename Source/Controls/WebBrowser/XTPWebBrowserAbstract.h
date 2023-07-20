// XTPWebBrowserAbstract.h : interfaces for the WebBrowser control.
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

#if !defined(__XTPWEBBROWSERABSTRACT_H__)
//{{AFX_CODEJOCK_PRIVATE
#	define __XTPWEBBROWSERABSTRACT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/XTPMacros.h"
#	include "Controls/WebBrowser/Providers/WebView2/XTPWebView2.h"

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPWebBrowserCtrl;

//{{AFX_CODEJOCK_PRIVATE
struct IXTPWebBrowserHost
{
	virtual ~IXTPWebBrowserHost()
	{
	}

	virtual CWnd& GetHostWindow() const									 = 0;
	virtual IUnknown* GetHostUnknown() const							 = 0;
	virtual IDispatch* GetExternal() const								 = 0;
	virtual const COleVariant* GetProperty(const CString& strName) const = 0;

	virtual void ShellExplorer_OnStatusTextChange(LPCTSTR lpszText)						= 0;
	virtual void ShellExplorer_OnProgressChange(long nProgress, long nProgressMax)		= 0;
	virtual void ShellExplorer_OnCommandStateChange(long nCommand, BOOL bEnable)		= 0;
	virtual void ShellExplorer_OnDownloadBegin()										= 0;
	virtual void ShellExplorer_OnDownloadComplete()										= 0;
	virtual void ShellExplorer_OnTitleChange(LPCTSTR lpszText)							= 0;
	virtual void ShellExplorer_OnPropertyChange(LPCTSTR lpszProperty)					= 0;
	virtual void ShellExplorer_OnNewWindow2(LPDISPATCH* ppDisp, BOOL* pbCancel)			= 0;
	virtual void ShellExplorer_OnQuit()													= 0;
	virtual void ShellExplorer_OnVisible(BOOL bVisible)									= 0;
	virtual void ShellExplorer_OnToolBar(BOOL bToolBar)									= 0;
	virtual void ShellExplorer_OnMenuBar(BOOL bMenuBar)									= 0;
	virtual void ShellExplorer_OnStatusBar(BOOL bStatusBar)								= 0;
	virtual void ShellExplorer_OnFullScreen(BOOL bFullScreen)							= 0;
	virtual void ShellExplorer_OnTheaterMode(BOOL bTheaterMode)							= 0;
	virtual void ShellExplorer_OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* pvtURL)   = 0;
	virtual void ShellExplorer_OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* pvtURL,
												 VARIANT* pvtFlags, VARIANT* pvtTargetFrameName,
												 VARIANT* pvtPostData, VARIANT* pvtHeaders,
												 BOOL* pbCancel)						= 0;
	virtual void ShellExplorer_OnDocumentComplete(LPDISPATCH pDisp, VARIANT* pvtURL)	= 0;
	virtual void ShellExplorer_OnNavigateError(LPDISPATCH pDisp, VARIANT* pvtURL,
											   VARIANT* pvtTargetFrameName, VARIANT* pvtStatusCode,
											   BOOL* pbCancel)							= 0;
	virtual void ShellExplorer_OnWindowSetResizable(BOOL bResizable)					= 0;
	virtual void ShellExplorer_OnWindowSetLeft(long nLeft)								= 0;
	virtual void ShellExplorer_OnWindowSetTop(long nTop)								= 0;
	virtual void ShellExplorer_OnWindowSetWidth(long nWidth)							= 0;
	virtual void ShellExplorer_OnWindowSetHeight(long nHeight)							= 0;
	virtual void ShellExplorer_OnWindowClosing(BOOL bIsChildWindow, BOOL FAR* pbCancel) = 0;
	virtual void ShellExplorer_OnSetSecureLockIcon(long nSecureLockIcon)				= 0;

	virtual HRESULT WebView2_NavigationStarting(ICoreWebView2NavigationStartingEventArgs* args) = 0;
	virtual HRESULT WebView2_ContentLoading(ICoreWebView2ContentLoadingEventArgs* args)			= 0;
	virtual HRESULT WebView2_SourceChanged(ICoreWebView2SourceChangedEventArgs* args)			= 0;
	virtual HRESULT WebView2_HistoryChanged(IUnknown* args)										= 0;
	virtual HRESULT
		WebView2_NavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args) = 0;
	virtual HRESULT
		WebView2_FrameNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args) = 0;
	virtual HRESULT
		WebView2_FrameNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args) = 0;
	virtual HRESULT
		WebView2_ScriptDialogOpening(ICoreWebView2ScriptDialogOpeningEventArgs* args) = 0;
	virtual HRESULT
		WebView2_PermissionRequested(ICoreWebView2PermissionRequestedEventArgs* args)			= 0;
	virtual HRESULT WebView2_ProcessFailed(ICoreWebView2ProcessFailedEventArgs* args)			= 0;
	virtual HRESULT WebView2_NewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args) = 0;
	virtual HRESULT WebView2_DocumentTitleChanged(IUnknown* args)								= 0;
	virtual HRESULT WebView2_ContainsFullScreenElementChanged(IUnknown* args)					= 0;
	virtual HRESULT
		WebView2_WebResourceRequested(ICoreWebView2WebResourceRequestedEventArgs* args) = 0;
	virtual HRESULT WebView2_WindowCloseRequested(IUnknown* args)						= 0;
};
//}}AFX_CODEJOCK_PRIVATE

//=======================================================================
// Summary:
//      Describes default WebBrowser control event sink interface and providers
//      default implementation of the event handlers.
//=======================================================================
struct IXTPWebBrowserEventSink
{
	//-----------------------------------------------------------------------
	// Summary:
	//      Handles event sink destruction
	//-----------------------------------------------------------------------
	virtual ~IXTPWebBrowserEventSink()
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles StatusTextChange event for the ShellExplorer WebBrowser provider type. Fires
	//      when the status bar text of the object has changed.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      lpszText - The new status bar text
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnStatusTextChange(CXTPWebBrowserCtrl& sender, LPCTSTR lpszText)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(lpszText);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles ProgressChange event for the ShellExplorer WebBrowser provider type. Fires when
	//      the progress of a download operation is updated on the object.
	// Parameters:
	//      sender    - WebBrowser control that triggered the event.
	//      nProgress - Specifies the amount of total progress to show, or -1 when progress is
	//                  complete.
	//      nProgressMax - Specifies the maximum progress value.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnProgressChange(CXTPWebBrowserCtrl& sender, long nProgress,
												long nProgressMax)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nProgress);
		XTP_UNUSED_PARAMETER(nProgressMax);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles CommandStateChange event for the ShellExplorer WebBrowser provider type. Fires
	//      when the enabled state of a command changes.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      nCommand - Specifies the command that changed. Refer to CommandStateChangeConstants in
	//                 Windows SDK for possible values.
	//      bEnable - Specifies the enabled state.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnCommandStateChange(CXTPWebBrowserCtrl& sender, long nCommand,
													BOOL bEnable)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nCommand);
		XTP_UNUSED_PARAMETER(bEnable);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles DownloadBegin event for the ShellExplorer WebBrowser provider type. Fires when a
	//      navigation operation begins.
	// Parameters:
	//      sender - WebBrowser control that triggered the event.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnDownloadBegin(CXTPWebBrowserCtrl& sender)
	{
		XTP_UNUSED_PARAMETER(sender);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles DownloadComplete event for the ShellExplorer WebBrowser provider type. Fires
	//      when a navigation operation completes.
	// Parameters:
	//      sender - WebBrowser control that triggered the event.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnDownloadComplete(CXTPWebBrowserCtrl& sender)
	{
		XTP_UNUSED_PARAMETER(sender);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles TitleChange event for the ShellExplorer WebBrowser provider type. Fires when the
	//      title of a document in the object becomes available or changes.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      lpszText - Specifies the new document title.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnTitleChange(CXTPWebBrowserCtrl& sender, LPCTSTR lpszText)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(lpszText);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles PropertyChange event for the ShellExplorer WebBrowser provider type. Fires when
	//      the a property of the object is changed.
	// Parameters:
	//      sender       - WebBrowser control that triggered the event.
	//      lpszProperty -  Specifies the name of the property whose value has changed.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnPropertyChange(CXTPWebBrowserCtrl& sender, LPCTSTR lpszProperty)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(lpszProperty);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NewWindow2 event for the ShellExplorer WebBrowser provider type. Fires when a
	//      new window is to be created.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      ppDisp   - An interface pointer that, optionally, receives the IDispatch interface
	//                 pointer of a new WebBrowser object or InternetExplorer object.
	//      pbCancel - Value that determines whether the current navigation should be canceled.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnNewWindow2(CXTPWebBrowserCtrl& sender, LPDISPATCH* ppDisp,
											BOOL* pbCancel)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(ppDisp);
		XTP_UNUSED_PARAMETER(pbCancel);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnQuit event for the ShellExplorer WebBrowser provider type. Fires before the
	//      Windows Internet Explorer application quits.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnQuit(CXTPWebBrowserCtrl& sender)
	{
		XTP_UNUSED_PARAMETER(sender);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnVisible event for the ShellExplorer WebBrowser provider type. Fires when the
	//      Visible property of the object is changed.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      bVisible - Specifies whether the object is visible.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnVisible(CXTPWebBrowserCtrl& sender, BOOL bVisible)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bVisible);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnToolBar event for the ShellExplorer WebBrowser provider type. Fires when the
	//      ToolBar property is changed.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      bToolBar - Specifies whether the toolbar is shown.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnToolBar(CXTPWebBrowserCtrl& sender, BOOL bToolBar)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bToolBar);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnMenuBar event for the ShellExplorer WebBrowser provider type. Fires when the
	//      MenuBar property is changed.
	// Parameters:
	//      sender   - WebBrowser control that triggered the event.
	//      bMenuBar - Specifies whether the menu bar is shown.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnMenuBar(CXTPWebBrowserCtrl& sender, BOOL bMenuBar)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bMenuBar);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnStatusBar event for the ShellExplorer WebBrowser provider type. Fires when the
	//      StatusBar property is changed.
	// Parameters:
	//      sender     - WebBrowser control that triggered the event.
	//      bStatusBar - Specifies whether the status bar is shown.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnStatusBar(CXTPWebBrowserCtrl& sender, BOOL bStatusBar)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bStatusBar);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnFullScreen event for the ShellExplorer WebBrowser provider type. Fires when
	//      the FullScreen property is changed.
	// Parameters:
	//      sender      - WebBrowser control that triggered the event.
	//      bFullScreen - Specifies whether the browser window is in full-screen mode.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnFullScreen(CXTPWebBrowserCtrl& sender, BOOL bFullScreen)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bFullScreen);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles OnTheaterMode event for the ShellExplorer WebBrowser provider type. Fires when
	//      the TheaterMode property is changed.
	// Parameters:
	//      sender       - WebBrowser control that triggered the event.
	//      bTheaterMode - Specifies whether the browser window is in theater mode.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnTheaterMode(CXTPWebBrowserCtrl& sender, BOOL bTheaterMode)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bTheaterMode);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NavigateComplete2 event for the ShellExplorer WebBrowser provider type. Fires
	//      after a navigation to a link is completed on a window element or a frameSet element.
	// Parameters:
	//      sender - WebBrowser control that triggered the event.
	//      pDisp  - A pointer to the IDispatch interface for the WebBrowser object that
	//               represents the window or frame. This interface can be queried for the
	//               IWebBrowser2 interface.
	//      pvtURL - A pointer to a Variant structure of type String that contains the URL, UNC file
	//               name, or PIDL that was navigated to. Note that this URL can be different from
	//               the URL that the browser was directed to navigate to. One reason is that this
	//               URL is the canonicalized and qualified URL. For example, if an application
	//               specified a URL of "www.microsoft.com" in a call to the Navigate method or the
	//               Navigate2 method, the URL passed by Navigate2 is "https://www.microsoft.com/."
	//               If the server has redirected the browser to a different URL, the redirected URL
	//               is reflected.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnNavigateComplete2(CXTPWebBrowserCtrl& sender, LPDISPATCH pDisp,
												   VARIANT* pvtURL)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(pDisp);
		XTP_UNUSED_PARAMETER(pvtURL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles BeforeNavigate2 event for the ShellExplorer WebBrowser provider type. Fires
	//      before navigation occurs in the given object (on either a window element or a frameset
	//      element).
	// Parameters:
	//      sender             - WebBrowser control that triggered the event.
	//      pDisp              - A pointer to the IDispatch interface for the WebBrowser object
	//                           that represents the window or frame. This interface can be queried
	//                           for the IWebBrowser2 interface.
	//      pvtURL             - A pointer to a VARIANT structure of type VT_BSTR that contains the
	//                           URL to navigate to.
	//      pvtFlags           - The result of an external window or tab that targets this browser.
	//                           Contains zero or beforeNavigateExternalFrameTarget (0x1).
	//      pvtTargetFrameName - A pointer to a VARIANT structure of type VT_BSTR that contains the
	//                           name of the frame in which to display the resource, or NULL, if no
	//                           named frame is targeted for the resource.
	//      pvtPostData        - A pointer to a VARIANT structure of type VT_BYREF|VT_VARIANT that
	//                           contains the data to send to the server if the HTTP POST
	//                           transaction is used.
	//      pvtHeaders         - A pointer to a VARIANT structure of type VT_BSTR that contains
	//                           additional HTTP headers to send to the server (HTTPURLs only). The
	//                           headers can specify information, such as the action required of the
	//                           server, the type of data passed to the server, or a status code.
	//      pbCancel           - An application can set this parameter to TRUE to cancel the
	//                           navigation operation, or to FALSE to allow the navigation operation
	//                           to proceed.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnBeforeNavigate2(CXTPWebBrowserCtrl& sender, LPDISPATCH pDisp,
												 VARIANT* pvtURL, VARIANT* pvtFlags,
												 VARIANT* pvtTargetFrameName, VARIANT* pvtPostData,
												 VARIANT* pvtHeaders, BOOL* pbCancel)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(pDisp);
		XTP_UNUSED_PARAMETER(pvtURL);
		XTP_UNUSED_PARAMETER(pvtFlags);
		XTP_UNUSED_PARAMETER(pvtTargetFrameName);
		XTP_UNUSED_PARAMETER(pvtPostData);
		XTP_UNUSED_PARAMETER(pvtHeaders);
		XTP_UNUSED_PARAMETER(pbCancel);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles DocumentComplete event for the ShellExplorer WebBrowser provider type.
	//      Fires when a document is completely loaded and initialized.
	// Parameters:
	//      sender       - WebBrowser control that triggered the event.
	//      pDisp        - A pointer to the IDispatch interface of the window or frame in which
	//                     the document is loaded. This IDispatch interface can be queried for the
	//                     IWebBrowser2 interface.
	//      pvtURL       - A pointer to a Variant structure of type String that specifies the URL,
	//                     UNC file name, or a PIDL of the loaded document.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnDocumentComplete(CXTPWebBrowserCtrl& sender, LPDISPATCH pDisp,
												  VARIANT* pvtURL)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(pDisp);
		XTP_UNUSED_PARAMETER(pvtURL);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NavigateError event for the ShellExplorer WebBrowser provider type.
	//      Fires when an error occurs during navigation.
	// Parameters:
	//      sender       - WebBrowser control that triggered the event.
	//      pDisp        - A pointer to an IDispatch interface for the WebBrowser object that
	//                     represents the window or frame in which the navigation error occurred.
	//                     This interface can be queried for the IWebBrowser2 interface.
	//      pvtURL       - A pointer to a VARIANT structure of type VT_BSTR that contains the
	//                     URL for which navigation failed.
	//      pvtTargetFrameName - A pointer to a VARIANT structure of type VT_BSTR that contains
	//                           the name of the frame in which to display the resource, or NULL if
	//                           no named frame was targeted for the resource.
	//      pvtStatusCode - A pointer to a VT_I4 containing an error status code, if available.
	//                      For a list of the possible HRESULT and HTTP status codes, see
	//                      NavigateError Event Status Codes.
	//      pbCancel - Specifies whether to cancel the navigation to an error page or
	//                 to any further autosearch.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnNavigateError(CXTPWebBrowserCtrl& sender, LPDISPATCH pDisp,
											   VARIANT* pvtURL, VARIANT* pvtTargetFrameName,
											   VARIANT* pvtStatusCode, BOOL* pbCancel)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(pDisp);
		XTP_UNUSED_PARAMETER(pvtURL);
		XTP_UNUSED_PARAMETER(pvtTargetFrameName);
		XTP_UNUSED_PARAMETER(pvtStatusCode);
		XTP_UNUSED_PARAMETER(pbCancel);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowSetResizable event for the ShellExplorer WebBrowser provider type.
	//      Fires to indicate whether the host window should allow resizing of the object.
	// Parameters:
	//      sender       - WebBrowser control that triggered the event.
	//      bResizable   - pecifies whether the WebBrowser control is resizable.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowSetResizable(CXTPWebBrowserCtrl& sender, BOOL bResizable)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bResizable);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowSetLeft event for the ShellExplorer WebBrowser provider type.
	//      Fires when the object changes its left position.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      nLeft   - Specifies the new left position of the WebBrowser window.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowSetLeft(CXTPWebBrowserCtrl& sender, long nLeft)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nLeft);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowSetTop event for the ShellExplorer WebBrowser provider type.
	//      Fires when the object changes its top position.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      nTop    - Specifies the new top position of the WebBrowser window.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowSetTop(CXTPWebBrowserCtrl& sender, long nTop)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nTop);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowSetWidth event for the ShellExplorer WebBrowser provider type.
	//      Fires when the object changes its width.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      nWidth  - Specifies the new width of the WebBrowser window.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowSetWidth(CXTPWebBrowserCtrl& sender, long nWidth)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nWidth);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowSetHeight event for the ShellExplorer WebBrowser provider type.
	//      Fires when the object changes its height.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      nHeight - Specifies the new height of the WebBrowser window.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowSetHeight(CXTPWebBrowserCtrl& sender, long nHeight)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nHeight);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowClosing event for the ShellExplorer WebBrowser provider type.
	//      Fires when the window of the object is about to be closed by script.
	// Parameters:
	//      sender         - WebBrowser control that triggered the event.
	//      bIsChildWindow - Specifies whether the window was created from script.
	//      pbCancel       - Specifies whether the window is prevented from closing.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnWindowClosing(CXTPWebBrowserCtrl& sender, BOOL bIsChildWindow,
											   BOOL FAR* pbCancel)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(bIsChildWindow);
		XTP_UNUSED_PARAMETER(pbCancel);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles SetSecureLockIcon event for the ShellExplorer WebBrowser provider type.
	//      Fires when there is a change in encryption level.
	// Parameters:
	//      sender          - WebBrowser control that triggered the event.
	//      nSecureLockIcon - specifies one of the SecureLockIconConstants from Windows SDK.
	//-----------------------------------------------------------------------
	virtual void ShellExplorer_OnSetSecureLockIcon(CXTPWebBrowserCtrl& sender, long nSecureLockIcon)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(nSecureLockIcon);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NavigationStarting event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_NavigationStarting(CXTPWebBrowserCtrl& sender,
												ICoreWebView2NavigationStartingEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles ContentLoading event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_ContentLoading(CXTPWebBrowserCtrl& sender,
											ICoreWebView2ContentLoadingEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles SourceChanged event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_SourceChanged(CXTPWebBrowserCtrl& sender,
										   ICoreWebView2SourceChangedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles HistoryChanged event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_HistoryChanged(CXTPWebBrowserCtrl& sender, IUnknown* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NavigationCompleted event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_NavigationCompleted(CXTPWebBrowserCtrl& sender,
												 ICoreWebView2NavigationCompletedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles FrameNavigationStarting event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_FrameNavigationStarting(CXTPWebBrowserCtrl& sender,
													 ICoreWebView2NavigationStartingEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles FrameNavigationCompleted event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_FrameNavigationCompleted(
		CXTPWebBrowserCtrl& sender, ICoreWebView2NavigationCompletedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles ScriptDialogOpening event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_ScriptDialogOpening(CXTPWebBrowserCtrl& sender,
												 ICoreWebView2ScriptDialogOpeningEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles PermissionRequested event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_PermissionRequested(CXTPWebBrowserCtrl& sender,
												 ICoreWebView2PermissionRequestedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles ProcessFailed event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_ProcessFailed(CXTPWebBrowserCtrl& sender,
										   ICoreWebView2ProcessFailedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles NewWindowRequested event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_NewWindowRequested(CXTPWebBrowserCtrl& sender,
												ICoreWebView2NewWindowRequestedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles DocumentTitleChanged event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_DocumentTitleChanged(CXTPWebBrowserCtrl& sender, IUnknown* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles ContainsFullScreenElementChanged event for the WebView2 WebBrowser provider
	//      type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_ContainsFullScreenElementChanged(CXTPWebBrowserCtrl& sender,
															  IUnknown* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WebResourceRequested event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_WebResourceRequested(CXTPWebBrowserCtrl& sender,
												  ICoreWebView2WebResourceRequestedEventArgs* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles WindowCloseRequested event for the WebView2 WebBrowser provider type.
	// Parameters:
	//      sender  - WebBrowser control that triggered the event.
	//      args    - Object that describes event arguments.
	//-----------------------------------------------------------------------
	virtual HRESULT WebView2_WindowCloseRequested(CXTPWebBrowserCtrl& sender, IUnknown* args)
	{
		XTP_UNUSED_PARAMETER(sender);
		XTP_UNUSED_PARAMETER(args);
		return S_OK;
	}
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPWEBBROWSERABSTRACT_H__)
