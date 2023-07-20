// XTPSyntaxEditTextIterator.h
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
#if !defined(__XTPSYNTAXEDITTEXTITERATOR_H__)
#	define __XTPSYNTAXEDITTEXTITERATOR_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditBufferManager;

//===========================================================================
// Summary:
//     This class provides functionality to navigate text in the data
//     buffer, seek forward/backward, retrieve text by lines, and
//     determine control symbols like CRLF.
//     When you create an instance of the CXTPSyntaxEditTextIterator class,
//     you should provide a pointer to data storage, represented by
//     the CXTPSyntaxEditBufferManager class.
// See Also:
//      CXTPSyntaxEditBufferManager.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditTextIterator : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pData - [in] Pointer to a CXTPSyntaxEditBufferManager object.
	// See Also:
	//     CXTPSyntaxEditBufferManager.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditTextIterator(CXTPSyntaxEditBufferManager* pData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditTextIterator object,
	//     handles cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditTextIterator(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current CRLF control symbol.
	// Returns:
	//     The current CRLF control symbol.
	// Remarks:
	//     Strings such as "\\r\\n", "\\n\\r", and "\\r" are
	//     recognized as CRLF control symbols.
	//-----------------------------------------------------------------------
	virtual CString GetEOL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions the "current position pointer" in the internal data
	//     buffer to the begin.
	// Remarks:
	//     Call this member function to set the "current position pointer"
	//     in the internal data buffer to the begin. Besides it resets
	//     all others internal text counters.
	// See Also:
	//     SeekPos()
	//-----------------------------------------------------------------------
	virtual void SeekBegin();

	//-----------------------------------------------------------------------
	// Summary:
	//     Repositions the "current position pointer" in the internal data
	//     buffer to the specified position.
	// Parameters:
	//     posLC       - [in] Position in the text to set as current.
	//     hBreakEvent - [in] Handle to the break event to stop function execution.
	// Remarks:
	//     Call this member function to set the "current position pointer"
	//     in the internal data buffer to the specified position.
	//     For big texts this operation may take a lot of time. hBreakEvent
	//     is used to to stop function execution without reaching the specified position.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// See Also:
	//     SeekBegin()
	//-----------------------------------------------------------------------
	virtual BOOL SeekPos(const XTP_EDIT_LINECOL& posLC, HANDLE hBreakEvent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns text of required length
	// Parameters:
	//     nCharsBuf - [in] length of the text in bytes to be returned.
	// Remarks:
	//     Call this member function to get text of required length.
	//     CRLF symbols are taken in account and don't remove from the
	//     returned string.
	// Returns:
	//     Pointer to the character string.
	//-----------------------------------------------------------------------
	virtual LPCTSTR GetText(int nCharsBuf = 512);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves current position and return pointer to the text begin.
	// Parameters:
	//     dwChars   - [in] counts to move.
	//     nCharsBuf - [in] length of the text in bytes to be returned.
	// Returns:
	//     Pointer to the character string.
	//-----------------------------------------------------------------------
	virtual LPCTSTR SeekNext(DWORD dwChars = 1, int nCharsBuf = 512);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves current position back for 1 character, and return pointer
	//     to the text begin.
	// Returns:
	//     Pointer to the character string or NULL if text begin is reached.
	//-----------------------------------------------------------------------
	virtual LPCTSTR SeekPrev();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the end of data has been reached.
	// Returns:
	//     Nonzero if either the data buffer contains no records or if
	//     the user has scrolled beyond the last position, otherwise 0.
	// Remarks:
	//     IsEOF can also be used to determine if the data buffer
	//     contains any data or is empty.
	//-----------------------------------------------------------------------
	virtual BOOL IsEOF() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the current position.
	// Returns:
	//     An XTP_EDIT_LINECOL structure containing the current position.
	// See Also:
	//     XTP_EDIT_LINECOL
	//-----------------------------------------------------------------------
	virtual XTP_EDIT_LINECOL GetPosLC() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last position.
	// Parameters:
	//     bWithEOL - TRUE to include EOL, FALSE otherwise.
	// Returns:
	//     An XTP_EDIT_LINECOL structure containing the last position.
	// See Also:
	//     XTP_EDIT_LINECOL
	//-----------------------------------------------------------------------
	virtual XTP_EDIT_LINECOL GetPosLC_last(BOOL bWithEOL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Decrements position.
	// Parameters:
	//     rLC - [in] Reference to an XTP_EDIT_LINECOL structure.
	//-----------------------------------------------------------------------
	virtual void LCPosDec(XTP_EDIT_LINECOL& rLC) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Increments position.
	// Parameters:
	//     rLC       - [in] Reference to an XTP_EDIT_LINECOL structure.
	//     nCharsAdd - [in] Number of characters to add.
	//-----------------------------------------------------------------------
	virtual void LCPosAdd(XTP_EDIT_LINECOL& rLC, int nCharsAdd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length of a specified line.
	// Parameters:
	//     nLine    - [in] Identifier of the line.
	//     bWithEOL - [in] TRUE to include end-of-line characters,
	//                     FALSE to ignore end-of-line characters.
	// Returns:
	//     The length of the specified line.
	//-----------------------------------------------------------------------
	virtual int GetLineLen(int nLine, BOOL bWithEOL = FALSE) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the length (in bytes) of a specified line.
	// Parameters:
	//     nLine    - [in] Identifier of the line.
	//     bWithEOL - [in] TRUE to include end-of-line characters,
	//                     FALSE to ignore end-of-line characters.
	// Returns:
	//     The length (in bytes) of the specified line.
	//-----------------------------------------------------------------------
	virtual int GetLineLenBytes(int nLine, BOOL bWithEOL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the offset value.
	// Parameters:
	//     nOffsetChars - [in] Offset value to be set.
	//-----------------------------------------------------------------------
	virtual void SetTxtOffset(int nOffsetChars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the file name.
	// Returns:
	//     The file name.
	//-----------------------------------------------------------------------
	virtual CString GetFileExt() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the maximum buffer size (in characters) for back text offset.
	// Returns:
	//     The maximum buffer size (in characters) for back text offset.
	// See Also:
	//     SetTxtOffset
	//-----------------------------------------------------------------------
	virtual int GetMaxBackOffset() const;

protected:
	CXTPSyntaxEditBufferManager* m_pData; // Pointer to the used CXTPSyntaxEditBufferManager object.

	XTP_EDIT_LINECOL m_LCpos; // Current position in the text.
	BOOL m_bEOF;			  // End-Of-File flag.

	CMap<DWORD, DWORD, int, int> m_mapLine2Len; // Map of line - length of line.

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the buffer.
	// Parameters:
	//     nOffsetB - [in] Offset (in bytes).
	// Returns:
	//     A pointer to the TCHAR buffer.
	//-----------------------------------------------------------------------
	TCHAR* GetBuffer(int nOffsetB) const;

	CArray<CHAR, CHAR> m_arBuffer; // Buffer of chars.
	int m_nBufSizeB;			   // Buffer length.
	int m_nNextLine;			   // Next line ID.

	int m_nBufOffsetB; // Offset.

	int m_nBufOffsetB_normal; // Offset.
	int m_nBufOffsetB_max;	// Offset.

	int m_nTmpOffsetC; // Offset.
	int m_nTmpOffsetB; // Offset.
};

////////////////////////////////////////////////////////////////////////////
AFX_INLINE TCHAR* CXTPSyntaxEditTextIterator::GetBuffer(int nOffsetB) const
{
	return (TCHAR*)((CHAR*)m_arBuffer.GetData() + nOffsetB);
}
AFX_INLINE int CXTPSyntaxEditTextIterator::GetMaxBackOffset() const
{
	return m_nBufOffsetB_normal;
}

////////////////////////////////////////////////////////////////////////////

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITTEXTITERATOR_H__)
