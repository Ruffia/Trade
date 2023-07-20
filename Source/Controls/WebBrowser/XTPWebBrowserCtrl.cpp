// XTPWebBrowserCtrl.cpp : implementation of the CXTPWebBrowserCtrl class.
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

#pragma warning(push)
// C4464: relative include path contains '..'
#pragma warning(disable : 4464)

#pragma push_macro("AFX_DATA")
#undef AFX_DATA
#define AFX_DATA AFX_DATA_IMPORT
#if (_MSC_VER >= 1300)
#	include <..\src\mfc\occimpl.h>
#else
#	include <..\src\occimpl.h>
#endif
#pragma pop_macro("AFX_DATA")

#pragma warning(pop)

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

#include "Controls/WebBrowser/XTPWebBrowserAbstract.h"
#include "Controls/WebBrowser/Providers/XTPWebBrowserProvider.h"
#include "Controls/WebBrowser/XTPWebBrowserCtrl.h"
#include "Controls/WebBrowser/Providers/XTPShellExplorerProvider.h"
#include "Controls/WebBrowser/Providers/XTPWebViewProvider.h"

#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#define XTP_WEBBROWSER_CLASSNAME _T("XTPWebBrowserCtrl")

IMPLEMENT_DYNAMIC(CXTPWebBrowserProvider, CXTPCmdTarget)

IMPLEMENT_DYNAMIC(CXTPWebBrowserCtrl, CWnd)

const UINT CXTPWebBrowserCtrl::m_WM_INITIALIZE = ::RegisterWindowMessage(
	_T("CXTPWebBrowserCtrl_WM_INITIALIZE"));

CXTPWebBrowserCtrl::CXTPWebBrowserCtrl(XTPWebBrowserProvider nProvider /*= xtpWebBrowserLatest*/)
	: m_nProvider(nProvider)
	, m_pProvider(NULL)
	, m_bSubclassed(TRUE)
	, m_pEventSink(NULL)
	, m_pExternal(NULL)
{
	if (xtpWebBrowserWebView == nProvider && !CXTPWebViewProvider::IsAvailable())
		m_nProvider = xtpWebBrowserShellExplorer;

	EnableAutomation();

	VERIFY(RegisterWindowClass());
}

CXTPWebBrowserCtrl::~CXTPWebBrowserCtrl()
{
	ReleaseProvider();
	SAFE_RELEASE(m_pExternal);

	POSITION pos = m_mapProps.GetStartPosition();
	while (NULL != pos)
	{
		CString strName;
		COleVariant* pvtValue = NULL;
		m_mapProps.GetNextAssoc(pos, strName, pvtValue);

		if (NULL != pvtValue)
			delete pvtValue;
	}
}

BOOL CXTPWebBrowserCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
								CCreateContext* pContext /*= NULL*/)
{
	return CWnd::Create(XTP_WEBBROWSER_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CXTPWebBrowserCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth,
								  int nHeight, HWND hWndParent, HMENU nIDorHMenu,
								  LPVOID lpParam /*= NULL*/)
{
	return CWnd::CreateEx(dwExStyle, XTP_WEBBROWSER_CLASSNAME, NULL, dwStyle, x, y, nWidth, nHeight,
						  hWndParent, nIDorHMenu, lpParam);
}

BOOL CXTPWebBrowserCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
								  CWnd* pParentWnd, UINT nID, LPVOID lpParam /*= NULL*/)
{
	return CWnd::CreateEx(dwExStyle, XTP_WEBBROWSER_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID,
						  lpParam);
}

const COleVariant* CXTPWebBrowserCtrl::GetProperty(const CString& strName) const
{
	COleVariant* pvtValue = NULL;
	m_mapProps.Lookup(strName, pvtValue);
	return pvtValue;
}

void CXTPWebBrowserCtrl::SetProperty(const CString& strName, COleVariant* pvtValue)
{
	COleVariant* pvtExisting = const_cast<COleVariant*>(GetProperty(strName));
	if (NULL != pvtExisting)
	{
		delete pvtExisting;
		m_mapProps.RemoveKey(strName);
	}

	if (NULL == pvtValue)
		return;

	m_mapProps[strName] = pvtValue;
}

BOOL CXTPWebBrowserCtrl::ReCreateProvider()
{
	if (NULL != m_pProvider)
	{
		m_strPendlingUrl = m_pProvider->GetUrl();
		ReleaseProvider();
	}

	InitializeProvider();

	return TRUE;
}

BOOL CXTPWebBrowserCtrl::SetProviderType(XTPWebBrowserProvider nProvider)
{
	if (nProvider == m_nProvider)
		return TRUE;

	if (xtpWebBrowserWebView == nProvider && !CXTPWebViewProvider::IsAvailable())
		return FALSE;

	if (nProvider == m_nProvider)
		return TRUE;

	if (NULL != m_pProvider)
	{
		m_strPendlingUrl = m_pProvider->GetUrl();
		ReleaseProvider();
	}

	m_nProvider = nProvider;

	InitializeProvider();

	return TRUE;
}

HRESULT CXTPWebBrowserCtrl::Navigate(LPCTSTR pUrl)
{
	ASSERT(NULL != pUrl);

	if (NULL == pUrl)
		return E_POINTER;

	if (NULL == m_pProvider)
	{
		// The browser is not yet initialized so the URL is saved for automatic navigation once it
		// is initialized.
		m_strPendlingUrl = pUrl;
		return S_FALSE;
	}

	return m_pProvider->Navigate(pUrl);
}

HRESULT CXTPWebBrowserCtrl::ExecuteScript(LPCWSTR pText, CComVariant* pvtResult /*= NULL*/)
{
	if (NULL == m_pProvider)
		return E_PENDING;

	return m_pProvider->ExecuteScript(pText, pvtResult);
}

void CXTPWebBrowserCtrl::SetExternal(IDispatch* pExternal)
{
	SAFE_RELEASE(m_pExternal);
	m_pExternal = pExternal;

	if (NULL != pExternal)
		pExternal->AddRef();

	if (NULL != m_pProvider)
		m_pProvider->UpdateExternal();
}

IUnknown* CXTPWebBrowserCtrl::GetHostUnknown() const
{
	return const_cast<CXTPWebBrowserCtrl*>(this)->GetControllingUnknown();
}

CWnd& CXTPWebBrowserCtrl::GetHostWindow() const
{
	return *const_cast<CXTPWebBrowserCtrl*>(this);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnStatusTextChange(LPCTSTR lpszText)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnStatusTextChange(*this, lpszText);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnProgressChange(long nProgress, long nProgressMax)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnProgressChange(*this, nProgress, nProgressMax);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnCommandStateChange(long nCommand, BOOL bEnable)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnCommandStateChange(*this, nCommand, bEnable);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnDownloadBegin()
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnDownloadBegin(*this);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnDownloadComplete()
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnDownloadComplete(*this);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnTitleChange(LPCTSTR lpszText)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnTitleChange(*this, lpszText);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnPropertyChange(LPCTSTR lpszProperty)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnPropertyChange(*this, lpszProperty);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnNewWindow2(LPDISPATCH* ppDisp, BOOL* pbCancel)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnNewWindow2(*this, ppDisp, pbCancel);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnQuit()
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnQuit(*this);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnVisible(BOOL bVisible)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnVisible(*this, bVisible);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnToolBar(BOOL bToolBar)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnToolBar(*this, bToolBar);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnMenuBar(BOOL bMenuBar)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnMenuBar(*this, bMenuBar);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnStatusBar(BOOL bStatusBar)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnStatusBar(*this, bStatusBar);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnFullScreen(BOOL bFullScreen)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnFullScreen(*this, bFullScreen);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnTheaterMode(BOOL bTheaterMode)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnTheaterMode(*this, bTheaterMode);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* pvtURL)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnNavigateComplete2(*this, pDisp, pvtURL);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* pvtURL,
														 VARIANT* pvtFlags,
														 VARIANT* pvtTargetFrameName,
														 VARIANT* pvtPostData, VARIANT* pvtHeaders,
														 BOOL* pbCancel)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnBeforeNavigate2(*this, pDisp, pvtURL, pvtFlags,
													  pvtTargetFrameName, pvtPostData, pvtHeaders,
													  pbCancel);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnDocumentComplete(LPDISPATCH pDisp, VARIANT* pvtURL)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnDocumentComplete(*this, pDisp, pvtURL);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnNavigateError(LPDISPATCH pDisp, VARIANT* pvtURL,
													   VARIANT* pvtTargetFrameName,
													   VARIANT* pvtStatusCode, BOOL* pbCancel)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnNavigateError(*this, pDisp, pvtURL, pvtTargetFrameName,
													pvtStatusCode, pbCancel);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowSetResizable(BOOL bResizable)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowSetResizable(*this, bResizable);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowSetLeft(long nLeft)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowSetLeft(*this, nLeft);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowSetTop(long nTop)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowSetTop(*this, nTop);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowSetWidth(long nWidth)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowSetWidth(*this, nWidth);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowSetHeight(long nHeight)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowSetHeight(*this, nHeight);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnWindowClosing(BOOL bIsChildWindow, BOOL FAR* pbCancel)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnWindowClosing(*this, bIsChildWindow, pbCancel);
}

void CXTPWebBrowserCtrl::ShellExplorer_OnSetSecureLockIcon(long nSecureLockIcon)
{
	if (NULL != m_pEventSink)
		m_pEventSink->ShellExplorer_OnSetSecureLockIcon(*this, nSecureLockIcon);
}

