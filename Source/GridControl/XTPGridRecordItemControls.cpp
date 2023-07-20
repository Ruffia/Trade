// XTPGridRecordItemControls.cpp
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
#include "Common/Tmschema.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridRecordItem.h"
#include "GridControl/XTPGridRecordItemControls.h"
#include "GridControl/XTPGridControlIIDs.h"

#pragma warning(disable : 4097) // TBase' used as synonym for class-name

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemControl

IMPLEMENT_SERIAL(CXTPGridRecordItemControl, CXTPCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CXTPGridRecordItemButton, CXTPGridRecordItemControl,
				 VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

IMPLEMENT_DYNAMIC(CXTPGridRecordItemControls, CXTPCmdTarget)
//////////////////////////////////////////////////////////////////////////

CXTPGridRecordItemControl::CXTPGridRecordItemControl(LPCTSTR szCaption)
	: m_strCaption(szCaption)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_nType  = 0;
	m_nIndex = 0;

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	m_clrCaption  = GetSysColor(COLOR_BTNTEXT);
	m_sizeControl = CSize(0, 0);
	m_rcControl   = CRect(0, 0, 0, 0);
	m_unFlags	 = 0;
	m_Alignment   = xtpItemControlUnknown;
	m_bEnabled	= TRUE;
	m_nState	  = 0;
	m_bThemed	 = FALSE;
}

CXTPGridRecordItemControl::~CXTPGridRecordItemControl()
{
	m_mapIcon.RemoveAll();
}

CFont* CXTPGridRecordItemControl::GetFont()
{
	return &m_xtpFontCaption;
}

void CXTPGridRecordItemControl::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("Type"), m_nType);
	PX_String(pPX, _T("Caption"), m_strCaption);
	PX_ULong(pPX, _T("CaptionColor"), (ULONG&)m_clrCaption);
	PX_DPI_Size(pPX, _T("Size"), m_sizeControl, CSize(-1, -1));
	PX_ULong(pPX, _T("Flags"), (ULONG&)m_unFlags);
	PX_Int(pPX, _T("Alignment"), (int&)m_Alignment);
	PX_Bool(pPX, _T("Enable"), m_bEnabled);
	PX_Bool(pPX, _T("Themed"), m_bThemed);

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("StateIcons")));

	if (pPX->IsStoring())
	{
		DWORD dwCount	= (DWORD)m_mapIcon.GetCount();
		POSITION posItem = pEnumItems->GetPosition(dwCount);

		POSITION posData = m_mapIcon.GetStartPosition();
		while (posData)
		{
			int nKey = 0, nValue = 0;
			m_mapIcon.GetNextAssoc(posData, nKey, nValue);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Int(&secItem, _T("State"), nKey);
			PX_Int(&secItem, _T("Icon"), nValue);
		}
	}
	else
	{
		m_mapIcon.RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			int nKey = 0, nValue = 0;
			PX_Int(&secItem, _T("State"), nKey, 0);
			PX_Int(&secItem, _T("Icon"), nValue, 0);

			m_mapIcon[nKey] = nValue;
		}
	}
}

void CXTPGridRecordItemControl::SetFont(CFont* pFont)
{
	ASSERT_VALID(pFont);

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	m_xtpFontCaption.DeleteObject();
	m_xtpFontCaption.CreateFontIndirect(&lf);
}

int CXTPGridRecordItemControl::GetIconIndex(int nState) const
{
	int nIconIndex = XTP_GRID_NOICON;
	if (m_mapIcon.Lookup(nState, nIconIndex))
		return nIconIndex;
	m_mapIcon.Lookup(0, nIconIndex);
	return nIconIndex;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecordItemControl, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Type", 1, GetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Index", 2, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Caption", 3, OleGetCaption, OleSetCaption,
						VT_BSTR)
	DISP_PROPERTY_ID(CXTPGridRecordItemControl, "CaptionColor", 4, m_clrCaption, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Font", 5, OleGetFont, OleSetFont, VT_FONT)
	DISP_PROPERTY_ID(CXTPGridRecordItemControl, "Alignment", 6, m_Alignment, VT_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemControl, "SetSize", 7, OleSetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Width", 8, GetWidth, SetWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Height", 9, GetHeight, SetHeight, VT_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemControl, "GetRect", 10, OleGetRect, VT_EMPTY,
					 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_PROPERTY_EX_ID(CXTPGridRecordItemControl, "Enable", 11, GetEnable, SetEnable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPGridRecordItemControl, "Themed", 12, m_bThemed, VT_BOOL)
	DISP_FUNCTION_ID(CXTPGridRecordItemControl, "SetIconIndex", 13, SetIconIndex, VT_EMPTY,
					 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemControl, "GetIconIndex", 14, GetIconIndex, VT_I4, VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemControl, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItemControl, XTPDIID_IGridItemControl, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemControl, XTPDIID_IGridItemControl)

BSTR CXTPGridRecordItemControl::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}

void CXTPGridRecordItemControl::OleSetCaption(LPCTSTR pcszCaption)
{
	m_strCaption = pcszCaption;
}

LPFONTDISP CXTPGridRecordItemControl::OleGetFont()
{
	return AxCreateOleFont(GetFont(), this,
						   (LPFNFONTCHANGED)&CXTPGridRecordItemControl::OleSetFont);
}

void CXTPGridRecordItemControl::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_xtpFontCaption.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_xtpFontCaption.CreateFontIndirect(&lf);
	}
}

void CXTPGridRecordItemControl::OleSetSize(long cx, long cy)
{
	m_sizeControl = CSize(cx, cy);
}

void CXTPGridRecordItemControl::OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom)
{
	if (pnLeft)
		*pnLeft = m_rcControl.left;
	if (pnTop)
		*pnTop = m_rcControl.top;
	if (pnRight)
		*pnRight = m_rcControl.right;
	if (pnBottom)
		*pnBottom = m_rcControl.bottom;
}

#endif

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemButton

CXTPGridRecordItemButton::CXTPGridRecordItemButton(LPCTSTR szCaption)
	: CXTPGridRecordItemControl(szCaption)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif

	m_nState = m_nSavedState = PBS_NORMAL;
}

void CXTPGridRecordItemButton::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPGridRecordItemControl::DoPropExchange(pPX);
}

void CXTPGridRecordItemButton::OnLButtonDown(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	UNREFERENCED_PARAMETER(pClickArgs);

	if (GetEnable())
		m_nState = PBS_PRESSED;
}

void CXTPGridRecordItemButton::OnLButtonUp(XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	// UNREFERENCED_PARAMETER(pClickArgs);
	//>>attempt for 23588 issue case
	if (pClickArgs && pClickArgs->pControl && pClickArgs->pControl->IsEditMode())
		return;
	//>>attempt for 23588 issue case

	m_nState = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
	if (m_rcControl.PtInRect(pClickArgs->ptClient) && pClickArgs->pControl
		&& m_rcControl.PtInRect(pClickArgs->pControl->m_mouseDownState.ptMouse))
	{
		XTP_NM_GRIDITEMCONTROL nm;
		::ZeroMemory(&nm, sizeof(nm));
		nm.pItem		= pClickArgs->pItem;
		nm.pRow			= pClickArgs->pRow;
		nm.pColumn		= pClickArgs->pColumn;
		nm.pt			= pClickArgs->ptClient;
		nm.pItemControl = this;
		pClickArgs->pControl->SendNotifyMessage(XTP_NM_GRID_ITEMBUTTONCLICK, (NMHDR*)&nm);
	}
}

void CXTPGridRecordItemButton::OnMouseEnter(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	if (m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}

void CXTPGridRecordItemButton::OnMouseLeave(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);
	m_nSavedState = m_nState;
	m_nState	  = GetEnable() ? PBS_NORMAL : PBS_DISABLED;
}

void CXTPGridRecordItemButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rcControl.PtInRect(point) && m_nSavedState == PBS_PRESSED && (nFlags & MK_LBUTTON))
		m_nState = m_nSavedState;
}

//////////////////////////////////////////////////////////////////////////
// CXTPGridRecordItemControls

CXTPGridRecordItemControls::CXTPGridRecordItemControls()
{
	m_pRecordItem = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecordItemControls::CXTPGridRecordItemControls(CXTPGridRecordItem* pRecordItem)
	: m_pRecordItem(pRecordItem)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridRecordItemControls::~CXTPGridRecordItemControls()
{
	RemoveAll();
}

void CXTPGridRecordItemControls::DoPropExchange(CXTPPropExchange* pPX)
{
	int nCount = (int)GetSize();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("RecordItemControl")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPGridRecordItemControl* pItemControl = GetAt(i);
			ASSERT(pItemControl);
			if (!pItemControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItemControl, RUNTIME_CLASS(CXTPGridRecordItemControl));
		}
	}
	else
	{
		RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPGridRecordItemControl* pItemControl = NULL;

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItemControl, RUNTIME_CLASS(CXTPGridRecordItemControl));

			if (!pItemControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			AddControl(pItemControl);
		}
	}
}

void CXTPGridRecordItemControls::RemoveAll()
{
	for (int nItem = (int)GetSize() - 1; nItem >= 0; nItem--)
	{
		CXTPGridRecordItemControl* pItem = GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}

	if (m_pRecordItem)
	{
		m_pRecordItem->m_pFocusedItemControl = 0;
	}

	TBase::RemoveAll();
}

void CXTPGridRecordItemControls::RemoveAt(int nIndex)
{
	RemoveAt(nIndex, 1);
}

