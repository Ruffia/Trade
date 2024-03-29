// XTPHexEdit.cpp : implementation of the CXTPHexEdit class.
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

#include "stdafx.h"

#include "Controls/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Util/XTPFunctions.h"
#include "Controls/Edit/XTPHexEdit.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CLIPFORMAT CF_BINARY = (CLIPFORMAT)::RegisterClipboardFormat(_T("BinaryData"));

static const BYTE chHexTable[] = { '0', '1', '2', '3', '4', '5', '6', '7',
								   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
#define TOHEX(a, b, t)                                                                             \
	{                                                                                              \
		*b++ = static_cast<t>(chHexTable[a >> 4]);                                                 \
		*b++ = static_cast<t>(chHexTable[a & 0xf]);                                                \
	}

/////////////////////////////////////////////////////////////////////////////
// CXTPHexEdit
/////////////////////////////////////////////////////////////////////////////
CXTPHexEdit::CXTPHexEdit()
{
	m_pData					  = NULL;
	m_nLength				  = 0;
	m_nTopIndex				  = 0;
	m_nCurrentAddress		  = 0;
	m_nSelStart				  = -1;
	m_nSelEnd				  = -1;
	m_nBytePerRow			  = 8;
	m_bDynamicBPR			  = FALSE;
	m_nLinesPerPage			  = 1;
	m_nLineHeight			  = 0;
	m_nNullWidth			  = 0;
	m_nOffHex				  = 0;
	m_nOffAscii				  = 0;
	m_nOffAddress			  = 0;
	m_bShowHex				  = TRUE;
	m_bShowAscii			  = TRUE;
	m_bShowAddress			  = TRUE;
	m_bAddressIsWide		  = TRUE;
	m_bShowCaret			  = TRUE;
	m_bUpdate				  = TRUE;
	m_bAllowDeletes			  = true;
	m_ptEditPos.x			  = 0;
	m_ptEditPos.y			  = 0;
	m_eEditMode				  = editNone;
	m_szCaret				  = CSize(0, 0);
	m_crBack				  = GetXtremeColor(COLOR_WINDOW);
	m_crText				  = GetXtremeColor(COLOR_WINDOWTEXT);
	m_crDisabledBack		  = GetXtremeColor(COLOR_3DFACE);
	m_crDisabledText		  = GetXtremeColor(COLOR_GRAYTEXT);
	m_crHighlightBack		  = GetXtremeColor(COLOR_HIGHLIGHT);
	m_crHighlightText		  = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crDisabledHighlightText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crDisabledHighlightBack = GetXtremeColor(COLOR_3DSHADOW);

	m_nMaxLength	= 0;
	m_dwBaseAddress = 0;

	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		AfxOleInit();
	}

	// create the default font used by the hex edit control.
	m_xtpFontHex.CreateFont(XTP_DPI_Y(-12), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
}

CXTPHexEdit::~CXTPHexEdit()
{
	if (m_xtpFontHex.GetSafeHandle())
		m_xtpFontHex.DeleteObject();

	if (m_pData)
	{
		free(m_pData);
		m_pData = NULL;
	}
}

IMPLEMENT_DYNAMIC(CXTPHexEdit, CXTPHexEditBase)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPHexEdit, CXTPHexEditBase)
	//{{AFX_MSG_MAP(CXTPHexEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_WM_NCPAINT()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPHexEdit::RestoreDefaultFont()
{
	if (m_xtpFontHex.GetSafeHandle())
		m_xtpFontHex.DeleteObject();

	// create the default font used by the hex edit control.
	m_xtpFontHex.CreateFont(XTP_DPI_Y(-12), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
}

void CXTPHexEdit::SetHexFont(CFont* pFont)
{
	ASSERT(pFont->GetSafeHandle());

	if (pFont->GetSafeHandle())
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		SetHexFont(&lf);
	}
}

void CXTPHexEdit::SetHexFont(LOGFONT* pLogFont)
{
	ASSERT(pLogFont != NULL);

	if (m_xtpFontHex.GetSafeHandle())
		m_xtpFontHex.DeleteObject();

	m_xtpFontHex.CreateFontIndirect(pLogFont);
}

COLORREF CXTPHexEdit::GetActualBackColor()
{
#ifdef _XTP_ACTIVEX
	if (!IsWindowEnabled())
#else
	if (IsReadOnly())
#endif
	{
		return m_crDisabledBack;
	}

	return m_crBack;
}

COLORREF CXTPHexEdit::GetActualTextColor()
{
	if (!IsWindowEnabled())
	{
		return m_crDisabledText;
	}

	return m_crText;
}

COLORREF CXTPHexEdit::GetActualHighlightBackColor()
{
	if (!IsWindowEnabled())
	{
		return m_crDisabledHighlightBack;
	}

	return m_crHighlightBack;
}

COLORREF CXTPHexEdit::GetActualHighlightTextColor()
{
	if (!IsWindowEnabled())
	{
		return m_crDisabledHighlightText;
	}

	return m_crHighlightText;
}

void CXTPHexEdit::OnPaint()
{
	CPaintDC dc(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc);

	OnDraw(&memDC);
}

LRESULT CXTPHexEdit::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC);
	}
	return 1;
}

