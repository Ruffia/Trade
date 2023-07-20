// XTPFontComboBox.cpp : implementation of the CXTPFontComboBox class.
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

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"

#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPVC80Helpers.h" // Visual Studio 2005 helper functions
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"

#include "Controls/Resource.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Util/XTPGlobal.h"
#include "Controls/ListBox/XTPListBox.h"
#include "Controls/Combo/XTPComboBox.h"
#include "Controls/Combo/XTPComboBoxTheme.h"
#include "Controls/Combo/XTPFontComboBox.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CXTPFontEnum
//////////////////////////////////////////////////////////////////////

CXTPFontEnum::CXTPFontEnum()
	: m_pDC(NULL)
{
	Init();
}

CXTPFontEnum::~CXTPFontEnum()
{
}

CXTPFontEnum& CXTPFontEnum::Get()
{
	return CXTPSingleton<CXTPFontEnum>::Instance();
}

bool CXTPFontEnum::DoesFontExist(CString& strFaceName)
{
	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);
		if (strFaceName.CompareNoCase(lf.lfFaceName) == 0)
		{
			strFaceName = lf.lfFaceName; // adjust case if necessary
			return true;
		}
	}
	return false;
}

CXTPLogFont* CXTPFontEnum::GetLogFont(const CString& strFaceName)
{
	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);
		if (strFaceName == lf.lfFaceName)
		{
			return &lf;
		}
	}
	return NULL;
}

bool CXTPFontEnum::AddFont(const LOGFONT* pLF, DWORD dwType)
{
	// See if the font name has already been added.
	CString strFaceName = pLF->lfFaceName;
	strFaceName.Remove('@');

	if (DoesFontExist(strFaceName) != 0)
	{
		return false;
	}

	// Save the CXTPLogFont struct.
	CXTPLogFont lf;
	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTPLogFont));
	lf.dwType   = dwType;
	lf.lfWidth  = 0;
	lf.lfHeight = CXTPFontCtrlBase::GetFontItemHeight();
	m_listFonts.AddTail(lf);

	return true;
}

BOOL CALLBACK CXTPFontEnum::EnumFontFamExProc(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/,
											  DWORD dwFontType, LPARAM lParam)
{
	CXTPFontEnum* pFontEnum = (CXTPFontEnum*)lParam;
	if (pFontEnum != NULL)
	{
		if (pFontEnum->AddFont(&pelf->elfLogFont, dwFontType))
		{
			// TODO:
		}
	}

	return TRUE;
}

int CXTPFontEnum::GetMaxWidth()
{
	CWindowDC dc(NULL);
	CFont font;
	CFont* p = dc.SelectObject(CXTPFontCtrlBase::CreateUIFont(font) ? &font
																	: &XTPAuxData().xtpFont);

	int iMaxWidth = 0;

	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);

		int nWidth = dc.GetTextExtent(lf.lfFaceName).cx;
		iMaxWidth  = __max(iMaxWidth, nWidth);
	}

	dc.SelectObject(p);
	font.DeleteObject();

	return iMaxWidth;
}

int CXTPFontEnum::GetMaxWidth(LPCTSTR lpString)
{
	ASSERT(NULL != lpString);

	CWindowDC dc(NULL);

	int nLength   = static_cast<int>(_tcslen(lpString));
	int iMaxWidth = 0;

	POSITION pos;
	for (pos = m_listFonts.GetHeadPosition(); pos; m_listFonts.GetNext(pos))
	{
		CXTPLogFont& lf = m_listFonts.GetAt(pos);
		CXTPFont font;
		if (font.CreateFontIndirect(&lf))
		{
			CXTPFontDC fontDC(&dc, &font);
			int nWidth = dc.GetTextExtent(lpString, nLength).cx;
			iMaxWidth  = __max(iMaxWidth, nWidth);
		}
	}

	return iMaxWidth;
}

void CXTPFontEnum::Init(CDC* pDC /*= NULL*/, BYTE nCharSet /*= DEFAULT_CHARSET*/)
{
	m_listFonts.RemoveAll();

	// Enumerate all styles of all fonts for the ANSI character set
	CXTPLogFont lf;
	lf.lfFaceName[0] = '\0';
	lf.lfCharSet	 = nCharSet;

	if (pDC == NULL || pDC->m_hDC == NULL)
	{
		CWindowDC dc(NULL);
		::EnumFontFamiliesEx(dc.m_hDC, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
	}
	else
	{
		::EnumFontFamiliesEx(pDC->m_hDC, &lf, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0);
	}

	m_pDC = pDC;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFontCtrlBase
/////////////////////////////////////////////////////////////////////////////

CXTPFontCtrlBase::CXTPFontCtrlBase()
	: m_dwStyle(xtpFontBoth)
	, m_csSymbol(_T("AaBbCc"))
	, m_pListBox(NULL)
	, m_pComboBox(NULL)
{
	XTPImageManager()->SetVectorIcon(_T("RT_XAML"), IDR_XAML_ICON_FONTCOMBO_FONTS, XTP_IDB_FONTTYPE,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
}

CXTPFontCtrlBase::~CXTPFontCtrlBase()
{
	XTPImageManager()->RemoveIcon(XTP_IDB_FONTTYPE);
}

void CXTPFontCtrlBase::OnInit(CXTPFontListBox* pListBox)
{
	m_pListBox = pListBox;
}

void CXTPFontCtrlBase::OnInit(CXTPFontComboBox* pComboBox)
{
	m_pComboBox = pComboBox;
}

int AFX_CDECL CXTPFontCtrlBase::GetFontItemHeight()
{
	return ::GetSystemMetrics(SM_CYVTHUMB) - (::GetSystemMetrics(SM_CYEDGE) / 2);
}

BOOL AFX_CDECL CXTPFontCtrlBase::CreateUIFont(CFont& font)
{
	LOGFONT lf;

	if (CXTPDrawHelpers::GetDefaultLogFont(&lf))
	{
		return font.CreateFontIndirect(&lf);
	}

	return FALSE;
}

void CXTPFontCtrlBase::OnDrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC	   = CDC::FromHandle(lpDIS->hDC);
	UINT itemState = lpDIS->itemState;
	UINT itemID	= lpDIS->itemID;
	CRect rcItem   = lpDIS->rcItem;

	if (itemID == (UINT)-1)
	{
		return;
	}

	CXTPLogFont* pLF = (CXTPLogFont*)lpDIS->itemData;
	if (pLF == NULL)
	{
		return;
	}

	BOOL bDisabled = ((itemState & ODS_DISABLED) == ODS_DISABLED);
	BOOL bSelected = ((itemState & ODS_SELECTED) == ODS_SELECTED);
	// 	BOOL bFocus    = ((itemState & ODS_FOCUS)    == ODS_FOCUS);

	COLORREF clrBack;
	COLORREF clrText;

	CXTPComboBoxTheme* pTheme = m_pComboBox ? m_pComboBox->GetTheme() : NULL;

	if (pTheme)
	{
		clrBack = bDisabled ? pTheme->m_clrBackDisabled
							: bSelected ? pTheme->m_clrBackHighlight : pTheme->m_clrBackground;
		clrText = bDisabled ? pTheme->m_clrTextDisabled
							: bSelected ? pTheme->m_clrTextHighlight : pTheme->m_clrText;
	}
	else
	{
		clrBack = bDisabled
					  ? GetXtremeColor(COLOR_3DFACE)
					  : bSelected ? GetXtremeColor(COLOR_HIGHLIGHT) : GetXtremeColor(COLOR_WINDOW);
		clrText = bDisabled ? GetXtremeColor(COLOR_GRAYTEXT)
							: bSelected ? GetXtremeColor(COLOR_HIGHLIGHTTEXT)
										: GetXtremeColor(COLOR_WINDOWTEXT);
	}

	pDC->SetTextColor(clrText);
	pDC->SetBkColor(clrBack);
	pDC->FillSolidRect(&rcItem, clrBack);

	//	if (bFocus)
	//	{
	//		pDC->DrawFocusRect(&rcItem);
	//	}

	CString csFaceName = pLF->lfFaceName;

	// true type or device flag set by SetItemData
	if (pLF->dwType & TRUETYPE_FONTTYPE)
	{
		// calculate the size of the TT bitmap.
		CRect rType(rcItem);
		rType.top	= ((rcItem.top + rcItem.bottom) / 2) - (XTP_DPI_Y(FontTypeWidth) / 2);
		rType.right  = rType.left + XTP_DPI_X(FontTypeWidth);
		rType.bottom = rType.top + XTP_DPI_Y(FontTypeWidth);

		// draw the TT symbol and replace the dark color with the theme text color.

		CXTPImageManagerIcon* pIcon = XTPImageManager()->GetImage(XTP_IDB_FONTTYPE, rType.Width());
		ASSERT_VALID(pIcon);

		if (pIcon->IsVectorIcon())
		{
			pIcon->Draw(pDC, rType.TopLeft(),
						CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON)));
		}

		CXTPDrawHelpers::ReplaceColor(pDC, rType, RGB(0, 0, 0), clrText);
	}

	// Define the size of the text area to draw
	CRect rcText(rcItem);
	rcText.left += XTP_DPI_X(FontTypeWidth) + XTP_DPI_X(6);

	pDC->SetBkMode(TRANSPARENT);

	if (m_dwStyle & xtpFontGUI)
	{
		CFont font;
		CFont* pFont = CreateUIFont(font) ? &font : &XTPAuxData().xtpFont;
		{
			CXTPFontDC fontDC(pDC, pFont);
			pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
		}
		font.DeleteObject();
	}

	if (m_dwStyle & xtpFontSample)
	{
		if (m_dwStyle & xtpFontGUI)
			rcText.left = rcText.left + CXTPFontEnum::Get().GetMaxWidth() + XTP_DPI_X(5);

		if ((m_dwStyle & xtpFontGUI) || (pLF->lfCharSet == SYMBOL_CHARSET))
			csFaceName = m_csSymbol;

		LOGFONT lfPreviewFont  = *pLF;
		lfPreviewFont.lfHeight = rcItem.Height();

		// Draw the text item.
		CXTPFont fntPreview;
		if (fntPreview.CreateFontIndirect(0 != lfPreviewFont.lfHeight ? &lfPreviewFont : pLF))
		{
			CXTPFontDC fontDC(pDC, &fntPreview);
			pDC->DrawText(csFaceName, rcText, DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void CXTPFontCtrlBase::OnMeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	if (m_pComboBox)
		ASSERT(lpMIS->CtlType == ODT_COMBOBOX);

	else if (m_pListBox)
		ASSERT(lpMIS->CtlType == ODT_LISTBOX);

	lpMIS->itemHeight = XTPToUIntChecked(GetFontItemHeight());
	lpMIS->itemWidth  = 0;
}

int CXTPFontCtrlBase::OnCompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT(m_pComboBox || m_pListBox);
	ASSERT(lpCIS->CtlType == ODT_LISTBOX);

	int iItem1 = (int)(WORD)lpCIS->itemID1;
	if (iItem1 == -1)
	{
		return -1;
	}

	CString strItem1;
	if (m_pComboBox)
	{
		m_pComboBox->GetLBText(iItem1, strItem1);
	}
	else if (m_pListBox)
	{
		m_pListBox->GetText(iItem1, strItem1);
	}

	int iItem2 = (int)(WORD)lpCIS->itemID2;
	if (iItem2 == -1)
	{
		return -1;
	}

	CString strItem2;
	if (m_pComboBox)
	{
		m_pComboBox->GetLBText(iItem2, strItem2);
	}
	else if (m_pListBox)
	{
		m_pListBox->GetText(iItem2, strItem2);
	}

	return strItem1.Collate(strItem2);
}

BOOL CXTPFontCtrlBase::GetSelFont(CXTPLogFont& lf)
{
	int iCurSel = LB_ERR;

	if (m_pListBox)
		iCurSel = m_pListBox->GetCurSel();

	if (m_pComboBox)
	{
		iCurSel = m_pComboBox->GetCurSel();

		if (iCurSel == CB_ERR)
		{
			CString strWindowText;
			m_pComboBox->GetWindowText(strWindowText);
			iCurSel = m_pComboBox->SelectString(-1, strWindowText);
		}
	}

	if (iCurSel == LB_ERR)
		return FALSE;

	CXTPLogFont* pLF = NULL;

	if (m_pListBox)
		pLF = (CXTPLogFont*)m_pListBox->GetItemDataPtr(iCurSel);

	if (m_pComboBox)
		pLF = (CXTPLogFont*)m_pComboBox->GetItemDataPtr(iCurSel);

	if (pLF == NULL)
		return FALSE;

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTPLogFont));

	return TRUE;
}

BOOL CXTPFontCtrlBase::GetSelFont(CString& strFaceName)
{
	CXTPLogFont lf;

	if (GetSelFont(lf))
	{
		strFaceName = lf.lfFaceName;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPFontCtrlBase::SetSelFont(CXTPLogFont& lf)
{
	return SetSelFont(lf.lfFaceName);
}

BOOL CXTPFontCtrlBase::SetSelFont(const CString& strFaceName)
{
	int iCount = 0;

	if (m_pListBox)
		iCount = m_pListBox->GetCount();

	if (m_pComboBox)
		iCount = m_pComboBox->GetCount();

	int i;
	for (i = 0; i < iCount; ++i)
	{
		CXTPLogFont* pLF = NULL;

		if (m_pListBox)
			pLF = (CXTPLogFont*)m_pListBox->GetItemDataPtr(i);

		if (m_pComboBox)
			pLF = (CXTPLogFont*)m_pComboBox->GetItemDataPtr(i);

		if (pLF != NULL)
		{
			if (strFaceName.CompareNoCase(pLF->lfFaceName) == 0)
			{
				if (m_pListBox)
					return (m_pListBox->SetCurSel(i) != LB_ERR);

				if (m_pComboBox)
					return (m_pComboBox->SetCurSel(i) != CB_ERR);
			}
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFontListBox
/////////////////////////////////////////////////////////////////////////////

CXTPFontListBox::CXTPFontListBox()
{
}

CXTPFontListBox::~CXTPFontListBox()
{
}

BEGIN_MESSAGE_MAP(CXTPFontListBox, CXTPListBox)
	//{{AFX_MSG_MAP(CXTPFontListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPFontListBox message handlers

void CXTPFontListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CXTPFontCtrlBase::OnDrawItem(lpDIS);
}

void CXTPFontListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	CXTPFontCtrlBase::OnMeasureItem(lpMIS);
}

int CXTPFontListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	return CXTPFontCtrlBase::OnCompareItem(lpCIS);
}

void CXTPFontListBox::Initialize(bool /*bAutoFont = true*/)
{
	CXTPListBox::Initialize(false);
	CXTPFontCtrlBase::OnInit(this);

	CXTPFontList& fontList = CXTPFontEnum::Get().GetFontList();

	POSITION pos;
	for (pos = fontList.GetHeadPosition(); pos; fontList.GetNext(pos))
	{
		CXTPLogFont& lf = fontList.GetAt(pos);
		int iIndex		= AddString(lf.lfFaceName);

		SetItemDataPtr(iIndex, &lf);
	}
}

///////////////////////////////////////////////////////////////////////////
// CXTPFontComboBox
///////////////////////////////////////////////////////////////////////////

CXTPFontComboBox::CXTPFontComboBox()
	: m_bIsOpen(FALSE)
{
}

CXTPFontComboBox::~CXTPFontComboBox()
{
}

IMPLEMENT_DYNAMIC(CXTPFontComboBox, CXTPComboBox)

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPFontComboBox, CXTPComboBox)
	//{{AFX_MSG_MAP(CXTPFontComboBox)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropDown)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CXTPFontCtrlBase::OnDrawItem(lpDIS);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFontComboBox diagnostics

#ifdef _DEBUG
void CXTPFontComboBox::AssertValid() const
{
	CXTPComboBox::AssertValid();

	DWORD dwStyle = XTPToULong(::GetWindowLong(m_hWnd, GWL_STYLE));

	// combo box is owner drawn, and must be created with the
	// following styles:

	ASSERT(dwStyle & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE));
	ASSERT(dwStyle & CBS_DROPDOWN);
	ASSERT(dwStyle & CBS_SORT);
	ASSERT(dwStyle & CBS_HASSTRINGS);
}
#endif

void CXTPFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	CXTPFontCtrlBase::OnMeasureItem(lpMIS);
}

int CXTPFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	return CXTPFontCtrlBase::OnCompareItem(lpCIS);
}

void CXTPFontComboBox::InitControl(LPCTSTR lpszFaceName, UINT nWidth /*= 0*/,
								   BOOL bEnable /*= TRUE*/)
{
	CXTPFontCtrlBase::OnInit(this);

	ASSERT(this);
	ResetContent();

	// MFCBUG: adjust height so display is the same as non-owner drawn
	// CComboBoxes. MFC adjusts the height of an owner-drawn CComboBox
	// 2-3 pixels larger than a non owner-drawn combo.

	int nHeight = GetFontItemHeight();
	SetItemHeight(-1, XTPToUIntChecked(nHeight));
	SetItemHeight(0, XTPToUIntChecked(nHeight));

	if (nWidth != 0)
	{
		SetDroppedWidth(nWidth);
	}
	else
	{
		SetDroppedWidth(XTPToUIntChecked(XTP_DPI_X(FontTypeWidth) + XTP_DPI_X(6)
										 + CXTPFontEnum::Get().GetMaxWidth() + XTP_DPI_X(5)
										 + CXTPFontEnum::Get().GetMaxWidth(m_csSymbol)));
	}

	// moved from constructor so resource is loaded when this gets called

	XTPImageManager()->SetVectorIcon(_T("RT_XAML"), IDR_XAML_ICON_FONTCOMBO_FONTS, XTP_IDB_FONTTYPE,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));

	EnableAutoCompletion(bEnable);

	CXTPFontList& fontList = CXTPFontEnum::Get().GetFontList();

	POSITION pos;
	for (pos = fontList.GetHeadPosition(); pos; fontList.GetNext(pos))
	{
		CXTPLogFont& lf = fontList.GetAt(pos);
		int iIndex		= AddString(lf.lfFaceName);

		SetItemDataPtr(iIndex, &lf);
	}

	if (lpszFaceName && _tcslen(lpszFaceName))
	{
		SetSelFont(lpszFaceName);
	}
}

BOOL CXTPFontComboBox::OnDropDown()
{
	m_bIsOpen = TRUE;
	GetSelFont(m_csSelected);
	Invalidate();
	return FALSE; // continue routing.
}

BOOL CXTPFontComboBox::OnCloseUp()
{
	m_bIsOpen = FALSE;
	return FALSE; // continue routing.
}

void CXTPFontComboBox::NotifyOwner(UINT nCode)
{
	CWnd* pWndOwner = GetOwner();
	if (::IsWindow(pWndOwner->GetSafeHwnd()))
	{
		pWndOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), nCode), (LPARAM)m_hWnd);
	}
}

BOOL CXTPFontComboBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					SetSelFont(m_csSelected);
					NotifyOwner(CBN_SELENDCANCEL);
				}
				return TRUE;
			}

			case VK_SPACE:
			case VK_RETURN:
			{
				if (GetDroppedState() == TRUE)
				{
					ShowDropDown(FALSE);
					NotifyOwner(CBN_SELENDOK);
				}
				return TRUE;
			}

			case VK_UP:
			case VK_DOWN:
			{
				if (GetDroppedState() == FALSE)
				{
					ShowDropDown(TRUE);
					return TRUE;
				}
				break;
			}
		}
	}

	return CXTPComboBox::PreTranslateMessage(pMsg);
}
