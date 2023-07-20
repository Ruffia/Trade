// XTPTabClientContext.h : interface for the CXTPTabClientContext class.
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
#if !defined(__XTPTABCLIENTCONTEXT_H__)
#	define __XTPTABCLIENTCONTEXT_H__
//{{AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPTabClientAlphaWnd is CWnd derived class represents alpha context helper window
//===========================================================================
class _XTP_EXT_CLASS CXTPAlphaWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabClientAlphaWnd object.
	//-----------------------------------------------------------------------
	CXTPAlphaWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabClientAlphaWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPAlphaWnd();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPTabClientAlphaWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE
};

//===========================================================================
// Summary:
//     CXTPTabClientContext is a standalone class. It is used internally
//     for docking and sizing tabs.
//===========================================================================
class _XTP_EXT_CLASS CXTPTabClientContext
{
private:
	class CXTPTabClientContextSticker
	{
	public:
		CXTPTabClientContextSticker();
		~CXTPTabClientContextSticker();

		void InitContext(CXTPTabClientContext* pContext);

		BOOL Enable();
		BOOL Update();
		void Disable();

		int GetLastWorkspace() const;
		CXTPTabClientWnd* GetLastTarget() const;
		XTPTabClientStickerPart GetLastSelectedSticker() const;
		CXTPTabClientSticker* GetSticker() const;

	private:
		CXTPTabClientContext* m_pContext;
		int m_lastWorkspace;
		CXTPTabClientWnd* m_pLastTarget;
		XTPTabClientStickerPart m_lastSelectedSticker;
		CXTPTabClientSticker* m_sticker;
		HCURSOR m_hCursor;
		HCURSOR m_hCursorNew;
	};

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	class CXTPDockPaneContextSticker
	{
	public:
		CXTPDockPaneContextSticker();
		~CXTPDockPaneContextSticker();

		void InitContext(CXTPTabClientContext* pContext);

		BOOL Enable();
		BOOL Update();
		void Disable();

		CXTPDockingPaneTabbedContainer* GetLastTarget() const;
		XTPDockingPaneStickerType GetLastSelectedSticker() const;
		// CXTPDockPaneSticker* GetSticker() const;

	private:
		CXTPTabClientContext* m_pContext;
		CXTPDockingPaneTabbedContainer* m_pLastTarget;
		XTPDockingPaneStickerType m_lastSelectedSticker;
		CXTPDockPaneSticker* m_sticker;
		HCURSOR m_hCursor;
		HCURSOR m_hCursorNew;
	};
#		endif
#	endif

	class CXTPAlphaWndBase
	{
	public:
		CXTPAlphaWndBase();
		virtual ~CXTPAlphaWndBase();

		void InitContext(CXTPTabClientContext* pContext);
		BOOL Enable();
		virtual BOOL Update() = 0;
		void Disable();

	protected:
		CXTPTabClientContext* m_pContext;
		PVOID m_pfnSetLayeredWindowAttributes;
		CXTPAlphaWnd m_wndAlpha;
		typedef BOOL(WINAPI* PFNSETLAYEREDWINDOWATTRIBUTES)(HWND hwnd, COLORREF crKey, BYTE bAlpha,
															DWORD dwFlags);
	};

	class CXTPTabClientAlphaWnd : public CXTPAlphaWndBase
	{
	public:
		virtual BOOL Update();
	};

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	class CXTPDockPaneAlphaWnd : public CXTPAlphaWndBase
	{
	public:
		virtual BOOL Update();
	};
#		endif
#	endif

	class CXTPDraggingContents
	{
	public:
		CXTPDraggingContents();
		~CXTPDraggingContents();

		void InitContext(CXTPTabClientContext* pContext);

		BOOL Enable();
		void Update();
		void Disable();

	private:
		CXTPTabClientContext* m_pContext;
		CXTPTabClientDraggingContents* m_pWndDrag;
	};

	class CXTPFocusRectBase
	{
	public:
		CXTPFocusRectBase();
		virtual ~CXTPFocusRectBase();

		void InitContext(CXTPTabClientContext* pContext);

		virtual void Enable()			   = 0;
		virtual void Update(CRect rcGroup) = 0;
		void Disable();

	protected:
		void InitLoop(CWnd* pWnd);
		void CancelLoop();
		void DrawFocusRect(BOOL bRemoveRect = FALSE);

		CXTPTabClientContext* m_pContext;
		BOOL m_focusRect;
		CDC* m_pDC;
		CRect m_rcGroup;
		CRect m_rectLast;
		CSize m_sizeLast;
	};

	class CXTPTabFocusRect : public CXTPFocusRectBase
	{
	public:
		virtual void Enable();
		virtual void Update(CRect rcGroup);
	};

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	class CXTPDockFocusRect : public CXTPFocusRectBase
	{
	public:
		virtual void Enable();
		virtual void Update(CRect rcGroup);
	};
#		endif
#	endif

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabClientContext object
	//-----------------------------------------------------------------------
	CXTPTabClientContext();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabClientContext object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabClientContext();

	//-----------------------------------------------------------------------
	// Summary:
	//      Get/set an associated tab client window pointer.
	// Parameters:
	//      pTabClientWnd - a new associated tab client window pointer.
	// Returns:
	//      An associated tab client window pointer.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd* GetTabClient() const;
	void SetTabClient(CXTPTabClientWnd* pTabClientWnd); // <combine
														// CXTPTabClientContext::GetTabClient@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Hit tests tab client window at the specific screen point.
	// Parameters:
	//      pt - Screen point at which a tab client window is to be tested.
	// Returns:
	//      A tab client window pointer at the specific screen point or NULL if no tab client window
	//      is detected.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd* FindTargetTabClient(POINT pt) const;

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	//-----------------------------------------------------------------------
	// Summary:
	//      Hit tests dockpane window at the specific screen point.
	// Parameters:
	//      pt - Screen point at which a dockpane window is to be tested.
	// Returns:
	//      A tab client window pointer at the specific screen point or NULL if no dockpane window
	//      is detected.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer* FindTargetDockPane(POINT pt) const;
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets a flag value that controls whether tab client stickers will be shown when
	//      needed.
	// Parameters:
	//      value - TRUE to show stickers, or FALSE otherwise.
	// Returns:
	//      TRUE if stickers will be shown when needed, or FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetShowTabClientContextStickers() const;
	void SetShowTabClientContextStickers(
		BOOL value); // <combine CXTPTabClientContext::GetShowTabClientContextStickers@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets a flag that controls whether tab client context will be drawn as a
	//      semi-transparent rectangle.
	// Parameters:
	//      value - TRUE to draw it semi-transparent, FALSE otherwise.
	// Returns:
	//      TRUE if tab client context is to be drawn semi-transparent, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetAlphaTabClientContext() const;
	void SetAlphaTabClientContext(
		BOOL value); // <combine
					 // CXTPTabClientContext::GetAlphaTabClientContext@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets a flag that controls whether tab floating frame contents will be shown while
	//      dragging.
	// Parameters:
	//      value - TRUE to show contents while dragging, FALSE otherwise.
	// Returns:
	//      TRUE if contents is to be shown while dragging, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetShowContentsWhileDragging() const;
	void SetShowContentsWhileDragging(
		BOOL value); // <combine CXTPTabClientContext::GetShowContentsWhileDragging@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets/sets a flag that controls whether tab floating floating frame enables frame theme
	//      set for the primary tab client.
	// Parameters:
	//      value - TRUE if frame theme is enabled, FALSE otherwise.
	// Returns:
	//      TRUE if frame theme is enabled, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsFloatingFrameThemeEnabled() const;
	void EnableFloatingFrameTheme(
		BOOL value); // <combine
					 // CXTPTabClientContext::IsFloatingFrameThemeEnabled@const>

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables tab stickers.
	// Returns:
	//      TRUE if stickers are enabled, FALSE otherwise.
	// See also:
	//      DisableTabSticker
	//-----------------------------------------------------------------------
	BOOL EnableTabSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Disables tab stickers.
	// See also:
	//      EnableTabSticker
	//-----------------------------------------------------------------------
	void DisableTabSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates tab stickers.
	//-----------------------------------------------------------------------
	BOOL UpdateTabSticker();

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	//-----------------------------------------------------------------------
	// Summary:
	//      Enables dock stickers.
	// Returns:
	//      TRUE if stickers are enabled, FALSE otherwise.
	// See also:
	//      DisableTabSticker
	//-----------------------------------------------------------------------
	BOOL EnableDockSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Disables dock stickers.
	// See also:
	//      EnableDockSticker
	//-----------------------------------------------------------------------
	void DisableDockSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates dock stickers.
	//-----------------------------------------------------------------------
	BOOL UpdateDockSticker();
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables tab context window.
	// Returns:
	//      TRUE if the context window is enabled.
	// See also:
	//      DisableTabContextWindow.
	//-----------------------------------------------------------------------
	BOOL EnableTabContextWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//      Disables context window.
	// See also:
	//      EnableTabContextWindow
	//-----------------------------------------------------------------------
	void DisableTabContextWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates tab context window.
	// Returns:
	//      TRUE if context window is updated, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL UpdateTabContextWindow();

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	//-----------------------------------------------------------------------
	// Summary:
	//      Enables dock context window.
	// Returns:
	//      TRUE if the context window is enabled.
	// See also:
	//      DisableDockContextWindow.
	//-----------------------------------------------------------------------
	BOOL EnableDockContextWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//      Disables dock context window.
	// See also:
	//      EnableDockContextWindow
	//-----------------------------------------------------------------------
	void DisableDockContextWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates dock context window.
	// Returns:
	//      TRUE if context window is updated, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL UpdateDockContextWindow();
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables/disables dragging tab client context contents.
	//-----------------------------------------------------------------------
	void EnableDraggingContents();
	void DisableDraggingContents(); // <combine CXTPTabClientContext::EnableDraggingContents>

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates tab client context contents while dragging.
	//-----------------------------------------------------------------------
	void UpdateDraggingContents();

	//-----------------------------------------------------------------------
	// Summary:
	//      Enables/disables showing focus rectangle for a dragged tab client context contents.
	//-----------------------------------------------------------------------
	void EnableTabFocusRect();
	void DisableTabFocusRect(); // <combine CXTPTabClientContext::EnableTabFocusRect>

	//-----------------------------------------------------------------------
	// Summary:
	//      Updates focus rectangle.
	// Parameters:
	//      rcGroup - New focus rectangle value.
	//-----------------------------------------------------------------------
	void UpdateTabFocusRect(CRect rcGroup);

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	void EnableDockFocusRect();
	void DisableDockFocusRect();
	void UpdateDockFocusRect(CRect rcGroup);
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the last target tab client window pointed by the mouse while dragging a frame.
	// Returns:
	//      A pointer to the last pointed target tab client window or NULL is there is no such.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd* GetLastTargetTabClient() const;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	CXTPDockingPaneTabbedContainer* GetLastTargetDockPane() const;
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the last pointed workspace index.
	// Returns:
	//      The last pointed workspace index.
	//-----------------------------------------------------------------------
	int GetLastWorkspace() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the last pointed sticker identifier.
	// Returns:
	//      The last pointed sticker identifier.
	// See also:
	//      XTPTabClientStickerPart
	//-----------------------------------------------------------------------
	XTPTabClientStickerPart GetLastSelectedTabClientSticker() const;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	XTPDockingPaneStickerType GetLastSelectedDockSticker() const;
#		endif
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//      Obtains the pointer to the last pointed sticker or NULL is there is no such.
	// Returns:
	//      The pointer to the last pointed sticker or NULL is there is no such.
	//-----------------------------------------------------------------------
	CXTPTabClientSticker* GetSticker() const;

private:
	BOOL m_bShowTabClientContextStickers; // If TRUE, tab client context stickers are drawn when the
										  // tab is being dragged and dropped.
										  // m_bAlphaTabClientContext must be TRUE.
	BOOL m_bAlphaTabClientContext;	 // If TRUE, alpha tab client context is used when dragging a
									   // tab, the shaded area indicates the tab new location of
									   // dropped.
	BOOL m_bShowContentsWhileDragging; // If TRUE, window contents is visible while dragging.
	BOOL m_bThemedFloatingFrames; // If TRUE, floating Tab frame will use the currently set theme.

	CXTPTabClientWnd* m_pTabClientWnd; // Tabbed client.

	CXTPTabClientContextSticker m_tabSticker;
	CXTPTabClientAlphaWnd m_tabAlphaWnd;
	CXTPTabFocusRect m_tabFocusRect;
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
	CXTPDockPaneContextSticker* m_pDockSticker;
	CXTPDockPaneAlphaWnd* m_pDockAlphaWnd;
	CXTPDockFocusRect* m_pDockFocusRect;
#		else
	// Required for padding.
	LPVOID m_pDockSticker;
	LPVOID m_pDockAlphaWnd;
	LPVOID m_pDockFocusRect;
#		endif
#	endif
	CXTPDraggingContents m_draggingContents;
};

AFX_INLINE void CXTPTabClientContext::SetShowTabClientContextStickers(BOOL value)
{
	m_bShowTabClientContextStickers = value;
}

AFX_INLINE void CXTPTabClientContext::SetAlphaTabClientContext(BOOL value)
{
	m_bAlphaTabClientContext = value;
}

AFX_INLINE void CXTPTabClientContext::SetShowContentsWhileDragging(BOOL value)
{
	m_bShowContentsWhileDragging = value;
}

AFX_INLINE void CXTPTabClientContext::EnableFloatingFrameTheme(BOOL value)
{
	m_bThemedFloatingFrames = value;
}

AFX_INLINE BOOL CXTPTabClientContext::GetAlphaTabClientContext() const
{
	return m_bAlphaTabClientContext;
}

AFX_INLINE BOOL CXTPTabClientContext::GetShowTabClientContextStickers() const
{
	return m_bShowTabClientContextStickers;
}

AFX_INLINE BOOL CXTPTabClientContext::GetShowContentsWhileDragging() const
{
	return m_bShowContentsWhileDragging;
}

AFX_INLINE BOOL CXTPTabClientContext::IsFloatingFrameThemeEnabled() const
{
	return m_bThemedFloatingFrames;
}

AFX_INLINE CXTPTabClientWnd* CXTPTabClientContext::GetLastTargetTabClient() const
{
	return m_tabSticker.GetLastTarget();
}
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
AFX_INLINE CXTPDockingPaneTabbedContainer* CXTPTabClientContext::GetLastTargetDockPane() const
{
	return m_pDockSticker->GetLastTarget();
}
#		endif
#	endif
AFX_INLINE int CXTPTabClientContext::GetLastWorkspace() const
{
	return m_tabSticker.GetLastWorkspace();
}

AFX_INLINE XTPTabClientStickerPart CXTPTabClientContext::GetLastSelectedTabClientSticker() const
{
	return m_tabSticker.GetLastSelectedSticker();
}
#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
AFX_INLINE XTPDockingPaneStickerType CXTPTabClientContext::GetLastSelectedDockSticker() const
{
	return m_pDockSticker->GetLastSelectedSticker();
}
#		endif
#	endif
AFX_INLINE CXTPTabClientSticker* CXTPTabClientContext::GetSticker() const
{
	return m_tabSticker.GetSticker();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCLIENTCONTEXT_H__)
