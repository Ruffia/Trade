// XTPGridHyperlink.cpp : implementation of the CXTPGridHyperlink class.
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

#include "StdAfx.h"

#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridHyperlink.h"
#include "GridControl/XTPGridControlIIDs.h"

#pragma warning(disable : 4097) // TBase' used as synonym for class-name

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

CXTPGridHyperlinkStyle::CXTPGridHyperlinkStyle(COLORREF color, XTPGridTextDecoration textDecoration)
	: m_color(color)
	, m_textDecoration(textDecoration)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

COLORREF CXTPGridHyperlinkStyle::GetColor() const
{
	return m_color;
}

void CXTPGridHyperlinkStyle::SetColor(COLORREF color)
{
	m_color = color;
}

XTPGridTextDecoration CXTPGridHyperlinkStyle::GetTextDecoration() const
{
	return m_textDecoration;
}

void CXTPGridHyperlinkStyle::SetTextDecoration(XTPGridTextDecoration textDecoration)
{
	m_textDecoration = textDecoration;
}

IMPLEMENT_DYNAMIC(CXTPGridHyperlink, CXTPCmdTarget)
IMPLEMENT_DYNAMIC(CXTPGridHyperlinks, CXTPCmdTarget)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPGridHyperlink::CXTPGridHyperlink(int nHyperTextBegin, int nHyperTextLen)
	: m_rcHyperSpot(0, 0, 0, 0)
	, m_nHyperTextBegin(nHyperTextBegin)
	, m_nHyperTextLen(nHyperTextLen)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridHyperlink::~CXTPGridHyperlink()
{
}

void CXTPGridHyperlink::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("HyperTextBegin"), m_nHyperTextBegin);
	PX_Int(pPX, _T("HyperTextLen"), m_nHyperTextLen);
}

CXTPGridHyperlinks::CXTPGridHyperlinks()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridHyperlinks::~CXTPGridHyperlinks()
{
	RemoveAll();
}

void CXTPGridHyperlinks::RemoveAt(int nIndex)
{
	RemoveAt(nIndex, 1);
}

void CXTPGridHyperlinks::RemoveAt(int nIndex, int nCount)
{
	if (nIndex < 0 || nIndex >= (int)GetSize() || (nIndex + nCount) > (int)GetSize())
	{
		ASSERT(FALSE);
		return;
	}

	for (int i = 0; i < nCount; ++i)
	{
		CXTPGridHyperlink* pItem = GetAt(nIndex);
		if (pItem)
			pItem->InternalRelease();

		TBase::RemoveAt(nIndex);
	}
}

void CXTPGridHyperlinks::RemoveAll()
{
	for (int nItem = (int)GetSize() - 1; nItem >= 0; nItem--)
	{
		CXTPGridHyperlink* pItem = GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}
	TBase::RemoveAll();
}

void CXTPGridHyperlinks::DoPropExchange(CXTPPropExchange* pPX)
{
	int nCount = (int)GetSize();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("Hyperlink")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPGridHyperlink* pItem = GetAt(i);
			ASSERT(pItem);
			if (pItem)
			{
				CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
				pItem->DoPropExchange(&secItem);
			}
		}
	}
	else
	{
		POSITION posItem = pEnumItems->GetPosition();
		while (posItem)
		{
			CXTPGridHyperlink* pItem = new CXTPGridHyperlink();
			if (!pItem)
			{
				return;
			}
			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			pItem->DoPropExchange(&secItem);

			Add(pItem);
		}
	}
}

void CXTPGridHyperlinks::CopyFrom(CXTPGridHyperlinks* pSrc)
{
	if (pSrc == this)
		return;

	RemoveAll();

	if (!pSrc)
		return;

	int nCount = pSrc->GetSize();
	for (int i = 0; i < nCount; i++)
	{
		CXTPGridHyperlink* pHlnk = pSrc->GetAt(i);
		if (pHlnk)
		{
			pHlnk->InternalAddRef();
			Add(pHlnk);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_INTERFACE_MAP(CXTPGridHyperlink, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridHyperlink, XTPDIID_IGridHyperlink, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridHyperlink, XTPDIID_IGridHyperlink)

BEGIN_DISPATCH_MAP(CXTPGridHyperlink, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPGridHyperlink, "HyperTextBegin", 1, m_nHyperTextBegin, VT_I4)
	DISP_PROPERTY_ID(CXTPGridHyperlink, "HyperTextLen", 2, m_nHyperTextLen, VT_I4)
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CXTPGridHyperlinks, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridHyperlinks, XTPDIID_IGridHyperlinks, Dispatch)
	// INTERFACE_PART(CXTPGridHyperlinks, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPGridHyperlinks, XTPDIID_IGridHyperlinks)
IMPLEMENT_ENUM_VARIANT(CXTPGridHyperlinks)

BEGIN_DISPATCH_MAP(CXTPGridHyperlinks, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPGridHyperlinks, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPGridHyperlinks, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridHyperlinks, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPGridHyperlinks, "AddHyperlink", 2, OleAddHyperlink, VT_DISPATCH,
					 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPGridHyperlinks, "Remove", 3, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPGridHyperlinks, "RemoveAll", 4, OleRemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

LPDISPATCH CXTPGridHyperlinks::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetSize())
	{
		CXTPGridHyperlink* pItem = GetAt(nIndex);
		return pItem ? pItem->GetIDispatch(TRUE) : NULL;
	}
	AfxThrowOleException(DISP_E_BADINDEX);
	return NULL;
}

LPDISPATCH CXTPGridHyperlinks::OleAddHyperlink(long nHyperTextBegin, long nHyperTextLen)
{
	CXTPGridHyperlink* pItem = new CXTPGridHyperlink(nHyperTextBegin, nHyperTextLen);
	if (!pItem)
		return NULL;

	Add(pItem);
	return pItem->GetIDispatch(TRUE);
}

#endif