HRESULT
CXTPWebBrowserCtrl::WebView2_NavigationStarting(ICoreWebView2NavigationStartingEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_NavigationStarting(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_ContentLoading(ICoreWebView2ContentLoadingEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_ContentLoading(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_SourceChanged(ICoreWebView2SourceChangedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_SourceChanged(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_HistoryChanged(IUnknown* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_HistoryChanged(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_NavigationCompleted(
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_NavigationCompleted(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_FrameNavigationStarting(
	ICoreWebView2NavigationStartingEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_FrameNavigationStarting(*this, args)
								 : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_FrameNavigationCompleted(
	ICoreWebView2NavigationCompletedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_FrameNavigationCompleted(*this, args)
								 : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_ScriptDialogOpening(
	ICoreWebView2ScriptDialogOpeningEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_ScriptDialogOpening(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_PermissionRequested(
	ICoreWebView2PermissionRequestedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_PermissionRequested(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_ProcessFailed(ICoreWebView2ProcessFailedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_ProcessFailed(*this, args) : S_OK);
}

HRESULT
CXTPWebBrowserCtrl::WebView2_NewWindowRequested(ICoreWebView2NewWindowRequestedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_NewWindowRequested(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_DocumentTitleChanged(IUnknown* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_DocumentTitleChanged(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_ContainsFullScreenElementChanged(IUnknown* args)
{
	return (NULL != m_pEventSink
				? m_pEventSink->WebView2_ContainsFullScreenElementChanged(*this, args)
				: S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_WebResourceRequested(
	ICoreWebView2WebResourceRequestedEventArgs* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_WebResourceRequested(*this, args) : S_OK);
}

HRESULT CXTPWebBrowserCtrl::WebView2_WindowCloseRequested(IUnknown* args)
{
	return (NULL != m_pEventSink ? m_pEventSink->WebView2_WindowCloseRequested(*this, args) : S_OK);
}

IUnknown* CXTPWebBrowserCtrl::GetBrowserObject() const
{
	if (NULL == m_pProvider)
		return NULL;

	return m_pProvider->GetBrowserObject();
}

BOOL CXTPWebBrowserCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!::GetClassInfo(hInst, XTP_WEBBROWSER_CLASSNAME, &wndcls))
	{
		wndcls.style	   = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance					  = hInst;
		wndcls.hIcon						  = NULL;
		wndcls.hCursor						  = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground				  = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
		wndcls.lpszMenuName					  = NULL;
		wndcls.lpszClassName				  = XTP_WEBBROWSER_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CXTPWebBrowserCtrl::InitializeProvider()
{
	ASSERT(NULL == m_pProvider);
	if (NULL != m_pProvider)
		return FALSE;

	switch (m_nProvider)
	{
		case xtpWebBrowserShellExplorer: m_pProvider = new CXTPShellExplorerProvider(*this); break;
		case xtpWebBrowserWebView: m_pProvider = new CXTPWebViewProvider(*this); break;
		default:
			TRACE(_T("Unexpected XTPWebBrowserProvider value %i\n"), m_nProvider);
			return FALSE;
	}

	HRESULT hr = m_pProvider->Initialize();
	if (S_OK != hr)
	{
		TRACE(_T("Browser provider initialization failed (code 0x%08X)\n"), hr);
		return FALSE;
	}

	if (!m_strPendlingUrl.IsEmpty())
	{
		m_pProvider->Navigate(m_strPendlingUrl);
		m_strPendlingUrl = _T("");
	}

	return TRUE;
}

void CXTPWebBrowserCtrl::ReleaseProvider()
{
	if (NULL != m_pProvider)
	{
		m_pProvider->ReleaseBrowser();
		m_pProvider->GetControllingUnknown()->Release();
		m_pProvider = NULL;
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPWebBrowserCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPWebBrowserCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CONTROLPARENT;
	m_bSubclassed = FALSE;

	return CWnd::PreCreateWindow(cs);
}

void CXTPWebBrowserCtrl::PreSubclassWindow()
{
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	CWnd::PreSubclassWindow();

	if (m_bSubclassed)
	{
		if (!InitializeProvider())
		{
			TRACE(_T("Unable to initialize CXTPWebBrowserCtrl"));
		}
	}
}

BOOL CXTPWebBrowserCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_WM_INITIALIZE == message && NULL == m_pProvider && !InitializeProvider())
	{
		TRACE(_T("Unable to initialize CXTPWebBrowserCtrl"));
		return TRUE;
	}

	if (NULL != m_pProvider && m_pProvider->OnHostWndMsg(message, wParam, lParam, pResult))
		return TRUE;

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

int CXTPWebBrowserCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	PostMessage(m_WM_INITIALIZE);

	return 0;
}

void CXTPWebBrowserCtrl::OnDestroy()
{
	ReleaseProvider();
	CWnd::OnDestroy();
}

void CXTPWebBrowserCtrl::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
}

void CXTPWebBrowserCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
