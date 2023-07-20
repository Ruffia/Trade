// XTPSyntaxEditBufferManager.h
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
#if !defined(__XTPSYNTAXEDITBUFFERMANAGER_H__)
#	define __XTPSYNTAXEDITBUFFERMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace XTPSyntaxEditLexAnalyser
{
class CXTPSyntaxEditLexTextSchema;
class CXTPSyntaxEditLexParser;
}; // namespace XTPSyntaxEditLexAnalyser

typedef XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexTextSchema CXTPSyntaxEditTextSchema;
class CXTPSyntaxEditLineMarksManager;
class CXTPSyntaxEditUndoRedoManager;
class CXTPNotifySink;
class CXTPNotifyConnection;
class CXTPSyntaxEditConfigurationManager;
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditConfigurationManager>
	CXTPSyntaxEditConfigurationManagerPtr;

//===========================================================================
// Summary:
//     This class is the main class for buffer management. One instance
//     of this class is attached with a CXTPSyntaxEditCtrl object. This class
//     contains a list of CBufferIndex classes which, in turn, contains
//     a position in the file (in case of un-modified index) or a CEditBuffer
//     pointer.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditBufferManager : public CXTPCmdTarget
{
	DECLARE_DYNCREATE(CXTPSyntaxEditBufferManager)
	friend class CXTPSyntaxEditCtrl;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditBufferManager object, handles cleanup
	//     and de-allocation.
	//-----------------------------------------------------------------------
	~CXTPSyntaxEditBufferManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the file has been modified.
	// Returns:
	//     TRUE if the file has been modified, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsModified() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CodePage for text conversion.
	// Returns:
	//     The CodePage for text conversion.
	//-----------------------------------------------------------------------
	UINT GetCodePage() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the tab size.
	// Returns:
	//     The tab size.
	//-----------------------------------------------------------------------
	int GetTabSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tab size (must be between 1-64).
	// Parameters:
	//      nTabSize   - [in] Tab size to set.
	//      bUpdateReg - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                        By default, this parameter is FALSE.
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetTabSize(int nTabSize, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the status of the overwrite flag.
	// Returns:
	//     TRUE if OVR is set, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetOverwriteFlag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the status for the overwrite flag.
	// Parameters:
	//     bOverwrite - [in] TRUE to set OVR, FALSE to set INS.
	//-----------------------------------------------------------------------
	void SetOverwriteFlag(BOOL bOverwrite);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the parser is enabled/disabled.
	// Returns:
	//     TRUE if the parser is enabled, FALSE if the parser is disabled.
	//-----------------------------------------------------------------------
	BOOL IsParserEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables the parser.
	// Parameters:
	//     bEnable - [in] TRUE to enable the parser, FALSE to disable the parser.
	//-----------------------------------------------------------------------
	void EnableParser(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CRLF style of the current document.
	// Returns:
	//     The CRLF style of the current document.
	//-----------------------------------------------------------------------
	int GetCurCRLFType() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the CRLF style for the current document.
	// Parameters:
	//     nStyle - [in] CRLF style to be set.
	//-----------------------------------------------------------------------
	void SetCRLFStyle(int nStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the CRLF text of the current document.
	// Returns:
	//     The CRLF text of the current document.
	//-----------------------------------------------------------------------
	CString GetCurCRLF() const;

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Internal implementation to get the CRLF text of the current document.
	// Parameters:
	//     nCRLFStyle - [in] Index of the CRLF style string.
	// Returns:
	//     The CRLF text of the current document.
	//-----------------------------------------------------------------------
	LPCTSTR GetCRLF(int nCRLFStyle = -1) const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if specified text is a CRLF.
	// Parameters:
	//     szCompText    - [in] Text to be checked.
	//     bMatchReverse - [in] TRUE to match reverse, FALSE otherwise.
	// Returns:
	//     TRUE if the specified text is a CRLF, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsTextCRLF(LPCTSTR szCompText, BOOL bMatchReverse = FALSE) const;

#	ifdef _UNICODE
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if specified text (non-unicode string) is a CRLF.
	// Parameters:
	//     szCompText    - [in] Text to be checked.
	//     bMatchReverse - [in] TRUE to match reverse, FALSE otherwise.
	// Returns:
	//     TRUE if the specified text is a CRLF, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsTextCRLF(LPCSTR szCompText, BOOL bMatchReverse = FALSE) const;
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of rows in the document.
	// Returns:
	//     The number of rows in the document.
	//-----------------------------------------------------------------------
	int GetRowCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated Undo/Redo manager.
	// Returns:
	//     A pointer to the associated Undo/Redo manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditUndoRedoManager* GetUndoRedoManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated line marks manager.
	// Returns:
	//     A pointer to the associated line marks manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLineMarksManager* GetLineMarksManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated lexical parser.
	// Returns:
	//     A pointer to the associated lexical parser.
	//-----------------------------------------------------------------------
	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* GetLexParser() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the associated configuration manager.
	// Returns:
	//     A pointer to the associated configuration manager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditConfigurationManager* GetLexConfigurationManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the associated configuration manager.
	// Parameters:
	//     pMan - [in] Pointer to the configuration manager to be set.
	//-----------------------------------------------------------------------
	void SetLexConfigurationManager(CXTPSyntaxEditConfigurationManager* pMan);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the events connection object.
	// Returns:
	//     A pointer to the events connection object.
	//-----------------------------------------------------------------------
	CXTPNotifyConnection* GetConnection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Cleans up all internal objects.
	//-----------------------------------------------------------------------
	void Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     Serializes actual data.
	// Parameters:
	//     ar - [in, out] Reference to the archive to save/load text.
	//-----------------------------------------------------------------------
	virtual void Serialize(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Serializes actual data.
	// Parameters:
	//     ar                      - [in, out] Reference to the archive to save/load text.
	//     bUnicode                - [in] TRUE to use UNICODE format for text in the archive.
	//     bWriteUnicodeFilePrefix - [in] TRUE to write Unicode File Prefix.
	//     nCodePage               - [in] Text code page (or -1 to use default).
	//     pcszFileExt             - [in] File extension to select appropriate parser schema.
	//     nDataSizeLimit          - [in] The number of bytes to stop storing text to archive
	//                                    (or -1 to store all).
	//-----------------------------------------------------------------------
	virtual void SerializeEx(CArchive& ar, BOOL bUnicode = -1, BOOL bWriteUnicodeFilePrefix = TRUE,
							 UINT nCodePage = (UINT)-1, LPCTSTR pcszFileExt = NULL,
							 int nDataSizeLimit = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the file pointer for the buffer.
	// Parameters:
	//     pFile       - [in] File pointer to be set.
	//     pcszFileExt - [in] Number of lines (Not used).
	// See Also:
	//     SetFileExt()
	//-----------------------------------------------------------------------
	void Load(CFile* pFile, LPCTSTR pcszFileExt = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//      Sets the file extension.
	// Parameters:
	//      strExt - [in] Reference to the file extension to be set.
	// See also:
	//      CString GetFileExt()
	//-----------------------------------------------------------------------
	void SetFileExt(const CString& strExt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the file extension.
	// Returns:
	//     The file extension.
	// See also:
	//     void SetFileExt(const CString& strExt);
	//-----------------------------------------------------------------------
	CString GetFileExt() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a line of text from either a file or buffer.
	// Parameters:
	//     iLine      - [in] Line for which to get the text.
	//     strText    - [out] Reference to a CString object
	//                        to receive the line of text.
	//     bAddCRLF   - [in] TRUE to add CRLF (if it exists), FALSE otherwise.
	//     iCRLFStyle - [in] CRLF style.
	// Returns:
	//     The line of text.
	//-----------------------------------------------------------------------
	void GetLineText(int iLine, CString& strText, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1);
	CString GetLineText(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1)
		const; // <combine CXTPSyntaxEditBufferManager::GetLineText@int@CString&@BOOL@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of a line of text in TCHARs/characters.
	// Parameters:
	//     iLine      - [in] Line for which to get the text.
	//     bAddCRLF   - [in] TRUE to add CRLF (if it exists), FALSE otherwise.
	//     iCRLFStyle - [in] CRLF style.
	// Returns:
	//     The length of the line of text in TCHARs/characters.
	//-----------------------------------------------------------------------
	int GetLineTextLength(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1) const;
	int GetLineTextLengthC(int iLine, BOOL bAddCRLF = FALSE, int iCRLFStyle = -1)
		const; // <combine CXTPSyntaxEditBufferManager::GetLineTextLength@int@BOOL@int@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum allowed length for a line of text.
	// Returns:
	//     The maximum allowed length for a line of text.
	//-----------------------------------------------------------------------
	int GetMaxLineTextLength() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of a line of text from either a file or buffer.
	// Parameters:
	//     nLineFrom   - [in] Start line identifier.
	//     nLineTo     - [in] End line identifier.
	//     bExpandTabs - [in] TRUE to expand tab characters, FALSE otherwise.
	// Returns:
	//     The length of the line of text.
	//-----------------------------------------------------------------------
	int CalcMaxLineTextLength(int nLineFrom = -1, int nLineTo = -1, BOOL bExpandTabs = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the text from the buffer bounded between either
	//     the row1/col1 and row2/col2 or the lcStart and lcEnd text coordinates.
	// Parameters:
	//     row1               - [in] Start line number.
	//     col1               - [in] Start column number.
	//     lcStart            - [in] Start text position.
	//     row2               - [in] End line number.
	//     col2               - [in] End column number.
	//     lcEnd              - [in] End text position.
	//     file               - [out] Reference to a CMemFile object
	//                                to receive the text.
	//     bColumnSelection   - [in] TRUE to copy the text block starting from
	//                               col1/lcStart and ending with
	//                               col2/lcEnd for each line between
	//                               row1/lcStart and row2/lcEnd,
	//                               FALSE to copy the full text between
	//                               row1/lcStart and row2/lcEnd.
	//     bForceDOSStyleCRLF - [in] TRUE to use DOS Style carriage return chars
	//                               to end each line,
	//                               FALSE to use the current carriage return style.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetBuffer(int row1, int col1, int row2, int col2, CMemFile& file,
				   BOOL bColumnSelection = FALSE, BOOL bForceDOSStyleCRLF = FALSE);
	BOOL GetBuffer(
		const XTP_EDIT_LINECOL& lcStart, const XTP_EDIT_LINECOL& lcEnd, CMemFile& file,
		BOOL bColumnSelection = FALSE,
		BOOL bForceDOSStyleCRLF =
			FALSE); // <combine
					// CXTPSyntaxEditBufferManager::GetBuffer@int@int@int@int@CMemFile&@BOOL@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts either text or a text block.
	// Parameters:
	//     szText   - [in] Text to be inserted.
	//     iRow     - [in] Start row for delete.
	//     nRow     - [in] Start row for delete.
	//     iCol     - [in] Start column for delete.
	//     nCol     - [in] Start column for delete.
	//     bCanUndo - [in] TRUE if this operation can be undone, FALSE otherwise.
	//     pFinalLC - [out] Pointer to an XTP_EDIT_LINECOL variable
	//                      to receive the operation final position
	//                      in the text (to set caret).
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL InsertText(LPCTSTR szText, int iRow, int iCol, BOOL bCanUndo = TRUE,
					XTP_EDIT_LINECOL* pFinalLC = NULL);
	BOOL InsertTextBlock(
		LPCTSTR szText, int nRow, int nCol, BOOL bCanUndo = TRUE,
		XTP_EDIT_LINECOL* pFinalLC =
			NULL); // <combine
				   // CXTPSyntaxEditBufferManager::InsertText@LPCTSTR@int@int@BOOL@XTP_EDIT_LINECOL*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a specified range of text.
	// Parameters:
	//     iRowFrom - [in] Start row for delete.
	//     iColFrom - [in] Start column for delete.
	//     iRowTo   - [in] End row for delete.
	//     iColTo   - [in] End column for delete.
	//     bCanUndo - [in] TRUE if this operation can be undone, FALSE otherwise.
	//     bDispCol - [in] TRUE if iColFrom and iColTo are display columns,
	//                     FALSE if iColFrom and iColTo are string columns.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL DeleteText(int iRowFrom, int iColFrom, int iRowTo, int iColTo, BOOL bCanUndo = TRUE,
					BOOL bDispCol = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a specified number of rows.
	// Parameters:
	//     nRow       - [in] Start row for delete.
	//     bCanUndo   - [in] TRUE if this operation can be undone, FALSE otherwise.
	//     nRowsCount - [in] Number of rows to delete.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveLine(int nRow, BOOL bCanUndo = TRUE, int nRowsCount = 1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the name of the main configuration file.
	// Returns:
	//     The name of the main configuration file.
	// See Also:
	//     CXTPSyntaxEditBufferManager::SetConfigFile
	//-----------------------------------------------------------------------
	CString GetConfigFile() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the the path to the main configuration file.
	// Parameters:
	//     szPath - [in] Path to be set.
	// Remarks:
	//     The configuration will be reloaded.
	// Returns:
	//     TRUE if the path was found, otherwise FALSE.
	// See Also:
	//     CXTPSyntaxEditBufferManager::GetConfigFile
	//-----------------------------------------------------------------------
	BOOL SetConfigFile(LPCTSTR szPath);

	void SetCodePage(UINT uCodePage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the master text schema.
	// Parameters:
	//     strExt - [in] Reference to the file extension to
	//                   the schema definition file.
	// Returns:
	//     A pointer to the master text schema.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextSchema* GetMasterTextSchema(const CString& strExt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the case of a specified character range in a specified row.
	// Parameters:
	//     nRow      - [in] Row to change case in.
	//     nDispFrom - [in] Start visible position to change case from.
	//     nDispTo   - [in] End visible position to change case to.
	//     bUpper    - [in] TRUE to change case to upper,
	//                      FALSE to change case to lower.
	//     bCanUndo  - [in] TRUE if this operation can be undone, FALSE otherwise.
	// Remarks:
	//      Configuration will be reloaded.
	//-----------------------------------------------------------------------
	void ChangeCase(int nRow, int nDispFrom, int nDispTo, BOOL bUpper, BOOL bCanUndo = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the tabification of a specified character range in a specified row.
	// Parameters:
	//     nRow      - [in] Row to change tabification in.
	//     nDispFrom - [in] Start visible position to change tabification from.
	//     nDispTo   - [in] End visible position to change tabification to.
	//     bTabify   - [in] TRUE to change spaces to tabs,
	//                      FALSE to change tabs to spaces.
	//     bCanUndo  - [in] TRUE if this operation can be undone, FALSE otherwise.
	// Remarks:
	//      Configuration will be reloaded.
	//-----------------------------------------------------------------------
	void ChangeTabification(int nRow, int nDispFrom, int nDispTo, BOOL bTabify,
							BOOL bCanUndo = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the column coordinates of text to string position.
	// Parameters:
	//     nLine    - [in] Text line identifier.
	//     nDispCol - [in] Column coordinates.
	// Returns:
	//     The converted string position.
	// See Also:
	//     StrPosToCol
	//-----------------------------------------------------------------------
	virtual int ColToStrPos(int nLine, int nDispCol) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts the string position of text to column coordinates.
	// Parameters:
	//     nLine   - [in] Text line identifier.
	//     nStrPos - [in] String position.
	// Returns:
	//     The converted column coordinates.
	// See Also:
	//     ColToStrPos
	//-----------------------------------------------------------------------
	virtual int StrPosToCol(int nLine, int nStrPos) const;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Cleans up the buffer and all other allocated memories.
	//-----------------------------------------------------------------------
	void CleanUp();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the average data size based on the average row length.
	// Parameters:
	//     nRowStart - [in] Start row identifier.
	//     nRowEnd   - [in] End row identifier.
	// Returns:
	//     The average data size.
	//-----------------------------------------------------------------------
	virtual UINT CalcAveDataSize(int nRowStart, int nRowEnd);

	int m_nAverageLineLen; // Average line length.

	CXTPNotifyConnection* m_pConnection; // Connection object to send notifications.

protected:
	int m_nTabSize;			   // Tab size.
	int m_iCRLFStyle;		   // CRLF style 0 - DOS, 1 - UNIX, 2 - MAC.
	UINT m_nCodePage;		   // CodePage for text conversion.
	BOOL m_bUnicodeFileFormat; // Determine is file format Unicode or ASCII.
	BOOL m_bOverwrite;		   // Overwrite mode status.

	enum XTP_SYNTAXEDIT_FILE_ENCODING
	{
		XTP_SYNTAXEDIT_FILE_UTF_8	 = 0,
		XTP_SYNTAXEDIT_FILE_UTF_8_BOM = 1,
		XTP_SYNTAXEDIT_FILE_UTF_16_LE = 2,
		XTP_SYNTAXEDIT_FILE_UTF_16_BE = 3,
	};
	XTP_SYNTAXEDIT_FILE_ENCODING m_nFileEncoding;

	int GetFileEncoding();		   // uploaded file encoding
	void SetFileEncoding(int val); // save file encoding

	CXTPSyntaxEditUndoRedoManager* m_pUndoRedoManager; // Pointer to the undo / redo manager.

	CXTPSyntaxEditLineMarksManager* m_pLineMarksManager; // Pointer to the line marks manager.

	XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* m_pLexParser; // Pointer to the lexical
																	 // parser.

	CXTPSyntaxEditConfigurationManagerPtr m_ptrLexConfigurationManager;
	// Pointer to the Lexical configuration manager

	CXTPSyntaxEditConfigurationManagerPtr m_ptrDefaultLexConfigurationManager;
	// Default Configuration Manager instance.

	CString m_strFileExt; // File extension.

	BOOL m_bIsParserEnabled; // TRUE if the parser is enabled, FALSE if the parser is disabled.

	//{{AFX_CODEJOCK_PRIVATE
	CXTPNotifySink* m_pLexConfigManSinkMT;

	virtual void OnLexConfigManEventHandler(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam);
	//}}AFX_CODEJOCK_PRIVATE

	//===========================================================================
	// Summary:
	//     This is helper class designed to manage a set of strings.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPSyntaxEditStringsManager
	{
		//{{AFX_CODEJOCK_PRIVATE
		friend class CXTPSyntaxEditBufferManager;
		//}}AFX_CODEJOCK_PRIVATE
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Default object constructor.
		//-----------------------------------------------------------------------
		CXTPSyntaxEditStringsManager();

		//-----------------------------------------------------------------------
		// Summary:
		//     Default object destructor. Handles cleanup and deallocation
		//-----------------------------------------------------------------------
		virtual ~CXTPSyntaxEditStringsManager();

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the number of managed strings.
		// Returns:
		//     The number of managed strings.
		//-----------------------------------------------------------------------
		int GetCount() const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the string at a specified index.
		// Parameters:
		//     nRow - [in] Index of the string to retrieve.
		// Returns:
		//     The string at the specified index.
		//-----------------------------------------------------------------------
		CString GetStr(int nRow) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the length of the string at a specified index
		//     in TCHARs/characters.
		// Parameters:
		//     nRow - [in] Index of the string to retrieve the length of.
		// Returns:
		//     The length of the string at the specified index
		//     in TCHARs/characters.
		//-----------------------------------------------------------------------
		int GetStrLen(int nRow) const;
		int GetStrLenC(int nRow)
			const; // <combine
				   // CXTPSyntaxEditBufferManager::CXTPSyntaxEditStringsManager::GetStrLen@int@const>

		//-----------------------------------------------------------------------
		// Summary:
		//     Sets the string at a specified index.
		// Parameters:
		//     nRow     - [in] Index.
		//     strText  - [in] Reference to the string to be set.
		//     pcszText - [in] Pointer to the NULL-terminated string to be set.
		// Remarks:
		//     If the specified index does not exist within the collection,
		//     then this method will do nothing.
		//-----------------------------------------------------------------------
		void SetAtGrowStr(int nRow, const CString& strText);
		void SetAtGrowStr(
			int nRow,
			LPCTSTR
				pcszText); // <combine
						   // CXTPSyntaxEditBufferManager::CXTPSyntaxEditStringsManager::SetAtGrowStr@int@const
						   // CString&>

		//-----------------------------------------------------------------------
		// Summary:
		//     Adds a specified string.
		// Parameters:
		//     nRow    - [in] String identifier.
		//     strText - [in] Reference to the string to be added.
		// Remarks:
		//     If an existing identifier is used, then its string value will be updated.
		// See Also:
		//     SetAtGrowStr
		//-----------------------------------------------------------------------
		void InsertStr(int nRow, const CString& strText);

		//-----------------------------------------------------------------------
		// Summary:
		//     Removes the string at a specified index.
		// Parameters:
		//     nRow - [in] Index of the string to be removed.
		// Returns:
		//     TRUE if successful, FALSE if a string did not exist at the specified index.
		//-----------------------------------------------------------------------
		BOOL RemoveStr(int nRow);

		//-----------------------------------------------------------------------
		// Summary:
		//     Removes all strings from the collection.
		//-----------------------------------------------------------------------
		void RemoveAllStrs();

		//-----------------------------------------------------------------------
		// Summary:
		//     Inserts text to the string at a specified index.
		// Parameters:
		//     nRow              - [in] Index of the string to insert text to.
		//     nPos              - [in] Start position in the string.
		//     pcszText          - [in] Text to be inserted.
		//     bGrowArrayIfNeed  - [in] TRUE to grow the strings array if nRow
		//                              is greater than the strings count,
		//                              FALSE to have this function do nothing
		//                              and return FALSE in this instance.
		//     chLeftSpaceFiller - [in] Character to be added to the left side
		//                              of the string to insert text to if
		//                              nPos is greater than the length of the
		//                              string. In this case, this character
		//                              will be added until the length of the
		//                              string is equal to nPos.
		// Returns:
		//     TRUE if the text was successfully inserted, otherwise FALSE.
		// See Also:
		//     DeleteText
		//-----------------------------------------------------------------------
		BOOL InsertText(int nRow, int nPos, LPCTSTR pcszText, BOOL bGrowArrayIfNeed = FALSE,
						TCHAR chLeftSpaceFiller = _T(' '));
		//-----------------------------------------------------------------------
		// Summary:
		//     Deletes text from the string at a specified index.
		// Parameters:
		//     nRow   - [in] Index of the string to delete text from.
		//     nPos   - [in] Start position in the string.
		//     nCount - [in] Number of characters to remove.
		// Returns:
		//     TRUE if the text was successfully deleted, otherwise FALSE.
		// See Also:
		//     InsertText
		//-----------------------------------------------------------------------
		BOOL DeleteText(int nRow, int nPos, int nCount);

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the string at a specified index.
		// Parameters:
		//     nRow             - [in] Index of the string to be retrieved.
		//     bGrowArrayIfNeed - [in] TRUE to grow the array of strings, FALSE otherwise.
		//                             The default value of this parameter is TRUE.
		// Returns:
		//     A pointer to the string at the specified index.
		//-----------------------------------------------------------------------
		CString* GetStrData(int nRow, BOOL bGrowArrayIfNeed = TRUE);
		CString* GetStrDataC(int nRow)
			const; // <combine
				   // CXTPSyntaxEditBufferManager::CXTPSyntaxEditStringsManager::GetStrData@int@BOOL>

		//-----------------------------------------------------------------------
		// Summary:
		//     Internal class, implements a collection of strings.
		//-----------------------------------------------------------------------
		class _XTP_EXT_CLASS CStringPtrArray : public CArray<CString*, CString*>
		{
			//{{AFX_CODEJOCK_PRIVATE
			typedef CArray<CString*, CString*> TBase;
			//}}AFX_CODEJOCK_PRIVATE
		public:
			//-------------------------------------------------------------------
			// Summary:
			//     Default object constructor.
			//-------------------------------------------------------------------
			CStringPtrArray();

			//-------------------------------------------------------------------
			// Summary:
			//     Default object destructor. Handles cleanup and deallocation
			//-------------------------------------------------------------------
			virtual ~CStringPtrArray();

			//-------------------------------------------------------------------
			// Summary:
			//     Deletes all string objects and removes them from the array.
			//-------------------------------------------------------------------
			void RemoveAll();
		};

		CStringPtrArray m_arStrings; // String collection.
	};

	CXTPSyntaxEditStringsManager m_Strings; // Strings manager helper object.

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditBufferManager)

	afx_msg BSTR OleGetFileExt();
	afx_msg void OleSetFileExt(LPCTSTR pcszFileExt);

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

////////////////////////////////////////////////////////////////////////////
AFX_INLINE CXTPSyntaxEditLineMarksManager* CXTPSyntaxEditBufferManager::GetLineMarksManager() const
{
	return m_pLineMarksManager;
}
AFX_INLINE XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser*
	CXTPSyntaxEditBufferManager::GetLexParser() const
{
	return m_pLexParser;
}
AFX_INLINE CXTPSyntaxEditConfigurationManager*
	CXTPSyntaxEditBufferManager::GetLexConfigurationManager() const
{
	return m_ptrLexConfigurationManager;
}
AFX_INLINE CXTPNotifyConnection* CXTPSyntaxEditBufferManager::GetConnection() const
{
	return m_pConnection;
}
AFX_INLINE CXTPSyntaxEditUndoRedoManager* CXTPSyntaxEditBufferManager::GetUndoRedoManager() const
{
	return m_pUndoRedoManager;
}
AFX_INLINE int CXTPSyntaxEditBufferManager::GetTabSize() const
{
	return m_nTabSize;
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::GetBuffer(const XTP_EDIT_LINECOL& lcStart,
													   const XTP_EDIT_LINECOL& lcEnd,
													   CMemFile& file, BOOL bColumnSelection,
													   BOOL bForceDOSStyleCRLF)
{
	return GetBuffer(lcStart.nLine, lcStart.nCol, lcEnd.nLine, lcEnd.nCol, file, bColumnSelection,
					 bForceDOSStyleCRLF);
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::GetOverwriteFlag() const
{
	return m_bOverwrite;
}
AFX_INLINE int CXTPSyntaxEditBufferManager::GetCurCRLFType() const
{
	return m_iCRLFStyle;
}
AFX_INLINE UINT CXTPSyntaxEditBufferManager::GetCodePage() const
{
	return m_nCodePage;
}
AFX_INLINE CString CXTPSyntaxEditBufferManager::GetFileExt() const
{
	return m_strFileExt;
}
AFX_INLINE BOOL CXTPSyntaxEditBufferManager::IsParserEnabled() const
{
	return m_bIsParserEnabled;
}

////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITBUFFERMANAGER_H__)
