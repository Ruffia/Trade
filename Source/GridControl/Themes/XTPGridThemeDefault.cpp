// XTPGridThemeDefault.cpp : implementation of the CXTPGridPaintManager class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPCustomHeap.h"

#include "GridControl/XTPGridDefines.h"
#include "GridControl/XTPGridAllocators.h"
#include "GridControl/XTPGridPaintManager.h"
#include "GridControl/XTPGridColumn.h"
#include "GridControl/XTPGridHeader.h"

#include "GridControl/Themes/XTPGridThemeDefault.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPGridThemeDefault
/////////////////////////////////////////////////////////////////////////////

CXTPGridThemeDefault::CXTPGridThemeDefault()
{
	m_columnStyle = xtpGridColumnShaded;
}

void CXTPGridThemeDefault::DrawColumnBackground(CDC* pDC, CXTPGridColumn* pColumn, CRect rcColumn,
												BOOL& /*bColumnPressed*/,
												BOOL& /*bDraggingFromHeader*/,
												CXTPGridHeader* pHeader)
{
	CXTPGridColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1)
										  : NULL;

	pDC->DrawEdge(rcColumn, EDGE_RAISED, BF_RECT);

	if (!pColumn->GetDrawHeaderDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right - XTP_DPI_X(1), rcColumn.top + XTP_DPI_Y(1),
						 rcColumn.Height() - XTP_DPI_Y(2), m_clrHeaderControl);
		pDC->SetPixel(rcColumn.right - XTP_DPI_X(1), rcColumn.top, m_clrControlLightLight);
	}
	if (pPrevColumn && !pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + XTP_DPI_Y(1),
						 rcColumn.Height() - XTP_DPI_Y(2), m_clrHeaderControl);
}