void CXTPHexEdit::OnDraw(CDC* pDC)
{
	// Get the client rect.
	CXTPClientRect rectClient(this);

	pDC->FillSolidRect(rectClient, GetActualBackColor());

	CXTPDCSaveState dcState(pDC);

	ASSERT(m_nCurrentAddress >= 0);
	ASSERT(m_nTopIndex >= 0);

	pDC->SelectObject(&m_xtpFontHex);

	int height = 0;
	int x	  = rectClient.TopLeft().x;
	int y	  = rectClient.TopLeft().y;

	int szBuf  = XTPToIntChecked(m_nBytePerRow * (4 * sizeof(TCHAR)));
	TCHAR* buf = new TCHAR[XTPToUIntChecked(szBuf)];
	if (!buf)
		return;

	pDC->SetBoundsRect(&rectClient, DCB_DISABLE);
	pDC->SetTextColor(GetActualTextColor());

	// Get char dimensions
	if (m_bUpdate)
	{
		const int h_delta = ::GetScrollPos(m_hWnd, SB_HORZ) * m_nNullWidth;

		pDC->GetCharWidth('0', '0', &m_nNullWidth);
		CSize sz	  = pDC->GetTextExtent(_T("0"), 1);
		m_nLineHeight = sz.cy;

		const int width_address = (m_bAddressIsWide ? 9 : 5) * m_nNullWidth;
		m_nOffHex				= m_bShowAddress ? width_address : 0;

		m_nOffAscii = m_nOffHex;
		if (m_bShowHex)
			m_nOffAscii += m_nBytePerRow * 3 * m_nNullWidth;

		m_nOffAddress = -h_delta;
		m_nOffHex -= h_delta;
		m_nOffAscii -= h_delta;

		m_nLinesPerPage = rectClient.Height() / m_nLineHeight;
		m_bUpdate		= FALSE;
		UpdateScrollbars();
	}

	if (m_pData)
	{
		height = rectClient.Height() / m_nLineHeight;
		height *= m_nLineHeight;

		if (m_bShowAddress)
		{
			TCHAR hexBuf[16];
			y		  = 0;
			CRect rcd = rectClient;
			rcd.left  = m_nOffAddress;

			// draw address
			int i;
			for (i = m_nTopIndex; (i < (m_nLength + 1)) && (rcd.TopLeft().y < height);
				 i += m_nBytePerRow)
			{
				const int width = m_bAddressIsWide ? 8 : 4;

#if (_MSC_VER > 1310) // VS2005
				_stprintf_s(hexBuf, _countof(hexBuf), _T("%0*lX"), width, m_dwBaseAddress + i);
#else
				_stprintf(hexBuf, _T("%0*lX"), width, m_dwBaseAddress + i);
#endif
				pDC->DrawText(hexBuf, width, rcd, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);
				rcd.TopLeft().y += m_nLineHeight;
			}
		}
		if (m_bShowHex)
		{
			y		  = 0;
			CRect rcd = rectClient;
			rcd.left = x = m_nOffHex;

			if (m_nSelStart != -1 && (m_eEditMode == editHigh || m_eEditMode == editLow))
			{
				int i;
				int n = 0;
				int selStart, selEnd;
				GetSel(selStart, selEnd);

				for (i = m_nTopIndex; (i < selStart) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p, TCHAR);

					*p++ = ' ';
					pDC->TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}

				pDC->SetTextColor(GetActualHighlightTextColor());
				pDC->SetBkColor(GetActualHighlightBackColor());
				for (; (i < selEnd) && (i < m_nLength) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p, TCHAR);

					*p++ = ' ';
					pDC->TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}
				pDC->SetTextColor(GetActualTextColor());
				pDC->SetBkColor(GetActualBackColor());
				for (; (i < m_nLength) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p, TCHAR);

					*p++ = ' ';
					pDC->TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}
			}
			else
			{
				int i;
				for (i = m_nTopIndex; (i < m_nLength) && (rcd.TopLeft().y < height);)
				{
					LPTSTR p = &buf[0];

					int n;
					for (n = 0; (n < m_nBytePerRow) && (i < m_nLength); n++)
					{
						TOHEX(m_pData[i], p, TCHAR);
						*p++ = ' ';
						i++;
					}

					while (n < m_nBytePerRow)
					{
						*p++ = ' ';
						*p++ = ' ';
						*p++ = ' ';
						n++;
					}

					pDC->DrawText(buf, m_nBytePerRow * 3, rcd,
								  DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);
					rcd.TopLeft().y += m_nLineHeight;
				}
			}
		}
		if (m_bShowAscii)
		{
			y		  = 0;
			CRect rcd = rectClient;
			rcd.left = x = m_nOffAscii;
			if (m_nSelStart != -1 && m_eEditMode == editAscii)
			{
				int i;
				int n = 0;
				int selStart, selEnd;
				GetSel(selStart, selEnd);

				for (i = m_nTopIndex; (i < selStart) && (y < height); i++)
				{
					buf[0] = static_cast<TCHAR>(isprint(m_pData[i]) ? m_pData[i] : '.');
					pDC->TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
				pDC->SetTextColor(GetActualHighlightTextColor());
				pDC->SetBkColor(GetActualHighlightBackColor());
				for (; (i < selEnd) && (y < height); i++)
				{
					buf[0] = static_cast<TCHAR>(isprint(m_pData[i]) ? m_pData[i] : '.');
					pDC->TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
				pDC->SetTextColor(GetActualTextColor());
				pDC->SetBkColor(GetActualBackColor());
				for (; (i < m_nLength) && y < height; i++)
				{
					buf[0] = static_cast<TCHAR>(isprint(m_pData[i]) ? m_pData[i] : '.');
					pDC->TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if (n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
			}
			else
			{
				int i;
				for (i = m_nTopIndex; (i < m_nLength) && (rcd.top < height);)
				{
					LPTSTR p = &buf[0];

					int n;
					for (n = 0; (n < m_nBytePerRow) && (i < m_nLength); n++)
					{
						*p++ = static_cast<TCHAR>(isprint(m_pData[i]) ? m_pData[i] : '.');
						i++;
					}

					pDC->DrawText(buf, n, rcd, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);
					rcd.top += m_nLineHeight;
				}
			}
		}
	}

	if (buf)
		delete[] buf;
}

void CXTPHexEdit::OnSetFocus(CWnd* pOldWnd)
{
	CXTPHexEditBase::OnSetFocus(pOldWnd);
	if (m_pData && !IsSelected())
	{
		if (m_ptEditPos.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}
		RepositionCaret(m_nCurrentAddress);
	}
}

void CXTPHexEdit::OnKillFocus(CWnd* pNewWnd)
{
	DestroyCaret();
	CXTPHexEditBase::OnKillFocus(pNewWnd);
}

void CXTPHexEdit::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	DoVScroll(nSBCode, false);
}

void CXTPHexEdit::DoVScroll(UINT nSBCode, bool bMoveCaret)
{
	if (!m_pData)
	{
		return;
	}

	BOOL bVisible = (0 != (GetStyle() & WS_VISIBLE));

	// Turn off redraw while scrolling, this will cause paint problems
	// with our control because we are painting off screen to reduce
	// flicker.  The default implementation assumes that WM_ERASEBKGND
	// has repainted the background, which in our case does not happen.

	if (bVisible)
		SetRedraw(FALSE);

	const int oa  = m_nTopIndex;
	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	si.fMask	  = SIF_PAGE | SIF_RANGE | SIF_POS;
	if (GetStyle() & WS_VSCROLL)
	{
		VERIFY(::GetScrollInfo(m_hWnd, SB_VERT, &si));
	}
	else
	{
		si.nPage = 0;
		si.nPos = si.nMin = si.nMax = 0;
	}

	switch (nSBCode)
	{
		case SB_LINEDOWN:
			if (si.nPos < (si.nMax - (int)si.nPage + 1))
			{
				m_nTopIndex += m_nBytePerRow;
			}
			break;

		case SB_LINEUP:
			if (m_nTopIndex > 0)
			{
				m_nTopIndex -= m_nBytePerRow;
				if (m_nTopIndex < 0)
					m_nTopIndex = 0;
			}
			break;

		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			if (si.nPos >= si.nMax)
				si.nPos = si.nMax;
			m_nTopIndex = si.nPos * m_nBytePerRow;
			break;

		case SB_PAGEUP:
			if (oa == 0)
			{
				m_nCurrentAddress = 0;
			}
			else
			{
				m_nTopIndex -= m_nBytePerRow * m_nLinesPerPage;
				if (m_nTopIndex < 0)
					m_nTopIndex = 0;
			}
			break;

		case SB_THUMBTRACK:
			si.fMask = SIF_TRACKPOS;

			// Call GetScrollInfo to get current tracking
			// position in si.nTrackPos

			if (!::GetScrollInfo(m_hWnd, SB_VERT, &si))
			{
				return; // GetScrollInfo failed
			}

			m_nTopIndex = si.nTrackPos * m_nBytePerRow;
			break;
	}

	if (bMoveCaret)
	{
		m_nCurrentAddress += (m_nTopIndex - oa);
	}

	// bug fix by John M. Drescher jdrescher@geocities.com
	if (m_nCurrentAddress < 0)
	{
		m_nCurrentAddress = 0;
	}

	UpdateScrollbars();
	RepositionCaret(m_nCurrentAddress);

	if (bVisible)
		SetRedraw(TRUE);

	// repaint the list box
	Invalidate();
	UpdateWindow();

	// repaint the scroll bar.
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
}

BOOL CXTPHexEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTPHexEditBase::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

BOOL CXTPHexEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return Create(_T("EDIT"), 0, dwStyle, rect, pParentWnd, nID);
}

BOOL CXTPHexEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						 const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		TRACE0("Failed to create hex edit control.\n");
		return FALSE;
	}

	CWnd::SetFont(&m_xtpFontHex);

	return TRUE;
}

BOOL CXTPHexEdit::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPHexEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if (!m_pData)
	{
		return;
	}

	if (nFlags & MK_SHIFT)
	{
		m_nSelStart = m_nCurrentAddress;
	}
	CPoint pt = CalcPos(point.x, point.y);
	if (pt.x > -1)
	{
		m_ptEditPos = pt;
		pt.x *= m_nNullWidth;
		pt.y *= m_nLineHeight;

		if (pt.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}

		SetCaretPos(pt);
		if (nFlags & MK_SHIFT)
		{
			m_nSelEnd = m_nCurrentAddress;
			if (m_eEditMode == editHigh || m_eEditMode == editLow)
			{
				m_nSelEnd++;
			}
			RedrawWindow();
		}
	}
	if (!(nFlags & MK_SHIFT))
	{
		if (::DragDetect(m_hWnd, point))
		{
			m_nSelStart = m_nCurrentAddress;
			m_nSelEnd   = m_nSelStart;
			SetCapture();
		}
		else
		{
			BOOL bsel = m_nSelStart != -1;

			m_nSelStart = -1;
			m_nSelEnd   = -1;
			if (bsel)
			{
				RedrawWindow();
			}
		}
	}
	if (!IsSelected())
	{
		if (m_bShowCaret)
			ShowCaret();
	}
}

// in: pixels
// out: character coordinates + change of m_nCurrrentAddress
CPoint CXTPHexEdit::CalcPos(int x, int y)
{
	if (y < 0 || x < 0)
		return CPoint(-1, -1);

	int lastVisRow = m_nLength / m_nBytePerRow - (m_nTopIndex / m_nBytePerRow);
	int row		   = y / m_nLineHeight;
	y			   = row + (m_nTopIndex / m_nBytePerRow);

	//  x += (m_nNullWidth - 1);
	x /= m_nNullWidth;

	// In address area
	const int right_of_address = (m_nOffAddress / m_nNullWidth) + // one past last
								 (!m_bShowAddress ? -1 : (m_bAddressIsWide ? 8 : 4));
	if (x < right_of_address)
	{
		// Set first byte in line as current.
		m_nCurrentAddress = m_nTopIndex + (m_nBytePerRow * row);
		m_eEditMode		  = editNone;
		return CPoint(0, row);
	}

	// In hex area
	const int right_of_hex_area = right_of_address
								  + (!m_bShowHex ? 0 : (m_nBytePerRow * 3)); // 3 = 3 chars per byte
																			 // (e.g. cc<space>)
	if (m_bShowHex && x < right_of_hex_area)
	{
		if ((x % 3) == 2)
		{
			++x;
		}
		const int col	 = (x - right_of_address) / 3;
		m_nCurrentAddress = m_nTopIndex + (m_nBytePerRow * row) + col;

		if (m_nCurrentAddress > m_nLength) // past end ?
		{
			x   = m_nLength % m_nBytePerRow;
			row = lastVisRow;

			x *= 3;
			x += (right_of_address + 1);

			m_nCurrentAddress = m_nLength;
		}

		m_eEditMode = ((x % 3) == 0) ? editHigh : editLow;

		return CPoint(x, row);
	}

	const int right_of_ascii_area = right_of_hex_area + m_nBytePerRow + 1;
	if (m_bShowAscii && x < right_of_ascii_area)
	{
		if (x == right_of_hex_area)
			++x; // advance from margin
		const int col	 = x - right_of_hex_area - 1;
		m_nCurrentAddress = m_nTopIndex + (m_nBytePerRow * row) + col;

		if (m_nCurrentAddress > m_nLength) // past end ?
		{
			x   = m_nLength % m_nBytePerRow;
			row = lastVisRow;
			x += (right_of_hex_area + 1);

			m_nCurrentAddress = m_nLength;
		}

		m_eEditMode = editAscii;
		return CPoint(x, row);
	}

	return CPoint(-1, -1);
}

