// XTPGridDataManager.h: interface for the CXTPGridDataManager class.
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
#if !defined(__XTPGRIDDATAMANAGER_H__)
#	define __XTPGRIDDATAMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER > 1000
#		pragma once
#	endif // _MSC_VER > 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPGridRecord;
class CXTPGridRecords;
class CXTPGridRecordItemVariant;
class CXTPGridDataManager;
class CXTPGridControl;
class CXTPGridRecordItem;

#	define XTP_GRIDDATAMANAGER_MAX_LAST_COM_ERRORS 3

//===========================================================================
// Summary:
//     Basic set of parameters transferred to item handlers. This structure
//     groups together essential parameters used in item processing
//     activities. It is the parent for all other structures:
//     XTP_GRIDRECORDITEM_DRAWARGS, XTP_GRIDRECORDITEM_CLICKARGS, etc.
// Example:
// <code>
// XTP_GRIDRECORDITEM_ARGS itemArgs(pControl, pRow, pColumn);
// pItem->OnBeginEdit(&itemArgs);
// </code>
// See Also:
//     CXTPGridRecordItem::OnBeginEdit
//===========================================================================
struct XTP_GRIDDATAMANAGER_COM_ERROR
{
	HRESULT hResult;	// The HRESULT error value.
	CString strMessage; // The error message.
	CString strSource;  // The error source.

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridDataManager object.
	//-----------------------------------------------------------------------
	XTP_GRIDDATAMANAGER_COM_ERROR()
	{
		hResult = 0;
		strMessage.Empty();
		strSource.Empty();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded assignment operator.
	//     Copies data values from an existing CXTPGridDataManager object.
	//-----------------------------------------------------------------------
	const XTP_GRIDDATAMANAGER_COM_ERROR& operator=(const XTP_GRIDDATAMANAGER_COM_ERROR& errorSrc)
	{
		this->hResult	= errorSrc.hResult;
		this->strMessage = errorSrc.strMessage;
		this->strSource  = errorSrc.strSource;

		return *this;
	}
};

class _XTP_EXT_CLASS CRstEvent : public XTPGRIDADODB::RecordsetEventsVt
{
private:
	ULONG m_cRef;
	CXTPGridDataManager* m_pDataManager;

public:
	CRstEvent(CXTPGridDataManager* pDataManager);
	virtual ~CRstEvent();

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	virtual HRESULT __stdcall raw_WillChangeField(long cFields, VARIANT Fields,
												  enum XTPGRIDADODB::EventStatusEnum* adStatus,
												  struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FieldChangeComplete(long cFields, VARIANT Fields,
													  struct XTPGRIDADODB::Error* pError,
													  enum XTPGRIDADODB::EventStatusEnum* adStatus,
													  struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillChangeRecord(enum XTPGRIDADODB::EventReasonEnum adReason,
												   long cRecords,
												   enum XTPGRIDADODB::EventStatusEnum* adStatus,
												   struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_RecordChangeComplete(enum XTPGRIDADODB::EventReasonEnum adReason,
													   long cRecords,
													   struct XTPGRIDADODB::Error* pError,
													   enum XTPGRIDADODB::EventStatusEnum* adStatus,
													   struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillChangeRecordset(enum XTPGRIDADODB::EventReasonEnum adReason,
													  enum XTPGRIDADODB::EventStatusEnum* adStatus,
													  struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_RecordsetChangeComplete(
		enum XTPGRIDADODB::EventReasonEnum adReason, struct XTPGRIDADODB::Error* pError,
		enum XTPGRIDADODB::EventStatusEnum* adStatus, struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_WillMove(enum XTPGRIDADODB::EventReasonEnum adReason,
										   enum XTPGRIDADODB::EventStatusEnum* adStatus,
										   struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_MoveComplete(enum XTPGRIDADODB::EventReasonEnum adReason,
											   struct XTPGRIDADODB::Error* pError,
											   enum XTPGRIDADODB::EventStatusEnum* adStatus,
											   struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_EndOfRecordset(VARIANT_BOOL* fMoreData,
												 enum XTPGRIDADODB::EventStatusEnum* adStatus,
												 struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FetchProgress(long Progress, long MaxProgress,
												enum XTPGRIDADODB::EventStatusEnum* adStatus,
												struct XTPGRIDADODB::_Recordset* pRecordset);
	virtual HRESULT __stdcall raw_FetchComplete(struct XTPGRIDADODB::Error* pError,
												enum XTPGRIDADODB::EventStatusEnum* adStatus,
												struct XTPGRIDADODB::_Recordset* pRecordset);

protected:
	CArray<XTPGRIDADODB::EventReasonEnum, XTPGRIDADODB::EventReasonEnum&> m_adReasonStack;
};

//===========================================================================
// Summary:
//     Represents a grid data manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPGridDataManager : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPGridDataManager);

	//{{AFX_CODEJOCK_PRIVATE
	friend class CXTPGridControl;
	friend class CRstEvent;
	void AFX_CDECL TRACE_ComError(_com_error& e);
	void AFX_CDECL TRACE_ProviderError(XTPGRIDADODB::_Connection* pConnDB);
	//}}AFX_CODEJOCK_PRIVATE

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPGridDataManager object.
	// Parameters:
	//     pGridControl - Pointer to a CXTPGridControl object.
	//-----------------------------------------------------------------------
	CXTPGridDataManager(CXTPGridControl* pGridControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPGridDataManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPGridDataManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the data source.
	// Parameter:
	//     pDataSource - Pointer to a custom ADODB object.
	//-----------------------------------------------------------------------
	virtual void SetDataSource(XTPGRIDADODB::_RecordsetPtr pDataSource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the data source.
	// Returns:
	//     A pointer to a custom ADODB object.
	//-----------------------------------------------------------------------
	virtual XTPGRIDADODB::_RecordsetPtr GetDataSource() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Binds the data (i.e. brings the data to the control).
	// Returns:
	//     TRUE if the operation was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DataBind();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the columns based on the fields.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT CreateColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the grid records.
	// Parameter:
	//     pRecordset - Pointer to a custom ADODB record set.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a record to the Grid control.
	// Parameter:
	//     pRecordset - Pointer to a custom ADODB record set
	//     bAddNow    - Add now.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT AddGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset, BOOL bAddNow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs preperation for the deletion of grid records.
	// Parameter:
	//     pRecordset - Pointer to a custom ADODB record set
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT PrepareDeleteGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears deleted records.
	//-----------------------------------------------------------------------
	virtual void ClearDeleteGridRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes the grid records.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteGridRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes the grid records.
	// Parameter:
	//     pRecordset - Pointer to a custom ADODB record set.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteGridRecords(XTPGRIDADODB::_RecordsetPtr pRecordset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates a field.
	// Parameter:
	//     pItem - Pointer to a grid record item variant.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateField(CXTPGridRecordItemVariant* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates a grid record.
	// Parameter:
	//     pRecord - Pointer to a grid record object.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT UpdateRecord(CXTPGridRecord* pRecord, CXTPGridRecordItem* pItemChanged = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a record.
	// Parameter:
	//     pRecord      - Pointer to a grid record object.
	//     bAddToGrid - TRUE if the record should also be added to
	//                    the Grid control, FALSE otherwise.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT AddRecord(CXTPGridRecord* pRecord, BOOL bAddToGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates an empty record.
	// Parameter:
	//     ppRecord - Pointer to a grid record pointer.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT CreateEmptyRecord(CXTPGridRecord** ppRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes a record.
	// Parameter:
	//     pRecord - Pointer to a grid record object.
	// Returns:
	//     S_OK if the operation was successful, otherwise E_FAIL.
	//-----------------------------------------------------------------------
	virtual HRESULT DeleteRecord(CXTPGridRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the last COM error.
	// Returns:
	//     An XTP_GRIDDATAMANAGER_COM_ERROR value specifying the COM error.
	//-----------------------------------------------------------------------
	virtual XTP_GRIDDATAMANAGER_COM_ERROR GetLastComError();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the data source.
	//-----------------------------------------------------------------------
	void RemoveDataSource();
	//-----------------------------------------------------------------------
	// Summary:
	//     Clears the Grid control.
	//-----------------------------------------------------------------------
	void ClearGridControl();

protected:
	CXTPGridControl* m_pGridControl;			// Pointer to the Grid control.
	XTPGRIDADODB::_ConnectionPtr m_pConnection; // The custom ADODB connection point.
	XTPGRIDADODB::_RecordsetPtr m_pDataSource;  // The data source.
	CRstEvent* m_pRstEvent;						// Pointer to the record set event object.
	DWORD m_dwRstEvent;							// The record set event.
	CXTPGridRecords* m_pDeleteRecords;			// The delete records.
	BOOL m_bLocalUpdate;						// Tells whether the update is local only.
	CArray<XTP_GRIDDATAMANAGER_COM_ERROR*, XTP_GRIDDATAMANAGER_COM_ERROR*> m_LastComError;

public:
#	ifdef _XTP_ACTIVEX
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPGridDataManager);

public:
	static CXTPGridDataManager* AFX_CDECL FromDispatch(LPDISPATCH pDisp);
	LPDISPATCH OleGetDataSource();
	void OleSetDataSource(LPDISPATCH lpDispatch);
	BOOL OleUpdateField(LPDISPATCH lpDispatch);
	BOOL OleUpdateRecord(LPDISPATCH lpDispatch);
	BOOL OleUpdateRecordItem(LPDISPATCH lpDispatch);
	BOOL OleAddRecord(LPDISPATCH lpDispatch, BOOL bAddToGrid);
	BOOL OleDeleteRecord(LPDISPATCH lpDispatch);
	LPDISPATCH OleCreateEmptyRecord();

//}}AFX_CODEJOCK_PRIVATE
#	endif /*_XTP_ACTIVEX*/

// CXTPGridDataManager diagnostics
#	ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	friend _XTP_EXT_CLASS CDumpContext& AFXAPI operator<<(CDumpContext& dc,
														  const CXTPGridDataManager& obj);
#	endif

}; // class _XTP_EXT_CLASS CXTPGridDataManager

AFX_INLINE XTPGRIDADODB::_RecordsetPtr CXTPGridDataManager::GetDataSource() const
{
	return m_pDataSource;
}

#	ifdef _DEBUG
_XTP_EXT_CLASS CDumpContext& AFXAPI operator<<(CDumpContext& dc, const CXTPGridDataManager& obj);
#	endif

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPGRIDDATAMANAGER_H__)
