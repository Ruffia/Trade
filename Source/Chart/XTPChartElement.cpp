// XTPChartElement.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartContent.h"
#include "Chart/XTPChartIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _XTP_ACTIVEX
IMPLEMENT_DYNAMIC(CXTPChartObject, CXTPCmdTarget)
#else
IMPLEMENT_DYNAMIC(CXTPChartObject, CObject)
#endif

IMPLEMENT_DYNAMIC(CXTPChartElement, CXTPChartObject)

CXTPChartElement::CXTPChartElement()
{
	m_pOwner = NULL;
	m_dwRef  = 1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartElement::~CXTPChartElement()
{
}

void CXTPChartElement::Release()
{
	m_pOwner = NULL;

	InternalRelease();
}

#ifndef _XTP_ACTIVEX

DWORD CXTPChartElement::InternalAddRef()
{
	return XTPToDWORDChecked(InterlockedIncrement(&m_dwRef));
}

DWORD CXTPChartElement::InternalRelease()
{
	if (m_dwRef == 0)
		return 0;

	LONG lResult = InterlockedDecrement(&m_dwRef);
	if (lResult == 0)
	{
		delete this;
	}
	return XTPToDWORDChecked(lResult);
}

#endif

void CXTPChartElement::OnChartChanged(XTPChartUpdateOptions updateOptions)
{
	if (m_pOwner)
	{
		m_pOwner->OnChartChanged(updateOptions);
	}
}

CXTPChartContent* CXTPChartElement::GetContent() const
{
	CXTPChartElement* pOwner = m_pOwner;
	if (!pOwner)
		return NULL;

	while (pOwner->m_pOwner != NULL)
	{
		pOwner = pOwner->m_pOwner;
	}

	if (pOwner->IsKindOf(RUNTIME_CLASS(CXTPChartContent)))
	{
		return (CXTPChartContent*)pOwner;
	}

	return NULL;
}

CXTPChartAppearance* CXTPChartElement::GetAppearance() const
{
	CXTPChartContent* pContent = GetContent();
	if (!pContent)
		return NULL;

	return pContent->GetAppearance();
}

void CXTPChartElement::DoPropExchange(CXTPPropExchange* /*pPX*/)
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartContainer

CXTPChartContainer::CXTPChartContainer()
	: m_bBeingFrequentlyUpdated(FALSE)
{
}

CXTPChartContainer::~CXTPChartContainer()
{
}

void CXTPChartContainer::OnChartChanged(XTPChartUpdateOptions /*updateOptions*/)
{
}

void CXTPChartContainer::SetCapture(CXTPChartElementView* /*pView*/)
{
}

void CXTPChartContainer::BeginFrequentUpdates()
{
	m_bBeingFrequentlyUpdated = TRUE;
}

void CXTPChartContainer::EndFrequentUpdates(
	XTPChartUpdateOptions updateOptions /*= xtpChartUpdateRedraw*/)
{
	if (m_bBeingFrequentlyUpdated)
	{
		m_bBeingFrequentlyUpdated = FALSE;
		OnChartChanged(updateOptions);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTextElement

IMPLEMENT_DYNAMIC(CXTPChartTextElement, CXTPChartElement)

//////////////////////////////////////////////////////////////////////////
// CXTPChartElementCollection

IMPLEMENT_DYNAMIC(CXTPChartElementCollection, CXTPChartElement)

CXTPChartElementCollection::CXTPChartElementCollection()
{
}

CXTPChartElementCollection::~CXTPChartElementCollection()
{
	RemoveAll();
}

void CXTPChartElementCollection::Release()
{
	m_pOwner = NULL;

	RemoveAll();

	CXTPChartElement::Release();
}

void CXTPChartElementCollection::RemoveAll()
{
	if (m_arrElements.GetSize() == 0)
		return;

	for (int i = 0; i < m_arrElements.GetSize(); i++)
	{
		SAFE_RELEASE(m_arrElements[i]);
	}
	m_arrElements.RemoveAll();

	OnChartChanged();
}

void CXTPChartElementCollection::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrElements.GetSize())
		return;

	CXTPChartElement* pElement = m_arrElements[nIndex];

	m_arrElements.RemoveAt(nIndex);
	SAFE_RELEASE(pElement);

	OnChartChanged();
}

void CXTPChartElementCollection::Remove(CXTPChartElement* pElement)
{
	for (int i = 0; i < m_arrElements.GetSize(); i++)
	{
		CXTPChartElement* p = m_arrElements[i];
		if (p == pElement)
		{
			RemoveAt(i);
			break;
		}
	}
}

void CXTPChartElementCollection::InsertAt(int nIndex, CXTPChartElement* pElement)
{
	m_arrElements.InsertAt(nIndex, pElement);
	pElement->m_pOwner = this;

	OnChartChanged();
}

int CXTPChartElementCollection::IndexOf(CXTPChartElement* pPanel) const
{
	for (int i = 0; i < m_arrElements.GetSize(); i++)
		if (m_arrElements[i] == pPanel)
			return i;

	return -1;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartElement, CXTPCmdTarget)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPChartElement, CXTPCmdTarget)
	INTERFACE_PART(CXTPChartElement, XTPDIID_ChartElement, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartElement, XTPDIID_ChartElement)

#endif
