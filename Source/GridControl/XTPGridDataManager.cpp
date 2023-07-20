// XTPGridDataManager.cpp : implementation of the CXTPGridDataManager class.
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
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridADO.h"
#include "GridControl/XTPGridDataManager.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRecordItem.h"
#include "ItemTypes/XTPGridRecordItemVariant.h"
#include "GridControl/XTPGridRecord.h"
#include "GridControl/XTPGridRecords.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridColumns.h"
#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CRstEvent

CRstEvent::CRstEvent(CXTPGridDataManager* pDataManager)
{
	m_cRef		   = 0;
	m_pDataManager = pDataManager;
}

CRstEvent::~CRstEvent()
{
	ASSERT(m_adReasonStack.GetSize() == 0);
}

HRESULT __stdcall CRstEvent::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = NULL;
	if (riid == __uuidof(IUnknown) || riid == __uuidof(XTPGRIDADODB::RecordsetEventsVt))
		*ppv = this;
	if (*ppv == NULL)
		return ResultFromScode(E_NOINTERFACE);
	AddRef();
	return NOERROR;
}

ULONG __stdcall CRstEvent::AddRef()
{
	return ++m_cRef;
}

ULONG __stdcall CRstEvent::Release()
{
	if (--m_cRef != 0)
		return m_cRef;
	delete this;
	return 0;
}

HRESULT __stdcall CRstEvent::raw_WillChangeField(long,	// cFields,
												 VARIANT, // Fields,
												 enum XTPGRIDADODB::EventStatusEnum* adStatus,
												 struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FieldChangeComplete(long,						  // cFields,
													 VARIANT,					  // Fields,
													 struct XTPGRIDADODB::Error*, // pError,
													 enum XTPGRIDADODB::EventStatusEnum* adStatus,
													 struct XTPGRIDADODB::_Recordset* // pRecordset
)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillChangeRecord(enum XTPGRIDADODB::EventReasonEnum adReason,
												  long,								   // cRecords,
												  enum XTPGRIDADODB::EventStatusEnum*, // adStatus,
												  struct XTPGRIDADODB::_Recordset* pRecordset)
{
	if (m_pDataManager->m_bLocalUpdate)
		return S_OK;
	switch (adReason)
	{
		case XTPGRIDADODB::adRsnUpdate:
		case XTPGRIDADODB::adRsnResynch: break;
		case XTPGRIDADODB::adRsnDelete:
			m_adReasonStack.Add(adReason);
			m_pDataManager->PrepareDeleteGridRecords(pRecordset);
			break;
		default: m_adReasonStack.Add(adReason); break;
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_RecordChangeComplete(enum XTPGRIDADODB::EventReasonEnum adReason,
													  long, // cRecords,
													  struct XTPGRIDADODB::Error* /*pError*/,
													  enum XTPGRIDADODB::EventStatusEnum* adStatus,
													  struct XTPGRIDADODB::_Recordset* pRecordset)
{
	if (*adStatus == XTPGRIDADODB::adStatusErrorsOccurred)
	{
		if (m_adReasonStack.GetSize() > 0)
			m_adReasonStack.RemoveAt(m_adReasonStack.GetSize() - 1);
		return S_OK;
	}
	if (m_pDataManager->m_bLocalUpdate)
		return S_OK;
	if (adReason == XTPGRIDADODB::adRsnUpdate)
	{
		int adPushedReason = 0;
		if (m_adReasonStack.GetSize() > 0)
		{
			adPushedReason = m_adReasonStack.GetAt(m_adReasonStack.GetSize() - 1);
			m_adReasonStack.RemoveAt(m_adReasonStack.GetSize() - 1);
		}
		switch (adPushedReason)
		{
			case XTPGRIDADODB::adRsnUpdate: m_pDataManager->UpdateGridRecords(pRecordset); break;
			case XTPGRIDADODB::adRsnAddNew: m_pDataManager->AddGridRecords(pRecordset, TRUE); break;
			case XTPGRIDADODB::adRsnDelete: m_pDataManager->DeleteGridRecords(); break;
			case XTPGRIDADODB::adRsnUndoDelete: m_pDataManager->ClearDeleteGridRecords(); break;
		}
	}
	else if (adReason == XTPGRIDADODB::adRsnResynch)
	{
		m_pDataManager->UpdateGridRecords(pRecordset);
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillChangeRecordset(enum XTPGRIDADODB::EventReasonEnum, // adReason,
													 enum XTPGRIDADODB::EventStatusEnum* adStatus,
													 struct XTPGRIDADODB::_Recordset* // pRecordset
)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_RecordsetChangeComplete(
	enum XTPGRIDADODB::EventReasonEnum adReason, struct XTPGRIDADODB::Error* /*pError*/,
	enum XTPGRIDADODB::EventStatusEnum* /*adStatus*/,
	struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	switch (adReason)
	{
		case XTPGRIDADODB::adRsnClose:
			m_pDataManager->ClearGridControl();
			break;
			//      case XTPGRIDADODB::adRsnOpen :
			//          m_pDataManager->AddGridRecords(pRecordset);
			//          break;
	}

	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_WillMove(enum XTPGRIDADODB::EventReasonEnum /*adReason*/,
										  enum XTPGRIDADODB::EventStatusEnum* adStatus,
										  struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_MoveComplete(enum XTPGRIDADODB::EventReasonEnum /*adReason*/,
											  struct XTPGRIDADODB::Error* /*pError*/,
											  enum XTPGRIDADODB::EventStatusEnum* adStatus,
											  struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_EndOfRecordset(VARIANT_BOOL* /*fMoreData*/,
												enum XTPGRIDADODB::EventStatusEnum* adStatus,
												struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FetchProgress(long /*Progress*/, long /*MaxProgress*/,
											   enum XTPGRIDADODB::EventStatusEnum* adStatus,
											   struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

HRESULT __stdcall CRstEvent::raw_FetchComplete(struct XTPGRIDADODB::Error* /*pError*/,
											   enum XTPGRIDADODB::EventStatusEnum* adStatus,
											   struct XTPGRIDADODB::_Recordset* /*pRecordset*/)
{
	*adStatus = XTPGRIDADODB::adStatusUnwantedEvent;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridDataManager

IMPLEMENT_DYNAMIC(CXTPGridDataManager, CXTPCmdTarget);

CXTPGridDataManager::CXTPGridDataManager(CXTPGridControl* pGridControl)
	: m_pGridControl(pGridControl)
	, m_pConnection(NULL)
	, m_pDataSource(NULL)
	, m_pRstEvent(NULL)
	, m_dwRstEvent(0)
	, m_pDeleteRecords(NULL)
	, m_bLocalUpdate(FALSE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridDataManager::~CXTPGridDataManager()
{
	for (int i = 0; i < m_LastComError.GetSize(); i++)
	{
		XTP_GRIDDATAMANAGER_COM_ERROR* pError = m_LastComError.GetAt(i);
		if (pError)
			delete pError;
	}
	m_LastComError.RemoveAll();
	if (m_pDeleteRecords)
	{
		m_pDeleteRecords->RemoveAll();
		delete m_pDeleteRecords;
	}
	RemoveDataSource();
}

void AFX_CDECL CXTPGridDataManager::TRACE_ComError(_com_error& e)
{
	LPCWSTR bstrSource(e.Source());

	// Print COM errors.
	TRACE(_T("COM_ERROR: HRESULT = %08lx, Code meaning = %s, Source = %ls\n"), e.Error(),
		  (LPCTSTR)e.ErrorMessage(), (LPCWSTR)bstrSource);
	// Save last COM error.
	XTP_GRIDDATAMANAGER_COM_ERROR* pError = NULL;
	if (m_LastComError.GetSize() >= XTP_GRIDDATAMANAGER_MAX_LAST_COM_ERRORS)
	{
		pError = m_LastComError.GetAt(m_LastComError.GetSize() - 1);
		m_LastComError.RemoveAt(m_LastComError.GetSize() - 1);
	}
	if (!pError)
		pError = new XTP_GRIDDATAMANAGER_COM_ERROR;
	if (!pError)
		return;
	pError->hResult	= e.Error();
	pError->strMessage = e.ErrorMessage();
	pError->strSource.Format(_T("%ls"), (LPCWSTR)bstrSource);
	m_LastComError.Add(pError);
}

void AFX_CDECL CXTPGridDataManager::TRACE_ProviderError(XTPGRIDADODB::_Connection* pConnDB)
{
	// Print Provider Errors from Connection object.

	ASSERT(pConnDB);
	if (!pConnDB)
		return;

	if (pConnDB->Errors->Count > 0)
	{
		long nCount = pConnDB->Errors->Count;

		TRACE(_T("DB ERROR(s): "));

		for (long i = 0; i < nCount; i++)
		{
			XTPGRIDADODB::ErrorPtr ptrErr = pConnDB->Errors->GetItem(i);
			TRACE(_T(" (Error- %x, '%ls'); "), ptrErr->Number, (LPCWSTR)ptrErr->Description);
		}
		TRACE(_T(" \n"));
	}
}

void CXTPGridDataManager::RemoveDataSource()
{
#ifndef _XTP_ACTIVEX
	try
	{
#endif
		// unadvise events
		if (m_pRstEvent)
		{
			IConnectionPointContainer* pCPC = NULL;
			IConnectionPoint* pCP			= NULL;
			HRESULT hr = m_pDataSource->QueryInterface(__uuidof(IConnectionPointContainer),
													   (void**)&pCPC);
			if (SUCCEEDED(hr))
			{
				hr = pCPC->FindConnectionPoint(__uuidof(XTPGRIDADODB::RecordsetEvents), &pCP);
				pCPC->Release();
				if (SUCCEEDED(hr))
				{
					hr = pCP->Unadvise(m_dwRstEvent);
					pCP->Release();
				}
			}
			// delete m_pRstEvent;
			m_pRstEvent->Release();
			m_pRstEvent = NULL;
		}
		// set data source pointer to NULL
		m_pDataSource = NULL;

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
	}
	catch (...)
	{
		TRACE(_T("Unknown error in RemoveDataSource()\n"));
	}
#endif
}

void CXTPGridDataManager::ClearGridControl()
{
	m_pGridControl->GetColumns()->Clear();
	m_pGridControl->ResetContent();
}

void CXTPGridDataManager::SetDataSource(XTPGRIDADODB::_RecordsetPtr pDataSource)
{
	// remove data source first
	if (m_pDataSource)
		RemoveDataSource();
	// set new data source
	if (pDataSource)
		m_pDataSource = pDataSource;
}

BOOL CXTPGridDataManager::DataBind()
{
	HRESULT hr = E_FAIL;
#ifndef _XTP_ACTIVEX
	try
	{
#endif
		if (m_pDataSource == NULL)
			return FALSE;

		m_pConnection = NULL;

		// get connection
		_variant_t vtConnection;
		if (SUCCEEDED(hr = m_pDataSource->get_ActiveConnection(&vtConnection))
			&& vtConnection.pdispVal)
		{
			m_pConnection = vtConnection;
		}
		// clear Grid control
		ClearGridControl();
		// create columns
		if (FAILED(hr = CreateColumns()))
			return FALSE;
		// create records
		if (FAILED(hr = AddGridRecords(m_pDataSource, FALSE)))
			return FALSE;
		m_pGridControl->Populate();
		// advise events
		m_pRstEvent = new CRstEvent(this);
		if (!m_pRstEvent)
			return FALSE;
		IUnknown* pUnk = NULL;
		hr			   = m_pRstEvent->QueryInterface(__uuidof(IUnknown), (void**)&pUnk);
		if (FAILED(hr))
			return FALSE;
		IConnectionPointContainer* pCPC = NULL;
		IConnectionPoint* pCP			= NULL;
		hr = m_pDataSource->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&pCPC);
		if (FAILED(hr))
			return FALSE;
		hr = pCPC->FindConnectionPoint(__uuidof(XTPGRIDADODB::RecordsetEvents), &pCP);
		pCPC->Release();
		if (FAILED(hr))
			return FALSE;
		hr = pCP->Advise(pUnk, &m_dwRstEvent);
		pCP->Release();
#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in DataBind()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? FALSE : TRUE;
}

HRESULT CXTPGridDataManager::CreateColumns()
{
	HRESULT hr = E_FAIL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		XTPGRIDADODB::FieldsPtr pFields;
		if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
			return hr;
		long lFields;
		if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
			return hr;
		VARIANT vtIdx;
		vtIdx.vt = VT_I4;
		for (int i = 0; i < lFields; i++)
		{
			vtIdx.lVal = i;
			XTPGRIDADODB::FieldPtr pField;
			if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
				break;
			/*          XTPGRIDADODB::DataTypeEnum dteType;
						if (FAILED(hr = pField->get_Type(&dteType)))
							break;
						if (dteType == XTPGRIDADODB::adLongVarBinary)
							continue;*/
			BSTR bstrName;
			pField->get_Name(&bstrName);
			CString strName(bstrName);
			::SysFreeString(bstrName);
			CXTPGridColumn* pColumn = new CXTPGridColumn(i, strName, 100);
			if (!pColumn)
				break;
			long lAttrs;
			if (SUCCEEDED(hr = pField->get_Attributes(&lAttrs))
				&& (lAttrs & XTPGRIDADODB::adFldUpdatable))
				pColumn->SetEditable(TRUE);
			m_pGridControl->AddColumn(pColumn);
		}
#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in CreateColumns()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPGridDataManager::UpdateGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif

		CXTPGridRecords* pRecords = m_pGridControl->GetRecords();
		if (!(pRecordset != NULL && pRecords))
			return E_FAIL;
		XTPGRIDADODB::FieldsPtr pFields;
		if (FAILED(hr = pRecordset->get_Fields(&pFields)) || pFields == NULL)
			return hr;
		long lFields;
		if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
			return hr;
		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// get record bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			CXTPGridRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
			if (pRecord)
			{
				// get field values
				VARIANT vtIdx;
				vtIdx.vt = VT_I4;
				for (int i = 0; i < lFields; i++)
				{
					vtIdx.lVal = i;
					XTPGRIDADODB::FieldPtr pField;
					if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
						break;
					VARIANT vtValue;
					if (FAILED(hr = pField->get_Value(&vtValue)))
						break;
					CXTPGridRecordItemVariant* pItem = (CXTPGridRecordItemVariant*)pRecord->GetItem(
						i);
					if (!pItem)
						break;
					pItem->SetValue(COleVariant(vtValue));
				}
				m_pGridControl->UpdateRecord(pRecord, TRUE);
			}
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}
#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in UpdateGridRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPGridDataManager::AddGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset, BOOL bAddNow)
{
	HRESULT hr = E_FAIL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		if (pRecordset == NULL)
			return E_FAIL;
		XTPGRIDADODB::FieldsPtr pFields;
		if (FAILED(hr = pRecordset->get_Fields(&pFields)) || pFields == NULL)
			return hr;
		long lFields;
		if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
			return hr;

		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// create record
			CXTPGridRecord* pRecord = new CXTPGridRecord;
			if (!pRecord)
				break;
			// get field values
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			for (int i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPGRIDADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				VARIANT vtValue;
				if (FAILED(hr = pField->get_Value(&vtValue)))
					break;
				CXTPGridRecordItemVariant* pItem = new CXTPGridRecordItemVariant(vtValue);
				if (!pItem)
					break;
				pRecord->AddItem(pItem);
			}
			// bind grid record to data source record by bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			pRecord->SetBookmark(vtBookmark);
			// add record to grid records collection
			if (bAddNow)
				m_pGridControl->AddRecordEx(pRecord);
			else
				m_pGridControl->AddRecord(pRecord);
			if (FAILED(hr))
				break;
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}
#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in AddGridRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPGridDataManager::PrepareDeleteGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		if (m_pDeleteRecords)
			m_pDeleteRecords->RemoveAll();
		else
			m_pDeleteRecords = new CXTPGridRecords(TRUE);
		if (!m_pDeleteRecords)
			return E_FAIL;
		CXTPGridRecords* pRecords = m_pGridControl->GetRecords();
		if (!(pRecordset != NULL && pRecords))
			return E_FAIL;
		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// get record bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			CXTPGridRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
			if (pRecord)
				m_pDeleteRecords->Add(pRecord);
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in PrepareDeleteGridRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? hr : S_OK;
}

void CXTPGridDataManager::ClearDeleteGridRecords()
{
	if (m_pDeleteRecords)
		m_pDeleteRecords->RemoveAll();
}

HRESULT CXTPGridDataManager::DeleteGridRecords()
{
	if (!m_pDeleteRecords)
		return E_FAIL;
	for (int i = 0; i < m_pDeleteRecords->GetCount(); i++)
	{
		m_pGridControl->RemoveRecordEx(m_pDeleteRecords->GetAt(i));
	}
	m_pDeleteRecords->RemoveAll();

	return S_OK;
}

HRESULT CXTPGridDataManager::DeleteGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset)
{
	HRESULT hr = E_FAIL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		CXTPGridRecords* pRecords = m_pGridControl->GetRecords();
		if (!(pRecordset != NULL && pRecords))
			return E_FAIL;
		long lRecords;
		if (FAILED(hr = pRecordset->get_RecordCount(&lRecords)))
			return hr;
		if (lRecords <= 0)
			return E_FAIL;
		pRecordset->MoveFirst();
		VARIANT_BOOL vtRecordsEof;
		pRecordset->get_bEOF(&vtRecordsEof);
		while (!vtRecordsEof)
		{
			// get record bookmark
			VARIANT vtBookmark;
			if (FAILED(hr = pRecordset->get_Bookmark(&vtBookmark)))
				break;
			CXTPGridRecord* pRecord = pRecords->FindRecordByBookmark(vtBookmark, TRUE);
			if (pRecord)
				m_pGridControl->RemoveRecordEx(pRecord);
			pRecordset->MoveNext();
			pRecordset->get_bEOF(&vtRecordsEof);
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in DeleteGridRecords()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	return FAILED(hr) ? hr : S_OK;
}

HRESULT CXTPGridDataManager::UpdateField(CXTPGridRecordItemVariant* pItem)
{
	BOOL bOk   = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		while (TRUE)
		{
			if (!pItem)
				break;
			CXTPGridRecord* pRecord = pItem->GetRecord();
			if (!pRecord)
				break;
			// get data source record bookmark
			VARIANT vtBookmark = pRecord->GetBookmark();
			if (vtBookmark.vt == VT_EMPTY)
				break;
			// move to specified record
			if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
				break;
			// update item
			XTPGRIDADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0
				|| pItem->GetIndex() >= lFields)
				break;
			VARIANT vtIdx;
			vtIdx.vt   = VT_I4;
			vtIdx.lVal = pItem->GetIndex();
			XTPGRIDADODB::FieldPtr pField;
			if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
				break;
			long lAttrs;
			if (FAILED(hr = pField->get_Attributes(&lAttrs)))
				break;
			if (!(lAttrs & (XTPGRIDADODB::adFldUpdatable | XTPGRIDADODB::adFldUnknownUpdatable)))
				break;
			XTPGRIDADODB::DataTypeEnum dteType;
			if (FAILED(hr = pField->get_Type(&dteType)))
				break;
			if (dteType == XTPGRIDADODB::adLongVarBinary)
				break;
			VARIANT vtValue = pItem->GetValue();
			if (FAILED(hr = pField->put_Value(vtValue)))
				break;
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			bOk = TRUE;
			break;
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in UpdateField()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPGridDataManager::UpdateRecord(CXTPGridRecord* pRecord,
										  CXTPGridRecordItem* pItemChanged /* = NULL */)
{
	BOOL bOk   = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		while (TRUE)
		{
			if (!pRecord)
				break;
			// get data source record bookmark
			VARIANT vtBookmark = pRecord->GetBookmark();
			if (vtBookmark.vt == VT_EMPTY)
				break;
			// move to specified record
			if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
				break;
			// update record
			XTPGRIDADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPGRIDADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				long lAttrs;
				if (FAILED(hr = pField->get_Attributes(&lAttrs)))
					break;
				if (!(lAttrs
					  & (XTPGRIDADODB::adFldUpdatable | XTPGRIDADODB::adFldUnknownUpdatable)))
					continue;
				XTPGRIDADODB::DataTypeEnum dteType;
				if (FAILED(hr = pField->get_Type(&dteType)))
					break;
				if (dteType == XTPGRIDADODB::adLongVarBinary)
					continue;
				CXTPGridRecordItemVariant* pItem = (CXTPGridRecordItemVariant*)pRecord->GetItem(i);
				if (!pItem)
					break;
				if (pItemChanged != NULL && pItemChanged != pItem)
					continue;

				VARIANT vtValue = pItem->GetValue();
				if (FAILED(hr = pField->put_Value(vtValue)))
					continue; // break;
			}
			if (FAILED(hr) || i < lFields)
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				return hr;
			}
			bOk = TRUE;
			break;
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in UpdateRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPGridDataManager::AddRecord(CXTPGridRecord* pRecord, BOOL bAddToGrid)
{
	BOOL bOk   = FALSE;
	HRESULT hr = E_FAIL;

	m_bLocalUpdate = TRUE;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		while (TRUE)
		{
			if (!pRecord)
				break;
			// add record without field list
			if (FAILED(hr = m_pDataSource->raw_AddNew()))
				break;
			// update fields
			XTPGRIDADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPGRIDADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;
				long lAttrs;
				if (FAILED(hr = pField->get_Attributes(&lAttrs)))
					break;
				if (!(lAttrs
					  & (XTPGRIDADODB::adFldUpdatable | XTPGRIDADODB::adFldUnknownUpdatable)))
					continue;
				CXTPGridRecordItemVariant* pItem = (CXTPGridRecordItemVariant*)pRecord->GetItem(i);
				if (!pItem)
					break;
				VARIANT vtValue = pItem->GetValue();
				// if (FAILED(hr = pField->put_Value(vtValue))) // DEBUG comment
				//    break;
				pField->put_Value(vtValue);
			}
			if (FAILED(hr) || i < lFields)
			{
				m_pDataSource->CancelUpdate();
				break;
			}
			// bind grid record to data source record by bookmark
			VARIANT vtBookmark;
			VariantInit(&vtBookmark);
			if (FAILED(hr = m_pDataSource->get_Bookmark(&vtBookmark)))
				break;
			pRecord->SetBookmark(vtBookmark);
			// update recordset
			if (FAILED(hr = m_pDataSource->Update()))
			{
				m_pDataSource->CancelUpdate();
				pRecord->SetBookmark(COleVariant());
				break;
			}
			// add grid record if required
			if (bAddToGrid)
				m_pGridControl->AddRecordEx(pRecord);
			bOk = TRUE;
			break;
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		pRecord->SetBookmark(COleVariant());
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in AddRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPGridDataManager::CreateEmptyRecord(CXTPGridRecord** ppRecord)
{
	BOOL bOk				= FALSE;
	HRESULT hr				= E_FAIL;
	CXTPGridRecord* pRecord = NULL;

#ifndef _XTP_ACTIVEX
	try
	{
#endif
		while (TRUE)
		{
			if (m_pDataSource == NULL || !ppRecord)
				break;
			// create empty record
			pRecord = new CXTPGridRecord;
			if (!pRecord)
				break;
			XTPGRIDADODB::FieldsPtr pFields;
			if (FAILED(hr = m_pDataSource->get_Fields(&pFields)) || pFields == NULL)
				break;
			long lFields;
			if (FAILED(hr = pFields->get_Count(&lFields)) || lFields == 0)
				break;
			long lRecords;
			if (FAILED(hr = m_pDataSource->get_RecordCount(&lRecords)))
				break;
			if (lRecords > 0)
			{
				m_pDataSource->MoveFirst();
				VARIANT_BOOL vtRecordsEof;
				if (FAILED(hr = m_pDataSource->get_bEOF(&vtRecordsEof)))
					break;
				if (vtRecordsEof)
					break;
			}
			VARIANT vtIdx;
			vtIdx.vt = VT_I4;
			int i;
			for (i = 0; i < lFields; i++)
			{
				vtIdx.lVal = i;
				XTPGRIDADODB::FieldPtr pField;
				if (FAILED(hr = pFields->get_Item(vtIdx, &pField)) || pField == NULL)
					break;

				VARIANT vtValue;
				if (lRecords == 0)
				{
					XTPGRIDADODB::DataTypeEnum dt;
					if (FAILED(hr = pField->get_Type(&dt)))
						break;

					if (dt == XTPGRIDADODB::adInteger || dt == XTPGRIDADODB::adTinyInt
						|| dt == XTPGRIDADODB::adSmallInt || dt == XTPGRIDADODB::adBigInt
						|| dt == XTPGRIDADODB::adUnsignedTinyInt
						|| dt == XTPGRIDADODB::adUnsignedSmallInt
						|| dt == XTPGRIDADODB::adUnsignedInt
						|| dt == XTPGRIDADODB::adUnsignedBigInt)
						vtValue.vt = VT_I4;
					else if (dt == XTPGRIDADODB::adBoolean)
						vtValue.vt = VT_BOOL;
					else if (dt == XTPGRIDADODB::adDate)
						vtValue.vt = VT_DATE;
					else if (dt == XTPGRIDADODB::adDouble)
						vtValue.vt = VT_R8;
					else if (dt == XTPGRIDADODB::adSingle)
						vtValue.vt = VT_R4;
					else if (dt == XTPGRIDADODB::adCurrency)
						vtValue.vt = VT_CY;
					else
						vtValue.vt = VT_BSTR;
				}
				else
				{
					if (FAILED(hr = pField->get_Value(&vtValue)))
						break;
				}

				vtValue.lVal					 = 0;
				CXTPGridRecordItemVariant* pItem = new CXTPGridRecordItemVariant(vtValue);
				if (!pItem)
					break;
				pRecord->AddItem(pItem);
			}
			if (FAILED(hr) || i < lFields)
				break;
			*ppRecord = pRecord;
			bOk		  = TRUE;
			break;
		}

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		if (pRecord)
			delete pRecord;
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		if (pRecord)
			delete pRecord;
		TRACE(_T("Unknown error in CreateEmptyRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif

	if (!bOk && pRecord)
		delete pRecord;

	return FAILED(hr) ? hr : bOk ? S_OK : E_FAIL;
}

HRESULT CXTPGridDataManager::DeleteRecord(CXTPGridRecord* pRecord)
{
	HRESULT hr = E_FAIL;

	// According AddRecord approach
	m_bLocalUpdate = TRUE;
#ifndef _XTP_ACTIVEX
	try
	{
#endif
		if (!pRecord)
		{
			m_bLocalUpdate = FALSE;
			return E_FAIL;
		}
		// get data source record bookmark
		VARIANT vtBookmark = pRecord->GetBookmark();
		if (vtBookmark.vt == VT_EMPTY)
		{
			return E_FAIL;
		}
		// move to specified record
		if (FAILED(hr = m_pDataSource->put_Bookmark(vtBookmark)))
		{
			m_bLocalUpdate = FALSE;
			return hr;
		}
		// delete record
		if (FAILED(hr = m_pDataSource->Delete(XTPGRIDADODB::adAffectCurrent)))
		{
			m_pDataSource->CancelUpdate();
			m_bLocalUpdate = FALSE;
			return hr;
		}
		m_pGridControl->RemoveRecordEx(pRecord);

#ifndef _XTP_ACTIVEX
	}
	catch (_com_error& e)
	{
		TRACE_ProviderError(m_pConnection);
		TRACE_ComError(e);
		m_pDataSource->CancelUpdate();
		hr = FAILED(hr) ? hr : E_FAIL;
	}
	catch (...)
	{
		TRACE(_T("Unknown error in CreateEmptyRecord()\n"));
		hr = FAILED(hr) ? hr : E_FAIL;
	}
#endif
	// According AddRecord approach
	m_bLocalUpdate = FALSE;

	return FAILED(hr) ? hr : S_OK;
}

XTP_GRIDDATAMANAGER_COM_ERROR CXTPGridDataManager::GetLastComError()
{
	XTP_GRIDDATAMANAGER_COM_ERROR Error;

	if (m_LastComError.GetSize() > 0)
	{
		XTP_GRIDDATAMANAGER_COM_ERROR* pError = m_LastComError.GetAt(m_LastComError.GetSize() - 1);
		m_LastComError.RemoveAt(m_LastComError.GetSize() - 1);
		if (pError)
		{
			Error = *pError;
			delete pError;
		}
	}

	return Error;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridDataManager, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPGridDataManager, "DataSource", 1, OleGetDataSource, OleSetDataSource,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridDataManager, "DataBind", 2, DataBind, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridDataManager, "UpdateField", 3, OleUpdateField, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridDataManager, "UpdateRecord", 4, OleUpdateRecord, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridDataManager, "AddRecord", 5, OleAddRecord, VT_BOOL,
					 VTS_DISPATCH VTS_BOOL)
	DISP_FUNCTION_ID(CXTPGridDataManager, "DeleteRecord", 6, OleDeleteRecord, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPGridDataManager, "CreateEmptyRecord", 7, OleCreateEmptyRecord, VT_DISPATCH,
					 VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridDataManager, "UpdateRecordItem", 8, OleUpdateRecordItem, VT_BOOL,
					 VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridDataManager, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridDataManager, XTPDIID_IGridDataManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridDataManager, XTPDIID_IGridDataManager)

CXTPGridDataManager* AFX_CDECL CXTPGridDataManager::FromDispatch(LPDISPATCH pDisp)
{
	CXTPGridDataManager* pInst = NULL;
	if (NULL != pDisp)
	{
		pInst = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPGridDataManager,
											 CXTPGridDataManager::FromIDispatchSafe(pDisp));
		if (NULL == pInst)
		{
			AfxThrowOleException(E_INVALIDARG);
		}
	}

	return pInst;
}

LPDISPATCH CXTPGridDataManager::OleGetDataSource()
{
	//  if (GetDataSource() == NULL)
	return NULL;
	//  return GetDataSource()->GetIDispatch(TRUE);
}

void CXTPGridDataManager::OleSetDataSource(LPDISPATCH lpDispatch)
{
	if (!lpDispatch)
	{
		SetDataSource(NULL);
		return;
	}
	XTPGRIDADODB::_RecordsetPtr pDataSource = NULL;
	HRESULT hr								= lpDispatch->QueryInterface(&pDataSource);
	if (pDataSource != NULL)
		SetDataSource(pDataSource);
}

BOOL CXTPGridDataManager::OleUpdateField(LPDISPATCH lpDispatch)
{
	if (!lpDispatch)
		return FALSE;

	CXTPGridRecordItemVariant* pItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPGridRecordItemVariant, CXTPGridRecordItemVariant::FromIDispatchSafe(lpDispatch));
	if (!pItem)
	{
		return FALSE;
	}

	return SUCCEEDED(UpdateField(pItem));
}

BOOL CXTPGridDataManager::OleUpdateRecord(LPDISPATCH lpDispatch)
{
	if (!lpDispatch)
		return FALSE;
	CXTPGridRecord* pRecord = CXTPGridRecord::FromDispatch(lpDispatch);
	if (!pRecord)
		return FALSE;
	return SUCCEEDED(UpdateRecord(pRecord));
}

BOOL CXTPGridDataManager::OleUpdateRecordItem(LPDISPATCH lpDispatch)
{
	if (!lpDispatch)
		return FALSE;

	CXTPGridRecordItem* pRecordItem = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPGridRecordItem, CXTPGridRecordItem::FromIDispatchSafe(lpDispatch));
	if (!pRecordItem)
	{
		return FALSE;
	}

	return SUCCEEDED(UpdateRecord(pRecordItem->GetRecord(), pRecordItem));
}

BOOL CXTPGridDataManager::OleAddRecord(LPDISPATCH lpDispatch, BOOL bAddToGrid)
{
	if (!lpDispatch)
		return FALSE;
	CXTPGridRecord* pRecord = CXTPGridRecord::FromDispatch(lpDispatch);
	if (!pRecord)
		return FALSE;
	pRecord->InternalAddRef();
	return SUCCEEDED(AddRecord(pRecord, bAddToGrid));
}

BOOL CXTPGridDataManager::OleDeleteRecord(LPDISPATCH lpDispatch)
{
	if (!lpDispatch)
		return FALSE;
	CXTPGridRecord* pRecord = CXTPGridRecord::FromDispatch(lpDispatch);
	if (!pRecord)
		return FALSE;
	return SUCCEEDED(DeleteRecord(pRecord));
}

LPDISPATCH CXTPGridDataManager::OleCreateEmptyRecord()
{
	CXTPGridRecord* pRecord = NULL;
	if (FAILED(CreateEmptyRecord(&pRecord)) || !pRecord)
		return NULL;
	return pRecord->GetIDispatch(FALSE);
}

#endif // #ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////////
// CXTPGridDataManager diagnostics

#ifdef _DEBUG
void CXTPGridDataManager::AssertValid() const
{
	CXTPCmdTarget::AssertValid();

	ASSERT(m_pGridControl);
}

void CXTPGridDataManager::Dump(CDumpContext& dc) const
{
	// The overridden Dump usually calls the Dump
	// function of its base class before printing
	// data members unique to the derived class.

	CXTPCmdTarget::Dump(dc);

	if (m_pConnection)
	{
		try
		{
			dc << _T("Connection String: ") << (LPCTSTR)m_pConnection->GetConnectionString()
			   << _T("\n");
			dc << _T("Connection State: ");
			switch (m_pConnection->GetState())
			{
				case XTPGRIDADODB::adStateClosed: dc << _T("Closed"); break;
				case XTPGRIDADODB::adStateOpen: dc << _T("Open"); break;
				case XTPGRIDADODB::adStateConnecting: dc << _T("Connecting"); break;
				case XTPGRIDADODB::adStateExecuting: dc << _T("Executing"); break;
				case XTPGRIDADODB::adStateFetching: dc << _T("Fetching"); break;
				default: dc << _T("Unknown");
			}
		}
		catch (_com_error& e)
		{
			UNREFERENCED_PARAMETER(e);
		}
		dc << _T("\n");
	}
	if (m_pDataSource)
	{
		try
		{
			dc << _T("Recordset State: ");
			switch (m_pDataSource->GetState())
			{
				case XTPGRIDADODB::adStateClosed: dc << _T("Closed"); break;
				case XTPGRIDADODB::adStateOpen: dc << _T("Open"); break;
				case XTPGRIDADODB::adStateConnecting: dc << _T("Connecting"); break;
				case XTPGRIDADODB::adStateExecuting: dc << _T("Executing"); break;
				case XTPGRIDADODB::adStateFetching: dc << _T("Fetching"); break;
				default: dc << _T("Unknown");
			}
			dc << _T("\n");
			dc << _T("Record Count: ") << (LONG)m_pDataSource->GetRecordCount();
		}
		catch (_com_error& e)
		{
			UNREFERENCED_PARAMETER(e);
		}
		dc << _T("\n");
	}
}

CDumpContext& AFXAPI operator<<(CDumpContext& dc, const CXTPGridDataManager& obj)
{
	obj.Dump(dc);

	return dc;
}
#endif //_DEBUG
