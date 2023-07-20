// XTPSyntaxEditLineMarksManager.h : header file
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
#if !defined(__XTPSYNTAXEDITLINEMARKSMANAGER_H__)
#	define __XTPSYNTAXEDITLINEMARKSMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

typedef LPCTSTR XTP_EDIT_LINEMARKTYPE;

// You can define your own line mark types (as string constants)
// Strings are case sensitive!
//
static const XTP_EDIT_LINEMARKTYPE xtpEditLMT_Bookmark   = _T("Bookmark");
static const XTP_EDIT_LINEMARKTYPE xtpEditLMT_Breakpoint = _T("Breakpoint");
static const XTP_EDIT_LINEMARKTYPE xtpEditLMT_Collapsed  = _T("Collapsed");

//===========================================================================
// Summary:
//     Enumerates types of mark refreshing
//===========================================================================
enum XTPSyntaxEditLineMarksRefreshType
{
	xtpEditLMRefresh_Unknown = 0,	// Unknown refresh state.
	xtpEditLMRefresh_Insert  = 0x01, // Mark inserted.
	xtpEditLMRefresh_Delete  = 0x02, // Mark deleted.

	xtpEditLMRefresh_Delete_only1 = 0x10, // Delete mark for first row of deleted text block.
	xtpEditLMRefresh_Delete_only2 = 0x20, // Delete mark for last row of deleted text block.
	xtpEditLMRefresh_InsertAt0	= 0x40, // Move mark for first row of inserted text block.
};

//{{AFX_CODEJOCK_PRIVATE

//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditVoidObj : public CXTPCmdTarget
{
public:
	typedef void(AFX_CDECL* TPFDeleter)(void*);

protected:
	void* m_pPtr; // A pointer to a handled object
	TPFDeleter m_pfDeleter;

public:
	//-----------------------------------------------------------------------
	// Parameters:  pPtr                : [in]Pointer to the handled object.
	//              pfDeleter           : [in]pointer to function which should
	//                                delete object in pPtr.
	//                                By default this parameter is NULL.
	// Summary: Default class constructor.
	// See Also: ~CXTPSmartPtrInternalT()
	//-----------------------------------------------------------------------
	CXTPSyntaxEditVoidObj(void* pPtr, TPFDeleter pfDeleter = NULL)
	{
		m_pPtr		= pPtr;
		m_pfDeleter = pfDeleter;
	};

	//-----------------------------------------------------------------------
	// Summary: Default class destructor.
	// Remarks: Call InternalRelease() for the not NULL handled object.
	// See Also: CXTPSmartPtrInternalT constructors
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditVoidObj()
	{
		if (m_pfDeleter)
		{
			m_pfDeleter(m_pPtr);
		}
		else
		{
			SAFE_DELETE(m_pPtr);
		}
	};

	//-----------------------------------------------------------------------
	// Summary: Get a handled object.
	// Returns: Pointer to the handled object.
	//-----------------------------------------------------------------------
	operator void*() const
	{
		return m_pPtr;
	}

	void* GetPtr() const
	{
		return m_pPtr;
	}

	//-----------------------------------------------------------------------
	// Summary: Check is handled object equal NULL.
	// Returns: TRUE if handled object equal NULL, else FALSE.
	//-----------------------------------------------------------------------
	BOOL operator!() const
	{
		return !m_pPtr;
	}
};
//---------------------------------------------------------------------------
typedef CXTPSmartPtrInternalT<CXTPSyntaxEditVoidObj> CXTPSyntaxEditVoidObjPtr;

//===========================================================================
enum XTPSyntaxEditLMParamType
{
	xtpEditLMPT_Unknown = 0, // unknown refresh state
	xtpEditLMPT_DWORD   = 1,
	xtpEditLMPT_double  = 2,
	xtpEditLMPT_Ptr		= 3,
};

struct _XTP_EXT_CLASS XTP_EDIT_LMPARAM
{
	// Data type
	XTPSyntaxEditLMParamType m_eType;

	// Data
	union {
		DWORD m_dwValue;   // xtpEditLMPT_DWORD
		double m_dblValue; // xtpEditLMPT_double
	};

protected:
	CXTPSyntaxEditVoidObjPtr m_Ptr;

public:
	// END Data

	XTP_EDIT_LMPARAM();
	virtual ~XTP_EDIT_LMPARAM();

	XTP_EDIT_LMPARAM(const XTP_EDIT_LMPARAM& rSrc);

	XTP_EDIT_LMPARAM(DWORD dwVal);
	XTP_EDIT_LMPARAM(double dblValue);
	XTP_EDIT_LMPARAM(void* pPtr);

	const XTP_EDIT_LMPARAM& operator=(const XTP_EDIT_LMPARAM& rSrc);

	const XTP_EDIT_LMPARAM& operator=(DWORD dwValue);
	const XTP_EDIT_LMPARAM& operator=(double dblValue);
	const XTP_EDIT_LMPARAM& operator=(void* pPtr);

	operator DWORD() const;
	operator double() const;
	operator void*() const;

	void SetPtr(void* pPtr, CXTPSyntaxEditVoidObj::TPFDeleter pfDeleter = NULL);

	void* GetPtr() const;

	BOOL IsValid() const;

	void Clear();
};

//===========================================================================
struct _XTP_EXT_CLASS XTP_EDIT_LMDATA
{
	int m_nRow;
	XTP_EDIT_LMPARAM m_Param;

	//-------------------------------
	XTP_EDIT_LMDATA();
	virtual ~XTP_EDIT_LMDATA();

	XTP_EDIT_LMDATA(const XTP_EDIT_LMDATA& rSrc);
	const XTP_EDIT_LMDATA& operator=(const XTP_EDIT_LMDATA& rSrc);
};

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     The CXTPSyntaxEditLineMarksManager class provides functionality to
//     manipulate marks on text lines. Used internally by the Smart Edit control.
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLineMarksManager : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPSyntaxEditLineMarksManager)
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default object constructor.
	//-----------------------------------------------------------------------
	CXTPSyntaxEditLineMarksManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditLineMarksManager object, handles
	//     cleanup and de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLineMarksManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds/removes a specified mark to/from a specified row.
	// Parameters:
	//     nRow    - [in] Row number.
	//     lmType  - [in] Mark type identifier.
	//     pParam  - [in] Pointer to XTP_EDIT_LMPARAM.
	//                    The default value for this parameter is NULL.
	// Remarks:
	//     If the row does not have the specified mark, then the mark
	//     will be added.
	//     If the row does have the specified mark, then the mark
	//     will be deleted.
	// See Also:
	//     XTP_EDIT_LMPARAM
	//-----------------------------------------------------------------------
	void AddRemoveLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType,
						   XTP_EDIT_LMPARAM* pParam = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a specified mark to a specified row.
	// Parameters:
	//     nRow   - [in] Row number.
	//     lmType - [in] Mark type identifier.
	//     pParam - [in] Pointer to XTP_EDIT_LMPARAM.
	//                   The default value for this parameter is NULL.
	// See Also:
	//     XTP_EDIT_LMPARAM
	//-----------------------------------------------------------------------
	void SetLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType, XTP_EDIT_LMPARAM* pParam = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a specified mark from a specified row.
	// Parameters:
	//     nRow   - [in] Row number.
	//     lmType - [in] Mark type identifier.
	//-----------------------------------------------------------------------
	void DeleteLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if a specified row has a specified mark.
	// Parameters:
	//     nRow   - [in] Row number.
	//     lmType - [in] Mark type identifier.
	//     pParam - [in] Pointer to XTP_EDIT_LMPARAM.
	//                   The default value for this parameter is NULL.
	// Returns:
	//     TRUE if the specified row has the specified mark, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasRowMark(int nRow, const XTP_EDIT_LINEMARKTYPE& lmType,
					XTP_EDIT_LMPARAM* pParam = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the row with a specified mark type
	//     that precedes a specified row.
	// Parameters:
	//     nRow   - [in] Row number.
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     The POSITION of the row with the specified mark type
	//     that precedes the specified row.
	//-----------------------------------------------------------------------
	POSITION FindPrevLineMark(int& nRow, const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the row with a specified mark type
	//     that follows a specified row.
	// Parameters:
	//     nRow   - [in] Row number.
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     The POSITION of the row with the specified mark type
	//     that follows the specified row.
	//-----------------------------------------------------------------------
	POSITION FindNextLineMark(int& nRow, const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the last row with a specified mark type.
	// Parameters:
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     The POSITION of the last row with the specified mark type.
	//-----------------------------------------------------------------------
	POSITION GetLastLineMark(const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the position of the first row with a specified mark type.
	// Parameters:
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     The POSITION of the first row with the specified mark type.
	//-----------------------------------------------------------------------
	POSITION GetFirstLineMark(const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the row with a specified mark type
	//     that follows a specified position.
	// Parameters:
	//     pos    - [in] Reference to a POSITION value.
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     A pointer to an XTP_EDIT_LMDATA structure that contains
	//     the identifier of the row with the specified mark type
	//     that follows the specified position.
	// See Also:
	//     XTP_EDIT_LMDATA
	//-----------------------------------------------------------------------
	XTP_EDIT_LMDATA* GetNextLineMark(POSITION& pos, const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the identifier of the row with a specified mark type
	//     at a specified position.
	// Parameters:
	//     pos    - [in] POSITION value.
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     A pointer to an XTP_EDIT_LMDATA structure that contains
	//     the identifier of the row with the specified mark type
	//     at the specified position.
	// See Also:
	//     XTP_EDIT_LMDATA
	//-----------------------------------------------------------------------
	XTP_EDIT_LMDATA* GetLineMarkAt(const POSITION pos, const XTP_EDIT_LINEMARKTYPE lmType) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the line marks for a specified range of rows.
	// Parameters:
	//     nRowFrom     - [in] Start row.
	//     nRowTo       - [in] End row.
	//     nRefreshType - [in] Refresh type; must be one of the values defined by
	//                         the XTPSyntaxEditLineMarksRefreshType enumeration.
	// See Also:
	//     XTPSyntaxEditLineMarksRefreshType.
	//-----------------------------------------------------------------------
	virtual void RefreshLineMarks(int nRowFrom, int nRowTo, int nRefreshType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all line marks of a specified type.
	// Parameters:
	//     lmType - [in] Mark type identifier.
	//-----------------------------------------------------------------------
	void RemoveAll(const XTP_EDIT_LINEMARKTYPE lmType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of line marks of a specified type.
	// Parameters:
	//     lmType - [in] Mark type identifier.
	// Returns:
	//     The number of line marks of the specified type.
	//-----------------------------------------------------------------------
	int GetCount(const XTP_EDIT_LINEMARKTYPE lmType) const;

private:
	//===========================================================================
	// Summary:
	//     This internal helper class is designed to store a sorted list of
	//     line marks data objects. This class is based on CArray and provides
	//     typical operations like fast binary searching inside it and basic
	//     line marks manipulations.
	//===========================================================================
	class CLineMarksList : public CXTPCmdTarget
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Default object destructor.
		//-----------------------------------------------------------------------
		virtual ~CLineMarksList();

		//-----------------------------------------------------------------------
		// Summary:
		//     Adds a specified element to the list.
		// Parameters:
		//     lmData - [in] Reference to an XTP_EDIT_LMDATA structure.
		// See Also:
		//     XTP_EDIT_LMDATA.
		//-----------------------------------------------------------------------
		void Add(const XTP_EDIT_LMDATA& lmData);

		//-----------------------------------------------------------------------
		// Summary:
		//     Removes the element with a specified key value from the list.
		// Parameters:
		//     nKey - [in] Key value.
		//-----------------------------------------------------------------------
		void Remove(const int nKey);

		//-----------------------------------------------------------------------
		// Summary:
		//     Removes all elements from the list.
		//-----------------------------------------------------------------------
		void RemoveAll();

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the number of elements in the list.
		// Returns:
		//     The number of elements in the list.
		//-----------------------------------------------------------------------
		int GetCount() const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the position of the element with a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The POSITION of the element with the specified key value.
		//-----------------------------------------------------------------------
		POSITION FindAt(int nKey) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the position of the element with the key value following
		//     a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The POSITION of the element with the key value following
		//     the specified key value.
		//-----------------------------------------------------------------------
		POSITION FindNext(int nKey) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the position of the element with the key value preceding
		//     a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The POSITION of the element with the key value preceding
		//     the specified key value.
		//-----------------------------------------------------------------------
		POSITION FindPrev(int nKey) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Refreshes the line marks for a specified range of rows.
		// Parameters:
		//     nRowFrom     - [in] Start row.
		//     nRowTo       - [in] End row.
		//     nRefreshType - [in] Refresh type; must be one of the values defined by
		//                         the XTPSyntaxEditLineMarksRefreshType enumeration.
		// See Also:
		//     XTPSyntaxEditLineMarksRefreshType
		//-----------------------------------------------------------------------
		void RefreshLineMarks(int nRowFrom, int nRowTo, int nRefreshType);

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the identifier of the row at a specified position.
		// Parameters:
		//     pos - [in] POSITION value.
		// Returns:
		//     A pointer to an XTP_EDIT_LMDATA structure that contains
		//     the identifier of the row at the specified position.
		//-----------------------------------------------------------------------
		XTP_EDIT_LMDATA* GetLineMarkAt(const POSITION pos) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the position of the first row with a mark.
		// Returns:
		//     The POSITION of the first row with a mark.
		//-----------------------------------------------------------------------
		POSITION GetFirstLineMark() const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the identifier of the row following a specified position.
		// Parameters:
		//     pos - [in] Reference to a POSITION value.
		// Returns:
		//     A pointer to an XTP_EDIT_LMDATA structure that contains
		//     the identifier of the row following the specified position.
		//-----------------------------------------------------------------------
		XTP_EDIT_LMDATA* GetNextLineMark(POSITION& pos) const;

	private:
		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the index of the element with a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The index of the element with the specified key value
		//     if found, otherwise -1.
		//-----------------------------------------------------------------------
		int FindIndex(const int nKey) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the index of the element with the key value preceding
		//     a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The index of the element with the key value preceding
		//     the specified key value if found, otherwise -1.
		//-----------------------------------------------------------------------
		int FindLowerIndex(const int nKey) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the index of the element with the key value following
		//     a specified key value.
		// Parameters:
		//     nKey - [in] Key value.
		// Returns:
		//     The index of the element with the key value following
		//     the specified key value if found, otherwise -1.
		//-----------------------------------------------------------------------
		int FindUpperIndex(const int nKey) const;

	private:
		typedef CArray<XTP_EDIT_LMDATA*, XTP_EDIT_LMDATA*> CXTPSyntaxEditLineMarkPointersArray;
		CXTPSyntaxEditLineMarkPointersArray m_array; // The array with the actual line data.
	};

public:
	typedef CXTPSmartPtrInternalT<CLineMarksList> CLineMarksListPtr; // SmartPointer for the class.
private:
	//===========================================================================
	// Summary:
	//      This class contains a map of line marks lists corresponding to the
	//      string values of line marks types.
	//      It allows for adding a new list with the new line mark type
	//      and retrieving a smart pointer of the requested list by its
	//      line mark type.
	//===========================================================================
	class CLineMarksListsMap
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Gets the list associated with a specified line mark type string.
		// Parameters:
		//     szMarkType - [in] Line mark type string.
		// See Also:
		//     CLineMarksListPtr
		//-----------------------------------------------------------------------
		CLineMarksListPtr GetList(LPCTSTR szMarkType) const;

		//-----------------------------------------------------------------------
		// Summary:
		//     Adds a new list associated with a specified line mark type string.
		// Parameters:
		//     szMarkType - [in] Line mark type string.
		// Returns:
		//     A pointer to the newly added list.
		// See Also:
		//     CLineMarksListPtr
		//-----------------------------------------------------------------------
		CLineMarksListPtr AddList(LPCTSTR szMarkType);

		//-----------------------------------------------------------------------
		// Summary:
		//     Refreshes the line marks for a specified range of rows.
		// Parameters:
		//     nRowFrom     - [in] Start row.
		//     nRowTo       - [in] End row.
		//     nRefreshType - [in] Refresh type; must be one of the values defined by
		//                         the XTPSyntaxEditLineMarksRefreshType enumeration.
		// See Also:
		//     XTPSyntaxEditLineMarksRefreshType
		//-----------------------------------------------------------------------
		void RefreshLineMarks(int nRowFrom, int nRowTo, int nRefreshType);

	private:
		CMap<CString, LPCTSTR, CLineMarksListPtr, CLineMarksListPtr&> m_map; // A map containing
																			 // line marks lists for
																			 // every mark type.
	};

	CLineMarksListsMap m_mapLists; // A collection of line marks lists for all line mark types.

protected:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPSyntaxEditLineMarksManager)

	afx_msg void OleAddRemoveLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg void OleSetLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg void OleDeleteLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg BOOL OleHasRowMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg POSITION OleFindPrevLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg POSITION OleFindNextLineMark(int nRow, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg POSITION OleGetLastLineMark(const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg POSITION OleGetFirstLineMark(const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg long OleGetLineMarkAt(const POSITION pos, const XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg void OleRemoveAll(XTP_EDIT_LINEMARKTYPE lmType);
	afx_msg long OleGetCount(XTP_EDIT_LINEMARKTYPE lmType);

	//}}AFX_CODEJOCK_PRIVATE
#	endif
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLINEMARKSMANAGER_H__)
