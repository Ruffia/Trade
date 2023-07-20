// XTPControlProgress.cpp : implementation of the CXTPControlProgress class.
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/Controls/Progress/XTPProgressBase.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlProgress.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Progress/XTPProgressPaintManager.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPProgressBase::CXTPProgressBase()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
}

CXTPProgressBase::~CXTPProgressBase()
{
}

void CXTPProgressBase::GetRange(int& nLower, int& nUpper) const
{
	nLower = m_nMin;
	nUpper = m_nMax;
}

void CXTPProgressBase::SetRange(int nLower, int nUpper)
{
	if (m_nMin != nLower || m_nMax != nUpper)
	{
		m_nMin = nLower;
		m_nMax = nUpper;
		RedrawProgress();
	}
}

void CXTPProgressBase::SetPos(int nPos)
{
	if (nPos < m_nMin)
		nPos = m_nMin;
	if (nPos > m_nMax)
		nPos = m_nMax;

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		RedrawProgress();
	}
}

IMPLEMENT_XTP_CONTROL(CXTPControlProgress, CXTPControl)

CXTPControlProgress::CXTPControlProgress()
{
	m_nWidth = XTP_DPI_X(100);
}

CSize CXTPControlProgress::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight > 0 ? m_nHeight : GetProgressPaintManager()->m_cyProgress);
}

void CXTPControlProgress::Draw(CDC* pDC)
{
	GetProgressPaintManager()->DrawProgress(pDC, this);
}

void CXTPControlProgress::RedrawProgress()
{
	RedrawParent(FALSE);
}

CXTPProgressPaintManager* CXTPControlProgress::GetProgressPaintManager() const
{
	return GetPaintManager()->GetProgressPaintManager();
}

CRect CXTPControlProgress::GetProgressRect()
{
	return GetRect();
}

void CXTPControlProgress::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControl::Copy(pControl, bRecursive);

		CXTPControlProgress* pControlProgress = DYNAMIC_DOWNCAST(CXTPControlProgress, pControl);
		if (!pControlProgress)
			return;

		SetRange(pControlProgress->m_nMin, pControlProgress->m_nMax);
		SetPos(pControlProgress->GetPos());
	}
}

void CXTPControlProgress::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Int(pPX, _T("Min"), m_nMin, 0);
	PX_Int(pPX, _T("Max"), m_nMax, 100);
	PX_Int(pPX, _T("Pos"), m_nPos, 0);
}

void CXTPControlProgress::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!bKeyboard && IsCustomizeMode() && IsCustomizeMovable())
	{
		CustomizeStartDrag(pt);
		return;
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlProgress, CXTPControl)
	DISP_PROPERTY_EX_ID(CXTPControlProgress, "Pos", 200, OleGetPos, OleSetPos, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlProgress, "Min", 201, m_nMin, OleRangeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlProgress, "Max", 202, m_nMax, OleRangeChanged, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPControlProgress, CXTPControl)
	INTERFACE_PART(CXTPControlProgress, XTPDIID_CommandBarProgress, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlProgress, XTPDIID_CommandBarProgress)

void CXTPControlProgress::OleRangeChanged()
{
	RedrawParent(FALSE);
}

void CXTPControlProgress::OleSetPos(int nPos)
{
	SetPos(nPos);
}

int CXTPControlProgress::OleGetPos()
{
	return GetPos();
}

#endif
