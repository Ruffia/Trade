// XTPSyntaxEditLexClassSubObjT.h
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
#if !defined(__XTPSYNTAXEDITLEXCLASSSUBOBJT_H__)
#	define __XTPSYNTAXEDITLEXCLASSSUBOBJT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#	pragma warning(disable : 4097)

// #define DBG_AUTOMAT

namespace XTPSyntaxEditLexAnalyser
{
//{{AFX_CODEJOCK_PRIVATE
//===========================================================================
class CXTPSyntaxEditLexNULL
{
public:
	LPCTSTR GetName() const
	{
		return NULL;
	};

	BOOL IsEmpty() const
	{
		return TRUE;
	}

	BOOL IfMy(LPCTSTR pcszElementName)
	{
		UNREFERENCED_PARAMETER(pcszElementName);
		return FALSE;
	}

	BOOL AddIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtr& ptr)
	{
		UNREFERENCED_PARAMETER(pcszElementName);
		UNREFERENCED_PARAMETER(ptr);
		return FALSE;
	}

	BOOL AppendIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtrArray& ptrAr)
	{
		UNREFERENCED_PARAMETER(pcszElementName);
		UNREFERENCED_PARAMETER(ptrAr);
		return FALSE;
	}

	CXTPSyntaxEditLexVariantPtrArray* GetIfMy(LPCTSTR pcszElementName)
	{
		UNREFERENCED_PARAMETER(pcszElementName);
		return NULL;
	}

	void CopyFrom(CXTPSyntaxEditLexNULL& rSrc)
	{
		UNREFERENCED_PARAMETER(rSrc);
	};

	void RemoveAll(){};
#	ifdef _DEBUG
	void DumpOffset(CDumpContext& dc, LPCTSTR pcszOffset)
	{
		dc << pcszOffset;
	};
#	endif
};

//---------------------------------------------------------------------------
template<class _TC0 = CXTPSyntaxEditLexNULL, class _TC1 = CXTPSyntaxEditLexNULL,
		 class _TC2 = CXTPSyntaxEditLexNULL, class _TC3 = CXTPSyntaxEditLexNULL,
		 class _TC4 = CXTPSyntaxEditLexNULL, class _TC5 = CXTPSyntaxEditLexNULL,
		 class _TC6 = CXTPSyntaxEditLexNULL, class _TC7 = CXTPSyntaxEditLexNULL,
		 class _TC8 = CXTPSyntaxEditLexNULL, class _TC9 = CXTPSyntaxEditLexNULL>

class CXTPSyntaxEditLexObj_SpecCollT
{
public:
	typedef CXTPSyntaxEditLexObj_SpecCollT<_TC0, _TC1, _TC2, _TC3, _TC4, _TC5, _TC6, _TC7, _TC8,
										   _TC9>
		Tthis;

	typedef _TC0 _Type0;
	typedef _TC1 _Type1;
	typedef _TC2 _Type2;
	typedef _TC3 _Type3;
	typedef _TC4 _Type4;
	typedef _TC5 _Type5;
	typedef _TC6 _Type6;
	typedef _TC7 _Type7;
	typedef _TC8 _Type8;
	typedef _TC9 _Type9;

	CXTPSyntaxEditLexObj_SpecCollT()
	{
		m_bEmpty = TRUE;

		SetSubMembers();
	};

	virtual ~CXTPSyntaxEditLexObj_SpecCollT(){};

protected:
	BOOL m_bEmpty;

	_TC0* m_p0;
	_TC1* m_p1;
	_TC2* m_p2;
	_TC3* m_p3;
	_TC4* m_p4;
	_TC5* m_p5;
	_TC6* m_p6;
	_TC7* m_p7;
	_TC8* m_p8;
	_TC9* m_p9;

	virtual void SetSubMember0()
	{
		m_p0 = NULL;
	};
	virtual void SetSubMember1()
	{
		m_p1 = NULL;
	};
	virtual void SetSubMember2()
	{
		m_p2 = NULL;
	};
	virtual void SetSubMember3()
	{
		m_p3 = NULL;
	};
	virtual void SetSubMember4()
	{
		m_p4 = NULL;
	};
	virtual void SetSubMember5()
	{
		m_p5 = NULL;
	};
	virtual void SetSubMember6()
	{
		m_p6 = NULL;
	};
	virtual void SetSubMember7()
	{
		m_p7 = NULL;
	};
	virtual void SetSubMember8()
	{
		m_p8 = NULL;
	};
	virtual void SetSubMember9()
	{
		m_p9 = NULL;
	};

	virtual void SetSubMembers()
	{
		SetSubMember0();
		SetSubMember1();
		SetSubMember2();
		SetSubMember3();
		SetSubMember4();
		SetSubMember5();
		SetSubMember6();
		SetSubMember7();
		SetSubMember8();
		SetSubMember9();
	};

public:
	BOOL IsEmpty() const
	{
		return m_bEmpty;
	}

	BOOL IfMy(LPCTSTR pcszElementName)
	{
		if (m_p0 && m_p0->IfMy(pcszElementName))
		{
		}
		else if (m_p1 && m_p1->IfMy(pcszElementName))
		{
		}
		else if (m_p2 && m_p2->IfMy(pcszElementName))
		{
		}
		else if (m_p3 && m_p3->IfMy(pcszElementName))
		{
		}
		else if (m_p4 && m_p4->IfMy(pcszElementName))
		{
		}
		else if (m_p5 && m_p5->IfMy(pcszElementName))
		{
		}
		else if (m_p6 && m_p6->IfMy(pcszElementName))
		{
		}
		else if (m_p7 && m_p7->IfMy(pcszElementName))
		{
		}
		else if (m_p8 && m_p8->IfMy(pcszElementName))
		{
		}
		else if (m_p9 && m_p9->IfMy(pcszElementName))
		{
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL AddIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtr& ptr)
	{
		if (m_p0 && m_p0->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p1 && m_p1->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p2 && m_p2->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p3 && m_p3->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p4 && m_p4->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p5 && m_p5->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p6 && m_p6->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p7 && m_p7->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p8 && m_p8->AddIfMy(pcszElementName, ptr))
		{
		}
		else if (m_p9 && m_p9->AddIfMy(pcszElementName, ptr))
		{
		}
		else
		{
			return FALSE;
		}
		m_bEmpty = FALSE;
		return TRUE;
	}

	BOOL AppendIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtrArray& ptrAr)
	{
		if (m_p0 && m_p0->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p1 && m_p1->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p2 && m_p2->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p3 && m_p3->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p4 && m_p4->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p5 && m_p5->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p6 && m_p6->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p7 && m_p7->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p8 && m_p8->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else if (m_p9 && m_p9->AppendIfMy(pcszElementName, ptrAr))
		{
		}
		else
		{
			return FALSE;
		}
		m_bEmpty = FALSE;
		return TRUE;
	}

	CXTPSyntaxEditLexVariantPtrArray* GetIfMy(LPCTSTR pcszElementName)
	{
		CXTPSyntaxEditLexVariantPtrArray* pData = NULL;

		if (m_p0)
		{
			pData = m_p0->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p1)
		{
			pData = m_p1->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p2)
		{
			pData = m_p2->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p3)
		{
			pData = m_p3->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p4)
		{
			pData = m_p4->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p5)
		{
			pData = m_p5->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p6)
		{
			pData = m_p6->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p7)
		{
			pData = m_p7->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p8)
		{
			pData = m_p8->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}
		if (m_p9)
		{
			pData = m_p9->GetIfMy(pcszElementName);
			if (pData)
				return pData;
		}

		return NULL;
	}

	void CopyFrom(const Tthis& rSrc)
	{
		RemoveAll();

		// m_p0 ? m_p0->CopyFrom(*rSrc.m_p0):1;

		// just call without accessing to the data (and no virtual)
		// NULL->CopyFrom(NULL) - is OK
		m_p0->CopyFrom(*rSrc.m_p0);
		m_p1->CopyFrom(*rSrc.m_p1);
		m_p2->CopyFrom(*rSrc.m_p2);
		m_p3->CopyFrom(*rSrc.m_p3);
		m_p4->CopyFrom(*rSrc.m_p4);
		m_p5->CopyFrom(*rSrc.m_p5);
		m_p6->CopyFrom(*rSrc.m_p6);
		m_p7->CopyFrom(*rSrc.m_p7);
		m_p8->CopyFrom(*rSrc.m_p8);
		m_p9->CopyFrom(*rSrc.m_p9);

		m_bEmpty = m_p0->IsEmpty() && m_p1->IsEmpty() && m_p2->IsEmpty() && m_p3->IsEmpty()
				   && m_p4->IsEmpty() && m_p5->IsEmpty() && m_p6->IsEmpty() && m_p7->IsEmpty()
				   && m_p8->IsEmpty() && m_p9->IsEmpty();
	}

	void RemoveAll()
	{
		m_p0->RemoveAll();
		m_p1->RemoveAll();
		m_p2->RemoveAll();
		m_p3->RemoveAll();
		m_p4->RemoveAll();
		m_p5->RemoveAll();
		m_p6->RemoveAll();
		m_p7->RemoveAll();
		m_p8->RemoveAll();
		m_p9->RemoveAll();
		m_bEmpty = TRUE;
	}

#	ifdef _DEBUG
	void DumpOffset(CDumpContext& dc, LPCTSTR pcszOffset)
	{
		m_p0->DumpOffset(dc, pcszOffset);
		m_p1->DumpOffset(dc, pcszOffset);
		m_p2->DumpOffset(dc, pcszOffset);
		m_p3->DumpOffset(dc, pcszOffset);
		m_p4->DumpOffset(dc, pcszOffset);
		m_p5->DumpOffset(dc, pcszOffset);
		m_p6->DumpOffset(dc, pcszOffset);
		m_p7->DumpOffset(dc, pcszOffset);
		m_p8->DumpOffset(dc, pcszOffset);
		m_p9->DumpOffset(dc, pcszOffset);
	}
#	endif
};

//{{AFX_CODEJOCK_PRIVATE
_XTP_EXT_CLASS void AFX_CDECL XTPDumpSyntaxEditLexVariant(CXTPSyntaxEditLexVariant* pVt,
														  CDumpContext& dc);
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
template<LPCTSTR (*_FuncName)(void)>
class CXTPSyntaxEditLexVariantPtrArrayExT : public CXTPSyntaxEditLexVariantPtrArray
{
public:
	CXTPSyntaxEditLexVariantPtrArrayExT(){};
	virtual ~CXTPSyntaxEditLexVariantPtrArrayExT(){};

	LPCTSTR GetName() const
	{
		return _FuncName();
	}

	BOOL IsEmpty() const
	{
		return GetSize() == 0;
	}

	BOOL IfMy(LPCTSTR pcszElementName)
	{
		int nCmpRes = _tcsicmp(GetName(), pcszElementName);
		return (nCmpRes == 0);
	}

	BOOL AddIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtr& ptr)
	{
		if (IfMy(pcszElementName))
		{
			Add(ptr);
			return TRUE;
		}
		return FALSE;
	}

	BOOL AppendIfMy(LPCTSTR pcszElementName, const CXTPSyntaxEditLexVariantPtrArray& ptrAr)
	{
		if (IfMy(pcszElementName))
		{
			Append(ptrAr);
			return TRUE;
		}
		return FALSE;
	}

	CXTPSyntaxEditLexVariantPtrArray* GetIfMy(LPCTSTR pcszElementName)
	{
		if (IfMy(pcszElementName))
		{
			return this;
		}
		return NULL;
	}

	void CopyFrom(const CXTPSyntaxEditLexVariantPtrArray& rSrc)
	{
		RemoveAll();
		Append(rSrc);
	}
#	ifdef _DEBUG
	void DumpOffset(CDumpContext& dc, LPCTSTR pcszOffset)
	{
		if (IsEmpty())
		{
			return;
		}
#		if _MSC_VER > 1100
		int nCount = (int)GetSize();

		dc << pcszOffset;
		dc << GetName() << _T("[") << nCount << _T("]");

		for (int i = 0; i < nCount; i++)
		{
			if (i)
			{
				dc << _T(", ");
			}

			CXTPSyntaxEditLexVariantPtr ptrV = GetAt(i);
			if (ptrV)
			{
				XTPDumpSyntaxEditLexVariant(ptrV, dc);
			}
			else
			{
				dc << _T("?<NULL>");
			}
		}
		dc << _T(" \n");
#		else
		UNREFERENCED_PARAMETER(dc);
		UNREFERENCED_PARAMETER(pcszOffset);
#		endif
	}
#	endif
};

class CXTPSyntaxEditLexAutomatWordsMap;
class _XTP_EXT_CLASS LA_CHAR_NODE
{
	friend class CXTPSyntaxEditLexAutomatWordsMap;
	friend class CXTPSyntaxEditLexAutomatMemMan;

public:
	//----------------------------------------------------------------
	enum NodeFlags
	{
		nfSubWordEnd = 0x0001,
	};
	//----------------------------------------------------------------
	WORD wChar;
	WORD wFlags;
	CXTPSyntaxEditLexAutomatWordsMap* pNextMap;

protected:
	LA_CHAR_NODE* pNextNode;

	void Clear();

public:
	LA_CHAR_NODE();
	virtual ~LA_CHAR_NODE();

	const LA_CHAR_NODE& operator=(const LA_CHAR_NODE& rSrc);

	// void Clear();
};
typedef LA_CHAR_NODE* PLA_CHAR_NODE;
//===========================================================================
//}}AFX_CODEJOCK_PRIVATE

//============================================================================
// Summary:
//     This class abstracts a word map as a kind of CXTPCmdTarget.
//============================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexAutomatWordsMap : public CXTPCmdTarget
{
	friend class CXTPSyntaxEditLexAutomatMemMan;

public:
	//------------------------------------------------------------------------
	// Summary:
	//     Constructor, constructs and initializes the object.
	//------------------------------------------------------------------------
	CXTPSyntaxEditLexAutomatWordsMap(UINT uHashTableSize = 17);

	//------------------------------------------------------------------------
	// Summary:
	//     Destroys the object and does the cleanup.
	//------------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexAutomatWordsMap();

	//------------------------------------------------------------------------
	// Summary:
	//     Removes all map entries.
	//------------------------------------------------------------------------
	void RemoveAll();

	//------------------------------------------------------------------------
	// Summary:
	//     Adds a word to the map.
	// Parameters:
	//     strWord        - Word to be added.
	//     uHashTableSize - Hash table size.
	//------------------------------------------------------------------------
	void AddWord(CString strWord, UINT uHashTableSize);

	//------------------------------------------------------------------------
	// Summary:
	//     Finds a word in the map.
	// Parameters:
	//     bMinWord            - Minimum word.
	//     pcszBuffer          - String buffer.
	//     rstrWord            - Reference to the word to find.
	//     nBufSize            - Buffer size.
	//     bConvertToLowerCase - TRUE to convert to lower case, FALSE otherwise.
	//     bIgnoreCaseDyn      - TRUE to ingnore case, FALSE otherwise.
	//------------------------------------------------------------------------
	BOOL FindWord(BOOL bMinWord, LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize,
				  BOOL bConvertToLowerCase, BOOL bIgnoreCaseDyn) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Initializes the word map.
	// Parameters:
	//     uHashTableSize - Hash table size.
	//------------------------------------------------------------------------
	void InitMap(UINT uHashTableSize);

	//------------------------------------------------------------------------
	// Summary:
	//     Gets the hash table size.
	// Returns:
	//     The hash table size.
	//------------------------------------------------------------------------
	UINT GetHashTableSize();

	//------------------------------------------------------------------------
	// Summary:
	//     Divides the hash table size.
	// Parameters:
	//     uHashTableSize - Current hash table size.
	//     uMin           - Minimum hash table size.
	// Returns:
	//     The divided hash table size.
	//------------------------------------------------------------------------
	static UINT AFX_CDECL DivideHTSize(UINT uHashTableSize, UINT uMin = 1);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	static UINT AFX_CDECL PrimeAdjustU_50(UINT uNumber);

	BOOL _FindWord(LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize, WORD wEndFlags,
				   BOOL bConvertToLowerCase, BOOL bTryToChangeCaseDyn,
				   BOOL bChangeCase = FALSE) const;
	//}}AFX_CODEJOCK_PRIVATE

	//------------------------------------------------------------------------
	// Summary:
	//     Retrieves the first character of a multi-byte string.
	// Parameters:
	//     pcszStr   - String to retrieve the first character of.
	//     rnCharLen - Reference to the length of the multi-byte character.
	// Returns:
	//     The first character of the multi-byte string.
	//------------------------------------------------------------------------
	WORD _GetChar(LPCTSTR pcszStr, int& rnCharLen) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Changes the case of a specified character.
	// Parameters:
	//     wChar - Character with the case to be changed.
	// Returns:
	//     The character after its case has been changed.
	//------------------------------------------------------------------------
	WORD _ChangeCase(WORD wChar) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Changes the case of a specified character to lower case.
	// Parameters:
	//     wChar - Character with the case to be changed.
	// Returns:
	//     The character after its case has been changed to lower case.
	//------------------------------------------------------------------------
	WORD _MakeLower(WORD wChar) const;

	//------------------------------------------------------------------------
	// Summary:
	//     Clears the word map.
	//------------------------------------------------------------------------
	void Clear()
	{
		RemoveAll();
	};

	//------------------------------------------------------------------------
	// Summary:
	//     Performs final cleanup.
	//------------------------------------------------------------------------
	virtual void OnFinalRelease();

	//------------------------------------------------------------------------
	// Summary:
	//     Performs lookup for a character node.
	// Parameters:
	//     wChar - Character.
	// Returns:
	//     A pointer to a LA_CHAR_NODE object.
	//------------------------------------------------------------------------
	const LA_CHAR_NODE* Lookup(WORD wChar) const;

	const LA_CHAR_NODE* SetAt(const LA_CHAR_NODE& newNode);

	CXTPSyntaxEditLexAutomatWordsMap* pNextFreeObj; // Pointer to the next free object.

	PLA_CHAR_NODE* m_pHashTable; // Hash table.
	UINT m_uHashTableSize;		 // Hash table size.

private:
};
XTP_DEFINE_SMART_PTR_INTERNAL(CXTPSyntaxEditLexAutomatWordsMap)

//============================================================================
// Summary:
//     This class helps to automate the lexical tags.
//============================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditLexTagsAutomat
{
public:
	//------------------------------------------------------------------------
	// Summary:
	//     Constructor, constructs and initializes the object.
	//------------------------------------------------------------------------
	CXTPSyntaxEditLexTagsAutomat();

	//------------------------------------------------------------------------
	// Summary:
	//      Destroys the object and does the cleanup.
	//------------------------------------------------------------------------
	virtual ~CXTPSyntaxEditLexTagsAutomat();

	//------------------------------------------------------------------------
	// Summary:
	//     Adds tags to the word map.
	// Parameters:
	//     pArTags             - Pointer to the tags to be added.
	//     bConvertToLowerCase - TRUE to convert to lower case, FALSE otherwise.
	//------------------------------------------------------------------------
	void AddTagsList(CXTPSyntaxEditLexVariantPtrArray* pArTags, BOOL bConvertToLowerCase);

	//------------------------------------------------------------------------
	// Summary:
	//     Determines if the word map is empty.
	// Returns:
	//     TRUE if the word map is empty, otherwise FALSE.
	//------------------------------------------------------------------------
	BOOL IsEmpty();

	//------------------------------------------------------------------------
	// Summary:
	//     Removes all map entries.
	//------------------------------------------------------------------------
	void RemoveAll();

	//------------------------------------------------------------------------
	// Summary:
	//     Copies word map values from an existing word map.
	// Parameter:
	//     rSrc - Reference to an existing word map.
	//------------------------------------------------------------------------
	void CopyFrom(const CXTPSyntaxEditLexTagsAutomat& rSrc);

	//------------------------------------------------------------------------
	// Summary:
	//     Finds the minimum word.
	// Parameter:
	//     pcszBuffer          - String buffer.
	//     rstrWord            - Reference to the word to find.
	//     nBufSize            - Buffer size.
	//     bConvertToLowerCase - TRUE to convert to lower case, FALSE otherwise.
	//     bIgnoreCaseDyn      - TRUE to ingnore case, FALSE otherwise.
	//------------------------------------------------------------------------
	BOOL FindMinWord(LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize, BOOL bConvertToLowerCase,
					 BOOL bIgnoreCaseDyn);

	//------------------------------------------------------------------------
	// Summary:
	//     Finds the maximum word.
	// Parameter:
	//     pcszBuffer          - String buffer.
	//     rstrWord            - Reference to the word to find.
	//     nBufSize            - Buffer size.
	//     bConvertToLowerCase - TRUE to convert to lower case, FALSE otherwise.
	//     bIgnoreCaseDyn      - TRUE to ingnore case, FALSE otherwise.
	//------------------------------------------------------------------------
	BOOL FindMaxWord(LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize, BOOL bConvertToLowerCase,
					 BOOL bIgnoreCaseDyn);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void _AddWords(CXTPSyntaxEditLexAutomatWordsMapPtr& rPtrMap, CStringArray& arTags,
				   BOOL bConvertToLowerCase);
	UINT _CalcHashTableSize(int nCount);

	BOOL _FindWord(BOOL bMinWord, LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize,
				   BOOL bConvertToLowerCase, BOOL bIgnoreCaseDyn);

public:
	CXTPSyntaxEditLexAutomatWordsMapPtr m_ptrWordsMap;
	CXTPSyntaxEditLexAutomatWordsMapPtr m_ptrWordsMap_not;
	//}}AFX_CODEJOCK_PRIVATE

private:
};
//{{AFX_CODEJOCK_PRIVATE
class _XTP_EXT_CLASS CXTPSyntaxEditLexAutomatMemMan : public CXTPCmdTarget
{
public:
	CXTPSyntaxEditLexAutomatMemMan();
	virtual ~CXTPSyntaxEditLexAutomatMemMan();

