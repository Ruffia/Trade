// XTPWebBrowserProvider.h : Declaration of the CXTPWebBrowserProvider class.
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

#if !defined(__XTPWEBBROWSERPROVIDER_H__)

//{{AFX_CODEJOCK_PRIVATE
#	define __XTPWEBBROWSERPROVIDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#	include <atlbase.h>
#	include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

class CXTPWebBrowserCtrl;

//{{AFX_CODEJOCK_PRIVATE
struct IXTPWebBrowserHost;
class CXTPWebBrowserControlSite;

#	define XTP_WEBBROWSER_RESOLVE_USERAGENT(pv)                                                   \
		(NULL != (pv) && VT_BSTR == (pv)->vt && NULL != (pv)->bstrVal                              \
				 && 0 < ::SysStringLen((pv)->bstrVal)                                              \
			 ? (pv)->bstrVal                                                                       \
			 : NULL)

//}}AFX_CODEJOCK_PRIVATE

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     Implements the base class for a WebBrowser provider.
//===========================================================================
class _XTP_EXT_CLASS CXTPWebBrowserProvider : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPWebBrowserProvider)

	friend class CXTPWebBrowserCtrl;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Handles provider construction.
	// Parameters:
	//      host - The host object.
	//-----------------------------------------------------------------------
	CXTPWebBrowserProvider(IXTPWebBrowserHost& host);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Handles provider destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPWebBrowserProvider();

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the native WebBrowser provider object or NULL if no provider is activated or the
	//      current provider does not provide access to its core object via IUnknown.
	// Returns:
	//      Obtains the native WebBrowser provider object.
	//-----------------------------------------------------------------------
	IUnknown* GetBrowserObject() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets/gets/revomes the client interface.
	// Parameters:
	//      riid - Client interface IID.
	//      pUnk - Client interface pointer.
	// Returns:
	//      Client interface pointer or NULL if no requested interface is added.
	// Remarks:
	//      If a provider supports client site, the client interface provided
	//      may be requested by the provider at any moment thus making it possible
	//      to customize and extend WebBrowser functionality by adding provider
	//      specific client interfaces implemented by the user code.
	//-----------------------------------------------------------------------
	IUnknown* GetClientInterface(REFIID riid) const;
	void SetClientInterface(
		REFIID riid, IUnknown* pUnk); // <combine CXTPWebBrowserProvider::GetClientInterface@REFIID>
	void RemoveClientInterface(REFIID riid); // <combine
											 // CXTPWebBrowserProvider::GetClientInterface@REFIID>

	//-----------------------------------------------------------------------
	// Summary:
	//      Executes a script by the active WebBrowser provider.
	// Parameters:
	//      pText     - Script code text.
	//      pvtResult - The optional pointer to the variant object that is supposed to receive the
	//      result of the executed script.
	// Returns:
	//      S_OK if script execution or its attempt are successful or an error code otherwise.
	// Remarks:
	//      The language of the script code is provider specific. Currently each provider supports
	//      JavaScript by default.
	//-----------------------------------------------------------------------
	virtual HRESULT ExecuteScript(LPCWSTR pText, CComVariant* pvtResult = NULL) = 0;

	//{{AFX_CODEJOCK_PRIVATE
	IXTPWebBrowserHost& GetHost() const;

	virtual HRESULT Initialize()															= 0;
	virtual HRESULT Navigate(LPCTSTR pUrl)													= 0;
	virtual void ReleaseBrowser()															= 0;
	virtual BOOL OnHostWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) = 0;
	//}}AFX_CODEJOCK_PRIVATE

protected:
	//{{AFX_CODEJOCK_PRIVATE
	const CString& GetUrl() const;
	void SetUrl(CString strUrl);
	virtual void OnCreateBrowser(IUnknown* pObject);
	virtual void OnDestroyBrowser();
	virtual CXTPWebBrowserControlSite* CreateControlSite(COleControlContainer* pCtrlContainer);

	virtual void UpdateExternal();
	//}}AFX_CODEJOCK_PRIVATE

private:
	void ReleaseSite();

protected:
	IXTPWebBrowserHost& m_Host; // The host control interface

private:
	CXTPWebBrowserProvider(const CXTPWebBrowserProvider&);
	CXTPWebBrowserProvider& operator=(const CXTPWebBrowserProvider&);

	COleControlContainer* m_pCtrlContainer;
	IUnknown* m_pBrowserObject;
	CXTPWebBrowserControlSite* m_pSite;
	CString m_strUrl;

	struct ClientInterface
	{
		IID iid;
		LPUNKNOWN pUnk;
	};

	typedef CArray<ClientInterface, const ClientInterface&> ClientInterfaceArray;

	ClientInterfaceArray* m_parrClientInterfaces;
};

AFX_INLINE IXTPWebBrowserHost& CXTPWebBrowserProvider::GetHost() const
{
	return m_Host;
}

AFX_INLINE IUnknown* CXTPWebBrowserProvider::GetBrowserObject() const
{
	return m_pBrowserObject;
}

AFX_INLINE const CString& CXTPWebBrowserProvider::GetUrl() const
{
	return m_strUrl;
}

AFX_INLINE void CXTPWebBrowserProvider::SetUrl(CString strUrl)
{
	m_strUrl = strUrl;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"

#endif
