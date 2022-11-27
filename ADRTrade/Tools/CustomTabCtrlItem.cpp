#include "stdafx.h"
#include <AFXPRIV.H>
#include "CustomTabCtrlItem.h"

// CCustomTabCtrlItem

CCustomTabCtrlItem::CCustomTabCtrlItem(CString sText,LPARAM lParam) : 
			m_sText(sText), 
			m_lParam(lParam),	
			m_bShape(TAB_SHAPE1),
			m_fSelected(FALSE),
			m_fHighlighted(FALSE),
			m_fHighlightChanged(FALSE)
{
}

void CCustomTabCtrlItem::operator=(const CCustomTabCtrlItem &other)
{
	m_sText = other.m_sText;
	m_lParam = other.m_lParam;
}

void CCustomTabCtrlItem::ComputeRgn(BOOL fOnTop)
{
	m_rgn.DeleteObject();

	CPoint pts[6];
	GetRegionPoints(m_rect,pts, fOnTop);
	m_rgn.CreatePolygonRgn(pts, 6, WINDING);
}

void CCustomTabCtrlItem::GetRegionPoints(const CRect& rc, CPoint* pts, BOOL fOnTop) const
{
	switch(m_bShape)
	{
	case TAB_SHAPE2:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left, rc.bottom+1);
				pts[1] = CPoint(rc.left, rc.top);
				pts[2] = CPoint(rc.left + rc.Height()/2, rc.bottom+1);
				pts[3] = CPoint(rc.left + rc.Height()/2, rc.bottom+1);
				pts[4] = CPoint(rc.left + rc.Height()/2, rc.bottom+1);
				pts[5] = CPoint(rc.left + rc.Height()/2, rc.bottom+1);
			}
			else
			{
				pts[0] = rc.TopLeft();
				pts[1] = CPoint(rc.left, rc.bottom);
				pts[2] = CPoint(rc.left + rc.Height()/2, rc.top);
				pts[3] = CPoint(rc.left + rc.Height()/2, rc.top);
				pts[4] = CPoint(rc.left + rc.Height()/2, rc.top);
				pts[5] = CPoint(rc.left + rc.Height()/2, rc.top);
			}
		}
		break;
	case TAB_SHAPE3:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left, rc.bottom+1);
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left, rc.top);
				pts[3] = CPoint(rc.left, rc.top);
				pts[4] = CPoint(rc.left, rc.top);
				pts[5] = CPoint(rc.left, rc.top);
			}
			else
			{
				pts[0] = rc.TopLeft();
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left, rc.bottom);
				pts[3] = CPoint(rc.left, rc.bottom);
				pts[4] = CPoint(rc.left, rc.bottom);
				pts[5] = CPoint(rc.left, rc.bottom);
			}
		}
		break;
	case TAB_SHAPE4:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left, rc.bottom+1);
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left + rc.Height()/2, rc.top);
				pts[3] = CPoint(rc.right - rc.Height()/2, rc.top);
				pts[4] = CPoint(rc.right - rc.Height()/4, rc.Height()/2);
				pts[5] = CPoint(rc.right, rc.bottom+1);
			}
			else
			{
				pts[0] = rc.TopLeft();
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left + rc.Height()/2, rc.bottom);
				pts[3] = CPoint(rc.right - rc.Height()/2, rc.bottom);
				pts[4] = CPoint(rc.right - rc.Height()/4, rc.Height()/2);
				pts[5] = CPoint(rc.right, rc.top);
			}
		}
		break;
	case TAB_SHAPE5:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left, rc.bottom+1);
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left + rc.Height()/2 , rc.top);
				pts[3] = CPoint(rc.right - rc.Height()/2, rc.top);
				pts[4] = CPoint(rc.right - rc.Height()/4, rc.Height()/2);
				pts[5] = CPoint(rc.right - rc.Height()/2, rc.bottom+1);
			}
			else
			{
				pts[0] = rc.TopLeft();
				pts[1] = CPoint(rc.left + rc.Height()/4, rc.Height()/2);
				pts[2] = CPoint(rc.left + rc.Height()/2 , rc.bottom);
				pts[3] = CPoint(rc.right - rc.Height()/2, rc.bottom);
				pts[4] = CPoint(rc.right - rc.Height()/4, rc.Height()/2);
				pts[5] = CPoint(rc.right - rc.Height()/2, rc.top);
			}
		}
		break;
	default:
		{
			pts[0] = CPoint(0,0);
			pts[1] = CPoint(0,0);
			pts[2] = CPoint(0,0);
			pts[3] = CPoint(0,0);
			pts[4] = CPoint(0,0);
			pts[5] = CPoint(0,0);
		}
		break;
	}
}

void CCustomTabCtrlItem::GetDrawPoints(const CRect& rc, CPoint* pts, BOOL fOnTop) const
{
	switch(m_bShape)
	{
	case TAB_SHAPE2:
	case TAB_SHAPE3:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left, rc.top);
				pts[1] = CPoint(rc.left + rc.Height()/2, rc.bottom);
			}
			else
			{
				pts[0] = CPoint(rc.left, rc.bottom);
				pts[1] = CPoint(rc.left + rc.Height()/2, rc.top);
			}
		}
		break;
	case TAB_SHAPE4:
	case TAB_SHAPE5:
		{
			if(fOnTop)
			{
				pts[0] = CPoint(rc.left,rc.bottom);
				pts[1] = CPoint(rc.left + rc.Height()/2, rc.top);
				pts[2] = CPoint(rc.right - rc.Height()/2, rc.top);
				pts[3] = CPoint(rc.right, rc.bottom);
			}
			else
			{
				pts[0] = rc.TopLeft();
				pts[1] = CPoint(rc.left + rc.Height()/2, rc.bottom);
				pts[2] = CPoint(rc.right - rc.Height()/2, rc.bottom);
				pts[3] = CPoint(rc.right, rc.top);
			}
		}
		break;
	}

}

void CCustomTabCtrlItem::Draw(CDC& dc, CFont& font, BOOL fOnTop, BOOL fRTL)
{
	COLORREF bgColor = GetSysColor((m_fSelected||m_fHighlighted) ? COLOR_WINDOW     : COLOR_3DFACE);
	COLORREF fgColor = GetSysColor((m_fSelected ||m_fHighlighted) ? COLOR_WINDOWTEXT : COLOR_BTNTEXT);

	CBrush brush(bgColor);

	CPen blackPen(PS_SOLID, 1, GetSysColor(COLOR_BTNTEXT));
	CPen shadowPen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));

	CPoint pts[4];
	CRect rc = m_rect;
	GetDrawPoints(rc, pts, fOnTop);
	
	// Paint item background
	dc.FillRgn(&m_rgn, &brush);	

	CPen* pOldPen = dc.SelectObject(&blackPen);

	// draw item
	switch(m_bShape)
	{
	case TAB_SHAPE2:
	case TAB_SHAPE3:
		{
			dc.MoveTo(pts[0]);
			dc.LineTo(pts[1]);
		
			if(!m_fSelected)
			{
				dc.SelectObject(&shadowPen);
				dc.MoveTo(pts[0].x-1,pts[0].y);
				dc.LineTo(pts[1].x-1,pts[1].y);
			}
		}
		break;
	case TAB_SHAPE4:
	case TAB_SHAPE5:
		{
			dc.MoveTo(pts[0]);
			dc.LineTo(pts[1]);
			
			dc.SelectObject(&shadowPen);
			dc.LineTo(pts[2]);
			
			if(!m_fSelected)
			{
				dc.MoveTo(pts[2].x-1,pts[2].y);
				dc.LineTo(pts[3].x-1,pts[3].y);
			}

			dc.SelectObject(&blackPen);
			dc.MoveTo(pts[2]);
			dc.LineTo(pts[3]);

			// draw item text

			COLORREF bgOldColor = dc.SetBkColor(bgColor);
			COLORREF fgOldColor = dc.SetTextColor(fgColor);
			rc.DeflateRect(rc.Height()/2, 2,rc.Height()/2,2);
			CFont* pOldFont = dc.SelectObject(&font);
			if(fRTL)
				dc.DrawText(m_sText, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_RTLREADING);
			else 
				dc.DrawText(m_sText, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			dc.SelectObject(pOldFont);
			dc.SetTextColor(fgOldColor);
			dc.SetBkColor(bgOldColor);
		}
		break;
	}
	dc.SelectObject(pOldPen);
}