	CXTPSyntaxEditLexAutomatWordsMap* NewMap(UINT uHashTableSize);
	LA_CHAR_NODE* NewNode();
	PLA_CHAR_NODE* NewHashTable(UINT uSize);

	void FreeObject(CXTPSyntaxEditLexAutomatWordsMap* pObj);
	void FreeObject(LA_CHAR_NODE* pObj);
	void FreeObject(PLA_CHAR_NODE* pObj);

	DWORD Lock();
	DWORD Unlock();

#	ifdef _DEBUG
	void Dump(CDumpContext& dc) const;
#	endif
	UINT m_uAllocatedMaps;
	UINT m_uUsedMaps;

	UINT m_uAllocatedNodes;
	UINT m_uUsedNodes;

	UINT m_uAllocatedTables;
	UINT m_uUsedTables;
	UINT m_uAllocatedTablesBytes;
	UINT m_uUsedTablesBytes;
	// END DEBUG -----------------------------------

protected:
	LONG m_lLockCount;

	void FreeAll();

	struct LA_HASH_TABLE
	{
		friend class CXTPSyntaxEditLexAutomatMemMan;

		UINT uSize;
		// protected:
		LA_HASH_TABLE* pNextFreeObj;

		void Clear()
		{
			PLA_CHAR_NODE* pData = GetData();
			::ZeroMemory(pData, uSize * sizeof(PLA_CHAR_NODE*));
		}

		PLA_CHAR_NODE* GetData()
		{
			return (PLA_CHAR_NODE*)(((byte*)this) + GetHeaderSizeB());
		}

