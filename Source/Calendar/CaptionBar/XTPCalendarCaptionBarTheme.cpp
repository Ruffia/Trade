// XTPCalendarCaptionBarTheme.cpp : implementation file
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

#include "Calendar/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarViewPart.h"
#include "Calendar/XTPCalendarView.h"
#include "Calendar/XTPCalendarTimeLineView.h"
#include "Calendar/XTPCalendarControl.h"
#include "Calendar/XTPCalendarMonthView.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarThemePart.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarTheme.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarControl.h"
#include "Calendar/CaptionBar/XTPCalendarCaptionBarPaintManager.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2000Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOfficeXPTheme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2003Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarOffice2013Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarVisualStudio2015Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarVisualStudio2017Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarVisualStudio2019Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarVisualStudio2022Theme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarResourceTheme.h"
#include "Calendar/CaptionBar/Themes/XTPCalendarCaptionBarNativeWindows10Theme.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CXTPCalendarCaptionBarTheme, CXTPCalendarWMHandler)

//////////////////////////////////////////////////////////////////////////

CXTPCalendarCaptionBarTheme::CXTPCalendarCaptionBarTheme()
{
	m_pSink = new CXTPNotifySink();

	m_pPaintManager			= NULL;
	m_pCaptionBar			= NULL;
	m_pCalendar				= NULL;
	m_nCheckHotStateTimerID = 0;

	m_bIsSwitchViewButtons = TRUE;
	m_bIsScrollDateButtons = TRUE;
	m_bIsDateLabel		   = TRUE;
	m_bTimelineEnable	  = TRUE;
	m_bMulticolumnsEnable  = TRUE;
	m_bIsExpandButton	  = FALSE;
	m_bIsOneLine		   = FALSE; // Old Layout - default
	m_pConnection		   = NULL;
	m_dScale			   = 1.0;

	// Init Theme Parts
	m_pButtonViewDay   = new CXTPSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewDay);
	m_pButtonViewWeek  = new CXTPSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewWeek);
	m_pButtonViewMonth = new CXTPSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewMonth);

	m_pButtonViewDay->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_DAY));
	m_pButtonViewWeek->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_WEEK));
	m_pButtonViewMonth->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_MONTH));

	m_pShowWorkWeek = new CXTPRadioButtonPart(this, xtpCalendarCaptionBar_ShowWorkWeek);
	m_pShowFullWeek = new CXTPRadioButtonPart(this, xtpCalendarCaptionBar_ShowFullWeek);

	CString s = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_SHOWWORKWEK);
	s.Replace(_T("Show w"), _T("W"));
	m_pShowWorkWeek->SetText(s);
	m_pShowWorkWeek->SetCheck(0);

	s = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_SHOWFULLKWEK);
	s.Replace(_T("Show f"), _T("F"));
	m_pShowFullWeek->SetText(s);
	m_pShowFullWeek->SetCheck(0);

	m_pShowTimeline = new CXTPCheckboxButtonPart(this, xtpCalendarCaptionBar_ShowTimeline);
	s				= XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_TIMELINE_MODE);
	m_pShowTimeline->SetText(s);
	m_pShowTimeline->SetCheck(0);

	m_pShowMcWeek = new CXTPCheckboxButtonPart(this, xtpCalendarCaptionBar_MultiColumnsWeek);
	// m_pShowMcWeek = new CXTPRadioButtonPart(this, xtpCalendarCaptionBar_MultiColumnsWeek);
	s = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_MULTICOLUMN_MODE);
	m_pShowMcWeek->SetText(s);
	m_pShowMcWeek->SetCheck(0);

	m_pScrollPrevDate = new CXTPScrollDateButtonPart(this, xtpCalendarCaptionBar_ScrollPrevDate);
	m_pScrollNextDate = new CXTPScrollDateButtonPart(this, xtpCalendarCaptionBar_ScrollNextDate);

	m_pDateLabel = new CXTPDateLabelPart(this, xtpCalendarCaptionBar_DateLabel);

	m_pExpandButton = new CXTPExpandButtonPart(this, xtpCalendarCaptionBar_ButtonExpand);

	SetPaintTheme(xtpCalendarThemeResource);

	RefreshMetrics();
}

CXTPCalendarCaptionBarTheme::~CXTPCalendarCaptionBarTheme()
{
	m_pSink->Delete();

	CMDTARGET_RELEASE(m_pConnection);

	CMDTARGET_RELEASE(m_pButtonViewDay);
	CMDTARGET_RELEASE(m_pButtonViewWeek);
	CMDTARGET_RELEASE(m_pButtonViewMonth);

	CMDTARGET_RELEASE(m_pShowWorkWeek);
	CMDTARGET_RELEASE(m_pShowFullWeek);

	CMDTARGET_RELEASE(m_pShowTimeline);
	CMDTARGET_RELEASE(m_pShowMcWeek);

	CMDTARGET_RELEASE(m_pScrollPrevDate);
	CMDTARGET_RELEASE(m_pScrollNextDate);

	CMDTARGET_RELEASE(m_pDateLabel);
	CMDTARGET_RELEASE(m_pExpandButton);

	CMDTARGET_RELEASE(m_pCalendar);
	CMDTARGET_RELEASE(m_pPaintManager);
}

void CXTPCalendarCaptionBarTheme::OnBeforeDestroy()
{
	m_pSink->UnadviseAll();
	CMDTARGET_RELEASE(m_pCalendar);
}

void CXTPCalendarCaptionBarTheme::SetOwner(CXTPCalendarCaptionBarControl* pCaptionBar)
{
	ASSERT(pCaptionBar);
	if (pCaptionBar)
	{
		m_pCaptionBar = pCaptionBar;
	}
}

void CXTPCalendarCaptionBarTheme::AttachCalendar(CXTPCalendarControl* pCalendar)
{
	m_pSink->UnadviseAll();
	CMDTARGET_RELEASE(m_pCalendar);

	m_pCalendar = pCalendar;
	CMDTARGET_ADDREF(m_pCalendar);

	if (m_pCalendar && m_pCalendar->GetConnection())
	{
		CXTPNotifyConnection* pConnection = m_pCalendar->GetConnection();

		m_pSink->Advise(
			pConnection, XTP_NC_CALENDARVIEWWASCHANGED,
			CreateNotfySinkClassDelegate(this, &CXTPCalendarCaptionBarTheme::OnEvent_Calendar));
		m_pSink->Advise(
			pConnection, XTP_NC_CALENDAR_THEME_CHANGED,
			CreateNotfySinkClassDelegate(this, &CXTPCalendarCaptionBarTheme::OnEvent_Calendar));

		m_pCaptionBar->AdjustLayout();

		if (XTPResourceImages() && XTPResourceImages()->GetConnection())
		{
			m_pSink->Advise(XTPResourceImages()->GetConnection(),
							XTP_NC_COMMON_RESOURCEIMAGES_CHANGED,
							CreateNotfySinkClassDelegate(
								this, &CXTPCalendarCaptionBarTheme::OnEvent_ResourceImagesChanged));
		}

		SetPaintTheme(GetCalendarCtrl()->GetPaintTheme());
	}
}

void CXTPCalendarCaptionBarTheme::SetPaintTheme(CXTPCalendarCaptionBarPaintManager* pPaintManager)
{
	// set default value.
	if (pPaintManager == NULL)
		pPaintManager = new CXTPCalendarCaptionBarOffice2000Theme();

	CMDTARGET_RELEASE(m_pPaintManager);
	m_pPaintManager = pPaintManager;

	RefreshMetrics();
	Redraw();
}

void CXTPCalendarCaptionBarTheme::SetPaintTheme(XTPCalendarTheme nTheme)
{
	switch (nTheme)
	{
		case xtpCalendarThemeOffice2000:
			SetPaintTheme(new CXTPCalendarCaptionBarOffice2000Theme(this));
			break;

		case xtpCalendarThemeOfficeXP:
			SetPaintTheme(new CXTPCalendarCaptionBarOfficeXPTheme(this));
			break;

		case xtpCalendarThemeOffice2003:
			SetPaintTheme(new CXTPCalendarCaptionBarOffice2003Theme(this));
			break;

		case xtpCalendarThemeOffice2013:
			SetPaintTheme(new CXTPCalendarCaptionBarOffice2013Theme(this));
			break;

		case xtpCalendarThemeVisualStudio2015:
			SetPaintTheme(new CXTPCalendarCaptionBarVisualStudio2015Theme(this));
			break;

		case xtpCalendarThemeVisualStudio2017:
			SetPaintTheme(new CXTPCalendarCaptionBarVisualStudio2017Theme(this));
			break;

		case xtpCalendarThemeVisualStudio2019:
			SetPaintTheme(new CXTPCalendarCaptionBarVisualStudio2019Theme(this));
			break;

		case xtpCalendarThemeVisualStudio2022:
			SetPaintTheme(new CXTPCalendarCaptionBarVisualStudio2022Theme(this));
			break;

		case xtpCalendarThemeNativeWindows10:
			SetPaintTheme(new CXTPCalendarCaptionBarNativeWindows10Theme(this));
			break;

		case xtpCalendarThemeResource:
			SetPaintTheme(new CXTPCalendarCaptionBarResourceTheme(this));
			break;

		case xtpCalendarThemeUnknown: ASSERT(!"xtpCalendarThemeUnknown cannot be set"); break;
	}
}

CXTPNotifyConnection* CXTPCalendarCaptionBarTheme::GetConnection()
{
	if (!m_pConnection)
		m_pConnection = new CXTPNotifyConnection();

	return m_pConnection;
}

int CXTPCalendarCaptionBarTheme::GetChildHandlersCount() const
{
	return m_arMembers.GetCount();
}

CXTPCalendarWMHandler* CXTPCalendarCaptionBarTheme::GetChildHandlerAt(int nIndex) const
{
	return m_arMembers.GetAt(nIndex);
}

void CXTPCalendarCaptionBarTheme::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCalendarWMHandler::OnMouseMove(nFlags, point);

	if (GetOwnerWnd() && m_nCheckHotStateTimerID == 0)
	{
		m_nCheckHotStateTimerID = GetOwnerWnd()->SetTimer(1, 100, NULL);
	}
}

BOOL CXTPCalendarCaptionBarTheme::OnTimer(UINT_PTR uTimerID)
{
	if (uTimerID == m_nCheckHotStateTimerID && GetOwnerWnd()
		&& ::GetCapture() != GetOwnerWnd()->GetSafeHwnd())
	{
		BOOL bMouseLeave;
		BOOL bChanged = _UpdateControlsHotState(&bMouseLeave);

		if (bMouseLeave)
		{
			GetOwnerWnd()->KillTimer(1);
			m_nCheckHotStateTimerID = 0;
		}

		if (bChanged)
			Redraw();

		return TRUE;
	}
	return FALSE;
}

CRect CXTPCalendarCaptionBarTheme::CalcMinRect(CDC* /*pDC*/, BOOL bExactSize)
{
	if (!IsSwitchViewButtons() && !IsScrollDateButtons() && !IsDateLabel() && !IsExpandButton())
		return CRect(0, 0, 0, 0);

	int nHeight = 0;
	if (m_bIsOneLine)
	{
		nHeight += XTP_DPI_Y(27);
	}
	else
	{
		if (IsSwitchViewButtons())
			nHeight += XTP_DPI_Y(27);

		int nH1 = (IsScrollDateButtons() || IsDateLabel()) ? XTP_DPI_Y(32) : 0;
		int nH2 = IsExpandButton() ? XTP_DPI_Y(30) : 0;

		nHeight += max(nH2, nH1);

		if (!bExactSize)
			nHeight += XTP_DPI_Y(11);
	}
	return CRect(0, 0, XTP_DPI_X(525), nHeight);
};

void CXTPCalendarCaptionBarTheme::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	if (!pDC && GetOwnerWnd() && GetOwnerWnd()->m_hWnd)
	{
		CClientDC dc(GetOwnerWnd());
		AdjustLayout(&dc, rcRect);
		return;
	}

	if (rcRect.IsRectNull() || rcRect.IsRectEmpty())
		return;

	m_rcRect = rcRect;
	CRect rc00(rcRect.left, rcRect.top, rcRect.left, rcRect.top);
	CRect rcClient = m_rcRect;

	CRect rc = rcClient;

	int nNextY				= rc.top;
	BOOL bSwitchViewButtons = IsSwitchViewButtons();
	BOOL bScrollDateButtons = IsScrollDateButtons();
	BOOL bShowExpandButton  = IsExpandButton();
	BOOL bShowDateLabel		= IsDateLabel();

	UpdateControlsState();

	int iWid1 = XTP_DPI_X(83);
	int iWid2 = XTP_DPI_X(20);
	int iWid3 = XTP_DPI_X(20);

	if (!m_bIsOneLine) // TWO LINE MODE
	{
		if (bSwitchViewButtons)
		{
			rc.bottom = rc.top + XTP_DPI_Y(26);

			if (!m_pButtonViewDay->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewDay->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewDay->GetRect().right;
			if (!m_pButtonViewWeek->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewWeek->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewWeek->GetRect().right;
			if (!m_pButtonViewMonth->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewMonth->AdjustLayout(pDC, rc);

			rc.left  = m_pButtonViewMonth->GetRect().right + XTP_DPI_X(21);
			rc.right = rc.left + iWid3;

			if (m_bTimelineEnable && !m_pShowTimeline->GetText().IsEmpty())
			{
				m_pShowTimeline->AdjustLayout(pDC, rc);

				rc.left  = m_pShowTimeline->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowWorkWeek->IsVisible())
			{
				m_pShowWorkWeek->AdjustLayout(pDC, rc);

				rc.left  = m_pShowWorkWeek->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowFullWeek->IsVisible())
			{
				m_pShowFullWeek->AdjustLayout(pDC, rc);
				rc.left  = m_pShowFullWeek->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowMcWeek->IsVisible())
			{
				m_pShowMcWeek->AdjustLayout(pDC, rc);
			}

			rc = m_pButtonViewDay->GetRect();

			nNextY = rc.bottom + XTP_DPI_Y(1);
		}
		else
		{
			m_pButtonViewDay->AdjustLayout(pDC, rc00);
			m_pButtonViewWeek->AdjustLayout(pDC, rc00);
			m_pButtonViewMonth->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->AdjustLayout(pDC, rc00);
			m_pShowFullWeek->AdjustLayout(pDC, rc00);
			m_pShowTimeline->AdjustLayout(pDC, rc00);
			m_pShowMcWeek->AdjustLayout(pDC, rc00);

			nNextY = rc.top;
		}

		int nNextX = rc.left;

		if (bScrollDateButtons)
		{
			rc.top = nNextY + XTP_DPI_Y(12);
			if (rc.top + XTP_DPI_Y(20) + XTP_DPI_Y(3) > rcClient.bottom)
				rc.top = max(nNextY + XTP_DPI_Y(3), rcClient.bottom - XTP_DPI_Y(23));

			rc.bottom = rc.top + XTP_DPI_Y(20);

			rc.left += m_nScrollDateButtonsMargin;

			rc.right = rc.left + iWid2;

			m_pScrollPrevDate->AdjustLayout(pDC, rc);

			rc.left  = m_pScrollPrevDate->GetRect().right + XTP_DPI_X(9);
			rc.right = rc.left + iWid2;

			m_pScrollNextDate->AdjustLayout(pDC, rc);

			nNextX = rc.right;
		}
		else
		{
			m_pScrollPrevDate->AdjustLayout(pDC, rc00);
			m_pScrollNextDate->AdjustLayout(pDC, rc00);
		}

		int nRightX = rcClient.right;
		if (bShowExpandButton)
		{
			int nDelta = (bScrollDateButtons || bShowDateLabel) ? XTP_DPI_Y(5) : XTP_DPI_Y(3);
			rc.top	 = nNextY + XTP_DPI_Y(13);
			if (rc.top + XTP_DPI_Y(18) + nDelta > rcClient.bottom)
				rc.top = max(nNextY + XTP_DPI_Y(1), rcClient.bottom - XTP_DPI_Y(18) - nDelta);

			rc.bottom = rc.top + XTP_DPI_Y(18);
			rc.right  = rcClient.right - XTP_DPI_X(2);
			rc.left   = rc.right - XTP_DPI_X(18);

			m_pExpandButton->AdjustLayout(pDC, rc);

			nRightX = m_pExpandButton->GetRect().left;
		}
		else
		{
			m_pExpandButton->AdjustLayout(pDC, rc00);
		}

		if (bShowDateLabel)
		{
			int nDelta = 3;
			rc.top	 = nNextY + XTP_DPI_Y(12);
			if (rc.top + XTP_DPI_Y(20) + nDelta > rcClient.bottom)
				rc.top = max(nNextY + XTP_DPI_Y(3), rcClient.bottom - XTP_DPI_Y(20) - nDelta);

			rc.bottom = rc.top + XTP_DPI_Y(22);
			rc.left   = nNextX + XTP_DPI_X(11);
			rc.right  = nRightX - XTP_DPI_X(2);

			m_pDateLabel->AdjustLayout(pDC, rc);
		}
		else
		{
			m_pDateLabel->AdjustLayout(pDC, rc00);
		}
	}
	else // ONE LINE MODE
	{
		int nNextX(0);
		if (bSwitchViewButtons)
		{
			if (!m_pButtonViewDay->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewDay->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewDay->GetRect().right;
			if (!m_pButtonViewWeek->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewWeek->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewWeek->GetRect().right;
			if (!m_pButtonViewMonth->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewMonth->AdjustLayout(pDC, rc);

			rc.left  = m_pButtonViewMonth->GetRect().right + XTP_DPI_X(21);
			rc.right = rc.left + iWid3;

			if (m_pShowTimeline->IsVisible())
			{
				m_pShowTimeline->AdjustLayout(pDC, rc);

				rc.left  = m_pShowTimeline->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowWorkWeek->IsVisible())
			{
				m_pShowWorkWeek->AdjustLayout(pDC, rc);
				rc.left  = m_pShowWorkWeek->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowFullWeek->IsVisible())
			{
				m_pShowFullWeek->AdjustLayout(pDC, rc);
				rc.left  = m_pShowFullWeek->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			if (m_pShowMcWeek->IsVisible())
			{
				m_pShowMcWeek->AdjustLayout(pDC, rc);

				rc.left  = m_pShowMcWeek->GetRect().right + XTP_DPI_X(11);
				rc.right = rc.left + iWid3;
			}

			nNextX = rc.left;
		}
		else
		{
			m_pButtonViewDay->AdjustLayout(pDC, rc00);
			m_pButtonViewWeek->AdjustLayout(pDC, rc00);
			m_pButtonViewMonth->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->AdjustLayout(pDC, rc00);
			m_pShowFullWeek->AdjustLayout(pDC, rc00);
			m_pShowTimeline->AdjustLayout(pDC, rc00);
			m_pShowMcWeek->AdjustLayout(pDC, rc00);
		}

		if (bScrollDateButtons)
		{
			rc.left  = nNextX + XTP_DPI_X(20);
			rc.right = rc.left + iWid2;

			m_pScrollPrevDate->AdjustLayout(pDC, rc);

			rc.left  = m_pScrollPrevDate->GetRect().right + XTP_DPI_X(9);
			rc.right = rc.left + iWid2;

			m_pScrollNextDate->AdjustLayout(pDC, rc);

			nNextX = rc.right;
		}
		else
		{
			m_pScrollPrevDate->AdjustLayout(pDC, rc00);
			m_pScrollNextDate->AdjustLayout(pDC, rc00);
		}

		int nRightX = rcClient.right;
		if (bShowExpandButton)
		{
			rc.right = rcClient.right - XTP_DPI_X(2);
			rc.left  = rc.right - XTP_DPI_X(18);

			m_pExpandButton->AdjustLayout(pDC, rc);

			nRightX = m_pExpandButton->GetRect().left;
		}
		else
		{
			m_pExpandButton->AdjustLayout(pDC, rc00);
		}

		if (bShowDateLabel)
		{
			rc.left  = nNextX + XTP_DPI_X(20);
			rc.right = nRightX - XTP_DPI_X(2);

			m_pDateLabel->AdjustLayout(pDC, rc);
		}
		else
		{
			m_pDateLabel->AdjustLayout(pDC, rc00);
		}
	}
}

void CXTPCalendarCaptionBarTheme::RefreshMetrics()
{
	if (m_pPaintManager)
	{
		m_pPaintManager->RefreshMetrics();
	}

	AdjustLayout(NULL, GetRect());
}

void CXTPCalendarCaptionBarTheme::Draw(CDC* pDC)
{
	if (m_pPaintManager)
	{
		m_pPaintManager->Draw(pDC);
	}
}

void CXTPCalendarCaptionBarTheme::DrawBorders(CDC* pDC, const CRect& rcRect, const CRect& rcBorders)
{
	if (m_pPaintManager)
	{
		m_pPaintManager->DrawBorders(pDC, rcRect, rcBorders);
	}
}

void CXTPCalendarCaptionBarTheme::Redraw(BOOL bUpdateNow)
{
	if (GetOwnerWnd() && GetOwnerWnd()->GetSafeHwnd())
	{
		GetOwnerWnd()->Invalidate(FALSE);

		if (bUpdateNow)
			GetOwnerWnd()->UpdateWindow();
	}
}

void CXTPCalendarCaptionBarTheme::OnEvent_Calendar(XTP_NOTIFY_CODE Event, WPARAM wParam,
												   LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (Event == XTP_NC_CALENDARVIEWWASCHANGED)
	{
		m_pCaptionBar->AdjustLayout();
	}
	else if (Event == XTP_NC_CALENDAR_THEME_CHANGED)
	{
		SetPaintTheme(GetCalendarCtrl()->GetPaintTheme());
	}
	else
	{
		ASSERT(FALSE);
		return;
	}

	Redraw();
}

void CXTPCalendarCaptionBarTheme::OnEvent_ResourceImagesChanged(XTP_NOTIFY_CODE Event,
																WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED)
	{
		RefreshMetrics();

		Redraw();
	}
}

void CXTPCalendarCaptionBarTheme::UpdateControlsState()
{
	if (!GetCalendarCtrl() || !GetCalendarCtrl()->GetActiveView())
		return;

	m_pShowWorkWeek->SetVisible(FALSE);
	m_pShowFullWeek->SetVisible(FALSE);
	m_pShowMcWeek->SetVisible(FALSE);

	BOOL bSwitchViewButtons = IsSwitchViewButtons();
	BOOL bScrollDateButtons = IsScrollDateButtons();
	BOOL bShowExpandButton  = IsExpandButton();
	BOOL bShowDateLabel		= IsDateLabel();

	m_pButtonViewDay->SetVisible(bSwitchViewButtons && !m_pButtonViewDay->GetText().IsEmpty());
	m_pButtonViewWeek->SetVisible(bSwitchViewButtons && !m_pButtonViewWeek->GetText().IsEmpty());
	m_pButtonViewMonth->SetVisible(bSwitchViewButtons && !m_pButtonViewMonth->GetText().IsEmpty());

	m_pScrollPrevDate->SetVisible(bScrollDateButtons);
	m_pScrollNextDate->SetVisible(bScrollDateButtons);

	m_pExpandButton->SetVisible(bShowExpandButton);
	m_pDateLabel->SetVisible(bShowDateLabel);

	m_pButtonViewDay->SetCheck(0);
	m_pButtonViewWeek->SetCheck(0);
	m_pButtonViewMonth->SetCheck(0);

	m_pShowTimeline->SetVisible(m_bTimelineEnable && !m_pShowTimeline->GetText().IsEmpty());

	int iScale	= GetCalendarCtrl()->GetTimeLineScale();
	int nViewType = GetCalendarCtrl()->GetActiveView()->GetViewType();

	BOOL bTimelineMode = GetCalendarCtrl()->m_bTimelineMode;
	m_pShowTimeline->SetCheck(bTimelineMode);

	if (nViewType == xtpCalendarDayView || (bTimelineMode && iScale == xtpTSPID_Day))
	{
		m_pButtonViewDay->SetCheck(1);
	}
	else if (nViewType == xtpCalendarMonthView || (bTimelineMode && iScale == xtpTSPID_Month))
	{
		m_pButtonViewMonth->SetCheck(1);
	}
	else
	{
		if (bTimelineMode && (iScale == xtpTSPID_Week || iScale == xtpTSPID_WorkWeek))
			m_pButtonViewWeek->SetCheck(1);

		if (bSwitchViewButtons)
		{
			m_pShowWorkWeek->SetVisible(!m_pShowWorkWeek->GetText().IsEmpty());
			m_pShowFullWeek->SetVisible(!m_pShowFullWeek->GetText().IsEmpty());
			m_pShowMcWeek->SetVisible(m_bMulticolumnsEnable && !m_pShowMcWeek->GetText().IsEmpty());
			m_pShowMcWeek->SetCheck(GetCalendarCtrl()->m_bMultiColumnWeekMode);
		}
		if (nViewType == xtpCalendarWorkWeekView)
		{
			m_pShowWorkWeek->SetCheck(1);
			m_pShowFullWeek->SetCheck(0);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarFullWeekView)
		{
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarWeekView)
		{
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarTimeLineView)
		{
			m_pShowTimeline->SetCheck(1);
		}
	}
}

void CXTPCalendarCaptionBarTheme::OnPartClick(int nPartID)
{
	BOOL bCancel = FALSE;
	if (m_pConnection)
		m_pConnection->SendEvent(XTP_NC_CALENDAR_CAPTIONBARPART_BEFORE_CLICK, (WPARAM)nPartID,
								 (LPARAM)&bCancel);

	if (GetCalendarCtrl() && !bCancel)
		GetCalendarCtrl()->SendNotification(XTP_NC_CALENDAR_CAPTIONBARPART_BEFORE_CLICK,
											(WPARAM)nPartID, (LPARAM)&bCancel);

	if (bCancel)
		return;

	if (nPartID == xtpCalendarCaptionBar_ButtonExpand)
	{
		m_pExpandButton->SetCheck(m_pExpandButton->GetCheck() ? 0 : 1);

		Redraw();
	}

	if (GetCalendarCtrl() && GetCalendarCtrl()->GetActiveView())
	{
		BOOL bUpdate	   = TRUE;
		BOOL bTimelineMode = m_pShowTimeline->GetCheck();

		if (nPartID == xtpCalendarCaptionBar_ButtonViewDay)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarDayView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ButtonViewWeek)
		{
			BOOL bMultiColumnWeekMode = GetCalendarCtrl()->m_bMultiColumnWeekMode;
			if (bMultiColumnWeekMode)
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
			else
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ButtonViewMonth)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarMonthView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowWorkWeek)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
			m_pShowWorkWeek->SetCheck(1);
			m_pShowFullWeek->SetCheck(0);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowFullWeek)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowTimeline)
		{
			m_pShowTimeline->SetCheck(!bTimelineMode);
			GetCalendarCtrl()->OnCalendarTimeline();
		}
		else if (nPartID == xtpCalendarCaptionBar_MultiColumnsWeek)
		{
			GetCalendarCtrl()->m_bMultiColumnWeekMode = !GetCalendarCtrl()->m_bMultiColumnWeekMode;
			int nViewType = GetCalendarCtrl()->GetActiveView()->GetViewType();
			if (nViewType == xtpCalendarWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
				m_pShowWorkWeek->SetCheck(0);
				m_pShowFullWeek->SetCheck(1);
			}
			else if (nViewType == xtpCalendarWorkWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
				m_pShowWorkWeek->SetCheck(1);
				m_pShowFullWeek->SetCheck(0);
			}
			else if (nViewType == xtpCalendarFullWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
				m_pShowWorkWeek->SetCheck(0);
				m_pShowFullWeek->SetCheck(1);
			}
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ScrollPrevDate
				 || nPartID == xtpCalendarCaptionBar_ScrollNextDate)
		{
			BOOL bNext = nPartID == xtpCalendarCaptionBar_ScrollNextDate;
			COleDateTime dtSelStart, dtSelEnd, dtFirst;
			CXTPCalendarView* pActiveView = GetCalendarCtrl()->GetActiveView();
			int nViewType				  = pActiveView->GetViewType();

			if (nViewType == xtpCalendarTimeLineView)
			{
				CXTPCalendarTimeLineView* pTimeLineView = (CXTPCalendarTimeLineView*)pActiveView;
				int iTimeScaleID						= pTimeLineView->GetTimeScaleID();
				dtSelStart								= pTimeLineView->GetStartViewDate();
				if (iTimeScaleID == xtpTSPID_Month)
				{
					int nSelDay = dtSelStart.GetDay();
					CXTPCalendarUtils::ShiftDate_Month(dtSelStart, bNext ? 1 : -1, 1);
					CXTPCalendarUtils::UpdateMonthDay(dtSelStart, nSelDay);
				}
				else if (iTimeScaleID == xtpTSPID_Week)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 7 : -7);
				}
				else if (iTimeScaleID == xtpTSPID_Day)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 1 : -1);
				}
				pTimeLineView->SetStartViewDate(dtSelStart);
				SCROLLINFO SI = { 0 };
				int nScrollStep;
				if (pTimeLineView->GetScrollBarInfoH(&SI, &nScrollStep))
				{
					int newPos = (bNext ? XTPToIntChecked(SI.nPos + SI.nPage)
										: XTPToIntChecked(SI.nPage < XTPToUIntChecked(SI.nPos)
															  ? SI.nPos - SI.nPage
															  : 1));
					pTimeLineView->ScrollH(newPos, newPos);
				}
			}
			else
			{
				int nGroupIndex = 0;
				BOOL bAllDay	= TRUE;
				dtFirst			= pActiveView->GetViewDayDate(0);
				if (!pActiveView->GetSelection(&dtSelStart, &dtSelEnd, &bAllDay, &nGroupIndex))
				{
					dtSelStart = dtFirst;
					dtSelEnd   = dtFirst + COleDateTimeSpan(1);
				}

				if (nViewType == xtpCalendarDayView)
				{
					int nDays = pActiveView->GetViewDayCount();
					dtSelStart += COleDateTimeSpan(bNext ? nDays : -nDays);
					dtSelEnd += COleDateTimeSpan(bNext ? nDays : -nDays);
				}
				else if (nViewType == xtpCalendarWorkWeekView
						 || nViewType == xtpCalendarFullWeekView
						 || nViewType == xtpCalendarWeekView)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 7 : -7, 0, 0, 0);
					dtSelEnd = dtSelStart;
				}
				else if (nViewType == xtpCalendarMonthView)
				{
					int nSelDay = dtSelStart.GetDay();
					CXTPCalendarUtils::ShiftDate_Month(dtSelStart, bNext ? 1 : -1, 1);
					CXTPCalendarUtils::UpdateMonthDay(dtSelStart, nSelDay);

					dtSelEnd = dtSelStart;

					CXTPCalendarMonthView* pMView = (CXTPCalendarMonthView*)pActiveView;
					pMView->SetMDayViewLightColorBaseDate(dtSelStart);
				}
				else
				{
					ASSERT(FALSE);
				}

				pActiveView->UnselectAllEvents();
				pActiveView->SetSelection(dtSelStart, dtSelEnd, bAllDay, nGroupIndex);

				GetCalendarCtrl()->SwitchActiveView((XTPCalendarViewType)nViewType);
			}
		}
		else
		{
			bUpdate = FALSE;
		}

		if (bUpdate)
		{
			m_pCaptionBar->AdjustLayout();
			Redraw();
		}
	}

	if (m_pConnection)
		m_pConnection->SendEvent(XTP_NC_CALENDAR_CAPTIONBARPART_CLICK, (WPARAM)nPartID, 0);

	if (GetCalendarCtrl())
		GetCalendarCtrl()->SendNotification(XTP_NC_CALENDAR_CAPTIONBARPART_CLICK, (WPARAM)nPartID,
											0);
}

