// XTPCalendarResource.cpp: implementation of the CXTPCalendarResource class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPSystemHelpers.h"

#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarUtils.h"
#include "Calendar/XTPCalendarNotifications.h"
#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEvent.h"
#include "Calendar/XTPCalendarEvents.h"
#include "Calendar/XTPCalendarData.h"
#include "Calendar/XTPCalendarCustomProperties.h"
#include "Calendar/XTPCalendarResource.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_SCHEDULES_DATA_VER 2

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CXTPCalendarResource, CXTPCmdTarget)
IMPLEMENT_DYNCREATE(CXTPCalendarResources, CXTPCmdTarget)

IMPLEMENT_DYNAMIC(CXTPCalendarSchedule, CXTPCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarSchedules, CXTPCmdTarget)

CXTPCalendarSchedule::CXTPCalendarSchedule(UINT uScheduleID, LPCTSTR pcszScheduleName)
{
	m_uScheduleID = uScheduleID;

	ASSERT(pcszScheduleName);
	if (pcszScheduleName)
	{
		m_strScheduleName = pcszScheduleName;
	}

	m_pCustomProperties = new CXTPCalendarCustomProperties();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarSchedule::~CXTPCalendarSchedule(){ CMDTARGET_RELEASE(m_pCustomProperties) }

///////////////////////////////////////////////////////////////////////////////
CXTPCalendarSchedules::CXTPCalendarSchedules()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarSchedules::~CXTPCalendarSchedules()
{
}

BOOL CXTPCalendarSchedules::AddNewSchedule(LPCTSTR pcszScheduleName)
{
	ASSERT(pcszScheduleName);
	if (!pcszScheduleName)
	{
		return FALSE;
	}

	UINT uNewID					  = _GetNextID();
	CXTPCalendarSchedule* pNewSch = new CXTPCalendarSchedule(uNewID, pcszScheduleName);
	if (pNewSch)
	{
		Add(pNewSch, FALSE);
	}

	return pNewSch != NULL;
}

BOOL CXTPCalendarSchedules::RemoveSchedule(UINT uScheduleID)
{
	int nFIdx = FindIndex(uScheduleID);

	if (nFIdx >= 0)
	{
		RemoveAt(nFIdx);
	}
	return nFIdx >= 0;
}

LPCTSTR CXTPCalendarSchedules::GetScheduleName(UINT uScheduleID) const
{
	int nFIdx = FindIndex(uScheduleID);

	if (nFIdx >= 0)
	{
		CXTPCalendarSchedule* pSch = GetAt(nFIdx, FALSE);
		if (pSch)
		{
			return pSch->GetName();
		}
	}
	return NULL;
}

void CXTPCalendarSchedules::SetScheduleName(UINT uScheduleID, LPCTSTR pcszNewName)
{
	int nFIdx = FindIndex(uScheduleID);

	if (nFIdx >= 0)
	{
		CXTPCalendarSchedule* pSch = GetAt(nFIdx, FALSE);
		if (pSch)
		{
			pSch->SetName(pcszNewName);
		}
	}
}

int CXTPCalendarSchedules::FindIndex(UINT uScheduleID) const
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarSchedule* pSch = GetAt(i, FALSE);
		if (pSch && pSch->GetID() == uScheduleID)
		{
			return i;
		}
	}
	return -1;
}

UINT CXTPCalendarSchedules::_GetNextID() const
{
	UINT uMaxID = 0;

	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarSchedule* pSch = GetAt(i, FALSE);
		if (pSch && uMaxID < pSch->GetID())
		{
			uMaxID = pSch->GetID();
		}
	}
	return uMaxID + 1;
}

void CXTPCalendarSchedules::DoPropExchange(CXTPPropExchange* pPX)
{
	if (!pPX)
	{
		ASSERT(FALSE);
		return;
	}

	if (pPX->IsLoading())
	{
		_Load(pPX);
	}
	else
	{
		_Save(pPX);
	}
}

void CXTPCalendarSchedules::_Save(CXTPPropExchange* pPX)
{
	if (!pPX || !pPX->IsStoring())
	{
		ASSERT(FALSE);
		return;
	}

	CMDTARGET_ADDREF(pPX);
	CXTPPropExchangeSection secSchedules(pPX); //->GetSection(_T("EventsSchedules")));
	secSchedules->EmptySection();

	long nVersion = XTP_SCHEDULES_DATA_VER;
	PX_Long(&secSchedules, _T("Version"), nVersion, XTP_SCHEDULES_DATA_VER);

	int nCount = GetCount();
	CXTPPropExchangeEnumeratorPtr pEnumerator(secSchedules->GetEnumerator(_T("Schedule")));
	POSITION posStorage = pEnumerator->GetPosition(XTPToDWORDChecked(nCount));

	int nSavedCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarSchedule* pSch = GetAt(i, FALSE);
		ASSERT(pSch);
		if (!pSch)
		{
			continue;
		}
		ULONG ulID		= pSch->GetID();
		CString strName = pSch->GetName();

		CXTPPropExchangeSection secSch(pEnumerator->GetNext(posStorage));
		PX_ULong(&secSch, _T("ID"), ulID);
		PX_String(&secSch, _T("Name"), strName);

		if (pSch->GetCustomProperties())
			pSch->GetCustomProperties()->DoPropExchange(&secSch);

		nSavedCount++;
	}
	ASSERT(nSavedCount == nCount);
}

void CXTPCalendarSchedules::_Load(CXTPPropExchange* pPX)
{
	if (!pPX || !pPX->IsLoading())
	{
		ASSERT(FALSE);
		return;
	}
	RemoveAll();

	CMDTARGET_ADDREF(pPX);
	CXTPPropExchangeSection secSchedules(pPX); //->GetSection(_T("EventsSchedules")));

	long nVersion = 0;
	PX_Long(&secSchedules, _T("Version"), nVersion, XTP_SCHEDULES_DATA_VER);

	if (nVersion != XTP_SCHEDULES_DATA_VER)
	{
		TRACE(_T("ERROR! XTPCalendarSchedules: Unsupported data file version. (%d) \n"), nVersion);
		return;
	}

	CXTPPropExchangeEnumeratorPtr pEnumerator(secSchedules->GetEnumerator(_T("Schedule")));
	POSITION posStorage = pEnumerator->GetPosition();

	while (posStorage)
	{
		ULONG ulID;
		CString strName;

		CXTPPropExchangeSection secSch(pEnumerator->GetNext(posStorage));
		PX_ULong(&secSch, _T("ID"), ulID, 0);
		PX_String(&secSch, _T("Name"), strName);

		CXTPCalendarSchedule* pNewSch = new CXTPCalendarSchedule(ulID, strName);
		if (!pNewSch)
		{
			return;
		}

		if (nVersion >= 2)
		{
			XTP_SAFE_CALL2(pNewSch, GetCustomProperties(), DoPropExchange(&secSch));
		}

		Add(pNewSch, FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////
CXTPCalendarResource::CXTPCalendarResource(CXTPCalendarControl* pCalendarCtrl)
	: m_pCalendarCtrl(pCalendarCtrl)
{
	m_pDataProvider					= NULL;
	m_bCloseDataProviderWhenDestroy = TRUE;
	m_Color							= DWORD(-1);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarResource::~CXTPCalendarResource()
{
	if (m_pDataProvider)
	{
		if (m_bCloseDataProviderWhenDestroy && m_pDataProvider->IsOpen())
		{
			m_pDataProvider->Close();
		}
		CMDTARGET_RELEASE(m_pDataProvider);
	}
}

void CXTPCalendarResource::SetDataProvider(CXTPCalendarData* pDataProvider,
										   BOOL bCloseDataProviderWhenDestroy)
{
	CMDTARGET_ADDREF(pDataProvider);
	CMDTARGET_RELEASE(m_pDataProvider);

	m_pDataProvider					= pDataProvider;
	m_bCloseDataProviderWhenDestroy = bCloseDataProviderWhenDestroy;
}

CXTPCalendarEventsPtr CXTPCalendarResource::RetrieveDayEvents(COleDateTime dtDay)
{
	// get current day date
	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	//
	// retrieve all events for this day from the corresponding data provider
	//
	CXTPCalendarEventsPtr ptrEvents = (m_pDataProvider
										   ? m_pDataProvider->RetrieveDayEvents(dtDay).Detach()
										   : new CXTPCalendarEvents());

	FilterEventsByScheduleID(ptrEvents);

	return ptrEvents;
}

BOOL CXTPCalendarResource::ExistsScheduleID(UINT uScheduleID, BOOL bReturnTrueIfSchedulesSetEmpty)
{
	const int nScheduleIDCount = (int)m_arScheduleIDs.GetSize();

	if (nScheduleIDCount == 0)
	{
		return bReturnTrueIfSchedulesSetEmpty;
	}

	// iterate all ScheduleID's
	for (int i = 0; i < nScheduleIDCount; i++)
	{
		UINT uScheduleID_I = m_arScheduleIDs.GetAt(i);

		if (uScheduleID_I == uScheduleID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPCalendarResource::FilterEventsByScheduleID(CXTPCalendarEvents* pEvents)
{
	// filter all events by ScheduleID collection
	//
	const int nScheduleIDCount = (int)m_arScheduleIDs.GetSize();

	// do not filter by schedule ID if no schedules set
	if (nScheduleIDCount && pEvents)
	{
		const int nEventCount = pEvents->GetCount();
		for (int nEvent = nEventCount - 1; nEvent >= 0; nEvent--)
		{
			BOOL bRemove = TRUE;

			// get next event from the collection
			CXTPCalendarEvent* pEvent = pEvents->GetAt(nEvent, FALSE);
			ASSERT(pEvent);

			if (pEvent)
			{
				UINT uScheduleIDEvent = pEvent->GetScheduleID();
				bRemove				  = !ExistsScheduleID(uScheduleIDEvent, TRUE);
			}

			if (bRemove)
			{
				pEvents->RemoveAt(nEvent);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
// Resources collection
//
//

CXTPCalendarResources::CXTPCalendarResources()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarResources::~CXTPCalendarResources()
{
}

CXTPCalendarResource* CXTPCalendarResources::Find(CXTPCalendarEvent* pEvent) const
{
	if (!pEvent || !pEvent->GetDataProvider())
		return NULL;

	CXTPCalendarData* pDPEvent = pEvent->GetDataProvider();
	UINT uScheduleIDEvent	  = pEvent->GetScheduleID();

	int nRCCount = GetCount();
	for (int i = 0; i < nRCCount; i++)
	{
		CXTPCalendarResource* pRC = GetAt(i, FALSE);
		ASSERT(pRC);
		if (pRC && pRC->GetDataProvider() == pDPEvent)
		{
			if (pRC->ExistsScheduleID(uScheduleIDEvent, TRUE))
				return pRC;
		}
	}
	return NULL;
}

CXTPCalendarEventsPtr CXTPCalendarResources::RetrieveDayEvents(COleDateTime dtDay) const
{
	CXTPCalendarEventsPtr ptrEvents;

	int nRCCount = GetCount();
	for (int i = 0; i < nRCCount; i++)
	{
		CXTPCalendarResource* pRC = GetAt(i, FALSE);
		ASSERT(pRC);
		if (pRC)
		{
			CXTPCalendarEventsPtr ptrEvents2;
			ptrEvents2 = pRC->RetrieveDayEvents(dtDay);
			if (!ptrEvents)
				ptrEvents = ptrEvents2;
			else
				ptrEvents->Append(ptrEvents2);
		}
	}

	return ptrEvents;
}

/////////////////////////////////////////////////////////////////////////////
CXTPCalendarResourcesNf::CXTPCalendarResourcesNf()
{
	m_pConnection = new CXTPNotifyConnection_internal();
}

CXTPCalendarResourcesNf::~CXTPCalendarResourcesNf()
{
	CXTPNotifySinkBase::UnadviseAll();
	CMDTARGET_RELEASE(m_pConnection);
}

void CXTPCalendarResourcesNf::ReBuildInternalData()
{
	UnadviseAll();

	// rebuild optimized data
	m_arResourcesGroupedByDP.RemoveAll();

	int nRCCount = GetCount();
	for (int nRCNr = 0; nRCNr < nRCCount; nRCNr++)
	{
		CXTPCalendarResource* pRCorig = GetAt(nRCNr, FALSE);

		if (!pRCorig || !pRCorig->GetDataProvider() || !pRCorig->GetSchedules())
		{
			ASSERT(FALSE);
			continue;
		}
		CXTPCalendarResource* pRCuniq = FindByDataProvider(&m_arResourcesGroupedByDP,
														   pRCorig->GetDataProvider());

		if (!pRCuniq)
		{
			pRCuniq = new CXTPCalendarResource();
			if (!pRCuniq)
			{
				return;
			}
			pRCuniq->SetDataProvider(pRCorig->GetDataProvider(), FALSE);

			m_arResourcesGroupedByDP.Add(pRCuniq, FALSE);
		}

		int nSchCount = (int)pRCorig->GetSchedules()->GetSize();
		for (int j = 0; j < nSchCount; j++)
		{
			UINT nSchID = pRCorig->GetSchedules()->GetAt(j);
			if (!pRCuniq->ExistsScheduleID(nSchID, FALSE))
			{
				pRCuniq->GetSchedules()->Add(nSchID);
			}
		}
	}

	//-------------------------
	if (!m_pConnection)
	{
		return;
	}
	// get notify IDs ---------
	CMap<XTP_NOTIFY_CODE, XTP_NOTIFY_CODE, int, int> mapNfIDs;
	int nConnCount = (int)m_pConnection->m_arrConnections.GetSize();
	for (int nConnNr = 0; nConnNr < nConnCount; nConnNr++)
	{
		CXTPNotifyConnection_internal::CONNECTION_DESCRIPTOR* pConnDesc =
			m_pConnection->m_arrConnections[nConnNr];
		ASSERT(pConnDesc);
		if (!pConnDesc)
		{
			continue;
		}
		mapNfIDs[pConnDesc->dwNotifyCode] = 1;
	}

	// advice
	POSITION posNfID = mapNfIDs.GetStartPosition();
	while (posNfID)
	{
		XTP_NOTIFY_CODE nfCode = 0;
		int nTmp;
		mapNfIDs.GetNextAssoc(posNfID, nfCode, nTmp);

		int nDPCount = m_arResourcesGroupedByDP.GetCount();
		for (int i = 0; i < nDPCount; i++)
		{
			CXTPNotifyConnection* pDPconn = XTP_SAFE_GET2(m_arResourcesGroupedByDP.GetAt(i),
														  GetDataProvider(), GetConnection(), NULL);
			ASSERT(pDPconn);

			if (pDPconn)
			{
				CXTPNotifySinkBase::Advise(pDPconn, nfCode);
			}
		}
	}
}

CXTPCalendarResource* CXTPCalendarResourcesNf::FindByDataProvider(
	const CXTPCalendarResources* pResources, CXTPCalendarData* pData)
{
	if (!pResources || !pData)
	{
		ASSERT(FALSE);
		return NULL;
	}

	int nCount = pResources->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarResource* pRC = pResources->GetAt(i);
		ASSERT(pRC);
		if (pRC && pRC->GetDataProvider() == pData)
		{
			return pRC;
		}
	}
	return NULL;
}

void CXTPCalendarResourcesNf::OnEvent(XTP_NOTIFY_CODE dwNotifyCode, WPARAM wParam, LPARAM lParam,
									  DWORD dwFlags)

{
	if (dwNotifyCode == XTP_NC_CALENDAREVENTWASADDED
		|| dwNotifyCode == XTP_NC_CALENDAREVENTWASDELETED
		|| dwNotifyCode == XTP_NC_CALENDAREVENTWASCHANGED)
	{
		CXTPCalendarEvent* pEvent = (CXTPCalendarEvent*)lParam;
		ASSERT(pEvent);

		if (!pEvent)
		{
			return;
		}
		CXTPCalendarResource* pRC = FindByDataProvider(&m_arResourcesGroupedByDP,
													   pEvent->GetDataProvider());

		UINT uSchID = pEvent->GetScheduleID();
		if (!pRC || !pRC->ExistsScheduleID(uSchID, TRUE))
		{
			return;
		}
	}

	ASSERT(GetConnection());

	if (GetConnection())
	{
		GetConnection()->SendEvent(dwNotifyCode, wParam, lParam, dwFlags);
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarSchedule, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarSchedule, "ID", 1, m_uScheduleID, VT_I4)
	DISP_PROPERTY_ID(CXTPCalendarSchedule, "Name", 2, m_strScheduleName, VT_BSTR)
	DISP_FUNCTION_ID(CXTPCalendarSchedule, "CustomProperties", 10, OleCustomProperties, VT_DISPATCH,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarSchedule, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarSchedule, XTPDIID_CalendarSchedule, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarSchedule, XTPDIID_CalendarSchedule)

LPDISPATCH CXTPCalendarSchedule::OleCustomProperties()
{
	return m_pCustomProperties ? m_pCustomProperties->GetIDispatch(TRUE) : NULL;
}

//===========================================================================
BEGIN_DISPATCH_MAP(CXTPCalendarSchedules, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarSchedules, "Count", 100, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarSchedules, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarSchedules, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarSchedules, "AddNewSchedule", 10, AddNewSchedule, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPCalendarSchedules, "RemoveSchedule", 11, RemoveSchedule, VT_BOOL, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarSchedules, "GetScheduleName", 12, OleGetScheduleName, VT_BSTR,
					 VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarSchedules, "SetScheduleName", 13, SetScheduleName, VT_EMPTY,
					 VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarSchedules, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarSchedules, XTPDIID_CalendarSchedules, Dispatch)
	// INTERFACE_PART(CXTPCalendarSchedules, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarSchedules, XTPDIID_CalendarSchedules)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarSchedules)

BSTR CXTPCalendarSchedules::OleGetScheduleName(UINT uScheduleID)
{
	CString strScheduleName = GetScheduleName(uScheduleID);
	return strScheduleName.AllocSysString();
}

/////////////////////////////////////////////////////////////////////////////
class CXTPCalendarScheduleIDs : public CXTPCmdTarget
{
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DYNAMIC(CXTPCalendarScheduleIDs)
	//}}AFX_CODEJOCK_PRIVATE
public:
	CXTPCalendarScheduleIDs(CUIntArray* pSrcArray, CCmdTarget* pDataOwner);
	virtual ~CXTPCalendarScheduleIDs();

protected:
	CUIntArray* m_pSrcArray;
	CCmdTarget* m_pDataOwner;

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCalendarScheduleIDs);
	DECLARE_ENUM_VARIANTLIST(CXTPCalendarScheduleIDs);

	long OleGetItem(long nIndex);
	long OleGetItemCount();

	void OleAdd(long nScheduleID);
	void OleRemove(long nIndex);
	void OleRemoveAll();
	//}}AFX_CODEJOCK_PRIVATE
};
//---------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CXTPCalendarScheduleIDs, CXTPCmdTarget)

CXTPCalendarScheduleIDs::CXTPCalendarScheduleIDs(CUIntArray* pSrcArray, CCmdTarget* pDataOwner)
{
	ASSERT(pSrcArray);
	m_pSrcArray = pSrcArray;

	m_pDataOwner = pDataOwner;
	CMDTARGET_ADDREF(m_pDataOwner);

	EnableAutomation();
	EnableTypeLib();
}

CXTPCalendarScheduleIDs::~CXTPCalendarScheduleIDs()
{
	CMDTARGET_RELEASE(m_pDataOwner);
}

//===========================================================================
BEGIN_DISPATCH_MAP(CXTPCalendarScheduleIDs, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "Count", 100, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "Item", DISPID_VALUE, OleGetItem, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "Add", 10, OleAdd, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "Remove", 11, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarScheduleIDs, "RemoveAll", 12, OleRemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarScheduleIDs, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarScheduleIDs, XTPDIID_CalendarScheduleIDs, Dispatch)
	// INTERFACE_PART(CXTPCalendarScheduleIDs, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarScheduleIDs, XTPDIID_CalendarScheduleIDs)

IMPLEMENT_ENUM_VARIANTLIST(CXTPCalendarScheduleIDs)

BOOL CXTPCalendarScheduleIDs::OleGetNextItem(/* [in, out] */ POSITION& rPos,
											 /* [out] */ VARIANT* pVar)
{
	INT_PTR nPos = (INT_PTR)rPos;

	ASSERT(pVar);
	ASSERT(m_pSrcArray);
	if (!pVar || !m_pSrcArray || nPos >= OleGetItemCount())
	{
		return FALSE;
	}

	pVar->vt   = VT_I4;
	pVar->lVal = OleGetItem((long)nPos);

	rPos = (POSITION)(nPos + 1);

	return TRUE;
}

POSITION CXTPCalendarScheduleIDs::OleGetFirstItemPosition()
{
	return 0;
}

long CXTPCalendarScheduleIDs::OleGetItemCount()
{
	ASSERT(m_pSrcArray);
	return m_pSrcArray ? (long)m_pSrcArray->GetSize() : 0;
}

long CXTPCalendarScheduleIDs::OleGetItem(long nIndex)
{
	ASSERT(m_pSrcArray);
	if (!m_pSrcArray)
	{
		return 0;
	}

	if (nIndex < 0 || nIndex >= m_pSrcArray->GetSize())
	{
		AfxThrowOleException(DISP_E_BADINDEX);
	}

	return (long)m_pSrcArray->GetAt((INT_PTR)nIndex);
}

void CXTPCalendarScheduleIDs::OleAdd(long nScheduleID)
{
	ASSERT(m_pSrcArray);
	if (!m_pSrcArray)
	{
		return;
	}
	m_pSrcArray->Add((UINT)nScheduleID);
}

void CXTPCalendarScheduleIDs::OleRemove(long nIndex)
{
	ASSERT(m_pSrcArray);
	if (!m_pSrcArray)
	{
		return;
	}
	m_pSrcArray->RemoveAt((INT_PTR)nIndex);
}

void CXTPCalendarScheduleIDs::OleRemoveAll()
{
	ASSERT(m_pSrcArray);
	if (!m_pSrcArray)
	{
		return;
	}
	m_pSrcArray->RemoveAll();
}

//===========================================================================
BEGIN_DISPATCH_MAP(CXTPCalendarResource, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarResource, "Name", 1, m_strResourceName, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarResource, "Color", 3, m_Color, VT_COLOR)

	DISP_FUNCTION_ID(CXTPCalendarResource, "DataProvider", 10, OleGetDataProvider, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarResource, "ScheduleIDs", 11, OleGetScheduleIDs, VT_DISPATCH,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarResource, "SetDataProvider", 12, OleSetDataProvider, VT_EMPTY,
					 VTS_DISPATCH VTS_BOOL)
	DISP_FUNCTION_ID(CXTPCalendarResource, "SetDataProvider2", 16, OleSetDataProvider2, VT_EMPTY,
					 VTS_BSTR VTS_BOOL)

	DISP_FUNCTION_ID(CXTPCalendarResource, "ExistsScheduleID", 13, ExistsScheduleID, VT_BOOL,
					 VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CXTPCalendarResource, "IsSchedulesSetEmpty", 14, IsSchedulesSetEmpty, VT_BOOL,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarResource, "RetrieveDayEvents", 15, OleRetrieveDayEvents,
					 VT_DISPATCH, VTS_DATE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarResource, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarResource, XTPDIID_ICalendarResource, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarResource, XTPDIID_ICalendarResource)

LPDISPATCH CXTPCalendarResource::OleGetDataProvider()
{
	CXTPCalendarData* pData = GetDataProvider();

	return pData ? pData->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPCalendarResource::OleGetScheduleIDs()
{
	CXTPCalendarScheduleIDs* pSchIDsWrapper;
	pSchIDsWrapper = new CXTPCalendarScheduleIDs(&m_arScheduleIDs, this);

	return pSchIDsWrapper ? pSchIDsWrapper->GetIDispatch(FALSE) : NULL;
}

void CXTPCalendarResource::OleSetDataProvider(LPDISPATCH pDispDataProvider,
											  BOOL bCloseDataProviderWhenDestroy)
{
	ASSERT(pDispDataProvider);
	if (!pDispDataProvider)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	CXTPCalendarData* pData = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPCalendarData,
														   CXTPCalendarData::FromIDispatchSafe(
															   pDispDataProvider));
	ASSERT(pData);
	if (!pData)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	SetDataProvider(pData, bCloseDataProviderWhenDestroy);
}

void CXTPCalendarResource::OleSetDataProvider2(LPCTSTR pcszConnectionString,
											   BOOL bCloseDataProviderWhenDestroy)
{
	CXTPCalendarData* pData = CXTPCalendarData::CreateDataProvider(pcszConnectionString);
	if (!pData)
	{
		AfxThrowOleException(E_FAIL);
	}
	SetDataProvider(pData, bCloseDataProviderWhenDestroy);
	CMDTARGET_RELEASE(pData);
}

LPDISPATCH CXTPCalendarResource::OleRetrieveDayEvents(DATE dtDay)
{
	CXTPCalendarEventsPtr ptrEvents = RetrieveDayEvents(dtDay);
	return ptrEvents ? ptrEvents->GetIDispatch(TRUE) : FALSE;
}

//===========================================================================
BEGIN_DISPATCH_MAP(CXTPCalendarResources, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarResources, "Count", 100, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarResources, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarResources, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarResources, "Add", 10, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarResources, "Remove", 11, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarResources, "RemoveAll", 12, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarResources, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarResources, XTPDIID_ICalendarResources, Dispatch)
	// INTERFACE_PART(CXTPCalendarResources, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarResources, XTPDIID_ICalendarResources)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarResources)

void CXTPCalendarResources::OleAdd(LPDISPATCH pResourceDisp)
{
	CXTPCalendarResource* pResource = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPCalendarResource, CXTPCalendarResource::FromIDispatchSafe(pResourceDisp));
	if (!pResource)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Add(pResource, TRUE);
}

#endif
