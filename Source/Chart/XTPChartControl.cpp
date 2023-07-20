// XTPChartControl.cpp
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include "GraphicLibrary/OpenGL/GL.h"
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "GraphicLibrary/GdiPlus/XTPGdiPlus.h"
#include "GraphicLibrary/XTGraphicUtils.h"

#include "Common/XTPTypeId.h"
#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPTypeId.h"

#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeriesPoint.h"
#include "Chart/XTPChartControl.h"
#include "Chart/XTPChartContent.h"
#include "Chart/XTPChartContentView.h"
#include "Chart/XTPChartDiagramView.h"
#include "Chart/Appearance/XTPChartAppearance.h"
#include "Chart/Drawing/XTPChartDeviceContext.h"
#include "Chart/Drawing/XTPChartDeviceCommand.h"
#include "Chart/Drawing/XTPChartDrawThread.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPChartControl

const TCHAR XTPCHART_CLASSNAME[] = _T("XTPChartControl");

CXTPChartControl::CXTPChartControl()
{
	m_pCommand		  = NULL;
	m_pDrawThread	 = NULL;
	m_bUpdateWindow   = FALSE;
	m_pContentView	= NULL;
	m_dwUpdateOptions = 0;
	m_pCaptureView	= NULL;
	m_pToolTipContext = NULL;

	VERIFY(RegisterWindowClass());
	m_pContent = new CXTPChartContent();
	m_pContent->AddContainer(this);
}

CXTPChartControl::~CXTPChartControl()
{
	SAFE_RELEASE(m_pContentView);

	// End the monitoring thread.
	if (m_pDrawThread)
	{
		m_pDrawThread->StopNotifications();
		m_pDrawThread = NULL;
	}

	SAFE_RELEASE(m_pCommand);

	CMDTARGET_RELEASE(m_pToolTipContext);

	SetContent(NULL);
}

void CXTPChartControl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
}

void CXTPChartControl::SetContent(CXTPChartContent* pContent)
{
	if (m_pContent == pContent)
		return;

	if (m_pContent)
	{
		m_pContent->RemoveContainer(this);
		SAFE_RELEASE(m_pContent);
	}

	if (pContent)
	{
		m_pContent = pContent;
		m_pContent->AddContainer(this);
		pContent->InternalAddRef();
	}

	OnChartChanged();
}

BOOL CXTPChartControl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	WNDCLASS wndcls;
	ZeroMemory(&wndcls, sizeof(wndcls));

	wndcls.style		 = CS_DBLCLKS;
	wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.hInstance	 = hInstance ? hInstance : XTPGetInstanceHandle();
	wndcls.hCursor		 = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.lpszClassName = XTPCHART_CLASSNAME;
	wndcls.hIcon		 = 0;
	wndcls.hbrBackground = 0;

	return AfxRegisterClass(&wndcls);
}

void CXTPChartControl::EnableToolTips(BOOL bEnable /* = TRUE */)
{
	CMDTARGET_RELEASE(m_pToolTipContext);

	if (bEnable)
	{
		m_pToolTipContext = new CXTPToolTipContext();
	}
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPChartControl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

BOOL CXTPChartControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(XTPCHART_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, NULL);
}