void CXTPHexEdit::CreateAddressCaret()
{
	DestroyCaret();
	m_szCaret = CSize(m_nNullWidth * (m_bAddressIsWide ? 8 : 4), m_nLineHeight);
	CreateSolidCaret(m_szCaret.cx, m_szCaret.cy);
}

void CXTPHexEdit::CreateEditCaret()
{
	DestroyCaret();
	m_szCaret = CSize(m_nNullWidth, m_nLineHeight);
	CreateSolidCaret(m_szCaret.cx, m_szCaret.cy);
}

void CXTPHexEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_pData)
	{
		return;
	}

	if (nFlags & MK_LBUTTON && m_nSelStart != -1)
	{
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(point))
		{
			if (point.y < 0)
			{
				DoVScroll(SB_LINEUP, true);
				point.y = 0;
			}
			else if (point.y > rc.Height())
			{
				DoVScroll(SB_LINEDOWN, true);
				point.y = rc.Height() - 1;
			}
		}

		//
		// we are selecting
		//
		int seo   = m_nSelEnd;
		CPoint pt = CalcPos(point.x, point.y);
		if (pt.x > -1)
		{
			m_nSelEnd = m_nCurrentAddress;
			if (m_eEditMode == editHigh || m_eEditMode == editLow)
			{
				m_nSelEnd++;
			}
		}
		if (IsSelected())
		{
			DestroyCaret();
		}

		if (seo != m_nSelEnd)
		{
			RedrawWindow();
		}
	}
}

void CXTPHexEdit::UpdateScrollbars()
{
	DWORD dwStyle = GetStyle();

	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	si.fMask	  = SIF_PAGE | SIF_RANGE | SIF_POS;

	if (dwStyle & WS_VSCROLL)
	{
		bool bDisable = false;
		si.nMin		  = 0;
		si.nMax		  = m_nLength / m_nBytePerRow - 1; // - m_nLinesPerPage + 1;
		if (si.nMax < m_nLinesPerPage - 1)
		{
			si.nMax  = 1;
			si.nPage = 1;
			bDisable = true;
			if (m_nTopIndex)
			{
				m_nTopIndex = 0;
				RepositionCaret(m_nCurrentAddress);
			}
		}
		else
		{
			si.nPage = XTPToUIntChecked(m_nLinesPerPage - 1);

			while (m_nTopIndex / m_nBytePerRow > (si.nMax - m_nLinesPerPage + 2) && m_nTopIndex > 0)
			{
				m_nTopIndex -= m_nBytePerRow;
				RepositionCaret(m_nCurrentAddress);
			}
		}
		if (m_nTopIndex < 0)
			m_nTopIndex = 0;
		si.nPos = m_nTopIndex / m_nBytePerRow;

		ModifyStyle(0, WS_VSCROLL);
		if (!bDisable)
		{
			SetScrollInfo(SB_VERT, &si, TRUE);
			EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
		}
		else
		{
			si.nMax  = si.nMin + 1;
			si.nPage = 0;
			si.nPos  = 0;
			SetScrollInfo(SB_VERT, &si, TRUE);
			EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
		}
	}

	if (dwStyle & (WS_HSCROLL | ES_AUTOHSCROLL))
	{
		CRect rc;
		GetClientRect(&rc);

		si.nMin  = 0;
		si.nMax  = ((m_bShowAddress ? (m_bAddressIsWide ? 9 : 5) : 0)
					+ (m_bShowHex ? m_nBytePerRow * 3 : 0) + (m_bShowAscii ? m_nBytePerRow : 0));
		si.nPage = m_nNullWidth == 0 ? 1
									 : XTPToUIntChecked(__min(rc.Width() / m_nNullWidth, si.nMax));
		si.nPos = 0;

		BOOL bVisible = si.nMax * m_nNullWidth >= (int)rc.Width();

		if (dwStyle & ES_AUTOHSCROLL)
		{
			ShowScrollBar(SB_HORZ, bVisible);
		}

		si.fMask &= ~SIF_POS;
		SetScrollInfo(SB_HORZ, &si, TRUE);

		if (bVisible && ((dwStyle & ES_AUTOHSCROLL) == 0))
		{
			EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
		}
	}
}

void CXTPHexEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsSelected())
	{
		ReleaseCapture();
	}

	CXTPHexEditBase::OnLButtonUp(nFlags, point);
}

BOOL CXTPHexEdit::IsReadOnly() const
{
	if (!IsWindowEnabled() || (GetStyle() & ES_READONLY) == ES_READONLY)
	{
		return TRUE;
	}

	return FALSE;
}

void CXTPHexEdit::OnChar(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if (!m_pData)
	{
		return;
	}

	if (m_nCurrentAddress < 0) // Shouldn't happen
	{
		return;
	}

	if (nChar == '\t')
	{
		return;
	}

	if (::GetKeyState(VK_CONTROL) < 0)
	{
		switch (nChar)
		{
			case 0x03:
				if (IsSelected())
				{
					OnEditCopy();
				}
				return;

			case 0x16: OnEditPaste(); return;

			case 0x18:
				if (IsSelected())
				{
					OnEditCut();
				}
				return;

			case 0x1a: OnEditUndo(); return;
		}
	}

	if (IsReadOnly())
	{
		return;
	}

	if (nChar == 0x08) // backspace
	{
		if (GetAllowDeletes())
		{
			if (IsSelected())
			{
				OnEditClear();
			}
			else if (m_nCurrentAddress > 0)
			{
				m_nCurrentAddress--;
				SelDelete(m_nCurrentAddress, m_nCurrentAddress + 1);
				RepositionCaret(m_nCurrentAddress);
				RedrawWindow();
				OnChange();
			}
			else
			{
				MessageBeep(0);
			}
		}
		return;
	}

	// can't go past end of buffer
	if ((UINT)m_nCurrentAddress >= m_nMaxLength)
	{
		MessageBeep(0);
		return;
	}

	if (m_eEditMode != editNone)
	{
		// see if need to grow the buffer
		if (m_nCurrentAddress >= m_nLength)
		{
			int iNewLength = (m_nLength + 1);
			LPBYTE p	   = (LPBYTE)calloc(XTPToSizeTChecked(iNewLength), 1);

			MEMCPY_S(p, m_pData, XTPToSizeTChecked(m_nLength));

			free(m_pData);
			m_pData = p;
			SetSel(-1, -1);
			m_nLength = iNewLength;
			m_bUpdate = TRUE;
		}
	}

	SetSel(-1, -1);
	switch (m_eEditMode)
	{
		case editNone: return;

		case editHigh:
		case editLow:
			if (isxdigit(XTPToIntChecked(nChar)))
			{
				UINT b = 0;

				if (nChar >= 'a')
					b = 10 + nChar - 'a';
				else if (nChar >= 'A')
					b = 10 + nChar - 'A';
				else if (nChar >= '0')
					b = nChar - '0';
				else
					break;

				if (b >= 16)
					break;

				if (m_eEditMode == editHigh)
				{
					m_pData[m_nCurrentAddress] = (unsigned char)((m_pData[m_nCurrentAddress] & 0x0f)
																 | (b << 4));
				}
				else
				{
					m_pData[m_nCurrentAddress] = (unsigned char)((m_pData[m_nCurrentAddress] & 0xf0)
																 | b);
				}
				Move(1, 0);
				OnChange();
			}
			break;

		case editAscii:
			m_pData[m_nCurrentAddress] = (unsigned char)nChar;
			Move(1, 0);
			OnChange();
			break;
	}
	RedrawWindow();
}

