// XTPTabClientSticker.h : interface for the CXTPTabClientSticker class.
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
#if !defined(__XTPTABCLIENTSTICKER_H__)
#	define __XTPTABCLIENTSTICKER_H__
//{{AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CWorkspace;
class CXTPImageManager;

//-----------------------------------------------------------------------
// Summary:
//      Identifies tab client sticker part.
//-----------------------------------------------------------------------
enum XTPTabClientStickerPart
{
	xtpTabClientStickerNone = 0,  // No docking stickers are displayed
	xtpTabClientStickerLeft = 1,  // Display the docking sticker indicating the tab can be docked to
								  // the left.
	xtpTabClientStickerRight = 2, // Display the docking sticker indicating the tab can be docked to
								  // the right.
	xtpTabClientStickerTop = 4,   // Display the docking sticker indicating the tab can be docked on
								  // the top.
	xtpTabClientStickerBottom = 8,  // Display the docking sticker indicating the tab can be docked
									// on the bottom.
	xtpTabClientStickerCenter = 16, // Display the docking sticker indicating the tab can be docked
									// in the center.
};

//=======================================================================
// Summary:
//      A sticker base control.
//=======================================================================

class CXTPStickerBase : public CWnd
{
	DECLARE_DYNAMIC(CXTPStickerBase);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Construct a sticker object.
	//-----------------------------------------------------------------------
	CXTPStickerBase();

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles resource cleanup.
	//-----------------------------------------------------------------------
	virtual ~CXTPStickerBase();

	//-----------------------------------------------------------------------
	// Summary:
	//      Creates a sticker control.
	// Returns:
	//      TRUE if the control is successfully create, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Create();
	using CWnd::Create;

	void SetTarget(CWnd* pTarget);
	CWnd* GetTarget() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPTabClientSticker)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	virtual void OnDraw(CDC& dc)  = 0;
	virtual int InitStickers()	= 0;
	virtual CRect GetTargetRect() = 0;
	void IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2);
	void RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc);
	void InitColors();
	void InitSize();

	CWnd* m_pTarget;
	CXTPMarkupContext* m_pMarkupContext;
	CXTPMarkupThemePart m_partBackNormal;
	CXTPMarkupThemePart m_partBackSelected;
	CXTPMarkupThemePart m_partLeftNormal;
	CXTPMarkupThemePart m_partLeftSelected;
	CXTPMarkupThemePart m_partTopNormal;
	CXTPMarkupThemePart m_partTopSelected;
	CXTPMarkupThemePart m_partRightNormal;
	CXTPMarkupThemePart m_partRightSelected;
	CXTPMarkupThemePart m_partBottomNormal;
	CXTPMarkupThemePart m_partBottomSelected;
	CXTPMarkupThemePart m_partCenterNormal;
	CXTPMarkupThemePart m_partCenterSelected;

	CSize m_szIcon;
	int m_delimeterX;
	int m_delimeterY;
	CRect m_rcSticker;
	CRect m_rcBack;
	CRect m_rcLeft;
	CRect m_rcTop;
	CRect m_rcRight;
	CRect m_rcBottom;
	CRect m_rcCenter;
	int m_stickers; // Allowed stickers for tab client
};

//=======================================================================
// Summary:
//      A tab client sticker control.
//=======================================================================
class CXTPTabClientSticker : public CXTPStickerBase
{
	DECLARE_DYNCREATE(CXTPTabClientSticker);

	CXTPTabClientSticker();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Construct a sticker object.
	// Parameters:
	//      pTabclientWnd - A pointer to the associated tab client window.
	// 	   nTabWorkspace - An index of the tab workspace.
	//-----------------------------------------------------------------------
	CXTPTabClientSticker(CXTPTabClientWnd* pTabClientWnd, int nTabWorkspace);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles resource cleanup.
	//-----------------------------------------------------------------------
	virtual ~CXTPTabClientSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines which part of the sticker is under the specified point.
	// Parameters:
	//      pt - A screen point to determine sticker part under.
	// Returns:
	//      Sticker part identifier.
	// See also:
	//      XTPTabClientStickerPart
	//-----------------------------------------------------------------------
	XTPTabClientStickerPart HitTest(POINT pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets the target tab client window pointer.
	// Parameters:
	// 	   pTarget - A pointer to the new target tab client window.
	// Returns:
	//      Target tab client window pointer or NULL if no target is provided.
	//-----------------------------------------------------------------------
	CXTPTabClientWnd* GetTargetTabClientWnd() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPTabClientSticker)
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	virtual void OnDraw(CDC& dc);
	virtual CRect GetTargetRect();
	virtual int InitStickers();
	void InitIcons();

	int m_nTargetWorkspace;
};

#	ifndef _XTP_COMMANDBARS_ACTIVEX
#		ifdef _XTP_INCLUDE_DOCKINGPANE
//=======================================================================
// Summary:
//      A dockpane sticker control.
//=======================================================================
class CXTPDockPaneSticker : public CXTPStickerBase
{
	DECLARE_DYNCREATE(CXTPDockPaneSticker);

	CXTPDockPaneSticker();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Construct a sticker object.
	// Parameters:
	//      pDockPane - A pointer to the associated dockpane client window.
	//-----------------------------------------------------------------------
	CXTPDockPaneSticker(CXTPDockingPaneTabbedContainer* pDockPane);

	//-----------------------------------------------------------------------
	// Summary:
	//      Handles resource cleanup.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockPaneSticker();

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines which part of the sticker is under the specified point.
	// Parameters:
	//      pt - A screen point to determine sticker part under.
	// Returns:
	//      Sticker part identifier.
	// See also:
	//      XTPDockingPaneStickerType
	//-----------------------------------------------------------------------
	XTPDockingPaneStickerType HitTest(POINT pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Gets the target dockpane client window pointer.
	// Parameters:
	// 	   pTarget - A pointer to the new target tab client window.
	// Returns:
	//      Target tab client window pointer or NULL if no target is provided.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer* GetTargetDockPaneWnd() const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPDockPaneSticker)
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

private:
	virtual void OnDraw(CDC& dc);
	virtual CRect GetTargetRect();
	virtual int InitStickers();
	void InitIcons();
};
#		endif
#	endif

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPTABCLIENTSTICKER_H__)
