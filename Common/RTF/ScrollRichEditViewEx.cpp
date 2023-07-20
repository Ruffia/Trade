// ScrollRichEditViewEx.cpp
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
#include "ScrollRichEditViewEx.h"
#include "RichEditCommands.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollRichEditViewEx

CScrollRichEditViewEx::CScrollRichEditViewEx()
	: m_hInstance(NULL)
{
	m_nTextHeight  = 0;
	m_nPageHeight  = 1000;
	m_nTotalHeight = 0;
	m_nScrollPos   = 0;

	m_bInScrollChanged = FALSE;

	WNDCLASS wc;
	if (XTPLoadWindowClass(xtpWindowClassRichEdit, xtpWindowClassVersionHighest,
						   xtpRichEditWindowClassDefault, &wc))
	{
		m_hInstance = wc.hInstance;
		m_strClass  = wc.lpszClassName;
	}

	m_clr	 = COLOR_WINDOWTEXT;
	m_clrBack = RGB(255, 255, 255);
}

CScrollRichEditViewEx::~CScrollRichEditViewEx()
{
}

IMPLEMENT_DYNCREATE(CScrollRichEditViewEx, CRichEditView)

BEGIN_MESSAGE_MAP(CScrollRichEditViewEx, CRichEditView)
	//{{AFX_MSG_MAP(CScrollRichEditViewEx)
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()

	//	HOME / Font
	ON_COMMAND(ID_FONT_BOLD, OnCharBold)
	ON_COMMAND(ID_FONT_ITALIC, OnCharItalic)
	ON_COMMAND(ID_FONT_UNDERLINE, OnCharUnderline)
	ON_COMMAND(ID_FONT_STRIKETHROUGH, OnCharStrikeout)
	ON_COMMAND(ID_FONT_SUBSCRIPT, OnCharSubscript)
	ON_COMMAND(ID_FONT_SUPERSCRIPT, OnCharSuperscript)
	ON_COMMAND(ID_FONT_INCREASE, OnFontGrow)
	ON_COMMAND(ID_FONT_DECREASE, OnFontShrink)
	ON_COMMAND(ID_FONT_EFFECTS, OnEmptyCommand)
	ON_COMMAND(ID_FONT_CHANGECASE, OnEmptyCommand)
	ON_COMMAND(ID_FONT_CLEAR, OnEmptyCommand)
	ON_XTP_EXECUTE(ID_FONT_FACE, OnEditFontFace)
	ON_XTP_EXECUTE(ID_FONT_SIZE, OnEditFontSize)
	ON_UPDATE_COMMAND_UI(ID_FONT_BOLD, OnUpdateCharBold)
	ON_UPDATE_COMMAND_UI(ID_FONT_UNDERLINE, OnUpdateCharUnderline)
	ON_UPDATE_COMMAND_UI(ID_FONT_ITALIC, OnUpdateCharItalic)
	ON_UPDATE_COMMAND_UI(ID_FONT_STRIKETHROUGH, OnUpdateCharStrikeout)
	ON_UPDATE_COMMAND_UI(ID_FONT_SUBSCRIPT, OnUpdateCharSubscript)
	ON_UPDATE_COMMAND_UI(ID_FONT_SUPERSCRIPT, OnUpdateCharSuperscript)
	ON_UPDATE_COMMAND_UI(ID_FONT_INCREASE, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_FONT_DECREASE, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_FONT_FACE, OnUpdateComboFontFace)
	ON_UPDATE_COMMAND_UI(ID_FONT_FACE_GALLERY, OnUpdateGalleryFontFace)
	ON_UPDATE_COMMAND_UI(ID_FONT_SIZE, OnUpdateComboFontSize)
	ON_UPDATE_COMMAND_UI(ID_FONT_SIZE_GALLERY, OnUpdateGalleryFontSize)

	ON_COMMAND(ID_FONT_HIGHLIGHTCOLOR, OnHighlightColor)
	ON_UPDATE_COMMAND_UI(ID_FONT_HIGHLIGHTCOLOR, OnUpdateHighlightColor)
	ON_XTP_EXECUTE(ID_GALLERY_FONTBACKCOLOR, OnGalleryHighlightColor)
	ON_UPDATE_COMMAND_UI(ID_GALLERY_FONTBACKCOLOR, OnUpdateGalleryHighlightColor)
	ON_COMMAND(ID_BACKCOLOR_NOCOLORS, OnGalleryNoHighlightColors)
	ON_UPDATE_COMMAND_UI(ID_BACKCOLOR_NOCOLORS, OnUpdateGalleryNoHighlightColors)

	ON_COMMAND(ID_FONT_COLOR, OnTextColor)
	ON_UPDATE_COMMAND_UI(ID_FONT_COLOR, OnUpdateTextColor)
	ON_XTP_EXECUTE(ID_FONT_COLOR_GALLERY, OnGalleryTextColor)
	ON_UPDATE_COMMAND_UI(ID_FONT_COLOR_GALLERY, OnUpdateGalleryTextColor)
	ON_COMMAND(XTP_IDS_AUTOMATIC, OnGalleryTextAutoColor)
	ON_UPDATE_COMMAND_UI(XTP_IDS_AUTOMATIC, OnUpdateGalleryTextAutoColor)

	//	HOME / Paragraph
	ON_COMMAND(ID_PARAGRAPH_BULLET, OnBullet)
	ON_COMMAND(ID_PARAGRAPH_NUMBERING, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_LIST, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_DECREASEINDENT, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_INCREASEINDENT, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_SORT, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_SHOWMARKS, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_LEFT, OnParaLeft)
	ON_COMMAND(ID_PARAGRAPH_CENTER, OnParaCenter)
	ON_COMMAND(ID_PARAGRAPH_RIGHT, OnParaRight)
	ON_COMMAND(ID_PARAGRAPH_JUSTIFY, OnParaJustify)
	ON_COMMAND(ID_PARAGRAPH_LINESPACING, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_SHADING, OnEmptyCommand)
	ON_COMMAND(ID_PARAGRAPH_NOBORDER, OnEmptyCommand)

	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_BULLET, OnUpdateBullet)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_NUMBERING, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_LIST, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_DECREASEINDENT, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_INCREASEINDENT, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_SORT, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_SHOWMARKS, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_LEFT, OnUpdateParaLeft)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_CENTER, OnUpdateParaCenter)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_RIGHT, OnUpdateParaRight)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_JUSTIFY, OnUpdateParaJustify)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_LINESPACING, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_SHADING, OnUpdateEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_NOBORDER, OnUpdateEmptyCommand)

	// PAGE LAYOUT / Paragraph
	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_INDENTLEFT, OnUpdateIndentLeft)
	ON_XTP_EXECUTE(ID_PARAGRAPH_INDENTLEFT, OnIndentLeft)
	ON_NOTIFY(XTP_FN_SPINUP, ID_PARAGRAPH_INDENTLEFT, OnIndentLeftSpin)
	ON_NOTIFY(XTP_FN_SPINDOWN, ID_PARAGRAPH_INDENTLEFT, OnIndentLeftSpin)

	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_INDENTRIGHT, OnUpdateIndentRight)
	ON_XTP_EXECUTE(ID_PARAGRAPH_INDENTRIGHT, OnIndentRight)
	ON_NOTIFY(XTP_FN_SPINUP, ID_PARAGRAPH_INDENTRIGHT, OnIndentRightSpin)
	ON_NOTIFY(XTP_FN_SPINDOWN, ID_PARAGRAPH_INDENTRIGHT, OnIndentRightSpin)

	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_SPACINGBEFORE, OnUpdateSpaceBefore)
	ON_XTP_EXECUTE(ID_PARAGRAPH_SPACINGBEFORE, OnSpaceBefore)
	ON_NOTIFY(XTP_FN_SPINUP, ID_PARAGRAPH_SPACINGBEFORE, OnSpaceBeforeSpin)
	ON_NOTIFY(XTP_FN_SPINDOWN, ID_PARAGRAPH_SPACINGBEFORE, OnSpaceBeforeSpin)

	ON_UPDATE_COMMAND_UI(ID_PARAGRAPH_SPACINGAFTER, OnUpdateSpaceAfter)
	ON_XTP_EXECUTE(ID_PARAGRAPH_SPACINGAFTER, OnSpaceAfter)
	ON_NOTIFY(XTP_FN_SPINUP, ID_PARAGRAPH_SPACINGAFTER, OnSpaceAfterSpin)
	ON_NOTIFY(XTP_FN_SPINDOWN, ID_PARAGRAPH_SPACINGAFTER, OnSpaceAfterSpin)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CScrollRichEditViewEx message handlers

#ifndef EM_GETSCROLLPOS
#	define EM_GETSCROLLPOS (WM_USER + 221)
#endif

#ifndef EM_SETSCROLLPOS
#	define EM_SETSCROLLPOS (WM_USER + 222)
#endif

#ifndef EM_SHOWSCROLLBAR
#	define EM_SHOWSCROLLBAR (WM_USER + 96)
#endif

void CScrollRichEditViewEx::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	m_nScrollPos = 0;

	SendMessage(EM_SHOWSCROLLBAR, SB_VERT, FALSE);
	SendMessage(EM_SHOWSCROLLBAR, SB_HORZ, FALSE);

	GetRichEditCtrl().SetEventMask(GetRichEditCtrl().GetEventMask() | ENM_SCROLL
								   | ENM_REQUESTRESIZE);

	UpdateScrollInfo();
}

void CScrollRichEditViewEx::OnDestroy()
{
	// Force release the internal OLE callback in order to avoid a memory leak.
	GetRichEditCtrl().SetOLECallback(NULL);

	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CRichEditView::OnDestroy();
}

void CScrollRichEditViewEx::OnEmptyCommand(UINT)
{
	REPORT_NOT_IMPLEMENTED();
}

void CScrollRichEditViewEx::OnEmptyCommand()
{
	REPORT_NOT_IMPLEMENTED();
}

void CScrollRichEditViewEx::OnUpdateEmptyCommand(CCmdUI* pCmdUI)
{
}

void CScrollRichEditViewEx::OnCharStrikeout()
{
	OnCharEffect(CFM_STRIKEOUT, CFE_STRIKEOUT);
}

void CScrollRichEditViewEx::OnCharSubscript()
{
#if (_MSC_VER < 1300)
	CHARFORMAT cf;
#else
	CHARFORMAT2 cf;
#endif

	cf = GetCharFormatSelection();
	if ((cf.dwEffects & CFE_SUBSCRIPT) == CFE_SUBSCRIPT)
	{
		cf.dwEffects -= CFE_SUBSCRIPT;
	}
	else
	{
		if ((cf.dwEffects & CFE_SUPERSCRIPT) == CFE_SUPERSCRIPT)
		{
			cf.dwEffects -= CFE_SUPERSCRIPT;
		}
		cf.dwEffects |= CFE_SUBSCRIPT;
	}

	SetCharFormat(cf);
}

void CScrollRichEditViewEx::OnCharSuperscript()
{
#if (_MSC_VER < 1300)
	CHARFORMAT cf;
#else
	CHARFORMAT2 cf;
#endif

	cf = GetCharFormatSelection();
	if ((cf.dwEffects & CFE_SUPERSCRIPT) == CFE_SUPERSCRIPT)
	{
		cf.dwEffects -= CFE_SUPERSCRIPT;
	}
	else
	{
		if ((cf.dwEffects & CFE_SUBSCRIPT) == CFE_SUBSCRIPT)
		{
			cf.dwEffects -= CFE_SUBSCRIPT;
		}

		cf.dwEffects |= CFE_SUPERSCRIPT;
	}

	SetCharFormat(cf);
}

void CScrollRichEditViewEx::OnFontGrow()
{
#if (_MSC_VER < 1300)
	CHARFORMAT cf;
#else
	CHARFORMAT2 cf;
#endif

	cf		  = GetCharFormatSelection();
	cf.dwMask = CFM_SIZE;
	cf.yHeight += 10;

	SetCharFormat(cf);
}

void CScrollRichEditViewEx::OnFontShrink()
{
#if (_MSC_VER < 1300)
	CHARFORMAT cf;
#else
	CHARFORMAT2 cf;
#endif

	cf		  = GetCharFormatSelection();
	cf.dwMask = CFM_SIZE;
	cf.yHeight -= 10;

	SetCharFormat(cf);
}

void CScrollRichEditViewEx::OnUpdateCharStrikeout(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_STRIKEOUT, CFE_STRIKEOUT);
}

void CScrollRichEditViewEx::OnUpdateCharSubscript(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_SUBSCRIPT, CFE_SUBSCRIPT);
}

void CScrollRichEditViewEx::OnUpdateCharSuperscript(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_SUPERSCRIPT, CFE_SUPERSCRIPT);
}

void CScrollRichEditViewEx::OnEditFontFace(NMHDR* pNMHDR, LRESULT* pResult)
{
	USES_CONVERSION;
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = DYNAMIC_DOWNCAST(CXTPControlComboBox, tagNMCONTROL->pControl);
	if (pControl && pControl->GetType() == xtpControlComboBox)
	{
		CString strFont = pControl->GetEditText();

#if (_MSC_VER < 1300)
		CHARFORMAT cf;
#else
		CHARFORMAT2 cf;
#endif
		ZeroMemory(&cf, sizeof(cf));
		cf.dwMask = CFM_FACE;

#if (_RICHEDIT_VER >= 0x0200)
		lstrcpyn(cf.szFaceName, strFont, LF_FACESIZE);
#else
		lstrcpynA(cf.szFaceName, T2A((LPTSTR)(LPCTSTR)strFont), LF_FACESIZE);
#endif
		SetCharFormat(cf);

		*pResult = 1; // Handled;
	}
}

void CScrollRichEditViewEx::OnUpdateComboFontFace(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);

	CXTPControlComboBox* pFontCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox,
													   CXTPControl::FromUI(pCmd));
	if (pFontCombo && pFontCombo->GetType() == xtpControlComboBox)
	{
		CHARFORMAT& cf = GetCharFormatSelection();

		if (pFontCombo->HasFocus())
			return;

		// the selection must be same font and charset to display correctly
		if ((cf.dwMask & (CFM_FACE | CFM_CHARSET)) == (CFM_FACE | CFM_CHARSET))
			pFontCombo->SetEditText(CString(cf.szFaceName));
		else
			pFontCombo->SetEditText(_T(""));
	}
}

void CScrollRichEditViewEx::OnUpdateGalleryFontFace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnEditFontSize(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = DYNAMIC_DOWNCAST(CXTPControlComboBox, tagNMCONTROL->pControl);
	if (pControl && pControl->GetType() == xtpControlComboBox)
	{
#if (_MSC_VER < 1300)
		CHARFORMAT cf;
#else
		CHARFORMAT2 cf;
#endif

		ZeroMemory(&cf, sizeof(cf));
		int nSize = CXTPControlSizeComboBox::GetTwipSize(pControl->GetEditText());
		if (nSize > 0)
		{
			cf.dwMask  = CFM_SIZE;
			cf.yHeight = nSize;

			SetCharFormat(cf);
		}

		*pResult = 1; // Handled;
	}
}

void CScrollRichEditViewEx::OnUpdateComboFontSize(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);

	CXTPControlComboBox* pFontCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox,
													   CXTPControl::FromUI(pCmd));
	if (pFontCombo && pFontCombo->GetType() == xtpControlComboBox)
	{
		CHARFORMAT& cf = GetCharFormatSelection();

		if (pFontCombo->HasFocus())
			return;

		int nTwip = (cf.dwMask & CFM_SIZE) ? cf.yHeight : -1;

		pFontCombo->SetEditText(CXTPControlSizeComboBox::TwipsToPointString(nTwip));
	}
}

void CScrollRichEditViewEx::OnUpdateGalleryFontSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

//	Highlight color

void CScrollRichEditViewEx::OnHighlightColor()
{
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cf);

	cf.dwMask |= CFM_BACKCOLOR;
	cf.dwEffects &= ~CFE_AUTOBACKCOLOR;
	cf.crBackColor = m_clrBack;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

void CScrollRichEditViewEx::OnUpdateHighlightColor(CCmdUI* pCmdUI)
{
	CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor,
													 CXTPControl::FromUI(pCmdUI));
	if (pPopup)
	{
		pPopup->SetColor(m_clrBack == COLORREF_NULL ? RGB(0xff, 0xff, 0xff) : m_clrBack);
	}

	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnGalleryHighlightColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pControl = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);
	if (pControl)
	{
		CXTPControlGalleryItem* pItem = pControl->GetItem(pControl->GetSelectedItem());
		if (pItem)
		{
			m_clrBack = (COLORREF)pItem->GetID();
			OnHighlightColor();
		}

		*pResult = TRUE; // Handled
	}
}

void CScrollRichEditViewEx::OnUpdateGalleryHighlightColor(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pControl = DYNAMIC_DOWNCAST(CXTPControlGallery,
													CXTPControl::FromUI(pCmdUI));
	if (pControl)
	{
		pControl->SetCheckedItem(m_clrBack);
	}

	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnGalleryNoHighlightColors()
{
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_BACKCOLOR;
	cf.dwEffects |= CFE_AUTOBACKCOLOR;
	cf.crBackColor = m_clrBack;
	m_clrBack	  = COLORREF_NULL;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

void CScrollRichEditViewEx::OnUpdateGalleryNoHighlightColors(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_clrBack == COLORREF_NULL ? 1 : 0);
	pCmdUI->Enable(TRUE);
}

//	Text color

void CScrollRichEditViewEx::OnTextColor()
{
	CHARFORMAT& cfm = GetCharFormatSelection();
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects &= ~CFE_AUTOCOLOR;
	cfm.crTextColor = m_clr;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CScrollRichEditViewEx::OnUpdateTextColor(CCmdUI* pCmd)
{
	CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor,
													 CXTPControl::FromUI(pCmd));
	if (pPopup)
	{
		pPopup->SetColor(m_clr);
	}

	pCmd->Enable(TRUE);
}

void CScrollRichEditViewEx::OnGalleryTextColor(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlGallery* pControl = DYNAMIC_DOWNCAST(CXTPControlGallery, tagNMCONTROL->pControl);
	if (pControl)
	{
		CXTPControlGalleryItem* pItem = pControl->GetItem(pControl->GetSelectedItem());
		if (pItem)
		{
			m_clr = (COLORREF)pItem->GetID();
			OnTextColor();
		}

		*pResult = TRUE; // Handled
	}

	*pResult = 1;
}

void CScrollRichEditViewEx::OnUpdateGalleryTextColor(CCmdUI* pCmdUI)
{
	CXTPControlGallery* pControl = DYNAMIC_DOWNCAST(CXTPControlGallery,
													CXTPControl::FromUI(pCmdUI));
	if (pControl)
	{
		pControl->SetCheckedItem(m_clr);
	}

	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnGalleryTextAutoColor()
{
	CHARFORMAT& cfm = GetCharFormatSelection();
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects |= CFE_AUTOCOLOR;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CScrollRichEditViewEx::OnUpdateGalleryTextAutoColor(CCmdUI* pCmd)
{
	CHARFORMAT& cfm = GetCharFormatSelection();
	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOCOLOR ? TRUE : FALSE);
}

void CScrollRichEditViewEx::OnParaJustify()
{
	OnParaAlign(0x4);
}

void CScrollRichEditViewEx::OnUpdateParaJustify(CCmdUI* pCmdUI)
{
	OnUpdateParaAlign(pCmdUI, 0x4);
}

CString FormatIndent(int nIndent)
{
	CString strIndent;

	if (nIndent % 1000 == 0)
		strIndent.Format(_T("%0.0f\""), (double)nIndent / 1000);
	else if (nIndent % 100 == 0)
		strIndent.Format(_T("%0.1f\""), (double)nIndent / 1000);
	else
		strIndent.Format(_T("%0.2f\""), (double)nIndent / 1000);

	return strIndent;
}

double StringToDouble(LPCTSTR lpszStr)
{
	USES_CONVERSION;
	return atof(T2A((LPTSTR)lpszStr));
}

void CScrollRichEditViewEx::OnUpdateIndentLeft(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));
	if (pControl && !pControl->HasFocus())
	{
		pControl->SetEditText(FormatIndent(GetIndentLeft()));
	}
	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnIndentLeft(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControlEdit* pControl  = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetIndentLeft(int(StringToDouble(pControl->GetEditText()) * 1000));
		*pResult = TRUE;
	}
}

