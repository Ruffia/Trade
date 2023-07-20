// XTPKeyboardManager.h : interface for the CXTPKeyboardManager class.
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
#if !defined(__XTPKEYBOARDMANAGER_H__)
#	define __XTPKEYBOARDMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPKeyboardManager class used to handle keyboard hooks.
//===========================================================================
class _XTP_EXT_CLASS CXTPKeyboardManager : public CNoTrackObject
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPKeyboardManager object.
	//-----------------------------------------------------------------------
	CXTPKeyboardManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPKeyboardManager object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPKeyboardManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start/end keyboard hooks.
	// Parameters:
	//     bSetup - TRUE to start keyboard hooks, FALSE to end keyboard hooks.
	// See Also:
	//     SetupCallWndProcHook
	//-----------------------------------------------------------------------
	void SetupKeyboardHook(BOOL bSetup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start/end message hooks.
	// Parameters:
	//     bSetup - TRUE to start message hooks, FALSE to end message hooks.
	// See Also:
	//     SetupKeyboardHook
	//-----------------------------------------------------------------------
	void SetupCallWndProcHook(BOOL bSetup = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hook keyboard notifications.
	// Parameters:
	//     pHook - CXTPHookManagerHookAble object that will receive keyboard notifications.
	// See Also:
	//     UnhookKeyboard
	//-----------------------------------------------------------------------
	void HookKeyboard(CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to unhook keyboard notifications.
	// Parameters:
	//     pHook - CXTPHookManagerHookAble object to remove.
	// See Also:
	//     HookKeyboard
	//-----------------------------------------------------------------------
	void UnhookKeyboard(CXTPHookManagerHookAble* pHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the keyboard is hooked.
	// Returns:
	//     TRUE if the keyboard is hooked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsKeyboardHooked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process keyboard hooks.
	// Parameters:
	//     nMessage - Message to process.
	//     wParam - Message specified parameter.
	//     lParam - Message specified parameter.
	// Returns:
	//     TRUE if the message was processed, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ProcessKeyboardHooks(UINT nMessage, WPARAM wParam, LPARAM lParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//      Increments/decrements input lock counter. If there is at least
	//      one input lock enabled then tracking won't be reported in any event.
	// See also:
	//      IsInputLocked
	//-----------------------------------------------------------------------
	void LockInput();
	void UnlockInput(); // <combine CXTPKeyboardManager::LockInput>

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if there is at least one input lock.
	// Returns:
	//      TRUE if there is at least one input lock enabled.
	// See also:
	//      LockInput, UnlockInput
	//-----------------------------------------------------------------------
	BOOL IsInputLocked() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GetMessageProc(int code, WPARAM wParam, LPARAM lParam);

#	ifdef _XTP_COMMANDBARS_ACTIVEX
public:
	void SetupGetMessageHook(BOOL bSetup = TRUE);
#	endif
	//}}AFX_CODEJOCK_PRIVATE

protected:
	HHOOK m_hHookKeyboard;	// Keyboard hook.
	HHOOK m_hHookCallWndProc; // Message hook.
	HHOOK m_hHookGetMessage;  // Message hook.
	LONG m_nInputLockCount;   // Number of input locks.

#	ifdef _AFXDLL
	AFX_MODULE_STATE* m_pModuleState; // Module state.
#	endif

	static CThreadLocal<CXTPKeyboardManager> _xtpKeyboardThreadState; // Instance of Keyboard hook.
	CList<CXTPHookManagerHookAble*, CXTPHookManagerHookAble*> m_lstKeyboardHooks; // List of
																				  // keyboard hooks.

private:
	friend CXTPKeyboardManager* XTPKeyboardManager();
	friend class CXTPCommandBars;
};

AFX_INLINE BOOL CXTPKeyboardManager::IsKeyboardHooked() const
{
	return m_lstKeyboardHooks.GetCount() > 0;
}

AFX_INLINE BOOL CXTPKeyboardManager::IsInputLocked() const
{
	return 0 < m_nInputLockCount;
}

AFX_INLINE CXTPKeyboardManager* XTPKeyboardManager()
{
	return CXTPKeyboardManager::_xtpKeyboardThreadState.GetData();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPKEYBOARDMANAGER_H__)
