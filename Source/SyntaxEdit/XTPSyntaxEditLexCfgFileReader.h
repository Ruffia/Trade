// XTPSyntaxEditLexCfgFileReader.h
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
#if !defined(__XTPSYNTAXEDITLEXCFGFILEREADER_H__)
#	define __XTPSYNTAXEDITLEXCFGFILEREADER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditLexCfgFileReader;

namespace XTPSyntaxEditLexAnalyser
{
//{{AFX_CODEJOCK_PRIVATE
//===========================================================================
// Summary:
//     XTPSyntaxEditTokenType type defines type of tokens that read by
//     CXTPSyntaxEditLexCfgFileReader class from configuration files.
// See also:
//     CXTPSyntaxEditLexCfgFileReader
//===========================================================================
enum XTPSyntaxEditTokenType
{
	xtpEditTokType_Unknown,
	xtpEditTokType_Delim,
	xtpEditTokType_Name,
	xtpEditTokType_Value,
	xtpEditTokType_Quoted,
	xtpEditTokType_Comment,
	xtpEditTokType_EOL,
	xtpEditTokType_Control
};

//===========================================================================
// Summary:
//     XTP_EDIT_LEXPROPINFO helper structure is used to group information
//     which describes one Lex Class property pair. The property pair is
//     property name and property value.
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_LEXPROPINFO
{
	XTP_EDIT_LEXPROPINFO();

	XTP_EDIT_LEXPROPINFO(const XTP_EDIT_LEXPROPINFO& rSrc);

	const XTP_EDIT_LEXPROPINFO& operator=(const XTP_EDIT_LEXPROPINFO& rSrc);

	CStringArray arPropName;  // stores property name chain
	CStringArray arPropValue; // stores value for property names with  corresponding index
	int nLine;
	int nOffset;
	int nPropertyLen;
};

//===========================================================================
// Summary:
//      CXTPSyntaxEditLexPropInfoArray type defines custom array type based on
//      CArray to store XTP_EDIT_LEXPROPINFO structures.
// See also:
//      XTP_EDIT_LEXPROPINFO
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexPropInfoArray
	: public CArray<XTP_EDIT_LEXPROPINFO, XTP_EDIT_LEXPROPINFO&>
{
public:
	CXTPSyntaxEditLexPropInfoArray();

	CXTPSyntaxEditLexPropInfoArray(const CXTPSyntaxEditLexPropInfoArray& rSrc);

	const CXTPSyntaxEditLexPropInfoArray& operator=(const CXTPSyntaxEditLexPropInfoArray& rSrc);
};

//===========================================================================
// Summary:
//     XTP_EDIT_LEXCLASSINFO structure is used to group information which describes
//     a set of properties for one Lex Class.
// See also:
//     XTP_EDIT_LEXPROPINFO
//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_LEXCLASSINFO
{
	XTP_EDIT_LEXCLASSINFO();

	XTP_EDIT_LEXCLASSINFO(const XTP_EDIT_LEXCLASSINFO& rSrc);

	const XTP_EDIT_LEXCLASSINFO& operator=(const XTP_EDIT_LEXCLASSINFO& rSrc);

	CString csClassName;						   // name to identify lex class
	CXTPSyntaxEditLexPropInfoArray arPropertyDesc; // array to store a set of lex class properties
	int nStartLine;
	int nEndLine;
};

//===========================================================================
// Summary:
//     Determines custom array type to store XTP_EDIT_LEXCLASSINFO structures.
// See also:
//     XTP_EDIT_LEXCLASSINFO.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexClassInfoArray
	: public CArray<XTP_EDIT_LEXCLASSINFO, XTP_EDIT_LEXCLASSINFO&>
{
public:
	CXTPSyntaxEditLexClassInfoArray();

	CXTPSyntaxEditLexClassInfoArray(const CXTPSyntaxEditLexClassInfoArray& rSrc);

	const CXTPSyntaxEditLexClassInfoArray& operator=(const CXTPSyntaxEditLexClassInfoArray& rSrc);

	BOOL m_bModified;
};
//}}AFX_CODEJOCK_PRIVATE
} // namespace XTPSyntaxEditLexAnalyser

//===========================================================================
// Summary:
//     This class provides functionality to read configuration information
//     from configuration files with a predefined structure. Its information
//     describes lex class instances that represent various lexical items
//     for given language.
//     You construct an instance of CXTPSyntaxEditLexCfgFileReader by calling
//     its constructor. Then you should call the ReadSource() member function
//     to read data from persistent storage (file for this implementation),
//     and process raw data into XTP_EDIT_LEXCLASSINFO structures. Call the
//     GetLexClassInfoArray() member function to return a pointer to an array
//     of XTP_EDIT_LEXCLASSINFO data structures.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexCfgFileReader
{
	friend class CXTPSingleton<CXTPSyntaxEditLexCfgFileReader>;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditLexCfgFileReader object, handles cleanup
	//     and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexCfgFileReader();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads data from the configuration file.
	// Parameters:
	//     csFileName - [in] Reference to a string containing the full path
	//                       to the configuration file.
	//     bSaveInfo  - [in] TRUE to save read data in the LexClassInfoMap,
	//                       FALSE otherwise.
	// Remarks:
	//     Use this member function to read configuration data from configuration
	//     file with predefined structure. Data loaded into internal data buffer.
	//-----------------------------------------------------------------------
	void ReadSource(const CString& csFileName, BOOL bSaveInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads data from resources.
	// Parameters:
	//     nResourceID - [in] Resource ID of the HTML configuration data.
	// Remarks:
	//     Use this member function to read configuration data from application
	//     resources with predefined structure. Data loaded into internal data buffer.
	//-----------------------------------------------------------------------
	void ReadSource(UINT nResourceID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads data from resources.
	// Parameters:
	//     bSaveInfo     - TRUE to save info, FALSE otherwise.
	//     sPassedString - String passed.
	//     pSyntaxScheme - Pointer to the string scheme.
	// Remarks:
	//     Used for non-file based settings.
	//     Formal file name for map is "InternalSyntaxSet"
	//-----------------------------------------------------------------------
	void ReadStringSource(BOOL bSaveInfo, CString sPassedString, CString* pSyntaxScheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the internal array.
	// Returns:
	//     A reference to the internal array.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray& GetLexClassInfoArray();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the internal map.
	// Returns:
	//     A reference to the internal map.
	//-----------------------------------------------------------------------
	CMapStringToPtr& GetLexClassInfoMap()
	{
		return m_mapLexClassInfo;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears a specified map and deletes its data.
	// Parameters:
	//     mapInfo - Reference to the map to clear.
	// See Also:
	//     CopyInfoMap
	//-----------------------------------------------------------------------
	void CleanInfoMap(CMapStringToPtr& mapInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies the internal map and its data to another map.
	// Parameters:
	//     mapInfo - [out] Reference to the map to copy to.
	// Remarks:
	//     Use CleanInfoMap to free (delete) map data.
	// See Also:
	//     CleanInfoMap
	//-----------------------------------------------------------------------
	void CopyInfoMap(CMapStringToPtr& mapInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes LexClasses to given schema configuration file
	// Parameters:
	//     csFileName      - Configuration file name.
	//     arLexClassDesc  - Array with lex class data (description) to save.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL WriteCfgFile(const CString& csFileName,
					  XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray& arLexClassDesc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes LexClasses to given schema configuration file
	// Parameters:
	//     arLexClassDesc - Array with lex class data (description) to save.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL WriteCfg2String(XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray& arLexClassDesc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts escape char sequences to corresponding char.
	// Parameters:
	//     strSrc  - [in] Text to be converted.
	//     bQuoted - [in] TRUE if the text should be interpreted as being
	//                    enclosed by single quotes (' '), FALSE otherwise.
	// Returns:
	//     The converted text.
	//-----------------------------------------------------------------------
	CString StrToES(CString strSrc, BOOL bQuoted);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts char to corresponding escape char sequences.
	// Parameters:
	//     strSrc  - [in] Text to be converted.
	//     bQuoted - [in] TRUE if the text should be interpreted as being
	//                    enclosed by single quotes (' '), FALSE otherwise.
	// Returns:
	//     The converted text.
	//-----------------------------------------------------------------------
	CString ESToStr(CString strSrc, BOOL bQuoted);

private:
	// A map describes whether property description has a flag or not.
	typedef CMap<void*, void*, bool, bool> CMapPtrToBool;
#	ifdef _DEBUG
	// Process error messages of file processing. Writes messages into debug window.
	void ProcessFileException(CFileException* pExc);
#	endif
	// Process next token from file.
	int GetToken();
	// Process next lex class token from file.
	int GetLexToken();
	// Process corresponding token
	int ProcessUnknowToken(TCHAR tchCurrSymbol);
	int ProcessDelimToken(TCHAR tchCurrSymbol);
	int ProcessNameToken(TCHAR tchCurrSymbol);
	int ProcessValueToken(TCHAR tchCurrSymbol);
	int ProcessQuotedToken(TCHAR tchCurrSymbol);
	int ProcessCommentToken(TCHAR tchCurrSymbol);
	int ProcessEOLToken(TCHAR tchCurrSymbol);
	int ProcessControlToken(TCHAR tchCurrSymbol);
	// Parses single lex class.
	void ParseLexClass(XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO& infoClass);
	// Searches for lex class.
	XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO*
		FindClassDesc(XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray& arInfoClass,
					  const CString& csClassName);
	// Searches for property of lex class.
	XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO* FindPropDesc(
		XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXCLASSINFO* pInfoClass,
		XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO* pInfoProp, CMapPtrToBool& mapUsed);
	// Read source string by string into string array.
	void ReadSource2(const CString& csFileName, CStringArray& arBuffer);
	// Writes single string into file.
	void WriteString(CFile& file, LPCTSTR pcszString);
	// Writes a number of string into file.
	void WriteStrings(CFile& file, CStringArray& arBuffer, int nFrom, int nTo);
	// Writes a whole property description into file.
	void WriteProp(CFile& file, CString& csOffset,
				   const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO& newInfoProp);
	void WriteProp(CFile& file, CString& csOffset,
				   const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO& oldInfoProp,
				   const XTPSyntaxEditLexAnalyser::XTP_EDIT_LEXPROPINFO& newInfoProp,
				   const CStringArray& arBuffer);
	// Process configuration data.
	void Parse(XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray& arLexClassDesc);

	CString* p_sSyntaxScheme;
	CString* p_sColorScheme;
	void SetSyntaxAndColorScheme(CString* pSyntaxScheme, CString* pColorScheme)
	{
		p_sSyntaxScheme = pSyntaxScheme;
		p_sColorScheme  = pColorScheme;

		m_csDataBuffer = *p_sSyntaxScheme;
	}

private:
	int m_nCurrLine;	 // Current processing line identifier.
	int m_nCurrLine_pos; // Position in current line.
	int m_nCurrPos;		 // Current position in data buffer.
	int m_nEOFPos;		 // EOF position.
	int m_nPrevPos;		 // Previous position in data buffer.
	int m_nTokenType;	// Type of processed token.
	BOOL m_bReadNames;   // Flag to read names of properties.
	BOOL m_bES;
	CString m_csDataBuffer; // Internal buffer to read file to.
	CString m_strToken;		// Text of processed token.
	CMapStringToPtr m_mapLexClassInfo;
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexClassInfoArray m_arLexClassInfo; // Lex classes
																				// array.

	// Singleton instantiation.
	CXTPSyntaxEditLexCfgFileReader();
	friend CXTPSyntaxEditLexCfgFileReader* AFX_CDECL XTPSyntaxEditLexConfig();
};

//===========================================================================
// Summary:
//     Gets the CXTPSyntaxEditLexCfgFileReader global object.
// Returns:
//     A pointer to a CXTPSyntaxEditLexCfgFileReader object.
//===========================================================================
AFX_INLINE CXTPSyntaxEditLexCfgFileReader* AFX_CDECL XTPSyntaxEditLexConfig()
{
	return &CXTPSingleton<CXTPSyntaxEditLexCfgFileReader>::Instance();
}

//===========================================================================
// Summary:
//      This class provides the ability to monitor changes in configuration files
//      and read new data from monitored files in time.
//===========================================================================
class CXTPSyntaxEditConfigurationManager;

// this class monitors files changes
class _XTP_EXT_CLASS CXTPSyntaxEditFileChangesMonitor
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditFileChangesMonitor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditFileChangesMonitor() object, handles cleanup
	//     and de-allocation. Stops monitoring if it has not stopped already.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditFileChangesMonitor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the default monitoring folder.
	// Parameters:
	//     strPath - [in] Reference to the path string.
	//-----------------------------------------------------------------------
	void SetDefaultFolder(const CString& strPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated configuration manager.
	// Parameters:
	//     pConfigMgr - [in] Pointer to the configuration manager to be set.
	// See Also:
	//     CXTPSyntaxEditConfigurationManager
	//-----------------------------------------------------------------------
	void SetConfigurationManager(CXTPSyntaxEditConfigurationManager* pConfigMgr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a file to be monitored.
	// Parameters:
	//     strFilename  - [in] Reference to the file name string.
	//     dwOwnerFlags - [in] Additional flags (from enum XTPSyntaxEditCfgObjects).
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	// Remarks:
	//     Adds a file to the bunch of files to monitor changes at. First, tries to
	//     check the file by the provided file name string, then tries the file in
	//     the default folder. Bad file names are ignored.
	// See Also:
	//     CXTPSyntaxEditConfigurationManager
	//-----------------------------------------------------------------------
	BOOL AddMonitorFile(CString& strFilename, DWORD dwOwnerFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Starts the thread which will be monitoring folder changes.
	//-----------------------------------------------------------------------
	void StartMonitoring();

	//-----------------------------------------------------------------------
	// Summary:
	//     Stops the thread which is monitoring folder changes.
	//-----------------------------------------------------------------------
	void StopMonitoring();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the contents of the folder, update file's status and
	//     re-reads changed configuration files.
	//-----------------------------------------------------------------------
	void RefreshFiles();

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all files which are monitored at the moment.
	//-----------------------------------------------------------------------
	void RemoveAll();

private:
	CString m_strPath;
	CWinThread* m_pThread;
	HANDLE m_evExitThread;
	static UINT AFX_CDECL ThreadMonitorProc(LPVOID);
	BOOL m_bES;

	CXTPSyntaxEditConfigurationManager* m_pConfigMgr; // Pointer to the associated configuration
													  // manager.

	// this class contain information about monitoring file
	class _XTP_EXT_CLASS CFMFileInfo
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Default object constructor.
		//-----------------------------------------------------------------------
		CFMFileInfo();

		CString m_strFileName; // Monitoring file name.

		BY_HANDLE_FILE_INFORMATION m_sysFileInfo; // File information.
		BOOL m_bExists;							  // TRUE if the file exists, otherwise FALSE.
		DWORD m_dwOwnerFlags;					  // Additional flags.

		//-----------------------------------------------------------------------
		// Summary:
		//     Copy operator.
		//-----------------------------------------------------------------------
		const CFMFileInfo& operator=(const CFMFileInfo& rSrc);
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the file information of a specified file.
	// Parameters:
	//     pcszFilePath - [in] Name of the file to retrieve the file information of.
	//     pInfo        - [out] Pointer to a BY_HANDLE_FILE_INFORMATION structure
	//                          to receive the file information.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetFileInfo(LPCTSTR pcszFilePath, BY_HANDLE_FILE_INFORMATION* pInfo);

	CArray<CFMFileInfo, CFMFileInfo&> m_arFiles; // Bunch of filenames to monitor changes at.
};

AFX_INLINE void CXTPSyntaxEditFileChangesMonitor::SetConfigurationManager(
	CXTPSyntaxEditConfigurationManager* pConfigMgr)
{
	m_pConfigMgr = pConfigMgr;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLEXCFGFILEREADER_H__)
