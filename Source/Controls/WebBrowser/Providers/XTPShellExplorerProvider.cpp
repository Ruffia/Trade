// XTPShellExplorerProvider.cpp : Implementation of the CXTPShellExplorerProvider class.
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
#include <Mshtml.h>
#include <MSHTMDID.H>

#include <mshtmhst.h>
#include <ExDispid.h>

#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPMacros.h"
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
#include "Controls/WebBrowser/Providers/XTPWebBrowserControlSite.h"
#include "Controls/WebBrowser/Providers/XTPShellExplorerProvider.h"

#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (_MSC_VER < 1300)

MIDL_INTERFACE("3050f6d0-98b5-11cf-bb82-00aa00bdce0b")
IDocHostUIHandler2 : public IDocHostUIHandler
{
public:
	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath(LPOLESTR * pchKey, DWORD dw) = 0;
};

EXTERN_C const IID IID_IDocHostUIHandler2 = __uuidof(IDocHostUIHandler2);

#endif

#pragma warning(push)
// C4616: #pragma warning : warning number '5204' out of range, must be between '4001' and '4999'
// C5204: '...': class has virtual functions, but its trivial destructor is not virtual; instances
// of objects derived from this class may not be destructed correctly
#pragma warning(disable : 4616 5204)

///////////////////////////////////////////////////////////////////////////////
// CXTPShellBrowserSite

class CXTPShellExplorerControlSite : public CXTPWebBrowserControlSite
{
	CXTPShellExplorerProvider* m_pProvider;

public:
	CXTPShellExplorerControlSite(COleControlContainer* pCtrlCont,
								 CXTPShellExplorerProvider* pProvider)
		: CXTPWebBrowserControlSite(pCtrlCont, pProvider)
		, m_pProvider(pProvider)
	{
	}

private:
	BSTR OnAmbientUserAgent();

	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(AmbientProps, IDispatch)
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
	STDMETHOD(GetIDsOfNames)
	(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	STDMETHOD(Invoke)
	(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
	 VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);
	END_INTERFACE_PART(AmbientProps)
};

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_INTERFACE_MAP(CXTPShellExplorerControlSite, CXTPWebBrowserControlSite)
	INTERFACE_PART(CXTPShellExplorerControlSite, IID_IDispatch, AmbientProps)
END_INTERFACE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BSTR CXTPShellExplorerControlSite::OnAmbientUserAgent()
{
	const COleVariant* pvtUserAgent = m_pProvider->GetHost().GetProperty(_T("UserAgent"));
	LPCWSTR pUserAgent				= XTP_WEBBROWSER_RESOLVE_USERAGENT(pvtUserAgent);
	if (NULL == pUserAgent)
		return NULL;

	if (NULL == pvtUserAgent || VT_BSTR != pvtUserAgent->vt || NULL == pvtUserAgent->bstrVal)
	{
		TRACE(_T("Unacceptable UserAgent value\n"));
		return NULL;
	}

	return ::SysAllocString(pvtUserAgent->bstrVal);
}

XTP_IMPLEMENT_IUNKNOWN(CXTPShellExplorerControlSite, AmbientProps)

STDMETHODIMP CXTPShellExplorerControlSite::XAmbientProps::GetTypeInfoCount(UINT* pctinfo)
{
	if (NULL == pctinfo)
		return E_POINTER;

	*pctinfo = 0;
	return S_OK;
}

STDMETHODIMP CXTPShellExplorerControlSite::XAmbientProps::GetTypeInfo(UINT iTInfo, LCID lcid,
																	  ITypeInfo** ppTInfo)
{
	XTP_UNUSED_PARAMETER(iTInfo);
	XTP_UNUSED_PARAMETER(lcid);
	XTP_UNUSED_PARAMETER(ppTInfo);

	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellExplorerControlSite::XAmbientProps::GetIDsOfNames(REFIID riid,
																		LPOLESTR* rgszNames,
																		UINT cNames, LCID lcid,
																		DISPID* rgDispId)
{
	XTP_UNUSED_PARAMETER(riid);
	XTP_UNUSED_PARAMETER(rgszNames);
	XTP_UNUSED_PARAMETER(cNames);
	XTP_UNUSED_PARAMETER(lcid);
	XTP_UNUSED_PARAMETER(rgDispId);

	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellExplorerControlSite::XAmbientProps::Invoke(
	DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
	VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
	METHOD_PROLOGUE(CXTPShellExplorerControlSite, AmbientProps);

	XTP_UNUSED_PARAMETER(riid);
	XTP_UNUSED_PARAMETER(lcid);
	XTP_UNUSED_PARAMETER(wFlags);
	XTP_UNUSED_PARAMETER(pExcepInfo);
	XTP_UNUSED_PARAMETER(puArgErr);
	XTP_UNUSED_PARAMETER(pDispParams);

	if (DISPID_AMBIENT_USERAGENT == dispIdMember)
	{
		if (NULL == pVarResult)
			return E_POINTER;

		BSTR bstrUA = pThis->OnAmbientUserAgent();
		if (NULL == bstrUA)
			return E_NOTIMPL;

		::VariantClear(pVarResult);
		pVarResult->vt		= VT_BSTR;
		pVarResult->bstrVal = bstrUA;

		return S_OK;
	}

	return E_NOTIMPL;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPShellBrowserSite

class CXTPShellBrowserSite : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPShellBrowserSite);

public:
	explicit CXTPShellBrowserSite(CXTPShellExplorerProvider* pProvider);

	IDocHostUIHandler& GetDocHostUIHandler();
	IOleCommandTarget& GetOleCommandTarget();

protected:
	DECLARE_INTERFACE_MAP()

	BEGIN_INTERFACE_PART(DocHostUIHandler2, IDocHostUIHandler2)
	STDMETHOD(ShowContextMenu)
	(/* [in] */ DWORD dwID,
	 /* [in] */ POINT* ppt,
	 /* [in] */ IUnknown* pcmdtReserved,
	 /* [in] */ IDispatch* pdispReserved);
	STDMETHOD(GetHostInfo)
	(
		/* [out][in] */ DOCHOSTUIINFO* pInfo);
	STDMETHOD(ShowUI)
	(
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject* pActiveObject,
		/* [in] */ IOleCommandTarget* pCommandTarget,
		/* [in] */ IOleInPlaceFrame* pFrame,
		/* [in] */ IOleInPlaceUIWindow* pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(ResizeBorder)
	(
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow* pUIWindow,
		/* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)
	(
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID* pguidCmdGroup,
		/* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)
	(
		/* [out] */ LPOLESTR* pchKey,
		/* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)
	(
		/* [in] */ IDropTarget* pDropTarget,
		/* [out] */ IDropTarget** ppDropTarget);
	STDMETHOD(GetExternal)
	(
		/* [out] */ IDispatch** ppDispatch);
	STDMETHOD(TranslateUrl)
	(
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR* pchURLIn,
		/* [out] */ OLECHAR** ppchURLOut);
	STDMETHOD(FilterDataObject)
	(
		/* [in] */ IDataObject* pDO,
		/* [out] */ IDataObject** ppDORet);
	STDMETHOD(GetOverrideKeyPath)
	(
		/* [annotation][out] */
		LPOLESTR* pchKey,
		/* [in] */ DWORD dw);
	END_INTERFACE_PART(DocHostUIHandler2)

	BEGIN_INTERFACE_PART(OleCommandTarget, IOleCommandTarget)
	STDMETHOD(Exec)
	(
		/* [unique][in] */ const GUID* pguidCmdGroup,
		/* [in] */ DWORD nCmdID,
		/* [in] */ DWORD nCmdexecopt,
		/* [unique][in] */ VARIANT* pvaIn,
		/* [unique][out][in] */ VARIANT* pvaOut);

	/*[input_sync]*/ STDMETHOD(QueryStatus)(
		/* [unique][in] */ const GUID* pguidCmdGroup,
		/* [in] */ ULONG cCmds,
		/* [out][in][size_is] */ OLECMD prgCmds[],
		/* [unique][out][in] */ OLECMDTEXT* pCmdText);
	END_INTERFACE_PART(OleCommandTarget)

	CXTPShellExplorerProvider* m_pProvider;
};

#pragma warning(pop)

IMPLEMENT_DYNAMIC(CXTPShellBrowserSite, CXTPCmdTarget);

CXTPShellBrowserSite::CXTPShellBrowserSite(CXTPShellExplorerProvider* pProvider)
	: m_pProvider(pProvider)
{
}

AFX_INLINE IDocHostUIHandler& CXTPShellBrowserSite::GetDocHostUIHandler()
{
	return m_xDocHostUIHandler2;
}

AFX_INLINE IOleCommandTarget& CXTPShellBrowserSite::GetOleCommandTarget()
{
	return m_xOleCommandTarget;
}

BEGIN_INTERFACE_MAP(CXTPShellBrowserSite, CXTPCmdTarget)
	INTERFACE_PART(CXTPShellBrowserSite, IID_IDocHostUIHandler, DocHostUIHandler2)
	INTERFACE_PART(CXTPShellBrowserSite, IID_IDocHostUIHandler2, DocHostUIHandler2)
	INTERFACE_PART(CXTPShellBrowserSite, IID_IOleCommandTarget, OleCommandTarget)
END_INTERFACE_MAP()

XTP_IMPLEMENT_IUNKNOWN(CXTPShellBrowserSite, OleCommandTarget)

#if (_MSC_VER < 1300)

static CLSID const CGID_DocHostCommandHandler =
	{ 0xf38bc242, 0xb950, 0x11d1, { 0x89, 0x18, 0x00, 0xc0, 0x4f, 0xc2, 0xc8, 0x36 } };

#endif

STDMETHODIMP CXTPShellBrowserSite::XOleCommandTarget::Exec(
	/* [unique][in] */ const GUID* pguidCmdGroup,
	/* [in] */ DWORD nCmdID,
	/* [in] */ DWORD /*nCmdexecopt*/,
	/* [unique][in] */ VARIANT* /*pvaIn*/,
	/* [unique][out][in] */ VARIANT* pvaOut)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, OleCommandTarget)

	if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
	{
		if (nCmdID == 40 /*OLECMDID_SHOWSCRIPTERROR*/
			&& pThis->m_pProvider->GetScriptErrorsSuppressed())
		{
			(*pvaOut).vt	  = VT_BOOL;
			(*pvaOut).boolVal = XTP_VARIANT_TRUE;

			return S_OK;
		}
	}

	return OLECMDERR_E_NOTSUPPORTED;
}

STDMETHODIMP CXTPShellBrowserSite::XOleCommandTarget::QueryStatus(
	/* [unique][in] */ const GUID* /*pguidCmdGroup*/,
	/* [in] */ ULONG /*cCmds*/,
	/* [out][in][size_is] */ OLECMD /*prgCmds*/[],
	/* [unique][out][in] */ OLECMDTEXT* /*pCmdText*/)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, OleCommandTarget)
	return OLECMDERR_E_NOTSUPPORTED;
}

XTP_IMPLEMENT_IUNKNOWN(CXTPShellBrowserSite, DocHostUIHandler2)

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, DocHostUIHandler2)

	pInfo->dwFlags = 0x4000
					 | 0x40000 /*DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_THEME*/;

	if (pThis->m_pProvider->GetStaticText())
		pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;
	if (pThis->m_pProvider->GetScrollBarStyle() == CXTPShellExplorerProvider::xtpScrollBarNone)
		pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	if (pThis->m_pProvider->GetScrollBarStyle() == CXTPShellExplorerProvider::xtpScrollBarFlat)
		pInfo->dwFlags |= DOCHOSTUIFLAG_FLAT_SCROLLBAR;

	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

	return S_OK;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::ShowUI(
	DWORD /*dwID*/, IOleInPlaceActiveObject* /*pActiveObject*/,
	IOleCommandTarget* /*pCommandTarget*/, IOleInPlaceFrame* /*pFrame*/,
	IOleInPlaceUIWindow* /*pDoc*/)
{
	return S_OK;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::HideUI(void)
{
	return S_OK;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::UpdateUI(void)
{
	return S_OK;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::EnableModeless(BOOL /*fEnable*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::OnDocWindowActivate(BOOL /*fActivate*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::OnFrameWindowActivate(BOOL /*fActivate*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::ResizeBorder(
	LPCRECT /*prcBorder*/, IOleInPlaceUIWindow* /*pUIWindow*/, BOOL /*fRameWindow*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::ShowContextMenu(
	DWORD /*dwID*/, POINT* /*pptPosition*/, IUnknown* /*pCommandTarget*/,
	IDispatch* /*pDispatchObjectHit*/)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, DocHostUIHandler2)

	// Don't show context menu
	if (!pThis->m_pProvider->GetContextMenuEnabled())
		return S_OK;

	// Otherwise, show default
	return S_FALSE;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::TranslateAccelerator(
	LPMSG /*lpMsg*/,
	/* [in] */ const GUID* /*pguidCmdGroup*/,
	/* [in] */ DWORD /*nCmdID*/)
{
	return S_FALSE;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::GetOptionKeyPath(LPOLESTR* pchKey, DWORD)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, DocHostUIHandler2)

	if (NULL == pchKey)
		return E_POINTER;

	const COleVariant* ptvKeyPath = pThis->m_pProvider->GetHost().GetProperty(_T("OptionKeyPath"));
	if (NULL == ptvKeyPath || VT_BSTR != ptvKeyPath->vt || NULL == ptvKeyPath->bstrVal)
		return E_NOTIMPL;

	*pchKey = reinterpret_cast<BSTR>(
		::CoTaskMemAlloc((wcslen(ptvKeyPath->bstrVal) + 1) * sizeof(WCHAR)));
	if (NULL == pchKey)
		return E_OUTOFMEMORY;

	lstrcpyW(*pchKey, ptvKeyPath->bstrVal);

	return S_OK;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::GetDropTarget(
	/* [in] */ IDropTarget* /*pDropTarget*/,
	/* [out] */ IDropTarget** /*ppDropTarget*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::GetExternal(
	/* [out] */ IDispatch** ppDispatch)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, DocHostUIHandler2)

	if (NULL == ppDispatch)
		return E_INVALIDARG;

	IDispatch* pDispatch = pThis->m_pProvider->GetHost().GetExternal();
	*ppDispatch			 = pDispatch;

	if (NULL != pDispatch)
	{
		pDispatch->AddRef();
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::TranslateUrl(
	/* [in] */ DWORD /*dwTranslate*/,
	/* [in] */ OLECHAR* /*pchURLIn*/,
	/* [out] */ OLECHAR** /*ppchURLOut*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::FilterDataObject(
	/* [in] */ IDataObject* /*pDO*/,
	/* [out] */ IDataObject** /*ppDORet*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPShellBrowserSite::XDocHostUIHandler2::GetOverrideKeyPath(LPOLESTR* pchKey, DWORD)
{
	METHOD_PROLOGUE(CXTPShellBrowserSite, DocHostUIHandler2)

	if (NULL == pchKey)
		return E_POINTER;

	const COleVariant* ptvKeyPath = pThis->m_pProvider->GetHost().GetProperty(
		_T("OverrideKeyPath"));
	if (NULL == ptvKeyPath || VT_BSTR != ptvKeyPath->vt || NULL == ptvKeyPath->bstrVal)
		return E_NOTIMPL;

	*pchKey = reinterpret_cast<BSTR>(
		::CoTaskMemAlloc((wcslen(ptvKeyPath->bstrVal) + 1) * sizeof(WCHAR)));
	if (NULL == pchKey)
		return E_OUTOFMEMORY;

	lstrcpyW(*pchKey, ptvKeyPath->bstrVal);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPShellExplorerProvider

IMPLEMENT_DYNAMIC(CXTPShellExplorerProvider, CXTPWebBrowserProvider)

BEGIN_INTERFACE_MAP(CXTPShellExplorerProvider, CXTPWebBrowserProvider)
	INTERFACE_PART(CXTPShellExplorerProvider, DIID_DWebBrowserEvents2, WebBrowserEvents2)
END_INTERFACE_MAP()

CXTPShellExplorerProvider::CXTPShellExplorerProvider(IXTPWebBrowserHost& host)
	: CXTPWebBrowserProvider(host)
	, m_pBrowser(new CWnd())
	, m_pBrowserApp(NULL)
	, m_pSite(NULL)
	, m_pOccManager(new COccManager())
	, m_pCpEvents(NULL)
	, m_dwCpEventsCookie(0)
	, m_bRegisterAsDropTarget(FALSE)
	, m_bContextMenuEnabled(FALSE)
	, m_nScrollBarStyle(xtpScrollBarStandard)
	, m_bScriptErrorsSuppressed(FALSE)
	, m_bStaticText(FALSE)
{
	m_pSite = new CXTPShellBrowserSite(this);
	m_Host.GetHostUnknown()->AddRef();

	SetClientInterface(IID_IDocHostUIHandler, &m_pSite->GetDocHostUIHandler());
	SetClientInterface(IID_IDocHostUIHandler2, &m_pSite->GetDocHostUIHandler());
	SetClientInterface(IID_IOleCommandTarget, &m_pSite->GetOleCommandTarget());
}

CXTPShellExplorerProvider::~CXTPShellExplorerProvider()
{
	SAFE_DELETE(m_pOccManager);
	m_pSite->GetControllingUnknown()->Release();

	ReleaseBrowser();

	SAFE_DELETE(m_pBrowser);

	m_Host.GetHostUnknown()->Release();
}

void CXTPShellExplorerProvider::SetRegisterAsDropTarget(BOOL bRegisterAsDropTarget /*= TRUE*/)
{
	m_bRegisterAsDropTarget = bRegisterAsDropTarget;

	if (NULL != m_pBrowserApp)
		m_pBrowserApp->put_RegisterAsDropTarget(m_bRegisterAsDropTarget ? XTP_VARIANT_TRUE
																		: XTP_VARIANT_FALSE);
}

HRESULT CXTPShellExplorerProvider::Navigate2(VARIANT* pvtURL, VARIANT* pvtFlags /*= NULL*/,
											 VARIANT* pvtTargetFrameName /*= NULL*/,
											 VARIANT* pvtPostData /*= NULL*/,
											 VARIANT* pvtHeaders /*= NULL*/)
{
	if (NULL == pvtURL)
		return E_POINTER;

	COleVariant vtUrl = *pvtURL;
	vtUrl.ChangeType(VT_BSTR);

	if (NULL == m_pBrowserApp)
	{
		// The browser is not yet initialized so the URL is saved for automatic navigation once it
		// is initialized.
		SetUrl(XTP_CW2CT(vtUrl.bstrVal));
		return S_FALSE;
	}

	COleVariant vtFlags;
	COleVariant vtTargetFrameName;
	COleVariant vtPostData;
	COleVariant vtHeaders;

	const COleVariant* pvtUserAgent = GetHost().GetProperty(_T("UserAgent"));
	BSTR pUserAgent					= XTP_WEBBROWSER_RESOLVE_USERAGENT(pvtUserAgent);
	if (NULL != pUserAgent)
	{
		UINT nUALen = ::SysStringLen(pUserAgent);
		if (NULL == pvtHeaders || VT_EMPTY == pvtHeaders->vt)
		{
			WCHAR* pBuff = new WCHAR[_countof("User-Agent: \r\n") + nUALen];
			lstrcpyW(pBuff, L"User-Agent: ");
			lstrcatW(pBuff, pUserAgent);
			lstrcatW(pBuff, L"\r\n");

			vtHeaders.vt	  = VT_BSTR;
			vtHeaders.bstrVal = ::SysAllocString(pBuff);
			delete[] pBuff;
		}
		else
		{
			ASSERT(VT_BSTR == pvtHeaders->vt);
			ASSERT(NULL != pvtHeaders->bstrVal);

			if (VT_BSTR == pvtHeaders->vt && NULL != pvtHeaders->bstrVal)
			{
				UINT nHdrLen = ::SysStringLen(pvtHeaders->bstrVal);

				WCHAR* pBuff = new WCHAR[_countof("User-Agent: \r\n") + nUALen + nHdrLen];
				lstrcpyW(pBuff, L"User-Agent: ");
				lstrcatW(pBuff, pUserAgent);
				lstrcatW(pBuff, L"\r\n");
				lstrcatW(pBuff, pvtHeaders->bstrVal);

				vtHeaders.vt	  = VT_BSTR;
				vtHeaders.bstrVal = ::SysAllocString(pBuff);
				delete[] pBuff;
			}
			else
			{
				pUserAgent = NULL;
			}
		}
	}

	HRESULT hr = m_pBrowserApp->Navigate2(
		pvtURL, (NULL != pvtFlags ? pvtFlags : &vtFlags),
		(NULL != pvtTargetFrameName ? pvtTargetFrameName : &vtTargetFrameName),
		(NULL != pvtPostData ? pvtPostData : &vtPostData),
		(NULL != pUserAgent || NULL == pvtHeaders ? &vtHeaders : pvtHeaders));

	if (S_OK == hr)
		SetUrl(XTP_CW2CT(vtUrl.bstrVal));

	return hr;
}

HRESULT CXTPShellExplorerProvider::ExecuteScript(LPCWSTR pText, CComVariant* pvtResult /*= NULL*/)
{
	ASSERT(NULL != pText);

	if (NULL == m_pBrowserApp)
		return E_PENDING;

	CComPtr<IDispatch> pDispDoc;
	HRESULT hr = m_pBrowserApp->get_Document(&pDispDoc);
	if (FAILED(hr) || !pDispDoc)
		return hr;

	if (!pDispDoc)
		return E_NOINTERFACE;

	CComPtr<IHTMLDocument2> pDoc;
	hr = pDispDoc->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<LPVOID*>(&pDoc));
	if (FAILED(hr))
		return hr;

	if (!pDoc)
		return E_NOINTERFACE;

	CComPtr<IHTMLWindow2> pWnd;
	hr = pDoc->get_parentWindow(&pWnd);
	if (FAILED(hr))
		return hr;

	if (!pWnd)
		return E_NOINTERFACE;

	static LPOLESTR arNames[] = { const_cast<LPOLESTR>(L"eval") };
	DISPID dispidEval		  = 0;
	hr = pWnd->GetIDsOfNames(IID_NULL, arNames, 1, LOCALE_SYSTEM_DEFAULT, &dispidEval);
	if (FAILED(hr))
		return hr;

	DISPPARAMS dp	  = { 0 };
	dp.cArgs		   = 1;
	CComVariant vtCode = pText;
	dp.rgvarg		   = &vtCode;
	hr = pWnd->Invoke(dispidEval, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, pvtResult,
					  NULL, NULL);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CXTPShellExplorerProvider::Initialize()
{
	if (::IsWindow(*m_pBrowser))
		return E_FAIL;

	RECT rcClient;
	m_Host.GetHostWindow().GetClientRect(&rcClient);

	AfxEnableControlContainer(m_pOccManager);

	// {8856F961-340A-11D0-A96B-00C04FD705A2}
	static const GUID SDK_CLSID_WebBrowser = { 0x8856F961,
											   0x340A,
											   0x11D0,
											   { 0xa9, 0x6b, 0x0, 0xc0, 0x4f, 0xd7, 0x05, 0xa2 } };

	// Create the control window
	// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
	if (!m_pBrowser->CreateControl(SDK_CLSID_WebBrowser, 0, WS_VISIBLE | WS_CHILD, rcClient,
								   &m_Host.GetHostWindow(), AFX_IDW_PANE_FIRST))
	{
		TRACE(_T("Unable to create Microsoft Web Browser control instance\n"));
		AfxEnableControlContainer(NULL);
		return E_FAIL;
	}

	AfxEnableControlContainer(NULL);

	// Obtain browser instance
	LPUNKNOWN lpUnk = m_pBrowser->GetControlUnknown();
	HRESULT hr		= lpUnk->QueryInterface(IID_IWebBrowser2, (void**)&m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		TRACE(_T("Unable to obtain IWebBrowser2 from the Microsoft Web Browser control ")
			  _T("instance\n"));
		ReleaseBrowser();
		return hr;
	}

	OnCreateBrowser(m_pBrowserApp);

	return S_OK;
}

HRESULT CXTPShellExplorerProvider::Navigate(LPCTSTR pUrl)
{
	ASSERT(NULL != pUrl);
	if (NULL == pUrl)
		return E_POINTER;

	return Navigate2(COleVariant(pUrl), COleVariant(), COleVariant(), COleVariant(), COleVariant());
}

void CXTPShellExplorerProvider::ReleaseBrowser()
{
	if (NULL != m_pBrowserApp)
	{
		OnDestroyBrowser();
		SAFE_RELEASE(m_pBrowserApp);
	}

	if (NULL != m_pCpEvents)
	{
		m_pCpEvents->Unadvise(m_dwCpEventsCookie);
		m_dwCpEventsCookie = 0;
		SAFE_RELEASE(m_pCpEvents);
	}

	if (::IsWindow(*m_pBrowser))
		m_pBrowser->DestroyWindow();
}

BOOL CXTPShellExplorerProvider::OnHostWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
											 LRESULT* pResult)
{
	ASSERT(NULL != pResult);

	switch (message)
	{
		case WM_SIZE: return OnSize(XTPToUInt(wParam), LOWORD(lParam), HIWORD(lParam));
		case WM_DESTROY: return OnDestroy();
		case WM_SETFOCUS: return OnSetFocus(CWnd::FromHandle(reinterpret_cast<HWND>(wParam)));
		case WM_KILLFOCUS: return OnKillFocus(CWnd::FromHandle(reinterpret_cast<HWND>(wParam)));
		case WM_LBUTTONDOWN:
			return OnLButtonDown(XTPToUInt(wParam), CPoint(LOWORD(lParam), HIWORD(lParam)));
		case WM_MOUSEACTIVATE:
			*pResult = OnMouseActivate(CWnd::FromHandle(reinterpret_cast<HWND>(wParam)),
									   LOWORD(lParam), HIWORD(lParam));
			return TRUE;
	}

	return FALSE;
}

void CXTPShellExplorerProvider::OnCreateBrowser(IUnknown* pObject)
{
	CXTPWebBrowserProvider::OnCreateBrowser(pObject);

	ASSERT(NULL != m_pBrowserApp);

	// Connect to events
	CComQIPtr<IConnectionPointContainer> pCPC = m_pBrowserApp;
	if (pCPC)
	{
		if (S_OK == pCPC->FindConnectionPoint(DIID_DWebBrowserEvents2, &m_pCpEvents) && m_pCpEvents)
		{
			m_pCpEvents->Advise(&m_xWebBrowserEvents2, &m_dwCpEventsCookie);
		}
		else
		{
			TRACE(_T("DWebBrowserEvents2 connection point not found\n"));
		}
	}

	// Initialize WebBrowser
	SetRegisterAsDropTarget(m_bRegisterAsDropTarget);

	const COleVariant* pvtUserAgent = m_Host.GetProperty(_T("UserAgent"));
	LPCWSTR pUserAgent				= XTP_WEBBROWSER_RESOLVE_USERAGENT(pvtUserAgent);
	if (NULL != pUserAgent)
	{
		CComQIPtr<IOleControl> pOleCtrl = m_pBrowserApp;
		if (pOleCtrl)
		{
			pOleCtrl->OnAmbientPropertyChange(DISPID_AMBIENT_USERAGENT);
		}
		else
		{
			TRACE(_T("IOleControl not extracted from WebBrowser\n"));
		}
	}

	READYSTATE state = READYSTATE_UNINITIALIZED;
	while (S_OK == m_pBrowserApp->get_ReadyState(&state) && READYSTATE_COMPLETE != state)
		XTPGetThread()->PumpMessage();

	if (!GetUrl().IsEmpty())
		Navigate(GetUrl());
}

CXTPWebBrowserControlSite*
	CXTPShellExplorerProvider::CreateControlSite(COleControlContainer* pCtrlContainer)
{
	return new CXTPShellExplorerControlSite(pCtrlContainer, this);
}

BOOL CXTPShellExplorerProvider::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message || WM_KEYUP == pMsg->message)
	{
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT
			|| pMsg->wParam == VK_RIGHT)
		{
			::SendMessage(::GetFocus(), pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPShellExplorerProvider::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (::IsWindow(*m_pBrowser))
	{
		// need to push non-client borders out of the client area
		CRect rect;
		m_Host.GetHostWindow().GetClientRect(rect);
		::AdjustWindowRectEx(rect, m_pBrowser->GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_pBrowser->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
								 SWP_NOACTIVATE | SWP_NOZORDER);
	}

	return FALSE;
}

BOOL CXTPShellExplorerProvider::OnDestroy()
{
	OnDestroyBrowser();
	ReleaseBrowser();

	return FALSE;
}

BOOL CXTPShellExplorerProvider::OnSetFocus(CWnd* /*pOldWnd*/)
{
	if (::IsWindow(m_pBrowser->m_hWnd))
		m_pBrowser->SetFocus();

	return TRUE;
}

BOOL CXTPShellExplorerProvider::OnKillFocus(CWnd* /*pNewWnd*/)
{
	// TODO: is needed?
	return FALSE;
}

BOOL CXTPShellExplorerProvider::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
	// TODO: is needed?
	return FALSE;
}

int CXTPShellExplorerProvider::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/,
											   UINT /*message*/)
{
	HWND hWndFocus = ::GetFocus();

	if (!hWndFocus
		|| (hWndFocus != m_pBrowser->m_hWnd && !::IsChild(m_pBrowser->m_hWnd, hWndFocus)))
	{
		m_pBrowser->SetFocus();

		// TODO:
		// if (m_pControlSite != NULL)
		//	m_pControlSite->OnFocus(TRUE);
	}

	return FALSE;
}

void CXTPShellExplorerProvider::OnStatusTextChange(LPCTSTR lpszText)
{
	m_Host.ShellExplorer_OnStatusTextChange(lpszText);
}

void CXTPShellExplorerProvider::OnProgressChange(long nProgress, long nProgressMax)
{
	m_Host.ShellExplorer_OnProgressChange(nProgress, nProgressMax);
}

void CXTPShellExplorerProvider::OnCommandStateChange(long nCommand, BOOL bEnable)
{
	m_Host.ShellExplorer_OnCommandStateChange(nCommand, bEnable);
}

void CXTPShellExplorerProvider::OnDownloadBegin()
{
	m_Host.ShellExplorer_OnDownloadBegin();
}

void CXTPShellExplorerProvider::OnDownloadComplete()
{
	m_Host.ShellExplorer_OnDownloadComplete();
}

