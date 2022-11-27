#pragma once

#include <Afxtempl.h>
#include <afxcmn.h>

#include "themeutil.h"

#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL		0x400000
#endif

// CustomTabCtrlItem

#define TAB_SHAPE1		0		//  Invisible

#define TAB_SHAPE2		1		//	 __
								//	| /
								//	|/

#define TAB_SHAPE3		2		//	|\
								//	|/

#define TAB_SHAPE4		3		//	____________
								//	\          /
								//   \________/

#define TAB_SHAPE5		4		//	___________
								//	\          \
								//	  \________/

#define RECALC_PREV_PRESSED			0
#define RECALC_NEXT_PRESSED			1
#define RECALC_ITEM_SELECTED		2
#define RECALC_RESIZED				3
#define RECALC_FIRST_PRESSED		4
#define RECALC_LAST_PRESSED			5
#define RECALC_EDIT_RESIZED			6
#define RECALC_CLOSE_PRESSED		7

#define MAX_LABEL_TEXT				30

typedef struct _CTC_NMHDR 
{
    NMHDR hdr;
	int	nItem;
	TCHAR pszText[MAX_LABEL_TEXT];
	LPARAM lParam;
	RECT rItem;
	POINT ptHitTest;
	BOOL fSelected;
	BOOL fHighlighted;
} CTC_NMHDR;

class CCustomTabCtrlItem
{
	friend class CCustomTabCtrl;
private:
								CCustomTabCtrlItem(CString sText, LPARAM lParam);
	void						ComputeRgn(BOOL fOnTop);
	void						Draw(CDC& dc, CFont& font, BOOL fOnTop, BOOL fRTL);
	BOOL						HitTest(CPoint pt)			{ return (m_bShape && m_rgn.PtInRegion(pt)) ? TRUE : FALSE; }
	void						GetRegionPoints(const CRect& rc, CPoint* pts, BOOL fOnTop) const;
	void						GetDrawPoints(const CRect& rc, CPoint* pts, BOOL fOnTop) const;
	void						operator=(const CCustomTabCtrlItem &other);
private:
	CString						m_sText;
	LPARAM						m_lParam;
	CRect						m_rect;
	CRect						m_rectText;
	CRgn						m_rgn;			
	BYTE						m_bShape;
	BOOL						m_fSelected;
	BOOL						m_fHighlighted;
	BOOL						m_fHighlightChanged;
};