void CXTPChartControl::Draw(CDC* pPaintDC, CRect rc)
{
	CXTPChartDeviceContext* pDC = m_pContent->CreateDeviceContext(this, pPaintDC->GetSafeHdc(), rc,
																  FALSE);

	if (NULL != pDC)
	{
		if (pDC->Is3D())
		{
			m_pContent->GetAppearance()->GetDiagram3DAppearance()->ApplyToDeviceContext(pDC);
		}
		else
		{
			m_pContent->GetAppearance()->GetDiagram2DAppearance()->ApplyToDeviceContext(pDC);
		}

		if (pDC->Initialize())
		{
			if (NULL == m_pCommand)
			{
				if (m_pContentView == 0
					|| ((m_dwUpdateOptions & xtpChartUpdateView) && m_pCaptureView == NULL))
				{
					if (NULL != m_pCaptureView)
					{
						m_pCaptureView = NULL;
						ReleaseCapture();
					}

					SAFE_RELEASE(m_pContentView);
					m_pContentView = m_pContent->CreateView(pDC, rc);
				}
				else if (m_dwUpdateOptions & xtpChartUpdateLayout)
				{
					m_pContentView->CalculateView(pDC, rc);
				}

				CXTPChartDeviceCommand* pCommand = NULL;

				if (NULL != m_pContentView)
				{
					pCommand = m_pContentView->CreateDeviceCommand(pDC);
				}

				m_pCommand = new CXTPChartDrawThreadDeviceCommand(pCommand);
			}

			pDC->Execute(m_pCommand);
		}

		delete pDC;
	}
}

void CXTPChartControl::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(rc);

#if 0
	_int64 nPerfomanceEnd;
	_int64 nPerfomanceStart;

	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceStart);
#endif

	if (m_bmpCache.GetSafeHandle() == 0)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		m_bmpCache.DeleteObject();
		m_bmpCache.Attach(CXTPImageManager::Create32BPPDIBSection(0, rc.Width(), rc.Height(), 0));

		CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

		Draw(&memDC, rc);

		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
	}
	else
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		CBitmap* pOldBitmap = memDC.SelectObject(&m_bmpCache);

		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
	}

	m_dwUpdateOptions = 0;

#if 0
	QueryPerformanceCounter((LARGE_INTEGER*)&nPerfomanceEnd);
	TRACE(_T("CXTPChartControl::OnPaint: %i \n"),  int(nPerfomanceEnd - nPerfomanceStart));
#endif
}

BOOL CXTPChartControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPChartControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPChartControl::OnChartChanged(XTPChartUpdateOptions updateOptions)
{
	m_bmpCache.DeleteObject();

	SAFE_RELEASE(m_pCommand);

	m_dwUpdateOptions |= updateOptions;

	if ((updateOptions & xtpChartUpdateView) && (m_pCaptureView == NULL))
	{
		SAFE_RELEASE(m_pContentView);
	}

	if (::IsWindow(m_hWnd))
	{
		Invalidate(FALSE);

		if (m_bUpdateWindow)
		{
			UpdateWindow();
		}
	}
}

void CXTPChartControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	OnChartChanged(xtpChartUpdateLayout);
}

CXTPChartElement* CXTPChartControl::HitTest(CPoint point) const
{
	if (!m_pCommand)
		return m_pContent;

	return m_pCommand->HitTest(point, m_pContent);
}

BOOL CXTPChartControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pContentView)
	{
		ScreenToClient(&pt);

		CXTPChartDiagramView* pDiagramView = m_pContentView->HitTestDiagramView(pt);

		if (pDiagramView)
		{
			pDiagramView->OnMouseWheel(nFlags, zDelta, pt);
		}
	}

	return TRUE;
}

void CXTPChartControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	// DWORD dwPoint = MAKELONG(point.x, point.y);

	if (m_pCaptureView)
	{
		m_pCaptureView->OnMouseMove(nFlags, point);
	}
	else
	{
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseMove(AxButtonState(), AxShiftState(), point.x, point.y);
#endif
}

void CXTPChartControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPChartControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

BOOL CXTPChartControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if (m_pCaptureView)
	{
		if (m_pCaptureView->OnSetCursor(point))
			return TRUE;
	}
	else if (m_pContentView)
	{
		CXTPChartDiagramView* pDiagramView = m_pContentView->HitTestDiagramView(point);

		if (pDiagramView)
		{
			if (pDiagramView->OnSetCursor(point))
				return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPChartControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	if (NULL != m_pContentView)
	{
		CXTPChartDiagramView* pDiagramView = m_pContentView->HitTestDiagramView(point);
		if (NULL != pDiagramView)
		{
			pDiagramView->OnLButtonUp(nFlags, point);
		}
	}

	if (NULL != m_pCaptureView)
	{
		m_pCaptureView = NULL;
		ReleaseCapture();
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif
}
void CXTPChartControl::OnCaptureChanged(CWnd* pWnd)
{
	CWnd::OnCaptureChanged(pWnd);

	if (IsBeingFrequentlyUpdated())
	{
		EndFrequentUpdates();
	}

	m_pCaptureView = NULL;
}

void CXTPChartControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())
		->FireEvent(DISPID_DBLCLICK, EVENT_PARAM(VTS_I2 VTS_I2 VTS_XPOS_PIXELS VTS_YPOS_PIXELS),
					(short)VK_LBUTTON, (short)AxShiftState(), point.x, point.y);
#endif
}

void CXTPChartControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus();

	NMHDR mHDR;
	mHDR.hwndFrom = m_hWnd;
	mHDR.code	 = NM_CLICK;
	mHDR.idFrom   = XTPToUIntPtr(GetDlgCtrlID());

	GetOwner()->SendMessage(WM_NOTIFY, (WPARAM)mHDR.idFrom, (LPARAM)&mHDR);

	if (NULL != m_pContentView)
	{
		CXTPChartDiagramView* pDiagramView = m_pContentView->HitTestDiagramView(point);
		if (NULL != pDiagramView)
		{
			pDiagramView->OnLButtonDown(nFlags, point);
		}
	}

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPChartControl::SetCapture(CXTPChartElementView* pView)
{
	m_pCaptureView = pView;

	CWnd::SetCapture();
}

void CXTPChartControl::PrintToDC(HDC hDC, CRect rcBounds)
{
	CXTPChartDeviceContext* pDC = m_pContent->CreateDeviceContext(this, hDC, rcBounds, FALSE);
	if (NULL != pDC)
	{
		m_pContent->DrawContent(pDC, rcBounds);
		delete pDC;
	}
}

void CXTPChartControl::SaveAsImage(LPCTSTR lpszFilePath, CSize szBounds)
{
	CBitmap bmp;
	bmp.Attach(CXTPImageManager::Create32BPPDIBSection(0, szBounds.cx, szBounds.cy, 0));

	{
		CDC dc;
		dc.CreateCompatibleDC(NULL);

		CBitmap* pOldBitmap = dc.SelectObject(&bmp);

		CRect rc(0, 0, szBounds.cx, szBounds.cy);
		CXTPChartDeviceContext* pDC = m_pContent->CreateDeviceContext(this, dc, rc, FALSE);
		if (NULL != pDC)
		{
			if (pDC->Initialize())
			{
				m_pContent->DrawContent(pDC, rc);
			}
			else
			{
				TRACE(_T("Unable to initialize chart device context.\n"));
			}

			delete pDC;
		}
		else
		{
			TRACE(_T("Unable to create chart device context.\n"));
		}

		dc.SelectObject(pOldBitmap);
	}

	CXTPGraphicUtils::SaveBitmapToFile((HBITMAP)bmp.GetSafeHandle(), lpszFilePath);
}

BOOL CXTPChartControl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

INT_PTR CXTPChartControl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CXTPChartSeriesPoint* pSeriesPoint = DYNAMIC_DOWNCAST(CXTPChartSeriesPoint, HitTest(point));

	if (pSeriesPoint)
	{
		nHit = (INT_PTR)pSeriesPoint;

		CString strTip = pSeriesPoint->GetTooltipText();
		if (strTip.GetLength() == 0)
			return -1;

		CRect rc;
		GetClientRect(rc);

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, rc, nHit, strTip);

		return nHit;
	}
	return -1;
}

void CXTPChartControl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, TRUE)
#endif
}

void CXTPChartControl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, FALSE)
#endif
}