void CXTPHexEdit::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	BOOL bShift = (::GetKeyState(VK_SHIFT) < 0);
	switch (nChar)
	{
		case VK_DOWN:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(0, 1);
				m_nSelEnd = m_nCurrentAddress;
				if (m_eEditMode == editHigh || m_eEditMode == editLow)
				{
					m_nSelEnd++;
				}
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(0, 1);
			break;
		case VK_UP:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(0, -1);
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(0, -1);
			break;
		case VK_LEFT:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(-1, 0);
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(-1, 0);
			break;
		case VK_RIGHT:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(1, 0);
				m_nSelEnd = m_nCurrentAddress;
				if (m_eEditMode == editHigh || m_eEditMode == editLow)
				{
					m_nSelEnd++;
				}
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(1, 0);
			break;
		case VK_PRIOR:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				DoVScroll(SB_PAGEUP, true);
				Move(0, 0);
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			DoVScroll(SB_PAGEUP, true);
			Move(0, 0);
			break;
		case VK_NEXT:
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				DoVScroll(SB_PAGEDOWN, true);
				Move(0, 0);
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			DoVScroll(SB_PAGEDOWN, true);
			Move(0, 0);
			break;
		case VK_HOME:
			if (m_eEditMode == editLow)
			{
				m_eEditMode = editHigh;
			}
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				if (::GetKeyState(VK_CONTROL) < 0)
				{
					DoVScroll(SB_THUMBTRACK, true);
					Move(0, 0);
				}
				else
				{
					m_nCurrentAddress /= m_nBytePerRow;
					m_nCurrentAddress *= m_nBytePerRow;
					Move(0, 0);
				}
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			if (::GetKeyState(VK_CONTROL) < 0)
			{
				m_nTopIndex = m_nCurrentAddress = 0;
				UpdateScrollbars();
				RepositionCaret(m_nCurrentAddress);
				RedrawWindow();
			}
			else
			{
				m_nCurrentAddress /= m_nBytePerRow;
				m_nCurrentAddress *= m_nBytePerRow;
				Move(0, 0);
			}
			break;
		case VK_END:
			if (m_eEditMode == editLow)
			{
				m_eEditMode = editHigh;
			}
			if (bShift)
			{
				if (!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				if (::GetKeyState(VK_CONTROL) < 0)
				{
					m_nCurrentAddress = m_nLength - 1;
					DoVScroll(SB_PAGEDOWN, false);
					Move(0, 0);
				}
				else
				{
					m_nCurrentAddress /= m_nBytePerRow;
					m_nCurrentAddress *= m_nBytePerRow;
					m_nCurrentAddress += m_nBytePerRow - 1;
					if (m_nCurrentAddress > m_nLength)
					{
						m_nCurrentAddress = m_nLength;
					}
					Move(0, 0);
				}
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			if (::GetKeyState(VK_CONTROL) < 0)
			{
				m_nCurrentAddress = m_nLength;
				DoVScroll(SB_PAGEDOWN, false);
				Move(0, 0);
			}
			else
			{
				m_nCurrentAddress /= m_nBytePerRow;
				m_nCurrentAddress *= m_nBytePerRow;
				m_nCurrentAddress += m_nBytePerRow - 1;
				if (m_nCurrentAddress > m_nLength)
					m_nCurrentAddress = m_nLength;
				Move(0, 0);
			}
			break;
		case VK_INSERT:
			if (IsReadOnly())
				break;

			SelInsert(m_nCurrentAddress, __max(1, m_nSelEnd - m_nSelStart));
			RepositionCaret(m_nCurrentAddress);
			OnChange();
			RedrawWindow();
			break;
		case VK_DELETE:
			if (IsReadOnly())
				break;

			if (GetAllowDeletes())
			{
				if (IsSelected())
				{
					OnEditClear();
				}
				else
				{
					if (m_nCurrentAddress < m_nLength)
					{
						SelDelete(m_nCurrentAddress, m_nCurrentAddress + 1);
						RedrawWindow();
						OnChange();
					}
					else
					{
						MessageBeep(0);
					}
				}
			}
			break;

		case VK_TAB:
		{
			int next_win_sel = 0;
			switch (m_eEditMode)
			{
				case editNone: m_eEditMode = editHigh; break;
				case editHigh:
				case editLow:
					if (bShift)
					{
						next_win_sel = GW_HWNDPREV;
					}
					else
					{
						m_eEditMode = editAscii;
					}
					break;
				case editAscii:
					if (bShift)
					{
						m_eEditMode = editHigh;
					}
					else
					{
						next_win_sel = GW_HWNDNEXT;
					}
					break;
			}
			if (next_win_sel)
			{
				CWnd* pWnd = this;
				for (;;)
				{
					pWnd = pWnd->GetNextWindow(XTPToUIntChecked(next_win_sel));
					if (pWnd == this)
					{
						break;
					}
					if (pWnd == NULL)
					{
						if (next_win_sel == GW_HWNDNEXT)
						{
							pWnd = GetNextWindow(GW_HWNDFIRST);
						}
						else
						{
							pWnd = GetNextWindow(GW_HWNDLAST);
						}
					}
					if (pWnd->GetStyle() & WS_TABSTOP)
					{
						pWnd->SetFocus();
						break;
					}
					if (pWnd == this)
					{
						m_eEditMode = editHigh;
						Move(0, 0);
						break;
					}
				}
			}
			else
			{
				Move(0, 0);
			}
			break;
		}
	}
}

void CXTPHexEdit::Move(int x, int y)
{
	switch (m_eEditMode)
	{
		case editNone: return;

		case editHigh:
			if (x != 0)
				m_eEditMode = editLow;
			if (x == -1)
				m_nCurrentAddress--;
			m_nCurrentAddress += y * m_nBytePerRow;
			break;

		case editLow:
			if (x != 0)
				m_eEditMode = editHigh;
			if (x == 1)
				m_nCurrentAddress++;
			m_nCurrentAddress += y * m_nBytePerRow;
			break;

		case editAscii:
		{
			m_nCurrentAddress += x;
			m_nCurrentAddress += y * m_nBytePerRow;
		}
		break;
	}

	if (m_nCurrentAddress < 0)
	{
		m_nCurrentAddress = 0;
	}

	if (m_nCurrentAddress >= m_nLength)
	{
		m_nCurrentAddress = m_nLength;
	}

	if ((x | y) != 0 && m_nCurrentAddress < m_nTopIndex)
	{
		DoVScroll(SB_LINEUP, false);
	}
	if ((x | y) != 0 && m_nCurrentAddress >= m_nTopIndex + m_nLinesPerPage * m_nBytePerRow)
	{
		DoVScroll(SB_LINEDOWN, false);
	}
	// ScrollIntoView(m_nCurrentAddress);
	RepositionCaret(m_nCurrentAddress);
}

void CXTPHexEdit::SetSel(int nSelStart, int nSelEnd)
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif

	if (GetFocus() == this)
		DestroyCaret();
	m_nSelStart = nSelStart;
	m_nSelEnd   = nSelEnd;
	if (!IsWindow(m_hWnd))
		return;
	RedrawWindow();
	if (GetFocus() == this)
	{
		if (m_ptEditPos.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}
		RepositionCaret(m_nCurrentAddress);

		if (m_bShowCaret)
			ShowCaret();
	}
}

void CXTPHexEdit::RepositionCaret(int p)
{
	if (GetFocus() != this)
		return;

	int x, y;

	y = (p - m_nTopIndex) / m_nBytePerRow;
	x = (p - m_nTopIndex) % m_nBytePerRow;

	switch (m_eEditMode)
	{
		case editNone:
			CreateAddressCaret();
			x = m_nOffAddress;
			break;
		case editHigh:
			CreateEditCaret();
			x *= m_nNullWidth * 3;
			x += m_nOffHex;
			break;
		case editLow:
			CreateEditCaret();
			x *= m_nNullWidth * 3;
			x += m_nNullWidth;
			x += m_nOffHex;
			break;
		case editAscii:
			CreateEditCaret();
			x *= m_nNullWidth;
			x += m_nOffAscii;
			break;
	}
	m_ptEditPos.x = x;
	m_ptEditPos.y = y * m_nLineHeight;
	CRect rc;
	GetClientRect(&rc);
	CRect rcCarret(m_ptEditPos, m_szCaret);
	CRect rcInt;
	rcInt.IntersectRect(rcCarret, rc);
	if (rcInt == rcCarret)
	{
		SetCaretPos(m_ptEditPos);

		if (m_bShowCaret)
			ShowCaret();
	}
}

void CXTPHexEdit::ScrollIntoView(int p)
{
	if (p < m_nTopIndex || p > m_nTopIndex + m_nLinesPerPage * m_nBytePerRow)
	{
		m_nTopIndex = (p / m_nBytePerRow) * m_nBytePerRow;
		m_nTopIndex -= (m_nLinesPerPage / 3) * m_nBytePerRow;
		if (m_nTopIndex < 0)
		{
			m_nTopIndex = 0;
		}

		UpdateScrollbars();
		RedrawWindow();
	}
}

void CXTPHexEdit::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		// keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(XTP_DPI_X(5), XTP_DPI_Y(5));
	}

	CMenu menu;
	XTPResourceManager()->LoadMenu(&menu, XTP_IDM_POPUP);

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);
	if (!pPopup)
		return;

	const int DISABLE_FLAGS = MF_GRAYED | MF_DISABLED | MF_BYCOMMAND;

	pPopup->EnableMenuItem(ID_EDIT_UNDO, DISABLE_FLAGS);

	if (!GetAllowDeletes() || IsReadOnly())
	{
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_CUT, DISABLE_FLAGS);
	}

	if (!IsSelected())
	{
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_CUT, DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_COPY, DISABLE_FLAGS);
	}

	if (IsReadOnly())
	{
		pPopup->EnableMenuItem(ID_EDIT_PASTE, DISABLE_FLAGS);
	}
	else if (OpenClipboard())
	{
		if (!IsClipboardFormatAvailable(CF_TEXT) && !IsClipboardFormatAvailable(CF_BINARY))
		{
			pPopup->EnableMenuItem(ID_EDIT_PASTE, DISABLE_FLAGS);
		}
		::CloseClipboard();
	}

	CXTPImageManager* pImageManager = new CXTPImageManager();
	if (pImageManager)
	{
		CString type	= _T("RT_XAML");
		HMODULE hModule = XTPResourceManager()->GetResourceHandle();

		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_COPY, ID_EDIT_COPY,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_CUT, ID_EDIT_CUT,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_PASTE, ID_EDIT_PASTE,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_UNDO, ID_EDIT_UNDO,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
		pImageManager->SetVectorIcon(hModule, type, IDR_XAML_ICON_HEXEDIT_CANCEL, ID_EDIT_CLEAR,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
	}

	XTPContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, pImageManager);
}

void CXTPHexEdit::OnEditClear()
{
	if (IsReadOnly())
	{
		return;
	}

	if (!GetAllowDeletes())
		return;

	NormalizeSel();
	m_nCurrentAddress = m_nSelStart;
	SelDelete(m_nSelStart, m_nSelEnd);
	RepositionCaret(m_nCurrentAddress);
	RedrawWindow();

	OnChange();
}

void CXTPHexEdit::OnEditCopy()
{
	if (!OpenClipboard())
		return;

	NormalizeSel();

	::EmptyClipboard();

	const int dwLen = m_nSelEnd - m_nSelStart;
	HGLOBAL hMemb   = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT,
									XTPToSizeTChecked(dwLen));
	if (!hMemb)
	{
		CloseClipboard();
		return;
	}

	// copy binary
	ASSERT(hMemb != NULL);
	LPBYTE p = (BYTE*)::GlobalLock(hMemb);
	if (!p)
		return;
	MEMCPY_S(p, m_pData + m_nSelStart, XTPToSizeTChecked(dwLen));
	::GlobalUnlock(hMemb);

	HGLOBAL hMema = NULL;
	if (m_eEditMode != editAscii)
	{
		hMema = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT,
							  XTPToSizeTChecked(dwLen * 3 + 1));
		if (!hMema)
		{
			CloseClipboard();
			return;
		}

		// copy ascii
		p = (BYTE*)::GlobalLock(hMema);
		if (!p)
			return;

		int i;
		for (i = 0; i < dwLen;)
		{
			TOHEX(m_pData[m_nSelStart + i], p, BYTE);
			*p++ = ' ';
			i++;
		}

		::GlobalUnlock(hMema);
	}
	else
	{
		hMema = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT,
							  XTPToSizeTChecked(dwLen + 1));
		if (!hMema)
		{
			CloseClipboard();
			return;
		}

		// copy ascii
		p = (BYTE*)::GlobalLock(hMema);
		if (!p)
			return;
		MEMCPY_S(p, m_pData + m_nSelStart, XTPToSizeTChecked(dwLen));
		p[dwLen] = 0;

		for (int i = 0; i < dwLen; p++, i++)
		{
			if (!isprint(*p))
			{
				*p = '.';
			}
		}
		::GlobalUnlock(hMema);
	}

	::SetClipboardData(CF_TEXT, hMema);
	::SetClipboardData(CF_BINARY, hMemb);

	VERIFY(CloseClipboard());
}

void CXTPHexEdit::OnEditCut()
{
	if (IsReadOnly())
		return;

	OnEditCopy();
	SelDelete(m_nSelStart, m_nSelEnd);
	RedrawWindow();
}

void CXTPHexEdit::OnEditPaste()
{
	if (IsReadOnly())
		return;

	if (!OpenClipboard())
		return;

	HGLOBAL hmem = NULL;
	LPBYTE p	 = NULL;
	DWORD dwLen  = 0;

	if (::IsClipboardFormatAvailable(CF_BINARY))
	{
		hmem  = ::GetClipboardData(CF_BINARY);
		p	 = hmem ? (BYTE*)::GlobalLock(hmem) : NULL;
		dwLen = hmem ? (DWORD)::GlobalSize(hmem) : 0;
	}
	else if (::IsClipboardFormatAvailable(CF_TEXT))
	{
		hmem  = ::GetClipboardData(CF_TEXT);
		p	 = hmem ? (BYTE*)::GlobalLock(hmem) : NULL;
		dwLen = p ? (DWORD)strlen((char*)p) : 0;
	}

	if (p && dwLen > 0)
	{
		int insert;

		if (m_nCurrentAddress > m_nLength)
			m_nCurrentAddress = m_nLength;

		int oa = m_nCurrentAddress;

		NormalizeSel();
		if (m_nSelStart == -1)
		{
			if (m_eEditMode == editLow)
			{
				m_nCurrentAddress++;
			}
			if (m_nCurrentAddress > m_nLength)
			{
				m_nCurrentAddress = m_nLength;
			}
			insert = m_nCurrentAddress;
			SelInsert(m_nCurrentAddress, XTPToIntChecked(dwLen));
		}
		else
		{
			insert = m_nSelStart;
			SelDelete(m_nSelStart, m_nSelEnd);
			SelInsert(insert, XTPToIntChecked(dwLen));
			SetSel(-1, -1);
		}

		if (insert + dwLen > m_nMaxLength)
		{
			dwLen = m_nMaxLength - insert;
		}

		MEMCPY_S(m_pData + insert, p, dwLen);

#ifdef _DEBUG
		_CrtCheckMemory();
#endif
		OnChange();

		m_nCurrentAddress = oa;
		RedrawWindow();
	}

	if (hmem)
	{
		::GlobalUnlock(hmem);
	}

	::CloseClipboard();
}

void CXTPHexEdit::OnEditSelectAll()
{
	m_nSelStart = 0;
	m_nSelEnd   = m_nLength;
	DestroyCaret();
	RedrawWindow();
}

void CXTPHexEdit::OnEditUndo()
{
}

void CXTPHexEdit::NormalizeSel()
{
	if (m_nSelStart > m_nSelEnd)
	{
		int nSelStart = m_nSelStart;
		m_nSelStart   = m_nSelEnd;
		m_nSelEnd	 = nSelStart;
	}

	if (m_nSelStart > m_nLength)
	{
		m_nSelStart = m_nLength;
	}

	if (m_nSelEnd > m_nLength)
	{
		m_nSelEnd = m_nLength;
	}
}

void CXTPHexEdit::GetSel(int& nSelStart, int& nSelEnd)
{
	nSelStart = m_nSelStart;
	nSelEnd   = m_nSelEnd;

	if (nSelStart > nSelEnd)
	{
		nSelStart = m_nSelEnd;
		nSelEnd   = m_nSelStart;
	}

	if (nSelStart > m_nLength)
		nSelStart = m_nLength;

	if (nSelEnd > m_nLength)
		nSelEnd = m_nLength;
}

void CXTPHexEdit::OnChange()
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (pOwner != NULL)
	{
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE), (LPARAM)m_hWnd);
	}
}

void CXTPHexEdit::SelDelete(int nSelStart, int nSelEnd)
{
	if (!GetAllowDeletes())
		return;

#ifdef _DEBUG
	_CrtCheckMemory();
#endif

	if (nSelStart >= m_nLength)
		return;

	NormalizeSel();

	const int nSelSize  = (nSelEnd - nSelStart);
	const int newLength = m_nLength - nSelSize;
	if (newLength < 0)
		return;

	MEMMOVE_S(m_pData + nSelStart, m_pData + nSelEnd, XTPToSizeTChecked(m_nLength - nSelEnd));

	SetSel(-1, -1);

	m_nLength = newLength;
	if (m_nCurrentAddress > m_nLength)
	{
		m_nCurrentAddress = m_nLength;
		RepositionCaret(m_nCurrentAddress);
	}
	m_bUpdate = TRUE;
}

void CXTPHexEdit::SelInsert(int nSelStart, int nLength)
{
	if ((UINT)(m_nLength + nLength) > m_nMaxLength)
	{
		nLength = (int)m_nMaxLength - m_nLength;
	}
	if (nLength <= 0)
	{
		MessageBeep(0);
		return;
	}
	if (nSelStart > m_nLength)
	{
		nSelStart = m_nLength;
	}

	LPBYTE p = (LPBYTE)calloc(XTPToSizeTChecked(m_nLength + nLength), 1);

	MEMCPY_S(p, m_pData, XTPToSizeTChecked(nSelStart));

	if (m_nLength > nSelStart)
	{
		MEMCPY_S(p + nSelStart + nLength, m_pData + nSelStart,
				 XTPToSizeTChecked(m_nLength - nSelStart));
	}

	free(m_pData);
	m_pData = p;
	SetSel(-1, -1);
	m_nLength = m_nLength + nLength;
	m_bUpdate = TRUE;
}

void CXTPHexEdit::SetData(LPBYTE p, int nLength, int nMaxLength)
{
	if (m_pData)
	{
		free(m_pData);
		m_nLength = 0;
	}

	m_pData = (LPBYTE)malloc(XTPToSizeTChecked(nLength));
	MEMCPY_S(m_pData, p, XTPToSizeTChecked(nLength));

	SetSel(-1, -1);
	m_nMaxLength	  = nMaxLength < 0 ? UINT(-1) : XTPToUIntChecked(__max(nMaxLength, nLength));
	m_nLength		  = nLength;
	m_nCurrentAddress = 0;
	m_ptEditPos.x = m_ptEditPos.y = 0;
	m_eEditMode					  = editHigh;
	m_nTopIndex					  = 0;
	m_bUpdate					  = TRUE;

	if (m_hWnd)
	{
		UpdateScrollbars();
		RepositionCaret(m_nCurrentAddress);
		RedrawWindow();

		// this will force the scroll bars to update if this is before the
		// initial update
		if (GetStyle() & WS_VSCROLL)
		{
			PostMessage(WM_VSCROLL, (WPARAM)-1, 0);
		}
	}
}

void CXTPHexEdit::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	SCROLLINFO si = { 0 };
	si.cbSize	 = sizeof(SCROLLINFO);
	si.fMask	  = SIF_PAGE | SIF_RANGE | SIF_POS | SIF_TRACKPOS;
	VERIFY(::GetScrollInfo(m_hWnd, SB_HORZ, &si));

	int newpos = si.nPos;

	switch (nSBCode)
	{
		case SB_LINELEFT: --newpos; break;

		case SB_LINERIGHT: ++newpos; break;

		case SB_PAGELEFT: newpos -= si.nPage; break;

		case SB_PAGERIGHT: newpos += si.nPage; break;

		case SB_THUMBTRACK: newpos = si.nTrackPos; break;
	}

	if (newpos < 0)
	{
		newpos = 0;
	}
	else if (newpos > si.nMax)
	{
		newpos = si.nMax;
	}

	if (newpos != si.nPos)
	{
		m_bUpdate = true;
		::SetScrollPos(m_hWnd, SB_HORZ, newpos, TRUE);
		RedrawWindow();
		RepositionCaret(m_nCurrentAddress);
	}
}

void CXTPHexEdit::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/)
{
}

DWORD CXTPHexEdit::SetAddressBase(DWORD dwBase)
{
	DWORD dwOldAddress = m_dwBaseAddress;
	m_dwBaseAddress	= dwBase;
	m_bUpdate		   = TRUE;

	return dwOldAddress;
}

void CXTPHexEdit::OnSize(UINT nType, int cx, int cy)
{
	BOOL bVisible = (0 != (GetStyle() & WS_VISIBLE));

	if (bVisible)
		SetRedraw(FALSE);

	CXTPHexEditBase::OnSize(nType, cx, cy);

	if (m_bDynamicBPR)
	{
		RecalcBPR();
	}

	m_bUpdate = TRUE;
	UpdateScrollbars();

	if (bVisible)
	{
		SetRedraw(TRUE);
		Invalidate();
		UpdateWindow();
	}
}

void CXTPHexEdit::RecalcBPR()
{
	CDC* dc			 = GetDC();
	HGDIOBJ hOldFont = dc->SelectObject(&m_xtpFontHex);

	dc->GetCharWidth('0', '0', &m_nNullWidth);
	CRect rc;

	GetClientRect(rc);
	int cx = rc.Width();

	if (m_bShowAddress)
		cx -= m_nOffHex;

	if (m_bShowAscii && m_bShowHex)
	{
		m_nBytePerRow = ((cx * 2) / 3) / (3 * m_nNullWidth); // 2/3rd is hex, 1/3rd is ascii
	}
	else if (m_bShowHex)
	{
		m_nBytePerRow = cx / (3 * m_nNullWidth);
	}
	else if (m_bShowAscii)
	{
		m_nBytePerRow = cx / m_nNullWidth;
	}
	m_bUpdate = TRUE;

	if (m_nBytePerRow <= 0)
		m_nBytePerRow = 1;

	dc->SelectObject(hOldFont);
	ReleaseDC(dc);
}

void CXTPHexEdit::OnNcPaint()
{
	// Use the OnNcPaint() handler to prevent the caret
	// from being shown when the control is first displayed

	CXTPHexEditBase::OnNcPaint(); // Be sure and call the parent class
								  // HideCaret();  // Hide the caret
}

BOOL CXTPHexEdit::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	OnVScroll(UINT(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP), 0, NULL);
	return TRUE;
}

void CXTPHexEdit::EnableScrollBars(bool /*bVertical*/, bool /*bHorizontal*/)
{
	XTP_ERROR_MSG("CXTPHexEdit::EnableScrollBars has been deprecated.\n"
				  "\n"
				  "You should now set scroll bars for the control through WS_HSCROLL/WS_VSCROLL "
				  "and/or the resource editor.");
}

int CXTPHexEdit::GetData(LPBYTE pData, int nLength)
{
	MEMCPY_S(pData, m_pData, XTPToSizeTChecked(__min(nLength, m_nLength)));
	return m_nLength;
}
