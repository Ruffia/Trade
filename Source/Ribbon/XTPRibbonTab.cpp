// XTPRibbonTab.cpp: implementation of the CXTPRibbonTab class.
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
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "TabManager/Includes.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/XTPRibbonTab.h"
#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonGroups.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonControlTab.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//

CXTPRibbonTabContextHeader::CXTPRibbonTabContextHeader(CXTPRibbonTab* pFirstTab)
{
	ASSERT(pFirstTab);
	if (!pFirstTab)
		return;
	m_pLastTab = m_pFirstTab = pFirstTab;
	m_color					 = pFirstTab->GetContextColor();
	m_strCaption			 = pFirstTab->GetContextCaption();
	m_rcRect.SetRectEmpty();
	pFirstTab->m_pContextHeader = this;
}

CXTPRibbonTabContextHeaders::CXTPRibbonTabContextHeaders()
{
}

CXTPRibbonTabContextHeaders::~CXTPRibbonTabContextHeaders()
{
	RemoveAll();
}
int CXTPRibbonTabContextHeaders::GetCount() const
{
	return (int)m_arrHeaders.GetSize();
}

CXTPRibbonTabContextHeader* CXTPRibbonTabContextHeaders::GetHeader(int nIndex) const
{
	return m_arrHeaders.GetAt(nIndex);
}

void CXTPRibbonTabContextHeaders::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		delete m_arrHeaders[i];
	}
	m_arrHeaders.RemoveAll();
}

CXTPRibbonTabContextHeader* CXTPRibbonTabContextHeaders::HitTest(CPoint pt) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrHeaders[i]->m_rcRect.PtInRect(pt))
			return m_arrHeaders[i];
	}
	return NULL;
}

void CXTPRibbonTabContextHeaders::Add(CXTPRibbonTabContextHeader* pHeader)
{
	m_arrHeaders.Add(pHeader);
}

IMPLEMENT_SERIAL(CXTPRibbonTab, CXTPTabManagerItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPRibbonTab::CXTPRibbonTab()
{
	m_nId = 0;

	m_pGroups				= new CXTPRibbonGroups;
	m_pGroups->m_pParentTab = this;

	m_nContextColor = xtpRibbonTabContextColorNone;

	m_pContextHeader = NULL;
	m_pRibbonBar	 = NULL;
	m_pParent		 = NULL;
}

CXTPRibbonTab::~CXTPRibbonTab()
{
	ASSERT(m_pGroups->GetCount() == 0);

	CMDTARGET_RELEASE(m_pGroups);
}

void CXTPRibbonTab::Copy(CXTPRibbonTab* pRibbonTab)
{
	if (this != pRibbonTab)
	{
		m_strCaption		= pRibbonTab->m_strCaption;
		m_nContextColor		= pRibbonTab->m_nContextColor;
		m_strContextCaption = pRibbonTab->m_strContextCaption;
		m_nId				= pRibbonTab->m_nId;
		m_bVisible			= pRibbonTab->m_bVisible;
		m_strToolTip		= pRibbonTab->m_strToolTip;

		if (m_pTabManager && pRibbonTab->IsSelected())
		{
			Select();
		}

		m_pGroups->Copy(pRibbonTab->m_pGroups);
	}
}

void CXTPRibbonTab::OnRemoved()
{
	m_pGroups->RemoveAll();
}

CXTPRibbonBar* CXTPRibbonTab::GetRibbonBar() const
{
	return m_pRibbonBar;
}

CXTPRibbonGroup* CXTPRibbonTab::AddGroup(LPCTSTR lpszCaption)
{
	return GetGroups()->Add(lpszCaption);
}

CXTPRibbonGroup* CXTPRibbonTab::AddGroup(int nID)
{
	CString strCaption;

	if (0 != nID)
		VERIFY(strCaption.LoadString(XTPToUInt(nID)));

	if (strCaption.Find('\n') != -1)
	{
		CString strPrompt = strCaption;
		AfxExtractSubString(strCaption, strPrompt, 1);
	}

	return GetGroups()->Add(strCaption, nID);
}

void CXTPRibbonTab::SetVisible(BOOL bVisible)
{
	if (bVisible == IsVisible())
		return;

	CXTPTabManagerItem::SetVisible(bVisible);

	if (!bVisible && IsSelected())
	{
		CXTPTabManagerItem* pItem = GetTabManager()->FindNextFocusable(-1, +1);
		if (pItem)
		{
			pItem->Select();
		}
	}
}

CString CXTPRibbonTab::GetCaption() const
{
	CString strCaption = CXTPTabManagerItem::GetCaption();

	CXTPCommandBar* pParent = GetParent();

	if (pParent && pParent->GetCommandBars()->IsAllCaps())
	{
		strCaption.MakeUpper();
	}

	return strCaption;
}

CXTPRibbonGroup* CXTPRibbonTab::FindGroup(int nID) const
{
	for (int i = 0; i < m_pGroups->GetCount(); i++)
	{
		if (m_pGroups->GetAt(i)->GetID() == nID)
			return m_pGroups->GetAt(i);
	}
	return NULL;
}

void CXTPRibbonTab::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		CXTPRibbonControlTab* pControlTab = ((CXTPRibbonControlTab*)pPX->m_pOwner);
		m_pRibbonBar  = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControlTab->GetParent());
		m_pTabManager = pControlTab;
		m_pParent	 = m_pRibbonBar;
	}

	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("ToolTip"), m_strToolTip, _T(""));
	PX_Enum(pPX, _T("ContextColor"), m_nContextColor, xtpRibbonTabContextColorNone);
	PX_String(pPX, _T("ContextCaption"), m_strContextCaption, _T(""));
	PX_Int(pPX, _T("Id"), m_nId, 0);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);

	if (pPX->IsStoring())
	{
		BOOL bSelected = IsSelected();
		PX_Bool(pPX, _T("Selected"), bSelected, FALSE);
	}
	else
	{
		BOOL bSelected = FALSE;
		PX_Bool(pPX, _T("Selected"), bSelected, 0);
		if (bSelected)
		{
			Select();
		}
	}

	if (_XTP_SCHEMA_1700 <= pPX->GetSchema())
	{
		PX_String(pPX, _T("CustomizeCaption"), m_strCustomizeCaption, _T(""));
	}

	CXTPPropExchangeSection secGroups(pPX->GetSection(_T("Groups")));
	m_pGroups->DoPropExchange(&secGroups);
}

BOOL CXTPRibbonTab::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_EVENT && nID == XTP_CN_REDRAWPARENT)
	{
		GetRibbonBar()->Redraw(0, FALSE);
		return TRUE;
	}
	return CXTPCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CXTPRibbonTab::SetContextTab(XTPRibbonTabContextColor color, int nID)
{
	m_nContextColor = color;
	if (nID != 0)
		VERIFY(m_strContextCaption.LoadString(XTPToUInt(nID)));
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonTab, CXTPTabManagerItem)
	DISP_PROPERTY_EX_ID(CXTPRibbonTab, "RibbonBar", 100, OleGetRibbonBar, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPRibbonTab, "Groups", 101, OleGetGroups, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPRibbonTab, "Id", 50, m_nId, VT_I4)
	DISP_PROPERTY_ID(CXTPRibbonTab, "Color", 51, m_nContextColor, VT_I4)
	DISP_PROPERTY_ID(CXTPRibbonTab, "ContextCaption", 52, m_strContextCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPRibbonTab, "CustomizeCaption", 54, m_strCustomizeCaption, VT_BSTR)
	DISP_FUNCTION_ID(CXTPRibbonTab, "GetRect", 110, OleGetRect, VT_EMPTY,
					 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_PROPERTY_ID(CXTPRibbonTab, "KeyboardTip", 53, m_strKeyboardTip, VT_BSTR)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonTab, XTPDIID_IRibbonTab)

BEGIN_INTERFACE_MAP(CXTPRibbonTab, CXTPTabManagerItem)
	INTERFACE_PART(CXTPRibbonTab, XTPDIID_IRibbonTab, Dispatch)
END_INTERFACE_MAP()

LPDISPATCH CXTPRibbonTab::OleGetRibbonBar()
{
	return GetRibbonBar()->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonTab::OleGetGroups()
{
	return GetGroups()->GetIDispatch(TRUE);
}

void CXTPRibbonTab::OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	if (pLeft == NULL || pTop == NULL || pRight == NULL || pBottom == NULL)
	{
		return;
	}
	if (!m_pParent || !m_pParent->m_hWnd)
		return;

	CRect rc(GetRect());

	m_pParent->OlePixelToUnits(rc);

	*pLeft   = rc.left;
	*pRight  = rc.right;
	*pTop	= rc.top;
	*pBottom = rc.bottom;
}

#endif
