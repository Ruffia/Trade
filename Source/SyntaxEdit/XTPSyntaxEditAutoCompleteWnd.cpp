// XTPSyntaxEditAutoCompleteWnd.cpp: implementation of the CXTPSyntaxEditAutoCompleteWnd class.
//
// (c)1998-2023 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <search.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "SyntaxEdit/Resource.h"

// common includes
#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"

// syntax editor includes
#include "SyntaxEdit/XTPSyntaxEditDefines.h"
#include "SyntaxEdit/XTPSyntaxEditStruct.h"
#include "SyntaxEdit/XTPSyntaxEditAutoCompleteWnd.h"
#include "SyntaxEdit/XTPSyntaxEditIIDs.h"
#include "SyntaxEdit/XTPSyntaxEditCtrl.h"
#include "SyntaxEdit/XTPSyntaxEditDrawTextProcessor.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_SYNTAXEDIT_AUTOCOMPLETE_HEIGHT_MAX 160
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPSyntaxEditAutoCompleteWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPSyntaxEditAutoCompleteWnd::CXTPSyntaxEditAutoCompleteWnd()
{
	RegisterWindowClass();

	m_strSearch			 = _T("");
	m_pParentWnd		 = NULL;
	m_nLineHeight		 = 0;
	m_nLines			 = 0;
	m_nWndHeight		 = XTP_DPI_Y(0);
	m_nWndWidth			 = XTP_DPI_X(160);
	m_nHighLightLine	 = -1;
	m_nFirstDisplayedStr = 0;
	m_nBordersHeight	 = 0;
	m_bActive			 = FALSE;
	m_bFixedBottom		 = FALSE;

	m_strDelims = _T("");

	m_pImageManager = new CXTPImageManager();

	CString type(_T("RT_XAML"));
	HMODULE hRes = XTPResourceManager()->GetResourceHandle();
	UINT nIconID = 0;
	m_pImageManager->SetVectorIcon(hRes, type, XTP_XAML_SMALL_ICON_NORMAL_CLASS, nIconID, 16);
	m_pImageManager->SetVectorIcon(hRes, type, XTP_XAML_LARGE_ICON_NORMAL_CLASS, nIconID, 32);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPSyntaxEditAutoCompleteWnd::~CXTPSyntaxEditAutoCompleteWnd()
{
	SAFE_DELETE(m_pImageManager);

	RemoveAll();
	DestroyWindow();
}

BOOL CXTPSyntaxEditAutoCompleteWnd::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	WNDCLASS wndcls;
	if (hInstance == NULL)
		hInstance = XTPGetInstanceHandle();

	if (!(::GetClassInfo(hInstance, XTP_EDIT_CLASSNAME_AUTOCOMPLETEWND, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style	   = CS_SAVEBITS | CS_DBLCLKS;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance					  = hInstance;
		wndcls.hIcon						  = NULL;
		wndcls.hCursor						  = ::LoadCursor(NULL, IDC_ARROW);
		wndcls.hbrBackground				  = (HBRUSH)(COLOR_WINDOW + 1);
		wndcls.lpszMenuName					  = NULL;
		wndcls.lpszClassName				  = XTP_EDIT_CLASSNAME_AUTOCOMPLETEWND;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CXTPSyntaxEditAutoCompleteWnd::Create(CWnd* pParentWnd)
{
	if (!CWnd::CreateEx(WS_EX_TOOLWINDOW, XTP_EDIT_CLASSNAME_AUTOCOMPLETEWND, NULL,
						WS_CHILD | WS_DLGFRAME | WS_VSCROLL | WS_DISABLED,
						CRect(0, 0, XTP_DPI_X(160), XTP_DPI_Y(160)), GetDesktopWindow(), NULL))
	{
		TRACE0("Failed to create Auto Complete View.\n");
		return FALSE;
	}

	CXTPClientRect rcClient(this);
	CXTPWindowRect rcWin(this);
	m_nBordersHeight = (rcWin.Height() - rcClient.Height());
	m_pParentWnd	 = DYNAMIC_DOWNCAST(CXTPSyntaxEditCtrl, pParentWnd);

	return TRUE;
}

void CXTPSyntaxEditAutoCompleteWnd::Show(CPoint pt, CString strSearch)
{
	if (Filter(strSearch) < 1)
		return;

	m_nFirstDisplayedStr = 0;
	int nSearch			 = Search(strSearch);
	m_nHighLightLine	 = nSearch;

	//<<>>Forum Proposal
	m_bHighLight = (m_nHighLightLine >= 0);
	//<<>>
	ScrollTo(nSearch);

	RefreshMetrics();

	AdjusLayout();

	CRect rcWnd(pt.x - XTP_DPI_X(21), pt.y, pt.x - XTP_DPI_X(21) + m_nWndWidth,
				pt.y + m_nWndHeight);

	if (GetParent())
		GetParent()->ClientToScreen(&rcWnd);

	_AdjustWndRect(rcWnd);

	if (GetParent())
		GetParent()->ScreenToClient(&rcWnd);

	// SetWindowPos(NULL, pt.x - 21, pt.y, m_nWndWidth, m_nWndHeight, SWP_SHOWWINDOW);
	SetWindowPos(NULL, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), SWP_SHOWWINDOW);

	BOOL bVScrollEnabled = AdjusLayout(rcWnd.Height());

	ShowScrollBar(SB_VERT, bVScrollEnabled);

	RedrawWindow();

	EnableWindow(TRUE);
	SetFocus();

	m_bActive		   = TRUE;
	int nSearchLen	 = strSearch.GetLength();
	m_bFilteredMode	= nSearchLen > 0;
	m_nStartReplacePos = m_pParentWnd->GetCurCol() - nSearchLen;
	m_nEndReplacePos   = m_nStartReplacePos + nSearchLen;
	m_strSearch		   = strSearch;
}

void CXTPSyntaxEditAutoCompleteWnd::_AdjustWndRect(CRect& rc)
{
	CRect rcWorkArea = XTPMultiMonitor()->GetWorkArea(); // from cursor point
	// CRect rcWorkArea = XTPMultiMonitor()->GetWorkArea(this);
	m_bFixedBottom = rc.bottom > rcWorkArea.bottom;

	if (m_bFixedBottom)
	{
		int nNewTop = rc.top - m_nWndHeight - m_pParentWnd->GetDrawTextProcessor().GetRowHeight();
		rc.top		= max(rcWorkArea.top, nNewTop);
		rc.bottom   = min(rcWorkArea.bottom, rc.top + m_nWndHeight);

		m_bFixedBottom = TRUE;
	}

	if (rc.right > rcWorkArea.right)
	{
		int nOffsetX = rc.right - rcWorkArea.right;
		rc.left		 = max(rcWorkArea.left, rc.left - nOffsetX);
		rc.right	 = rcWorkArea.right;
	}

	if (rc.left < rcWorkArea.left)
	{
		int nOffsetX = rcWorkArea.left - rc.left;
		rc.right	 = rc.right + nOffsetX;
		rc.left		 = rcWorkArea.left;
	}
}

void CXTPSyntaxEditAutoCompleteWnd::SetWndWidth(int nWidth)
{
	m_nWndWidth = nWidth;

	if (m_hWnd)
	{
		CXTPWindowRect rcWnd(this);
		if (GetParent())
			GetParent()->ScreenToClient(&rcWnd);
		rcWnd.right = rcWnd.left + m_nWndWidth;

		MoveWindow(&rcWnd);

		AdjusLayout(rcWnd.Height());

		RedrawWindow();
	}
}

void CXTPSyntaxEditAutoCompleteWnd::UpdateFilteredList()
{
	int nIdx = Search(m_strSearch);
	if (nIdx >= 0)
	{
		Filter(m_strSearch);

		m_nFirstDisplayedStr = 0;
		nIdx				 = Search(m_strSearch);

		m_nHighLightLine = nIdx;

		AdjusLayout();

		ScrollTo(nIdx);

		CXTPWindowRect rcWnd(this);
		CRect rcWorkArea = XTPMultiMonitor()->GetWorkArea(this);

		if (m_bFixedBottom)
			rcWnd.top = rcWnd.bottom - m_nWndHeight;
		else
			rcWnd.bottom = min(rcWorkArea.bottom, rcWnd.top + m_nWndHeight);

		if (GetParent())
			GetParent()->ScreenToClient(&rcWnd);

		MoveWindow(&rcWnd);

		AdjusLayout(rcWnd.Height());

		RedrawWindow();
	}
}

void CXTPSyntaxEditAutoCompleteWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	int nHitLine = HitTest(point);
	if (nHitLine > -1)
	{
		m_bHighLight	 = TRUE;
		m_nHighLightLine = m_nFirstDisplayedStr + nHitLine;
		RedrawWindow();
	}
}

void CXTPSyntaxEditAutoCompleteWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
	int nHitLine = HitTest(point);
	if (nHitLine > -1)
	{
		m_nHighLightLine = m_nFirstDisplayedStr + nHitLine;
		RedrawWindow();
	}
}

void CXTPSyntaxEditAutoCompleteWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
	m_bHighLight = TRUE;
	ReturnSelected(FALSE);
	Hide();
}

void CXTPSyntaxEditAutoCompleteWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nDataMax  = (int)m_arrACDataFiltered.GetSize();
	int nPage	 = m_nLines - 1;
	int nLastDisp = m_nFirstDisplayedStr + nPage;
	m_bHighLight  = TRUE;
	switch (nChar)
	{
		case VK_UP:
			m_nHighLightLine -= nRepCnt;

			if (m_nHighLightLine >= 0)
			{
				if (m_nFirstDisplayedStr > m_nHighLightLine && ScrollTo(m_nHighLightLine))
				{
					SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
				}

				RedrawWindow();
			}
			else
			{
				m_nHighLightLine = 0;
			}

			break;
		case VK_DOWN:
			m_nHighLightLine += nRepCnt;
			if (m_nHighLightLine < nDataMax)
			{
				int nL	= m_nLines - 1;
				nLastDisp = m_nFirstDisplayedStr + nL;
				if (nLastDisp < m_nHighLightLine)
				{
					if (ScrollTo(m_nHighLightLine - nL))
					{
						SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
					}
				}

				RedrawWindow();
			}
			else
			{
				m_nHighLightLine = nDataMax - 1;
			}
			break;
		case VK_PRIOR:
			if (m_nFirstDisplayedStr < m_nHighLightLine)
			{
				m_nHighLightLine = m_nFirstDisplayedStr;
			}
			else
			{
				if (m_nHighLightLine > 0)
					m_nHighLightLine = max(0, m_nHighLightLine - m_nLines + 1);
				else
					m_nHighLightLine -= m_nLines + 1;
			}

			if (m_nHighLightLine >= 0)
			{
				if (m_nFirstDisplayedStr > m_nHighLightLine && ScrollTo(m_nHighLightLine))
				{
					SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
				}

				RedrawWindow();
			}
			else
			{
				m_nHighLightLine = 0;
			}
			break;
		case VK_NEXT:

			if (m_nFirstDisplayedStr + nPage > m_nHighLightLine)
			{
				m_nHighLightLine = m_nFirstDisplayedStr + nPage;
			}
			else
			{
				if (m_nHighLightLine < nDataMax)
					m_nHighLightLine = min(nDataMax - 1, m_nHighLightLine + nPage);
				else
					m_nHighLightLine += nPage;
			}

			if (m_nHighLightLine < nDataMax)
			{
				if (nLastDisp < m_nHighLightLine)
				{
					if (ScrollTo(m_nHighLightLine - nPage))
					{
						SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
					}
				}

				RedrawWindow();
			}
			else
			{
				m_nHighLightLine = nDataMax - 1;
			}

			break;
		case VK_END:
			if (m_nHighLightLine != nDataMax - 1 && ScrollTo(nDataMax - m_nLines))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
				m_nHighLightLine = nDataMax - 1;
				RedrawWindow();
			}
			break;
		case VK_HOME:
			if (m_nHighLightLine != 0 && ScrollTo(0))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
				m_nHighLightLine = 0;
				RedrawWindow();
			}
			break;
		case VK_BACK:

			m_nEndReplacePos--;
			if (m_strSearch.GetLength() > 0)
				m_strSearch = m_strSearch.Left(m_strSearch.GetLength() - 1);

			if (m_nStartReplacePos >= m_nEndReplacePos)
			{
				Hide();
				m_pParentWnd->SendMessage(WM_LBUTTONUP, 0, 0);
			}
			else
			{
				UpdateFilteredList();
			}
			break;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_MENU:
		case VK_CONTROL:
		case VK_ESCAPE: Hide(); break;

		case VK_RETURN:
		case VK_SPACE:
			ReturnSelected(FALSE);
			Hide();
			break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditAutoCompleteWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (IsCloseTag((TCHAR)nChar))
	{
		ReturnSelected(FALSE);
		Hide();
	}

	if (isalnum((TCHAR)nChar) || (TCHAR)nChar == _T('_')) //<<>>
	{
		m_nEndReplacePos++;
		m_strSearch += (TCHAR)nChar;

		UpdateFilteredList();

		int nFound = Search(m_strSearch);
		if (nFound >= 0)
		{
			m_nHighLightLine = nFound;
			m_bHighLight	 = TRUE;
			if (m_nFirstDisplayedStr > m_nHighLightLine)
			{
				if (ScrollTo(m_nHighLightLine))
					SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
			}
			if (m_nFirstDisplayedStr + m_nLines <= m_nHighLightLine)
			{
				if (ScrollTo(m_nHighLightLine - m_nLines + 1))
					SetScrollPos(SB_VERT, m_nFirstDisplayedStr, TRUE);
			}
			RedrawWindow();
			CString sTxt = m_arrACDataFiltered.GetAt(m_nHighLightLine)->m_strText;
			if (m_bFilteredMode && sTxt.CompareNoCase(m_strSearch) == 0)
			{
				int nCnt = (int)m_arrACDataFiltered.GetSize();
				if (nCnt == 1)
				{
					ReturnSelected(TRUE);
					Hide();
				}
			}
		}
		else if (m_bFilteredMode)
		{
			Hide();
		}
		else
		{
			m_bHighLight = FALSE;
			RedrawWindow();
		}
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditAutoCompleteWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNREFERENCED_PARAMETER(nPos);
	UNREFERENCED_PARAMETER(pScrollBar);

	SCROLLINFO info = { 0 };
	GetScrollInfo(SB_VERT, &info);

	switch (nSBCode)
	{
		case SB_LINEDOWN:
			if (ScrollTo(m_nFirstDisplayedStr + 1))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr);
				RedrawWindow();
			}

			break;
		case SB_LINEUP:
			if (ScrollTo(m_nFirstDisplayedStr - 1))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr);
				RedrawWindow();
			}

			break;
		case SB_PAGEDOWN:
			if (ScrollTo(m_nFirstDisplayedStr + m_nLines - 1))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr);
				RedrawWindow();
			}
			break;
		case SB_PAGEUP:
			if (ScrollTo(m_nFirstDisplayedStr - m_nLines + 1))
			{
				SetScrollPos(SB_VERT, m_nFirstDisplayedStr);
				RedrawWindow();
			}
			break;
		case SB_TOP:
			SetScrollPos(SB_VERT, info.nMin);
			RedrawWindow();
			break;
		case SB_BOTTOM:
			SetScrollPos(SB_VERT, info.nMax);
			RedrawWindow();
			break;
		case SB_THUMBTRACK:
		{
			SCROLLINFO si = { 0 };
			si.cbSize	 = sizeof(SCROLLINFO);
			si.fMask	  = SIF_TRACKPOS;

			if (GetScrollInfo(SB_VERT, &si))
			{
				if (si.nTrackPos != m_nFirstDisplayedStr && ScrollTo(si.nTrackPos))
				{
					SetScrollPos(SB_VERT, si.nTrackPos);
					RedrawWindow();
				}
			}
		}
		break;
		case SB_ENDSCROLL: return;
		default: break;
	}
}

