// XTPShellExplorerProvider.h : Declaration of the CXTPShellExplorerProvider class.
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

#if !defined(__XTPSHELLEXPLORERPROVIDER_H__)

//{{AFX_CODEJOCK_PRIVATE
#	define __XTPSHELLEXPLORERPROVIDER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"
#	pragma warning(push)
// C4616: #pragma warning : warning number '5204' out of range, must be between '4001' and '4999'
// C5204: '...': class has virtual functions, but its trivial destructor is not virtual; instances
// of objects derived from this class may not be destructed correctly
#	pragma warning(disable : 4616 5204)

//{{AFX_CODEJOCK_PRIVATE
struct IXTPWebBrowserHost;
class CXTPShellBrowserSite;
//}}AFX_CODEJOCK_PRIVATE

//=======================================================================
// Summary:
//      Implements ShellExplorer WebBrowser provider and gives access to ShellExplorer specific
//      functionality.
//=======================================================================
class _XTP_EXT_CLASS CXTPShellExplorerProvider : public CXTPWebBrowserProvider
{
	DECLARE_DYNAMIC(CXTPShellExplorerProvider)

	friend class CXTPShellBrowserSite;

public:
	//=======================================================================
	// Summary:
	//      Defines ShellExplorer scrollbar styles
	//=======================================================================
	enum ScrollBarStyle
	{
		xtpScrollBarStandard, // Normal scrollbars
		xtpScrollBarFlat,	 // Flat scrollbars
		xtpScrollBarNone	  // Scrollbars are turned off
	};

	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs provider object.
	// Parameters:
	//      host - The host control interface.
	//-----------------------------------------------------------------------
	CXTPShellExplorerProvider(IXTPWebBrowserHost& host);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles provider destruction.
	//-----------------------------------------------------------------------
	~CXTPShellExplorerProvider();

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets a flag value that enables registration of the WebBrowser control as drop
	//      target.
	// Parameters:
	//      bRegisterAsDropTarget - The new flag value.
	// Returns:
	//      The current flag value.
	//-----------------------------------------------------------------------
	BOOL GetRegisterAsDropTarget() const;
	void SetRegisterAsDropTarget(
		BOOL bRegisterAsDropTarget = TRUE); // <combine
											// CXTPShellExplorerProvider::GetRegisterAsDropTarget>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets a flag value that enables WebBrowser control context menu.
	// Parameters:
	//      bContextMenuEnabled - The new flag value.
	// Returns:
	//      The current flag value.
	//-----------------------------------------------------------------------
	BOOL GetContextMenuEnabled() const;
	void SetContextMenuEnabled(
		BOOL bContextMenuEnabled = TRUE); // <combine
										  // CXTPShellExplorerProvider::GetContextMenuEnabled>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets WebBrowser control scrollbar style.
	// Parameters:
	//      nScrollBarStyle - The new scrollbar style.
	// Returns:
	//      The current scrollbar style.
	//-----------------------------------------------------------------------
	ScrollBarStyle GetScrollBarStyle() const;
	void SetScrollBarStyle(
		ScrollBarStyle nScrollBarStyle); // <combine CXTPShellExplorerProvider::GetScrollBarStyle>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets a flag that suppresses script errors on a loaded web page.
	// Parameters:
	//      bScriptErrorsSuppressed - The new flag value.
	// Returns:
	//      The current flag value.
	//-----------------------------------------------------------------------
	BOOL GetScriptErrorsSuppressed() const;
	void SetScriptErrorsSuppressed(
		BOOL bScriptErrorsSuppressed =
			TRUE); // <combine CXTPShellExplorerProvider::GetScriptErrorsSuppressed>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/Sets a flag that enables selection of the text in the form.
	// Parameters:
	//      bStaticText - The new flag value.
	// Returns:
	//      The current flag value.
	//-----------------------------------------------------------------------
	BOOL GetStaticText() const;
	void SetStaticText(BOOL bStaticText = TRUE); // <combine
												 // CXTPShellExplorerProvider::GetStaticText>

	//-----------------------------------------------------------------------
	// Summary:
	//     Navigates to the specified URL and provides additional parameters for the browser to use
	//     and customize navigation.
	// Parameters:
	//     pvtURL   - A pointer to a VARIANT that evaluates to the URL of the resource to display,
	//                the full path to the file location, or a PIDL that represents a folder in the
	//                Shell namespace.
	//     pvtFlags - A pointer to a VARIANT of type VT_I4 or VT_I2 that specifies a combination of
	//                the values defined by the BrowserNavConstants enumeration from the Windows
	//                SDK.
	//     pvtTargetFrameName - A pointer to a case-sensitive string that contains the name of the
	//                          frame in which to display the resource ("_blank", "_parent",
	//                          "_self", "_top" or a named HTML frame).
	//     pvtPostData - A pointer to a VARIANT of type VT_ARRAY|VT_UI1 that is sent to the server
	//                   as part of a HTTP POST transaction. A POST transaction typically is used to
	//                   send data collected by an HTML form. If this parameter does not specify any
	//                   POST data, this method issues an HTTP GET transaction. This parameter is
	//                   ignored if URL is not an HTTP (or HTTPS) URL.
	//     pvtHeaders - A pointer to a VARIANT of type VT_BSTR that contains additional HTTP headers
	//                  to send to the server. These headers are added to the default Windows
	//                  Internet Explorer headers. For example, headers can specify the action
	//                  required of the server, the type of data being passed to the server, or a
	//                  status code. This parameter is ignored if the URL is not an HTTP (or HTTPS)
	//                  URL.
	// Returns:
	//      S_OK if navigation is successul or an error code otherwise.
	//-----------------------------------------------------------------------
	HRESULT Navigate2(VARIANT* pvtURL, VARIANT* pvtFlags = NULL, VARIANT* pvtTargetFrameName = NULL,
					  VARIANT* pvtPostData = NULL, VARIANT* pvtHeaders = NULL);

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

private:
	virtual HRESULT Initialize();
	virtual HRESULT Navigate(LPCTSTR pUrl);
	virtual void ReleaseBrowser();
	virtual BOOL OnHostWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCreateBrowser(IUnknown* pObject);
	virtual CXTPWebBrowserControlSite* CreateControlSite(COleControlContainer* pCtrlContainer);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL OnSize(UINT nType, int cx, int cy);
	BOOL OnDestroy();
	BOOL OnSetFocus(CWnd* pOldWnd);
	BOOL OnKillFocus(CWnd* pNewWnd);
	BOOL OnLButtonDown(UINT nFlags, CPoint point);
	int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);

	afx_msg void OnStatusTextChange(LPCTSTR lpszText);
	afx_msg void OnProgressChange(long nProgress, long nProgressMax);
	afx_msg void OnCommandStateChange(long nCommand, BOOL bEnable);
	afx_msg void OnDownloadBegin();
	afx_msg void OnDownloadComplete();
	afx_msg void OnTitleChange(LPCTSTR lpszText);
	afx_msg void OnPropertyChange(LPCTSTR lpszProperty);
	afx_msg void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	afx_msg void OnQuit();
	afx_msg void OnVisible(BOOL bVisible);
	afx_msg void OnToolBar(BOOL bToolBar);
	afx_msg void OnMenuBar(BOOL bMenuBar);
	afx_msg void OnStatusBar(BOOL bStatusBar);
	afx_msg void OnFullScreen(BOOL bFullScreen);
	afx_msg void OnTheaterMode(BOOL bTheaterMode);
	afx_msg void OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	afx_msg void OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags,
								   VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers,
								   BOOL* Cancel);
	afx_msg void OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL);

	afx_msg void OnNavigateError(LPDISPATCH pDisp, VARIANT* URL, VARIANT* TargetFrameName,
								 VARIANT* StatusCode, BOOL* Cancel);

	afx_msg void OnWindowSetResizable(BOOL Resizable);
	afx_msg void OnWindowSetLeft(long Left);
	afx_msg void OnWindowSetTop(long Top);
	afx_msg void OnWindowSetWidth(long Width);
	afx_msg void OnWindowSetHeight(long Height);
	afx_msg void OnWindowClosing(BOOL IsChildWindow, BOOL FAR* Cancel);
	afx_msg void OnSetSecureLockIcon(long SecureLockIcon);

	DECLARE_INTERFACE_MAP();

private:
	BEGIN_INTERFACE_PART(WebBrowserEvents2, DWebBrowserEvents2)
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
		/* [out] */ UINT* pctinfo);

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ ITypeInfo** ppTInfo);

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
		/* [in] */ REFIID riid,
		/* [size_is][in] */ LPOLESTR* rgszNames,
		/* [range][in] */ UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ DISPID* rgDispId);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
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
		UINT* puArgErr);
	END_INTERFACE_PART(WebBrowserEvents2)

private:
	CXTPShellExplorerProvider(const CXTPShellExplorerProvider&);
	CXTPShellExplorerProvider& operator=(const CXTPShellExplorerProvider&);

	CWnd* m_pBrowser;
	IWebBrowser2* m_pBrowserApp;
	CXTPShellBrowserSite* m_pSite;
	COccManager* m_pOccManager;
	IConnectionPoint* m_pCpEvents;
	DWORD m_dwCpEventsCookie;

	BOOL m_bRegisterAsDropTarget;
	BOOL m_bContextMenuEnabled;
	ScrollBarStyle m_nScrollBarStyle;
	BOOL m_bScriptErrorsSuppressed;
	BOOL m_bStaticText;
};

AFX_INLINE BOOL CXTPShellExplorerProvider::GetRegisterAsDropTarget() const
{
	return m_bRegisterAsDropTarget;
}

AFX_INLINE void
	CXTPShellExplorerProvider::SetContextMenuEnabled(BOOL bContextMenuEnabled /*= TRUE*/)
{
	m_bContextMenuEnabled = bContextMenuEnabled;
}

AFX_INLINE BOOL CXTPShellExplorerProvider::GetContextMenuEnabled() const
{
	return m_bContextMenuEnabled;
}

AFX_INLINE void CXTPShellExplorerProvider::SetScrollBarStyle(ScrollBarStyle nScrollBarStyle)
{
	m_nScrollBarStyle = nScrollBarStyle;
}

AFX_INLINE CXTPShellExplorerProvider::ScrollBarStyle
	CXTPShellExplorerProvider::GetScrollBarStyle() const
{
	return m_nScrollBarStyle;
}

AFX_INLINE void
	CXTPShellExplorerProvider::SetScriptErrorsSuppressed(BOOL bScriptErrorsSuppressed /*= TRUE*/)
{
	m_bScriptErrorsSuppressed = bScriptErrorsSuppressed;
}

AFX_INLINE BOOL CXTPShellExplorerProvider::GetScriptErrorsSuppressed() const
{
	return m_bScriptErrorsSuppressed;
}

AFX_INLINE void CXTPShellExplorerProvider::SetStaticText(BOOL bStaticText /*= TRUE*/)
{
	m_bStaticText = bStaticText;
}

AFX_INLINE BOOL CXTPShellExplorerProvider::GetStaticText() const
{
	return m_bStaticText;
}

#	pragma warning(pop)
#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"

#endif
