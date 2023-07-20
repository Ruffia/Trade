// XTPShortcutManager.h : interface for the CXTPShortcutManager class.
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
#if !defined(__XTPSHORTCUTMANAGER_H__)
#	define __XTPSHORTCUTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPPropExchange;
class CXTPCommandBars;
class CXTPShortcutManager;

struct XTP_SHORTCUTMANAGER_ACCEL
{
	struct SHORTCUTACCEL
	{
		BYTE fVirt; // Specifies the accelerator behavior.
		WORD key;   // Specifies the accelerator key.
	};

	int cmd;			  // Specifies the accelerator identifier.
	SHORTCUTACCEL key[2]; // Double key combination.
};

//===========================================================================
// Summary:
//     CXTPShortcutManagerAccel represents an accelerator key used
//     in an accelerator table.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManagerAccel
	: public CXTPCmdTarget
	, public XTP_SHORTCUTMANAGER_ACCEL
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutManagerAccel object.
	//-------------------------------------------------------------------------
	CXTPShortcutManagerAccel();
	CXTPShortcutManagerAccel(ACCEL* pAccel);
	CXTPShortcutManagerAccel(const XTP_SHORTCUTMANAGER_ACCEL& accel);
	CXTPShortcutManagerAccel(const CXTPShortcutManagerAccel& accel);

public:
	const CXTPShortcutManagerAccel& operator=(const CXTPShortcutManagerAccel& accel);

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
protected:
	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPShortcutManagerAccel);
	DECLARE_INTERFACE_MAP();
	BSTR OleGetShortcutText();
	void OleSetShortcutText(LPCTSTR lpszShortcut);
	//}}AFX_CODEJOCK_PRIVATE
#	endif

public:
	CXTPShortcutManager* m_pManager;
};

//===========================================================================
// Summary:
//     CXTPShortcutManagerAccelTable represents an accelerator table for
//     the CXTPShortcutManager class.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManagerAccelTable
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutManagerAccelTable object.
	//-------------------------------------------------------------------------
	CXTPShortcutManagerAccelTable(CXTPShortcutManager* pManager);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutManagerAccelTable object, handles
	//     cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPShortcutManagerAccelTable();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the count of accels in the table.
	// Returns:
	//     Number of accels in the table.
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves an accel by its index.
	// Parameters:
	//     nIndex - Index of the accel to retrieve.
	// Returns:
	//     Pointer to a CXTPShortcutManagerAccel object.
	//-----------------------------------------------------------------------
	CXTPShortcutManagerAccel* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new accelerator to the table.
	// Parameters:
	//     accel - Pointer to the accel to add.
	//-----------------------------------------------------------------------
	void Add(const XTP_SHORTCUTMANAGER_ACCEL& accel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create an array of all the elements in the table.
	// Returns:
	//     Pointer to a XTP_SHORTCUTMANAGER_ACCEL array.
	//-----------------------------------------------------------------------
	XTP_SHORTCUTMANAGER_ACCEL* CopyAccels() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to copy accels.
	// Parameters:
	//     hAccelTable - Windows HACCEL structure.
	//     pAccelTable - Another accel table to copy from.
	//     lpAccel - Array of ACCEL structure.
	//     nSize - Count of elements in lpAccel.
	//-----------------------------------------------------------------------
	void CopyAccelTable(HACCEL hAccelTable);
	void CopyAccelTable(CXTPShortcutManagerAccelTable*
							pAccelTable); // <combine
										  // CXTPShortcutManagerAccelTable::CopyAccelTable@HACCEL>
	void CopyAccelTable(LPACCEL lpAccel,
						int nSize); // <combine
									// CXTPShortcutManagerAccelTable::CopyAccelTable@HACCEL>

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all elements in the table.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove an accel by its index.
	// Parameters:
	//     nIndex - Index of the accel to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

protected:
	CArray<CXTPShortcutManagerAccel*, CXTPShortcutManagerAccel*> m_arrAccels; // Array of accels.
	CXTPShortcutManager* m_pManager;										  // Parent manager.
};

//===========================================================================
// Summary:
//     CXTPShortcutManager is a standalone class used to manipulate accelerators of a frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManager : public CXTPCmdTarget
{
public:
	//{{AFX_CODEJOCK_PRIVATE
	class _XTP_EXT_CLASS CKeyNameText
	{
	public:
		CKeyNameText();
		CString Translate(UINT nKey);
		void SetAt(UINT uiVirtKey, LPCTSTR lpszKeyNameText);
		WORD Parse(LPCTSTR lpszKey);

	protected:
		CMap<WORD, WORD, CString, CString> m_mapVirtualKeys;
	};

	class _XTP_EXT_CLASS CKeyHelper
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyHelper object.
		//-----------------------------------------------------------------------
		CKeyHelper(const CXTPShortcutManagerAccel* lpAccel, CXTPShortcutManager* pManager = NULL);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyHelper object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyHelper();

	public:
		void Format(CString& str) const;
		CString Format(const CXTPShortcutManagerAccel::SHORTCUTACCEL* pAccel) const;
		int Priority();

		static BOOL AFX_CDECL EqualAccels(const CXTPShortcutManagerAccel* pFirst,
										  const CXTPShortcutManagerAccel* pSecond)
		{
			return ((pFirst->key[0].fVirt | FNOINVERT) == (pSecond->key[0].fVirt | FNOINVERT))
				   && (pFirst->key[0].key == pSecond->key[0].key)
				   && ((pFirst->key[1].fVirt | FNOINVERT) == (pSecond->key[1].fVirt | FNOINVERT))
				   && (pFirst->key[1].key == pSecond->key[1].key);
		}

		CXTPShortcutManager* GetShortcutManager() const
		{
			return m_pManager;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     The GetKeyNameText function retrieves a string that represents the name of a key.
		// Parameters:
		//     uiVirtKey - virtual-key of accelerator.
		//-----------------------------------------------------------------------
		static CString AFX_CDECL GetLocalKeyNameText(UINT uiVirtKey);

	protected:
		void AddVirtKeyStr(CString& str, UINT uiVirtKey, BOOL bLast = FALSE) const;

	protected:
		const CXTPShortcutManagerAccel* m_lpAccel;
		CXTPShortcutManager* m_pManager;

	public:
		BOOL m_bAllowLocaleKey;
	};

#	ifdef _XTP_INCLUDE_CONTROLS
	class _XTP_EXT_CLASS CKeyAssign : public CXTPEdit
#	else
	class _XTP_EXT_CLASS CKeyAssign : public CEdit
#	endif
	{
	public:
		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyAssign object.
		//-------------------------------------------------------------------------
		CKeyAssign(CXTPShortcutManager* pManager = NULL);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyAssign object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyAssign();

	public:
		BOOL IsKeyDefined() const;
		CXTPShortcutManagerAccel* GetAccel();
		void SetAccel(CXTPShortcutManagerAccel* pAccel);
		void ResetKey();

	protected:
		void SetAccelFlag(int nKey, BYTE bFlag, BOOL bOn);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		BOOL TranslateDoubleKeyShortcutsMessage(MSG* pMsg);
		BOOL TranslateSingleKeyShortcutsMessage(MSG* pMsg);

	public:
		BOOL m_bExtendedOnly;
		BOOL m_bAllowDoubleKeyShortcuts;

	protected:
		int m_nKeyDefined;
		CXTPShortcutManagerAccel m_accel;
		CKeyHelper m_keyHelper;
	};
	//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutManager object.
	// Parameters:
	//     pCommandBars - Owner CommandBars object.
	//-------------------------------------------------------------------------
	CXTPShortcutManager(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPShortcutManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the shortcut text for a specified command.
	// Parameters:
	//     nCmd - Command with the shortcut text to retrieve.
	//     strShortcut  - Shortcut text reference.
	// Returns:
	//     TRUE if the shortcut was found.
	//-----------------------------------------------------------------------
	virtual BOOL FindDefaultAccelerator(int nCmd, CString& strShortcut);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the default shortcut for a specified command.
	// Parameters:
	//     nCmd - Command with the accel to retrieve.
	//     strShortcut - Shortcut text reference.
	// Returns:
	//     TRUE if the command was found.
	//-----------------------------------------------------------------------
	BOOL FindDefaultFrameAccelerator(int nCmd, CString& strShortcut);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve a text presentation of ACCEL.
	//     You can override this method.
	// Parameters:
	//     lpAccel - Accelerator to format.
	//     pPriority - Returns the priority of the accelerator to be used to determine
	//                 what accelerator is used as the default.
	//-----------------------------------------------------------------------
	virtual CString Format(CXTPShortcutManagerAccel* lpAccel, int* pPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function retrieves a CString object that represents the name of a key.
	//     Override this member function to provide additional functionality.
	// Parameters:
	//     uiVirtKey - Virtual-key of accelerator.
	// Returns:
	//     A CString object that represents the name of a key.
	//-----------------------------------------------------------------------
	virtual CString GetKeyNameText(UINT uiVirtKey);

	// ---------------------------------------------------------------------
	// Summary:
	//     The OnPreviewEditKey function is called to determine if a
	//     user-defined shortcut is allowed.
	// Parameters:
	//     pAccel - User pressed key.
	// Returns:
	//     TRUE if the key is allowed.
	// ---------------------------------------------------------------------
	virtual BOOL OnPreviewEditKey(CXTPShortcutManagerAccel* pAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the user adds a new shortcut.
	// Parameters:
	//     pAccel - New shortcut to add.
	// See Also:
	//     OnBeforeRemove
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeAdd(CXTPShortcutManagerAccel* pAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before the user removes a shortcut.
	// Parameters:
	//     pAccel - Shortcut to be removed.
	// See Also:
	//     OnBeforeAdd
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeRemove(CXTPShortcutManagerAccel* pAccel);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to localize the name text of a shortcut key.
	// Parameters:
	//     uiVirtKey - Virtual-key of accelerator.
	//     lpszKeyNameText - New text that will be associated with the virtual-key.
	// Example:
	//     <code>pCommandBars->GetShortcutManager()->SetKeyNameText(VK_CONTROL, _T("Strg"));</code>
	//-----------------------------------------------------------------------
	void SetKeyNameText(UINT uiVirtKey, LPCTSTR lpszKeyNameText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to use system localized shortcuts.
	// Parameters:
	//     bSystemKey - TRUE to allow the system to localize shortcuts instead
	//                  of using a predefined shortcuts table.
	// Example:
	//     <code>pCommandBars->GetShortcutManager()->UseSystemKeyNameText(TRUE);</code>
	//-----------------------------------------------------------------------
	void UseSystemKeyNameText(BOOL bSystemKey);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to disable/enable shortcuts usage.
	// Parameters:
	//     bDisable - TRUE to disable shortcuts, FALSE to enable shortcuts.
	//-----------------------------------------------------------------------
	virtual void DisableShortcuts(BOOL bDisable);

	//-----------------------------------------------------------------------
	// Summary:
	//     The TranslateAccelerator function processes accelerator keys for menu commands.
	// Parameters:
	//     lpMsg - Pointer to an MSG structure that contains message information.
	// Returns:
	//     Nonzero if the function is successful. FALSE if the function fails.
	//-----------------------------------------------------------------------
	virtual BOOL TranslateAccelerator(LPMSG lpMsg);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the accelerators to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void SaveShortcuts(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve accelerators from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Pointer to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void LoadShortcuts(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Either reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX    - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to create original accelerators.
	// Parameters:
	//     nIDResource - Resource identifier of Accelerator table.
	//----------------------------------------------------------------------
	void SetAccelerators(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the accelerator table.
	// Parameters:
	//     hAccelTable - New accelerators to be set.
	//----------------------------------------------------------------------
	void SetDefaultAccelerator(HACCEL hAccelTable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the accelerator table.
	// Returns:
	//     Accelerator table handle.
	//----------------------------------------------------------------------
	virtual CXTPShortcutManagerAccelTable* GetDefaultAccelerator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get original accelerators.
	// Parameters:
	//     Accelerator table handle.
	//----------------------------------------------------------------------
	CXTPShortcutManagerAccelTable* GetOriginalAccelerator() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to reset shortcuts.
	//----------------------------------------------------------------------
	virtual void Reset();

public:
	//{{AFX_CODEJOCK_PRIVATE
	void CreateOriginalAccelTable();

	static TCHAR AFX_CDECL ToUpper(TCHAR vkTCHAR);
	static BOOL AFX_CDECL CompareAccelKey(TCHAR chAccel, UINT wParam);
	static UINT AFX_CDECL AccelKeyToVirtualKey(TCHAR chAccel);
	static int AFX_CDECL FindAccelPos(LPCTSTR lpszString);

	void UpdateAcellTable(LPACCEL lpAccel, int nSize);
	void UpdateAcellTable(XTP_SHORTCUTMANAGER_ACCEL* lpAccel, int nSize);

	void SerializeShortcuts(CArchive& ar);
	void AddShortcut(long fVirt, long key, long cmd);
	void AddShortcut(long cmd, LPCTSTR lpszKey);
	BOOL GetShortcut(int ID, CXTPShortcutManagerAccel* pAccel);

	BOOL ParseShortcut(CString strShortcut, CXTPShortcutManagerAccel* accel);

private:
	BOOL ParseShortcut(CString strShortcutKey, BYTE& fVirt, WORD& key) const;
	BOOL ParseShortcutVirtKey(CString& strShortcutKey, int nAccel) const;
	BOOL IsAccelMessage(CXTPShortcutManagerAccel::SHORTCUTACCEL& accel, int nKeyState,
						LPMSG lpMsg) const;
	int GetAccelKeyState() const;

	//}}AFX_CODEJOCK_PRIVATE

#	ifdef _XTP_COMMANDBARS_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
protected:
	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB_EX(CXTPShortcutManager);
	DECLARE_CONNECTION_EX(CXTPShortcutManager)
	DECLARE_INTERFACE_MAP();
	DECLARE_ENUM_VARIANT(CXTPShortcutManager);

	afx_msg void OleAdd(long fVirt, long key, long cmd);
	afx_msg void OleAddShortcut(long cmd, LPCTSTR lpszShortcut);
	afx_msg BOOL OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(long Index);
	afx_msg void OleDeleteAll();
	afx_msg void OleDelete(long nID);
	afx_msg BOOL OleGetEnabled();
	afx_msg void OleSetEnahbled(BOOL Eanbled);
	afx_msg BOOL OleGetEnableCustomShortcutText();
	afx_msg void OleSetEnableCustomShortcutText(BOOL Eanbled);

public:
	afx_msg BOOL OleBeforeTranslate(long cmd);
	LPDISPATCH OleFindShortcut(long nCmd);

	enum {
		dispidAdd = 1L,
	};
//}}AFX_CODEJOCK_PRIVATE
#	endif

public:
	BOOL m_bAllowEscapeShortcut;	 // TRUE to allow escape to be used as a shortcut.
	BOOL m_bUseSystemKeyNameText;	// TRUE to use localized shortcuts.
	BOOL m_bAllowDoubleKeyShortcuts; // TRUE to allow double key shortcuts(ex: Ctrl+K, Ctrl+V).
	int m_nDisableShortcuts;		 // Number of shortcuts that have been disabled.
	BOOL m_bDisableOnCapture; // Specifies whether the commandbars will process keybindings while
							  // another control is capturing input.
	BOOL m_nEnableCustomShortcutText; // TRUE to allow adding custom text to be displayed as a
									  // shortcut.

protected:
	CXTPShortcutManagerAccelTable* m_pAccelTable;		  // Accelerator table.
	CXTPShortcutManagerAccelTable* m_pOriginalAccelTable; // Original accelerator table.
	CXTPCommandBars* m_pCommandBars;					  // Parent CommandBars pointer.
	CKeyNameText* m_pKeyNameText;						  // KeyNameText map.

	friend class CKeyHelper;
};

AFX_INLINE CXTPShortcutManagerAccelTable* CXTPShortcutManager::GetDefaultAccelerator() const
{
	return m_pAccelTable;
}
AFX_INLINE CXTPShortcutManagerAccelTable* CXTPShortcutManager::GetOriginalAccelerator() const
{
	return m_pOriginalAccelTable;
}
AFX_INLINE void CXTPShortcutManager::UseSystemKeyNameText(BOOL bSystemKey)
{
	m_bUseSystemKeyNameText = bSystemKey;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPSHORTCUTMANAGER_H__)
