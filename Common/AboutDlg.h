// AboutDlg.h
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

#if !defined(__ABOUTDLG_H__)
#	define __ABOUTDLG_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	if (_MSC_VER > 1310) // VS2005
#		if defined _M_IX86
#			pragma comment(                                                                       \
				linker,                                                                            \
				"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		elif defined _M_IA64
#			pragma comment(                                                                       \
				linker,                                                                            \
				"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		elif defined _M_X64
#			pragma comment(                                                                       \
				linker,                                                                            \
				"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		else
#			pragma comment(                                                                       \
				linker,                                                                            \
				"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#		endif
#	endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog
#	include "Resource.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL)
		: CDialog(IDD_ABOUTBOX, pParent)
	{
		CXTPNonClientMetrics ncm;

		ncm.lfMenuFont.lfWeight = FW_BOLD;
		m_fontBold.CreateFontIndirect(&ncm.lfMenuFont);
	}

	void SetCredits(LPCTSTR lpszInfo, LPCTSTR lpszURL)
	{
		m_strCreditsInfo = lpszInfo;
		m_strCreditsURL  = lpszURL;
	}

	void SetAxialisCredits()
	{
		SetCredits(_T("CommandBars icons is a courtesy of Axilalis SA"),
				   _T("http://www.axialis.com"));
	}

	//{{AFX_DATA(CAboutDlg)
	CStatic m_txtPackageVersion;
	CStatic m_txtCopyrightInfo;
	CStatic m_txtAppName;
	CStatic m_txtCreditsInfo;
	CStatic m_txtCreditsWeb;
	CStatic m_txtBorder;
	CStatic m_bmpCodejock;
#	ifdef __XTPBUTTON_H__
	CXTPButton m_btnOk;
	CXTPHyperLink m_txtURL;
	CXTPHyperLink m_txtEmail;
	CXTPHyperLink m_txtCreditsURL;
#	else
	CButton m_btnOk;
	CStatic m_txtCreditsURL;
#	endif
	//}}AFX_DATA

	CFont m_fontBold;
	CString m_strCreditsInfo;
	CString m_strCreditsURL;

	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CAboutDlg)
		DDX_Control(pDX, IDC_TXT_TITLE, m_txtPackageVersion);
		DDX_Control(pDX, IDC_TXT_COPYRIGHT, m_txtCopyrightInfo);
		DDX_Control(pDX, IDC_TXT_APPNAME, m_txtAppName);
		DDX_Control(pDX, IDC_TXT_CREDITS_INFO, m_txtCreditsInfo);
		DDX_Control(pDX, IDC_TXT_CREDITS_WEB, m_txtCreditsWeb);
		DDX_Control(pDX, IDC_TXT_CREDITS_URL, m_txtCreditsURL);
		DDX_Control(pDX, IDC_STATIC_BORDER, m_txtBorder);
		DDX_Control(pDX, IDC_STATIC_BMP_CODEJOCK, m_bmpCodejock);
		DDX_Control(pDX, IDOK, m_btnOk);
#	ifdef __XTPBUTTON_H__
		DDX_Control(pDX, IDC_TXT_URL, m_txtURL);
		DDX_Control(pDX, IDC_TXT_EMAIL, m_txtEmail);
#	endif
		//}}AFX_DATA_MAP
	}

	void MoveControl(CWnd& wndCtrl, int y, BOOL bResize = FALSE)
	{
		CRect r;
		wndCtrl.GetWindowRect(&r);

		if (bResize)
		{
			r.bottom += y;
		}
		else
		{
			r.OffsetRect(0, y);
		}

		ScreenToClient(&r);
		wndCtrl.MoveWindow(&r);
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		CBitmap bitmap, bitmapScaled;
		bitmap.LoadBitmap(IDC_BMP_CODEJOCK);
		BOOL bRet = FALSE;
		CDC srcDC, destDC;
		if (srcDC.CreateCompatibleDC(NULL) && destDC.CreateCompatibleDC(NULL))
		{
			CBitmap* pSrcBitmap = srcDC.SelectObject(&bitmap);
			BITMAP bmpInfo;
			if (bitmap.GetBitmap(&bmpInfo))
			{
				CRect r;
				m_bmpCodejock.GetClientRect(&r);
				int width  = XTP_DPI_X(int(bmpInfo.bmWidth));
				int height = XTP_DPI_Y(int(bmpInfo.bmHeight));
				if (width > r.Width())
				{
					width	= r.Width();
					double d = ((double)width) / ((double)bmpInfo.bmWidth);
					d *= ((double)bmpInfo.bmHeight);
					d += 0.5;
					height = (int)d;
				}

				if (width != bmpInfo.bmWidth || height != bmpInfo.bmHeight)
				{
					if (bitmapScaled.CreateCompatibleBitmap(&srcDC, width, height))
					{
						CBitmap* pDstBitmap = destDC.SelectObject(&bitmapScaled);
						if (destDC.StretchBlt(0, 0, width, height, &srcDC, 0, 0, bmpInfo.bmWidth,
											  bmpInfo.bmHeight, SRCCOPY))
							bRet = TRUE;

						destDC.SelectObject(pDstBitmap);
					}
				}
			}

			srcDC.SelectObject(pSrcBitmap);
		}

		if (bRet)
		{
			m_bmpCodejock.SetBitmap((HBITMAP)bitmapScaled.Detach());
		}
		else
		{
			m_bmpCodejock.SetBitmap((HBITMAP)bitmap.Detach());
		}

		// set credits if enabled.
		if (!m_strCreditsInfo.IsEmpty() && !m_strCreditsURL.IsEmpty())
		{
			// make credits visible.
			m_txtCreditsInfo.ModifyStyle(0L, WS_VISIBLE);
			m_txtCreditsURL.ModifyStyle(0L, WS_VISIBLE);
			m_txtCreditsWeb.ModifyStyle(0L, WS_VISIBLE);

			// update credits text.
			m_txtCreditsInfo.SetWindowText(m_strCreditsInfo);
			m_txtCreditsURL.SetWindowText(m_strCreditsURL);
#	ifdef __XTPBUTTON_H__
			m_txtCreditsURL.SetURL(m_strCreditsURL);
			m_txtCreditsURL.SetUnderline(false);
#	endif

			// resize dialog to display credits.
			CRect r;
			GetWindowRect(&r);
			r.bottom += XTP_DPI_Y(47);
			MoveWindow(&r);

			// resize border.
			MoveControl(m_txtBorder, XTP_DPI_Y(47), TRUE);

			// move ok button.
			MoveControl(m_btnOk, XTP_DPI_Y(47));

			// move credits text.
			MoveControl(m_txtCreditsInfo, XTP_DPI_Y(-47));
			MoveControl(m_txtCreditsWeb, XTP_DPI_Y(-47));
			MoveControl(m_txtCreditsURL, XTP_DPI_Y(-47));
		}

		// get a pointer to CWinApp.
		CWinApp* pApp = AfxGetApp();
		ASSERT(pApp != NULL);

		// set the sample title.
		CString csSampleTitle;
		csSampleTitle.Format(_T("%s Application"), pApp->m_pszAppName);
		m_txtAppName.SetWindowText(csSampleTitle);

		// set the package version.
		CString csPackageVersion;
		csPackageVersion.Format(_T( "%s v%d.%d.%d" ), _XTP_PACKAGE_NAME, _XTPLIB_VERSION_MAJOR,
								_XTPLIB_VERSION_MINOR, _XTPLIB_VERSION_REVISION);
		m_txtPackageVersion.SetWindowText(csPackageVersion);

		// set the about caption.
		CString csAboutCaption;
		csAboutCaption.Format(_T("About %s" ), pApp->m_pszAppName);
		SetWindowText(csAboutCaption);

		// set the copyright info.
		CString csCopyrightInfo;
		csCopyrightInfo.Format(_T( "%s\nAll Rights Reserved" ), _XTP_COPYRIGHT_INFO);
		m_txtCopyrightInfo.SetWindowText(csCopyrightInfo);

		// set the title text to bold font.
		m_txtPackageVersion.SetFont(&m_fontBold);

#	ifdef __XTPBUTTON_H__

		// define the url for our hyperlinks.
		m_txtURL.SetURL(_T( "http://www.codejock.com" ));
		m_txtURL.SetUnderline(false);

		m_txtEmail.SetURL(_T( "mailto:sales@codejock.com" ));
		m_txtEmail.SetUnderline(false);

		// set OK button style.
		m_btnOk.SetTheme(xtpControlThemeOffice2000);
		m_btnOk.SetUseVisualStyle(TRUE);
#	endif

		return TRUE; // return TRUE unless you set the focus to a control
					 // EXCEPTION: OCX Property Pages should return FALSE
	}
};

#	ifdef ENABLE_MANIFESTEDITOR

#		if !defined(_UNICODE) && (_MSC_VER < 1400)

#			if _MSC_VER > 1200					// MFC 7.0
#				include <..\src\mfc\afximpl.h> // MFC Global data
#			else
#				include <..\src\afximpl.h> // MFC Global data
#			endif

#		endif

AFX_INLINE void EnableManifestEditor()
{
#		if !defined(_UNICODE) && (_MSC_VER < 1400)
	OSVERSIONINFO ovi = { sizeof(OSVERSIONINFO) };
	::GetVersionEx(&ovi);

	if ((ovi.dwPlatformId >= VER_PLATFORM_WIN32_NT) && (ovi.dwMajorVersion >= 5))
	{
#			if (_MSC_VER >= 1200)
		afxData.bWin95 = TRUE;
#			else
		afxData.bWin32s = TRUE;
#			endif
	}
#		endif
}

#	else

AFX_INLINE void EnableManifestEditor()
{
}

#	endif

AFX_INLINE void ShowSampleHelpPopup(CWnd* pParentWnd, UINT nIDResource)
{
#	ifdef __XTPBUTTON_H__
	CXTPPopupControl* pPopup = new CXTPPopupControl();
	pPopup->SetTransparency(200);
	pPopup->SetTheme(xtpPopupThemeOffice2003);
	pPopup->AllowMove(TRUE);
	pPopup->SetAnimateDelay(500);
	pPopup->SetPopupAnimation();
	pPopup->SetShowDelay(5000);
	pPopup->SetAutoDelete(TRUE);

	CXTPPopupItem* pItemText = (CXTPPopupItem*)pPopup->AddItem(
		new CXTPPopupItem(CRect(8, 12, 500, 130)));
	pItemText->SetRTFText(nIDResource);
	pItemText->FitToContent();
	CSize sz(pItemText->GetRect().Size());

	// close icon.
	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)pPopup->AddItem(
		new CXTPPopupItem(CRect(sz.cx + 2, 10, sz.cx + 2 + 16, 10 + 16)));

	pPopup->GetImageManager()->SetIcon(IDI_POPUP_CLOSE, IDI_POPUP_CLOSE);
	pItemIcon->SetIconIndex(IDI_POPUP_CLOSE);

	pItemIcon->SetButton(TRUE);
	pItemIcon->SetID(XTP_ID_POPUP_CLOSE);

	pPopup->SetPopupSize(CSize(sz.cx + 20, sz.cy + 20));
	pPopup->Show(pParentWnd);
#	else
	UNREFERENCED_PARAMETER(nIDResource);
	UNREFERENCED_PARAMETER(pParentWnd);
#	endif
}

AFX_INLINE void SetSampleDescriptionText(CWnd& wndTarget, LPCTSTR lpText)
{
	USES_CONVERSION;

	ASSERT(NULL != lpText);

	static CFont fntText;

	if (NULL == fntText.m_hObject)
	{
		LOGFONT lf = { 0 };
		if (0 == ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf))
			return;

		lf.lfHeight = ::MulDiv(lf.lfHeight, 3, 2);
		if (!fntText.CreateFontIndirect(&lf))
			return;
	}

	HFONT hOldFont = reinterpret_cast<HFONT>(wndTarget.SendMessage(WM_GETFONT));
	if (hOldFont != reinterpret_cast<HFONT>(fntText.m_hObject))
		wndTarget.SendMessage(WM_SETFONT, XTPToWPARAM(fntText.m_hObject));

	wndTarget.SetWindowText(lpText);
}

AFX_INLINE void SetSampleDescriptionText(CRichEditCtrl& wndRichEdit, LPCTSTR lpText)
{
	USES_CONVERSION;

	ASSERT(NULL != lpText);

	LOGFONT lf = { 0 };
	if (0 == ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf))
		return;

	CHARFORMAT cf = { 0 };
	cf.cbSize	 = sizeof(CHARFORMAT);
	cf.dwMask	 = (lf.lfWeight <= FW_SEMIBOLD ? CFM_BOLD : 0) | (lf.lfItalic ? CFM_ITALIC : 0)
				| (lf.lfUnderline ? CFM_UNDERLINE : 0) | (lf.lfStrikeOut ? CFM_STRIKEOUT : 0)
				| CFM_SIZE | CFM_COLOR | CFM_OFFSET | CFM_PROTECTED | CFM_FACE;
	cf.dwEffects = CFE_AUTOCOLOR;

	int nHeight = lf.lfHeight;
	if (nHeight < 0)
	{
		CDC* pDC = wndRichEdit.GetDC();
		nHeight  = MulDiv(-nHeight, 72, pDC->GetDeviceCaps(LOGPIXELSY));
		wndRichEdit.ReleaseDC(pDC);
	}

	cf.yHeight		   = ::MulDiv(nHeight, 3, 2) * 15; // 1 pixel = 15 twips
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

#	if (_RICHEDIT_VER >= 0x0200)
	lstrcpyn(cf.szFaceName, lf.lfFaceName, LF_FACESIZE);
#	else
	lstrcpynA(cf.szFaceName, T2A(lf.lfFaceName), LF_FACESIZE);
#	endif

	wndRichEdit.SetDefaultCharFormat(cf);
	wndRichEdit.SetWindowText(lpText);
}

AFX_INLINE void DrawSampleDescription(CDC* pDC, const CRect& rc, LPCTSTR lpText)
{
	ASSERT_VALID(pDC);
	ASSERT(NULL != lpText);

	static CFont fntText;
	static CSize fntSize;

	if (NULL == fntText.m_hObject)
	{
		LOGFONT lf = { 0 };
		if (0 == ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf))
			return;

		lf.lfHeight = ::MulDiv(lf.lfHeight, 3, 2);
		if (!fntText.CreateFontIndirect(&lf))
			return;

		CXTPFontDC fontDC(pDC, &fntText);
		fntSize = pDC->GetTextExtent(_T("#"), 1);
	}

	if (NULL == fntText.m_hObject)
		return;

	CXTPFontDC fontDC(pDC, &fntText);

	LPCTSTR lpBegin = lpText;
	LPCTSTR lpEnd   = lpText;

	CRect rcText = rc;
	rcText.DeflateRect(fntSize.cx * 3, fntSize.cy * 2);

	while (_T('\0') != *lpBegin)
	{
		if (_T('\r') == *lpEnd || _T('\n') == *lpEnd || _T('\0') == *lpEnd)
		{
			CRect rcComputed = rcText;
			int nHeight		 = pDC->DrawText(lpBegin,
										 static_cast<int>(static_cast<INT_PTR>(lpEnd - lpBegin)),
										 &rcComputed, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

			pDC->DrawText(lpBegin, static_cast<int>(static_cast<INT_PTR>(lpEnd - lpBegin)), &rcText,
						  DT_LEFT | DT_WORDBREAK);
			rcText.top += nHeight;

			if (_T('\r') == *lpEnd)
				++lpEnd;

			if (_T('\n') == *lpEnd)
				++lpEnd;

			lpBegin = lpEnd;
		}

		++lpEnd;
	}
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ABOUTDLG_H__)