void CScrollRichEditViewEx::OnIndentLeftSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetIndentLeft(int(StringToDouble(pControl->GetEditText()) * 1000)
					  + tagNMCONTROL->iDelta * 100);

		pControl->SetEditText(FormatIndent(GetIndentLeft()));
	}
	*pResult = 1;
}

void CScrollRichEditViewEx::SetIndentLeft(long nIndentLeft)
{
	if (nIndentLeft < -11000)
		nIndentLeft = -11000;
	if (nIndentLeft > 22000)
		nIndentLeft = 22000;

	PARAFORMAT pf;
	pf.cbSize		 = sizeof(PARAFORMAT);
	pf.dwMask		 = PFM_STARTINDENT;
	pf.dxStartIndent = nIndentLeft;

	::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

long CScrollRichEditViewEx::GetIndentLeft()
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_STARTINDENT;

	::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	return pf.dxStartIndent;
}

void CScrollRichEditViewEx::OnUpdateIndentRight(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));
	if (pControl && !pControl->HasFocus())
	{
		pControl->SetEditText(FormatIndent(GetIndentRight()));
	}
	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnIndentRight(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControlEdit* pControl  = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetIndentRight(int(StringToDouble(pControl->GetEditText()) * 1000));
		*pResult = TRUE;
	}
}

void CScrollRichEditViewEx::OnIndentRightSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetIndentRight(int(StringToDouble(pControl->GetEditText()) * 1000)
					   + tagNMCONTROL->iDelta * 100);

		pControl->SetEditText(FormatIndent(GetIndentRight()));
	}
	*pResult = 1;
}

void CScrollRichEditViewEx::SetIndentRight(long nIndentRight)
{
	if (nIndentRight < -11000)
		nIndentRight = -11000;
	if (nIndentRight > 22000)
		nIndentRight = 22000;

	PARAFORMAT pf;
	pf.cbSize		 = sizeof(PARAFORMAT);
	pf.dwMask		 = PFM_RIGHTINDENT;
	pf.dxRightIndent = nIndentRight;

	::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

long CScrollRichEditViewEx::GetIndentRight()
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_RIGHTINDENT;

	::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	return pf.dxRightIndent;
}

#define SPACE_SIZE 20

CString FormatSpacing(int nIndent)
{
	CString strSpacing;
	strSpacing.Format(_T("%i pt"), (int)(nIndent / SPACE_SIZE));
	return strSpacing;
}

void CScrollRichEditViewEx::OnUpdateSpaceBefore(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));
	if (pControl && !pControl->HasFocus())
	{
		pControl->SetEditText(FormatSpacing(GetSpaceBefore()));
	}

	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnSpaceBefore(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControlEdit* pControl  = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetSpaceBefore(_ttoi(pControl->GetEditText()) * SPACE_SIZE);
		*pResult = TRUE;
	}
}

void CScrollRichEditViewEx::OnSpaceBeforeSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetSpaceBefore(int(_ttoi(pControl->GetEditText()) * SPACE_SIZE)
					   + tagNMCONTROL->iDelta * 6 * SPACE_SIZE);

		pControl->SetEditText(FormatSpacing(GetSpaceBefore()));
	}

	*pResult = 1;
}

void CScrollRichEditViewEx::SetSpaceBefore(long nSpaceBefore)
{
	if (nSpaceBefore < 0)
		nSpaceBefore = 0;

	PARAFORMAT2 pf;
	pf.cbSize		 = sizeof(PARAFORMAT2);
	pf.dwMask		 = PFM_SPACEBEFORE;
	pf.dySpaceBefore = nSpaceBefore;

	::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

long CScrollRichEditViewEx::GetSpaceBefore()
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_SPACEBEFORE;

	::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	return pf.dySpaceBefore;
}

void CScrollRichEditViewEx::OnUpdateSpaceAfter(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));
	if (pControl && !pControl->HasFocus())
	{
		pControl->SetEditText(FormatSpacing(GetSpaceAfter()));
	}

	pCmdUI->Enable(TRUE);
}

void CScrollRichEditViewEx::OnSpaceAfter(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControlEdit* pControl  = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetSpaceAfter(int(_ttoi(pControl->GetEditText()) * SPACE_SIZE));
		*pResult = TRUE;
	}
}

void CScrollRichEditViewEx::OnSpaceAfterSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPUPDOWN* tagNMCONTROL = (NMXTPUPDOWN*)pNMHDR;
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, tagNMCONTROL->pControl);

	if (pControl)
	{
		SetSpaceAfter(int(_ttoi(pControl->GetEditText()) * SPACE_SIZE)
					  + tagNMCONTROL->iDelta * 6 * SPACE_SIZE);

		pControl->SetEditText(FormatSpacing(GetSpaceAfter()));
	}

	*pResult = 1;
}

void CScrollRichEditViewEx::SetSpaceAfter(long nSpaceAfter)
{
	if (nSpaceAfter < 0)
		nSpaceAfter = 0;

	PARAFORMAT2 pf;
	pf.cbSize		= sizeof(PARAFORMAT2);
	pf.dwMask		= PFM_SPACEAFTER;
	pf.dySpaceAfter = nSpaceAfter;

	::SendMessage(m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
}

long CScrollRichEditViewEx::GetSpaceAfter()
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof(PARAFORMAT2);
	pf.dwMask = PFM_SPACEAFTER;

	::SendMessage(m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	return pf.dySpaceAfter;
}
CScrollBar* CScrollRichEditViewEx::GetParentScrollbar(int nBar) const
{
	return NULL;
}

COLORREF CScrollRichEditViewEx::GetFrameColor()
{
	return RGB(255, 255, 255);
}

CRect CScrollRichEditViewEx::GetParentViewRect()
{
	return CRect();
}

CScrollBar* CScrollRichEditViewEx::GetScrollBarCtrl(int nBar) const
{
	ASSERT(nBar == SB_VERT);
	UNUSED(nBar);

	if (!AfxGetMainWnd())
		return NULL;
	return GetParentScrollbar(nBar);
}

void CScrollRichEditViewEx::UpdateScrollInfo()
{
	m_nTotalHeight = max(m_nPageHeight, m_nTextHeight + 40 + 10);

	CScrollBar* pScrollBar = GetScrollBarCtrl(SB_VERT);
	if (!pScrollBar)
		return;

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_ALL;
	si.nPos   = m_nScrollPos;

	si.nPage = GetPageSize();
	si.nMax  = m_nTotalHeight;

	pScrollBar->SetScrollInfo(&si);
}

void CScrollRichEditViewEx::OnEditScrollChanged()
{
	if (m_bInScrollChanged)
		return;

	POINT pt;
	GetRichEditCtrl().SendMessage(EM_GETSCROLLPOS, 0, (LPARAM)&pt);

	m_nScrollPos = 40 + pt.y;

	UpdateScrollInfo();
	UpdatePosition();
}

void CScrollRichEditViewEx::OnRequestResize(NMHDR* pNotifyStruct, LRESULT* /*result*/)
{
	REQRESIZE* rs = (REQRESIZE*)pNotifyStruct;

	m_nTextHeight = rs->rc.bottom - rs->rc.top - 3;

	UpdateScrollInfo();
}

void CScrollRichEditViewEx::UpdatePosition()
{
	CRect rc = GetParentViewRect();
	MoveWindow(rc, TRUE);

	SendMessage(WM_NCPAINT);
}

void CScrollRichEditViewEx::SetScrollPos(int nScrollPos)
{
	int nMaxPos = m_nTotalHeight - GetPageSize() + 1;

	if (nScrollPos > nMaxPos)
		nScrollPos = nMaxPos;

	if (nScrollPos < 0)
		nScrollPos = 0;

	if (m_nScrollPos == nScrollPos)
		return;

	m_nScrollPos = nScrollPos;

	int nEditScroll = max(0, m_nScrollPos - 40);

	m_bInScrollChanged = TRUE;
	POINT pt		   = { 0, nEditScroll };
	SendMessage(EM_SETSCROLLPOS, 0, (LPARAM)&pt);
	m_bInScrollChanged = FALSE;

	UpdateScrollInfo();
	UpdatePosition();
}

int CScrollRichEditViewEx::GetPageSize()
{
	return CXTPClientRect(GetParent()).Height() - 50;
}

BOOL CScrollRichEditViewEx::PreCreateWindow(CREATESTRUCT& cs)
{
	return CRichEditView::PreCreateWindow(cs);
	cs.lpszClass = m_strClass;
	cs.hInstance = m_hInstance;
}

void CScrollRichEditViewEx::OnVScroll(UINT nScrollCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	int y = GetScrollPos(SB_VERT);

	switch (nScrollCode)
	{
		case SB_TOP: y = 0; break;
		case SB_BOTTOM: y = m_nTotalHeight; break;
		case SB_LINEUP: y -= 20; break;
		case SB_LINEDOWN: y += 20; break;
		case SB_PAGEUP: y -= GetPageSize(); break;
		case SB_PAGEDOWN: y += GetPageSize(); break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION: y = nPos; break;
	}

	SetScrollPos(y);
}

void CScrollRichEditViewEx::OnSize(UINT nType, int cx, int cy)
{
	CRichEditView::OnSize(nType, cx, cy);

	UpdateScrollInfo();
}

void CScrollRichEditViewEx::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	// TODO: Add your specialized code here and/or call the base class

	CRichEditView::CalcWindowRect(lpClientRect, nAdjustType);

	int nWidth = 820;

	int nClientWidth = lpClientRect->right - lpClientRect->left;
	int nBorders	 = max(8, nClientWidth - nWidth);

	int nTopMargin = max(4, 40 - m_nScrollPos);

	int nBottomMargin = m_nScrollPos + GetPageSize() - m_nTotalHeight + 10;
	nBottomMargin	 = max(1, nBottomMargin);

	lpClientRect->top += nTopMargin;
	lpClientRect->left += nBorders / 2;
	lpClientRect->right -= nBorders / 2;
	lpClientRect->bottom -= nBottomMargin;
}

void CScrollRichEditViewEx::OnNcPaint()
{
	CWindowDC dc(this);
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	int nBottomMargin = m_nScrollPos + GetPageSize() - m_nTotalHeight + 10;

	COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);

	COLORREF clrFrame = GetFrameColor();

	dc.FillSolidRect(1, 0, rc.Width() - 2, 1, m_nScrollPos < 40 ? clrFrame : clrWindow);
	dc.FillSolidRect(0, 0, 1, rc.Height(), clrFrame);
	dc.FillSolidRect(rc.Width() - 1, 0, 1, rc.Height(), clrFrame);
	dc.FillSolidRect(1, rc.Height() - 1, rc.Width() - 2, 1,
					 nBottomMargin > 0 ? clrFrame : clrWindow);

#if 0
	dc.FillSolidRect(1, 1, rc.Width() - 2, 2, clrWindow);
	dc.FillSolidRect(1, 1, 2, rc.Height() - 2, clrWindow);
	dc.FillSolidRect(rc.Width() - 3, 1, 2, rc.Height() - 2, clrWindow);
	dc.FillSolidRect(1, rc.Height() - 3, rc.Width() - 2, 2, clrWindow);
#endif
}

BOOL CScrollRichEditViewEx::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	SetScrollPos(m_nScrollPos + (zDelta < 0 ? 60 : -60));

	return TRUE;
}