BOOL CXTPSyntaxEditAutoCompleteWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nCount = labs(zDelta) / WHEEL_DELTA;
	for (int i = 0; i < nCount; i++)
	{
		SendMessage(WM_VSCROLL, (WPARAM)(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN));
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CXTPSyntaxEditAutoCompleteWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(pWnd);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return TRUE;
}

BOOL CXTPSyntaxEditAutoCompleteWnd::AdjusLayout(int nHeightMax)
{
	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, &m_xtpFontBasic);

	int nDataLines = (int)m_arrACDataFiltered.GetSize();
	m_nLineHeight  = dc.GetTextExtent(_T("A"), 1).cy + XTP_DPI_Y(6);

	if (nHeightMax <= 0)
		nHeightMax = XTP_SYNTAXEDIT_AUTOCOMPLETE_HEIGHT_MAX;

	if (nDataLines * m_nLineHeight < nHeightMax)
	{
		m_nLines = nDataLines;
	}
	else
	{
		m_nLines = (int)nHeightMax / m_nLineHeight;
	}

	BOOL bVScrollEnabled = nDataLines > m_nLines;

	ShowScrollBar(SB_VERT, bVScrollEnabled);
	EnableScrollBarCtrl(SB_VERT, bVScrollEnabled);

	//--------------------------------------------------
	m_nWndHeight = m_nLines * m_nLineHeight + m_nBordersHeight;

	CXTPClientRect rcClient(this);

	CRect rc(0, 0, rcClient.Width(), m_nLineHeight);
	m_arrGrid.RemoveAll();

	for (int nLine = 0; nLine < m_nLines; nLine++)
	{
		m_arrGrid.Add(rc);
		rc.OffsetRect(0, m_nLineHeight);
	}

	//--------------------------------------------------
	if (bVScrollEnabled)
	{
		SCROLLINFO scrollInfo = { 0 };

		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask  = SIF_ALL;
		scrollInfo.nMin   = 0;
		scrollInfo.nMax   = max(0, nDataLines - 1);
		scrollInfo.nPage  = XTPToUIntChecked(m_nLines);
		scrollInfo.nPos   = m_nFirstDisplayedStr;

		SetScrollInfo(SB_VERT, &scrollInfo, FALSE);
	}

	return bVScrollEnabled;
}

