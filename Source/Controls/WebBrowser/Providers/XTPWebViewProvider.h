// XTPWebViewProvider.h : Declaration of the CXTPWebViewProvider class.
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

#if !defined(__XTPWEBVIEWPROVIDER_H__)
//{{AFX_CODEJOCK_PRIVATE
#	define __XTPWEBVIEWPROVIDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

struct IXTPWebBrowserHost;
struct ICoreWebView2Controller;
struct ICoreWebView2;

// Use this macro for linking WebView2LoaderStatic.lib instead of linking WebView2Loader.dll
// dynamically.
// #define XTP_USE_WEBVIEW2_STATIC_LIB

//{{AFX_CODEJOCK_PRIVATE
class CXTPWebViewProviderSpy;
//}}AFX_CODEJOCK_PRIVATE

//=======================================================================
// Summary:
//      Implements WebView2 WebBrowser provider and gives access to WebView2 specific
//      functionality.
//=======================================================================
class _XTP_EXT_CLASS CXTPWebViewProvider : public CXTPWebBrowserProvider
{
	DECLARE_DYNAMIC(CXTPWebViewProvider)

	friend class CXTPWebViewProviderSpy;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs provider object.
	// Parameters:
	//      host - The host control interface.
	//-----------------------------------------------------------------------
	CXTPWebViewProvider(IXTPWebBrowserHost& host);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles provider destruction.
	//-----------------------------------------------------------------------
	~CXTPWebViewProvider();

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if WebView2 implementation is available and can be used.
	// Returns:
	//      TRUE if WebView2 implementation is available and can be used, otherwise FALSE.
	//-----------------------------------------------------------------------

	static BOOL IsAvailable();

	//-----------------------------------------------------------------------
	// Summary:
	//      Executes a JavaScript code.
	// Parameters:
	//      pText     - Script code text.
	//      pvtResult - The optional pointer to the variant object that is supposed to receive the
	//                  result of the executed script.
	// Returns:
	//      S_OK if script execution or its attempt are successful or an error code otherwise.
	//-----------------------------------------------------------------------
	virtual HRESULT ExecuteScript(LPCWSTR pText, CComVariant* pvtResult = NULL);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	// CXTPWebBrowserProvider overrides
	virtual HRESULT Initialize();
	virtual HRESULT Navigate(LPCTSTR pUrl);
	virtual void ReleaseBrowser();
	virtual BOOL OnHostWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCreateBrowser(IUnknown* pObject);
	virtual void UpdateExternal();
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL OnSize(UINT nType, int cx, int cy);
	BOOL OnDestroy();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void ApplyUserAgent();
	void HookEvents();
	void UnhookEvents();
	void Layout();
	void SetPendingExternalReady();
	void OnExternalReady();

	void OnBeforeNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	void OnBeforeContentLoading(ICoreWebView2ContentLoadingEventArgs* args);
	void OnBeforeSourceChanged(ICoreWebView2SourceChangedEventArgs* args);
	void OnBeforeHistoryChanged(IUnknown* args);
	void OnBeforeNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	void OnBeforeFrameNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	void OnBeforeFrameNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	void OnBeforeScriptDialogOpening(ICoreWebView2ScriptDialogOpeningEventArgs* args);
	void OnBeforePermissionRequested(ICoreWebView2PermissionRequestedEventArgs* args);
	void OnBeforeProcessFailed(ICoreWebView2ProcessFailedEventArgs* args);
	void OnBeforeNewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args);
	void OnBeforeDocumentTitleChanged(IUnknown* args);
	void OnBeforeContainsFullScreenElementChanged(IUnknown* args);
	void OnBeforeWebResourceRequested(ICoreWebView2WebResourceRequestedEventArgs* args);
	void OnBeforeWindowCloseRequested(IUnknown* args);

	void OnAfterNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	void OnAfterContentLoading(ICoreWebView2ContentLoadingEventArgs* args);
	void OnAfterSourceChanged(ICoreWebView2SourceChangedEventArgs* args);
	void OnAfterHistoryChanged(IUnknown* args);
	void OnAfterNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	void OnAfterFrameNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args);
	void OnAfterFrameNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args);
	void OnAfterScriptDialogOpening(ICoreWebView2ScriptDialogOpeningEventArgs* args);
	void OnAfterPermissionRequested(ICoreWebView2PermissionRequestedEventArgs* args);
	void OnAfterProcessFailed(ICoreWebView2ProcessFailedEventArgs* args);
	void OnAfterNewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args);
	void OnAfterDocumentTitleChanged(IUnknown* args);
	void OnAfterContainsFullScreenElementChanged(IUnknown* args);
	void OnAfterWebResourceRequested(ICoreWebView2WebResourceRequestedEventArgs* args);
	void OnAfterWindowCloseRequested(IUnknown* args);
	//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPWebViewProvider(const CXTPWebViewProvider&);
	CXTPWebViewProvider& operator=(const CXTPWebViewProvider&);

	static void StaticInit();

private:
#	ifndef XTP_USE_WEBVIEW2_STATIC_LIB
	static HMODULE m_hWebView2Loader;
#	endif

	typedef HRESULT(STDAPICALLTYPE* PFNGetAvailableCoreWebView2BrowserVersionString)(
		PCWSTR browserExecutableFolder, LPWSTR* versionInfo);
	static PFNGetAvailableCoreWebView2BrowserVersionString
		m_pfnGetAvailableCoreWebView2BrowserVersionString;

	typedef HRESULT(STDAPICALLTYPE* PFNCreateCoreWebView2EnvironmentWithOptions)(
		PCWSTR browserExecutableFolder, PCWSTR userDataFolder,
		ICoreWebView2EnvironmentOptions* environmentOptions,
		ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler* environmentCreatedHandler);
	static PFNCreateCoreWebView2EnvironmentWithOptions m_pfnCreateCoreWebView2EnvironmentWithOptions;

	ICoreWebView2* m_pWebView;
	ICoreWebView2Controller* m_pWebViewController;
	CComVariant* m_pvtScriptResult;
	BOOL m_bReadyToReturnFromScript;
	BOOL m_bPendingExternalReady;
	CComVariant* m_pvtDummyResult;
	BOOL m_bControllerCreationHandled;
	DWORD m_dwUnusedPadding;

	struct
	{
		EventRegistrationToken NavigationStarting;
		EventRegistrationToken ContentLoading;
		EventRegistrationToken SourceChanged;
		EventRegistrationToken HistoryChanged;
		EventRegistrationToken NavigationCompleted;
		EventRegistrationToken FrameNavigationStarting;
		EventRegistrationToken FrameNavigationCompleted;
		EventRegistrationToken ScriptDialogOpening;
		EventRegistrationToken PermissionRequested;
		EventRegistrationToken ProcessFailed;
		EventRegistrationToken NewWindowRequested;
		EventRegistrationToken DocumentTitleChanged;
		EventRegistrationToken ContainsFullScreenElementChanged;
		EventRegistrationToken WebResourceRequested;
		EventRegistrationToken WindowCloseRequested;
	} m_eventTokens;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif
