// XTPFontComboBox.h interface for the CXTPFontComboBox class.
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
#if !defined(__XTPFONTCOMBOBOX_H__)
#	define __XTPFONTCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

// ----------------------------------------------------------------------
// Summary:
//     List used for enumerating CXTPLogFont structures.
// Remarks:
//     CList definition used by the CXTPFontEnum class to maintain
//     a list of CXTPLogFont objects representing each font displayed
//     for a CXTPFontListBox control.
// See Also:
//     CXTPFontEnum::GetFontList
// ----------------------------------------------------------------------
typedef CList<CXTPLogFont, CXTPLogFont&> CXTPFontList;

//===========================================================================
// Summary:
//     CXTPFontEnum is a standalone singleton font enumeration class. It is
//     used to enumerate a list of fonts found installed for the operating system.
// Remarks:
//     CXTPFontEnum is a singleton class, which means that it can only be
//     instantiated a single time. The constructor is private, so the only
//     way to access members of this class is to use the object's Get() method.
//     To retrieve a list of available fonts for your operating system, you
//     would make the following call:
// Example:
//     The following example demonstrates the use of the CXTPFontEnum class:
// <code>
// CXTPFontEnum::Get().GetFontList()
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPFontEnum
{
	friend class CXTPSingleton<CXTPFontEnum>;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFontEnum object. CXTPFontEnum is a singleton
	//     class. To instantiate an object, use the static method Get().
	//-----------------------------------------------------------------------
	CXTPFontEnum();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFontEnum object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFontEnum();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This static member function will return a reference to the one
	//     and only CXTPFontEnum object. You can use this function to access
	//     data members for the CXTPFontEnum class.
	// Example:
	//     The following example demonstrates the use of Get():
	// <code>
	// CXTPFontEnum::Get().GetFontList()
	// </code>
	// Returns:
	//     A reference to a CXTPFontEnum object.
	//-----------------------------------------------------------------------
	static CXTPFontEnum& AFX_CDECL Get();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine the existence of the font
	//     specified by 'strFaceName'.
	// Parameters:
	//     strFaceName - Reference to a NULL-terminated string that represents
	//                   the font name.
	// Returns:
	//     true if the font exists, otherwise false.
	//-----------------------------------------------------------------------
	bool DoesFontExist(CString& strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get a pointer to the font specified
	//     by 'strFaceName'.
	// Parameters:
	//     strFaceName - A NULL-terminated string that represents the font
	//                   name.
	// Returns:
	//     A pointer to the CXTPLogFont structure for the specified item,
	//     or NULL if no font was found.
	//-----------------------------------------------------------------------
	CXTPLogFont* GetLogFont(const CString& strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used by the callback function to retrieve the
	//     current width for the longest font name in the list.
	// Returns:
	//     An integer value that represents the width for the
	//     longest font name in the list.
	//-----------------------------------------------------------------------
	int GetMaxWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the widest size of a string provided using available fonts.
	// Returns:
	//     An integer value that represents the widest size of a string.
	//-----------------------------------------------------------------------
	int GetMaxWidth(LPCTSTR lpString);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get a reference to the font list.
	// Returns:
	//     A reference to the CXTPFontList used by this class.
	//-----------------------------------------------------------------------
	CXTPFontList& GetFontList();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPFontEnum class to initialize
	//     the font list. You can also call this member function to reinitialize
	//     the font enumeration. For example, if you changed printers and you
	//     want to enumerate printer fonts, or you wanted to use a different
	//     character set.
	// Parameters:
	//     pDC      - Pointer to a valid device context. If NULL, then the
	//                screen device context is used.
	//     nCharSet - Represents the character set to enumerate.
	//-----------------------------------------------------------------------
	void Init(CDC* pDC = NULL, BYTE nCharSet = DEFAULT_CHARSET);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     The EnumFontFamExProc function is an application defined-callback
	//     function used with the EnumFontFamiliesEx function. It is used to process
	//     the fonts and is called once for each enumerated font. The FONTENUMPROC
	//     type defines a pointer to this callback function. EnumFontFamExProc
	//     is a placeholder for the application defined-function name.
	// Parameters:
	//     pelf       - Pointer to an ENUMLOGFONTEX structure that contains
	//                  information about the logical attributes of the font.
	//     lpntm      - Pointer to a structure that contains information
	//                  about the physical attributes of a font. The function
	//                  uses the NEWTEXTMETRICEX structure for TrueType fonts
	//                  and the TEXTMETRIC structure for other fonts.
	//     dwFontType - Specifies the type of the font. This parameter can be a
	//                  combination of the values in the Remarks section.
	//     lParam - Specifies the application-defined data passed by the
	//              EnumFontFamiliesEx function.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//          * <b>DEVICE_FONTTYPE</b>: The font is a device font.
	//          * <b>RASTER_FONTTYPE</b>: The font is a raster font.
	//          * <b>TRUETYPE_FONTTYPE</b>: The font is a TrueType font.
	// Returns:
	//     To continue enumeration, the return value must be a nonzero value.
	//     To stop enumeration, the return value must be zero.
	//-----------------------------------------------------------------------
	static BOOL CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* lpntm,
										   DWORD dwFontType, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the font enumeration callback to
	//     add a font to the font list.
	// Parameters:
	//     pLF    - Pointer to a valid LOGFONT structure.
	//     dwType - Specifies the type of the font. This parameter can be a combination
	//              of the values in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//          * <b>DEVICE_FONTTYPE</b>: The font is a device font.
	//          * <b>RASTER_FONTTYPE</b>: The font is a raster font.
	//          * <b>TRUETYPE_FONTTYPE</b>: The font is a TrueType font.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	bool AddFont(const LOGFONT* pLF, DWORD dwType);

protected:
	CXTPFontList m_listFonts; // List of fonts found during enumeration.
	CDC* m_pDC;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPFontList& CXTPFontEnum::GetFontList()
{
	return m_listFonts;
}

// -------------------------------------------------------------------
// Summary:
//     Enumeration used to determine font display.
// Remarks:
//     XTPFontStyle type defines the constants used by the CXTPFontListBox class to
//     determine how the fonts will be displayed in the CXTPFontListBox control.
// See Also:
//     CXTPFontListBox, CXTPFontListBox::SetListStyle
//
// <KEYWORDS xtpFontGUI, xtpFontSample, xtpFontBoth>
// -------------------------------------------------------------------
enum XTPFontStyle
{
	xtpFontUnknown = 0x00, // Display type not defined.
	xtpFontGUI	 = 0x01, // Display font name with GUI font style.
	xtpFontSample  = 0x02, // Display font name with its own font style.
	xtpFontBoth	= 0x03, // Display font name with GUI font style, then a sample display to the
						   // right.
};

class CXTPFontListBox;
class CXTPFontComboBox;

//===========================================================================
// Summary:
//     CXTPFontCtrlBase is a base class used by CXTPFontListBox and
//     CXTPFontComboBox controls to perform common drawing routines.
//===========================================================================
class _XTP_EXT_CLASS CXTPFontCtrlBase
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Standard constructor.
	//-----------------------------------------------------------------------
	CXTPFontCtrlBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Handles object destruction.
	//-----------------------------------------------------------------------
	virtual ~CXTPFontCtrlBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the default height for
	//     combo or list items.
	// Returns:
	//     The default height, in pixels, for combo or list items.
	//-----------------------------------------------------------------------
	static int AFX_CDECL GetFontItemHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create a default font for a
	//     combo or list box.
	// Parameters:
	//     font - Reference to a CFont object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL CreateUIFont(CFont& font);

	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the font for the currently
	//     selected item.
	// Parameters:
	//     lf          - Reference to a CXTPLogFont structure.
	//     strFaceName - A reference to a valid CString object to receive
	//                   the logfont face name.
	// Remarks:
	//     The first version of GetSelFont will retrieve the selected font
	//     name. The second version will get the CXTPLogFont for the
	//     currently selected item.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// ------------------------------------------------------------------------
	virtual BOOL GetSelFont(CXTPLogFont& lf);
	virtual BOOL GetSelFont(CString& strFaceName); // <combine
												   // CXTPFontCtrlBase::GetSelFont@CXTPLogFont&>

	// -------------------------------------------------------------------
	// Summary:
	//     Call this member function to select the font for the list box.
	// Parameters:
	//     lf          - Reference to a CXTPLogFont structure.
	//     strFaceName - A reference to a valid CString object to receive
	//                   the logfont face name.
	// Remarks:
	//     The first version of SetSelFont will set the selected font by
	//     using its face name. The second version will set the
	//     selected font by using a CXTPLogFont object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// -------------------------------------------------------------------
	virtual BOOL SetSelFont(CXTPLogFont& lf);
	virtual BOOL SetSelFont(
		const CString& strFaceName); // <combine CXTPFontCtrlBase::SetSelFont@CXTPLogFont&>

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the framework when a visual aspect
	//     of an owner-drawn combo or list box changes.
	// Parameters:
	//     lpDIS - A pointer to a DRAWITEMSTRUCT structure that contains
	//             information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void OnDrawItem(LPDRAWITEMSTRUCT lpDIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by the framework when a combo or list
	//     box with an owner-drawn style is created.
	// Parameters:
	//     lpMIS - A long pointer to a MEASUREITEMSTRUCT structure.
	//-----------------------------------------------------------------------
	virtual void OnMeasureItem(LPMEASUREITEMSTRUCT lpMIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to determine the relative position
	//     of a new item in the list-box portion of a sorted owner-draw
	//     combobox. By default, this member function does nothing.
	// Parameters:
	//     lpCIS - A long pointer to a COMPAREITEMSTRUCT structure.
	// Returns:
	//     Indicates the relative position of the two items described
	//     in the COMPAREITEMSTRUCT structure. It can be any of the
	//     following values:
	//
	//     Value Meaning:
	//     * -1: Item 1 sorts before item 2.
	//     *  0: Item 1 and item 2 sort the same.
	//     *  1: Item 1 sorts after item 2.
	//-----------------------------------------------------------------------
	virtual int OnCompareItem(LPCOMPAREITEMSTRUCT lpCIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the list or combobox pointer
	//     for the base class.
	// Parameters:
	//     pListBox  - Pointer to a valid CXTPFontListBox object.
	//     pComboBox - Pointer to a valid CXTPFontComboBox object.
	//-----------------------------------------------------------------------
	virtual void OnInit(CXTPFontListBox* pListBox);
	virtual void OnInit(CXTPFontComboBox* pComboBox); // <combine
													  // CXTPFontCtrlBase::OnInit@CXTPFontListBox*>

protected:
	DWORD m_dwStyle;			   // Display style indicating how to render the font list.
	CString m_csSymbol;			   // String displayed for the symbol characters.
	CXTPFontListBox* m_pListBox;   // Pointer to the owner list box.
	CXTPFontComboBox* m_pComboBox; // Pointer to the owner combobox.

	enum
	{
		FontTypeWidth = 16
	};
};

//===========================================================================
// Summary:
//     CXTPFontListBox is a CXTPListBox derived class. It is used to create
//     a font selection list box.
// Remarks:
//     You can choose to display the font name with the GUI font style, display
//     the font name with its own font style, or display the font name with
//     the default GUI font style and a sample display to the right.
//===========================================================================
class _XTP_EXT_CLASS CXTPFontListBox
	: public CXTPListBox
	, public CXTPFontCtrlBase
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFontListBox object.
	//-----------------------------------------------------------------------
	CXTPFontListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFontListBox object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFontListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font display style for the font
	//     list box. There are three styles to choose from that include displaying
	//     the font in the default GUI font, displaying the font in its own font
	//     style, or displaying both the font name in the default GUI font and
	//     a sample to the right.
	// Parameters:
	//     dwStyle - Specifies the style for the font list box. See XTPFontStyle for
	//               a list of available styles.
	// See Also:
	//     XTPFontStyle
	//-----------------------------------------------------------------------
	void SetListStyle(DWORD dwStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the font list box and populate it
	//     with a list of available fonts. This method should be called directly
	//     after creating or sub-classing the control.
	// Parameters:
	//     bAutoFont - true to enable automatic font initialization.
	//-----------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont = true);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPFontListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPFontListBox)
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPFontListBox::SetListStyle(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

//===========================================================================
// Summary:
//     CXTPFontComboBox is a CXTPComboBox derived class. It is used to create
//     a combobox that displays a drop list of available fonts for your system.
//     The fonts are displayed in their various styles.
//===========================================================================
class _XTP_EXT_CLASS CXTPFontComboBox
	: public CXTPComboBox
	, public CXTPFontCtrlBase
{
	DECLARE_DYNAMIC(CXTPFontComboBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFontComboBox object.
	//-----------------------------------------------------------------------
	CXTPFontComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFontComboBox object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFontComboBox();

public:
	// --------------------------------------------------------------------------
	// Summary:
	//     Sets the font style for the CXTPFontComboBox control.
	// Parameters:
	//     dwStyle - Specifies the style for the font list box. Styles can be any
	//               one of the values in the Remarks section.
	// Remarks:
	//     Call this member function to set the font display style for the
	//     font list box. There are three styles to choose from that include
	//     displaying the font in the default GUI font, displaying the font
	//     in its own font style, or displaying both the font name in the
	//     default GUI font and a sample to the right.
	//
	//     The style can be any of the following values:
	//
	//     * <b>xtpFontGUI</b>: Display font name with GUI font style.
	//     * <b>xtpFontSample</b>: Display font name with its own font style.
	//     * <b>xtpFontBoth</b>: Display font name with GUI font style,
	//                           then a sample display to the right.
	// --------------------------------------------------------------------------
	void SetListStyle(DWORD dwStyle);

	// ------------------------------------------------------------------------
	// Summary:
	//     Initializes the CXTPFontComboBox control.
	// Parameters:
	//     lpszFaceName -  A NULL-terminated string that represents the logfont
	//                     face name.
	//     nWidth       -  The minimum allowable width of the list box portion of
	//                     the combobox in pixels.
	//     bEnable      -  TRUE to enable auto-completion, otherwise FALSE.
	// Remarks:
	//     Call this member function to initialize the font list box and
	//     populate it with a list of available fonts.
	// ------------------------------------------------------------------------
	virtual void InitControl(LPCTSTR lpszFaceName = NULL, UINT nWidth = 0, BOOL bEnable = TRUE);

protected:
	// ------------------------------------------------------------------------
	// Summary:
	//      Called by the font combobox to send CBN notifications to the owner
	//      window to support keyboard navigation.
	// Parameters:
	//      nCode - CBN notification message to send.
	// ------------------------------------------------------------------------
	virtual void NotifyOwner(UINT nCode);

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPFontComboBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPFontComboBox)
	afx_msg BOOL OnDropDown();
	afx_msg BOOL OnCloseUp();
	//}}AFX_MSG
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
public:
#	ifdef _DEBUG
	virtual void AssertValid() const;
#	endif
	//}}AFX_CODEJOCK_PRIVATE

private:
	CString m_csSelected; // Selected text set when CBN_DROPDOWN is called.
	BOOL m_bIsOpen;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPFontComboBox::SetListStyle(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTPFONTCOMBOBOX_H__)
