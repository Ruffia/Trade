// XTPSystemPreview.cpp : implementation of the CXTPSystemPreview class.
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

#include "stdafx.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/System/XTPSystemPreview.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <comdef.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"
#pragma warning(push)
// C4616: #pragma warning : warning number '...' out of range, must be between '4001' and '4999'
// C4619: #pragma warning : there is no warning number 'number'
// C5204: '...': class has virtual functions, but its trivial destructor is not virtual; instances
// of objects derived from this class may not be destructed correctly
#pragma warning(disable : 4616 4619 5204)

struct CXTPSystemPreview::IPreviewHandler : public IUnknown
{
public:
	static const GUID IID;

	virtual HRESULT STDMETHODCALLTYPE SetWindow(
		/* [in] */ HWND hwnd,
		/* [in] */ const RECT* prc) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetRect(
		/* [in] */ const RECT* prc) = 0;

	virtual HRESULT STDMETHODCALLTYPE DoPreview(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE Unload(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetFocus(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE QueryFocus(
		/* [out] */ HWND* phwnd) = 0;

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(
		/* [in] */ MSG* pmsg) = 0;
};

// {8895b1c6-b41f-4c1c-a562-0d564250836f}
const GUID CXTPSystemPreview::IPreviewHandler::IID =
	{ 0x8895b1c6, 0xb41f, 0x4c1c, { 0xa5, 0x62, 0x0d, 0x56, 0x42, 0x50, 0x83, 0x6f } };

struct CXTPSystemPreview::IInitializeWithStream : public IUnknown
{
public:
	static const GUID IID;

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Initialize(
		/* [annotation][in] */
		IStream* pstream,
		/* [annotation][in] */
		DWORD grfMode) = 0;
};

// {b824b49d-22ac-4161-ac8a-9916e8fa3f7f}
const GUID CXTPSystemPreview::IInitializeWithStream::IID =
	{ 0xb824b49d, 0x22ac, 0x4161, { 0xac, 0x8a, 0x99, 0x16, 0xe8, 0xfa, 0x3f, 0x7f } };

struct CXTPSystemPreview::IInitializeWithFile : public IUnknown
{
public:
	static const GUID IID;

	virtual HRESULT STDMETHODCALLTYPE Initialize(
		/* [string][in] */ LPCWSTR pszFilePath,
		/* [in] */ DWORD grfMode) = 0;
};

// {b7d14566-0509-4cce-a71f-0a554233bd9b}
const GUID CXTPSystemPreview::IInitializeWithFile::IID =
	{ 0xb7d14566, 0x0509, 0x4cce, { 0xa7, 0x1f, 0x0a, 0x55, 0x42, 0x33, 0xbd, 0x9b } };

CXTPSystemPreview::CXTPSystemPreview()
	: m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_pPreview(NULL)
	, m_pDataStream(NULL)
	, m_hParentWindow(NULL)
{
}

CXTPSystemPreview::~CXTPSystemPreview()
{
	if (NULL != m_pPreview)
	{
		m_pPreview->Unload();
		m_pPreview->Release();
	}

	if (NULL != m_pDataStream)
		m_pDataStream->Release();
}

BOOL CXTPSystemPreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPSystemPreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPSystemPreview::HasOwnNavigator() const
{
	return FALSE;
}

UINT CXTPSystemPreview::GetNumberOfPages() const
{
	return 1;
}

SIZE CXTPSystemPreview::GetPreferredPageSize() const
{
	CSize sz(100, 100);
	return sz;
}

void CXTPSystemPreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPSystemPreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	UNREFERENCED_PARAMETER(dc);
	UNREFERENCED_PARAMETER(clientSize);
	UNREFERENCED_PARAMETER(updateRect);
}

BOOL CXTPSystemPreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	UNREFERENCED_PARAMETER(result);

	if ((msg->message == WM_WINDOWPOSCHANGING) || (msg->message == WM_WINDOWPOSCHANGED))
	{
		WINDOWPOS* pWinPos = reinterpret_cast<WINDOWPOS*>(msg->lParam);
		CRect rec(0, 0, pWinPos->cx, pWinPos->cy);
		m_pPreview->SetRect(&rec);
	}

	return FALSE;
}

BOOL CXTPSystemPreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPSystemPreview::Activate()
{
	if (NULL != m_pPreview)
		m_pPreview->SetFocus();
}

BOOL CXTPSystemPreview::LoadFile(LPCTSTR pFilePath, HWND hwndParent)
{
	return LoadFile(NULL, pFilePath, hwndParent);
}

BOOL CXTPSystemPreview::LoadFile(IStream* pStream, LPCTSTR pImpliedFileName, HWND hwndParent)
{
	ASSERT(NULL == m_pPreview);
	ASSERT(NULL == m_pDataStream);

	m_hParentWindow = hwndParent;
	CString sExt(::PathFindExtension(pImpliedFileName));
	if (sExt.Find(_T("."), 0) != 0)
	{
		CString s = _T(".") + sExt;
		sExt	  = s;
	}

	CString sKey = sExt + _T("\\ShellEx\\{8895b1c6-b41f-4c1c-a562-0d564250836f}");
	CArray<TCHAR, TCHAR> buff;
	buff.SetSize(MAX_PATH);
	LONG cData = (MAX_PATH - 1) * sizeof(TCHAR);
	RegQueryValue(HKEY_CLASSES_ROOT, sKey, &buff[0], &cData);
	_bstr_t sData(&buff[0]);

	CLSID clsId;
	HRESULT hr = CLSIDFromString(sData, &clsId);
	if (FAILED(hr))
		return FALSE;

	ASSERT(NULL == m_pPreview);
	hr = CoCreateInstance(clsId, NULL, CLSCTX_LOCAL_SERVER, IPreviewHandler::IID,
						  reinterpret_cast<LPVOID*>(&m_pPreview));
	if (FAILED(hr))
		return FALSE;

	IInitializeWithStream* streamInit = NULL;
	hr								  = m_pPreview->QueryInterface(IInitializeWithStream::IID,
									   reinterpret_cast<LPVOID*>(&streamInit));
	if (SUCCEEDED(hr))
	{
		if (NULL != pStream)
		{
			m_pDataStream = pStream;
			m_pDataStream->AddRef();
		}
		else if (FAILED(XTPCreateReadOnlyFileStream(pImpliedFileName, &m_pDataStream)))
		{
			streamInit->Release();
			return FALSE;
		}

		hr = streamInit->Initialize(m_pDataStream, STGM_READ);

		streamInit->Release();

		if (FAILED(hr))
			return FALSE;
	}
	else
	{
		IInitializeWithFile* fileInit = NULL;
		hr = m_pPreview->QueryInterface(IInitializeWithFile::IID, (LPVOID*)&fileInit);
		if (SUCCEEDED(hr))
		{
			hr = fileInit->Initialize(_bstr_t(pImpliedFileName), STGM_READ);

			fileInit->Release();

			if (FAILED(hr))
				return FALSE;
		}
	}

	CXTPClientRect rcParent(hwndParent);
	hr = m_pPreview->SetWindow(hwndParent, &rcParent);
	if (FAILED(hr))
		return FALSE;

	hr = m_pPreview->DoPreview();
	if (FAILED(hr))
		return FALSE;

	m_bLoadFinished = TRUE;
	m_bLoadSuccess  = TRUE;

	return TRUE;
}

#pragma warning(pop)
#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
