// XTPRibbonGroups.cpp: implementation of the CXTPRibbonGroups class.
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
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonGroups.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonPaintManager.h"
#include "Ribbon/XTPRibbonTab.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonGroups::CXTPRibbonGroups()
{
	m_pParentTab	   = NULL;
	m_nGroupsScrollPos = 0;

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPRibbonGroups::~CXTPRibbonGroups()
{
	RemoveAll();
}

void CXTPRibbonGroups::RemoveAll()
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPRibbonGroup* pGroup = GetAt(nIndex);
		pGroup->OnGroupRemoved();
		pGroup->InternalRelease();
	}
	m_arrGroups.RemoveAll();
	RefreshIndexes();
}

void CXTPRibbonGroups::RefreshIndexes()
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPRibbonGroup* pGroup = GetAt(nIndex);
		pGroup->m_nIndex		= nIndex;
	}
}

CXTPRibbonBar* CXTPRibbonGroups::GetRibbonBar() const
{
	return m_pParentTab->GetRibbonBar();
}

CXTPRibbonGroup* CXTPRibbonGroups::Add(LPCTSTR lpszCaption, int nId)
{
	return InsertAt(GetCount(), lpszCaption, nId);
}

CXTPRibbonGroup* CXTPRibbonGroups::InsertAt(int nIndex, CXTPRibbonGroup* pGroup)
{
	pGroup->m_pGroups	= this;
	pGroup->m_pRibbonBar = m_pParentTab->GetRibbonBar();
	pGroup->m_pParent	= m_pParentTab->GetParent();

	m_arrGroups.InsertAt(nIndex, pGroup);

	RefreshIndexes();

	pGroup->OnGroupAdded();

	return pGroup;
}

CXTPRibbonGroup* CXTPRibbonGroups::InsertAt(int nIndex, LPCTSTR lpszCaption, int nId)
{
	CXTPRibbonGroup* pGroup = new CXTPRibbonGroup;
	pGroup->SetID(nId);
	pGroup->SetCaption(lpszCaption);

	InsertAt(nIndex, pGroup);

#ifndef _XTP_COMMANDBARS_ACTIVEX
	pGroup->LoadToolBar(-1 == nId ? -1 : XTPToUInt(nId));
#endif

	return pGroup;
}

void CXTPRibbonGroups::Remove(CXTPRibbonGroup* pGroup)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		if (pGroup == GetAt(nIndex))
		{
			Remove(nIndex);
			return;
		}
	}
}

void CXTPRibbonGroups::Remove(int nIndex)
{
	if ((nIndex >= 0) && (nIndex < GetCount()))
	{
		CXTPRibbonGroup* pGroup = GetAt(nIndex);
		ASSERT(pGroup);

		pGroup->OnGroupRemoved();
		m_arrGroups.RemoveAt(nIndex);
		pGroup->InternalRelease();
		RefreshIndexes();
	}
}

CXTPRibbonGroup* CXTPRibbonGroups::HitTest(CPoint point) const
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPRibbonGroup* pGroup = GetAt(nIndex);
		if (pGroup->IsVisible() && pGroup->GetRect().PtInRect(point))
		{
			return pGroup;
		}
	}

	return NULL;
}

void CXTPRibbonGroups::Copy(CXTPRibbonGroups* pGroups)
{
	if (this != pGroups)
	{
		RemoveAll();

		for (int nIndex = 0; nIndex < pGroups->GetCount(); nIndex++)
		{
			CXTPRibbonGroup* pGroup = pGroups->GetAt(nIndex);
			CXTPRibbonGroup* pClone = (CXTPRibbonGroup*)pGroup->GetRuntimeClass()->CreateObject();
			InsertAt(nIndex, pClone);

			pClone->Copy(pGroup);
			pClone->m_pGroups = this;
		}
	}
}

void CXTPRibbonGroups::_ReduceSmartLayoutToolBar(CDC* pDC, int* pWidth, int nWidth)
{
	int nResult = _CalcSmartLayoutToolBar(pWidth);
	if (nResult <= nWidth)
	{
		if (nResult == nWidth)
			return;

		for (int i = GetCount() - 1; i >= 0; i--)
		{
			CXTPRibbonGroup* pGroup = GetAt(i);
			if (!pGroup->IsVisible())
				continue;

			if (pGroup->OnExtendSize(nWidth - nResult))
			{
				pWidth[i] = pGroup->OnCalcDynamicSize(pDC);
				break;
			}
		}

		return;
	}

	BOOL bReduced = FALSE;

	do
	{
		bReduced = FALSE;

		for (int nLevel = 0; nLevel < 4; nLevel++)
		{
			for (int i = GetCount() - 1; i >= 0; i--)
			{
				CXTPRibbonGroup* pGroup = GetAt(i);
				if (!pGroup->IsVisible())
					continue;

				if (pGroup->OnReduceSize(nLevel, nResult - nWidth))
				{
					int nGroupWidth = pGroup->OnCalcDynamicSize(pDC);

					nResult -= (pWidth[i] - nGroupWidth);
					pWidth[i] = nGroupWidth;

					bReduced = TRUE;
					break;
				}
			}

			if (bReduced)
				break;
		}

		if (bReduced && nResult <= nWidth)
		{
			return;
		}
	} while (bReduced);
}

int CXTPRibbonGroups::_CalcSmartLayoutToolBar(int* pWidth)
{
	int nWidth = 0;
	for (int i = 0; i < GetCount(); i++)
	{
		nWidth += pWidth[i];

		if (i != GetCount() - 1 && pWidth[i] != 0)
		{
			nWidth += XTP_DPI_X(7);
		}
	}
	return nWidth;
}

void CXTPRibbonGroups::CalcDynamicSize(CDC* pDC, int nLength, DWORD /*dwMode*/,
									   const CRect& rcBorder)
{
	CXTPRibbonScrollableBar* pScrollableBar = GetScrollableBar(m_pParentTab->GetParent());

	int nCount = GetCount();

	if (nCount == 0)
	{
		pScrollableBar->EnableGroupsScroll(FALSE, FALSE);
		return;
	}

	int* pData = new int[XTPToUIntChecked(nCount)];
	int i;

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonGroup* pGroup = GetAt(i);
		if (pGroup->IsVisible())
		{
			pGroup->OnBeforeCalcSize(pDC);
		}
	}

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonGroup* pGroup = GetAt(i);
		pData[i]				= pGroup->IsVisible() ? pGroup->OnCalcDynamicSize(pDC) : 0;
	}

	int nGroupsLength = nLength - rcBorder.right - rcBorder.left;

	_ReduceSmartLayoutToolBar(pDC, pData, nGroupsLength);

	CRect rcBorders = rcBorder;

	const int nGroupSpacing = GetRibbonBar()->GetRibbonPaintManager()->m_nGroupSpacing;

	int nTotalWidth = -XTP_DPI_X(7);

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonGroup* pGroup = GetAt(i);
		if (pGroup->IsVisible())
		{
			nTotalWidth += pData[i] + nGroupSpacing + XTP_DPI_X(5);
		}
	}

	int nScrollPos = pScrollableBar->m_nGroupsScrollPos;

	if (nTotalWidth > nGroupsLength)
	{
		if (nScrollPos > nTotalWidth - nGroupsLength)
		{
			nScrollPos = nTotalWidth - nGroupsLength;
		}
	}
	else
	{
		nScrollPos = 0;
	}
	if (nScrollPos < 0)
		nScrollPos = 0;

	m_nGroupsScrollPos = nScrollPos;

	pScrollableBar->EnableGroupsScroll(nScrollPos > 0,
									   nTotalWidth - nGroupsLength - nScrollPos > 0);

	rcBorders.left -= nScrollPos;

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonGroup* pGroup = GetAt(i);
		if (pGroup->IsVisible())
		{
			pGroup->OnAdjustBorders(pData[i], rcBorders);
			rcBorders.left += pData[i] + nGroupSpacing + XTP_DPI_X(5);
		}
	}

	for (i = 0; i < nCount; i++)
	{
		CXTPRibbonGroup* pGroup = GetAt(i);
		if (pGroup->IsVisible())
		{
			pGroup->OnAfterCalcSize();
		}
	}

	delete[] pData;
}

void CXTPRibbonGroups::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		long nCount = GetCount();
		int i;

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Group")));
		POSITION posEnum = pEnumerator->GetPosition(XTPToDWORDChecked(nCount), FALSE);

		for (i = 0; i < nCount; i++)
		{
			CXTPRibbonGroup* pGroup = GetAt(i);

			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			PX_Object(&secItem, pGroup, RUNTIME_CLASS(CXTPRibbonGroup));
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Group")));
		POSITION posEnum = pEnumerator->GetPosition(0, FALSE);

		RemoveAll();

		while (posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			secItem->m_pOwner		= this;
			CXTPRibbonGroup* pGroup = NULL;
			PX_Object(&secItem, pGroup, RUNTIME_CLASS(CXTPRibbonGroup));

			if (!pGroup)
				AfxThrowArchiveException(CArchiveException::badClass);

			m_arrGroups.Add(pGroup);
			RefreshIndexes();
		}
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonGroups, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "InsertGroup", 100, OleInsertGroup, VT_DISPATCH,
					 VTS_I4 VTS_BSTR VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroups, "GroupCount", 101, OleGetItemCount, SetNotSupported,
						VT_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "Group", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "AddGroup", 102, OleAddGroup, VT_DISPATCH, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "RemoveGroup", 103, OleRemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroups, "RemoveAll", 104, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroups, "RibbonTab", 105, OleGetRibbonTab, SetNotSupported,
						VT_DISPATCH)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonGroups, XTPDIID_IRibbonGroups)

BEGIN_INTERFACE_MAP(CXTPRibbonGroups, CXTPCmdTarget)
	INTERFACE_PART(CXTPRibbonGroups, XTPDIID_IRibbonGroups, Dispatch)
	// INTERFACE_PART(CXTPRibbonGroups, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_ENUM_VARIANT(CXTPRibbonGroups)

LPDISPATCH CXTPRibbonGroups::OleInsertGroup(int nIndex, LPCTSTR lpszTitle, int nId)
{
	CXTPRibbonGroup* pItem = InsertAt(nIndex, lpszTitle, nId);
	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonGroups::OleAddGroup(LPCTSTR lpszTitle, int nId)
{
	CXTPRibbonGroup* pItem = Add(lpszTitle, nId);
	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonGroups::OleGetRibbonTab()
{
	return m_pParentTab->GetIDispatch(TRUE);
}

long CXTPRibbonGroups::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPRibbonGroups::OleGetItem(int nIndex)
{
	CXTPRibbonGroup* pItem = GetAt(nIndex);

	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}

void CXTPRibbonGroups::OleRemoveAt(int nIndex)
{
	Remove(nIndex);
}

#endif