void CXTPSyntaxEditAutoCompleteWnd::OnPaint()
{
	CPaintDC dc(this);
	CXTPFontDC fontDC(&dc, &m_xtpFontBasic);

	CRect rc;
	CRect rcText;
	CString str;

	int nData			 = 0;
	int nHighLightedLine = m_nHighLightLine - m_nFirstDisplayedStr;

	for (int nLine = 0; nLine < m_nLines; nLine++)
	{
		rc.CopyRect(m_arrGrid.GetAt(nLine));

		CPoint ptIcon(rc.left + XTP_DPI_X(1), rc.top + XTP_DPI_Y(1));

		rc.DeflateRect(XTP_DPI_X(19), 0, 0, 0);

		if (nLine == nHighLightedLine && m_bHighLight)
		{
			dc.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height(), m_clrHighLight);
			dc.SetTextColor(m_clrHighLightText);
		}
		else
		{
			dc.SetTextColor(m_clrWindowText);
			dc.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height(), m_clrWindow);
		}

		rcText.CopyRect(rc);
		rcText.DeflateRect(XTP_DPI_X(2), XTP_DPI_Y(3), XTP_DPI_X(2), XTP_DPI_Y(3));
		nData = m_nFirstDisplayedStr + nLine;

		CXTPImageManagerIcon* pIcon = m_pImageManager->GetImage(
			XTPToUInt(m_arrACDataFiltered.GetAt(nData)->m_nIcon), XTP_DPI_X(16));
		XTP_SAFE_CALL1(pIcon, Draw(&dc, ptIcon, XTP_DPI_SIZE(CSize(16, 16))));

		dc.TextOut(rcText.left, rcText.top, m_arrACDataFiltered.GetAt(nData)->m_strText);

		if (nLine == nHighLightedLine)
		{
			dc.DrawFocusRect(rc);
		}
	}
}

void CXTPSyntaxEditAutoCompleteWnd::RefreshMetrics()
{
	m_xtpFontBasic.CreateStockObject(DEFAULT_GUI_FONT);
	m_clrHighLight	 = GetSysColor(COLOR_HIGHLIGHT);
	m_clrWindow		   = GetSysColor(COLOR_WINDOW);
	m_clrWindowText	= GetSysColor(COLOR_WINDOWTEXT);
	m_clrHighLightText = GetSysColor(COLOR_HIGHLIGHTTEXT);
}

int CXTPSyntaxEditAutoCompleteWnd::HitTest(CPoint ptTest) const
{
	int nHitLine = -1;
	for (int nLine = 0; nLine < m_nLines; nLine++)
	{
		if (m_arrGrid.GetAt(nLine).PtInRect(ptTest))
		{
			nHitLine = nLine;
		}
	}

	return nHitLine;
}

void CXTPSyntaxEditAutoCompleteWnd::SetList(CXTPSyntaxEditACDataArray& parrData)
{
	int i, j;

	RemoveAll();
	m_arrACData.Copy(parrData);

	// Insert additional strings
	for (i = 0; i < m_InsertItems.GetSize(); i++)
	{
		XTP_EDIT_ACDATA* pData = m_InsertItems.GetAt(i);
		m_arrACData.Add(new XTP_EDIT_ACDATA(pData->m_nIcon, pData->m_strText));
	}

	// Remove strings
	for (i = 0; i < m_RemoveItems.GetSize(); i++)
	{
		for (j = 0; j < m_arrACData.GetSize(); j++)
		{
			if (m_RemoveItems.GetAt(i)->m_strText == m_arrACData.GetAt(j)->m_strText)
			{
				m_arrACData.RemoveAt(j);
			}
		}
	}

	Sort();
}

void CXTPSyntaxEditAutoCompleteWnd::SetCloseTags(CString strCloseTags)
{
	m_strCloseTags = strCloseTags;
}

BOOL CXTPSyntaxEditAutoCompleteWnd::IsCloseTag(CString strToTest)
{
	BOOL bTagFound = FALSE;
	if (strToTest == _T("~"))
		return bTagFound;

	m_strTmpCloseTag += strToTest;
	int nTmpLen = m_strTmpCloseTag.GetLength();
	int nFound  = -1;
	if ((nFound = m_strCloseTags.Find(m_strTmpCloseTag)) >= 0)
	{
		if (nFound + nTmpLen == m_strCloseTags.GetLength())
		{
			bTagFound = TRUE;
		}
		else
		{
			TCHAR ch = m_strCloseTags.GetAt(nFound + nTmpLen);
			if (ch == _T('~'))
			{
				bTagFound = TRUE;
			}
		}
	}
	else
	{
		m_strTmpCloseTag = _T("");
	}

	return bTagFound;
}

void CXTPSyntaxEditAutoCompleteWnd::SetOpenTags(CString strOpenTags)
{
	m_strOpenTags = strOpenTags;
}

BOOL CXTPSyntaxEditAutoCompleteWnd::IsOpenTag(CString strToTest)
{
	BOOL bTagFound = FALSE;
	if (strToTest == _T("~"))
		return bTagFound;

	m_strTmpOpenTag += strToTest;
	int nTmpLen = m_strTmpOpenTag.GetLength();
	int nFound  = -1;
	if ((nFound = m_strOpenTags.Find(m_strTmpOpenTag)) >= 0)
	{
		if (nFound + nTmpLen == m_strOpenTags.GetLength())
		{
			bTagFound = TRUE;
		}
		else
		{
			TCHAR ch = m_strOpenTags.GetAt(nFound + nTmpLen);
			if (ch == _T('~'))
			{
				bTagFound = TRUE;
			}
		}
	}
	else
	{
		m_strTmpOpenTag = _T("");
	}

	return bTagFound;
}

BOOL CXTPSyntaxEditAutoCompleteWnd::ScrollTo(int nNewLine)
{
	BOOL bScrollRequestCompleted = FALSE;

	if (nNewLine + m_nLines <= m_arrACDataFiltered.GetSize() && nNewLine >= 0)
	{
		bScrollRequestCompleted = TRUE;
		m_nFirstDisplayedStr	= nNewLine;
	}

	return bScrollRequestCompleted;
}

void CXTPSyntaxEditAutoCompleteWnd::ReturnSelected(BOOL bAdjust)
{
	if (m_nHighLightLine < 0 || !m_bHighLight)
		return;

	int nCurrentRow = m_pParentWnd->GetCurrentDocumentRow();

	CString strRet = m_arrACDataFiltered.GetAt(m_nHighLightLine)->m_strText;

	if (m_strTmpCloseTag.GetLength() > 0)
	{
		strRet += m_strTmpCloseTag;
		m_strTmpCloseTag = _T("");
	}
	else if (bAdjust)
	{
		strRet			 = strRet.Left(strRet.GetLength() - 1);
		m_nEndReplacePos = max(m_nStartReplacePos, m_nEndReplacePos - 1);
	}

	m_pParentWnd->Select(nCurrentRow, m_nStartReplacePos, nCurrentRow, m_nEndReplacePos, FALSE);
	m_pParentWnd->ReplaceSel(strRet);

	m_pParentWnd->SetCurCaretPos(m_pParentWnd->GetCurrentDocumentRow(), m_pParentWnd->m_nDispCol);
	m_pParentWnd->SetDocModified();
}

void CXTPSyntaxEditAutoCompleteWnd::Hide()
{
	if (!m_bActive)
		return;

	m_bActive = FALSE;
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
	m_bFilteredMode = FALSE;
	m_pParentWnd->PostMessage(WM_LBUTTONUP, 0, 0);
	RedrawWindow();
}

int CXTPSyntaxEditAutoCompleteWnd::CompareACData(const XTP_EDIT_ACDATA** p1,
												 const XTP_EDIT_ACDATA** p2)
{
	const XTP_EDIT_ACDATA* pData1 = *p1;
	const XTP_EDIT_ACDATA* pData2 = *p2;
	return _tcsicmp((LPCTSTR)pData1->m_strText, (LPCTSTR)pData2->m_strText);
}

void CXTPSyntaxEditAutoCompleteWnd::Sort()
{
	int nCount = (int)m_arrACData.GetSize();
	if (nCount > 1)
	{
		typedef int(AFX_CDECL * GENERICCOMPAREFUNC)(const void*, const void*);
		qsort(m_arrACData.GetData(), XTPToSizeTChecked(nCount), sizeof(XTP_EDIT_ACDATA*),
			  (GENERICCOMPAREFUNC)CompareACData);
	}
}

int CXTPSyntaxEditAutoCompleteWnd::CompareACDataToSearch(const XTP_EDIT_ACDATA** ppKey,
														 const XTP_EDIT_ACDATA** ppElem)
{
	int nKeyLength = (*ppKey)->m_strText.GetLength();
	return (*ppElem)->m_strText.Left(nKeyLength).CompareNoCase((*ppKey)->m_strText);
}

int CXTPSyntaxEditAutoCompleteWnd::Search(CString strSearch)
{
	int nFirstOccurrence = -1;
	if (strSearch.GetLength() == 0)
		return 0;

	XTP_EDIT_ACDATA* key = new XTP_EDIT_ACDATA(0, strSearch);

	XTP_EDIT_ACDATA** pFirst = &m_arrACDataFiltered[0];
	UINT nNum				 = (UINT)m_arrACDataFiltered.GetSize();
	typedef int(AFX_CDECL * GENERICCOMPAREFUNC)(const void*, const void*);

	XTP_EDIT_ACDATA** pRez = (XTP_EDIT_ACDATA**)_lfind(&key, pFirst, &nNum,
													   sizeof(XTP_EDIT_ACDATA*),
													   (GENERICCOMPAREFUNC)CompareACDataToSearch);

	delete key;

	if (pRez)
	{
		nFirstOccurrence = XTPToIntChecked(XTPToLongChecked((LONG_PTR)pRez - (LONG_PTR)pFirst)
										   / sizeof(XTP_EDIT_ACDATA*));
	}

	return nFirstOccurrence;
}

int CXTPSyntaxEditAutoCompleteWnd::Filter(CString strSearch)
{
	int nFound = 0;
	m_arrACDataFiltered.RemoveAll();
	if (strSearch.GetLength() == 0)
	{
		m_arrACDataFiltered.Copy(m_arrACData);
		return (int)m_arrACDataFiltered.GetSize();
	}

	if (!m_arrACData.GetSize())
	{
		return 0;
	}

	XTP_EDIT_ACDATA* key	 = new XTP_EDIT_ACDATA(0, strSearch);
	XTP_EDIT_ACDATA** pFirst = &m_arrACData[0];
	XTP_EDIT_ACDATA** pNext  = NULL;
	UINT nNum				 = (UINT)m_arrACData.GetSize();
	typedef int(AFX_CDECL * GENERICCOMPAREFUNC)(const void*, const void*);
	while (nNum > 0)
	{
		pNext = (XTP_EDIT_ACDATA**)_lfind(&key, pFirst, &nNum, sizeof(XTP_EDIT_ACDATA*),
										  (GENERICCOMPAREFUNC)CompareACDataToSearch);

		if (pNext)
		{
			nNum -= (LONG)((LONG_PTR)pNext - (LONG_PTR)pFirst) / sizeof(XTP_EDIT_ACDATA*);
			pFirst = pNext;
			nNum--;
			pFirst++;
			m_arrACDataFiltered.Add(*pNext);
			nFound++;
		}
		else
		{
			nNum--;
			pFirst++;
		}
	}

	delete key;

	return nFound;
}

void CXTPSyntaxEditAutoCompleteWnd::RemoveAll()
{
	int nDataLines = (int)m_arrACData.GetSize();
	m_arrACDataFiltered.RemoveAll();
	XTP_EDIT_ACDATA* pACData = NULL;
	for (int nDataLine = 0; nDataLine < nDataLines; nDataLine++)
	{
		pACData = m_arrACData.GetAt(nDataLine);
		ASSERT(pACData);
		delete pACData;
	}
	m_arrACData.RemoveAll();
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPSyntaxEditAutoCompleteWnd, CWnd)
	INTERFACE_PART(CXTPSyntaxEditAutoCompleteWnd, XTPDIID_SyntaxEditAutoCompleteWnd, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSyntaxEditAutoCompleteWnd, XTPDIID_SyntaxEditAutoCompleteWnd)

BEGIN_DISPATCH_MAP(CXTPSyntaxEditAutoCompleteWnd, CWnd)

	DISP_PROPERTY_EX_ID(CXTPSyntaxEditAutoCompleteWnd, "Icons", 1, OleGetIcons, OleSetIcons,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPSyntaxEditAutoCompleteWnd, "InsertItem", 2, OleInsertItem, VT_EMPTY,
					 VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPSyntaxEditAutoCompleteWnd, "RemoveItem", 3, OleRemoveItem, VT_EMPTY,
					 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPSyntaxEditAutoCompleteWnd, "IsExistItem", 4, OleIsExistItem, VT_BOOL,
					 VTS_BSTR)
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditAutoCompleteWnd, "Delimiters", 5, OleGetDelimiters,
						OleSetDelimiters, VT_BSTR)

END_DISPATCH_MAP()

LPDISPATCH CXTPSyntaxEditAutoCompleteWnd::OleGetIcons()
{
	return m_pImageManager->GetIDispatch(TRUE);
}

void CXTPSyntaxEditAutoCompleteWnd::OleSetIcons(LPDISPATCH lpDispatch)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = NULL;

	if (NULL != lpDispatch)
	{
		m_pImageManager = CXTPImageManager::FromDispatch(lpDispatch);
		lpDispatch->AddRef();
	}
}

void CXTPSyntaxEditAutoCompleteWnd::OleInsertItem(int iconID, const LPCTSTR str)
{
	InsertItem(iconID, str);
}

void CXTPSyntaxEditAutoCompleteWnd::OleRemoveItem(const LPCTSTR str)
{
	RemoveItem(str);
}

bool CXTPSyntaxEditAutoCompleteWnd::OleIsExistItem(const LPCTSTR str)
{
	for (int i = 0; i < m_arrACData.GetSize(); i++)
	{
		if (str == m_arrACData.GetAt(i)->m_strText)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPSyntaxEditAutoCompleteWnd::OleSetDelimiters(LPCTSTR pcszDelimeters)
{
	CString s(pcszDelimeters);
	SetDelimiters(pcszDelimeters);
}

BSTR CXTPSyntaxEditAutoCompleteWnd::OleGetDelimiters()
{
	CString strDelimiters = GetDelimiters();
	return strDelimiters.AllocSysString();
}

#endif