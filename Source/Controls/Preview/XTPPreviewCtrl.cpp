// XTPPreviewCtrl.cpp : implementation of the CXTPPreviewCtrl class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"

#include "Controls/Preview/XTPPreviewAbstract.h"
#include "Controls/Preview/XTPPreviewCtrl.h"
#include "Controls/Preview/XTPPreviewHostWindow.h"
#include "Controls/Preview/XTPPreviewNavigatorWindow.h"

#include "Controls/Preview/Providers/System/XTPSystemPreviewProvider.h"
#include "Controls/Preview/Providers/CommonPicture/XTPCommonPicturePreviewProvider.h"
#include "Controls/Preview/Providers/GIF/XTPGifPreviewProvider.h"
#include "Controls/Preview/Providers/MSG/XTPMsgPreviewProvider.h"
#include "Controls/Preview/Providers/Text/XTPTextPreviewProvider.h"
#include "Controls/Preview/Providers/XAML/XTPXamlPreviewProvider.h"

#include "Controls/Resource.h"

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#define XTP_PREVIEW_CLASSNAME _T("XTPPreviewCtrl")

#ifndef WS_EX_COMPOSITED
#	define WS_EX_COMPOSITED 0x02000000L
#endif

IMPLEMENT_DYNAMIC(CXTPPreviewCtrl, CWnd)

CXTPPreviewCtrl::CXTPPreviewCtrl()
	: m_bChildrenCreated(FALSE)
	, m_bShowNavigator(FALSE)
	, m_bSubclassed(TRUE)
	, m_hCompositionWnd(NULL)
	, m_crBack(COLORREF_NULL)
	, m_crFore(COLORREF_NULL)
	, m_bAutoActivate(FALSE)
{
	VERIFY(m_strLoadingMessage.LoadString(XTP_IDS_PREVIEW_DEFAULT_LOADING_MESSAGE));
	VERIFY(m_strFailedMessage.LoadString(XTP_IDS_PREVIEW_DEFAULT_FAILED_MESSAGE));

	RegisterWindowClass();
	m_iNavHeight	   = CalcNavigatorHeight();
	m_pHostWindow	  = new CXTPPreviewHostWindow(this);
	m_pNavigatorWindow = new CXTPPreviewNavigatorWindow(this);
}

CXTPPreviewCtrl::~CXTPPreviewCtrl()
{
	for (int i = 0; i < m_providers.GetSize(); i++)
	{
		IXTPPreviewProvider* provider = reinterpret_cast<IXTPPreviewProvider*>(
			m_providers.GetAt(i));
		delete provider;
		m_providers.SetAt(i, NULL);
	}

	m_providers.RemoveAll();
	m_additionalAssociations.RemoveAll();

	delete m_pHostWindow;
	delete m_pNavigatorWindow;
}

BOOL CXTPPreviewCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
							 CCreateContext* pContext /*= NULL*/)
{
	return CWnd::Create(XTP_PREVIEW_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CXTPPreviewCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth,
							   int nHeight, HWND hWndParent, HMENU nIDorHMenu,
							   LPVOID lpParam /*= NULL*/)
{
	return CWnd::CreateEx(dwExStyle, XTP_PREVIEW_CLASSNAME, NULL, dwStyle, x, y, nWidth, nHeight,
						  hWndParent, nIDorHMenu, lpParam);
}

BOOL CXTPPreviewCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
							   UINT nID, LPVOID lpParam /*= NULL*/)
{
	return CWnd::CreateEx(dwExStyle, XTP_PREVIEW_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID,
						  lpParam);
}

void CXTPPreviewCtrl::Preview(LPCTSTR lpFilePath)
{
	Preview(NULL, lpFilePath);
}

void CXTPPreviewCtrl::Preview(IStream* pStream, LPCTSTR lpImpliedFileName)
{
	m_pHostWindow->SetPreview(NULL);

	m_bShowNavigator = FALSE;
	ChangeChildrenPosition(NULL);

	if (NULL == lpImpliedFileName)
		return;

	CString message;

	CString sFileName(::PathFindFileName(lpImpliedFileName));

	LPCTSTR lpExt = ::PathFindExtension(lpImpliedFileName);
	ASSERT(NULL != lpExt);

	if (_T('.') == *lpExt)
		++lpExt;

	if (_T('\0') == *lpExt)
	{
		message = m_strFailedMessage;
		message.Replace(_T("%"), sFileName);
		m_pHostWindow->ShowMessage(message);
		return;
	}

	message = m_strLoadingMessage;
	message.Replace(_T("%"), sFileName);
	m_pHostWindow->ShowMessage(message);

	message = m_strFailedMessage;
	message.Replace(_T("%"), sFileName);

	CList<IXTPPreviewProvider*, IXTPPreviewProvider*> foundProviders;
	for (int i = 0; i < m_providers.GetSize(); i++)
	{
		IXTPPreviewProvider* provider = static_cast<IXTPPreviewProvider*>(m_providers.GetAt(i));
		LPCTSTR lpDefAssoc			  = provider->GetDefaultFileAssociations();
		if ((NULL != lpDefAssoc ? IsExtensionInString(lpDefAssoc, lpExt) : TRUE)
			|| IsExtensionInString(m_additionalAssociations.GetAt(i), lpExt))
		{
			foundProviders.AddTail(provider);
		}
	}

	if (foundProviders.IsEmpty())
	{
		m_pHostWindow->ShowMessage(message);
		return;
	}

	IXTPPreview* pPreview = NULL;
	while (!foundProviders.IsEmpty() && NULL == pPreview)
	{
		if (NULL != pStream)
			pPreview = foundProviders.GetHead()->CreatePreview(*pStream, lpImpliedFileName,
															   *m_pHostWindow);
		else
			pPreview = foundProviders.GetHead()->CreatePreview(lpImpliedFileName, *m_pHostWindow);

		foundProviders.RemoveHead();
	}

	if (NULL == pPreview)
	{
		m_pHostWindow->ShowMessage(message);
		return;
	}

	while (!pPreview->IsReady())
	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				AfxPostQuitMessage((int)msg.wParam);
				break;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	if (!pPreview->IsAvailable())
	{
		delete pPreview;
		m_pHostWindow->ShowMessage(message);
		return;
	}

	m_pHostWindow->SetPreview(pPreview);

	if ((pPreview->GetNumberOfPages() > 1) && (!pPreview->HasOwnNavigator()))
		m_bShowNavigator = TRUE;
	else
		m_bShowNavigator = FALSE;

	if (NULL != m_hCompositionWnd)
	{
		LONG_PTR lCompExStyle = ::GetWindowLongPtr(m_hCompositionWnd, GWL_EXSTYLE);
		if (pPreview->SupportsCompositedParent() && 0 == (lCompExStyle & WS_EX_COMPOSITED))
			::SetWindowLongPtr(m_hCompositionWnd, GWL_EXSTYLE, lCompExStyle | WS_EX_COMPOSITED);

		if (!pPreview->SupportsCompositedParent() && 0 != (lCompExStyle & WS_EX_COMPOSITED))
			::SetWindowLongPtr(m_hCompositionWnd, GWL_EXSTYLE, lCompExStyle & ~WS_EX_COMPOSITED);
	}

	SetWindowPos(NULL, m_rPrevPlace.left, m_rPrevPlace.top, m_rPrevPlace.Width(),
				 m_rPrevPlace.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);

	if (m_bAutoActivate)
		pPreview->Activate();

	UpdateWindow();
}

void CXTPPreviewCtrl::AddProvider(IXTPPreviewProvider* provider, LPCTSTR associations /*= NULL*/)
{
	m_providers.Add(provider);
	m_additionalAssociations.Add(NULL != associations ? associations : _T(""));

	ASSERT(NULL != m_pHostWindow);
	provider->BindHost(*m_pHostWindow);
}

void CXTPPreviewCtrl::AddDefaultProviders()
{
	AddProvider(new CXTPSystemPreviewProvider());
	AddProvider(new CXTPCommonPicturePreviewProvider());
	AddProvider(new CXTPGifPreviewProvider());
	AddProvider(new CXTPMsgPreviewProvider());
	AddProvider(new CXTPTextPreviewProvider());
	AddProvider(new CXTPXamlPreviewProvider());
}

void CXTPPreviewCtrl::SetLoadingMessage(LPCTSTR message)
{
	m_strLoadingMessage = message;
}

void CXTPPreviewCtrl::SetFailedMessage(LPCTSTR message)
{
	m_strFailedMessage = message;
}

BOOL CXTPPreviewCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!::GetClassInfo(hInst, XTP_PREVIEW_CLASSNAME, &wndcls))
	{
		wndcls.style	   = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance					  = hInst;
		wndcls.hIcon						  = NULL;
		wndcls.hCursor						  = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground				  = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
		wndcls.lpszMenuName					  = NULL;
		wndcls.lpszClassName				  = XTP_PREVIEW_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPreviewCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(XTP_IDC_NAVIGATOR_FIRST_BTN, OnFirst)
	ON_COMMAND(XTP_IDC_NAVIGATOR_LAST_BTN, OnLast)
	ON_COMMAND(XTP_IDC_NAVIGATOR_PREV_BTN, OnPrev)
	ON_COMMAND(XTP_IDC_NAVIGATOR_NEXT_BTN, OnNext)
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPPreviewCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_CONTROLPARENT;
	m_bSubclassed = FALSE;

	return CWnd::PreCreateWindow(cs);
}

BOOL CXTPPreviewCtrl::IsExtensionInString(const CString& strAssociation, LPCTSTR lpExt) const
{
	ASSERT(NULL != lpExt);

	LPCTSTR lpTokenStart = strAssociation;
	LPCTSTR lpTokenEnd;
	LPCTSTR lpEnd = lpTokenStart + strAssociation.GetLength();

	while (lpTokenStart < lpEnd)
	{
		lpTokenEnd = _tcschr(lpTokenStart, _T(';'));
		if (NULL == lpTokenEnd)
		{
			lpTokenEnd = lpEnd;
		}

		LPCTSTR lpCh	= lpTokenStart;
		LPCTSTR lpExtCh = lpExt;
		while (lpCh < lpTokenEnd && _T('\0') != *lpExtCh)
		{
			if (tolower(*lpCh) != tolower(*lpExtCh))
				break;

			++lpCh;
			++lpExtCh;
		}

		if (lpCh == lpTokenEnd && _T('\0') == *lpExtCh)
			return TRUE;

		lpTokenStart = lpTokenEnd + 1;
	}

	return FALSE;
}

int CXTPPreviewCtrl::CalcNavigatorHeight()
{
	CBitmap cbm;
	VERIFY(cbm.LoadBitmap(XTP_IDR_FIRST_BITMAP));

	BITMAP bm = { 0 };
	cbm.GetBitmap(&bm);
	return (bm.bmHeight + bm.bmHeight / 2);
}

void CXTPPreviewCtrl::CreateHostAndNavigator()
{
	if (!m_bChildrenCreated)
	{
		HCURSOR hArrowCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

		CRect rec;
		GetClientRect(&rec);

		LPCTSTR lpNavigatorWndClass = AfxRegisterWndClass(
			CS_DBLCLKS, hArrowCursor, reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1), NULL);
		ASSERT(NULL != lpNavigatorWndClass);

		m_pNavigatorWindow->CreateEx(
			WS_EX_CONTROLPARENT, lpNavigatorWndClass, _T("File Preview Navigator"),
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CRect(0, rec.Height() - m_iNavHeight, rec.Width(), rec.Height()), this, 0);

		LPCTSTR lpHostWndClass = AfxRegisterWndClass(CS_DBLCLKS, hArrowCursor,
													 reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
													 NULL);
		ASSERT(NULL != lpHostWndClass);

		m_pHostWindow->CreateEx(WS_EX_CONTROLPARENT, lpHostWndClass, _T("File Preview Host"),
								WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
								CRect(0, 0, rec.Width(), rec.Height()), this, 0);

		m_bChildrenCreated = TRUE;
	}
}

void CXTPPreviewCtrl::PreSubclassWindow()
{
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	CWnd::PreSubclassWindow();

	if (m_bSubclassed)
		CreateHostAndNavigator();
}

int CXTPPreviewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateHostAndNavigator();
	return 0;
}

void CXTPPreviewCtrl::OnDestroy()
{
	CWnd::OnDestroy();
	m_bChildrenCreated = FALSE;
}

void CXTPPreviewCtrl::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
	ChangeChildrenPosition(lpwndpos);
}

void CXTPPreviewCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	ChangeChildrenPosition(lpwndpos);
}

void CXTPPreviewCtrl::OnFirst()
{
	m_pHostWindow->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_FIRST_BTN, BN_CLICKED),
							   XTPToLPARAM(m_hWnd));
}

void CXTPPreviewCtrl::OnLast()
{
	m_pHostWindow->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_LAST_BTN, BN_CLICKED),
							   XTPToLPARAM(m_hWnd));
}

void CXTPPreviewCtrl::OnPrev()
{
	m_pHostWindow->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_PREV_BTN, BN_CLICKED),
							   XTPToLPARAM(m_hWnd));
}

void CXTPPreviewCtrl::OnNext()
{
	m_pHostWindow->SendMessage(WM_COMMAND, MAKELONG(XTP_IDC_NAVIGATOR_NEXT_BTN, BN_CLICKED),
							   XTPToLPARAM(m_hWnd));
}

void CXTPPreviewCtrl::ChangeChildrenPosition(WINDOWPOS* lpwndpos)
{
	WINDOWPOS wndPos;
	if (NULL == lpwndpos)
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		VERIFY(GetWindowPlacement(&wp));

		wndPos.x  = wp.rcNormalPosition.left;
		wndPos.y  = wp.rcNormalPosition.top;
		wndPos.cx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		wndPos.cy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		lpwndpos  = &wndPos;
	}

	m_rPrevPlace.left   = lpwndpos->x;
	m_rPrevPlace.top	= lpwndpos->y;
	m_rPrevPlace.right  = lpwndpos->x + lpwndpos->cx;
	m_rPrevPlace.bottom = lpwndpos->y + lpwndpos->cy;

	if ((lpwndpos->cx > 0) && (lpwndpos->cy > 0))
	{
		CRect r;
		GetClientRect(&r);
		int cx = r.Width(), cy = r.Height();
		if (m_bShowNavigator)
			cy = cy - m_iNavHeight;

		HDWP dwp = BeginDeferWindowPos(2);
		dwp		 = DeferWindowPos(dwp, *m_pHostWindow, NULL, 0, 0, cx, cy - 1, SWP_NOZORDER);
		dwp = DeferWindowPos(dwp, *m_pNavigatorWindow, NULL, 0, cy, cx, m_iNavHeight, SWP_NOZORDER);
		EndDeferWindowPos(dwp);
	}
}

#include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
