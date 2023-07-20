// ThemeDlg.h : Implementation for CThemeDlg class.
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

#if !defined(__THEMEDLG_H__)
#	define __THEMEDLG_H__

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	ifndef WM_DWMCOLORIZATIONCOLORCHANGED
#		define WM_DWMCOLORIZATIONCOLORCHANGED 0x0320
#	endif

/////////////////////////////////////////////////////////////////////////////
// CThemeDlg dialog

class CThemeDlg : public CDialog
{
public:
	CThemeDlg(CWnd* pParent = NULL)
		: CDialog(CThemeDlg::IDD, pParent)
		, m_crDefaultColorizationColor(::GetXtremeColor(COLOR_ACTIVECAPTION))
	{
		m_bPreview   = FALSE;
		m_dwExclude  = NULL;
		m_crBackGray = GetSysColor(COLOR_BTNFACE);
		m_brBackGray.CreateSolidBrush(m_crBackGray);
		SetTheme(xtpThemeDlgOffice2000);
	}

	enum
	{
		IDD = IDD_THEME_DIALOG
	};
	CButton m_btnOk;
	CButton m_btnApply;
#	ifdef _XTP_INCLUDE_CONTROLS
	CXTPColorPicker m_btnAccent;
#	endif
	CButton m_chkPreview;
	CComboBox m_cbBack;
	CListBox m_lbTheme;
	CListBox m_lbColor;
	CListBox m_lbAccent;
	CStatic m_txtBack;
	CStatic m_txtColor;
	CStatic m_txtAccent;
	BOOL m_bPreview;
	THEME_COLOR m_tc;

	virtual BOOL IsModified() const
	{
		return (m_tc != m_tcOld);
	}

	virtual void ExcludeTheme(DWORD dwExclude)
	{
		m_dwExclude = dwExclude;
	}

	virtual void SetTheme(int nTheme, int nColor = xtpThemeColorUndef,
						  int nAccent = xtpThemeAccentUndef, int nBack = xtpThemeRibbonBackUndef,
						  BOOL bNotify = FALSE)
	{
		m_tc = m_tcOld = THEME_COLOR(nTheme, nColor, nAccent, nBack);

		if (bNotify)
		{
			SetSelTheme(m_tc, bNotify);
		}
	}

	virtual void EnableAutoPreview(BOOL bPreview)
	{
		m_bPreview = bPreview;

		if (::IsWindow(m_chkPreview.m_hWnd))
			m_chkPreview.SetCheck(m_bPreview);

		if (::IsWindow(m_btnApply.m_hWnd))
			m_btnApply.ShowWindow(m_bPreview ? SW_HIDE : SW_SHOW);
	}

	virtual void AddCustomTheme(int nID, LPCTSTR lpszTheme, BOOL bSelected = FALSE,
								int nInsert = -1)
	{
		if (!::IsWindow(m_hWnd))
			return;

		int nItem = LB_ERR;

		if (nInsert == -1)
		{
			// add the theme to the list box and set item data.
			nItem = m_lbTheme.AddString(lpszTheme);
		}
		else
		{
			// add the theme to the list box and set item data.
			nItem = m_lbTheme.InsertString(nInsert, lpszTheme);
		}

		m_lbTheme.SetItemData(nItem, (DWORD)nID);

		if (bSelected)
		{
			m_lbTheme.SetCurSel(nItem);
			OnSelChangeListTheme();
		}
	}

	virtual BOOL InsertAfter(LPCTSTR lpszAfter, int nID, LPCTSTR lpszTheme, BOOL bSelected = FALSE)
	{
		for (int i = 0; i < m_lbTheme.GetCount(); ++i)
		{
			CString strItem;
			m_lbTheme.GetText(i, strItem);

			if (strItem.CompareNoCase(lpszAfter) == 0)
			{
				AddCustomTheme(nID, lpszTheme, bSelected, i + 1);
				return TRUE;
			}
		}
		return FALSE;
	}

	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL)
	{
		if (!CDialog::Create(nIDTemplate, pParentWnd))
			return FALSE;

		m_pParentWnd = pParentWnd;

		return TRUE;
	}

	virtual BOOL RenameItem(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
	{
		for (int i = 0; i < m_lbTheme.GetCount(); ++i)
		{
			CString strItem;
			m_lbTheme.GetText(i, strItem);

			if (strItem.CompareNoCase(lpszOldName) == 0)
			{
				int nCurSel		 = m_lbTheme.GetCurSel();
				DWORD_PTR dwData = m_lbTheme.GetItemData(i);

				m_lbTheme.DeleteString(i);
				m_lbTheme.InsertString(i, lpszNewName);
				m_lbTheme.SetItemData(i, dwData);

				if (nCurSel == i)
				{
					m_lbTheme.SetCurSel(i);
					OnSelChangeListTheme();
				}

				return TRUE;
			}
		}
		return FALSE;
	}

protected:
	DWORD m_dwExclude;
	THEME_COLOR m_tcOld;
	COLORREF m_crBackGray;
	CBrush m_brBackGray;
	const COLORREF m_crDefaultColorizationColor;

	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDOK, m_btnOk);
		DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);
		DDX_Control(pDX, IDC_CHECK_PREVIEW, m_chkPreview);
		DDX_Control(pDX, IDC_COMBO_BACK, m_cbBack);
		DDX_Control(pDX, IDC_LIST_THEME, m_lbTheme);
		DDX_Control(pDX, IDC_LIST_COLOR, m_lbColor);
		DDX_Control(pDX, IDC_LIST_ACCENT, m_lbAccent);
		DDX_Control(pDX, IDC_TXT_BACK, m_txtBack);
		DDX_Control(pDX, IDC_TXT_COLOR, m_txtColor);
		DDX_Control(pDX, IDC_TXT_ACCENT, m_txtAccent);
#	ifdef _XTP_INCLUDE_CONTROLS
		DDX_Control(pDX, IDC_BUTTON_ACCENT, m_btnAccent);
#	endif
	}

	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
#	ifdef _XTP_INCLUDE_CONTROLS
			case WM_SHOWWINDOW:
			{
				if (!wParam)
					break;

				if (XTPSystemVersion()->IsWin10OrGreater())
				{
					BOOL bUpdateColor = (m_btnAccent.GetDefaultColor() == m_btnAccent.GetColor());
					COLORREF crColorization = XTPGetColorizationColor();

					if (COLORREF_NULL == crColorization)
						crColorization = m_crDefaultColorizationColor;

					m_btnAccent.SetDefaultColor(crColorization);
					if (bUpdateColor)
						m_btnAccent.SetColor(crColorization);

					m_btnAccent.RedrawButton();
				}
			}
			break;

			case WM_DWMCOLORIZATIONCOLORCHANGED:
			{
				BOOL bUpdateColor = (m_btnAccent.GetDefaultColor() == m_btnAccent.GetColor());

				COLORREF crColorization = RGB((wParam & 0x00ff0000) >> 16,
											  (wParam & 0x0000ff00) >> 8, (wParam & 0x000000ff));
				m_btnAccent.SetDefaultColor(crColorization);
				if (bUpdateColor)
					m_btnAccent.SetColor(crColorization);

				m_btnAccent.RedrawButton();
			}
			break;
#	endif
			case WM_CTLCOLORLISTBOX:
			{
				HDC hDC   = (HDC)wParam;
				HWND hWnd = (HWND)lParam;

				if (!::IsWindowEnabled(hWnd))
				{
					::SetBkColor(hDC, m_crBackGray);
					return (LRESULT)(HBRUSH)m_brBackGray;
				}
			}
			break;
			case WM_COMMAND:
			{
				switch (HIWORD(wParam))
				{
					case LBN_SELCHANGE:
					{
						switch (LOWORD(wParam))
						{
							case IDC_LIST_THEME: OnSelChangeListTheme(); break;
							case IDC_LIST_COLOR: OnSelChange(); break;
							case IDC_LIST_ACCENT: OnSelChange(); break;
						}
					}
					break;
					case CBN_SELENDOK:
					{
						switch (LOWORD(wParam))
						{
							case IDC_COMBO_BACK: OnSelChange(); break;
						}
					}
					break;
					case BN_CLICKED:
					{
						switch (LOWORD(wParam))
						{
							case IDC_BUTTON_APPLY: OnApply(); break;
							case IDC_CHECK_PREVIEW: OnCheckPreview(); break;
						}
					}
					break;
#	ifdef _XTP_INCLUDE_CONTROLS
					case CPN_XTP_SELENDOK: ColorChanged(); break;
#	endif
				}
			}
			break;
		}

		return CDialog::WindowProc(uMsg, wParam, lParam);
	}

	virtual void BuildThemeList()
	{
		if (!::IsWindow(m_hWnd))
			return;

		for (int i = 0; i < _countof(g_szThemes); ++i)
		{
			// if theme is hidden skip and continue.
			switch (i)
			{
				case xtpThemeDlgOffice2000:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2000)
						continue;
				}
				break;
				case xtpThemeDlgOfficeXP:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICEXP)
						continue;
				}
				break;
				case xtpThemeDlgOffice2003:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2003)
						continue;
				}
				break;
				case xtpThemeDlgOffice2007:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2007)
						continue;
				}
				break;
				case xtpThemeDlgOffice2010:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2010)
						continue;
				}
				break;
				case xtpThemeDlgOffice2013:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2013)
						continue;
				}
				break;
				case xtpThemeDlgOffice2016:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_OFFICE2016)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio6:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO6)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2005:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2005)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2008:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2008)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2010:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2010)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2012:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2012)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2015:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2015)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2017:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2017)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2019:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2019)
						continue;
				}
				break;
				case xtpThemeDlgVisualStudio2022:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_VISUALSTUDIO2022)
						continue;
				}
				break;
				case xtpThemeDlgNativeWindows:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_NATIVEWINDOWS)
						continue;
				}
				break;
				case xtpThemeDlgNativeWindows10:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_NATIVEWINDOWS10)
						continue;
				}
				break;
				case xtpThemeDlgCustom:
				{
					if (m_dwExclude & XTP_EXCLUDE_THEME_CUSTOM)
						continue;
				}
				break;
			}

			// add the theme to the list box and set item data.
			int nItem = m_lbTheme.AddString(g_szThemes[i]);
			m_lbTheme.SetItemData(nItem, (DWORD)i);

			if (i == m_tc.nTheme)
			{
				m_lbTheme.SetCurSel(nItem);
			}
		}

		// if no selections made, select the first item.
		if (m_lbTheme.GetCurSel() == LB_ERR && m_lbTheme.GetCount() > 0)
			m_tc.nTheme = xtpThemeDlgOffice2000;

		// hide back combobox if not used.
		if (m_tc.nBack == xtpThemeRibbonBackUndef)
		{
			m_cbBack.ShowWindow(SW_HIDE);
			m_txtBack.ShowWindow(SW_HIDE);
		}

		BuildColorList();
	}

	virtual void BuildColorList()
	{
		if (!::IsWindow(m_hWnd))
			return;

		// reset list and combo boxes.
		m_lbColor.ResetContent();
		m_lbAccent.ResetContent();
		m_cbBack.ResetContent();
#	ifdef _XTP_INCLUDE_CONTROLS
		m_btnAccent.EnableWindow(FALSE);
#	endif

		int i = 0;

		switch (m_tc.nTheme)
		{
			case xtpThemeDlgOffice2007:
			{
				for (i = 0; i < _countof(g_szColor2007); ++i)
					m_lbColor.AddString(g_szColor2007[i]);
			}
			break;
			case xtpThemeDlgOffice2010:
			{
				for (i = 0; i < _countof(g_szColor2010); ++i)
					m_lbColor.AddString(g_szColor2010[i]);
			}
			break;
			case xtpThemeDlgOffice2013:
			{
				for (i = 0; i < _countof(g_szColor2013); ++i)
					m_lbColor.AddString(g_szColor2013[i]);

				for (i = 0; i < _countof(g_szAccent2016); ++i)
					m_lbAccent.AddString(g_szAccent2016[i]);

				for (i = 0; i < _countof(g_szBack); ++i)
					m_cbBack.AddString(g_szBack[i]);
			}
			break;
			case xtpThemeDlgOffice2016:
			{
				for (i = 0; i < _countof(g_szColor2016); ++i)
					m_lbColor.AddString(g_szColor2016[i]);

				for (i = 0; i < _countof(g_szAccent2016); ++i)
					m_lbAccent.AddString(g_szAccent2016[i]);

				for (i = 0; i < _countof(g_szBack); ++i)
					m_cbBack.AddString(g_szBack[i]);
			}
			break;
			case xtpThemeDlgVisualStudio2012:
			{
				for (i = 0; i < _countof(g_szColor2012); ++i)
					m_lbColor.AddString(g_szColor2012[i]);
			}
			break;
			case xtpThemeDlgVisualStudio2015:
			{
				for (i = 0; i < _countof(g_szColor2015); ++i)
					m_lbColor.AddString(g_szColor2015[i]);
			}
			break;
			case xtpThemeDlgVisualStudio2017:
			{
				for (i = 0; i < _countof(g_szColor2017); ++i)
					m_lbColor.AddString(g_szColor2017[i]);
			}
			break;
			case xtpThemeDlgVisualStudio2019:
			{
				for (i = 0; i < _countof(g_szColor2019); ++i)
					m_lbColor.AddString(g_szColor2019[i]);
			}
			break;
			case xtpThemeDlgVisualStudio2022:
			{
				for (i = 0; i < _countof(g_szColor2022); ++i)
					m_lbColor.AddString(g_szColor2022[i]);
			}
			break;
			case xtpThemeDlgNativeWindows10:
			{
				for (i = 0; i < _countof(g_szColorWindows10); ++i)
					m_lbColor.AddString(g_szColorWindows10[i]);

#	ifdef _XTP_INCLUDE_CONTROLS
				m_btnAccent.EnableWindow(TRUE);
#	endif
			}
			break;
		}

		// enable / disable color listbox.
		if (m_lbColor.GetCount() > 0)
		{
			m_lbColor.EnableWindow(TRUE);
			m_txtColor.EnableWindow(TRUE);

			if (m_lbColor.SetCurSel(m_tc.nColor) == LB_ERR)
			{
				m_tc.nColor = 0;
				m_lbColor.SetCurSel(0);
			}
		}
		else
		{
			m_lbColor.EnableWindow(FALSE);
			m_txtColor.EnableWindow(FALSE);
		}

		// enable / disable accent listbox.
		if (m_lbAccent.GetCount() > 0)
		{
			m_lbAccent.EnableWindow(TRUE);
			m_txtAccent.EnableWindow(TRUE);

			if (m_lbAccent.SetCurSel(m_tc.nAccent) == LB_ERR)
			{
				m_tc.nAccent = 0;
				m_lbAccent.SetCurSel(0);
			}
		}
		else
		{
			m_lbAccent.EnableWindow(FALSE);
			m_txtAccent.EnableWindow(FALSE);
		}

		if (m_tc.nBack != xtpThemeRibbonBackUndef)
		{
			// enable / disable back combobox.
			if (m_cbBack.GetCount() > 0)
			{
				m_cbBack.EnableWindow(TRUE);
				m_txtBack.EnableWindow(TRUE);

				if (m_cbBack.SetCurSel(m_tc.nBack) == LB_ERR)
				{
					m_tc.nBack = xtpThemeRibbonBackNone;
					m_cbBack.SetCurSel(0);
				}
			}
			else
			{
				m_cbBack.EnableWindow(FALSE);
				m_txtBack.EnableWindow(FALSE);
			}
		}
	}

	virtual void SendNotify(const THEME_COLOR& tc)
	{
		if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
		{
			m_pParentWnd->SendMessage(WM_XTP_THEMECHANGED, 0, (LPARAM)&tc);
		}
	}

	virtual void EnableButtons()
	{
		m_btnOk.EnableWindow(IsModified());
		m_btnApply.EnableWindow(IsModified());
	}

	virtual int SetSelTheme(const THEME_COLOR& tc, BOOL bNotify = FALSE)
	{
		if (!::IsWindow(m_hWnd))
			return LB_ERR;

		for (int i = 0; i < m_lbTheme.GetCount(); ++i)
		{
			int nTheme = (int)m_lbTheme.GetItemData(i);

			if (nTheme == tc.nTheme)
			{
				m_lbTheme.SetCurSel(i);
				BuildColorList();

				if (bNotify)
				{
					SendNotify(tc);
				}

				return i;
			}
		}
		return m_lbTheme.GetCurSel();
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();

		// Set the default font for all dialog controls.
		SendMessageToDescendants(WM_SETFONT, (WPARAM)(HFONT)XTPFontManager()->GetFont());

		BuildThemeList();

#	ifdef _XTP_INCLUDE_CONTROLS
		COLORREF crColorization = XTPGetColorizationColor();

		if (COLORREF_NULL == crColorization)
			crColorization = m_crDefaultColorizationColor;

		m_btnAccent.SetDefaultColor(crColorization);
		m_btnAccent.SetColor(crColorization);
#	else
		GetDlgItem(IDC_BUTTON_ACCENT)->ShowWindow(SW_HIDE);
#	endif

		return TRUE;
	}

	virtual void OnCancel()
	{
		if (IsModified())
		{
			m_tc = m_tcOld;
			SetSelTheme(m_tc, TRUE);
			EnableButtons();
		}

		CDialog::OnCancel();
	}

	virtual void OnOK()
	{
		if (IsModified())
		{
			m_tcOld = m_tc;
			BuildColorList();
			EnableButtons();
			SendNotify(m_tc);
		}

		CDialog::OnOK();
	}

	void OnApply()
	{
		if (IsModified())
		{
			m_tcOld = m_tc;
			EnableButtons();
			SendNotify(m_tc);
		}
	}

	void OnSelChangeListTheme()
	{
		int nItem = m_lbTheme.GetCurSel();

		if (nItem == LB_ERR)
			return;

		// set defaults.
		m_tc.nTheme  = (int)m_lbTheme.GetItemData(nItem);
		m_tc.nColor  = xtpThemeColorUndef;
		m_tc.nAccent = xtpThemeAccentUndef;
		m_tc.nBack   = xtpThemeRibbonBackUndef;

		BuildColorList();
		EnableButtons();

#	ifdef _XTP_INCLUDE_CONTROLS
		if (m_btnAccent.IsWindowEnabled())
		{
			m_tc.nAccent = m_btnAccent.GetColor();

			if (m_tc.nAccent == static_cast<int>(XTPGetColorizationColor()))
				m_tc.nAccent = COLORREF_NULL;

			m_tc.nBack = 0;
		}
#	endif

		if (m_bPreview)
		{
			SendNotify(m_tc);
		}
	}

	void OnSelChange()
	{
		int nItem = m_lbTheme.GetCurSel();

		if (nItem == LB_ERR)
			return;

		// set defaults.
		m_tc.nTheme  = (int)m_lbTheme.GetItemData(nItem);
		m_tc.nColor  = (int)m_lbColor.GetCurSel();
		m_tc.nAccent = (int)m_lbAccent.GetCurSel();
		m_tc.nBack   = (int)m_cbBack.GetCurSel();

		EnableButtons();

#	ifdef _XTP_INCLUDE_CONTROLS
		if (m_btnAccent.IsWindowEnabled())
		{
			m_tc.nAccent = m_btnAccent.GetColor();

			if (m_tc.nAccent == static_cast<int>(XTPGetColorizationColor()))
				m_tc.nAccent = COLORREF_NULL;

			m_tc.nBack = 0;
		}
#	endif

		if (m_bPreview)
		{
			SendNotify(m_tc);
		}
	}

	void OnCheckPreview()
	{
		m_bPreview = !m_bPreview;
		m_chkPreview.SetCheck(m_bPreview);
		m_btnApply.ShowWindow(m_bPreview ? SW_HIDE : SW_SHOW);
	}

	void ColorChanged()
	{
#	ifdef _XTP_INCLUDE_CONTROLS
		if (m_bPreview)
		{
			m_tc.nAccent = m_btnAccent.GetColor();

			if (m_tc.nAccent == static_cast<int>(XTPGetColorizationColor()))
				m_tc.nAccent = COLORREF_NULL;

			m_tc.nBack = 0;

			SendNotify(m_tc);
		}

		EnableButtons();

		GetParentFrame()->PostMessage(WM_SETFOCUS);

#	endif
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__THEMEDLG_H__)
