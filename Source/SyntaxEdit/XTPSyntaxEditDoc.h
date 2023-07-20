// XTPSyntaxEditDoc.h : header file
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
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSYNTAXEDITSYNTAXEDITDOC_H__)
#	define __XTPSYNTAXEDITSYNTAXEDITDOC_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSyntaxEditBufferManager;

//===========================================================================
// Summary:
//     This class represents the Document portion of an Edit control. It
//     extends the functionality provided by the CDocument class from MFC's
//     document-view model implementation. The CXTPSyntaxEditDoc class works
//     together with CXTPSyntaxEditView and provides storage facilities for it.
// See Also:
//     CXTPSyntaxEditView
//===========================================================================
class _XTP_EXT_CLASS CXTPSyntaxEditDoc : public CDocument
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected object constructor. Used by dynamic creation
	//-----------------------------------------------------------------------
	CXTPSyntaxEditDoc();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSyntaxEditDoc object, handles cleanup and
	//     de-allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSyntaxEditDoc();

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNCREATE(CXTPSyntaxEditDoc)
	//}}AFX_CODEJOCK_PRIVATE

	// Attributes
public:
	// Operations
public:
	// Overrides
	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPSyntaxEditDoc)
public:
	virtual void Serialize(CArchive& ar); // overridden for document i/o
	virtual void OnChangedViewList();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();

protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL
	//}}AFX_CODEJOCK_PRIVATE

	// Implementation
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Re-opens an opened file.
	// Returns:
	//     TRUE if the file was re-opened successfully, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL ReOpen();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the edit buffer manager.
	// Parameters:
	//     pBuffer - [in] Pointer to the buffer manager to be set.
	// See also:
	//     CXTPSyntaxEditBufferManager
	//-----------------------------------------------------------------------
	void SetDataManager(CXTPSyntaxEditBufferManager* pBuffer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the edit buffer manager.
	// Returns:
	//     A pointer to the edit buffer manager.
	// See also:
	//     CXTPSyntaxEditBufferManager
	//-----------------------------------------------------------------------
	CXTPSyntaxEditBufferManager* GetDataManager() const;

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
	//     CXTPSyntaxEditCtrl::GetConfigFile
	//-----------------------------------------------------------------------
	void SetConfigFile(LPCTSTR szPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if auto-reload for external files is enabled/disabled.
	// Returns:
	//     TRUE if auto-reload for external files is enabled,
	//     FALSE if auto-reload for external files is disabled.
	//-----------------------------------------------------------------------
	BOOL GetAutoReload() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables auto-reload for external files.
	// Parameters:
	//     bAutoReload - [in] TRUE to enable auto-reload for external files,
	//                        FALSE to disable auto-reload for external files.
	//     bUpdateReg  - [in] TRUE to update this setting in the registry, FALSE otherwise.
	//                        By default, this parameter is FALSE.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetAutoReload(BOOL bAutoReload, BOOL bUpdateReg = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current file has been externally modified and
	//     prompts the user to reload it.
	// Parameters:
	//     lpszPathName - Full file name.
	// Remarks:
	//     This method only works when auto-reload is enabled.
	// Returns:
	//     TRUE if the file was externally modified and reloaded,
	//     otherwise FALSE.
	// See Also:
	//     GetAutoReload, SetAutoReload
	//-----------------------------------------------------------------------
	BOOL CheckFileModified(LPCTSTR lpszPathName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the current file is read-only.
	// Returns:
	//     TRUE if the current file is read-only, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsReadonly();

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets a first view for the document object.
	// Returns:
	//     A pointer to a CXTPSyntaxEditView object.
	// See Also:
	//     CDocument::GetFirstViewPosition, CDocument::GetNextView
	//-----------------------------------------------------------------------
	CXTPSyntaxEditView* GetFirstView();

#	ifdef _DEBUG
	//{{AFX_CODEJOCK_PRIVATE
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	//}}AFX_CODEJOCK_PRIVATE
#	endif

	// Generated message map functions
protected:
	virtual BOOL SaveFileData(LPCTSTR lpszPathName);

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTPSyntaxEditDoc)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPSyntaxEditBufferManager* m_ptrDataMan; // Data manager pointer.

	BOOL m_bAutoReload; // TRUE if auto-reload for externally modified files is enabled, FALSE
						// otherwise.
	BOOL m_bOpened;		// TRUE if opened, FALSE otherwise.
	BOOL m_bNewFile; // TRUE if either a new file is created or a file is opened, FALSE otherwise.
	BOOL m_bDocCreated; // TRUE if a document is created, FALSE otherwise.
	BOOL m_bReloading;  // TRUE if the document is being reloaded, FALSE otherwise.

	int m_iTopRow;	 // Identifier of top row.
	int m_iHScrollPos; // Scrolling position.

	WIN32_FIND_DATA m_curFileData;
};

AFX_INLINE CXTPSyntaxEditBufferManager* CXTPSyntaxEditDoc::GetDataManager() const
{
	return m_ptrDataMan;
}
AFX_INLINE BOOL CXTPSyntaxEditDoc::GetAutoReload() const
{
	return m_bAutoReload;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSYNTAXEDITSYNTAXEDITDOC_H__)
