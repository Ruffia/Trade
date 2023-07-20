// XTPTextPreview.cpp : implementation of the CXTPTextPreview class.
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

#include "Common/XTPCasting.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/Providers/Text/XTPTextPreview.h"
#include "Controls/Preview/Providers/Text/XTPTextPreviewProvider.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

CXTPTextPreview::CXTPTextPreview()
	: m_bLoadFinished(FALSE)
	, m_bLoadSuccess(FALSE)
	, m_bCreated(FALSE)
	, m_pStream(NULL)
	, m_ulSize(ULARGE_INTEGER())
{
}

CXTPTextPreview::~CXTPTextPreview()
{
	if (m_bCreated)
		m_RichEdit.DestroyWindow();

	if (INVALID_HANDLE_VALUE != reinterpret_cast<HANDLE>(m_inFile.m_hFile))
		m_inFile.Close();

	if (NULL != m_pStream)
		m_pStream->Release();
}

BOOL CXTPTextPreview::IsReady() const
{
	return m_bLoadFinished;
}

BOOL CXTPTextPreview::IsAvailable() const
{
	return m_bLoadSuccess;
}

BOOL CXTPTextPreview::HasOwnNavigator() const
{
	return FALSE;
}

UINT CXTPTextPreview::GetNumberOfPages() const
{
	return UINT(m_bLoadSuccess ? 1 : 0);
}

SIZE CXTPTextPreview::GetPreferredPageSize() const
{
	CSize sz(100, 100);
	return sz;
}

void CXTPTextPreview::GoToPage(UINT page)
{
	UNREFERENCED_PARAMETER(page);
}

void CXTPTextPreview::Draw(HDC dc, SIZE clientSize, RECT updateRect)
{
	UNREFERENCED_PARAMETER(dc);
	UNREFERENCED_PARAMETER(clientSize);
	UNREFERENCED_PARAMETER(updateRect);
}

BOOL CXTPTextPreview::OnHostWndMsg(LPMSG msg, LRESULT* result)
{
	UNREFERENCED_PARAMETER(result);

	if ((msg->message == WM_WINDOWPOSCHANGING) || (msg->message == WM_WINDOWPOSCHANGED))
	{
		WINDOWPOS* pWinPos = (WINDOWPOS*)msg->lParam;
		m_RichEdit.SetWindowPos(NULL, 0, 0, pWinPos->cx, pWinPos->cy,
								SWP_NOACTIVATE | SWP_NOZORDER);
		m_RichEdit.UpdateWindow();
	}

	return FALSE;
}

BOOL CXTPTextPreview::SupportsCompositedParent() const
{
	return TRUE;
}

void CXTPTextPreview::Activate()
{
	if (::IsWindow(m_RichEdit))
		m_RichEdit.SetFocus();
}

BOOL CXTPTextPreview::CreateRichEditCtrl(HWND parent)
{
	m_bCreated	= FALSE;
	CWnd* pParent = CWnd::FromHandle(parent);
	if (NULL == pParent)
		return FALSE;

	CRect rec;
	pParent->GetClientRect(&rec);

	m_bCreated = m_RichEdit.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL
									   | ES_AUTOHSCROLL | ES_MULTILINE | ES_READONLY,
								   rec, pParent, 0);
	if (m_bCreated)
	{
		LONG exStyle = GetWindowLong(m_RichEdit, GWL_EXSTYLE);
		SetWindowLong(m_RichEdit, GWL_EXSTYLE, exStyle | WS_EX_CLIENTEDGE);
	}

	return m_bCreated;
}

BOOL CXTPTextPreview::LoadFile(LPCTSTR pFilePath)
{
	m_bLoadFinished = FALSE;
	m_bLoadSuccess  = FALSE;

	if (NULL != m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

	if (INVALID_HANDLE_VALUE != reinterpret_cast<HANDLE>(m_inFile.m_hFile))
		m_inFile.Close();

	if (!m_bCreated)
		return FALSE;

	m_RichEdit.SetWindowText(_T(""));

	if (!m_inFile.Open(pFilePath, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	m_ulSize.QuadPart = m_inFile.GetLength();

	EDITSTREAM es;
	es.dwError	 = 0;
	es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(StreamInFile); // Set the callback
	es.dwCookie	= reinterpret_cast<DWORD_PTR>(this);

	m_RichEdit.StreamIn(SF_TEXT, es);
	m_bLoadFinished = TRUE;
	m_RichEdit.SetTargetDevice(NULL, 0);
	m_bLoadSuccess = TRUE;

	return TRUE;
}

BOOL CXTPTextPreview::LoadFile(IStream& stream)
{
	m_bLoadFinished = FALSE;
	m_bLoadSuccess  = FALSE;

	if (NULL != m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

	if (INVALID_HANDLE_VALUE != reinterpret_cast<HANDLE>(m_inFile.m_hFile))
		m_inFile.Close();

	if (!m_bCreated)
		return FALSE;

	STATSTG stat;
	if (S_OK != stream.Stat(&stat, STATFLAG_NONAME))
	{
		TRACE(_T("Unable to obtain stream stats\n"));
		return FALSE;
	}

	m_RichEdit.SetWindowText(_T(""));

	m_pStream = &stream;
	m_pStream->AddRef();
	m_ulSize = stat.cbSize;

	EDITSTREAM es;
	es.dwError	 = 0;
	es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(StreamInFile); // Set the callback
	es.dwCookie	= reinterpret_cast<DWORD_PTR>(this);

	m_RichEdit.StreamIn(SF_TEXT, es);
	m_bLoadFinished = TRUE;
	m_RichEdit.SetTargetDevice(NULL, 0);
	m_bLoadSuccess = TRUE;

	return TRUE;
}

DWORD CALLBACK CXTPTextPreview::StreamInFile(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	ASSERT(NULL != pbBuff);
	ASSERT(NULL != pcb);

	CXTPTextPreview* pThis = reinterpret_cast<CXTPTextPreview*>(dwCookie);
	ASSERT(NULL != pThis);

	if (NULL != pThis->m_pStream)
	{
		ULONG size = static_cast<ULONG>(__min(pThis->m_ulSize.QuadPart, XTPToULongPtrChecked(cb)));
		if (S_OK != pThis->m_pStream->Read(pbBuff, size, reinterpret_cast<ULONG*>(pcb)))
		{
			TRACE(_T("Unable to read stream data\n"));
			*pcb = 0;
			return 0;
		}

		ASSERT(XTPToULongChecked(*pcb) <= pThis->m_ulSize.QuadPart);
		pThis->m_ulSize.QuadPart -= XTPToULongChecked(*pcb);

		if (0 == pThis->m_ulSize.QuadPart)
		{
			pThis->m_pStream->Release();
			pThis->m_pStream = NULL;
		}
	}
	else
	{
		if (INVALID_HANDLE_VALUE == reinterpret_cast<HANDLE>(pThis->m_inFile.m_hFile))
		{
			*pcb = 0;
			return 0;
		}

		*pcb = XTPToInt(pThis->m_inFile.Read(pbBuff, XTPToUIntChecked(cb)));

		if (pThis->m_ulSize.QuadPart == pThis->m_inFile.GetPosition())
			pThis->m_inFile.Close();
	}

	return 0;
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