void CXTPShellExplorerProvider::OnTitleChange(LPCTSTR lpszText)
{
	m_Host.ShellExplorer_OnTitleChange(lpszText);
}

void CXTPShellExplorerProvider::OnPropertyChange(LPCTSTR lpszProperty)
{
	m_Host.ShellExplorer_OnPropertyChange(lpszProperty);
}

void CXTPShellExplorerProvider::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	m_Host.ShellExplorer_OnNewWindow2(ppDisp, Cancel);
}

void CXTPShellExplorerProvider::OnQuit()
{
	m_Host.ShellExplorer_OnQuit();
}

void CXTPShellExplorerProvider::OnVisible(BOOL bVisible)
{
	m_Host.ShellExplorer_OnVisible(bVisible);
}

void CXTPShellExplorerProvider::OnToolBar(BOOL bToolBar)
{
	m_Host.ShellExplorer_OnToolBar(bToolBar);
}

void CXTPShellExplorerProvider::OnMenuBar(BOOL bMenuBar)
{
	m_Host.ShellExplorer_OnMenuBar(bMenuBar);
}

void CXTPShellExplorerProvider::OnStatusBar(BOOL bStatusBar)
{
	m_Host.ShellExplorer_OnStatusBar(bStatusBar);
}

void CXTPShellExplorerProvider::OnFullScreen(BOOL bFullScreen)
{
	m_Host.ShellExplorer_OnFullScreen(bFullScreen);
}

void CXTPShellExplorerProvider::OnTheaterMode(BOOL bTheaterMode)
{
	m_Host.ShellExplorer_OnTheaterMode(bTheaterMode);
}

void CXTPShellExplorerProvider::OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* URL)
{
	m_Host.ShellExplorer_OnNavigateComplete2(pDisp, URL);
}

void CXTPShellExplorerProvider::OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags,
												  VARIANT* TargetFrameName, VARIANT* PostData,
												  VARIANT* Headers, BOOL* Cancel)
{
	m_Host.ShellExplorer_OnBeforeNavigate2(pDisp, URL, Flags, TargetFrameName, PostData, Headers,
										   Cancel);
}

void CXTPShellExplorerProvider::OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	m_Host.ShellExplorer_OnDocumentComplete(pDisp, URL);
}

void CXTPShellExplorerProvider::OnNavigateError(LPDISPATCH pDisp, VARIANT* URL,
												VARIANT* TargetFrameName, VARIANT* StatusCode,
												BOOL* Cancel)
{
	m_Host.ShellExplorer_OnNavigateError(pDisp, URL, TargetFrameName, StatusCode, Cancel);
}

void CXTPShellExplorerProvider::OnWindowSetResizable(BOOL Resizable)
{
	m_Host.ShellExplorer_OnWindowSetResizable(Resizable);
}

void CXTPShellExplorerProvider::OnWindowSetLeft(long Left)
{
	m_Host.ShellExplorer_OnWindowSetLeft(Left);
}

void CXTPShellExplorerProvider::OnWindowSetTop(long Top)
{
	m_Host.ShellExplorer_OnWindowSetTop(Top);
}

void CXTPShellExplorerProvider::OnWindowSetWidth(long Width)
{
	m_Host.ShellExplorer_OnWindowSetWidth(Width);
}

void CXTPShellExplorerProvider::OnWindowSetHeight(long Height)
{
	m_Host.ShellExplorer_OnWindowSetHeight(Height);
}

void CXTPShellExplorerProvider::OnWindowClosing(BOOL IsChildWindow, BOOL FAR* Cancel)
{
	m_Host.ShellExplorer_OnWindowClosing(IsChildWindow, Cancel);
}

void CXTPShellExplorerProvider::OnSetSecureLockIcon(long SecureLockIcon)
{
	m_Host.ShellExplorer_OnSetSecureLockIcon(SecureLockIcon);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPShellExplorerProvider::XWebBrowserEvents2

XTP_IMPLEMENT_IUNKNOWN(CXTPShellExplorerProvider, WebBrowserEvents2)

HRESULT STDMETHODCALLTYPE CXTPShellExplorerProvider::XWebBrowserEvents2::GetTypeInfoCount(
	/* [out] */ UINT* pctinfo)
{
	if (NULL != pctinfo)
		*pctinfo = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CXTPShellExplorerProvider::XWebBrowserEvents2::GetTypeInfo(
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo** ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(ppTInfo);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CXTPShellExplorerProvider::XWebBrowserEvents2::GetIDsOfNames(
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR* rgszNames,
	/* [range][in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID* rgDispId)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(rgDispId);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CXTPShellExplorerProvider::XWebBrowserEvents2::Invoke(
	/* [annotation][in] */
	DISPID dispIdMember,
	/* [annotation][in] */
	REFIID riid,
	/* [annotation][in] */
	LCID lcid,
	/* [annotation][in] */
	WORD wFlags,
	/* [annotation][out][in] */
	DISPPARAMS* pDispParams,
	/* [annotation][out] */
	VARIANT* pVarResult,
	/* [annotation][out] */
	EXCEPINFO* pExcepInfo,
	/* [annotation][out] */
	UINT* puArgErr)
{
	METHOD_PROLOGUE(CXTPShellExplorerProvider, WebBrowserEvents2);

	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(pVarResult);
	UNREFERENCED_PARAMETER(pExcepInfo);
	UNREFERENCED_PARAMETER(puArgErr);

	if (NULL == pDispParams)
		return E_INVALIDARG;

	switch (dispIdMember)
	{
		case DISPID_STATUSTEXTCHANGE:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			if (VT_BSTR != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			pThis->OnStatusTextChange(XTP_CW2CT(pDispParams->rgvarg[0].bstrVal));
			return S_OK;
		}
		case DISPID_PROGRESSCHANGE:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[1];
			arg1.ChangeType(VT_I4);

			COleVariant arg2 = pDispParams->rgvarg[0];
			arg2.ChangeType(VT_I4);

			pThis->OnProgressChange(arg1.lVal, arg2.lVal);
			return S_OK;
		}
		case DISPID_COMMANDSTATECHANGE:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[1];
			arg1.ChangeType(VT_I4);

			COleVariant arg2 = pDispParams->rgvarg[0];
			arg2.ChangeType(VT_BOOL);

			pThis->OnCommandStateChange(arg1.lVal, XTP_VARIANT_TRUE == arg2.boolVal);
			return S_OK;
		}
		case DISPID_DOWNLOADBEGIN:
		{
			if (0 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			pThis->OnDownloadBegin();
			return S_OK;
		}
		case DISPID_DOWNLOADCOMPLETE:
		{
			if (0 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			pThis->OnDownloadComplete();
			return S_OK;
		}
		case DISPID_TITLECHANGE:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			if (VT_BSTR != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			pThis->OnTitleChange(XTP_CW2CT(pDispParams->rgvarg[0].bstrVal));
			return S_OK;
		}
		case DISPID_PROPERTYCHANGE:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			if (VT_BSTR != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			pThis->OnPropertyChange(XTP_CW2CT(pDispParams->rgvarg[0].bstrVal));
			return S_OK;
		}
		case DISPID_NEWWINDOW2:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			if ((VT_DISPATCH | VT_BYREF) != pDispParams->rgvarg[1].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[1].ppdispVal)
				return E_POINTER;

			if ((VT_BOOL | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pboolVal)
				return E_POINTER;

			BOOL bCancel = FALSE;
			pThis->OnNewWindow2(pDispParams->rgvarg[1].ppdispVal, &bCancel);

			*pDispParams->rgvarg[0].pboolVal = (bCancel ? XTP_VARIANT_TRUE : XTP_VARIANT_FALSE);

			return S_OK;
		}
		case DISPID_QUIT:
		{
			if (0 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			pThis->OnQuit();
			return S_OK;
		}
		case DISPID_ONVISIBLE:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnVisible(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_ONTOOLBAR:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnToolBar(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_ONMENUBAR:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnMenuBar(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_ONSTATUSBAR:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnStatusBar(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_ONFULLSCREEN:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnFullScreen(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_ONTHEATERMODE:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnTheaterMode(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case DISPID_NAVIGATECOMPLETE2:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[1];
			arg1.ChangeType(VT_DISPATCH);

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pvarVal)
				return E_POINTER;

			pThis->OnNavigateComplete2(arg1.pdispVal, pDispParams->rgvarg[0].pvarVal);
			return S_OK;
		}
		case DISPID_BEFORENAVIGATE2:
		{
			if (7 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[6];
			arg1.ChangeType(VT_DISPATCH);

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[5].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[5].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[4].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[4].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[3].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[3].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[2].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[2].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[1].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[1].pvarVal)
				return E_POINTER;

			if ((VT_BOOL | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pboolVal)
				return E_POINTER;

			BOOL bCancel = FALSE;
			pThis->OnBeforeNavigate2(arg1.pdispVal, pDispParams->rgvarg[5].pvarVal,
									 pDispParams->rgvarg[4].pvarVal, pDispParams->rgvarg[3].pvarVal,
									 pDispParams->rgvarg[2].pvarVal, pDispParams->rgvarg[1].pvarVal,
									 &bCancel);

			*pDispParams->rgvarg[0].pboolVal = (bCancel ? XTP_VARIANT_TRUE : XTP_VARIANT_FALSE);
			return S_OK;
		}
		case DISPID_DOCUMENTCOMPLETE:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[1];
			arg1.ChangeType(VT_DISPATCH);

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pvarVal)
				return E_POINTER;

			pThis->OnDocumentComplete(arg1.pdispVal, pDispParams->rgvarg[0].pvarVal);
			return S_OK;
		}
		case 271 /*DISPID_NAVIGATEERROR*/:
		{
			if (5 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[4];
			arg1.ChangeType(VT_DISPATCH);

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[3].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[3].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[2].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[2].pvarVal)
				return E_POINTER;

			if ((VT_VARIANT | VT_BYREF) != pDispParams->rgvarg[1].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[1].pvarVal)
				return E_POINTER;

			if ((VT_BOOL | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pboolVal)
				return E_POINTER;

			BOOL bCancel = FALSE;
			pThis->OnNavigateError(arg1.pdispVal, pDispParams->rgvarg[3].pvarVal,
								   pDispParams->rgvarg[2].pvarVal, pDispParams->rgvarg[1].pvarVal,
								   &bCancel);

			*pDispParams->rgvarg[0].pboolVal = (bCancel ? XTP_VARIANT_TRUE : XTP_VARIANT_FALSE);
			return S_OK;
		}
		case 262 /*DISPID_WINDOWSETRESIZABLE*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_BOOL);

			pThis->OnWindowSetResizable(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case 264 /*DISPID_WINDOWSETLEFT*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_I4);

			pThis->OnWindowSetLeft(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case 265 /*DISPID_WINDOWSETTOP*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_I4);

			pThis->OnWindowSetTop(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case 267 /*DISPID_WINDOWSETHEIGHT*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_I4);

			pThis->OnWindowSetHeight(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case 266 /*DISPID_WINDOWSETWIDTH*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_I4);

			pThis->OnWindowSetWidth(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
		case 263 /*DISPID_WINDOWCLOSING*/:
		{
			if (2 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[1];
			arg1.ChangeType(VT_BOOL);

			if ((VT_BOOL | VT_BYREF) != pDispParams->rgvarg[0].vt)
				return DISP_E_BADVARTYPE;

			if (NULL == pDispParams->rgvarg[0].pboolVal)
				return E_POINTER;

			BOOL bCancel = FALSE;
			pThis->OnWindowClosing(XTP_VARIANT_TRUE == arg1.boolVal, &bCancel);

			*pDispParams->rgvarg[0].pboolVal = (bCancel ? XTP_VARIANT_TRUE : XTP_VARIANT_FALSE);

			return S_OK;
		}
		case 269 /*DISPID_SETSECURELOCKICON*/:
		{
			if (1 != pDispParams->cArgs)
				return DISP_E_BADPARAMCOUNT;

			COleVariant arg1 = pDispParams->rgvarg[0];
			arg1.ChangeType(VT_I4);

			pThis->OnSetSecureLockIcon(XTP_VARIANT_TRUE == arg1.boolVal);
			return S_OK;
		}
	}

	return DISP_E_MEMBERNOTFOUND;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
