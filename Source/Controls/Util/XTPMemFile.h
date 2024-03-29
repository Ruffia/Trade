// XTPMemFile.h interface for the CXTPMemFile class.
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
#if !defined(__XTMEMFILE_H__)
#	define __XTMEMFILE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

//===========================================================================
// Summary:
//     CXTPMemFile is a CMemFile derived class. It is used to create a CXTPMemFile
//     object to support memory files.
// Remarks:
//     These memory files behave like disk files except that the file is stored
//     in RAM rather than on disk. A memory file is useful for fast temporary
//     storage or for transferring raw bytes or serialized objects between
//     independent processes.
//
//     CXTPMemFile objects can automatically allocate their own memory, or you
//     can attach your own memory block to the CXTPMemFile object by calling
//     Attach. In either case, memory for growing the memory file automatically
//     is allocated in nGrowBytes-sized increments if 'nGrowBytes' is not zero.
//
//     The memory block will automatically be deleted upon destruction of the
//     CXTPMemFile object if the memory was originally allocated by the CXTPMemFile
//     object. Otherwise, you are responsible for de-allocating the memory you
//     attached to the object.
//
//     You can access the memory block through the pointer supplied when you
//     detach it from the CXTPMemFile object by calling Detach.
//
//     The most common use of CXTPMemFile is to create a CXTPMemFile object and
//     use it by calling CFile member functions. Note that creating a CXTPMemFile
//     automatically opens it: you do not call CFile::Open, which is only used
//     for disk files. Because CXTPMemFile doesn't use a disk file, the data
//     member CFile::m_hFile is not used and has no meaning.
//
//     The CFile member functions Duplicate, LockRange, and UnlockRange are
//     not implemented for CXTPMemFile. If you call these functions on a CXTPMemFile
//     object, you will get a CNotSupportedException.
//
//     CXTPMemFile uses the run-time library functions malloc, realloc, and
//     free to allocate, reallocate, and deallocate memory and the intrinsic
//     memcpy to block copy memory when reading and writing. If you would like
//     to change this behavior or the behavior when CXTPMemFile grows a file,
//     derive your own class from CXTPMemFile and override the appropriate functions.
//===========================================================================
class _XTP_EXT_CLASS CXTPMemFile : public CMemFile
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMemFile object. This overload opens an empty memory
	//     file. Note that the file is opened by the constructor and that you
	//     should not call CFile::Open.
	// Parameters:
	//     nGrowBytes   - The memory allocation increment in bytes.
	//-----------------------------------------------------------------------
	CXTPMemFile(UINT nGrowBytes = 1024);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMemFile object. This overload opens an empty memory
	//     file. Note that the file is opened by the constructor and that you
	//     should not call CFile::Open.
	// Parameters:
	//     nGrowBytes   - The memory allocation increment in bytes.
	//     lpBuffer     - Pointer to the buffer to be attached to CXTPMemFile.
	//     nBufferSize  - An integer that specifies the size of the buffer in bytes.
	//-----------------------------------------------------------------------
	CXTPMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMemFile object. This overload opens an empty memory
	//     file. Note that the file is opened by the constructor and that you
	//     should not call CFile::Open.
	// Parameters:
	//     lpszFileName - A string that is the path to the desired file. The path
	//                    can be relative, absolute, or a network name (UNC).
	//     uiOpenFlags  - A UINT that defines the file's sharing and access mode.
	//                    It specifies the action to take when opening the file. You
	//                    can combine options by using the bitwise-OR (|) operator.
	//                    One access permission and one share option are required.
	//                    The modeCreate and modeNoInherit modes are optional. See the
	//                    CFile constructor for a list of mode options.
	//-----------------------------------------------------------------------
	CXTPMemFile(LPCTSTR lpszFileName, UINT uiOpenFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMemFile object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPMemFile();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function forces any data remaining in the file buffer
	//     to be written to the file. The use of Flush does not guarantee flushing
	//     of CArchive buffers. If you are using an archive, call CArchive::Flush
	//     first.
	//-----------------------------------------------------------------------
	virtual void Flush();

#	if _MSC_VER > 1200 // MFC 7.0
	using CMemFile::Open;
#	endif

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function opens and loads a physical file into memory.
	// Parameters:
	//     lpszFileName - Specifies a NULL-terminated string that is the path to
	//                    the desired file.
	//     nOpenFlags   - Specifies a UINT that defines the sharing and access mode in
	//                    the file. It specifies the action to take when opening the
	//                    file. You can combine options by using the bitwise-OR (|)
	//                    operator. One access permission and one share option are
	//                    required. The modeCreate and modeNoInherit modes are optional.
	//                    See the CFile constructor for a list of mode options.
	//     pError       - Specifies a pointer to an existing file-exception object
	//                    that receives the status of a failed operation.
	// Returns:
	//     true if successful, or false if it fails.
	//-----------------------------------------------------------------------
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the contents of the memory to the disk
	//     and closes it.
	//-----------------------------------------------------------------------
	virtual void Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function reads a string.
	// Parameters:
	//     rString - A CString reference to an object to receive the string
	//               that is read.
	// Returns:
	//     TRUE if successful, or FALSE if there is an error.
	//-----------------------------------------------------------------------
	virtual BOOL ReadString(CString& rString);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method writes data from a buffer to the file associated with
	//     the CArchive object. The terminating null character, \\0, is not written
	//     to the file, nor is a newline character automatically written.
	// Parameters:
	//     lpsz  - Specifies a pointer to a buffer containing a NULL-terminated text
	//             string.
	//-----------------------------------------------------------------------
	virtual void WriteString(LPCTSTR lpsz);

#	if _MSC_VER > 1200 // MFC 7.0
	using CMemFile::Duplicate;
#	endif // MFC 7.0

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will initialize the CXTPMemFile object with
	//     the information specified in the 'fDuplicate' object.
	// Parameters:
	//     strDup - A NULL-terminated string.
	//     fDuplicate - A pointer to a valid CFile object.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Duplicate(CFile* fDuplicate);
	virtual bool Duplicate(LPCTSTR strDup); // <combine CXTPMemFile::Duplicate@CFile* >

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function discards all changes to the file since Open()
	//     or last Flush().
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Discard();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function inserts any file and retrieves the length of
	//     the actual copied bytes.
	// Parameters:
	//     fSrc        - A pointer to a valid CFile object.
	//     strSrc      - Specifies a NULL-terminated string that is the path
	//                   to the desired file.
	//     dwSourcePos - Represents the source file position.
	//     dwDestPos   - Represents the destination file position.
	//     dwBytes     - Number of bytes to insert.
	// Returns:
	//     A DWORD value that represents the length of the copied bytes.
	//-----------------------------------------------------------------------
	virtual DWORD Insert(CFile* fSrc, DWORD dwSourcePos, DWORD dwDestPos, DWORD dwBytes);
	virtual DWORD Insert(LPCTSTR strSrc, DWORD dwSourcePos, DWORD dwDestPos,
						 DWORD dwBytes); // <combine CXTPMemFile::Insert@CFile*@DWORD@DWORD@DWORD>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function extracts bytes to a file and retrieves the
	//     length of the actual copied bytes.
	// Parameters:
	//     fDest      - A pointer to a valid CFile object.
	//     strDest    - Specifies a NULL-terminated string that is the path
	//                  to the desired file.
	//     dwStartPos - Represents the starting position.
	//     dwBytes    - Number of bytes to extract.
	// Returns:
	//     A DWORD value that represents the length of the copied bytes.
	//-----------------------------------------------------------------------
	virtual DWORD Extract(CFile* fDest, DWORD dwStartPos, DWORD dwBytes);
	virtual DWORD Extract(LPCTSTR strDest, DWORD dwStartPos,
						  DWORD dwBytes); // <combine CXTPMemFile::Extract@CFile*@DWORD@DWORD>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function finds data in the file.
	// Parameters:
	//     pData     - Pointer to the buffer to receive the data found.
	//     dwDataLen - Size of the data to find.
	//     lStartPos - Starting position.
	// Returns:
	//     A LONG data type.
	//-----------------------------------------------------------------------
	LONG FindData(void* pData, DWORD dwDataLen, LONG lStartPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will initialize the CXTPMemFile object with
	//     the object specified by 'fDup'.
	// Parameters:
	//     fDup - A pointer to a valid CFile object.
	//-----------------------------------------------------------------------
	void operator=(CFile* fDup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will initialize the CXTPMemFile object with
	//     the object specified by 'strDup'.
	// Parameters:
	//     strDup - Specifies a NULL-terminated string that is the path to
	//              the desired file.
	//-----------------------------------------------------------------------
	void operator=(CString strDup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will adjust the file position.
	// Parameters:
	//     dwFilePos - A DWORD value that specifies the file position.
	//-----------------------------------------------------------------------
	void operator=(DWORD dwFilePos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will append the CXTPMemFile object with the
	//     object specified by 'fApp'.
	// Parameters:
	//     fApp - A pointer to a valid CFile object.
	//-----------------------------------------------------------------------
	void operator+=(CFile* fApp);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will append the CXTPMemFile object with the
	//     object specified by 'strApp'.
	// Parameters:
	//     strApp - Specifies a NULL-terminated string that is the path to
	//              the desired file.
	//-----------------------------------------------------------------------
	void operator+=(CString strApp);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member operator will perform indexing operations for the
	//     CXTPMemFile object.
	// Parameters:
	//     dwFilePos - A DWORD value that specifies the file position.
	// Returns:
	//     A BYTE data type.
	//-----------------------------------------------------------------------
	BYTE operator[](DWORD dwFilePos);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads the file into memory.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Load();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the file to disk.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Save();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function imports the data of a CFile derived object
	//     (operator=).
	// Parameters:
	//     fImp - A pointer to a valid CFile object.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Import(CFile* fImp);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function appends a CFile derived object to the file
	//     (operator+=).
	// Parameters:
	//     fApp - A pointer to a valid CFile object.
	// Returns:
	//     true if successful, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool Append(CFile* fApp);

private:
	// Unsupported APIs
	virtual CFile* Duplicate() const;

private:
	UINT m_uiOpenFlags;
	bool m_bOpen;
	CFile m_File;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // #if !defined(__XTMEMFILE_H__)
