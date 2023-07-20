// XTPGridSections.cpp : implementation of the CXTPGridSections class.
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
#include "Common/XTPColorManager.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridControl.h"
#include "GridControl/XTPGridSection.h"
#include "GridControl/XTPGridSections.h"
#include "GridControl/XTPGridRows.h"
#include "GridControl/XTPGridRow.h"
#include "GridControl/XTPGridPaintManager.h"

#include "GridControl/XTPGridControlIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPGridSections::CXTPGridSections(CXTPGridControl* pControl)
	: m_rcSections(0, 0, 0, 0)
	, m_pControl(pControl)
{
	Add(new CXTPGridSection(pControl, xtpRowTypeHeader));
	Add(new CXTPGridSection(pControl, xtpRowTypeBody));
	Add(new CXTPGridSection(pControl, xtpRowTypeFooter));

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPGridSections::~CXTPGridSections()
{
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		SAFE_DELETE(pSection);
	}

	RemoveAll();
}

CXTPGridSection* CXTPGridSections::FindSection(CXTPGridRow* pRow) const
{
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		if (pSection->GetRows()->Contains(pRow))
		{
			return pSection;
		}
	}

	return NULL;
}

BOOL CXTPGridSections::EnsureVisible(CDC* pDC, CXTPGridRow* pRow)
{
	// None of the parameters may be null
	ASSERT(NULL != pDC);
	ASSERT(NULL != pRow);

	BOOL bResult = FALSE;

	if (NULL != pRow)
	{
		CXTPGridSection* pSection = pRow->GetSection();
		ASSERT(NULL != pSection); // Section must always be valid

		if (NULL != pSection)
		{
			bResult = pSection->EnsureVisible(pDC, pRow);
		}
	}

	return bResult;
}

CXTPGridRow* CXTPGridSections::GetFocusedRow() const
{
	CXTPGridRow* pFocusedRow = NULL;

	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		pFocusedRow				  = pSection->GetRows()->GetFocusedRow();

		if (NULL != pFocusedRow)
		{
			return pFocusedRow;
		}
	}

	return NULL;
}

CRect CXTPGridSections::GetRect() const
{
	return m_rcSections;
}

CXTPGridSection* CXTPGridSections::HitTest(CPoint pt) const
{
	CXTPGridSection* pHitSection = NULL;

	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		if (pSection->m_rcSection.PtInRect(pt))
		{
			ASSERT(NULL == pHitSection); // There can only be one
			pHitSection = pSection;
		}
	}

	return pHitSection;
}

void CXTPGridSections::Draw(CDC* pDC)
{
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		if (pSection->IsVisible())
		{
			pSection->Draw(pDC);
		}
	}
}

void CXTPGridSections::AdjustLayout(CDC* pDC, CRect rcSections)
{
	m_rcSections = rcSections;

	CArray<int, int> arrHeightSection;
	CArray<int, int> arrHeightDivider;

	arrHeightSection.SetSize(GetSize());
	arrHeightDivider.SetSize(GetSize());

	int nSection;

	//////////////////////////////////////////////////////////////////////////
	// Calculate height of sections
	//////////////////////////////////////////////////////////////////////////

	for (nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		int nHeightSection = 0; // Height of the section in pixels
		int nHeightDivider = 0; // Height of the divider in pixels

		if (pSection->IsVisible())
		{
			// Divider
			XTPGridFixedRowsDividerStyle dividerStyle = pSection->GetDividerStyle();
			nHeightDivider = m_pControl->GetPaintManager()->GetSectionDividerHeight(dividerStyle);

			// Section
			switch (pSection->GetHeightMode())
			{
				case xtpGridSectionHeightModeFixed: { nHeightSection = pSection->GetHeight();
				}
				break;

				case xtpGridSectionHeightModeRelative:
				{
					double dPercent = rcSections.Height() / 100.0; // Pixels per percent
					nHeightSection  = static_cast<int>(dPercent * pSection->GetHeight());
				}
				break;

				case xtpGridSectionHeightModeAuto:
				case xtpGridSectionHeightModeAutoExpand:
				case xtpGridSectionHeightModeAutoShrink:
				{
					nHeightSection = pSection->GetTotalHeight(pDC);
				}
				break;

				case xtpGridSectionHeightModeExpand:
				{
					// Remaining height is calculated in the next step
					nHeightSection = 0;
				}
				break;
			}
		}

		arrHeightSection[nSection] = nHeightSection;
		arrHeightDivider[nSection] = nHeightDivider;
	}

	//////////////////////////////////////////////////////////////////////////
	// Calculate remaining height
	//////////////////////////////////////////////////////////////////////////

	int nHeightExpand = rcSections.Height();

	for (nSection = 0; nSection < GetSize(); nSection++)
	{
		nHeightExpand -= arrHeightSection[nSection];
		nHeightExpand -= arrHeightDivider[nSection];
	}

	if (nHeightExpand < 0)
	{
		// Shrink auto shrink section
		for (nSection = 0; nSection < GetSize(); nSection++)
		{
			CXTPGridSection* pSection = GetAt(nSection);

			if (xtpGridSectionHeightModeAutoShrink == pSection->GetHeightMode())
			{
				int nHeightSection = arrHeightSection[nSection];

				nHeightSection += nHeightExpand; // nHeightExpand is negative
				nHeightSection = max(0, nHeightSection);

				arrHeightSection[nSection] = nHeightSection;
			}
		}
	}

	nHeightExpand = max(0, nHeightExpand);

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	for (nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		if (xtpGridSectionHeightModeExpand == pSection->GetHeightMode())
		{
			arrHeightSection[nSection] = nHeightExpand;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	int nTop = rcSections.top;

	for (nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);

		// Divider at the top
		if (xtpGridSectionDividerStyleNone != pSection->GetDividerStyle()
			&& xtpGridSectionDividerPositionTop == pSection->GetDividerPosition())
		{
			pSection->m_rcDivider.left   = rcSections.left;
			pSection->m_rcDivider.right  = rcSections.right;
			pSection->m_rcDivider.top	= nTop;
			pSection->m_rcDivider.bottom = nTop + arrHeightDivider[nSection];

			nTop += arrHeightDivider[nSection];
		}

		pSection->m_rcSection.left   = rcSections.left;
		pSection->m_rcSection.right  = rcSections.right;
		pSection->m_rcSection.top	= nTop;
		pSection->m_rcSection.bottom = nTop + arrHeightSection[nSection];

		nTop += arrHeightSection[nSection];

		// Divider at the bottom
		if (xtpGridSectionDividerStyleNone != pSection->GetDividerStyle()
			&& xtpGridSectionDividerPositionBottom == pSection->GetDividerPosition())
		{
			pSection->m_rcDivider.left   = rcSections.left;
			pSection->m_rcDivider.right  = rcSections.right;
			pSection->m_rcDivider.top	= nTop;
			pSection->m_rcDivider.bottom = nTop + arrHeightDivider[nSection];

			nTop += arrHeightDivider[nSection];
		}
	}
}

void CXTPGridSections::SetMarkupContext(CXTPMarkupContext* pMarkupContext)
{
	// Set markup context for all sections
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		pSection->SetMarkupContext(pMarkupContext);
	}
}

void CXTPGridSections::ReSortRows()
{
	// Re-sort rows of all sections
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		ASSERT(NULL != pSection);

		if (pSection->IsAllowSort())
		{
			pSection->ReSortRows();
		}
	}

	m_pControl->AdjustIndentation();
	m_pControl->AdjustLayout();
	m_pControl->RedrawControl();
}

void CXTPGridSections::ResetContent()
{
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		ASSERT(NULL != pSection);

		pSection->ResetContent();
	}
}

void CXTPGridSections::RefreshIndexes(BOOL bAdjustLayout)
{
	// Re-sort rows of all sections
	for (int nSection = 0; nSection < GetSize(); nSection++)
	{
		CXTPGridSection* pSection = GetAt(nSection);
		ASSERT(NULL != pSection);

		pSection->RefreshIndexes();
	}

	if (bAdjustLayout)
	{
		m_pControl->AdjustScrollBars();
	}
}

#ifdef _XTP_ACTIVEX

IMPLEMENT_OLETYPELIB_EX(CXTPGridSections, XTPDIID_IGridSections)

BEGIN_DISPATCH_MAP(CXTPGridSections, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPGridSections, "Count", 1, OleGetCount, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPGridSections, "Section", 2, OleGetSection, VT_DISPATCH, VTS_I4)
	// DISP_PROPERTY_EX_ID(CXTPGridSections, "Section", 2, OleGetSection, SetNotSupported,
	// VT_DISPATCH)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPGridSections, CXTPCmdTarget)
	INTERFACE_PART(CXTPGridSections, XTPDIID_IGridSections, Dispatch)
END_INTERFACE_MAP()

LPDISPATCH CXTPGridSections::OleGetSection(int nIndex) const
{
	CXTPGridSection* pSection = GetAt(nIndex);

	if (NULL != pSection)
	{
		return pSection->GetIDispatch(TRUE);
	}
	else
	{
		return NULL;
	}
}

#endif
