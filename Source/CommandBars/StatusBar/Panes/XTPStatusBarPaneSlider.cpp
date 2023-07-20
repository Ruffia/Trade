// XTPStatusBarPaneSlider.cpp.cpp
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
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/Slider/XTPSliderPaintManager.h"
#include "CommandBars/StatusBar/XTPStatusBar.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPane.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPaneScrollBar.h"
#include "CommandBars/StatusBar/Panes/XTPStatusBarPaneSlider.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarSliderPane

IMPLEMENT_DYNAMIC(CXTPStatusBarSliderPane, CXTPStatusBarScrollBarPane)

CXTPStatusBarSliderPane::CXTPStatusBarSliderPane()
{
}

CXTPStatusBarSliderPane::~CXTPStatusBarSliderPane()
{
}

CXTPScrollBarPaintManager* CXTPStatusBarSliderPane::GetScrollBarPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetSliderPaintManager();
}

void CXTPStatusBarSliderPane::SetTicks(double* pTicks, int nCount)
{
	SAFE_DELETE(m_pTicks);

	if (nCount)
	{
		m_pTicks = new XTP_SLIDERTICKS(pTicks, nCount);
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPStatusBarSliderPane, CXTPStatusBarPane)
	DISP_PROPERTY_ID(CXTPStatusBarSliderPane, "Min", 120, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPStatusBarSliderPane, "Max", 121, m_nMax, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBarSliderPane, "Value", 122, GetPos, SetPos, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBarSliderPane, "SetTooltipPart", 123, OleSetTooltipPart, VT_EMPTY,
					 VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPStatusBarSliderPane, "SetTicks", 124, OleSetTicks, VT_EMPTY, VTS_VARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPStatusBarSliderPane, CXTPStatusBarPane)
	INTERFACE_PART(CXTPStatusBarSliderPane, XTPDIID_StatusBarSliderPane, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBarSliderPane, XTPDIID_StatusBarSliderPane)

void CXTPStatusBarSliderPane::OleSetTooltipPart(int nPart, LPCTSTR lpszTooltip)
{
	SetTooltipPart(nPart, lpszTooltip);
}

void CXTPStatusBarSliderPane::OleSetTicks(const VARIANT& Ticks)
{
	int nCount  = 1;
	double* pID = NULL;

	if ((Ticks.vt & VT_ARRAY) != 0)
	{
		int nType = Ticks.vt & (~VT_ARRAY);

		if (nType != VT_VARIANT && nType != VT_I4 && nType != VT_I2 && nType != VT_R4
			&& nType != VT_R8)
			return;

		COleSafeArray saSrc(Ticks);

		if (saSrc.GetDim() != 1)
			return;

		nCount = saSrc.GetOneDimSize();

		long nLBound = 0;
		saSrc.GetLBound(1, &nLBound);

		if (nCount < 1)
			return;

		pID = new double[nCount];

		for (long nIndex = 0; nIndex < nCount; nIndex++)
		{
			long nElement = nIndex + nLBound;

			if (nType == VT_VARIANT)
			{
				VARIANT varData;
				saSrc.GetElement(&nElement, &varData);

				COleVariant var(varData);
				var.ChangeType(VT_R8);

				pID[nIndex] = V_R8(&var);
			}
			else if (nType == VT_I2 || nType == VT_I4)
			{
				int nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
			else if (nType == VT_R4)
			{
				float nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
			else if (nType == VT_R8)
			{
				double nItem = 0;
				saSrc.GetElement(&nElement, &nItem);

				pID[nIndex] = nItem;
			}
		}
	}
	else
	{
		COleVariant var(Ticks);
		var.ChangeType(VT_R8);

		pID	= new double[1];
		pID[0] = V_R8(&var);
	}

	SetTicks(pID, nCount);
	delete[] pID;
}

#endif
