// XTPSkinManagerResource.h: interface for the CXTPSkinManagerResourceFile class.
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
#if !defined(__XTPSKINMANAGERRESOURCE_H__)
#	define __XTPSKINMANAGERRESOURCE_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPSkinImage;
class CXTPSkinManagerSchema;
class CXTPSkinManager;

//===========================================================================
// Summary:
//     CXTPSkinManagerResourceFile is a standalone class that represents a
//     manager of visual style resources
//===========================================================================
class _XTP_EXT_CLASS CXTPSkinManagerResourceFile
	: public CXTPCmdTarget
	, public CXTPSynchronized
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSkinManagerResourceFile object.
	// Parameters:
	//     pSkinManager - Pointer to the parent SkinFramework manager.
	//-----------------------------------------------------------------------
	CXTPSkinManagerResourceFile(CXTPSkinManager* pSkinManager = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSkinManagerResourceFile object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPSkinManagerResourceFile();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Opens the visual style file.
	// Parameters:
	//     lpszResourcePath - Path to the specified visual style.
	//     lpszIniFileName  - Name of the .ini file to load.
	//-----------------------------------------------------------------------
	virtual BOOL Open(LPCTSTR lpszResourcePath, LPCTSTR lpszIniFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads the .ini file.
	// Parameters:
	//     str - Reference to the string used to load the next line
	//           from the resource file.
	//-----------------------------------------------------------------------
	virtual BOOL ReadString(CString& str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Closes the visual style file.
	//-----------------------------------------------------------------------
	virtual void Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads a specified image from a resource file.
	// Parameters:
	//     strImageFile - Path to the specified image.
	// Returns:
	//     A pointer to the newly loaded CXTPSkinImage object containing
	//     the loaded bitmap.
	//-----------------------------------------------------------------------
	virtual CXTPSkinImage* LoadImage(CString strImageFile);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the CXTPSkinManagerSchema class associated with loaded resources.
	// Returns:
	//     A pointer to the newly created CXTPSkinManagerSchema class that
	//     will be used to read resources.
	// See Also:
	//     CXTPSkinManagerSchema
	//-----------------------------------------------------------------------
	virtual CXTPSkinManagerSchema* CreateSchema();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the resource path.
	// Returns:
	//     The resource path.
	//-----------------------------------------------------------------------
	CString GetResourcePath() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the .ini file name.
	// Returns:
	//     The .ini file name.
	//-----------------------------------------------------------------------
	CString GetIniFileName() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the handle of the skin module.
	// Returns:
	//     The handle of the skin module.
	//-----------------------------------------------------------------------
	HMODULE GetModuleHandle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the handle of the skin module.
	// Parameters:
	//     hModule - Handle to be set.
	//-----------------------------------------------------------------------
	void SetModuleHandle(HMODULE hModule);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the owner skin manager object.
	// Returns:
	//     A pointer to the owner skin manager object.
	//-----------------------------------------------------------------------
	CXTPSkinManager* GetSkinManager() const;

private:
	CString GetDefaultIniFileName();

protected:
	LPWSTR m_lpTextFile;	// Pointer to the next string.
	LPWSTR m_lpTextFileEnd; // Pointer to end of the file.
	HMODULE m_hModule;		// Handle of the visual style.
	HMODULE m_hInstance;	// Handle of the visual style.

	CString m_strResourcePath;   // Resource path.
	CString m_strIniFileName;	// .ini file name.
	CXTPSkinManager* m_pManager; // Parent manager class.

	CStdioFile* m_pIniFile; // Pointer to the .ini file on disk.

	friend class CXTPSkinManager;
};

AFX_INLINE CString CXTPSkinManagerResourceFile::GetIniFileName() const
{
	return m_strIniFileName;
}
AFX_INLINE CString CXTPSkinManagerResourceFile::GetResourcePath() const
{
	return m_strResourcePath;
}
AFX_INLINE HMODULE CXTPSkinManagerResourceFile::GetModuleHandle() const
{
	return m_hModule;
}
AFX_INLINE void CXTPSkinManagerResourceFile::SetModuleHandle(HMODULE hModule)
{
	m_hInstance = hModule;
}
AFX_INLINE CXTPSkinManager* CXTPSkinManagerResourceFile::GetSkinManager() const
{
	return m_pManager;
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif // !defined(__XTPSKINMANAGERRESOURCE_H__)