void CXTPGridRecordItemControls::RemoveAt(int nIndex, int nCount)
{
	if (nIndex < 0 || nIndex >= (int)GetSize() || (nIndex + nCount) > (int)GetSize())
	{
		ASSERT(FALSE);
		return;
	}

	for (int i = 0; i < nCount; ++i)
	{
		CXTPGridRecordItemControl* pItem = GetAt(nIndex);

		if (m_pRecordItem && m_pRecordItem->m_pFocusedItemControl == pItem)
		{
			m_pRecordItem->m_pFocusedItemControl = 0;
		}

		if (pItem)
			pItem->InternalRelease();

		TBase::RemoveAt(nIndex);

		RefreshIndexes(nIndex);
	}
}

void CXTPGridRecordItemControls::RefreshIndexes(int nIndexStart)
{
	for (int i = nIndexStart; i < GetSize(); i++)
	{
		CXTPGridRecordItemControl* pItem = GetAt(i);

		if (NULL != pItem)
		{
			pItem->m_nIndex = i;
		}
	}
}

CXTPGridRecordItemControl* CXTPGridRecordItemControls::AddControl(int nType, int nIndex)
{
	CXTPGridRecordItemControl* pControl = NULL;
	switch (nType)
	{
		case xtpItemControlTypeButton:
			pControl = (CXTPGridRecordItemControl*)new CXTPGridRecordItemButton;
			break;
		default: ASSERT(FALSE);
	}
	if (pControl)
		pControl->m_nType = nType;

	return AddControl(pControl, nIndex);
}

CXTPGridRecordItemControl*
	CXTPGridRecordItemControls::AddControl(CXTPGridRecordItemControl* pControl, int nIndex)
{
	if (!pControl)
		return NULL;

	if (nIndex < 0 || nIndex >= GetSize())
		nIndex = Add(pControl);
	else
		InsertAt(nIndex, pControl);

	pControl->m_nIndex = nIndex;

	RefreshIndexes(nIndex + 1);

	return pControl;
}

void CXTPGridRecordItemControls::CopyFrom(CXTPGridRecordItemControls* pSrc)
{
	if (pSrc == this)
		return;

	RemoveAll();

	if (!pSrc)
		return;

	int nCount = pSrc->GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CXTPGridRecordItemControl* pItem = pSrc->GetAt(i);
		if (pItem)
		{
			pItem->InternalAddRef();
			Add(pItem);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGridRecordItemControlHookWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPGridRecordItemControlHookWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

CXTPGridRecordItemControlHookWnd::CXTPGridRecordItemControlHookWnd(
	XTP_GRIDRECORDITEM_CLICKARGS* pClickArgs)
{
	m_ClickArgs = *pClickArgs;

	m_ClickArgs.AddRef();
}

CXTPGridRecordItemControlHookWnd::~CXTPGridRecordItemControlHookWnd()
{
	m_ClickArgs.Release();
}

void CXTPGridRecordItemControlHookWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	m_ClickArgs.ptClient = point;
	ClientToScreen(&m_ClickArgs.ptClient);
	m_ClickArgs.pItem->OnLButtonDown(&m_ClickArgs);
}

void CXTPGridRecordItemControlHookWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	m_ClickArgs.ptClient = point;
	ClientToScreen(&m_ClickArgs.ptClient);
	m_ClickArgs.pItem->OnLButtonUp(&m_ClickArgs);
}

void CXTPGridRecordItemControlHookWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	m_ClickArgs.pItem->OnMouseMove(nFlags, point, m_ClickArgs.pControl);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPGridRecordItemButton, CXTPGridRecordItemControl)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemButton, CXTPGridRecordItemControl)
	INTERFACE_PART(CXTPGridRecordItemButton, XTPDIID_IGridItemButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemButton, XTPDIID_IGridItemButton)

//////////////////////////////////////////////////////////////////////////
BEGIN_DISPATCH_MAP(CXTPGridRecordItemControls, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridRecordItemControls, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecordItemControls, "RemoveAll", 2, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridRecordItemControls, "RemoveAt", 3, OleRemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemControls, "AddButton", 4, OleAddButton, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridRecordItemControls, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH,
					 VTS_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridRecordItemControls, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridRecordItemControls, XTPDIID_IGridItemControls, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridRecordItemControls, XTPDIID_IGridItemControls)
IMPLEMENT_ENUM_VARIANT(CXTPGridRecordItemControls)

LPDISPATCH CXTPGridRecordItemControls::OleAddButton(int nIndex)
{
	CXTPGridRecordItemControl* pItemControl = AddControl(xtpItemControlTypeButton, nIndex);
	if (!pItemControl)
		return NULL;
	return pItemControl->GetIDispatch(TRUE);
}

void CXTPGridRecordItemControls::OleRemoveAt(int nIndex)
{
	RemoveAt(nIndex);
}

LPDISPATCH CXTPGridRecordItemControls::OleGetItem(long nIndex)
{
	CXTPGridRecordItemControl* pItemControl = GetAt(nIndex);

	if (pItemControl)
		return pItemControl->GetIDispatch(TRUE);

	return NULL;
}

#endif
