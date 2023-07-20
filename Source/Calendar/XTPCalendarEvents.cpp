// XTPCalendarEvents.cpp: implementation of the CXTPCalendarEvents class.
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
#include "Common/XTPFramework.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPSystemHelpers.h"

#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarEvents.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarEvents, CXTPCmdTarget)

CXTPCalendarEvents::CXTPCalendarEvents()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEvents::~CXTPCalendarEvents()
{
	RemoveAll();
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarEvent* CXTPCalendarEvents::GetAt(int nIndex, BOOL bWithAddRef) const
{
	CXTPCalendarEvent* pEvent = m_arEvents[nIndex];
	if (bWithAddRef && pEvent)
	{
		pEvent->InternalAddRef();
	}
	return pEvent;
}

void CXTPCalendarEvents::SetAt(int nIndex, CXTPCalendarEvent* pEvent, BOOL bWithAddRef)
{
	if (bWithAddRef && pEvent)
	{
		pEvent->InternalAddRef();
	}

	CXTPCalendarEvent* pPrevEvent = m_arEvents[nIndex];
	if (pPrevEvent)
	{
		pPrevEvent->InternalRelease();
	}

	m_arEvents[nIndex] = pEvent;
}

void CXTPCalendarEvents::InsertAt(int nIndex, CXTPCalendarEvent* pEvent, BOOL bWithAddRef)
{
	if (bWithAddRef && pEvent)
	{
		pEvent->InternalAddRef();
	}

	m_arEvents.InsertAt(nIndex, pEvent);
}

void CXTPCalendarEvents::Add(CXTPCalendarEvent* pNewEvent, BOOL bWithAddRef)
{
	if (bWithAddRef && pNewEvent)
	{
		pNewEvent->InternalAddRef();
	}
	m_arEvents.Add(pNewEvent);
}

void CXTPCalendarEvents::Append(CXTPCalendarEvents* pEventsArray)
{
	if (!pEventsArray)
	{
		ASSERT(FALSE);
		return;
	}

	int nCount = pEventsArray->GetCount();
	for (int nEvent = 0; nEvent < nCount; nEvent++)
	{
		CXTPCalendarEvent* pEvent = pEventsArray->GetAt(nEvent, FALSE);
		Add(pEvent, TRUE);
	}
}

void CXTPCalendarEvents::RemoveAt(int nIndex)
{
	CXTPCalendarEvent* pEvent = GetAt(nIndex, FALSE);
	m_arEvents.RemoveAt(nIndex);
	if (pEvent)
	{
		pEvent->InternalRelease();
	}
}

void CXTPCalendarEvents::RemoveAll()
{
	// cleanup
	int nCount = GetCount();
	for (int nEvent = 0; nEvent < nCount; nEvent++)
	{
		CXTPCalendarEvent* pEvent = m_arEvents.GetAt(nEvent);
		if (pEvent)
		{
			pEvent->InternalRelease();
		}
	}

	m_arEvents.RemoveAll();
}

int CXTPCalendarEvents::Find(DWORD dwEventID) const
{
	int nCount = GetCount();
	for (int nEvent = 0; nEvent < nCount; nEvent++)
	{
		CXTPCalendarEvent* pEvent = m_arEvents.GetAt(nEvent);
		DWORD dwID				  = pEvent->GetEventID();
		if (dwID == dwEventID)
		{
			return nEvent;
		}
	}
	return -1;
}

CXTPCalendarEvent* CXTPCalendarEvents::FindEvent(DWORD dwEventID) const
{
	CXTPCalendarEvent* pFEvent = NULL;

	int nFIndex = Find(dwEventID);
	if (nFIndex >= 0)
	{
		pFEvent = GetAt(nFIndex, FALSE);
	}
	return pFEvent;
}

int CXTPCalendarEvents::Find(CXTPCalendarEvent* pFEvent) const
{
	int nCount = GetCount();
	for (int nEvent = 0; nEvent < nCount; nEvent++)
	{
		CXTPCalendarEvent* pEvent = m_arEvents.GetAt(nEvent);
		if (pEvent && pEvent->IsEqualIDs(pFEvent))
		{
			return nEvent;
		}
	}
	return -1;
}

CXTPCalendarEvent* CXTPCalendarEvents::FindEvent(CXTPCalendarEvent* pFEvent) const
{
	CXTPCalendarEvent* pFindedEvent = NULL;

	int nFIndex = Find(pFEvent);
	if (nFIndex >= 0)
	{
		pFindedEvent = GetAt(nFIndex, FALSE);
	}
	return pFindedEvent;
}

void CXTPCalendarEvents::Sort(T_CompareFunc pCompareFunc)
{
	typedef int(AFX_CDECL * GENERICCOMPAREFUNC)(const void*, const void*);

	qsort(m_arEvents.GetData(), (size_t)m_arEvents.GetSize(), sizeof(CXTPCalendarEvent*),
		  (GENERICCOMPAREFUNC)pCompareFunc);
}

int AFX_CDECL CXTPCalendarEvents::CompareEvents_ForView(const CXTPCalendarEvent** ppEv1,
														const CXTPCalendarEvent** ppEv2)
{
	if (*ppEv1 == NULL || *ppEv2 == NULL)
	{
		ASSERT(FALSE);
		return XTPCompare(*ppEv1, *ppEv2);
	}

	int nPeriodDays1 = (*ppEv1)->GetEventPeriodDays();
	int nPeriodDays2 = (*ppEv2)->GetEventPeriodDays();

	int nAllDayEvent1 = (*ppEv1)->IsAllDayEvent() ? 1 : 0;
	int nAllDayEvent2 = (*ppEv2)->IsAllDayEvent() ? 1 : 0;

	BOOL bIsMultidayEvent = (nAllDayEvent1 && nAllDayEvent2
							 || nPeriodDays1 > 1 && nPeriodDays2 > 1);
	int nCmp			  = 0;
	//---------------------------------------------------------------------------
	if (bIsMultidayEvent)
	{
		nCmp = XTPCompare((*ppEv1)->GetStartTime(), (*ppEv2)->GetStartTime());
		if (nCmp)
		{
			// extra criteria <<
			if (nCmp == 0)
				nCmp = XTPCompare((*ppEv1)->GetEndTime(), (*ppEv2)->GetEndTime());
			// extra criteria >>
			return nCmp;
		}
	}
	//---------------------------------------------------------------------------
	nCmp = XTPCompare(nPeriodDays2, nPeriodDays1);
	if (nCmp)
	{
		return nCmp;
	}

	nCmp = XTPCompare(nAllDayEvent2, nAllDayEvent1);
	if (nCmp)
	{
		return nCmp;
	}

	if (!bIsMultidayEvent)
	{
		nCmp = XTPCompare((*ppEv1)->GetStartTime(), (*ppEv2)->GetStartTime());
		if (nCmp)
		{
			return nCmp;
		}
	}

	nCmp = XTPCompare((*ppEv1)->GetDurationMinutes(), (*ppEv2)->GetDurationMinutes());
	if (nCmp)
	{
		return -nCmp;
	}

	//-----------------------------------------------------
	nCmp = XTPCompare((*ppEv1)->GetSubject(), (*ppEv2)->GetSubject());
	if (nCmp)
	{
		return nCmp;
	}

	//-----------------------------------------------------
	nCmp = CompareEvents_ByID(ppEv1, ppEv2);

	return nCmp;
}

int AFX_CDECL CXTPCalendarEvents::CompareEvents_ForViewByStart(const CXTPCalendarEvent** ppEv1,
															   const CXTPCalendarEvent** ppEv2)
{
	if (*ppEv1 == NULL || *ppEv2 == NULL)
	{
		ASSERT(FALSE);
		return XTPCompare(*ppEv1, *ppEv2);
	}

	int nPeriodDays1 = (*ppEv1)->GetEventPeriodDays();
	int nPeriodDays2 = (*ppEv2)->GetEventPeriodDays();

	int nAllDayEvent1 = (*ppEv1)->IsAllDayEvent() ? 1 : 0;
	int nAllDayEvent2 = (*ppEv2)->IsAllDayEvent() ? 1 : 0;

	int nCmp = 0;
	//---------------------------------------------------------------------------
	nCmp = XTPCompare((*ppEv1)->GetStartTime(), (*ppEv2)->GetStartTime());
	if (nCmp)
	{
		return nCmp;
	}
	//---------------------------------------------------------------------------
	nCmp = XTPCompare(nPeriodDays2, nPeriodDays1);
	if (nCmp)
	{
		return nCmp;
	}

	nCmp = XTPCompare(nAllDayEvent2, nAllDayEvent1);
	if (nCmp)
	{
		return nCmp;
	}

	nCmp = XTPCompare((*ppEv1)->GetDurationMinutes(), (*ppEv2)->GetDurationMinutes());
	if (nCmp)
	{
		return -nCmp;
	}

	//-----------------------------------------------------
	nCmp = XTPCompare((*ppEv1)->GetSubject(), (*ppEv2)->GetSubject());
	if (nCmp)
	{
		return nCmp;
	}

	//-----------------------------------------------------
	nCmp = CompareEvents_ByID(ppEv1, ppEv2);

	return nCmp;
}

int AFX_CDECL CXTPCalendarEvents::CompareEvents_ByID(const CXTPCalendarEvent** ppEv1,
													 const CXTPCalendarEvent** ppEv2)
{
	if (*ppEv1 == NULL || *ppEv2 == NULL)
	{
		ASSERT(FALSE);
		return XTPCompare(*ppEv1, *ppEv2);
	}

	//-----------------------------------------------------
	DWORD dwID1 = (*ppEv1)->GetEventID();
	DWORD dwID2 = (*ppEv2)->GetEventID();

	int nCmp = XTPCompare(dwID1, dwID2);

	return nCmp;
}

void CXTPCalendarEvents::CloneEvents()
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvOrig = GetAt(i, FALSE);
		CXTPCalendarEventPtr ptrEvClone;

		if (pEvOrig)
			ptrEvClone = pEvOrig->CloneEvent();

		if (ptrEvClone)
			SetAt(i, ptrEvClone);
		else
			RemoveAt(i);
	}
}
/////////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarEvents, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarEvents, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvents, "Event", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEvents, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEvents, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarEvents, "Remove", 3, OleRemove, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarEvents, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEvents, XTPDIID_CalendarEvents, Dispatch)
	// INTERFACE_PART(CXTPCalendarEvents, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEvents, XTPDIID_CalendarEvents)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarEvents)

int CXTPCalendarEvents::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPCalendarEvents::OleGetItem(long nIndex)
{
	CXTPCalendarEvent* pEvent = GetAt(nIndex);
	if (!pEvent)
		AfxThrowOleException(DISP_E_BADINDEX);

	return pEvent->GetIDispatch(TRUE);
}

void CXTPCalendarEvents::OleAdd(LPDISPATCH pEventDisp)
{
	CXTPCalendarEvent* pEvent = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPCalendarEvent,
															 CXTPCalendarEvent::FromIDispatchSafe(
																 pEventDisp));
	if (!pEvent)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Add(pEvent, TRUE);
}

void CXTPCalendarEvents::OleRemove(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		RemoveAt(nIndex);
	}
	else
	{
		AfxThrowOleException(DISP_E_BADINDEX);
	}
}

#endif