BOOL CXTPCalendarCaptionBarTheme::_UpdateControlsHotState(BOOL* pbMouseLeave)
{
	if (pbMouseLeave)
		*pbMouseLeave = FALSE;

	CPoint ptMouse(0, 0);

	if (GetCursorPos(&ptMouse) && GetOwnerWnd())
		GetOwnerWnd()->ScreenToClient(&ptMouse);
	else
		return FALSE;

	BOOL bMouseLeave = !m_rcRect.PtInRect(ptMouse);

	if (pbMouseLeave)
		*pbMouseLeave = bMouseLeave;

	if (!bMouseLeave)
		return FALSE;

	BOOL bResetFocused = CWnd::GetFocus() != GetOwnerWnd();
	BOOL bChanged	  = FALSE;

	int nCount = m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
		{
			int nNewState = pPart->GetState() & (~(xtpBtnStateMouseOver | xtpBtnStatePressed));
			if (bResetFocused)
				nNewState = nNewState & (~xtpBtnStateFocused);

			if (nNewState != pPart->GetState())
			{
				pPart->SetState(nNewState);
				bChanged = TRUE;
			}
		}
	}

	return bChanged;
}

void CXTPCalendarCaptionBarTheme::OnKillFocus(CWnd* pNewWnd)
{
	UNREFERENCED_PARAMETER(pNewWnd);

	BOOL bChanged = _UpdateControlsHotState();

	if (bChanged)
		Redraw();
}

CWnd* CXTPCalendarCaptionBarTheme::GetOwnerWnd() const
{
	return m_pCaptionBar;
}

XTPCalendarTheme CXTPCalendarCaptionBarTheme::GetPaintTheme() const
{
	if (m_pPaintManager)
	{
		return m_pPaintManager->GetPaintTheme();
	}

	return xtpCalendarThemeUnknown;
}

void CXTPCalendarCaptionBarTheme::SetExpandButtonCheck(BOOL bCheck)
{
	m_pExpandButton->SetCheck(bCheck ? 1 : 0);
}

BOOL CXTPCalendarCaptionBarTheme::GetExpandButtonCheck()
{
	return (m_pExpandButton->GetCheck() > 0 ? TRUE : FALSE);
}