		static UINT AFX_CDECL GetHeaderSizeB()
		{
			UINT uHeader = sizeof(LA_HASH_TABLE);
			return uHeader;
		}

		static UINT AFX_CDECL GetHeaderSize()
		{
			UINT uHeader  = GetHeaderSizeB();
			UINT uHeader2 = uHeader / sizeof(PLA_CHAR_NODE);
			uHeader2 += (uHeader % sizeof(PLA_CHAR_NODE)) ? 1 : 0;
			return uHeader2;
		}

		static LA_HASH_TABLE* AFX_CDECL GetHeader(PLA_CHAR_NODE* pData)
		{
			return (LA_HASH_TABLE*)(((byte*)pData) - GetHeaderSizeB());
		}
	};

	CXTPSyntaxEditLexAutomatWordsMap* m_pFreeMaps;
	LA_CHAR_NODE* m_pFreeNodes;

	LA_HASH_TABLE* m_pFreeTables;

	CList<CXTPSyntaxEditLexAutomatWordsMap*, CXTPSyntaxEditLexAutomatWordsMap*> m_allocatedMaps;
	CList<LA_CHAR_NODE*, LA_CHAR_NODE*> m_allocatedNodes;
	CList<PLA_CHAR_NODE*, PLA_CHAR_NODE*> m_allocatedTables;
};

////////////////////////////////////////////////////////////////////////////
template<LPCTSTR (*_FuncName)(void)>
class CXTPSyntaxEditLexVariantPtrArrayExAutomatT
	: public CXTPSyntaxEditLexVariantPtrArrayExT<_FuncName>
{
public:
	typedef CXTPSyntaxEditLexVariantPtrArrayExT<_FuncName> TBase;
	typedef CXTPSyntaxEditLexVariantPtrArrayExAutomatT<_FuncName> Tthis;

	CXTPSyntaxEditLexVariantPtrArrayExAutomatT()
	{
#	ifdef _DEBUG
		m_bAutomatWasBuilt = FALSE;
#	endif
	};
	virtual ~CXTPSyntaxEditLexVariantPtrArrayExAutomatT(){};

	void BuildAutomat(BOOL bConvertToLowerCase = FALSE)
	{
		m_automat.RemoveAll();
		m_automat.AddTagsList(this, bConvertToLowerCase);
#	ifdef _DEBUG
		m_bAutomatWasBuilt = TRUE;
#	endif
	}

	void CopyFrom(const Tthis& rSrc)
	{
		TBase::CopyFrom(rSrc);
		m_automat.CopyFrom(rSrc.m_automat);

#	ifdef _DEBUG
		m_bAutomatWasBuilt = rSrc.m_bAutomatWasBuilt;
#	endif
	}

	BOOL FindMinWord(LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize,
					 BOOL bConvertToLowerCase = FALSE, BOOL bIgnoreCaseDyn = FALSE)
	{
		ASSERT(m_bAutomatWasBuilt);
		return m_automat.FindMinWord(pcszBuffer, rstrWord, nBufSize, bConvertToLowerCase,
									 bIgnoreCaseDyn);
	}

	BOOL FindMaxWord(LPCTSTR pcszBuffer, CString& rstrWord, int nBufSize,
					 BOOL bConvertToLowerCase = FALSE, BOOL bIgnoreCaseDyn = FALSE)
	{
		ASSERT(m_bAutomatWasBuilt);
		return m_automat.FindMaxWord(pcszBuffer, rstrWord, nBufSize, bConvertToLowerCase,
									 bIgnoreCaseDyn);
	}

protected:
	CXTPSyntaxEditLexTagsAutomat m_automat;
#	ifdef _DEBUG
	BOOL m_bAutomatWasBuilt;
#	endif
};
} // namespace XTPSyntaxEditLexAnalyser
//}}AFX_CODEJOCK_PRIVATE

////////////////////////////////////////////////////////////////////////////
//#pragma warning(pop)

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITLEXCLASSSUBOBJT_H__)
