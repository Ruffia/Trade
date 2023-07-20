// XTPSyntaxEditLexParser.h
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
#if !defined(__XTPSYNTAXEDITLEXPARSER_H__)
#	define __XTPSYNTAXEDITLEXPARSER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditColorTheme;
class CXTPNotifySink;
class CXTPNotifyConnection;
class CXTPSyntaxEditBufferManager;
class CXTPNotifyConnectionMT;

////////////////////////////////////////////////////////////////////////////
//{{AFX_CODEJOCK_PRIVATE
typedef CList<XTP_EDIT_TEXTBLOCK, XTP_EDIT_TEXTBLOCK> CXTPSyntaxEditTextBlockList;
typedef CArray<XTP_EDIT_TEXTBLOCK, XTP_EDIT_TEXTBLOCK&> CXTPSyntaxEditTextBlockArray;
//}}AFX_CODEJOCK_PRIVATE

namespace XTPSyntaxEditLexAnalyser
{
class CXTPSyntaxEditLexObj_ActiveTags;
class CXTPSyntaxEditLexClass;
class CXTPSyntaxEditLexClassInfoArray;
class CXTPSyntaxEditLexOnScreenParseCnt;

//========================================================================
// Summary:
//     Enumerates flags which indicate the results of parse function calls.
// See Also:
//     CXTPSyntaxEditLexClass, CXTPSyntaxEditLexTextSchema
//========================================================================
enum XTPSyntaxEditLexParseResult
{
	xtpEditLPR_Unknown	= 0,	 // Undefined value.
	xtpEditLPR_StartFound = 0x001, // Start text block was found.
	xtpEditLPR_EndFound   = 0x002, // End text block was found.
	xtpEditLPR_Iterated   = 0x010, // Indicates that the current text position has already moved to
								   // the next char inside a function call.
	xtpEditLPR_TBpop1 = 0x020, // Indicates that the new found text block should be inserted before
							   // the previous one and as a parent of it.
	xtpEditLPR_Error = 0x100,  // Iterated parse process breaked due to some error. See trace for
							   // details.
	xtpEditLPR_RunFinished = 0x200, // Run iterated parse process (or some big part of it) has
									// finished.
	xtpEditLPR_RunBreaked = 0x400,  // Run iterated parse process (or some big part of it) has
									// breaked.
	xtpEditLPR_RunRestart = 0x800,  // Restart run children from the beginning.

	// xtpEditLPR_NeedFullReparse = 0x1000, // Cannot reparse only specified piece of text. Full
	// reparse is necessary.
};

//===========================================================================
// Summary:
//     This class is designed to store the coordinates of a text region.
// See Also:
//     XTP_EDIT_LINECOL
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTextRegion
{
public:
	XTP_EDIT_LINECOL m_posStart; // Start position in document coordinates.
	XTP_EDIT_LINECOL m_posEnd;   // End position in document coordinates.

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears coordinates.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets coordinates.
	// Parameters:
	//     pLCStart - [in] Start position in document coordinates.
	//     pLCEnd   - [in] End position in document coordinates.
	//-----------------------------------------------------------------------
	void Set(const XTP_EDIT_LINECOL* pLCStart, const XTP_EDIT_LINECOL* pLCEnd);
};

//{{AFX_CODEJOCK_PRIVATE
typedef CArray<CXTPSyntaxEditTextRegion, const CXTPSyntaxEditTextRegion&>
	CXTPSyntaxEditInvalidZoneArray;
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class stores tokens definition: a list of tokens and
//     corresponding lists of start and end tokens separators.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexTokensDef
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexTokensDef(){};
	//-----------------------------------------------------------------------
	// Summary:
	//     Copy object constructor.
	// Parameters:
	//     rSrc - [in] Reference to source CXTPSyntaxEditLexTokensDef object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexTokensDef(const CXTPSyntaxEditLexTokensDef& rSrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Object destructor. Handles clean up and deallocations.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexTokensDef();

	//-----------------------------------------------------------------------
	// Summary:
	//     Assignment operator for class.
	// Parameters:
	//     rSrc - [in] Reference to source CXTPSyntaxEditLexTokensDef object.
	// Returns:
	//     A reference to a CXTPSyntaxEditLexTokensDef object.
	//-----------------------------------------------------------------------
	const CXTPSyntaxEditLexTokensDef& operator=(const CXTPSyntaxEditLexTokensDef& rSrc);

	CStringArray m_arTokens;	// List of tokens.
	CStringArray m_arStartSeps; // List of start separators.
	CStringArray m_arEndSeps;   // List of end separators.
};

//{{AFX_CODEJOCK_PRIVATE
typedef CArray<CXTPSyntaxEditLexTokensDef, const CXTPSyntaxEditLexTokensDef&>
	CXTPSyntaxEditLexTokensDefArray;
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     This class is designed to easily manipulate parser options.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexParserSchemaOptions : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexParserSchemaOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//     Object constructor.
	// Parameters:
	//     rSrc - [in] Reference to source CXTPSyntaxEditLexParserSchemaOptions object.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexParserSchemaOptions(const CXTPSyntaxEditLexParserSchemaOptions& rSrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded equals operator.
	// Parameters:
	//     rSrc - [in] Reference to source CXTPSyntaxEditLexParserSchemaOptions object.
	//-----------------------------------------------------------------------
	const CXTPSyntaxEditLexParserSchemaOptions&
		operator=(const CXTPSyntaxEditLexParserSchemaOptions& rSrc);

	// global:
	BOOL m_bFirstParseInSeparateThread;			  // {0,1} default=1
	BOOL m_bEditReparceInSeparateThread;		  // {0,1} default=1
	BOOL m_bConfigChangedReparceInSeparateThread; // {0,1} default=1
	DWORD m_dwMaxBackParseOffset; // default=100; maximum back buffer size. Some times parser look
								  // back for the text from current position.
	DWORD m_dwEditReparceTimeout_ms; // default=500 ms; time out for start reparse after last key
									 // was pressed.
	DWORD m_dwOnScreenSchCacheLifeTime_sec; // default=180 sec; time out for on screen parsed pices
											// of text. for memory using optimization.
	DWORD m_dwParserThreadIdleLifeTime_ms; // default=60 sec; time out for existing of parser thread
										   // when parser idle (no parse requests).
};

//{{AFX_CODEJOCK_PRIVATE
//=======================================================================
// Summary:
//
//=======================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexClassSchema : public CXTPCmdTarget
{
	friend class CXTPSyntaxEditLexTextSchema;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexClassSchema();
	//-----------------------------------------------------------------------
	// Summary:
	//      Object destructor. Handles clean up and deallocations.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexClassSchema();

	virtual void AddPreBuildClass(CXTPSyntaxEditLexClass* pClass);

	virtual BOOL Build();

	virtual BOOL Copy(CXTPSyntaxEditLexClassSchema* pDest);

	virtual void Close();

	virtual void RemoveAll();

	virtual CXTPSyntaxEditLexClassPtrArray* GetClasses(BOOL bShortSch);

	virtual CXTPSyntaxEditLexClassPtrArray* GetPreBuildClasses();
	virtual CXTPSyntaxEditLexClass* GetPreBuildClass(const CString& strName);

	virtual CXTPSyntaxEditLexClass* GetNewClass(BOOL bForFile);

	virtual CXTPSyntaxEditLexObj_ActiveTags* GetActiveTagsFor(CXTPSyntaxEditLexClass* pTopClass);

	// virtual CXTPSyntaxEditLexObj_ActiveTags* GetActiveTags(BOOL bShortSch);

protected:
	virtual BOOL Build_ChildrenFor(BOOL bDynamic, CXTPSyntaxEditLexClass* pCBase,
								   CStringArray& rarAdded, int& rnNextClassID, int nLevel = 0);

	virtual CXTPSyntaxEditLexClassPtrArray* GetChildrenFor(CXTPSyntaxEditLexClass* pClass,
														   BOOL& rbSelfChild);
	virtual CXTPSyntaxEditLexClassPtrArray* GetDynChildrenFor(CXTPSyntaxEditLexClass* pClass,
															  BOOL& rbSelfChild);

	virtual void GetDynParentsList(CXTPSyntaxEditLexClass* pClass, CStringArray& rarDynParents,
								   CStringArray& rarProcessedClasses);

	virtual int CanBeParentDynForChild(CString strParentName, CXTPSyntaxEditLexClass* pCChild);

	virtual BOOL Build_ShortTree();

	virtual BOOL PostBuild_Step(CXTPSyntaxEditLexClassPtrArray* pArClasses);

	virtual BOOL CopyChildrenFor(BOOL bShort, CXTPSyntaxEditLexClass* pCDest,
								 CXTPSyntaxEditLexClass* pCSrc, int nLevel);

	CXTPSyntaxEditLexClassPtrArray m_arPreBuildClassesList;

	CXTPSyntaxEditLexClassPtrArray m_arClassesTreeShort;
	CXTPSyntaxEditLexClassPtrArray m_arClassesTreeFull;

	// CXTPSyntaxEditLexObj_ActiveTags   m_arAllActiveTagsShort;
	// CXTPSyntaxEditLexObj_ActiveTags   m_arAllActiveTagsFull;
};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     The CXTPSyntaxEditLexTextBlock class represents a block of text which
//     corresponds to a lex class in a text schema.
//     It stores the start position, end position, and pointer to the lex class
//     object in a class schema along with some other information.
//     Text blocks are organized as a bidirectional list and as a tree structure.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexTextBlock : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditLexTextSchema;
	friend class CXTPSyntaxEditLexClass;
	friend class CXTPSyntaxEditLexClass_file;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexTextBlock();
	//-----------------------------------------------------------------------
	// Summary:
	//     Object destructor. Handles clean up and deallocations.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexTextBlock();

	CXTPSyntaxEditLexClassPtr m_ptrLexClass;   // Pointer to the LexClass object.
	CXTPSyntaxEditLexTextBlockPtr m_ptrParent; // Pointer to the parent block object.

	CXTPSyntaxEditLexTextBlockPtr m_ptrPrev; // Pointer to the previous block.
	CXTPSyntaxEditLexTextBlockPtr m_ptrNext; // Pointer to the next block.

	XTP_EDIT_LINECOL m_PosStartLC; // Text block start position.
	XTP_EDIT_LINECOL m_PosEndLC;   // Text block start position.
	BOOL m_bEndByParent;		   // Text block ended when the parent block ended.

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the text block end position.
	// Parameters:
	//     bMaxIfInvalid - [in] TRUE to get the maximum value if
	//                          the text block end position is invalid,
	//                          FALSE to get the minimum value if
	//                          the text block end position is invalid.
	// Returns:
	//     The text block end position if it is valid. Otherwise the
	//     maximum or minimum value depending on bMaxIfInvalid.
	// See Also:
	//     XTP_EDIT_LINECOL, m_PosEndLC, m_PosStartLC.
	//--------------------------------------------------------------------
	XTP_EDIT_LINECOL GetPosEndLC(BOOL bMaxIfInvalid = TRUE);

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the text block start tag end position in the text.
	// Returns:
	//     The text block start tag end position in XLC form.
	// See Also:
	//     XTP_EDIT_LINECOL::GetXLC(), XTP_EDIT_XLC() macros.
	//--------------------------------------------------------------------
	DWORD GetStartTagEndXLC();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the text block end tag begin position in the text.
	// Returns:
	//     The text block end tag begin position in XLC form.
	// See Also:
	//     XTP_EDIT_LINECOL::GetXLC(), XTP_EDIT_XLC() macros.
	//--------------------------------------------------------------------
	DWORD GetEndTagBeginXLC();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the text block end tag end position in the text.
	// Returns:
	//     The text block end tag end position in XLC form.
	// See Also:
	//     XTP_EDIT_LINECOL::GetXLC(), XTP_EDIT_XLC() macros.
	//--------------------------------------------------------------------
	DWORD GetEndTagEndXLC();

	//--------------------------------------------------------------------
	// Summary:
	//     Releases references to other objects.
	// Remarks:
	//     This method is used in text schema destruction process.
	// See Also:
	//     CXTPSyntaxEditLexTextSchema::Close().
	//--------------------------------------------------------------------
	virtual void Close();
	// virtual void Close(BOOL bWithPrev = FALSE, BOOL bWithNext = FALSE);

	//--------------------------------------------------------------------
	// Summary:
	//     Determines if the lex class name of a specified text block
	//     is equal to the lex class name of the current text block.
	// Parameters:
	//     pTB2 - [in] Pointer to a CXTPSyntaxEditLexTextBlock object.
	// Returns:
	//     TRUE if the lex class name of the specified text block
	//     is equal to the lex class name of the current text block,
	//     otherwise FALSE.
	//--------------------------------------------------------------------
	virtual BOOL IsEqualLexClasses(CXTPSyntaxEditLexTextBlock* pTB2) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Determines if a specified text block is included within
	//     the current text block.
	// Parameters:
	//     pTB2 - [in] Pointer to a CXTPSyntaxEditLexTextBlock object.
	// Returns:
	//     TRUE if the specified text block is included within
	//     the current textblock, otherwise FALSE.
	//--------------------------------------------------------------------
	virtual BOOL IsInclude(CXTPSyntaxEditLexTextBlock* pTB2) const;

	//--------------------------------------------------------------------
	// Summary:
	//     Determines if the current text block appears to
	//     be partly closed or closed.
	// Returns:
	//     TRUE if some or all references to other objects are NULL,
	//     FALSE otherwise.
	//--------------------------------------------------------------------
	virtual BOOL IsLookLikeClosed() const;

	//--------------------------------------------------------------------
	// Summary:
	//     Sets the end position to not ended children blocks.
	// Parameters:
	//     pTxtSch      - [in] Text schema pointer to send notifications.
	//     bEndByParent - [in] BOOL flag
	// Returns:
	//     Ended children blocks count.
	//--------------------------------------------------------------------
	virtual int EndChildren(CXTPSyntaxEditLexTextSchema* pTxtSch = NULL, BOOL bEndByParent = TRUE);

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the previous child block from a specified child block.
	// Parameters:
	//     pChild      - [in] Pointer to the child block to get
	//                        the previous child block from.
	//     bWithAddRef - [in] TRUE to call InternalAddRef, FALSE otherwise.
	// Remarks:
	//     The current text block is the parent block to the two child blocks.
	// Returns:
	//     A pointer to the previous child block if successful, otherwise NULL.
	//--------------------------------------------------------------------
	virtual CXTPSyntaxEditLexTextBlock* GetPrevChild(CXTPSyntaxEditLexTextBlock* pChild,
													 BOOL bWithAddRef);

#	ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#	endif

protected:
	int m_nStartTagLen;  // Start tag length.
	int m_nEndTagXLCLen; // End tag length in XLC form.

	CXTPSyntaxEditLexTextBlockPtr m_ptrLastChild; // For internal use only. This variable
												  // does not always contain the correct value.
};

//===========================================================================
// Summary:
//     The CXTPSyntaxEditLexTextSchema class is used to store data which is
//     necessary to parse a text and parsing results. It also contains
//     methods to run lex classes from a class schema, to update text schema
//     when text is edited, to reparse text, to get information which is
//     necessary to draw colorized text, and other helper methods.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexTextSchema : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPSyntaxEditLexClass;
	friend class CXTPSyntaxEditLexClass_file;
	friend class CXTPSyntaxEditLexTextBlock;
	friend class CXTPSyntaxEditLexParser;
	//}}AFX_CODEJOCK_PRIVATE
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pcszSchName - [in] Text schema name.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexTextSchema(LPCTSTR pcszSchName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Object destructor. Handles clean up and deallocations.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexTextSchema();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text schema name.
	// Returns:
	//     The text schema name.
	//-----------------------------------------------------------------------
	virtual CString GetSchName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys all interlocked object references and
	//     frees all stored objects.
	// Remarks:
	//     This method deletes the schema object.
	// See Also:
	//     RemoveAll, CXTPSyntaxEditLexClassSchema::Close
	//-----------------------------------------------------------------------
	virtual void Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys the text blocks list.
	// Parameters:
	//     pFirst - Pointer to the first element in the list to destroy.
	// See Also:
	//     CXTPSyntaxEditLexTextBlock::Close
	//-----------------------------------------------------------------------
	static void AFX_CDECL Close(CXTPSyntaxEditLexTextBlock* pFirst);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all text blocks from the schema.
	// Remarks:
	//     This method does not destroy the schema object.
	//     It only removes the text blocks.
	// See Also:
	//     RemoveAll, CXTPSyntaxEditLexClassSchema::Close
	//-----------------------------------------------------------------------
	virtual void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a new text schema object and initializes its internal
	//     infrastructure from the current schema.
	// Remarks:
	//     This method copies the class schema, but does not copy the text blocks.
	// Returns:
	//     A pointer to the newly created and initialized text schema object.
	//-----------------------------------------------------------------------
	virtual CXTPSyntaxEditLexTextSchema* Clone();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the notification connection.
	// Returns:
	//     A pointer to the notification connection.
	//-----------------------------------------------------------------------
	virtual CXTPNotifyConnection* GetConnection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Runs parse for the text.
	// Parameters:
	//     bShort              - [in] TRUE to use the short
	//                                (global blocks only) class schema,
	//                                FALSE to use the full
	//                                (global and onscreen blocks) class schema.
	//     pTxtIter            - [in] Pointer to a CTextIter object.
	//     nRowStart           - [in] Start text position for parse/reparse.
	//     nRowEnd             - [in] End text position for parse/reparse.
	//     bSendProgressEvents - [in] TRUE to send progress parsing events using
	//                                the connection object, FALSE otherwise.
	// Remarks:
	//     Special parameters values:
	//     * pLCStart = NULL and pLCEnd = NULL to
	//       parse from the last parsed block to the end of the text.
	//
	//     * pLCStart = XTP_EDIT_LINECOL::Pos1 and pLCEnd = NULL to
	//       parse/reparse all text from the beginning.
	// Returns:
	//     The parse result; one of the values defined by
	//     the XTPSyntaxEditLexParseResult enumeration.
	//-----------------------------------------------------------------------
	virtual int RunParseUpdate(BOOL bShort, CTextIter* pTxtIter,
							   const XTP_EDIT_LINECOL* pLCStart = NULL,
							   const XTP_EDIT_LINECOL* pLCEnd   = NULL,
							   BOOL bSendProgressEvents			= FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text region for which the schema was updated during
	//     the last parse/reparse operation.
	// Returns:
	//     A reference to a CXTPSyntaxEditTextRegion object.
	//-----------------------------------------------------------------------
	virtual const CXTPSyntaxEditTextRegion& GetUpdatedTextRegion() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Runs parse for the text on screen.
	// Parameters:
	//     pTxtIter             - [in] Pointer to a CTextIter object.
	//     nRowStart            - [in] Start row for parse.
	//     nRowEnd              - [in] End row for parse.
	//     rPtrScreenSchFirstTB - [out] Reference to a CXTPSyntaxEditLexTextBlockPtr
	//                                  object to receive the first text block of
	//                                  the built text schema.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL RunParseOnScreen(CTextIter* pTxtIter, int nRowStart, int nRowEnd,
								  CXTPSyntaxEditLexTextBlockPtr& rPtrScreenSchFirstTB);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a specified row as a list of blocks with different colors to draw.
	// Parameters:
	//     pTxtIter         - [in] Pointer to a CTextIter object.
	//     nRow             - [in] Row identifier.
	//     nColFrom         - [in] Start column.
	//     nColTo           - [in] End column.
	//     clrDefault       - [in] Reference to an XTP_EDIT_COLORVALUES structure
	//                             containing the default colors.
	//     rBlocks          - [out] Pointer to a CXTPSyntaxEditTextBlockList
	//                              object to receive the list of color blocks.
	//     pptrTBStartCache - [in, out] Reference to a cached text block to
	//                                  start the search.
	//                                  The value of this parameter may be NULL.
	//     pFirstSchTB      - [in] Reference to the first block of the
	//                             external text schema or NULL.
	//                             This parameter is used for OnScreen schemas.
	//-----------------------------------------------------------------------
	virtual void GetRowColors(CTextIter* pTxtIter, int nRow, int nColFrom, int nColTo,
							  const XTP_EDIT_COLORVALUES& clrDefault,
							  CXTPSyntaxEditTextBlockList* rBlocks,
							  CXTPSyntaxEditLexTextBlockPtr* pptrTBStartCache = NULL,
							  CXTPSyntaxEditLexTextBlock* pFirstSchTB		  = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a specified row as a list of collapsible blocks.
	// Parameters:
	//     nRow             - [in] Row identifier. Passing a negative value
	//                             will return the blocks for all rows.
	//     rArBlocks        - [out] Reference to a CXTPSyntaxEditRowsBlockArray
	//                              object to receive the list of collapsable blocks.
	//     pptrTBStartCache - [in, out] Reference to a cached text block to
	//                                  start the search.
	//                                  The value of this parameter may be NULL.
	//-----------------------------------------------------------------------
	virtual void GetCollapsableBlocksInfo(int nRow, CXTPSyntaxEditRowsBlockArray& rArBlocks,
										  CXTPSyntaxEditLexTextBlockPtr* pptrTBStartCache = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the text attributes from the lex class of a specified text block.
	// Parameters:
	//     rTB         - [out] Reference to an XTP_EDIT_TEXTBLOCK structure
	//                         to receive the text attributes.
	//     pTextBlock  - [in] Pointer to a CXTPSyntaxEditLexTextBlock object.
	//-----------------------------------------------------------------------
	virtual void GetTextAttributes(XTP_EDIT_TEXTBLOCK& rTB, CXTPSyntaxEditLexTextBlock* pTextBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a specified color theme to this text schema.
	// Parameters:
	//     pTheme - [in] Pointer to the color theme to apply.
	//-----------------------------------------------------------------------
	virtual void ApplyTheme(CXTPSyntaxEditColorTheme* pTheme);

	//--------------------------------------------------------------------
	// Summary:
	//     Loads a specified class schema.
	// Parameters:
	//     arClassInfo - [in] Reference to a CXTPSyntaxEditLexClassInfoArray object.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//--------------------------------------------------------------------
	BOOL LoadClassSchema(CXTPSyntaxEditLexClassInfoArray* arClassInfo);

	//--------------------------------------------------------------------
	// Summary:
	//     Determines if this schema supports a specified file extension.
	// Parameters:
	//     strExt - [in] Reference to the file extension.
	// Remarks:
	//     Extension string format: ".ext"
	//     For example: ".cpp"
	// Returns:
	//     TRUE if the specified file extension is supported, otherwise FALSE.
	//--------------------------------------------------------------------
	BOOL IsFileExtSupported(const CString& strExt);

	//--------------------------------------------------------------------
	// Summary:
	//     Builds the schema if necessary, otherwise does nothing.
	// Remarks:
	//     Call this member function before using a schema object.
	//
	//     In order to make edit control startup faster, schemas
	//     are not built during loading. This member function will
	//     build the schema if necessary or do nothing otherwise.
	//--------------------------------------------------------------------
	void BuildIfNeed();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the top lex class for a specified file extension.
	// Parameters:
	//     strExt - [in] Reference to the file extension.
	// Remarks:
	//     Extension string format: ".ext"
	//     For example: ".cpp"
	//     InternalAddRef is called for the returned object.
	// Returns:
	//     A pointer to the lex class if successful, otherwise NULL.
	//--------------------------------------------------------------------
	CXTPSyntaxEditLexClass* GetTopClassForFileExt(const CString& strExt);

	//--------------------------------------------------------------------
	// Summary:
	//     Retrieves a specified text block by its identifier.
	// Parameters:
	//     dwID - [in] Identifier of the text block to retrieve
	//                 (obtained from an xtpEditOnTextBlockParsed notification).
	// Remarks:
	//     This method should be used together with
	//     an xtpEditOnTextBlockParsed notification.
	//
	//     InternalAddRef is called for the returned object.
	// Returns:
	//     A pointer to the text block if successful, otherwise NULL.
	//--------------------------------------------------------------------
	virtual CXTPSyntaxEditLexTextBlock* GetLastParsedBlock(WPARAM dwID = 0);

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the first text schema block.
	// Remarks:
	//     InternalAddRef is called for the returned object.
	// Returns:
	//     A pointer to the first text schema block if successful, otherwise NULL.
	//--------------------------------------------------------------------
	CXTPSyntaxEditLexTextBlock* GetBlocks();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the critical section object to lock schema data
	//     (i.e. text blocks and others).
	// Returns:
	//     A pointer to the critical section object.
	//--------------------------------------------------------------------
	CMutex* GetDataLoker();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the critical section object to lock class schema.
	// Returns:
	//     A pointer to the critical section object.
	//--------------------------------------------------------------------
	CMutex* GetClassSchLoker();

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the event object to break the parsing process.
	// Returns:
	//     A pointer to the event object.
	//--------------------------------------------------------------------
	CEvent* GetBreakParsingEvent();

	//{{AFX_CODEJOCK_PRIVATE
	static void AFX_CDECL TraceClrBlocks(CXTPSyntaxEditTextBlockArray& arBlocks);
	void TraceTxtBlocks(BOOL bFull); // for DEBUG only
									 //}}AFX_CODEJOCK_PRIVATE

protected:
	//========================================================================
	// Summary:
	//     This enum describes the set of UpdateTextBlocks return results.
	//========================================================================
	enum XTPSyntaxEditUpdateTBRes
	{
		xtpEditUTBNothing = 0,		// Void result.
		xtpEditUTBError   = 0x0001, // Error result.
		xtpEditUTBReparse = 0x0002, // One or few global blocks changed and a reparse is needed.
		xtpEditUTBNearestUpdated = 0x0100, // This flag is returned from UpdateTBNearest only.
	};

	//--------------------------------------------------------------------
	// Summary:
	//     Gets the class schema of this text schema.
	// Remarks:
	//     InternalAddRef is called for the returned object.
	// Returns:
	//     A pointer to the class schema of this text schema.
	//--------------------------------------------------------------------
	CXTPSyntaxEditLexClassSchema* GetClassSchema();

	//-----------------------------------------------------------------------
	// Summary:
	//     Applies a specified color theme to a specified lex class array.
	// Parameters:
	//     pTheme      - [in] Pointer to the color theme to apply.
	//     ptrClasses  - [in] Pointer to a lex class array.
	// See Also:
	//     ApplyTheme
	//-----------------------------------------------------------------------
	virtual void ApplyThemeRecursive(CXTPSyntaxEditColorTheme* pTheme,
									 CXTPSyntaxEditLexClassPtrArray* ptrClasses);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates text schema positions after an editing action.
	// Parameters:
	//     posFrom     - [in] Start editing position.
	//     posTo       - [in] End editing position.
	//     eEditAction - [in] Edit action. Must be one of the values defined by
	//                        the XTPSyntaxEditEditAction enumeration.
	// Returns:
	//     The update result. One of the values defined by
	//     the XTPSyntaxEditUpdateTBRes enumeration.
	//-----------------------------------------------------------------------
	int UpdateTextBlocks(XTP_EDIT_LINECOL posFrom, XTP_EDIT_LINECOL posTo, int eEditAction);

	//{{AFX_CODEJOCK_PRIVATE
	virtual int RunChildren(CTextIter* pTxtIter, CXTPSyntaxEditLexTextBlockPtr ptrTxtBlock,
							CXTPSyntaxEditLexClass* pBase,
							CXTPSyntaxEditLexOnScreenParseCnt* pOnScreenRunCnt = NULL);

	virtual CXTPSyntaxEditLexTextBlock* GetPrevBlock(BOOL bWithAddRef = TRUE);

	virtual CXTPSyntaxEditLexTextBlock* GetNewBlock();
	//}}AFX_CODEJOCK_PRIVATE

private:
	int UpdateTBNearest(CXTPSyntaxEditLexTextBlock* pNarestTB1, int nLineDiff, int nColDiff,
						XTP_EDIT_LINECOL posFrom, XTP_EDIT_LINECOL posTo, int eEditAction);

	//-------------------------------
	CXTPSyntaxEditLexTextBlock* FindNearestTextBlock(XTP_EDIT_LINECOL posText);

	virtual BOOL InitScreenSch(CTextIter* pTxtIter, int nRowStart, int nRowEnd,
							   CXTPSyntaxEditLexTextBlockPtr& rPtrScreenSchFirstTB,
							   CXTPSyntaxEditLexTextBlockPtr& rPtrTBParentToRun);

	virtual CXTPSyntaxEditLexTextBlockPtr InitScreenSch_RunTopClass(CTextIter* pTxtIter);

	virtual CXTPSyntaxEditLexTextBlock* CopyShortTBtoFull(CXTPSyntaxEditLexTextBlock* pTB);
	virtual CXTPSyntaxEditLexClass* FindLexClassByID(CXTPSyntaxEditLexClassPtrArray* pClassesAr,
													 int nClassID);

	virtual int Run_OnScreenTBStack(CTextIter* pTxtIter, CXTPSyntaxEditLexTextBlock* pTBParentToRun,
									CXTPSyntaxEditLexOnScreenParseCnt* pRunCnt);

	virtual void UpdateLastSchBlock(CXTPSyntaxEditLexTextBlock* pLastTB, BOOL bPermanently = FALSE);
	virtual CXTPSyntaxEditLexTextBlock* GetLastSchBlock(BOOL bWithAddRef = TRUE);

	virtual UINT SendEvent_OnTextBlockParsed(CXTPSyntaxEditLexTextBlock* pTB);

	virtual int Run_ParseUpdate0(BOOL bShort, CTextIter* pTxtIter, const XTP_EDIT_LINECOL* pLCStart,
								 const XTP_EDIT_LINECOL* pLCEnd, BOOL bSendProgressEvents);

	virtual int Run_ClassesUpdate1(CTextIter* pTxtIter, CXTPSyntaxEditLexTextBlockPtr ptrStartTB,
								   BOOL bStarted);

	virtual int Run_ClassesUpdate2(CTextIter* pTxtIter, CXTPSyntaxEditLexClassPtrArray* pArClasses,
								   CXTPSyntaxEditLexTextBlockPtr ptrParentTB,
								   CXTPSyntaxEditLexOnScreenParseCnt* pOnScreenRunCnt = NULL);

	virtual BOOL IsBlockStartStillHere(CTextIter* pTxtIter, CXTPSyntaxEditLexTextBlock* pTB);
	virtual void FinishNewChain(BOOL bByBreak, BOOL bEOF);
	virtual void UpdateNewChainParentsChildren();
	virtual void EndBlocksByParent(CXTPSyntaxEditLexTextBlock* pTBStart,
								   CXTPSyntaxEditLexTextBlock* pTBEnd);

	virtual void SeekNextEx(CTextIter* pTxtIter, CXTPSyntaxEditLexClass* pRunClass = NULL,
							CXTPSyntaxEditLexOnScreenParseCnt* pOnScreenRunCnt = NULL,
							int nChars										   = 1);

	static void AFX_CDECL AddClrBlock(XTP_EDIT_TEXTBLOCK& rClrB,
									  CXTPSyntaxEditTextBlockArray& arBlocks);

	//==== Run parse update context members ====
	CXTPSyntaxEditTextRegion m_curInvalidZone;

	CXTPSyntaxEditLexTextBlockPtr m_ptrNewChainTB1;
	CXTPSyntaxEditLexTextBlockPtr m_ptrNewChainTB2;
	CXTPSyntaxEditLexTextBlockPtr m_ptrOldChainTBFirst;

	typedef CMap<WPARAM, WPARAM, CXTPSyntaxEditLexTextBlockPtr, CXTPSyntaxEditLexTextBlockPtr&>
		CXTPSyntaxEditLexID2TBmap;

	CXTPSyntaxEditLexID2TBmap m_mapLastParsedBlocks;
	CXTPSyntaxEditLexTextBlockPtr m_ptrLastParsedBlock;
	BOOL m_bSendProgressEvents;

	int m_nNoEndedClassesCount;
	//==== END Run parse update context members ====

private:
	// CCriticalSection m_csData;
	// CCriticalSection m_csClassSch;
	CMutex m_lockData;
	CMutex m_lockClassSch;

	CEvent m_evBreakParsing;

	CXTPSyntaxEditLexClassSchema* m_pClassSchema;

	CXTPSyntaxEditLexTextBlockPtr m_ptrFirstBlock;
	CXTPSyntaxEditLexTextBlockPtr m_ptrLastSchBlock;

	int m_nSeekNext_TagWaitChars;

	CString m_strSchName;

	CXTPNotifyConnectionMT* m_pConnectMT;
};

//===========================================================================
// Summary:
//      This class used to 'run' lex schema parsing in a separate thread.
//      It also controls this working thread and provides inter-thread interaction.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexParser : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLexParser();

	//-----------------------------------------------------------------------
	// Summary:
	//     Object destructor. Handles clean up and deallocations.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexParser();

	//-------------------------------------------------------------------------
	// Summary:
	//     Stops parsing, destroys the working thread, and
	//     unadvises from all events.
	// See Also:
	//     CloseParseThread, StopParseInThread
	//-------------------------------------------------------------------------
	virtual void Close();

	//-------------------------------------------------------------------------
	// Summary:
	//     Stops parsing and destroys the working thread.
	// See Also:
	//     Close, StopParseInThread
	//-------------------------------------------------------------------------
	virtual void CloseParseThread();

	//-----------------------------------------------------------------------
	// Summary:
	//     Starts parsing a specified text range in a separate (working) thread.
	// Parameters:
	//     pBuffer         - Pointer to a CXTPSyntaxEditBufferManager object.
	//     pLCStart        - Pointer to the start text position.
	//                       Set to NULL to parse from the beginning.
	//     pLCEnd          - Pointer to the end text position.
	//                       Set to NULL to parse until the end.
	//     eEditAction     - Edit action; must be either 0 or one of the values
	//                       defined by the XTPSyntaxEditEditAction enumeration.
	//     bRunWithoutWait - TRUE to start parsing immediately,
	//                       FALSE to wait until the timeout
	//                       specified in global schema options.
	// Remarks:
	//     If the parser is already running and parsing another text range,
	//     then the specified text range will be added to a queue.
	// See Also:
	//     StopParseInThread, CloseParseThread
	//-----------------------------------------------------------------------
	virtual void StartParseInThread(CXTPSyntaxEditBufferManager* pBuffer,
									const XTP_EDIT_LINECOL* pLCStart = NULL,
									const XTP_EDIT_LINECOL* pLCEnd = NULL, int eEditAction = 0,
									BOOL bRunWithoutWait = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Stops parsing in thread.
	// See Also:
	//     Close, StartParseInThread
	//-------------------------------------------------------------------------
	virtual void StopParseInThread();

	//-------------------------------------------------------------------------
	// Summary:
	//     Stops parsing in thread before a call to OnEditChanged.
	// See Also:
	//     OnEditChanged
	//-------------------------------------------------------------------------
	virtual void OnBeforeEditChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reparses text after editing.
	// Parameters:
	//     posFrom     - Reference to the start position for the edit action.
	//     posTo       - Reference to the end position for the edit action.
	//     eEditAction - Edit action; must be one of the values defined by
	//                   the XTPSyntaxEditEditAction enumeration.
	//     pBuffer     - Pointer to a CXTPSyntaxEditBufferManager object.
	// See Also:
	//     OnBeforeEditChanged
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(const XTP_EDIT_LINECOL& posFrom, const XTP_EDIT_LINECOL& posTo,
							   int eEditAction, CXTPSyntaxEditBufferManager* pBuffer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the active text schema.
	// Returns:
	//     A pointer to the active text schema.
	// See Also:
	//     SetTextSchema
	//-----------------------------------------------------------------------
	virtual CXTPSyntaxEditLexTextSchema* GetTextSchema();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the active text schema.
	// Parameters:
	//     pTextSchema - Pointer to a text schema to be set.
	// See Also:
	//     GetTextSchema
	//-----------------------------------------------------------------------
	virtual void SetTextSchema(CXTPSyntaxEditLexTextSchema* pTextSchema);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the schema options.
	// Parameters:
	//     strExt - Reference to a file extension to lookup schema.
	// Returns:
	//     A pointer to the schema options.
	//-----------------------------------------------------------------------
	virtual const CXTPSyntaxEditLexParserSchemaOptions* GetSchemaOptions(const CString& strExt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the priority of the working thread.
	// Returns:
	//     The priority of the working thread.
	// See Also:
	//     SetParseThreadPriority, CWinThread::GetThreadPriority
	//-----------------------------------------------------------------------
	virtual int GetParseThreadPriority();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the priority for the working thread.
	// Parameters:
	//     nPriority - Priority to be set.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     GetParseThreadPriority, CWinThread::GetThreadPriority
	//-----------------------------------------------------------------------
	virtual BOOL SetParseThreadPriority(int nPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the auto-complete words of the active text schema.
	// Parameters:
	//     rArTokens - Reference to an array to receive the auto-complete
	//                 words of the active text schema.
	//     bAppend   - TRUE to append to the contents of the specified array,
	//                 FALSE to replace the contents of the specified array.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetTokensForAutoCompleate(CXTPSyntaxEditLexTokensDefArray& rArTokens,
										   BOOL bAppend);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the notification connection.
	// Returns:
	//     A pointer to the notification connection.
	//-----------------------------------------------------------------------
	virtual CXTPNotifyConnection* GetConnection();

protected:
	//{{AFX_CODEJOCK_PRIVATE
	class CXTPSyntaxEditParseThreadParams
	{
	public:
		CXTPSyntaxEditParseThreadParams();

		void AddParseZone(const CXTPSyntaxEditTextRegion& rZone);

		//---------------------------------
		CMutex lockThreadParams;
		CXTPSyntaxEditBufferManager* ptrBuffer;
		CXTPSyntaxEditInvalidZoneArray arInvalidZones;
		CEvent evParseRun;
		CEvent evRunWithoutWait;
		CEvent evExitThread;
	};

	virtual CXTPSyntaxEditParseThreadParams* GetParseInThreadParams();
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	virtual void SelfCloseParseThread();

	static UINT AFX_CDECL ThreadParseProc(LPVOID);

	CCriticalSection m_csParserData;
	CWinThread* m_pParseThread;
	int m_nParseThreadPriority; // Saved priority to the parse thread

	CXTPSyntaxEditParseThreadParams m_PThreadParams;

	CXTPSyntaxEditLexTextSchema* m_ptrTextSchema;

	typedef CMap<CString, LPCTSTR, CXTPSyntaxEditLexParserSchemaOptions*,
				 CXTPSyntaxEditLexParserSchemaOptions*>
		CXTPSyntaxEditLexParserSchemaOptionsMap;

	CXTPSyntaxEditLexParserSchemaOptionsMap m_mapSchOptions;
	CXTPSyntaxEditLexParserSchemaOptions* m_pSchOptions_default;

	CXTPNotifyConnection* m_pConnection;
	CXTPNotifySink* m_pSinkMT;
	//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
	virtual void OnParseEvent_NotificationHandler(XTP_NOTIFY_CODE Event, WPARAM wParam,
												  LPARAM lParam);

	virtual void GetStrsFromLVArray(CXTPSyntaxEditLexVariant* pLVArray,
									CStringArray& rArStrs) const;

	virtual BOOL ReadSchemaOptions(const CString& strExt, CXTPSyntaxEditLexTextSchema* pTextSchema,
								   CXTPSyntaxEditLexParserSchemaOptions* pOpt);
	//}}AFX_CODEJOCK_PRIVATE

private:
	void RemoveAllOptions();
};

//}}AFX_CODEJOCK_PRIVATE

////////////////////////////////////////////////////////////////////////////
AFX_INLINE const CXTPSyntaxEditTextRegion& CXTPSyntaxEditLexTextSchema::GetUpdatedTextRegion() const
{
	return m_curInvalidZone;
}

AFX_INLINE CMutex* CXTPSyntaxEditLexTextSchema::GetDataLoker()
{
	return &m_lockData;
}

AFX_INLINE CMutex* CXTPSyntaxEditLexTextSchema::GetClassSchLoker()
{
	return &m_lockClassSch;
}

AFX_INLINE CEvent* CXTPSyntaxEditLexTextSchema::GetBreakParsingEvent()
{
	return &m_evBreakParsing;
}

AFX_INLINE XTP_EDIT_LINECOL CXTPSyntaxEditLexTextBlock::GetPosEndLC(BOOL bMaxIfInvalid)
{
	if (!m_PosEndLC.IsValidData())
	{
		return bMaxIfInvalid ? XTP_EDIT_LINECOL::MAXPOS : XTP_EDIT_LINECOL::MINPOS;
	}
	return m_PosEndLC;
}

AFX_INLINE DWORD CXTPSyntaxEditLexTextBlock::GetStartTagEndXLC()
{
	return m_PosStartLC.GetXLC() + m_nStartTagLen;
}

AFX_INLINE DWORD CXTPSyntaxEditLexTextBlock::GetEndTagBeginXLC()
{
	return m_PosEndLC.GetXLC() - (m_nEndTagXLCLen > 0 ? m_nEndTagXLCLen : 0);
}

AFX_INLINE DWORD CXTPSyntaxEditLexTextBlock::GetEndTagEndXLC()
{
	return m_PosEndLC.GetXLC() - (m_nEndTagXLCLen < 0 ? m_nEndTagXLCLen : 0);
}
} // namespace XTPSyntaxEditLexAnalyser

////////////////////////////////////////////////////////////////////////////
//#pragma warning(pop)
////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLEXPARSER_H__)
