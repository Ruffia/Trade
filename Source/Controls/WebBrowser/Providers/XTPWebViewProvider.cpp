// XTPWebViewProvider.cpp : Implementation of the CXTPWebViewProvider ActiveX Control class.
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <atlbase.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"

#include "Controls/WebBrowser/XTPWebBrowserAbstract.h"
#include "Controls/WebBrowser/XTPWebBrowserCtrl.h"
#include "Controls/WebBrowser/Providers/XTPWebBrowserProvider.h"
#include "Controls/WebBrowser/Providers/XTPWebViewProvider.h"

#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"

#ifdef XTP_USE_WEBVIEW2_STATIC_LIB
#	pragma comment(lib, "Version.lib")
#	pragma comment(lib, "WebView2LoaderStatic.lib")
#endif

#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT

#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#undef AFX_DATA
#define AFX_DATA

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

#	define XTP_WEBVIEW2_HANDLER(name) CXTPCoreWebView2##name##Handler

#	define XTP_WEBVIEW2_CREATE_HANDLER2(provider, name, baseName, var)                            \
		CComPtr<ICoreWebView2##baseName##Handler> var =                                            \
			XTPCreateCoObject<CXTPCoreWebView2##name##Handler, ICoreWebView2##baseName##Handler,   \
							  CXTPWebViewProvider>(provider)

#	define XTP_WEBVIEW2_CREATE_HANDLER(provider, name, var)                                       \
		XTP_WEBVIEW2_CREATE_HANDLER2(provider, name, name, var)

#	define XTP_WEBVIEW2_BEGIN_HANDLER2(name, baseName, args)                                      \
		class CXTPCoreWebView2##name##Handler : public CXTPCmdTarget                               \
		{                                                                                          \
		public:                                                                                    \
			CXTPCoreWebView2##name##Handler(CXTPWebViewProvider& provider)                         \
				: m_Provider(provider)                                                             \
			{                                                                                      \
				m_Provider.GetControllingUnknown()->AddRef();                                      \
			}                                                                                      \
                                                                                                   \
			~CXTPCoreWebView2##name##Handler()                                                     \
			{                                                                                      \
				m_Provider.GetControllingUnknown()->Release();                                     \
			}                                                                                      \
                                                                                                   \
		private:                                                                                   \
			CXTPWebViewProvider& m_Provider;                                                       \
                                                                                                   \
			DECLARE_INTERFACE_MAP()                                                                \
                                                                                                   \
			BEGIN_INTERFACE_PART(CoreWebView2##name##Handler, ICoreWebView2##baseName##Handler)    \
                                                                                                   \
			virtual HRESULT STDMETHODCALLTYPE Invoke args                                          \
			{                                                                                      \
				METHOD_PROLOGUE(CXTPCoreWebView2##name##Handler, CoreWebView2##name##Handler);     \
				CXTPWebViewProvider& Provider = pThis->m_Provider;

#	define XTP_WEBVIEW2_BEGIN_HANDLER(name, args) XTP_WEBVIEW2_BEGIN_HANDLER2(name, name, args)

#	define XTP_WEBVIEW2_END_HANDLER2(name, baseName)                                              \
		}                                                                                          \
		END_INTERFACE_PART(CoreWebView2##name##Handler)                                            \
		}                                                                                          \
		;                                                                                          \
                                                                                                   \
		BEGIN_INTERFACE_MAP(CXTPCoreWebView2##name##Handler, CXTPCmdTarget)                        \
			INTERFACE_PART(CXTPCoreWebView2##name##Handler, IID_ICoreWebView2##baseName##Handler,  \
						   CoreWebView2##name##Handler)                                            \
		END_INTERFACE_MAP()                                                                        \
		XTP_IMPLEMENT_IUNKNOWN(CXTPCoreWebView2##name##Handler, CoreWebView2##name##Handler);

#	define XTP_WEBVIEW2_END_HANDLER(name) XTP_WEBVIEW2_END_HANDLER2(name, name)

#	define XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER2(name, baseName, argsType)                        \
		XTP_WEBVIEW2_BEGIN_HANDLER2(name##Event, baseName##Event,                                  \
									(ICoreWebView2* /*view*/, argsType * args))                    \
		{                                                                                          \
			static_cast<CXTPWebViewProviderSpy&>(Provider).OnBefore##name(args);                   \
			HRESULT hr = static_cast<CXTPWebViewProviderSpy&>(Provider).m_Host.WebView2_##name(    \
				args);                                                                             \
			static_cast<CXTPWebViewProviderSpy&>(Provider).OnAfter##name(args);                    \
			return hr;                                                                             \
		}                                                                                          \
		XTP_WEBVIEW2_END_HANDLER2(name##Event, baseName##Event)

#	define XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(name, argsType)                                   \
		XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER2(name, name, argsType)

template<class T, class I, class Arg>
inline CComPtr<I> XTPCreateCoObject(Arg& arg)
{
	T* obj = new T(arg);
	CComPtr<I> ptr;
	IUnknown* unk = obj->GetControllingUnknown();
	VERIFY(S_OK == unk->QueryInterface(&ptr));
	unk->Release();
	return ptr;
}

class CXTPWebViewProviderSpy : public CXTPWebViewProvider
{
	CXTPWebViewProviderSpy(IXTPWebBrowserHost& host);

public:
	using CXTPWebViewProvider::GetUrl;
	using CXTPWebViewProvider::HookEvents;
	using CXTPWebViewProvider::Layout;
	using CXTPWebViewProvider::m_bControllerCreationHandled;
	using CXTPWebViewProvider::m_Host;
	using CXTPWebViewProvider::m_pvtScriptResult;
	using CXTPWebViewProvider::m_pWebView;
	using CXTPWebViewProvider::m_pWebViewController;
	using CXTPWebViewProvider::Navigate;
	using CXTPWebViewProvider::OnAfterContainsFullScreenElementChanged;
	using CXTPWebViewProvider::OnAfterContentLoading;
	using CXTPWebViewProvider::OnAfterDocumentTitleChanged;
	using CXTPWebViewProvider::OnAfterFrameNavigationCompleted;
	using CXTPWebViewProvider::OnAfterFrameNavigationStarting;
	using CXTPWebViewProvider::OnAfterHistoryChanged;
	using CXTPWebViewProvider::OnAfterNavigationCompleted;
	using CXTPWebViewProvider::OnAfterNavigationStarting;
	using CXTPWebViewProvider::OnAfterNewWindowRequested;
	using CXTPWebViewProvider::OnAfterPermissionRequested;
	using CXTPWebViewProvider::OnAfterProcessFailed;
	using CXTPWebViewProvider::OnAfterScriptDialogOpening;
	using CXTPWebViewProvider::OnAfterSourceChanged;
	using CXTPWebViewProvider::OnAfterWebResourceRequested;
	using CXTPWebViewProvider::OnAfterWindowCloseRequested;
	using CXTPWebViewProvider::OnBeforeContainsFullScreenElementChanged;
	using CXTPWebViewProvider::OnBeforeContentLoading;
	using CXTPWebViewProvider::OnBeforeDocumentTitleChanged;
	using CXTPWebViewProvider::OnBeforeFrameNavigationCompleted;
	using CXTPWebViewProvider::OnBeforeFrameNavigationStarting;
	using CXTPWebViewProvider::OnBeforeHistoryChanged;
	using CXTPWebViewProvider::OnBeforeNavigationCompleted;
	using CXTPWebViewProvider::OnBeforeNavigationStarting;
	using CXTPWebViewProvider::OnBeforeNewWindowRequested;
	using CXTPWebViewProvider::OnBeforePermissionRequested;
	using CXTPWebViewProvider::OnBeforeProcessFailed;
	using CXTPWebViewProvider::OnBeforeScriptDialogOpening;
	using CXTPWebViewProvider::OnBeforeSourceChanged;
	using CXTPWebViewProvider::OnBeforeWebResourceRequested;
	using CXTPWebViewProvider::OnBeforeWindowCloseRequested;
	using CXTPWebViewProvider::OnCreateBrowser;
	using CXTPWebViewProvider::OnExternalReady;
	using CXTPWebViewProvider::SetPendingExternalReady;
	using CXTPWebViewProvider::UpdateExternal;
};

#	pragma warning(push)
// C4616: #pragma warning : warning number '5204' out of range, must be between '4001' and '4999'
// C5204: '...': class has virtual functions, but its trivial destructor is not virtual; instances
// of objects derived from this class may not be destructed correctly
#	pragma warning(disable : 4616 5204)

XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(NavigationStarting, ICoreWebView2NavigationStartingEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(ContentLoading, ICoreWebView2ContentLoadingEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(SourceChanged, ICoreWebView2SourceChangedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(HistoryChanged, IUnknown);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(NavigationCompleted,
									 ICoreWebView2NavigationCompletedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER2(FrameNavigationStarting, NavigationStarting,
									  ICoreWebView2NavigationStartingEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER2(FrameNavigationCompleted, NavigationCompleted,
									  ICoreWebView2NavigationCompletedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(ScriptDialogOpening,
									 ICoreWebView2ScriptDialogOpeningEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(PermissionRequested,
									 ICoreWebView2PermissionRequestedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(ProcessFailed, ICoreWebView2ProcessFailedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(NewWindowRequested, ICoreWebView2NewWindowRequestedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(DocumentTitleChanged, IUnknown);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(ContainsFullScreenElementChanged, IUnknown);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(WebResourceRequested,
									 ICoreWebView2WebResourceRequestedEventArgs);
XTP_WEBVIEW2_IMPLEMENT_EVENT_HANDLER(WindowCloseRequested, IUnknown);

///////////////////////////////////////////////////////////////////////////////
// CreateCoreWebView2ControllerCompleted handler

XTP_WEBVIEW2_BEGIN_HANDLER(CreateCoreWebView2ControllerCompleted,
						   (HRESULT errorCode, ICoreWebView2Controller* controller))
{
	XTP_UNUSED_PARAMETER(errorCode);

	CXTPWebViewProviderSpy& provider = static_cast<CXTPWebViewProviderSpy&>(Provider);

	if (NULL == controller)
	{
		TRACE(_T("ICoreWebView2Controller not specified\n"));
		provider.m_bControllerCreationHandled = TRUE;
		return S_OK;
	}

	provider.m_pWebViewController = controller;
	provider.m_pWebViewController->AddRef();

	HRESULT hr = provider.m_pWebViewController->get_CoreWebView2(&provider.m_pWebView);
	if (FAILED(hr))
	{
		TRACE(_T("get_CoreWebView2 failed\n"));
		provider.m_bControllerCreationHandled = TRUE;
		return hr;
	}

	if (NULL == provider.m_pWebView)
	{
		TRACE(_T("ICoreWebView2 not provided\n"));
		provider.m_bControllerCreationHandled = TRUE;
		return E_NOINTERFACE;
	}

	provider.OnCreateBrowser(provider.m_pWebViewController);

	provider.m_bControllerCreationHandled = TRUE;

	return S_OK;
}
XTP_WEBVIEW2_END_HANDLER(CreateCoreWebView2ControllerCompleted)

///////////////////////////////////////////////////////////////////////////////
// ExecuteScriptCompleted handler

XTP_WEBVIEW2_BEGIN_HANDLER(ExecuteScriptCompleted, (HRESULT errorCode, LPCWSTR resultObjectAsJson))
{
	XTP_UNUSED_PARAMETER(errorCode);

	CXTPWebViewProviderSpy& provider = static_cast<CXTPWebViewProviderSpy&>(Provider);

	CComVariant*& pResult = provider.m_pvtScriptResult;
	if (NULL != pResult)
	{
		if (NULL != resultObjectAsJson)
			*pResult = CComBSTR(resultObjectAsJson);
		else
			pResult->Clear();

		pResult = NULL;
	}

	provider.OnExternalReady();

	return S_OK;
}
XTP_WEBVIEW2_END_HANDLER(ExecuteScriptCompleted)

///////////////////////////////////////////////////////////////////////////////
// CreateCoreWebView2EnvironmentCompleted handler

XTP_WEBVIEW2_BEGIN_HANDLER(CreateCoreWebView2EnvironmentCompleted,
						   (HRESULT errorCode, ICoreWebView2Environment* createdEnvironment))
{
	XTP_UNUSED_PARAMETER(errorCode);

	XTP_WEBVIEW2_CREATE_HANDLER(Provider, CreateCoreWebView2ControllerCompleted, pCtrlHandler);

	CXTPWebViewProviderSpy& provider = static_cast<CXTPWebViewProviderSpy&>(Provider);

	provider.m_bControllerCreationHandled = FALSE;

	HWND hWnd = provider.m_Host.GetHostWindow();
	if (S_OK != createdEnvironment->CreateCoreWebView2Controller(hWnd, pCtrlHandler))
		return FALSE;

	while (!provider.m_bControllerCreationHandled)
		XTPGetThread()->PumpMessage();

	return S_OK;
}
XTP_WEBVIEW2_END_HANDLER(CreateCoreWebView2EnvironmentCompleted)

#	pragma warning(pop)

#endif

///////////////////////////////////////////////////////////////////////////////
// CXTPWebViewProvider

#ifndef XTP_USE_WEBVIEW2_STATIC_LIB
HMODULE CXTPWebViewProvider::m_hWebView2Loader = NULL;
#endif
CXTPWebViewProvider::PFNGetAvailableCoreWebView2BrowserVersionString
	CXTPWebViewProvider::m_pfnGetAvailableCoreWebView2BrowserVersionString = NULL;
CXTPWebViewProvider::PFNCreateCoreWebView2EnvironmentWithOptions
	CXTPWebViewProvider::m_pfnCreateCoreWebView2EnvironmentWithOptions = NULL;

IMPLEMENT_DYNAMIC(CXTPWebViewProvider, CXTPWebBrowserProvider)

void CXTPWebViewProvider::StaticInit()
{
	static BOOL bInitAttempted = FALSE;

	if (bInitAttempted)
		return;

	bInitAttempted = TRUE;

#ifndef XTP_USE_WEBVIEW2_STATIC_LIB

	m_hWebView2Loader = ::LoadLibrary(
#	ifdef _WIN64
		_T("WebView2Loader64.dll")
#	else
		_T("WebView2Loader32.dll")
#	endif
	);

	if (NULL != m_hWebView2Loader)
	{
		m_pfnGetAvailableCoreWebView2BrowserVersionString =
			XTPToFunctionPtr<PFNGetAvailableCoreWebView2BrowserVersionString>(
				::GetProcAddress(m_hWebView2Loader,
								 "GetAvailableCoreWebView2BrowserVersionString"));
		m_pfnCreateCoreWebView2EnvironmentWithOptions =
			XTPToFunctionPtr<PFNCreateCoreWebView2EnvironmentWithOptions>(
				::GetProcAddress(m_hWebView2Loader, "CreateCoreWebView2EnvironmentWithOptions"));
	}
	else
	{
		m_pfnGetAvailableCoreWebView2BrowserVersionString = NULL;
		m_pfnCreateCoreWebView2EnvironmentWithOptions	 = NULL;
	}

#else

	m_pfnGetAvailableCoreWebView2BrowserVersionString = GetAvailableCoreWebView2BrowserVersionString;
	m_pfnCreateCoreWebView2EnvironmentWithOptions	 = CreateCoreWebView2EnvironmentWithOptions;

#endif
}

CXTPWebViewProvider::CXTPWebViewProvider(IXTPWebBrowserHost& host)
	: CXTPWebBrowserProvider(host)
	, m_pWebView(NULL)
	, m_pWebViewController(NULL)
	, m_pvtScriptResult(NULL)
	, m_bReadyToReturnFromScript(FALSE)
	, m_bPendingExternalReady(FALSE)
	, m_pvtDummyResult(new CComVariant)
	, m_bControllerCreationHandled(FALSE)
{
	StaticInit();

	m_Host.GetHostUnknown()->AddRef();
}

CXTPWebViewProvider::~CXTPWebViewProvider()
{
	ReleaseBrowser();

	m_Host.GetHostUnknown()->Release();

	delete m_pvtDummyResult;
}

BOOL CXTPWebViewProvider::IsAvailable()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	LPWSTR pVersion = NULL;

	StaticInit();

	if (NULL == m_pfnGetAvailableCoreWebView2BrowserVersionString)
		return FALSE;

	if (S_OK != m_pfnGetAvailableCoreWebView2BrowserVersionString(NULL, &pVersion))
		return FALSE;

	return (NULL != pVersion);
#else

	return FALSE;

#endif
}

HRESULT CXTPWebViewProvider::ExecuteScript(LPCWSTR pText, CComVariant* pvtResult /*= NULL*/)
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	ASSERT(NULL != pText);
	ASSERT("Expecting script result before complete page loading is not supported due to "
		   "asynchronous result processing."
		   && (m_bReadyToReturnFromScript ? TRUE : NULL == pvtResult));

	if (NULL == m_pWebView)
		return E_PENDING;

	if (NULL != pvtResult && m_bReadyToReturnFromScript)
	{
		m_pvtScriptResult = pvtResult;
		XTP_WEBVIEW2_CREATE_HANDLER(*this, ExecuteScriptCompleted, pScriptHandler);
		HRESULT hr = m_pWebView->ExecuteScript(pText, pScriptHandler);
		if (FAILED(hr))
		{
			m_pvtScriptResult = NULL;
			return hr;
		}

		while (NULL != m_pvtScriptResult)
			if (!XTPGetThread()->PumpMessage())
				break;
	}
	else
	{
		HRESULT hr = m_pWebView->ExecuteScript(pText, NULL);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;

#else

	XTP_UNUSED_PARAMETER(pText);
	XTP_UNUSED_PARAMETER(pvtResult);

	return E_NOTIMPL;

#endif
}

HRESULT CXTPWebViewProvider::Initialize()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	if (NULL == m_pfnCreateCoreWebView2EnvironmentWithOptions)
		return E_FAIL;

	const COleVariant* pvtUDF = GetHost().GetProperty(_T("UDF"));
	LPCWSTR lpUDF			  = (NULL != pvtUDF && VT_BSTR == pvtUDF->vt && NULL != pvtUDF->bstrVal
						 ? pvtUDF->bstrVal
						 : NULL);

	XTP_WEBVIEW2_CREATE_HANDLER(*this, CreateCoreWebView2EnvironmentCompleted, pEnvHandler);
	return m_pfnCreateCoreWebView2EnvironmentWithOptions(NULL, lpUDF, NULL, pEnvHandler);

#else

	return E_NOTIMPL;

#endif
}

HRESULT CXTPWebViewProvider::Navigate(LPCTSTR pUrl)
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	ASSERT(NULL != pUrl);
	if (NULL == pUrl)
		return E_POINTER;

	if (NULL == m_pWebView)
	{
		// The browser is not yet initialized so the URL is saved for automatic navigation
		// once it is initialized.
		SetUrl(pUrl);
		return S_FALSE;
	}

	HRESULT hr = m_pWebView->Navigate(XTP_CT2CW(pUrl));
	if (S_OK == hr)
		SetUrl(pUrl);

	return hr;

#else
	XTP_UNUSED_PARAMETER(pUrl);

	return E_NOTIMPL;

#endif
}

void CXTPWebViewProvider::ReleaseBrowser()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	if (NULL != m_pWebView)
	{
		OnDestroyBrowser();
		UnhookEvents();
		SAFE_RELEASE(m_pWebView);
	}

	SAFE_RELEASE(m_pWebViewController);

#endif
}

BOOL CXTPWebViewProvider::OnHostWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	XTP_UNUSED_PARAMETER(pResult);

	ASSERT(NULL != pResult);

	switch (message)
	{
		case WM_SIZE: return OnSize(XTPToUInt(wParam), LOWORD(lParam), HIWORD(lParam));
		case WM_DESTROY: return OnDestroy();
	}

#else

	XTP_UNUSED_PARAMETER(message);
	XTP_UNUSED_PARAMETER(wParam);
	XTP_UNUSED_PARAMETER(lParam);
	XTP_UNUSED_PARAMETER(pResult);

#endif

	return FALSE;
}

void CXTPWebViewProvider::OnCreateBrowser(IUnknown* pObject)
{
	ASSERT(NULL != m_pWebViewController);
	ASSERT(NULL != m_pWebView);

	CXTPWebBrowserProvider::OnCreateBrowser(pObject);

	m_pWebViewController->put_IsVisible(TRUE);

	ApplyUserAgent();
	HookEvents();
	SetPendingExternalReady();
	UpdateExternal();
	Layout();

	if (NULL == m_Host.GetExternal())
	{
		// If external is provided then initial navigation will occur in OnExternalReady as a result
		// of UpdateExternal call above.
		if (!GetUrl().IsEmpty())
			Navigate(GetUrl());
	}
}

void CXTPWebViewProvider::UpdateExternal()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	if (NULL == m_pWebView)
		return;

	IDispatch* pExternal = m_Host.GetExternal();
	if (NULL == pExternal)
		return;

	VARIANT vtValue;
	vtValue.vt		 = VT_DISPATCH;
	vtValue.pdispVal = pExternal;
	m_pWebView->AddHostObjectToScript(L"external", &vtValue);

	m_pvtScriptResult = m_pvtDummyResult;

	static const WCHAR szExternalUpdateCode[] = L"(async ()=> { window.external = await "
												L"chrome.webview.hostObjects.external.sync(); })()";
	if (m_bPendingExternalReady)
	{
		XTP_WEBVIEW2_CREATE_HANDLER(*this, ExecuteScriptCompleted, pHandler);
		if (FAILED(m_pWebView->ExecuteScript(szExternalUpdateCode, pHandler)))
			m_bPendingExternalReady = FALSE;
	}
	else
	{
		m_pWebView->ExecuteScript(szExternalUpdateCode, NULL);
	}

#endif
}

void CXTPWebViewProvider::ApplyUserAgent()
{
	const COleVariant* pvtUserAgent = m_Host.GetProperty(_T("UserAgent"));
	LPCWSTR pUserAgent				= XTP_WEBBROWSER_RESOLVE_USERAGENT(pvtUserAgent);
	if (NULL == pUserAgent)
		return;

	CComPtr<ICoreWebView2Settings> pSettings;
	if (FAILED(m_pWebView->get_Settings(&pSettings)) || !pSettings)
	{
		TRACE(_T("ICoreWebView2Settings not obtained\n"));
		return;
	}

	CComQIPtr<ICoreWebView2Settings2> pSettings2 = (IUnknown*)pSettings;
	if (!pSettings2)
	{
		TRACE(_T("ICoreWebView2Settings2 not obtained\n"));
		return;
	}

	if (FAILED(pSettings2->put_UserAgent(pUserAgent)))
	{
		TRACE(_T("UserAgent not accepted\n"));
		return;
	}
}

void CXTPWebViewProvider::HookEvents()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	ASSERT(NULL != m_pWebView);

	ZeroMemory(&m_eventTokens, sizeof(m_eventTokens));

#	define XTP_WEBVIEW2_ADD_HANDLER2(name, baseName)                                              \
		XTP_WEBVIEW2_CREATE_HANDLER2(*this, name##Event, baseName##Event, p##name##EventHandler);  \
		if (S_OK != m_pWebView->add_##name(p##name##EventHandler, &m_eventTokens.name))            \
		{                                                                                          \
			TRACE(_T("Unable to add WebView2 event handler: ") _T(#name) _T("\n"));                \
		}

#	define XTP_WEBVIEW2_ADD_HANDLER(name) XTP_WEBVIEW2_ADD_HANDLER2(name, name)

	XTP_WEBVIEW2_ADD_HANDLER(NavigationStarting);
	XTP_WEBVIEW2_ADD_HANDLER(NavigationCompleted);
	XTP_WEBVIEW2_ADD_HANDLER(ContentLoading);
	XTP_WEBVIEW2_ADD_HANDLER(SourceChanged);
	XTP_WEBVIEW2_ADD_HANDLER(HistoryChanged);
	XTP_WEBVIEW2_ADD_HANDLER2(FrameNavigationStarting, NavigationStarting);
	XTP_WEBVIEW2_ADD_HANDLER2(FrameNavigationCompleted, NavigationCompleted);
	XTP_WEBVIEW2_ADD_HANDLER(ScriptDialogOpening);
	XTP_WEBVIEW2_ADD_HANDLER(PermissionRequested);
	XTP_WEBVIEW2_ADD_HANDLER(ProcessFailed);
	XTP_WEBVIEW2_ADD_HANDLER(NewWindowRequested);
	XTP_WEBVIEW2_ADD_HANDLER(DocumentTitleChanged);
	XTP_WEBVIEW2_ADD_HANDLER(ContainsFullScreenElementChanged);
	XTP_WEBVIEW2_ADD_HANDLER(WebResourceRequested);
	XTP_WEBVIEW2_ADD_HANDLER(WindowCloseRequested);

#endif
}

void CXTPWebViewProvider::UnhookEvents()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	ASSERT(NULL != m_pWebView);

#	define XTP_WEBVIEW2_REMOVE_HANDLER(name)                                                      \
		if (0 != m_eventTokens.name.value)                                                         \
			m_pWebView->remove_##name(m_eventTokens.name);

	XTP_WEBVIEW2_REMOVE_HANDLER(NavigationStarting);
	XTP_WEBVIEW2_REMOVE_HANDLER(ContentLoading);
	XTP_WEBVIEW2_REMOVE_HANDLER(SourceChanged);
	XTP_WEBVIEW2_REMOVE_HANDLER(HistoryChanged);
	XTP_WEBVIEW2_REMOVE_HANDLER(NavigationCompleted);
	XTP_WEBVIEW2_REMOVE_HANDLER(FrameNavigationStarting);
	XTP_WEBVIEW2_REMOVE_HANDLER(FrameNavigationCompleted);
	XTP_WEBVIEW2_REMOVE_HANDLER(ScriptDialogOpening);
	XTP_WEBVIEW2_REMOVE_HANDLER(PermissionRequested);
	XTP_WEBVIEW2_REMOVE_HANDLER(ProcessFailed);
	XTP_WEBVIEW2_REMOVE_HANDLER(NewWindowRequested);
	XTP_WEBVIEW2_REMOVE_HANDLER(DocumentTitleChanged);
	XTP_WEBVIEW2_REMOVE_HANDLER(ContainsFullScreenElementChanged);
	XTP_WEBVIEW2_REMOVE_HANDLER(WebResourceRequested);
	XTP_WEBVIEW2_REMOVE_HANDLER(WindowCloseRequested);

	ZeroMemory(&m_eventTokens, sizeof(m_eventTokens));

#endif
}

BOOL CXTPWebViewProvider::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	Layout();
	return FALSE;
}

BOOL CXTPWebViewProvider::OnDestroy()
{
	ReleaseBrowser();
	return FALSE;
}

void CXTPWebViewProvider::Layout()
{
#ifdef XTP_BROWSER_WEBVIEW_SUPPORTED

	if (NULL != m_pWebViewController)
	{
		CRect bounds;
		GetClientRect(m_Host.GetHostWindow(), &bounds);
		m_pWebViewController->put_Bounds(bounds);
	}

#endif
}

void CXTPWebViewProvider::SetPendingExternalReady()
{
	m_bPendingExternalReady = TRUE;
}

void CXTPWebViewProvider::OnExternalReady()
{
	// Calling of this method makes sense only during WebView2 controller initialization. It is
	// called asynchronously.
	if (m_bPendingExternalReady)
	{
		m_bPendingExternalReady = FALSE;

		if (!GetUrl().IsEmpty())
			Navigate(GetUrl());
	}
}

void CXTPWebViewProvider::OnBeforeNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
	m_bReadyToReturnFromScript = FALSE;
	UpdateExternal();
}

void CXTPWebViewProvider::OnBeforeContentLoading(ICoreWebView2ContentLoadingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeSourceChanged(ICoreWebView2SourceChangedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeHistoryChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeNavigationCompleted(
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
	UpdateExternal();
}

void CXTPWebViewProvider::OnBeforeFrameNavigationStarting(
	ICoreWebView2NavigationStartingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
	m_bReadyToReturnFromScript = FALSE;
}

void CXTPWebViewProvider::OnBeforeFrameNavigationCompleted(
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeScriptDialogOpening(
	ICoreWebView2ScriptDialogOpeningEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforePermissionRequested(
	ICoreWebView2PermissionRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeProcessFailed(ICoreWebView2ProcessFailedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeNewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeDocumentTitleChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeContainsFullScreenElementChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeWebResourceRequested(
	ICoreWebView2WebResourceRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnBeforeWindowCloseRequested(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterNavigationStarting(ICoreWebView2NavigationStartingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterContentLoading(ICoreWebView2ContentLoadingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterSourceChanged(ICoreWebView2SourceChangedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterHistoryChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterNavigationCompleted(ICoreWebView2NavigationCompletedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
	m_bReadyToReturnFromScript = TRUE;
}

void CXTPWebViewProvider::OnAfterFrameNavigationStarting(
	ICoreWebView2NavigationStartingEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterFrameNavigationCompleted(
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
	m_bReadyToReturnFromScript = TRUE;
}

void CXTPWebViewProvider::OnAfterScriptDialogOpening(ICoreWebView2ScriptDialogOpeningEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterPermissionRequested(ICoreWebView2PermissionRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterProcessFailed(ICoreWebView2ProcessFailedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterNewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterDocumentTitleChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterContainsFullScreenElementChanged(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterWebResourceRequested(
	ICoreWebView2WebResourceRequestedEventArgs* args)
{
	XTP_UNUSED_PARAMETER(args);
}

void CXTPWebViewProvider::OnAfterWindowCloseRequested(IUnknown* args)
{
	XTP_UNUSED_PARAMETER(args);
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
