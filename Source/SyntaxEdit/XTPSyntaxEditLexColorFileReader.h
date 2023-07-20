// XTPSyntaxEditLexColorFileReader.h: interface for the CXTLexColorFileReader class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITLEXCOLORFILEREADER_H__)
#	define __XTPSYNTAXEDITLEXCOLORFILEREADER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditColorTheme;
class CXTPNotifyConnection;
class CXTPNotifyConnectionMT;
struct XTP_EDIT_SCHEMAFILEINFO;
class CXTPSyntaxEditSchemaFileInfoList;

//-----------------------------------------------------------------------
// Summary:
//     This enumeration describes allowable configuration managers flags.
//-----------------------------------------------------------------------
enum XTPSyntaxEditCfgFlags
{
	xtpEditCfgFileAdd	= 0x0001, // File should be added.
	xtpEditCfgFileRemove = 0x0002, // File should be removed.
};

//===========================================================================
// Summary:
//     This storage class is designed to contain information from the color
//     settings file section. It helps to easily retrieve color information.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorInfo
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Object constructor.
	// Parameters:
	//     pTheme       - [in] Pointer to a CXTPSyntaxEditColorTheme object
	//                         (theme that this color information belongs to).
	//     strClassName - [in] Reference to the lex class name.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorInfo(CXTPSyntaxEditColorTheme* pTheme);
	CXTPSyntaxEditColorInfo(
		const CString& strClassName,
		CXTPSyntaxEditColorTheme*
			pTheme); // <combine
					 // CXTPSyntaxEditColorInfo::CXTPSyntaxEditColorInfo@CXTPSyntaxEditColorTheme*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditColorTheme() object, handles cleanup and
	//     de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorInfo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a parameter.
	// Parameters:
	//     strName  - [in] Reference to the parameter name.
	//     strValue - [in] Reference to the parameter value.
	//-----------------------------------------------------------------------
	void AddParam(const CString& strName, const CString& strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the value of a parameter with a specified name.
	// Parameters:
	//     strName  - [in] Name of the parameter to retrieve the value of.
	//     bDynamic - [in] TRUE to search for the value of the parameter
	//                     within parent themes (dynamically),
	//                     FALSE to only search for the value of the parameter
	//                     within the current theme.
	// Returns:
	//     The value of the parameter with the specified name.
	//-----------------------------------------------------------------------
	const CString GetParam(const CString& strName, BOOL bDynamic = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the hexadecimal value of a parameter with a specified name.
	// Parameters:
	//     strName  - [in] Name of the parameter to retrieve the value of.
	//     bDynamic - [in] TRUE to search for the value of the parameter
	//                     within parent themes (dynamically),
	//                     FALSE to only search for the value of the parameter
	//                     within the current theme.
	// Returns:
	//     The hexadecimal value of the parameter with the specified name.
	//-----------------------------------------------------------------------
	DWORD GetHexParam(const CString& strName, BOOL bDynamic = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the first parameter name position.
	// Returns:
	//     The first parameter name position.
	//-----------------------------------------------------------------------
	POSITION GetFirstParamNamePosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the next parameter name.
	// Parameters:
	//     pos - [in] Reference to a POSITION value.
	// Returns:
	//     The next parameter name.
	//-----------------------------------------------------------------------
	const CString GetNextParamName(POSITION& pos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the lex class name.
	// Returns:
	//     The lex class name.
	//-----------------------------------------------------------------------
	const CString GetClassName() const;

private:
	CString m_strClassName;				// Lex class name.
	CMapStringToString m_mapParams;		// Name/value parameters map.
	CXTPSyntaxEditColorTheme* m_pTheme; // Pointer to the theme this color information belongs to.
};

//==============================================================================
// Summary:
//     This collection class stores the color info objects.
//==============================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorInfoArray
	: public CArray<CXTPSyntaxEditColorInfo*, CXTPSyntaxEditColorInfo*>
{
	typedef CArray<CXTPSyntaxEditColorInfo*, CXTPSyntaxEditColorInfo*> TBase;

public:
	//--------------------------------------------------------------------------
	// Summary:
	//     Constructor, creates and initializes the object.
	//--------------------------------------------------------------------------
	CXTPSyntaxEditColorInfoArray(){};

	//--------------------------------------------------------------------------
	// Summary:
	//     Destructor, do the cleaups and destroys the object.
	//--------------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorInfoArray()
	{
		RemoveAll();
	};

	//--------------------------------------------------------------------------
	// Summary:
	//     Removes all of the color info objects.
	//--------------------------------------------------------------------------
	void RemoveAll()
	{
		for (int i = 0; i < GetSize(); i++)
		{
			if (GetAt(i))
			{
				delete GetAt(i);
			}
		}
		TBase::RemoveAll();
	}
};

class CXTPSyntaxEditColorThemesManager;
class CXTPSyntaxEditFileChangesMonitor;
class CXTPSyntaxEditPropertiesTheme;

//==============================================================================
// Summary:
//     This class provides the functionality to read configuration information
//     from configuration files with a predefined structure. Its information
//     describes the color settings for the corresponding lexical items of
//     a given language.
//     You construct an instance of CXTPSyntaxEditColorTheme by calling its
//     constructor. Then you should call the Load member function
//     to read data from persistent storage.
//==============================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorTheme
{
	friend class CXTPSyntaxEditPropertiesTheme;

	//--------------------------------------------------------------------------
	// Summary:
	//     Defines a custom array type to store CXTPSyntaxEditColorInfo structures.
	// See Also:
	//     CXTPSyntaxEditColorInfo.
	//--------------------------------------------------------------------------
	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditColorInfo*, CXTPSyntaxEditColorInfo*>
		CXTPSyntaxEditMapLexColorInfo;

public:
	//--------------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pThemesManager - [in] Pointer to the parent themes manager.
	//--------------------------------------------------------------------------
	CXTPSyntaxEditColorTheme(CXTPSyntaxEditColorThemesManager* pThemesManager);

	//--------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditColorTheme() object, handles cleanup and
	//     de-allocation.
	//--------------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorTheme();

	//--------------------------------------------------------------------------
	// Summary:
	//     Parses the associated color schema file and fills
	//     internal color information structures.
	// Parameters:
	//     csFileName - [in] Reference to the configuration file name.
	//--------------------------------------------------------------------------
	virtual void Load(const CString& csFileName);

	//--------------------------------------------------------------------------
	// Summary:
	//     Loads a color scheme.
	// Parameters:
	//     arPassedString - [in] Reference to an array of color scheme attributes.
	//--------------------------------------------------------------------------
	virtual void LoadFromString(CStringArray& arPassedString);

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the name of the parent schema.
	// Returns:
	//     The name of the parent schema.
	//--------------------------------------------------------------------------
	const CString GetParentThemeName();

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the parent schema.
	// Returns:
	//     A pointer to the parent schema if it exists, otherwise NULL.
	//--------------------------------------------------------------------------
	CXTPSyntaxEditColorTheme* GetParentTheme();

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the color information.
	// Parameters:
	//     strLexClass      - [in] Reference to the lex class name.
	//     strThemeFilename - [in] Reference to the configuration file name.
	//     bDynamic         - [in] TRUE to search for the color information
	//                             within parent themes (dynamically),
	//                             FALSE to only search for the color information
	//                             within the current theme.
	// Remarks:
	//     strThemeFilename is required in order to avoid calling recursion.
	// Returns:
	//     A pointer to the color information.
	//--------------------------------------------------------------------------
	CXTPSyntaxEditColorInfo* GetColorInfo(const CString& strLexClass,
										  const CString& strThemeFilename, BOOL bDynamic = TRUE);

	//--------------------------------------------------------------------------
	// Summary:
	//     Gets the configuration file name.
	// Returns:
	//     The configuration file name.
	//--------------------------------------------------------------------------
	CString GetFileName();

	//--------------------------------------------------------------------------
	// Summary:
	//     Saves colors to the configuration file.
	// Parameters:
	//     strParentThemeName  - Parent theme name.
	//     pColorInfoArray_new - Pointer to the colors information array.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetFileName
	//--------------------------------------------------------------------------
	BOOL WriteCfgFile(CString strParentThemeName,
					  CXTPSyntaxEditColorInfoArray* pColorInfoArray_new);

protected:
	//--------------------------------------------------------------------------
	// Summary:
	//     Cleans up the collection.
	//--------------------------------------------------------------------------
	void Cleanup();

	//--------------------------------------------------------------------------
	// Summary:
	//     Parses a single section of the configuration file and
	//     creates a CXTPSyntaxEditColorInfo object from it.
	// Parameters:
	//     csSection      - [in] Reference to the section name
	//                           (in the configuration file).
	//     arPassedString - [in] Reference to a CStringArray object.
	// Returns:
	//     A pointer to the newly created CXTPSyntaxEditColorInfo object.
	//--------------------------------------------------------------------------
	CXTPSyntaxEditColorInfo* ParseSection(const CString& csSection);
	CXTPSyntaxEditColorInfo* ParseSectionFromString(
		const CString& csSection,
		CStringArray& arPassedString); // < combine
									   // CXTPSyntaxEditColorTheme::ParseSection@const
									   // CString&>

private:
	CStringList m_arSections; // All section names from the theme configuration file.
	CString m_csFileName;	 // Filename of the corresponding theme configuration file.
	CXTPSyntaxEditMapLexColorInfo m_mapLexColorInfo; // Internal map - lex class name to color info.
	CXTPSyntaxEditColorThemesManager* m_pThemesManager; // Pointer to the parent themes manager.
};

AFX_INLINE CString CXTPSyntaxEditColorTheme::GetFileName()
{
	return m_csFileName;
}

//===========================================================================
// Summary:
//     This class provides the functionality to store the set of
//     all color themes that would be written from configuration files.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditColorThemesManager
{
	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditColorTheme*, CXTPSyntaxEditColorTheme*>
		CXTPSyntaxEditMapThemes;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorThemesManager();
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditColorThemesManager() object,
	//     handles cleanup and de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditColorThemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a theme.
	// Parameters:
	//     strThemeName     - [in] Reference to the theme name.
	//     strThemeFilename - [in] Reference to the configuration file name.
	//     arPassedString   - [in] Reference to an array of attribute strings.
	//-----------------------------------------------------------------------
	void LoadTheme(const CString& strThemeName, const CString& strThemeFilename);
	void LoadThemeFromString(
		const CString& strThemeName,
		CStringArray& arPassedString); // <combine
									   // CXTPSyntaxEditColorThemesManager::LoadTheme@const
									   // CString&@const CString&>

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new entity to the "theme name - configuration file" map.
	// Parameters:
	//     strThemeName     - [in] Reference to the theme name.
	//     strThemeFilename - [in] Reference to the configuration file name.
	//-----------------------------------------------------------------------
	void AddThemeInfo(const CString& strThemeName, const CString& strThemeFilename);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color theme with a specified name.
	// Parameters:
	//     strThemeName - [in] Reference to the theme name.
	// Returns:
	//     A pointer to the color theme with the specified name.
	// See Also:
	//     CXTPSyntaxEditColorTheme
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorTheme* GetTheme(const CString& strThemeName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets all theme names.
	// Returns:
	//     A reference to the CStringArray object containing all themes names.
	//-----------------------------------------------------------------------
	CStringArray& GetThemes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reloads configuration settings from a specified configuration file.
	// Parameters:
	//     csFileName - [in] Reference to the configuration file name.
	//     nCfgFlags  - [in] Action flags.
	// Returns:
	//     The reloaded theme name if found, otherwise an empty string.
	// Remarks:
	//     Does nothing if the specified file is not found in
	//     the collection of files from the main configuration file.
	// See Also:
	//     XTPSyntaxEditCfgFileRemove
	//-----------------------------------------------------------------------
	CString ReloadFile(const CString& csFileName, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all loaded data.
	//-----------------------------------------------------------------------
	void RemoveAll();

private:
	CXTPSyntaxEditMapThemes m_mapThemes; // Themes map.
	CStringArray m_arThemeNames;		 // Themes names array.
	CMapStringToString m_mapFileToTheme; // Theme name to configuration file name map.
};

typedef XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema CXTPSyntaxEditTextSchema;
typedef CXTPSmartPtrInternalT<XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema>
	CXTPSyntaxEditLexTextSchemaPtr;
typedef CXTPSyntaxEditLexTextSchemaPtr CXTPSyntaxEditTextSchemaPtr;

//===========================================================================
// Summary:
//     This class provides the functionality to store the set of
//     all text schemes that would be written from configuration files.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTextSchemesManager
{
	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditTextSchemaPtr, CXTPSyntaxEditTextSchemaPtr&>
		CXTPSyntaxEditMapSchemes;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditTextSchemesManager() object,
	//     handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds text scheme information.
	// Parameters:
	//     info - [in] Reference to the text scheme information.
	//-----------------------------------------------------------------------
	void AddTextSchemeInfo(XTP_EDIT_SCHEMAFILEINFO& info);

	//-----------------------------------------------------------------------
	// Summary:
	//     Implements common logic on creating new text scheme object and
	//     loading it from file or resources.
	// Parameters:
	//     info - [in] Reference to schema information.
	//-----------------------------------------------------------------------
	void LoadTextScheme(XTP_EDIT_SCHEMAFILEINFO& info);

	//-----------------------------------------------------------------------
	// Summary:
	//     Implements common logic on creating new text scheme object and
	//     loading it string - used for non-file configuration mode.
	// Parameters:
	//     info            - [in] Reference to schema information.
	//     p_sSyntaxScheme - Pointer to syntax scheme string.
	//-----------------------------------------------------------------------
	void LoadTextStringScheme(XTP_EDIT_SCHEMAFILEINFO& info, CString* p_sSyntaxScheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a schema by its name.
	// Parameters:
	//     strSchemeName - [in] Reference to the schema name.
	// Returns:
	//     A CXTPSyntaxEditTextSchemaPtr object.
	// See Also:
	//     CXTPSyntaxEditTextSchemaPtr
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemaPtr GetSchema(const CString& strSchemeName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a schema name by its file name.
	// Parameters:
	//     csFileName - [in] Reference to the file name.
	// Returns:
	//     The schema name.
	//-----------------------------------------------------------------------
	CString GetSchemaName(const CString& csFileName) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a file name by its schema name.
	// Parameters:
	//     strSchemeName - [in] Reference to the schema name.
	// Returns:
	//     The file name.
	//-----------------------------------------------------------------------
	CString GetSchemaFileName(const CString& strSchemeName) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets all scheme names.
	// Returns:
	//     A reference to the CStringArray object containing all scheme names.
	//-----------------------------------------------------------------------
	CStringArray& GetSchemes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the schema list.
	// Returns:
	//     A pointer to a CXTPSyntaxEditSchemaFileInfoList object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditSchemaFileInfoList* GetSchemaList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a schema by its file extension.
	// Parameters:
	//     strFileExt - [in] Reference to the file extension string.
	// Returns:
	//     A CXTPSyntaxEditTextSchemaPtr object.
	// See also:
	//     CXTPSyntaxEditTextSchemaPtr
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemaPtr FindSchema(const CString& strFileExt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the theme for all text schemes.
	// Parameters:
	//     pTheme - [in] Pointer to the theme to be set.
	// See Also:
	//     CXTPSyntaxEditColorTheme
	//-----------------------------------------------------------------------
	void SetTheme(CXTPSyntaxEditColorTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reloads configuration settings from a specified configuration file.
	// Parameters:
	//     csFileName   - [in] Reference to the configuration file name.
	//     nCfgFlags    - [in] Action flags.
	// Remarks:
	//     Does nothing if the specified file is not found in
	//     the collection of files from the main configuration file.
	//-----------------------------------------------------------------------
	BOOL ReloadFile(const CString& csFileName, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all loaded data.
	//-----------------------------------------------------------------------
	void RemoveAll();

private:
	CXTPSyntaxEditMapSchemes m_mapSchemes;
	CXTPSyntaxEditSchemaFileInfoList* m_plistSchemes;
	CStringArray m_arSchemeNames;
	CCriticalSection m_csReadFile;
};

AFX_INLINE CStringArray& CXTPSyntaxEditTextSchemesManager::GetSchemes()
{
	return m_arSchemeNames;
}

AFX_INLINE CXTPSyntaxEditSchemaFileInfoList* CXTPSyntaxEditTextSchemesManager::GetSchemaList() const
{
	return m_plistSchemes;
}

//===========================================================================
// Summary:
//     This class manages all configuration parameters of the control.
//     It worries about read configuration files from the disk when
//     the Edit control is starting, stores it in internal structures
//     and refreshes parameters in case the configuration files are
//     changed after the control was started.
// See Also:
//     CXTPSyntaxEditFileChangesMonitor, CXTPSyntaxEditColorThemesManager
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditConfigurationManager : public CXTPCmdTarget
{
public:
	// Configuration manager sub-objects
	enum XTPSyntaxEditCfgObjects
	{
		xtpEditCfgObjSchMan		= 0x0001, // Schemes manager.
		xtpEditCfgObjThemeMan   = 0x0002, // Themes manager.
		xtpEditCfgObjMainConfig = 0x0004, // Main configuration file.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditConfigurationManager() object,
	//     handles cleanup and de-allocation
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditConfigurationManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reloads settings from a specified file.
	// Parameters:
	//     strConfigFilePath - Reference to the string of the file's full path.
	// Remarks:
	//     Reloads configuration settings from the specified new configuration file.
	//-----------------------------------------------------------------------
	void ReloadConfig(const CString& strConfigFilePath);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reloads configuration settings from the stored main configuration file.
	//-----------------------------------------------------------------------
	void ReloadConfig();

	//-----------------------------------------------------------------------
	// Summary:
	//     Asynchronously reloads configuration settings from
	//     the stored main configuration file.
	// Parameters:
	//     nAsyncThreadPriority - Priority for the working thread which
	//                            reloads the configuration.
	//-----------------------------------------------------------------------
	void ReloadConfigAsync(int nAsyncThreadPriority = THREAD_PRIORITY_NORMAL);

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the name of the main configuration file.
	// Returns:
	//     The name of the main configuration file.
	// See Also:
	//     CXTPSyntaxEditConfigurationManager::ReloadConfig
	//--------------------------------------------------------------------
	CString GetConfigFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reloads configuration settings from a specified configuration file.
	// Parameters:
	//     csFileName   - [in] Reference to the configuration file name.
	//     dwOwnerFlags - [in] Owner flags.
	//     nCfgFlags    - [in] Action flags.
	// Remarks:
	//     Does nothing if the specified file is not found in
	//     the collection of files from the main configuration file.
	// See Also:
	//     XTPSyntaxEditCfgFileRemove
	//-----------------------------------------------------------------------
	void ReloadFile(CString csFileName, DWORD dwOwnerFlags, int nCfgFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the color theme manager.
	// Returns:
	//     A reference to the color theme manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditColorThemesManager& GetThemeManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text scheme manager.
	// Returns:
	//     A reference to the text scheme manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchemesManager& GetTextSchemesManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the current theme name.
	// Parameters:
	//     strThemeName - [in] Reference to the theme name.
	//      pActiveSch  - [in] Pointer to the active schema to apply the current theme.
	//-----------------------------------------------------------------------
	void SetTheme(const CString& strThemeName, CXTPSyntaxEditTextSchema* pActiveSch = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Closes the configuration monitor session.
	//-----------------------------------------------------------------------
	void Close();
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current theme name.
	// Returns:
	//     The current theme name.
	//-----------------------------------------------------------------------
	CString GetCurrentTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the notification connection.
	// Returns:
	//     A pointer to the notification connection.
	//-----------------------------------------------------------------------
	CXTPNotifyConnection* GetConnection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets the syntax and color scheme.
	// Parameters:
	//      pSyntaxScheme - Syntax scheme.
	//      pColorScheme  - Color scheme.
	//-----------------------------------------------------------------------
	void SetSyntaxAndColorScheme(CString* pSyntaxScheme, CString* pColorScheme)
	{
		p_sSyntaxScheme = pSyntaxScheme;
		p_sColorScheme  = pColorScheme;
	}
	BOOL m_bUseMonitor;		// TRUE if the control uses a special thread to synchronize syntax rules
							// with files with Syntax and Color schemes, FALSE if the control works
							// "off-line".
	BOOL m_bConfigFileMode; // TRUE if the control reads Config file with links to detailed
							// setting (Syntax and Color schemes), FALSE if Syntax and
							// Color schemes are passed as strings using special functions.
	CString m_sIniSet;		// The ini file settings.
	CString* p_sSyntaxScheme; // Pointer to the syntax scheme.
	CString* p_sColorScheme;  // Pointer to the color scheme.

private:
	static UINT AFX_CDECL ReloadConfigAsync_Proc(LPVOID pThis);

	CString m_strMainIniFilename; // Main configuration file name.

	CXTPSyntaxEditFileChangesMonitor* m_pFolderMonitor;	// Pointer to the folder monitor object.
	CXTPSyntaxEditColorThemesManager m_ColorThemeManager;  // Color theme manager.
	CXTPSyntaxEditTextSchemesManager m_TextSchemesManager; // Text theme manager.

	CString m_strCurrentThemeName; // Current theme name.

	CXTPNotifyConnectionMT* m_pConnectMT; // Notification connection.
	CCriticalSection m_DataLockerCS;	  // Critical section.

	HANDLE m_hReloadThread;		 // Handle of monitoring thread.
	CEvent* m_pBreakReloadEvent; // Pointer to event.
};

AFX_INLINE const CString CXTPSyntaxEditColorInfo::GetClassName() const
{
	return m_strClassName;
}

AFX_INLINE CXTPSyntaxEditColorThemesManager& CXTPSyntaxEditConfigurationManager::GetThemeManager()
{
	return m_ColorThemeManager;
}

AFX_INLINE CXTPSyntaxEditTextSchemesManager&
	CXTPSyntaxEditConfigurationManager::GetTextSchemesManager()
{
	return m_TextSchemesManager;
}

AFX_INLINE CString CXTPSyntaxEditConfigurationManager::GetCurrentTheme() const
{
	return m_strCurrentThemeName;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLEXCOLORFILEREADER_H__)
