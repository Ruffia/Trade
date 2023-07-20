// XTPDialogBase.h : interface for the CXTPDialogBase.
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
#if !defined(__XTPDIALOGBASEEX_H__)
#	define __XTPDIALOGBASEEX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//{{AFX_CODEJOCK_PRIVATE

class CXTPCommandBars;
class CXTPToolBar;
class CXTPMenuBar;

template<class TBase>
class CXTPDialogBaseParentWrapper : public TBase
{
public:
	CXTPDialogBaseParentWrapper()
	{
	}

	CXTPDialogBaseParentWrapper(UINT nIDTemplate, CWnd* pParentWnd)
		: TBase(nIDTemplate, pParentWnd)
	{
	}

	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
		: TBase(lpszTemplateName, pParentWnd)
	{
	}

#	if (_MSC_VER > 1200)
	CXTPDialogBaseParentWrapper(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
		: TBase(nIDTemplate, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a non-DHTML dialog");

		UNREFERENCED_PARAMETER(nHtmlResID);
	}

	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd* pParentWnd)
		: TBase(lpszTemplateName, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a non-DHTML dialog");

		UNREFERENCED_PARAMETER(szHtmlResID);
	}
#	endif // (_MSC_VER > 1200)
};

#	if (_MSC_VER > 1200)
template<>
class CXTPDialogBaseParentWrapper<CDHtmlDialog> : public CDHtmlDialog
{
public:
	CXTPDialogBaseParentWrapper()
	{
	}

	CXTPDialogBaseParentWrapper(UINT nIDTemplate, CWnd* pParentWnd)
		: CDHtmlDialog(nIDTemplate, 0, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a DHTML dialog");
	}

	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
		: CDHtmlDialog(lpszTemplateName, 0, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a DHTML dialog");
	}

	CXTPDialogBaseParentWrapper(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
		: CDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd)
	{
	}

	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd* pParentWnd)
		: CDHtmlDialog(lpszTemplateName, szHtmlResID, pParentWnd)
	{
	}
};

template<>
class CXTPDialogBaseParentWrapper<CXTPResizeDHtmlDialog> : public CXTPResizeDHtmlDialog
{
public:
	CXTPDialogBaseParentWrapper()
	{
	}

	CXTPDialogBaseParentWrapper(UINT nIDTemplate, CWnd* pParentWnd)
		: CXTPResizeDHtmlDialog(nIDTemplate, 0, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a DHTML dialog");
	}

	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
		: CXTPResizeDHtmlDialog(lpszTemplateName, 0, pParentWnd)
	{
		ASSERT(!"This constructor is added for interface compatibility only and must never be "
				"called for a DHTML dialog");
	}

	CXTPDialogBaseParentWrapper(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd)
		: CXTPResizeDHtmlDialog(nIDTemplate, nHtmlResID, pParentWnd)
	{
	}

	CXTPDialogBaseParentWrapper(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd* pParentWnd)
		: CXTPResizeDHtmlDialog(lpszTemplateName, szHtmlResID, pParentWnd)
	{
	}
};

#	endif // (_MSC_VER > 1200)

class _XTP_EXT_CLASS CXTPDialogBaseImpl
{
protected:
	CXTPDialogBaseImpl()
	{
#	ifdef _XTP_INCLUDE_COMMANDBARS
		m_pCommandBars = 0;
#	endif
	}

	~CXTPDialogBaseImpl();

#	ifdef _XTP_INCLUDE_COMMANDBARS
	virtual BOOL InitCommandBars(CRuntimeClass* pCommandBarsClass = NULL);
	void DockRightOf(CXTPToolBar* pBarToDock, CXTPToolBar* pBarOnLeft);
	virtual void SaveCommandBars(LPCTSTR lpszProfileName);
	virtual void LoadCommandBars(LPCTSTR lpszProfileName, BOOL bSilent = FALSE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	virtual CWnd* GetCommandBarsSite() = 0;

	CXTPCommandBars* GetCommandBars() const
	{
		return m_pCommandBars;
	}

private:
	CXTPCommandBars* m_pCommandBars;
#	endif
};

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTPDialogBase is a TBase derived class. It represents the parent
//     class for a CXTPDialog class.
//===========================================================================
template<class TBase>
class CXTPDialogBase
	: public CXTPDialogBaseParentWrapper<TBase>
	, private CXTPDialogBaseImpl
{
	typedef CXTPDialogBaseParentWrapper<TBase> Parent;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialog object.
	// Parameters:
	//     nIDTemplate - Contains the ID number of a dialog-template resource.
	//     lpszTemplateName - Contains a NULL-terminated string that is
	//                        the name of a dialog-template resource.
	//     pParentWnd  - Pointer to the parent of the DialogBase control.
	//     nHtmlResID - HTML page resource ID for DHTML dialogs.
	//     szHtmlResID - HTML page resource ID for DHTML dialogs.
	//-----------------------------------------------------------------------
	CXTPDialogBase()
	{
	}

	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBase(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: CXTPDialogBaseParentWrapper<TBase>(nIDTemplate, pParentWnd)
	{
	}

	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBase(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL)
		: CXTPDialogBaseParentWrapper<TBase>(lpszTemplateName, pParentWnd)
	{
	}

#	if (_MSC_VER > 1200)
	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBase(UINT nIDTemplate, UINT nHtmlResID, CWnd* pParentWnd = NULL)
		: CXTPDialogBaseParentWrapper<TBase>(nIDTemplate, nHtmlResID, pParentWnd)
	{
	}

	//<combine CXTPDialogBase::CXTPDialogBase>
	CXTPDialogBase(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID, CWnd* pParentWnd = NULL)
		: CXTPDialogBaseParentWrapper<TBase>(lpszTemplateName, szHtmlResID, pParentWnd)
	{
	}
#	endif

#	ifdef _XTP_INCLUDE_COMMANDBARS
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates command bars.
	// Parameters:
	//     pCommandBarsClass - Custom runtime class of CommandBars. It can be
	//                         used if you want to override some methods
	//                         of the CXTPCommandBars class.
	// Returns:
	//     Nonzero if successful, otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL InitCommandBars(CRuntimeClass* pCommandBarsClass = NULL)
	{
		return CXTPDialogBaseImpl::InitCommandBars(pCommandBarsClass);
	}

	using CXTPDialogBaseParentWrapper<TBase>::RepositionBars;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will re-dock a toolbar specified by 'pBarToDock'
	//     to the right of a newly docked toolbar specified by 'pBarOnLeft'.
	// Parameters:
	//     pBarToDock - A CXTPToolBar pointer to the toolbar to be docked.
	//     pBarOnLeft - A CXTPToolBar pointer to the already docked toolbar.
	//-----------------------------------------------------------------------
	void DockRightOf(CXTPToolBar* pBarToDock, CXTPToolBar* pBarOnLeft)
	{
		CXTPDialogBaseImpl::DockRightOf(pBarToDock, pBarOnLeft);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a NULL-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//-----------------------------------------------------------------------
	virtual void SaveCommandBars(LPCTSTR lpszProfileName)
	{
		CXTPDialogBaseImpl::SaveCommandBars(lpszProfileName);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a NULL-terminated string that specifies
	//                       the name of a section in the initialization file
	//                       or a key in the Windows registry where state
	//                       information is stored.
	//     bSilent         - TRUE to disable user notifications when command bars
	//                       are restored to their original state.
	//-----------------------------------------------------------------------
	virtual void LoadCommandBars(LPCTSTR lpszProfileName, BOOL bSilent = FALSE)
	{
		CXTPDialogBaseImpl::LoadCommandBars(lpszProfileName, bSilent);
	}

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the CommandBars object.
	// Returns:
	//     A pointer to the CommandBars object.
	//----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars() const
	{
		return CXTPDialogBaseImpl::GetCommandBars();
	}

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual CWnd* GetCommandBarsSite()
	{
		return this;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (CXTPDialogBaseImpl::PreTranslateMessage(pMsg))
			return TRUE;

		if (TBase::PreTranslateMessage(pMsg))
			return TRUE;

		return FALSE;
	}

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (CXTPDialogBaseImpl::OnWndMsg(message, wParam, lParam, pResult))
			return TRUE;

		return TBase::OnWndMsg(message, wParam, lParam, pResult);
	}

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

//===========================================================================
// Summary:
//     CXTPDialogEx is a CXTPDialogBase derived class. Use this class in
//     your dialog base application.
//===========================================================================
class _XTP_EXT_CLASS CXTPDialogEx : public CXTPDialogBase<CDialog>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogEx object.
	// Parameters:
	//     nIDTemplate - Contains the ID number of a dialog-template resource.
	//     lpszTemplateName - Contains a NULL-terminated string that is
	//                        the name of a dialog-template resource.
	//     pParentWnd - Pointer to the parent of the Dialog control.
	//-----------------------------------------------------------------------
	CXTPDialogEx()
	{
	}

	CXTPDialogEx(UINT nIDTemplate, CWnd* pParentWnd = NULL) //<combine CXTPDialogEx::CXTPDialogEx>
		: CXTPDialogBase<CDialog>(nIDTemplate, pParentWnd)
	{
	}

	CXTPDialogEx(LPCTSTR lpszTemplateName,
				 CWnd* pParentWnd = NULL) //<combine CXTPDialogEx::CXTPDialogEx>
		: CXTPDialogBase<CDialog>(lpszTemplateName, pParentWnd)
	{
	}

	//{{AFX_CODEJOCK_PRIVATE
#	ifdef _XTP_INCLUDE_COMMANDBARS
	_XTP_DEPRECATE("Use CXTPCommandBars::SetMenu instead")
	void SetMenuBar(CXTPMenuBar* pMenuBar);
#	endif
	//}}AFX_CODEJOCK_PRIVATE
};

#	if (_MSC_VER > 1200)
//===========================================================================
// Summary:
//     CXTPDHtmlDialogEx is a CXTPDialogBase derived class. Use this class in
//     your html dialog base application.
//===========================================================================
class _XTP_EXT_CLASS CXTPDHtmlDialogEx : public CXTPDialogBase<CDHtmlDialog>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDHtmlDialogEx object.
	// Parameters:
	//     nHtmlResID - Contains the ID number of a dialog-template resource.
	//     szHtmlResID - Contains a NULL-terminated string that is
	//                   the name of a dialog-template resource.
	//     pParentWnd - Pointer to the parent of the Dialog control.
	//-----------------------------------------------------------------------
	CXTPDHtmlDialogEx()
	{
	}

	CXTPDHtmlDialogEx(UINT nIDTemplate, UINT nHtmlResID = 0,
					  CWnd* pParentWnd = NULL) //<combine CXTPDHtmlDialogEx::CXTPDHtmlDialogEx>
		: CXTPDialogBase<CDHtmlDialog>(nIDTemplate, nHtmlResID, pParentWnd)
	{
	}

	CXTPDHtmlDialogEx(LPCTSTR lpszTemplateName, LPCTSTR szHtmlResID,
					  CWnd* pParentWnd = NULL) //<combine CXTPDHtmlDialogEx::CXTPDHtmlDialogEx>
		: CXTPDialogBase<CDHtmlDialog>(lpszTemplateName, szHtmlResID, pParentWnd)
	{
	}

	//{{AFX_CODEJOCK_PRIVATE
#		ifdef _XTP_INCLUDE_COMMANDBARS
	_XTP_DEPRECATE("Use CXTPCommandBars::SetMenu instead")
	void SetMenuBar(CXTPMenuBar* pMenuBar);
#		endif
	//}}AFX_CODEJOCK_PRIVATE
};
#	endif

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDIALOGBASEEX_H__)
