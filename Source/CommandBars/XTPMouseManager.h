// XTPMouseManager.h : interface for the CXTPMouseManager class.
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
#if !defined(__XTPMOUSEMANAGER_H__)
#	define __XTPMOUSEMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPCommandBar;

//===========================================================================
// Summary:
//     CXTPMouseManager is standalone class.
//     It used in CommandBars to manage the mouse.
//===========================================================================
class _XTP_EXT_CLASS CXTPMouseManager : public CNoTrackObject
{
	friend class CXTPMouseManager* XTPMouseManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMouseManager object.
	//-----------------------------------------------------------------------
	CXTPMouseManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Array of the tracking bars.
	//-----------------------------------------------------------------------
	class _XTP_EXT_CLASS CTrackArray : public CArray<CXTPCommandBar*, CXTPCommandBar*>
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Call this method to find the position of a specified command bar.
		// Parameters:
		//     pCommandBar - CommandBar to find.
		// Returns:
		//     The position of the command bar in the list if successful.
		//     If unsuccessful, a value of -1.
		//-----------------------------------------------------------------------
		int Find(const CXTPCommandBar* pCommandBar) const;
	};

private:
	class CTrustedArray : public CArray<HWND, HWND>
	{
	public:
		int Find(const HWND hWnd) const;
		void Remove(HWND hWnd);
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMouseManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPMouseManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a tracking bar.
	// Parameters:
	//     pTrack        - Pointer to a CXTPCommandBar object.
	//     bCaptureMouse - TRUE to capture the mouse.
	//-----------------------------------------------------------------------
	void SetTrack(CXTPCommandBar* pTrack, BOOL bCaptureMouse = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the tracking bar.
	// Parameters:
	//     pTrack - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	void RemoveTrack(CXTPCommandBar* pTrack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all tracking bars.
	//-----------------------------------------------------------------------
	void SendTrackLost();

	//-----------------------------------------------------------------------
	// Summary:
	//     Hooks window for the mouse leave event.
	// Parameters:
	//     hwnd - Window handle.
	//-----------------------------------------------------------------------
	void TrackMouseLeave(HWND hwnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if the command bar is tracked.
	// Parameters:
	//     pItem - Pointer to a CXTPCommandBar object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsTrackedLock(const CXTPCommandBar* pItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the selected command bar.
	// Parameters:
	//     pSelected - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	void SetSelected(CXTPCommandBar* pSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the selected command bar.
	// Parameters:
	//     pSelected - Pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	void RemoveSelected(CXTPCommandBar* pSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Captures the mouse cursor.
	//-----------------------------------------------------------------------
	void LockMouseMove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Unlocks the mouse cursor.
	//-----------------------------------------------------------------------
	void UnlockMouseMove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if the mouse cursor locked.
	// Returns:
	//     TRUE if the mouse cursor is locked, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsMouseLocked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Check if the command bar needs to be expanded
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsForceExpanded() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets expanded state.
	// Parameters:
	//     bForceExpanded - TRUE to expand bars.
	//-----------------------------------------------------------------------
	void SetForceExpanded(BOOL bForceExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a window to the trusted window list.
	// Parameters:
	//     hWnd - Window handle.
	//-----------------------------------------------------------------------
	void AddTrustedWindow(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a window from the trusted window list.
	// Parameters:
	//     hWnd - Window handle.
	//-----------------------------------------------------------------------
	void RemoveTrustedWindow(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the top parent window is active.
	// Parameters:
	//     hWnd - Window handle.
	// Returns:
	//     TRUE if the top parent window is active, otherwise FALSE.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL IsTopParentActive(HWND hWnd);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to disable one WM_RBUTTONUP message.
	//-------------------------------------------------------------------------
	void IgnoreLButtonUp();

	//-------------------------------------------------------------------------
	// Summary:
	//     Refreshes the mouse cursor.
	//-------------------------------------------------------------------------
	static void AFX_CDECL RefreshCursor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameter:
	//     point - Specifies the point to be tested.
	// Returns:
	//     A pointer to a CXTPCommandBar item that occupies the specified
	//     point or NULL if no item occupies the point.
	//-----------------------------------------------------------------------
	CXTPCommandBar* HitTest(POINT point) const;

	//{{AFX_CODEJOCK_PRIVATE
	BOOL PreviewTackLost(CXTPCommandBar* pContextMenu, MSG* msg) const;
	void LockTrackRecurse(BOOL bLockTrack);
	void SendTrackLostRecurse();
	BOOL IsTrackedLockRecurse() const;
	CTrackArray& GetTrackArray();
	CXTPCommandBar* GetTopTracked() const;
	BOOL IsRelated(HWND hWndParent, HWND hWnd) const;
	void EnableSkipTrackedMouseMoveDuplicates(BOOL bEnable = TRUE);
	//}}AFX_CODEJOCK_PRIVATE

private:
	void DeliverMessage(CXTPCommandBar* pCapture, WPARAM wParam, POINT pt);
	BOOL PreTranslateMouseEvents(WPARAM wParam, POINT point);
	void SetupHook(BOOL);

	static void CALLBACK TrackMouseTimerProc(HWND hWnd, UINT /*uMsg*/, UINT_PTR idEvent,
											 DWORD /*dwTime*/);
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	int m_nLock;
	CTrackArray m_arrTracked;

	static CThreadLocal<CXTPMouseManager> _xtpMouseThreadState;

	HHOOK m_hHookMouse;
	CXTPCommandBar* m_pSelected;
	HWND m_hwndLeave;
	BOOL m_bForceExpanded;
	CTrustedArray m_arrTrusted;
	BOOL m_bIgnoreLButtonUp;
	BOOL m_bSkipTrackedMouseMoveDuplicates;
	CPoint m_ptTrackedPos;

#	ifdef _AFXDLL
	AFX_MODULE_STATE* m_pModuleState;
#	endif

	friend class CXTPCustomizeSheet;
	friend class CXTPCommandBars;
};

AFX_INLINE CXTPMouseManager* XTPMouseManager()
{
	return CXTPMouseManager::_xtpMouseThreadState.GetData();
}

AFX_INLINE CXTPMouseManager::CTrackArray& CXTPMouseManager::GetTrackArray()
{
	return m_arrTracked;
}

AFX_INLINE void CXTPMouseManager::SetSelected(CXTPCommandBar* pSelected)
{
	m_pSelected = pSelected;
}
AFX_INLINE void CXTPMouseManager::RemoveSelected(CXTPCommandBar* pSelected)
{
	if (m_pSelected == pSelected)
		m_pSelected = NULL;
}
AFX_INLINE void CXTPMouseManager::LockMouseMove()
{
	m_nLock++;
}
AFX_INLINE void CXTPMouseManager::UnlockMouseMove()
{
	m_nLock--;
}
AFX_INLINE BOOL CXTPMouseManager::IsMouseLocked() const
{
	return m_nLock > 0;
}

AFX_INLINE BOOL CXTPMouseManager::IsForceExpanded() const
{
	return m_bForceExpanded;
}
AFX_INLINE void CXTPMouseManager::SetForceExpanded(BOOL bForceExpanded)
{
	m_bForceExpanded = bForceExpanded;
}
AFX_INLINE void CXTPMouseManager::IgnoreLButtonUp()
{
	m_bIgnoreLButtonUp = TRUE;
}

AFX_INLINE void CXTPMouseManager::EnableSkipTrackedMouseMoveDuplicates(BOOL bEnable /*= TRUE*/)
{
	m_bSkipTrackedMouseMoveDuplicates = bEnable;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPMOUSEMANAGER_H__)
