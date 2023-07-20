// XTPSyntaxEditPropertiesPage.h : header file
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
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITPROPERTIESPAGE_H__)
#	define __XTPSYNTAXEDITPROPERTIESPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditPropertiesDlg;
class CXTPSyntaxEditView;
class CXTPSyntaxEditTextSchemesManager;
class CXTPSyntaxEditPaintManager;
struct XTP_EDIT_SCHEMAFILEINFO;

class CXTPSyntaxEditConfigurationManager;

namespace XTPSyntaxEditLexAnalyser
{
class CXTPSyntaxEditLexPropInfoArray;
class CXTPSyntaxEditLexClassInfoArray;
struct XTP_EDIT_LEXCLASSINFO;
struct XTP_EDIT_LEXPROPINFO;
} // namespace XTPSyntaxEditLexAnalyser

//===========================================================================
// Summary:
//     CXTPSyntaxEditTipWnd implements a tooltip window for
//     the CXTPSyntaxEditTipListBox class.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTipWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTipWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditTipWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Create a tooltip window.
	// Parameters:
	//     pListBox - Pointer to a CListBox to attach.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     ShowTip
	//-----------------------------------------------------------------------
	virtual BOOL Create(CListBox* pListBox);

	using CWnd::Create;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the attached list box has owner draw flags
	//     (LBS_OWNERDRAWFIXED or LBS_OWNERDRAWVARIABLE).
	// Returns:
	//     TRUE if the attached list box is owner drawn, otherwise FALSE.
	// See Also:
	//     CListBox, LBS_OWNERDRAWFIXED, LBS_OWNERDRAWVARIABLE
	//-----------------------------------------------------------------------
	virtual BOOL IsOwnerDrawn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows the tooltip for a specified list box item.
	// Parameters:
	//     iIndex - Index of the list box item to show the tooltip for.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     HideTip
	//-----------------------------------------------------------------------
	virtual BOOL ShowTip(int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Hides the tooltip.
	// Returns:
	//     TRUE if the tooltip was visible prior to using this method and
	//     was successfully hidden by using this method,
	//     FALSE if the tooltip was hidden prior to using this method.
	// See Also:
	//     ShowTip
	//-----------------------------------------------------------------------
	virtual BOOL HideTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip text color.
	// Returns:
	//     The tooltip text color.
	// See Also:
	//     GetBackColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tooltip background color.
	// Returns:
	//     The tooltip background color.
	// See Also:
	//     GetTextColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackColor() const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Registers the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of the resource where the control is located.
	// Returns:
	//     TRUE if the window class was successfully registered, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the tooltip for the owner drawn list box.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rClient - Tooltip rectangle (in client coordinates of this window).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     DrawTip
	//-----------------------------------------------------------------------
	virtual BOOL OwnerDrawTip(CDC* pDC, CRect rClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the tooltip.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rClient - Tooltip rectangle (in client coordinates of this window).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     OwnerDrawTip
	//-----------------------------------------------------------------------
	virtual BOOL DrawTip(CDC* pDC, CRect rClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the rectangle for a specified item.
	// Parameters:
	//     iItem - [in] List box item index.
	//     rItem - [out] Reference to a CRect object to receive
	//                   the rectangle for the specified item.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CalcItemRect(int iItem, CRect& rItem);

//{{AFX_VIRTUAL(CXTPSyntaxEditTipWnd)
//}}AFX_VIRTUAL

//{{AFX_CODEJOCK_PRIVATE
#	ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#	endif
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcPaint();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_iIndex;		  // Current item index.
	CRect m_rWindow;	  // Window rect.
	CPoint m_ptCursor;	// Cursor position.
	CListBox* m_pListBox; // Pointer to the attached list box.

private:
	void SetTipTimer();
	void KillTipTimer();
	UINT m_uIDEvent1;
	UINT m_uIDEvent2;
};

//===========================================================================
// Summary:
//      This class implements a list box with a tooltip.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTipListBox : public CListBox
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTipListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which list box item, if any, is at a specified point.
	// Parameters:
	//     pPoint    - Pointer to a POINT object or a NULL value. If the value of this
	//                 parameter is NULL, then the current mouse coordinates are used.
	//     point     - Point to be tested.
	//     bIsClient - TRUE if the point contains client coordinates,
	//                 FALSE if the point contains screen coordinates.
	// Returns:
	//     The list box item index at the specified point, if any, otherwise LB_ERR.
	//-----------------------------------------------------------------------
	int HitTest(LPPOINT pPoint = NULL) const;
	int HitTest(CPoint point,
				BOOL bIsClient = FALSE) const; // <combine
											   // CXTPSyntaxEditTipListBox::HitTest@LPPOINT@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows the tooltip at a specified point, if any.
	// Parameters:
	//     point     - Point to be tested.
	//     bIsClient - TRUE if the point contains client coordinates,
	//                 FALSE if the point contains screen coordinates.
	// Returns:
	//     The list box item index at the specified point, if any, otherwise LB_ERR.
	//-----------------------------------------------------------------------
	int ShowTip(CPoint point, BOOL bIsClient = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the selection was changed by the user.
	// Returns:
	//     TRUE if the selection was changed by the user, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SelChanged(); // const;

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	DWORD m_dwIdx; // Selection indexes: LOWORD - previous selection index, HIWORD - active
				   // selection index.
	CXTPSyntaxEditTipWnd m_wndInfoTip; // Tooltip window.
};

//===========================================================================
// Summary:
//      This class implements a combo box with a tooltip.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTipComboBox : public CComboBox
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the attached list box.
	// Returns:
	//     A reference to the attached list box.
	// See Also:
	//     CXTPSyntaxEditTipListBox
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTipListBox& GetListBox();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE
private:
	CXTPSyntaxEditTipListBox m_wndListBox;
};

AFX_INLINE CXTPSyntaxEditTipListBox& CXTPSyntaxEditTipComboBox::GetListBox()
{
	return m_wndListBox;
}

//===========================================================================
// Summary:
//     This class implements a property page for common edit options.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditPropertiesPageEdit : public CPropertyPage
{
	DECLARE_DYNCREATE(CXTPSyntaxEditPropertiesPageEdit)

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEditView  - Pointer to a CXTPSyntaxEditView object.
	// -------------------------------------------------------------------
	CXTPSyntaxEditPropertiesPageEdit(CXTPSyntaxEditView* pEditView = NULL);

	// -------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	// -------------------------------------------------------------------
	virtual ~CXTPSyntaxEditPropertiesPageEdit();

	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_EDIT_PAGEEDITOR
	};
	BOOL m_bAutoReload;
	BOOL m_bHorzScrollBar;
	BOOL m_bVertScrollBar;
	BOOL m_bSyntaxColor;
	BOOL m_bAutoIndent;
	BOOL m_bSelMargin;
	BOOL m_bLineNumbers;
	BOOL m_bVirtualSpace;
	BOOL m_bCollapsibleNodes;
	int m_nCaretStyle;
	int m_nTabType;
	int m_nTabSize;
	CButton m_btnRadioSpaces;
	CButton m_btnRadioTab;
	CButton m_btnRadioCaretThin;
	CButton m_btnRadioCaretThick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
public:
	virtual BOOL OnApply();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	// -------------------------------------------------------------------
	// Summary:
	//     Reads controls state (options) from the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     WriteRegistryValues
	// -------------------------------------------------------------------
	BOOL ReadRegistryValues();

	// -------------------------------------------------------------------
	// Summary:
	//     Writes controls state (options) to the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     ReadRegistryValues
	// -------------------------------------------------------------------
	BOOL WriteRegistryValues();

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the value of the modified flag.
	// Parameters:
	//     bChanged - TRUE if options were modified, FALSE otherwise.
	// See Also:
	//     CPropertyPage::SetModified
	// -------------------------------------------------------------------
	void SetModified(BOOL bChanged = TRUE);

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnInitDialog();
	afx_msg void OnChkAutoReload();
	afx_msg void OnChkHorzScrollBar();
	afx_msg void OnChkVertScrollBar();
	afx_msg void OnChkSyntaxColor();
	afx_msg void OnChkAutoIndent();
	afx_msg void OnChkSelMargin();
	afx_msg void OnChkLineNumbers();
	afx_msg void OnChkVirtualSpace();
	afx_msg void OnChangeTabsSize();
	afx_msg void OnTabsSpaces();
	afx_msg void OnTabsTab();
	afx_msg void OnCaretThin();
	afx_msg void OnCaretThick();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bModified;				 // Modified flag.
	CXTPSyntaxEditView* m_pEditView; // Pointer to a CXTPSyntaxEditView object.
};

//===========================================================================
// Summary:
//     This class implements property page font options.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditPropertiesPageFont : public CPropertyPage
{
	DECLARE_DYNCREATE(CXTPSyntaxEditPropertiesPageFont)

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEditView  - Pointer to a CXTPSyntaxEditView object.
	// -------------------------------------------------------------------
	CXTPSyntaxEditPropertiesPageFont(CXTPSyntaxEditView* pEditView = NULL);

	// -------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	// -------------------------------------------------------------------
	virtual ~CXTPSyntaxEditPropertiesPageFont();

	// -------------------------------------------------------------------
	// Summary:
	//     Gets the editor font.
	// Returns:
	//     A reference to the editor font.
	// See Also:
	//     GetSafeLogFont
	// -------------------------------------------------------------------
	CFont& GetEditFont();

	// -------------------------------------------------------------------
	// Summary:
	//     Gets the LOGFONT data for the editor font object.
	// Parameters:
	//     lf - Reference to a LOGFONT structure to receive the font data.
	// Returns:
	//     TRUE if the font data was obtained, otherwise FALSE.
	// See Also:
	//     GetEditFont
	// -------------------------------------------------------------------
	BOOL GetSafeLogFont(LOGFONT& lf);

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the font for a specified CFont object.
	// Parameters:
	//     editFont - Reference to the CFont object to set the font of.
	//     lf       - Reference to a LOGFONT structure containing
	//                the font to be set.
	// Remarks:
	//     The previous font will be deleted.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     CFont::CreateFontIndirect
	// -------------------------------------------------------------------
	BOOL CreateSafeFontIndirect(CFont& editFont, const LOGFONT& lf);

	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_EDIT_PAGEFONT
	};
	CXTPSyntaxEditColorComboBox m_wndComboHiliteText;
	CXTPSyntaxEditColorComboBox m_wndComboHiliteBack;
	CXTPSyntaxEditColorComboBox m_wndComboText;
	CXTPSyntaxEditColorComboBox m_wndComboBack;
	CXTPSyntaxEditColorSampleText m_txtSampleSel;
	CXTPSyntaxEditColorSampleText m_txtSample;
	CButton m_btnCustomText;
	CButton m_btnCustomBack;
	CButton m_btnCustomHiliteText;
	CButton m_btnCustomHiliteBack;
	CXTPSyntaxEditTipComboBox m_wndComboScript;
	CXTPSyntaxEditTipComboBox m_wndComboStyle;
	CXTPSyntaxEditTipComboBox m_wndComboSize;
	CXTPSyntaxEditTipComboBox m_wndComboName;
	BOOL m_bStrikeOut;
	BOOL m_bUnderline;
	CString m_csName;
	CString m_csSize;
	CString m_csStyle;
	COLORREF m_crHiliteText;
	COLORREF m_crHiliteBack;
	COLORREF m_crText;
	COLORREF m_crBack;
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
public:
	virtual BOOL OnApply();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the font name combo box.
	// -------------------------------------------------------------------
	void InitFontCombo();

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the font style combo box.
	// -------------------------------------------------------------------
	void InitStyleCombo();

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the font size combo box.
	// -------------------------------------------------------------------
	void InitSizeCombo();

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the font script combo box.
	// -------------------------------------------------------------------
	void InitScriptCombo();

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the color combo box.
	// -------------------------------------------------------------------
	void InitColorComboxes();

	// -------------------------------------------------------------------
	// Summary:
	//     Updates the font for a sample text control.
	// -------------------------------------------------------------------
	void UpdateSampleFont();

	// -------------------------------------------------------------------
	// Summary:
	//     Updates the colors for a sample text control.
	// -------------------------------------------------------------------
	void UpdateSampleColors();

	// -------------------------------------------------------------------
	// Summary:
	//     Gets the text of the selected list box item and updates
	//     the combo box.
	// Parameters:
	//     comboBox   - [in] Reference to a CComboBox object.
	//     csItemText - [out] Reference to a CString to receive the text of
	//                        the selected list box item.
	// Returns:
	//     The index of the selected list box item if successful, otherwise CB_ERR.
	// See Also:
	//     CComboBox::GetLBText
	// -------------------------------------------------------------------
	int GetLBText(CComboBox& comboBox, CString& csItemText);

	// -------------------------------------------------------------------
	// Summary:
	//     Reads controls state (options) from the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     WriteRegistryValues
	// -------------------------------------------------------------------
	BOOL ReadRegistryValues();

	// -------------------------------------------------------------------
	// Summary:
	//     Writes controls state (options) to the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     ReadRegistryValues
	// -------------------------------------------------------------------
	BOOL WriteRegistryValues();

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the value of the modified flag.
	// Parameters:
	//     bChanged - TRUE if options were modified, FALSE otherwise.
	// See Also:
	//     CPropertyPage::SetModified
	// -------------------------------------------------------------------
	void SetModified(BOOL bChanged = TRUE);

	// -------------------------------------------------------------------
	// Summary:
	//     Gets the edit control paint manager.
	// Returns:
	//     A pointer to the edit control paint manager.
	// -------------------------------------------------------------------
	CXTPSyntaxEditPaintManager* GetPaintManager();

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeComboNames();
	afx_msg void OnSelChangeComboStyles();
	afx_msg void OnSelChangeComboSizes();
	afx_msg void OnChkStrikeOut();
	afx_msg void OnChkUnderline();
	afx_msg void OnSelEndOkScript();
	afx_msg void OnBtnCustomText();
	afx_msg void OnBtnCustomBack();
	afx_msg void OnBtnCustomHiliteText();
	afx_msg void OnBtnCustomtHiliteBack();
	afx_msg void OnSelEndOkHiliteText();
	afx_msg void OnSelEndOkHiliteBack();
	afx_msg void OnSelEndOkText();
	afx_msg void OnSelEndOkBack();

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	const UINT m_uFaceSize; // Face name maximum string size.

	BYTE m_iCharSet;				 // Char set.
	BOOL m_bModified;				 // Modified flag.
	CXTPSyntaxEditView* m_pEditView; // Pointer to a CXTPSyntaxEditView object.

	CXTPFont m_xtpFontEdit; // Editor font.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_editFont, m_xtpFontEdit, GetEditFontHandle);
};

//---------------------------------------------------------------------------

AFX_INLINE CFont& CXTPSyntaxEditPropertiesPageFont::GetEditFont()
{
	return m_xtpFontEdit;
}

//===========================================================================
// Summary:
//     This class implements property page colors options.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditPropertiesPageColor : public CPropertyPage
{
	DECLARE_DYNCREATE(CXTPSyntaxEditPropertiesPageColor)

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEditView  - Pointer to a CXTPSyntaxEditView object.
	// -------------------------------------------------------------------
	CXTPSyntaxEditPropertiesPageColor(CXTPSyntaxEditView* pEditView = NULL);

	// -------------------------------------------------------------------
	// Summary:
	//     Default object destructor.
	// -------------------------------------------------------------------
	virtual ~CXTPSyntaxEditPropertiesPageColor();

	// -------------------------------------------------------------------
	// Summary:
	//     Writes controls state (options) to the registry.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// -------------------------------------------------------------------
	BOOL WriteRegistryValues();

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the value of the modified flag.
	// Parameters:
	//     bChanged - TRUE if options were modified, FALSE otherwise.
	// See Also:
	//     CPropertyPage::SetModified
	// -------------------------------------------------------------------
	void SetModified(BOOL bChanged = TRUE);

	//{{AFX_CODEJOCK_PRIVATE
	enum
	{
		IDD = XTP_IDD_EDIT_PAGECOLOR
	};
	CXTPSyntaxEditColorSampleText m_txtSampleSel;
	CXTPSyntaxEditColorSampleText m_txtSample;
	CXTPSyntaxEditColorComboBox m_wndComboHiliteText;
	CXTPSyntaxEditColorComboBox m_wndComboHiliteBack;
	CXTPSyntaxEditColorComboBox m_wndComboText;
	CXTPSyntaxEditColorComboBox m_wndComboBack;
	CButton m_btnBold;
	CButton m_btnItalic;
	CButton m_btnUnderline;
	CButton m_btnCustomText;
	CButton m_btnCustomBack;
	CButton m_btnCustomHiliteText;
	CButton m_btnCustomHiliteBack;
	CXTPSyntaxEditTipListBox m_lboxName;
	CXTPSyntaxEditTipListBox m_lboxProp;
	CStatic m_gboxSampleText;
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	COLORREF m_crHiliteText;
	COLORREF m_crHiliteBack;
	COLORREF m_crText;
	COLORREF m_crBack;

public:
	virtual BOOL OnSetActive();

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
public:
	virtual BOOL OnApply();
	//}}AFX_CODEJOCK_PRIVATE

protected:
	// -------------------------------------------------------------------
	// Summary:
	//     Updates the colors for a sample text control.
	// -------------------------------------------------------------------
	void UpdateSampleColors();

	// -------------------------------------------------------------------
	// Summary:
	//     Updates the font for a sample text control.
	// -------------------------------------------------------------------
	void UpdateFont();

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes dialog controls for a specified lex schema.
	// Parameters:
	//     pSchemaInfo - Pointer to an XTP_EDIT_SCHEMAFILEINFO object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// -------------------------------------------------------------------
	BOOL InitSchemaClasses(XTP_EDIT_SCHEMAFILEINFO* pSchemaInfo);

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes dialog controls for a specified lex class.
	// Parameters:
	//     infoClass - Reference to an XTP_EDIT_LEXCLASSINFO object.
	// -------------------------------------------------------------------
	void InitClassData(const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO& infoClass);

	// -------------------------------------------------------------------
	// Summary:
	//     Determines if a specified class is a top-level schema class.
	// Parameters:
	//     infoClass - Reference to an XTP_EDIT_LEXCLASSINFO object.
	// Returns:
	//     TRUE if the specified class is a top-level schema class, otherwise FALSE.
	// -------------------------------------------------------------------
	BOOL IsTopLevelClass(const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO& infoClass);

	// -------------------------------------------------------------------
	// Summary:
	//     Updates controls state (enabled/disabled).
	// -------------------------------------------------------------------
	void EnableControls();

	// -------------------------------------------------------------------
	// Summary:
	//     Gets a specified lex property as a comma-separated string.
	// Parameters:
	//     infoProp - Reference to an XTP_EDIT_LEXPROPINFO object.
	// Returns:
	//     The specified lex property as a comma-separated string.
	// See Also:
	//     XTP_EDIT_LEXPROPINFO
	// -------------------------------------------------------------------
	CString GetPropValue(const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO& infoProp) const;

	// -------------------------------------------------------------------
	// Summary:
	//     Sets defaults controls state.
	// -------------------------------------------------------------------
	void SetDefaults();

	// -------------------------------------------------------------------
	// Summary:
	//     Determines if a specified property data exists within a specified array.
	// Parameters:
	//     arrProp      - Reference to the properties data array.
	//     lpszPropName - Lex property name.
	// Returns:
	//     The index of the specified property data within the specified array
	//     if it exists, otherwise a value of -1.
	// -------------------------------------------------------------------
	int PropExists(XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexPropInfoArray& arrProp,
				   LPCTSTR lpszPropName);

	// -------------------------------------------------------------------
	// Summary:
	//     Updates the font parameters for the currently selected lex class.
	// Parameters:
	//     lpszPropName - Lex class property name (font attribute).
	//     bValue       - Reference to the value to be set.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     UpdateColorValue
	// -------------------------------------------------------------------
	BOOL UpdateFontValue(BOOL& bValue, LPCTSTR lpszPropName);

	// -------------------------------------------------------------------
	// Summary:
	//     Updates the color parameters for currently selected lex class.
	// Parameters:
	//     combo        - Reference to an edit color combo box.
	//     color        - Reference to the value to be set.
	//     lpszPropName - Lex class property name (color attribute).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     UpdateFontValue
	// -------------------------------------------------------------------
	BOOL UpdateColorValue(CXTPSyntaxEditColorComboBox& combo, COLORREF& color,
						  LPCTSTR lpszPropName);

	// -------------------------------------------------------------------
	// Summary:
	//     Gets the display name property value of a lex class.
	// Parameters:
	//     info - Reference to an XTP_EDIT_LEXCLASSINFO object.
	// Returns:
	//     The display name property value of the lex class.
	// -------------------------------------------------------------------
	CString GetDisplayName(const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO& info) const;

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnCustomText();
	afx_msg void OnBtnCustomBack();
	afx_msg void OnBtnCustomHiliteText();
	afx_msg void OnBtnCustomtHiliteBack();
	afx_msg void OnChkBold();
	afx_msg void OnChkItalic();
	afx_msg void OnChkUnderline();
	afx_msg void OnSelEndOkHiliteText();
	afx_msg void OnSelEndOkHiliteBack();
	afx_msg void OnSelEndOkText();
	afx_msg void OnSelEndOkBack();
	afx_msg void OnSelChangeSchemaNames();
	afx_msg void OnSelChangeSchemaProp();
	afx_msg void OnDblClickSchema();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bModified;									 // Modified flag.
	CXTPSyntaxEditView* m_pEditView;					 // Pointer to a CXTPSyntaxEditView object.
	CXTPSyntaxEditConfigurationManager* m_ptrConfigMgr;  // Pointer to Configuration Manager.
	CXTPSyntaxEditTextSchemesManager* m_pTextSchemesMgr; // Pointer to Schemes Manager.
	CXTPFont m_xtpFontEdit;								 // Editor font.
	XTP_SUBSTITUTE_GDI_MEMBER_WITH_CACHED(CFont, m_editFont, m_xtpFontEdit, GetEditFontHandle);
	CMapStringToPtr m_mapLexClassInfo; // Map lex schema name to schema classes array
									   // (CXTPSyntaxEditLexClassInfoArray*).
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray* m_parLexClassInfo; // Currently
																				  // selected schema
																				  // classes.
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITPROPERTIESPAGE_H__)
