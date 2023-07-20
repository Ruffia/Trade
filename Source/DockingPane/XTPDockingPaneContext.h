// XTPDockingPaneContext.h : interface for the CXTPDockingPaneContext class.
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
#if !defined(__XTPDOCKINGPANECONTEXT_H__)
#	define __XTPDOCKINGPANECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPDockingPane;
class CXTPDockingPaneBase;
class CXTPDockingPaneManager;
class CXTPDockingPaneContext;
class CXTPMarkupThemePart;
class CXTPImageManager;
class CXTPMarkupContext;

//===========================================================================
// Summary:
//     CXTPDockingPaneContextAlphaWnd is a CWnd derived class that represents
//     an alpha context helper window.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContextAlphaWnd : public CWnd
{
	DECLARE_DYNCREATE(CXTPDockingPaneContextAlphaWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContextAlphaWnd object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextAlphaWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContextAlphaWnd object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContextAlphaWnd();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPDockingPaneContextAlphaWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	friend class CXTPDockingPaneContext;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneContextStickerWnd is a CWnd derived class that represents
//     a sticker window.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContextStickerWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContextStickerWnd object.
	// Parameters:
	//     pContext - Parent context object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextStickerWnd(CXTPDockingPaneContext* pContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContextStickerWnd object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContextStickerWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the sticker.
	// Parameters:
	//     pDC - Device context pointer.
	//-----------------------------------------------------------------------
	void OnDraw(CDC* pDC);

	//{{AFX_CODEJOCK_PRIVATE
public:
	XTPDockingPaneStickerType HitTest(CPoint pt);
	struct SPRITEINFO;

protected:
	void DrawSprite(CDC* pDC, UINT nID, SPRITEINFO* pSpriteInfo, BOOL bClientBitmap = TRUE);
	void DrawTransparent(CDC* pDC, const CPoint& ptDest, const CSize& sz, CBitmap* pBitmap);

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneContextStickerWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

public:
	XTPDockingPaneStickerType m_typeSticker;	 // Type of the sticker.
	XTPDockingPaneStickerType m_selectedSticker; // Selected sticker part.
	CXTPDockingPaneContext* m_pContext;			 // Pointer to a docking pane context.

	friend class CXTPDockingPaneContext;

	void Init();

protected:
	const UINT STICKERS_BACK;
	const UINT STICKERS_CENTER;
	const UINT STICKERS_LEFT;
	const UINT STICKERS_TOP;
	const UINT STICKERS_RIGHT;
	const UINT STICKERS_BOTTOM;

	void InitSize_VC2005();
	void InitIcons_VC2005();
	void InitSize_VC2008();
	void InitIcons_VC2008();
	void InitSize();
	void InitColors();
	void InitIcons();

	CXTPImageManager* m_pImgManager;
	CXTPMarkupContext* m_pMarkupContext;
	CXTPMarkupThemePart* m_partBackNormal;
	CXTPMarkupThemePart* m_partBackSelected;
	CXTPMarkupThemePart* m_partSmallBackNormal;
	CXTPMarkupThemePart* m_partSmallBackSelected;
	CXTPMarkupThemePart* m_partLeftNormal;
	CXTPMarkupThemePart* m_partLeftSelected;
	CXTPMarkupThemePart* m_partTopNormal;
	CXTPMarkupThemePart* m_partTopSelected;
	CXTPMarkupThemePart* m_partRightNormal;
	CXTPMarkupThemePart* m_partRightSelected;
	CXTPMarkupThemePart* m_partBottomNormal;
	CXTPMarkupThemePart* m_partBottomSelected;
	CXTPMarkupThemePart* m_partCenterNormal;
	CXTPMarkupThemePart* m_partCenterSelected;

	CSize m_szIcon;
	int m_delimeterX;
	int m_delimeterY;
	CRect m_rcSticker;
	CRect m_rcBack_3x3;
	CRect m_rcBack_1x1;
	CRect m_rcLeft;
	CRect m_rcTop;
	CRect m_rcRight;
	CRect m_rcBottom;
	CRect m_rcCenter;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneContext is a standalone class. It is used internally
//     for docking and sizing panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContext
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContext object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContext();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContext object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContext();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to drag a pane.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneBase object.
	//     point - Initial mouse position.
	//-----------------------------------------------------------------------
	virtual void Drag(CXTPDockingPaneBase* pPane, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to drag a pane.
	// Parameters:
	//     pPane  - Pointer to a CXTPDockingPaneBase object.
	//     point  - Initial mouse position.
	//     rcPane - Size of the pane.
	//-----------------------------------------------------------------------
	void Drag(CXTPDockingPaneBase* pPane, CPoint point, CRect rcPane);

	//-----------------------------------------------------------------------
	// Summary:
	//      This method is called inside the WM_SIZING handler of a mini-frame
	//      to update the sticky position of the frame.
	// Parameters:
	//      pWnd   - Mini-frame pointer.
	//      nSide  - Side that was dragged by the user.
	//      lpRect - Rectangle of the frame.
	//-----------------------------------------------------------------------
	virtual void OnSizingFloatingFrame(CXTPDockingPaneMiniWnd* pWnd, UINT nSide, LPRECT lpRect);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to track the size of the pane.
	//-----------------------------------------------------------------------
	virtual void Track();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user moves the mouse cursor in track mode.
	// Parameters:
	//     pt - New mouse cursor position.
	//-----------------------------------------------------------------------
	virtual void Move(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize a loop.
	//-----------------------------------------------------------------------
	virtual void InitLoop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when a loop is canceled.
	//-----------------------------------------------------------------------
	virtual void CancelLoop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the focus rectangle.
	// Parameters:
	//     bRemoveRect - TRUE to delete the focus rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawFocusRect(BOOL bRemoveRect = FALSE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to adjust the rectangle.
	// Parameters:
	//     rect - Rectangle to be changed.
	//     pt   - Position of the rectangle to be changed.
	//-----------------------------------------------------------------------
	static void AFX_CDECL AdjustRectangle(CRect& rect, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the cursor to the available work area.
	// Parameters:
	//     pt - Pointer to the mouse cursor.
	//-----------------------------------------------------------------------
	static void AFX_CDECL AdjustCursor(CPoint& pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create a context menu.
	// Parameters:
	//     pWnd - A pointer to a CXTPDockingPaneContextAlphaWnd object.
	//-----------------------------------------------------------------------
	void CreateContextWindow(CXTPDockingPaneContextAlphaWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh the docking stickers.
	//-----------------------------------------------------------------------
	void UpdateDockingStickers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove the docking stickers.
	//-----------------------------------------------------------------------
	void DestroyDockingStickers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new sticker window.
	// Parameters:
	//     rc          - Boundary rectangle of the sticker.
	//     typeSticker - Type of sticker to create.
	// Returns:
	//     New Sticker window.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextStickerWnd* CreateNewSticker(CRect rc,
													   XTPDockingPaneStickerType typeSticker);

	//{{AFX_CODEJOCK_PRIVATE
public:
	CSize GetStickerSize(XTPDockingPaneStickerType type) const;
	XTPDockingContextStickerStyle GetStickerStyle() const;
	static void AFX_CDECL EnsureVisible(CRect& rectDragFrame);

protected:
	void _CreateRectangleRgn(CRgn& rgnResult, CRect rc);
	void _CreateRgn(CRgn& rgnResult, CRect rc, BOOL bTabbedRgn, BOOL bRemove = FALSE);
	virtual void FindContainer(CPoint pt);
	BOOL CanDock(CRect rc, CPoint pt, CXTPDockingPaneBase* pPane, BOOL bInside = TRUE);
	BOOL CanSideDock(CXTPDockingPaneBase* pPane, CRect rcFrame);

	void RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc);
	BOOL IsAllowDockingTo(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction);
	BOOL IsAllowAttachTo(CXTPDockingPaneBase* pPane);
	virtual void UpdateStickyFrame(CRect& rectDragFrame);
	void UpdateStickyFrame(CRect& rectDragFrame, CWnd* pHost);
	void UpdateSizingStickyFrame(UINT nSide, CRect& rectDragFrame, CWnd* pWnd);

	BOOL IsBehind(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pPaneRef);

	void AcquireDesktopDC();
	void ReleaseDesktopDC();

#	ifndef _XTP_ACTIVEX
#		ifdef _XTP_INCLUDE_COMMANDBARS
	BOOL IsAllowAttachToTabClient(CXTPDockingPaneBase* pPane);
#		endif
#	endif

	//}}AFX_CODEJOCK_PRIVATE

private:
	void IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2);
	void ProcessPendingPaintMessages();

protected:
	CXTPDockingPaneContextAlphaWnd m_wndContext;	 // Alpha context control.
	CXTPDockingPaneContextAlphaWnd m_wndAttachedTab; // Attached tab control.

	PVOID m_pfnSetLayeredWindowAttributes; // Pointer to SetLayeredWindowAttributes method.

	BOOL m_bUseDockingStickers; // TRUE to use docking stickers.
	BOOL m_bUseAlphaContext;	// TRUE to use alpha context.

	CRect m_rectStickerPane;				 // Area occupied by the sticker pane.
	CXTPDockingPaneBase* m_pStickerPane;	 // Sticker Pane.
	CXTPDockingPaneBase* m_pLastStickerPane; // Last Sticker Pane.

	CList<CXTPDockingPaneContextStickerWnd*, CXTPDockingPaneContextStickerWnd*>
		m_lstStickers;					// List of the stickers.
	CPoint m_ptLast;					// Holds the last selection point.
	CPoint m_ptSticky;					// Position of the sticky control.
	CRect m_rectDragFrame;				// Area occupied by the drag frame.
	CRect m_rectDragFrameScreen;		// Screen drag frame.
	CXTPDockingPaneManager* m_pManager; // Parent pane manager.
	CXTPDockingPaneBase* m_pPane;		// Pointer to the associated pane.
	BOOL m_bDragKeyboard;				// TRUE if the keyboard was used to drag the pane.

protected:
	//{{AFX_CODEJOCK_PRIVATE
	BOOL m_bResetDC;
	CXTPDockingPaneBase* m_pContainer;
	CRect m_rectContainer;
	BOOL m_bAttachLast;
	BOOL m_bAttach;
	BOOL m_bSideDock;
	BOOL m_bFloatable;
	BOOL m_bDockable;
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bDitherLast;
	CMap<UINT, UINT, CRgn*, CRgn*> m_rgnStickers;
	XTPDockingPaneDirection m_containDirection;
	CDC* m_pDC;
	//}}AFX_CODEJOCK_PRIVATE

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneContextAlphaWnd;
	friend class CXTPDockingPaneContextStickerWnd;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPDOCKINGPANECONTEXT_H__)
