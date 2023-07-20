// XTPRibbonGroup.cpp: implementation of the CXTPRibbonGroup class.
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
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollInfo.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "CommandBars/XTPControlEdit.h"
#include "CommandBars/XTPControlComboBox.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/Gallery/XTPControlGalleryItems.h"
#include "CommandBars/XTPCommandBarsIIDs.h"

#include "Ribbon/XTPRibbonGroup.h"
#include "Ribbon/XTPRibbonGroups.h"
#include "Ribbon/XTPRibbonPopups.h"
#include "Ribbon/XTPRibbonBar.h"
#include "Ribbon/XTPRibbonPaintManager.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#	define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CXTPRibbonGroup, CXTPCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonGroupOption
IMPLEMENT_XTP_CONTROL(CXTPRibbonGroupOption, CXTPControlButton)

CXTPRibbonGroupOption::CXTPRibbonGroupOption()
{
	SetFlags(xtpFlagNoMovable);
}

void CXTPRibbonGroupOption::Draw(CDC* pDC)
{
	if (m_pRibbonGroup)
	{
		m_pRibbonGroup->GetRibbonBar()->GetRibbonPaintManager()->DrawControlGroupOption(pDC, this,
																						TRUE);
	}
	else
	{
		CXTPControlButton::Draw(pDC);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonGroupControlPopup

CXTPRibbonGroupControlPopup::CXTPRibbonGroupControlPopup(CXTPRibbonGroup* pGroup)
{
	m_pRibbonGroup = pGroup;
	m_pPopupGroup  = pGroup;
	SetFlags(xtpFlagManualUpdate | xtpFlagNoMovable);
	m_controlType = xtpControlButtonPopup;
	m_pCommandBar = CXTPPopupBar::CreatePopupBar(0);
}

CSize CXTPRibbonGroupControlPopup::GetSize(CDC* pDC)
{
	return GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawControlGroupPopup(pDC, this,
																						  FALSE);
}

void CXTPRibbonGroupControlPopup::Draw(CDC* pDC)
{
	GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawControlGroupPopup(pDC, this, TRUE);
}

BOOL CXTPRibbonGroupControlPopup::IsTransparent() const
{
	return TRUE;
}

CXTPRibbonBar* CXTPRibbonGroupControlPopup::GetRibbonBar() const
{
	ASSERT(m_pParent);
	if (!m_pParent)
		return NULL;

	if (DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pParent))
		return (CXTPRibbonBar*)m_pParent;

	if (DYNAMIC_DOWNCAST(CXTPRibbonGroupPopupToolBar, m_pParent))
		return ((CXTPRibbonGroupPopupToolBar*)m_pParent)->m_pRibbonBar;

	if (DYNAMIC_DOWNCAST(CXTPRibbonTabPopupToolBar, m_pParent))
		return ((CXTPRibbonTabPopupToolBar*)m_pParent)->m_pRibbonBar;

	if (DYNAMIC_DOWNCAST(CXTPRibbonBarMorePopupToolBar, m_pParent))
		return ((CXTPRibbonBarMorePopupToolBar*)m_pParent)->m_pRibbonBar;

	ASSERT(FALSE);
	return NULL;
}

void CXTPRibbonGroupControlPopup::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	if (this != pControl)
	{
		CXTPControlButton::Copy(pControl, bRecursive);

		if (m_pParent && pControl->GetParent()
			&& GetRibbonBar() != ((CXTPRibbonGroupControlPopup*)pControl)->GetRibbonBar())
		{
			m_pPopupGroup = GetRibbonBar()->FindGroup(GetID());
		}
		else
		{
			m_pPopupGroup = ((CXTPRibbonGroupControlPopup*)pControl)->m_pPopupGroup;
		}
	}
}

void CXTPRibbonGroupControlPopup::GenerateCommandBarList(
	DWORD& /*nID*/, CXTPCommandBarList* /*pCommandBarList*/,
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM* /*pParam*/)
{
}

void CXTPRibbonGroupControlPopup::OnLButtonUp(CPoint /*point*/)
{
}

BOOL CXTPRibbonGroupControlPopup::IsVisible(DWORD dwSkipFlags /*= 0*/) const
{
	return CXTPControlPopup::IsVisible(dwSkipFlags)
		   && (!m_pPopupGroup || m_pPopupGroup->m_bVisible);
}

IMPLEMENT_XTP_CONTROL(CXTPRibbonGroupControlPopup, CXTPControlPopup)

BOOL CXTPRibbonGroupControlPopup::OnSetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
	if (bPopup)
	{
		CMDTARGET_RELEASE(m_pCommandBar);

		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);

		if (!m_pPopupGroup)
		{
			m_pCommandBar = new CXTPPopupToolBar();
			m_pCommandBar->SetCommandBars(pCommandBars);
			m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected()));
		}
		else
		{
			m_pCommandBar = new CXTPRibbonGroupPopupToolBar(m_pPopupGroup);
			m_pCommandBar->SetCommandBars(pCommandBars);

			m_pCommandBar->EnableCustomization(FALSE);
			m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected()));
		}
	}
	else
	{
		ASSERT(m_pCommandBar);

		if (m_pCommandBar)
		{
			m_pCommandBar->SetTrackingMode(FALSE);

			if (m_pPopupGroup && m_pPopupGroup->GetParent())
			{
				// we need to copy the popped ribbon group to the existing (in order not to lose
				// user's changes)

				CXTPRibbonGroupPopupToolBar* pPopupToolbar =
					DYNAMIC_DOWNCAST(CXTPRibbonGroupPopupToolBar, m_pCommandBar);

				if (pPopupToolbar && pPopupToolbar->m_pRibbonGroup)
				{
					CXTPControl* pSelectedControl = m_pPopupGroup->GetParent()->GetSelected();
					int index = pSelectedControl ? pSelectedControl->GetIndex() : 0;

					m_pPopupGroup->RemoveAll();
					m_pPopupGroup->Copy(pPopupToolbar->m_pRibbonGroup);

					m_pPopupGroup->GetParent()->SetSelected(index);
				}
			}
		}
	}

	RedrawParent();
	return TRUE;
}

CXTPRibbonGroup::CXTPRibbonGroup()
{
	m_nIndex	 = -1;
	m_pGroups	= NULL;
	m_pParent	= NULL;
	m_pRibbonBar = NULL;
	m_rcGroup.SetRectEmpty();
	m_nId				 = -1;
	m_nIconId			 = -1;
	m_bReduced			 = FALSE;
	m_nAllowReduce		 = 4;
	m_bVisible			 = TRUE;
	m_bControlsCentering = FALSE;

	m_bAutoArrangeEditCaption = TRUE;

#ifdef _XTP_COMMANDBARS_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_pControlGroupOption				  = new CXTPRibbonGroupOption();
	m_pControlGroupOption->m_pRibbonGroup = this;

	m_pControlGroupPopup = new CXTPRibbonGroupControlPopup(this);

	m_pControlGroupPopup->m_buttonRibbonStyle = xtpButtonIconAndCaptionBelow;

	m_bShowOptionButton = FALSE;

	m_pLayoutInfo = NULL;

	m_nRowCount			= 2;
	m_bControlsGrouping = FALSE;
}

CXTPRibbonGroup::~CXTPRibbonGroup()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pControlGroupOption);
	CMDTARGET_RELEASE(m_pControlGroupPopup);
}

void CXTPRibbonGroup::SetID(int nId)
{
	m_nId = nId;
	m_pControlGroupOption->SetID(m_nId);
	m_pControlGroupPopup->SetID(m_nId);
}

void CXTPRibbonGroup::SetCaption(LPCTSTR lpszCaption)
{
	if (m_strCaption != lpszCaption)
	{
		m_strCaption = lpszCaption;
		if (m_pParent)
			m_pParent->DelayLayout();

		CXTPControlPopup* pGroupPopup = GetControlGroupPopup();
		if (pGroupPopup)
		{
			pGroupPopup->SetCaption(GetCaption());

			CXTPControlAction* pAction = pGroupPopup->GetAction();
			if (pAction)
				pAction->SetCaption(GetCaption());
		}
	}
}

void CXTPRibbonGroup::Copy(CXTPRibbonGroup* pGroup)
{
	if (this != pGroup)
	{
		m_nId					  = pGroup->m_nId;
		m_nIconId				  = pGroup->m_nIconId;
		m_bShowOptionButton		  = pGroup->m_bShowOptionButton;
		m_strCaption			  = pGroup->m_strCaption;
		m_pGroups				  = pGroup->m_pGroups;
		m_bControlsGrouping		  = pGroup->m_bControlsGrouping;
		m_bControlsCentering	  = pGroup->m_bControlsCentering;
		m_bAutoArrangeEditCaption = pGroup->m_bAutoArrangeEditCaption;
		m_bVisible				  = pGroup->m_bVisible;
		m_nAllowReduce			  = pGroup->m_nAllowReduce;

		int nControlPos = m_pControlGroupOption->GetIndex();

		for (int i = 0; i < pGroup->GetCount(); i++)
		{
			CXTPControl* pControl = pGroup->GetAt(i);

			CXTPControl* pClone	= m_pParent->GetControls()->AddClone(pControl, nControlPos + i);
			pClone->m_pRibbonGroup = this;
			pClone->SetHideFlag(xtpHideRibbonTab, FALSE);
			m_arrControls.Add(pClone);
			pClone->InternalAddRef();
		}

		m_pControlGroupOption->Copy(pGroup->m_pControlGroupOption, FALSE);
		m_pControlGroupOption->SetHideFlag(xtpHideRibbonTab, FALSE);

		m_pControlGroupPopup->Copy(pGroup->m_pControlGroupPopup, FALSE);
		m_pControlGroupPopup->SetHideFlag(xtpHideRibbonTab, FALSE);
	}
}

void CXTPRibbonGroup::Remove(CXTPControl* pControl)
{
	for (int nIndex = 0; nIndex < m_arrControls.GetSize(); nIndex++)
	{
		if (m_arrControls.GetAt(nIndex) == pControl)
		{
			RemoveAt(nIndex);
			return;
		}
	}
}

CXTPControl* CXTPRibbonGroup::FindControl(int nId) const
{
	return FindControl(xtpControlError, nId, FALSE, FALSE);
}

CXTPControl* CXTPRibbonGroup::FindControl(XTPControlType type, int nId, BOOL bVisible,
										  BOOL bRecursive) const
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = GetAt(nIndex);
		if ((type == xtpControlError || pControl->GetType() == type)
			&& (!bVisible || pControl->IsVisible()) && (nId == -1 || nId == pControl->GetID()))
			return pControl;

		if (bRecursive)
		{
			CXTPCommandBar* pBar = pControl->GetCommandBar();
			if (pBar)
			{
				pControl = pBar->GetControls()->FindControl(type, nId, bVisible, bRecursive);
				if (pControl != NULL)
					return pControl;
			}
		}
	}
	return NULL;
}

CXTPControl* CXTPRibbonGroup::SetControlType(CXTPControl* pControl, XTPControlType type)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (IsPopupControlType(pControl->GetType()) && IsPopupControlType(type))
	{
		pControl->m_controlType = type;
		return pControl;
	}

	CXTPControl* pNew = Add(type, 0, _T(""), IndexOf(pControl) + 1, FALSE);
	pNew->CXTPControl::Copy(pControl, FALSE);
	pNew->m_controlType = type;

	Remove(pControl);

	return pNew;
}
void CXTPRibbonGroup::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPControl* pControl	= m_arrControls.GetAt(nIndex);
		pControl->m_pRibbonGroup = NULL;
		m_arrControls.RemoveAt(nIndex);

		GetRibbonBar()->GetControls()->Remove(pControl);
		pControl->InternalRelease();
	}
}

void CXTPRibbonGroup::RemoveAll()
{
	for (int nIndex = (int)m_arrControls.GetSize() - 1; nIndex >= 0; nIndex--)
	{
		CXTPControl* pControl	= m_arrControls.GetAt(nIndex);
		pControl->m_pRibbonGroup = NULL;
		m_arrControls.RemoveAt(nIndex);

		m_pParent->GetControls()->Remove(pControl);
		pControl->InternalRelease();
	}
}

CXTPRibbonTab* CXTPRibbonGroup::GetParentTab() const
{
	return m_pGroups->GetParentTab();
}

BOOL CXTPRibbonGroup::IsHighlighted() const
{
	CXTPRibbonScrollableBar* pScrollableBar = GetScrollableBar(m_pParent);

	return pScrollableBar && pScrollableBar->GetHighlightedGroup() == this;
}

int CXTPRibbonGroup::GetMinimumWidth(CDC* pDC)
{
	CXTPFontDC fnt(pDC, &GetRibbonBar()->GetRibbonPaintManager()->m_fontGroupCaption);
	CSize sz = pDC->GetTextExtent(m_strCaption);

	if (m_bShowOptionButton)
		sz.cx += GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();

	return sz.cx;
}

BOOL CXTPRibbonGroup::IsOptionButtonVisible() const
{
	return !m_bReduced && m_bShowOptionButton;
}

void CXTPRibbonGroup::SetRect(CRect rc)
{
	m_rcGroup = rc;

	if (m_bReduced)
	{
		CRect rcEntry(rc.left, rc.top, rc.right, rc.bottom);
		m_pControlGroupPopup->SetHideFlag(xtpHideWrap, FALSE);
		m_pControlGroupPopup->SetRect(rcEntry);
		m_pControlGroupPopup->SetWrap(TRUE);
	}
	else
	{
		m_pControlGroupPopup->SetHideFlag(xtpHideWrap, TRUE);
	}

	if (!m_bShowOptionButton)
	{
		m_pControlGroupOption->SetHideFlag(xtpHideGeneric, TRUE);
	}
	else if (m_bReduced)
	{
		m_pControlGroupOption->SetHideFlag(xtpHideWrap, TRUE);
	}
	else
	{
		int nHeight = GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();
		m_pControlGroupOption->SetWrap(TRUE);
		m_pControlGroupOption->SetHideFlag(xtpHideGeneric, FALSE);
		m_pControlGroupOption->SetHideFlag(xtpHideWrap, FALSE);

		// It's goog idea to refactor option group rectangle caclulation
		// to an actual implementation of paint manager later.
		CSize szGroupOption = GetRibbonBar()->GetRibbonPaintManager()->GetControlGroupOptionSize();
		CRect rcGroupOption = rc;
		rcGroupOption.right -= XTP_DPI_X(2);
		rcGroupOption.bottom -= XTP_DPI_Y(2);
		rcGroupOption.left = rcGroupOption.right - nHeight;
		rcGroupOption.top  = rcGroupOption.bottom - nHeight;
		rcGroupOption.left += rcGroupOption.Width() / 2 - szGroupOption.cx / 2;
		rcGroupOption.top += rcGroupOption.Height() / 2 - szGroupOption.cy / 2;
		m_pControlGroupOption->SetRect(rcGroupOption);
	}
}

void CXTPRibbonGroup::RepositionControls(CDC* pDC)
{
	CXTPRibbonPaintManager* pPaintManager = GetRibbonBar()->GetRibbonPaintManager();

	int x = m_rcGroup.left + XTP_DPI_X(4);
	int y = m_rcGroup.top + pPaintManager->GetGroupCaptionHeight();

	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControl* pControl = GetAt(i);
		if (!pControl->IsVisible())
			continue;

		CSize sz = pControl->GetSize(pDC);
		pControl->SetRect(CRect(x, y, x + sz.cx, y + sz.cy));

		x += sz.cx;
	}
}

CRect CXTPRibbonGroup::GetCaptionRect() const
{
	CRect rcCaption(m_rcGroup);
	rcCaption.top = rcCaption.bottom
					- GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();
	return rcCaption;
}

void CXTPRibbonGroup::Draw(CDC* pDC, CRect rcClipBox)
{
	GetRibbonBar()->GetRibbonPaintManager()->DrawGroup(pDC, this);

	CXTPControl* pPreviousControl = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControl* pControl = GetAt(i);
		if (!pControl->IsVisible())
			continue;

		if (pControl->GetBeginGroup() && pPreviousControl != NULL)
		{
			CXTPPaintManager* pPaintManager = GetRibbonBar()->GetPaintManager();
			if (!m_bControlsGrouping)
			{
				pPaintManager->DrawCommandBarSeparator(pDC, GetRibbonBar(), pControl);
			}
			else if (!pControl->GetWrap())
			{
				pPaintManager->DrawCommandBarGroupSeparator(pDC, GetRibbonBar(), pPreviousControl,
															pControl);
			}
		}

		if (CRect().IntersectRect(rcClipBox, pControl->GetRect()))
		{
			pControl->Draw(pDC);
		}

		pPreviousControl = pControl;
	}

	if (m_pControlGroupPopup->IsVisible())
	{
		m_pControlGroupPopup->Draw(pDC);
	}

	if (m_pControlGroupOption->IsVisible())
	{
		m_pControlGroupOption->Draw(pDC);
	}
}

int CXTPRibbonGroup::IndexOf(CXTPControl* pControl) const
{
	if (pControl->GetRibbonGroup() != this)
		return -1;

	return pControl->GetIndex() + GetCount() - m_pControlGroupOption->GetIndex();
}

CXTPControl* CXTPRibbonGroup::AddClone(CXTPControl* pClone, int nBefore, BOOL bRecursive)
{
	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	CXTPControl* pControl = (CXTPControl*)pClone->GetRuntimeClass()->CreateObject();
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->InsertAt(pControl, nControlPos);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->Copy(pClone, bRecursive);

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

CXTPControl* CXTPRibbonGroup::Add(XTPControlType controlType, int nId, LPCTSTR lpszParameter,
								  int nBefore, BOOL bTemporary)
{
	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	CXTPControl* pControl = GetRibbonBar()->GetControls()->Add(controlType, nId, lpszParameter,
															   nControlPos, bTemporary);
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

void CXTPRibbonGroup::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;

		GetRibbonBar()->RebuildControls(GetRibbonBar()->GetSelectedTab());
		GetRibbonBar()->OnRecalcLayout();
	}
}

BOOL CXTPRibbonGroup::IsVisible() const
{
	if (m_pRibbonBar != m_pParent)
		return m_bVisible;

	return GetRibbonBar()->GetSelectedTab() == GetParentTab() && m_bVisible;
}

void CXTPRibbonGroup::OnGroupAdded()
{
	CXTPControl* pGroupOption = GetControlGroupOption();

	m_pParent->GetControls()->Add(pGroupOption);
	pGroupOption->InternalAddRef();

	pGroupOption->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	pGroupOption->SetID(GetID());

	CXTPControl* pGroupPopup = (CXTPControl*)GetControlGroupPopup();
	m_pParent->GetControls()->Add(pGroupPopup);
	pGroupPopup->SetCaption(GetCaption());
	pGroupPopup->SetDescription(NULL);

	pGroupPopup->InternalAddRef();

	pGroupPopup->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	CXTPCommandBars* pCommandBars = m_pRibbonBar->GetCommandBars();
	ASSERT(pCommandBars);

	if (pCommandBars && pCommandBars->IsActionsEnabled())
	{
		CXTPControlAction* pAction = pCommandBars->CreateAction(GetID());
		if (pAction->GetCaption().IsEmpty())
			pAction->SetCaption(GetCaption());

		pGroupPopup->SetAction(pAction);
		pGroupOption->SetAction(pAction);
	}
}

void CXTPRibbonGroup::OnGroupRemoved()
{
	RemoveAll();

	if (m_pParent == m_pRibbonBar)
	{
		GetControlGroupPopup()->m_pRibbonGroup = NULL;
		m_pParent->GetControls()->Remove(GetControlGroupPopup());

		if (m_pRibbonBar->m_pHighlightedGroup == this)
		{
			m_pRibbonBar->m_pHighlightedGroup = NULL;
		}
	}

	GetControlGroupOption()->m_pRibbonGroup = NULL;
	m_pParent->GetControls()->Remove(GetControlGroupOption());
}

BOOL CXTPRibbonGroup::SetButtons(UINT* pButtons, int nCount)
{
	BOOL bSeparator = FALSE;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();
	CWnd* pSite				  = pRibbonBar->GetSite();

	for (int i = 0; i < nCount; i++)
	{
		if (pButtons[i] == 0)
			bSeparator = TRUE;
		else
		{
			XTPControlType controlType = xtpControlButton;
			XTPButtonStyle buttonStyle = xtpButtonAutomatic;
			CXTPControl* pControl	  = NULL;
			UINT nID				   = pButtons[i];

			XTP_COMMANDBARS_CREATECONTROL cs;

			if (pSite)
			{
				cs.nID		   = nID;
				cs.pControl	= NULL;
				cs.bToolBar	= TRUE;
				cs.pMenu	   = NULL;
				cs.nIndex	  = i;
				cs.strCaption  = pRibbonBar->GetTitle();
				cs.controlType = controlType;
				cs.pCommandBar = pRibbonBar;
				cs.buttonStyle = buttonStyle;

				if (pSite->SendMessage(WM_XTP_BEFORECREATECONTROL, 0, (LPARAM)&cs) != 0)
				{
					pControl	= cs.pControl;
					controlType = cs.controlType;
					buttonStyle = cs.buttonStyle;
					nID			= cs.nID;
				}
			}

			int nControlPos = m_pControlGroupOption->GetIndex();

			if (pControl == NULL)
			{
				pControl = pRibbonBar->GetControls()->Add(controlType, XTPToInt(nID), NULL,
														  nControlPos);
				if (pControl)
				{
					pControl->SetStyle(buttonStyle);
					if (controlType == xtpControlPopup)
						pControl->SetIconId(XTPToInt(nID));
				}
			}
			else
				pRibbonBar->GetControls()->Add(pControl, XTPToInt(nID), NULL, nControlPos);

			if (!pControl)
				continue;

			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}

			m_arrControls.InsertAt(GetCount(), pControl);
			pControl->InternalAddRef();

			pControl->m_pRibbonGroup = this;
			pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

			if (pSite)
			{
				cs.pControl = pControl;
				pSite->SendMessage(WM_XTP_AFTERCREATECONTROL, 0, (LPARAM)&cs);
			}
		}
	}
	return TRUE;
}

CXTPControl* CXTPRibbonGroup::Add(CXTPControl* pControl, int nId, LPCTSTR lpszParameter,
								  int nBefore, BOOL bTemporary)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->Add(pControl, nId, lpszParameter, nControlPos, bTemporary);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

CXTPControl* CXTPRibbonGroup::InsertAt(CXTPControl* pControl, int nBefore)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->InsertAt(pControl, nControlPos);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

BOOL CXTPRibbonGroup::LoadToolBar(UINT nIDResource, BOOL bLoadIcons)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
		{
			return (WORD*)(this + 1);
		}
	};

	ASSERT(this);
	ASSERT(lpszResourceName != NULL);
	if (!lpszResourceName)
		return FALSE;

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	if (!hInst)
		return FALSE;

	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	ASSERT(pData->wVersion == 1);

	int i;
	UINT* pItems = new UINT[pData->wItemCount];

	for (i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];

	BOOL bResult = SetButtons(pItems, pData->wItemCount);

	if (bLoadIcons)
	{
		CXTPImageManager* pImageManager = GetRibbonBar()->GetImageManager();

		if (!pImageManager->SetIcons(nIDResource, pItems, pData->wItemCount,
									 CSize(pData->wWidth, pData->wHeight)))
			bResult = FALSE;
	}

	delete[] pItems;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;
}

BOOL CXTPRibbonGroup::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_EVENT && nID == XTP_CN_REDRAWPARENT)
	{
		GetRibbonBar()->Redraw(0, FALSE);
		return TRUE;
	}
	return CXTPCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

struct CXTPRibbonGroup::CONTROLINFO
{
	CRect rcControl;
	CRect rcRow;
	CSize szControl;
	BOOL bHide;
	BOOL bWrap;
	BOOL bBeginGroup;
	BOOL bWrapGroup;
	XTPButtonStyle buttonStyle;
	CXTPControl* pControl;
	int nTailWidth;

	void EnsureButtonSize(CDC* pDC)
	{
		if (buttonStyle != pControl->GetStyle())
		{
			buttonStyle = pControl->GetStyle();
			szControl   = pControl->GetSize(pDC);
		}
	}
	void Attach(CDC* pDC, CXTPControl* p)
	{
		pControl = p;
		rcRow.SetRectEmpty();

		szControl   = pControl->GetSize(pDC);
		rcControl   = pControl->GetRect();
		bBeginGroup = pControl->GetBeginGroup();
		bWrapGroup  = bBeginGroup || (pControl->GetFlags() & xtpFlagWrapRow);
		buttonStyle = pControl->GetStyle();
		bWrap = bHide = FALSE;
		nTailWidth	= 0;
	}

	void Detach()
	{
		pControl->SetHideWrap(bHide);
		pControl->SetRowRect(rcRow);
		if (!bHide)
			pControl->SetRect(rcControl);
		pControl->SetWrap(bWrap);
	}
};

struct CXTPRibbonGroup::LAYOUTINFO
{
	CONTROLINFO* pControls;
	int nControlCount;
	int nMinWidth;
	int nWidth;
	BOOL bOldReduced;
};

BOOL CXTPRibbonGroup::OnExtendSize(int nWidthAvail)
{
	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	for (int i = nDataCount - 1; i >= 0; i--)
	{
		if (pData[i].pControl->GetType() == xtpControlGallery
			&& ((CXTPControlGallery*)pData[i].pControl)->IsResizable() & 1)
		{
			CXTPControlGallery* pGallery = (CXTPControlGallery*)pData[i].pControl;
			int cx = pGallery->GetItems() ? pGallery->GetItems()->GetItemSize().cx : 0;

			if (cx > 0 && cx <= nWidthAvail)
			{
				int nCount		= pGallery->GetItems()->GetItemCount();
				CRect rcBorders = pGallery->GetBorders();
				int nMaxWidth   = nCount * cx + rcBorders.left + rcBorders.right;

				if (pData[i].szControl.cx + cx <= nMaxWidth)
				{
					pData[i].szControl.cx = min(nMaxWidth,
												pData[i].szControl.cx + int(nWidthAvail / cx) * cx);

					return TRUE;
				}
			}

			return FALSE;
		}
	}

	return FALSE;
}

BOOL CXTPRibbonGroup::OnReduceSize(int nLevel, int nWidthAvail)
{
	if (m_pLayoutInfo == NULL)
		return FALSE;

	if (m_pLayoutInfo->nWidth < m_pLayoutInfo->nMinWidth)
	{
		if (nLevel == 2 && m_pLayoutInfo->nWidth > XTP_DPI_X(16))
		{
			m_pLayoutInfo->nMinWidth = max(m_pLayoutInfo->nWidth,
										   m_pLayoutInfo->nMinWidth - nWidthAvail);
			return TRUE;
		}

		return FALSE;
	}

	BOOL bReduced = FALSE;

	int nResultHeight = GetRibbonBar()->CalcGroupsHeight() - XTP_DPI_Y(2)
						- GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();

	int nButtonSize = GetRibbonBar()->GetButtonSize().cy;

	int nControlsPerGroup = int(nResultHeight / nButtonSize);

	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	bReduced = FALSE;

	int* pButtons = new int[XTPToUIntChecked(nControlsPerGroup)];

	if (nLevel == 0 && !m_bControlsGrouping && m_nAllowReduce > nLevel)
	{
		if (nControlsPerGroup > 1)
		{
			for (int i = nDataCount - 1; i >= 2; i--)
			{
				if (pData[i].bWrapGroup)
					continue;

				int nItems = 0;

				for (int j = i; j >= 0; j--)
				{
					CONTROLINFO* pControl = &pData[j];

					if ((nItems == nControlsPerGroup - 1 || !pControl->bWrapGroup)
						&& pControl->pControl->m_buttonRibbonStyle == xtpButtonIconAndCaptionBelow
						&& pControl->pControl->GetType() != xtpControlGallery)
					{
						pButtons[nItems] = j;
						nItems += 1;
					}
					else
					{
						break;
					}
					if (nItems == nControlsPerGroup)
						break;
				}

				if (nItems == nControlsPerGroup)
				{
					for (int k = 0; k < nItems; k++)
						pData[pButtons[k]].pControl->m_buttonRibbonStyle = xtpButtonIconAndCaption;
					bReduced = TRUE;
					break;
				}
			}
		}
	}

	if (nLevel == 1 && !m_bControlsGrouping && m_nAllowReduce > nLevel)
	{
		if (nControlsPerGroup > 1)
		{
			for (int i = nDataCount - 1; i >= 2; i--)
			{
				if (pData[i].bWrapGroup)
					continue;

				int nItems  = 0;
				BOOL bFound = FALSE;
				int j;

				for (j = i; j >= 0; j--)
				{
					CONTROLINFO* pControl = &pData[j];

					if ((nItems == nControlsPerGroup - 1 || !pControl->bWrapGroup)
						&& pControl->pControl->GetType() != xtpControlGallery)
					{
						pButtons[nItems] = j;
						nItems += 1;
						bFound = bFound
								 || pControl->pControl->m_buttonRibbonStyle
										== xtpButtonIconAndCaptionBelow;
					}
					else
					{
						break;
					}
					if (nItems == nControlsPerGroup)
						break;
				}
				i = j;
				if (nItems == nControlsPerGroup && bFound)
				{
					for (int k = 0; k < nItems; k++)
						pData[pButtons[k]].pControl->m_buttonRibbonStyle = xtpButtonIconAndCaption;

					bReduced = TRUE;
					break;
				}
			}
		}
	}

	if (nLevel == 1 && m_bControlsGrouping && m_nRowCount == 2 && m_nAllowReduce > nLevel)
	{
		m_nRowCount = 3;
		bReduced	= TRUE;
	}

	if (nLevel == 2 && !bReduced && !m_bControlsGrouping && m_nAllowReduce > nLevel)
	{
		if (nControlsPerGroup > 1)
		{
			for (int i = nDataCount - 1; i >= 2; i--)
			{
				if (pData[i].bWrapGroup)
					continue;

				int nItems = 0;
				int nLeft  = 0;

				for (int j = i; j >= 0; j--)
				{
					CONTROLINFO* pControl = &pData[j];

					int nOffset = pControl->rcControl.left;
					if (nItems != 0 && (nOffset != nLeft))
						break;

					if ((nItems == nControlsPerGroup - 1 || !pControl->bWrapGroup)
						&& pControl->pControl->m_buttonRibbonStyle == xtpButtonIconAndCaption
						&& pControl->pControl->GetType() != xtpControlGallery)
					{
						pButtons[nItems] = j;
						nItems += 1;
						nLeft = nOffset;
					}
					else
					{
						break;
					}
					if (nItems == nControlsPerGroup)
						break;
				}

				if (nItems == nControlsPerGroup && GetCount() > nControlsPerGroup)
				{
					for (int k = 0; k < nItems; k++)
						pData[pButtons[k]].pControl->m_buttonRibbonStyle = xtpButtonIcon;
					bReduced = TRUE;
					break;
				}
			}
		}
	}

	if (nLevel == 2 && !bReduced && !m_bControlsGrouping && m_nAllowReduce > nLevel)
	{
		for (int i = nDataCount - 1; i >= 0; i--)
		{
			if (pData[i].pControl->GetType() == xtpControlGallery
				&& pData[i].pControl->m_buttonRibbonStyle != xtpButtonIconAndCaptionBelow)
			{
				pData[i].pControl->m_buttonRibbonStyle = xtpButtonIconAndCaptionBelow;
				bReduced							   = TRUE;
				break;
			}
		}
	}

	delete[] pButtons;

	if (nLevel == 3)
	{
		if (nDataCount > 1 && !m_bReduced && m_nAllowReduce > nLevel)
		{
			m_bReduced = TRUE;

			for (int j = 0; j < nDataCount; j++)
			{
				CONTROLINFO* pControl = &pData[j];
				pControl->bHide		  = TRUE;
			}

			bReduced = TRUE;
		}
	}

	return bReduced;
}

void CXTPRibbonGroup::OnBeforeCalcSize(CDC* pDC)
{
	CSize szLargeIcon = GetRibbonBar()->GetLargeIconSize(FALSE);

	ASSERT(m_pLayoutInfo == NULL);
	m_pLayoutInfo			   = new LAYOUTINFO;
	m_pLayoutInfo->pControls   = NULL;
	m_pLayoutInfo->nMinWidth   = GetMinimumWidth(pDC);
	m_pLayoutInfo->bOldReduced = m_bReduced;

	int nVisibleCount = 0;
	int j;

	for (j = 0; j < GetCount(); j++)
	{
		CXTPControl* pControl = GetAt(j);

		if (pControl->IsVisible(xtpHideWrap))
		{
			nVisibleCount++;
		}
	}

	m_pLayoutInfo->nControlCount = nVisibleCount;

	if (nVisibleCount == 0)
		return;

	m_pLayoutInfo->pControls = new CONTROLINFO[XTPToUIntChecked(nVisibleCount)];

	int k = 0;

	for (j = 0; j < GetCount(); j++)
	{
		CXTPControl* pControl = GetAt(j);

		if (pControl->IsVisible(xtpHideWrap))
		{
			pControl->m_buttonRibbonStyle = xtpButtonAutomatic;

			if (!m_bControlsGrouping && pControl->GetStyle() == xtpButtonAutomatic
				&& pControl->GetType() != xtpControlGallery)
			{
				BOOL bDrawImage = pControl->GetIconId() > 0
									  ? pControl->GetImageManager()->IsPrimaryImageExists(
											XTPToUInt(pControl->GetIconId()), szLargeIcon.cx)
											!= NULL
									  : FALSE;

				pControl->m_buttonRibbonStyle = bDrawImage ? xtpButtonIconAndCaptionBelow
														   : xtpButtonIconAndCaption;

				CXTPControlButton* pControlButton = DYNAMIC_DOWNCAST(CXTPControlButton, pControl);
				if (pControlButton)
				{
					CXTPImageManagerIcon* pIcon = NULL;
					pIcon						= pControlButton->GetImageManager()->GetImage(
						  XTPToUInt(pControl->GetIconId()));

					if (pIcon && pIcon->IsVectorIcon()
						&& pControlButton->GetStyleEx() != xtpButtonAutomatic)
						pControl->m_buttonRibbonStyle = pControlButton->GetStyleEx();
				}
			}

			m_pLayoutInfo->pControls[k].Attach(pDC, pControl);
			k++;
		}
	}

	ASSERT(k == nVisibleCount);

	m_bReduced  = FALSE;
	m_nRowCount = 2;
}

void CXTPRibbonGroup::ArrangeEditCaption()
{
	CArray<CXTPControl*, CXTPControl*> arrColumn;

	int i = 0, j = 0, k;
	while (i < m_pLayoutInfo->nControlCount)
	{
		arrColumn.RemoveAll();

		for (j = i; j < m_pLayoutInfo->nControlCount; j++)
		{
			if (m_pLayoutInfo->pControls[j].rcControl.left
					!= m_pLayoutInfo->pControls[i].rcControl.left
				|| m_pLayoutInfo->pControls[j].rcControl.right
					   != m_pLayoutInfo->pControls[i].rcControl.right)
			{
				break;
			}

			if (m_pLayoutInfo->pControls[j].pControl->GetType() == xtpControlEdit
				|| m_pLayoutInfo->pControls[j].pControl->GetType() == xtpControlComboBox)
			{
				arrColumn.Add(m_pLayoutInfo->pControls[j].pControl);
			}
		}

		if (arrColumn.GetSize() > 1)
		{
			int nLabelWidth = 0;

			for (k = 0; k < (int)arrColumn.GetSize(); k++)
			{
				CXTPControl* pControl = arrColumn[k];

				if (pControl->GetType() == xtpControlEdit)
				{
					nLabelWidth = max(nLabelWidth, ((CXTPControlEdit*)pControl)->GetLabelWidth());
				}
				else
				{
					nLabelWidth = max(nLabelWidth,
									  ((CXTPControlComboBox*)pControl)->GetLabelWidth());
				}
			}

			for (k = 0; k < (int)arrColumn.GetSize(); k++)
			{
				CXTPControl* pControl = arrColumn[k];

				if (pControl->GetType() == xtpControlEdit)
				{
					((CXTPControlEdit*)pControl)->SetLabelWidth(nLabelWidth);
				}
				else
				{
					((CXTPControlComboBox*)pControl)->SetLabelWidth(nLabelWidth);
				}
			}
		}

		i = j;
	}
}

void CXTPRibbonGroup::OnAfterCalcSize()
{
	if (!m_pLayoutInfo)
		return;

	if (!m_bReduced && m_bAutoArrangeEditCaption && !m_bControlsGrouping)
		ArrangeEditCaption();

	for (int i = 0; i < m_pLayoutInfo->nControlCount; i++)
	{
		if (i == 0)
			m_pLayoutInfo->pControls[i].bWrap = TRUE;

		m_pLayoutInfo->pControls[i].Detach();
	}

	if (m_pLayoutInfo->bOldReduced && !m_bReduced)
	{
		GetRibbonBar()->m_bGroupReducedChanged = TRUE;
	}

	delete[] m_pLayoutInfo->pControls;
	delete m_pLayoutInfo;

	m_pLayoutInfo = NULL;
}

int CXTPRibbonGroup::_WrapSpecialDynamicSize(int nWidth, BOOL bIncludeTail)
{
	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	int nWraps = 0, x = 0;

	for (int i = 0; i < nDataCount; i++)
	{
		CONTROLINFO* pControl = &pData[i];

		pControl->bWrap  = FALSE;
		CSize szControl  = pControl->szControl;
		BOOL bBeginGroup = pControl->bBeginGroup && i > 0;
		BOOL bWrapGroup  = pControl->bWrapGroup && i > 0;

		if (bWrapGroup && (pControl->pControl->GetFlags() & xtpFlagWrapRow))
		{
			x				= szControl.cx;
			pControl->bWrap = TRUE;
			nWraps++;
			continue;
		}
		else if (bWrapGroup)
		{
			if ((bIncludeTail && (x + pControl->nTailWidth >= nWidth))
				|| (!bIncludeTail && nWraps == 0 && (x + pControl->nTailWidth >= nWidth)))
			{
				x				= szControl.cx;
				pControl->bWrap = TRUE;
				nWraps++;
				continue;
			}
		}

		if (bBeginGroup)
			x += XTP_DPI_X(3);

		x += szControl.cx;
	}
	return nWraps;
}

int CXTPRibbonGroup::_GetSizeSpecialDynamicSize()
{
	int nTotalWidth = 0, x = 0, y = 0, nRowHeight = 0;

	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	for (int i = 0; i < nDataCount; i++)
	{
		CONTROLINFO* pControl = &pData[i];

		CSize szControl = pControl->szControl;

		if (pControl->bWrap)
		{
			x = 0;
			y += nRowHeight;
		}
		else
		{
			BOOL bBeginGroup = pControl->bBeginGroup && i > 0;

			if (bBeginGroup)
				x += XTP_DPI_X(3);
		}

		pControl->rcControl.SetRect(XTP_DPI_X(2) + x, y, XTP_DPI_X(2) + x + szControl.cx,
									y + szControl.cy);

		x += szControl.cx;
		nRowHeight  = max(nRowHeight, szControl.cy);
		nTotalWidth = max(nTotalWidth, x);
	}
	return nTotalWidth + XTP_DPI_X(4);
}

BOOL CXTPRibbonGroup::_FindBestWrapSpecialDynamicSize()
{
	int nMax = _GetSizeSpecialDynamicSize();
	int nMin = 0, nMid = 0;

	int nCurrent = _WrapSpecialDynamicSize(nMin, TRUE);
	int nTarget  = 2;
	if (nCurrent < nTarget)
		return FALSE;

	if (nCurrent == nTarget)
		return TRUE;

	while (nMin < nMax)
	{
		nMid	 = (nMin + nMax) / 2;
		nCurrent = _WrapSpecialDynamicSize(nMid, TRUE);

		if (nCurrent <= nTarget)
		{
			nMax = nMid;
		}
		else
		{
			if (nMin == nMid)
			{
				_WrapSpecialDynamicSize(nMax, TRUE);
				break;
			}
			nMin = nMid;
		}
	}

	return TRUE;
}

int CXTPRibbonGroup::_CalcSpecialDynamicSize(CDC* /*pDC*/)
{
	int nResultHeight = GetRibbonBar()->CalcGroupsHeight() - XTP_DPI_Y(9)
						- GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();

	int i;

	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	int nTailWidth = 0;
	for (i = nDataCount - 1; i >= 0; i--)
	{
		CONTROLINFO* pControl = &pData[i];

		CSize szControl = pControl->szControl;

		nTailWidth += szControl.cx;
		pControl->nTailWidth = nTailWidth;

		if (pControl->bWrapGroup)
		{
			nTailWidth = 0;
		}
	}

	int nBaseHeight = GetRibbonBar()->GetRibbonPaintManager()->GetEditHeight();

	int nRowCount   = _WrapSpecialDynamicSize(32000, FALSE) + 1;
	int nTotalWidth = 0;

	if (nRowCount == 1)
	{
		nRowCount   = 3;
		nTotalWidth = _GetSizeSpecialDynamicSize();

		BOOL bAllow3Row = nResultHeight >= nBaseHeight * 3;
		BOOL bAllow2Row = nResultHeight >= nBaseHeight * 2;

		if (!bAllow2Row)
		{
			_WrapSpecialDynamicSize(32000, FALSE);
			nRowCount = 1;
		}
		else
		{
			if (!(bAllow3Row && m_nRowCount == 3 && _FindBestWrapSpecialDynamicSize()))
			{
				nRowCount = _WrapSpecialDynamicSize(nTotalWidth / 2, FALSE) + 1;
			}
		}
	}

	nTotalWidth = _GetSizeSpecialDynamicSize();

	int nOffset		 = (nResultHeight + XTP_DPI_Y(1) - nBaseHeight * nRowCount) / (nRowCount + 1);
	int nTotalOffset = nOffset;
	int nRow		 = 0;

	for (i = 0; i < nDataCount; i++)
	{
		CONTROLINFO* pControl = &pData[i];

		if (i > 0 && pControl->bWrap)
		{
			nOffset = (nResultHeight - nBaseHeight * nRowCount - nTotalOffset) / (nRowCount - nRow);
			nTotalOffset += nOffset;
			nRow++;
		}

		pControl->rcControl.OffsetRect(0, nTotalOffset);
	}

	m_pLayoutInfo->nWidth = nTotalWidth;

	return max(m_pLayoutInfo->nWidth, m_pLayoutInfo->nMinWidth);
}

int CXTPRibbonGroup::OnCalcDynamicSize(CDC* pDC)
{
	if (m_bReduced)
	{
		CSize szControl				 = GetControlGroupPopup()->GetSize(pDC);
		return m_pLayoutInfo->nWidth = szControl.cx - XTP_DPI_X(5);
	}

	if (m_bControlsGrouping)
		return _CalcSpecialDynamicSize(pDC);

	int nResultHeight = GetRibbonBar()->CalcGroupsHeight() - XTP_DPI_Y(9)
						- GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();

	CSize sizeResult(0, nResultHeight);

	int dyPrev = 0, dxPrev = 0, x = XTP_DPI_X(2);

	int nDataCount	 = m_pLayoutInfo->nControlCount;
	CONTROLINFO* pData = m_pLayoutInfo->pControls;

	for (int i = 0; i < nDataCount; i++)
	{
		CONTROLINFO* pControl = &pData[i];

		BOOL bFullRow = pControl->pControl->GetStyle() == xtpButtonIconAndCaptionBelow;
		pControl->EnsureButtonSize(pDC);
		CSize szControl = pControl->szControl;

		int dx = szControl.cx;
		int dy = szControl.cy;

		BOOL bBeginGroup = pControl->bBeginGroup && i > 0;
		BOOL bWrapGroup  = pControl->bWrapGroup && i > 0;

		if (bBeginGroup)
			x += XTP_DPI_X(6);

		if (i > 0 && !bWrapGroup && !bFullRow && dyPrev + dy <= nResultHeight)
		{
			pControl->rcControl.SetRect(x, dyPrev, x + dx, dyPrev + dy);

			dxPrev = max(dxPrev, dx);
		}
		else
		{
			x += dxPrev;
			pControl->rcControl.SetRect(x, 0, x + dx, dy);
			dxPrev = dx;
		}
		sizeResult.cx = max(x + dx, sizeResult.cx);

		dyPrev = pControl->rcControl.bottom;
	}

	m_pLayoutInfo->nWidth = sizeResult.cx + XTP_DPI_X(2);

	return max(m_pLayoutInfo->nWidth, m_pLayoutInfo->nMinWidth);
}

void CXTPRibbonGroup::CenterColumn(int nFirstItem, int nLastItem, int nGroupHeight)
{
	int nHeight = 0;
	int i;
	for (i = nFirstItem; i <= nLastItem; i++)
	{
		CONTROLINFO* pControl = &m_pLayoutInfo->pControls[i];

		if (pControl->bHide)
			continue;

		nHeight += pControl->rcControl.Height();
	}

	if (nHeight >= nGroupHeight)
		return;

	int nItems  = nLastItem - nFirstItem + 1;
	int nOffset = (nGroupHeight - nHeight) / (nItems + 1);

	for (i = nFirstItem; i <= nLastItem; i++)
	{
		CONTROLINFO* pControl = &m_pLayoutInfo->pControls[i];

		if (pControl->bHide)
			continue;

		pControl->rcControl.OffsetRect(0, nOffset);
		nOffset += nOffset;
	}
}

void CXTPRibbonGroup::OnAdjustBorders(int nWidth, CRect rcBorder)
{
	CXTPRibbonPaintManager* pPaintManager = GetRibbonBar()->GetRibbonPaintManager();
	int nGroupCaptionHeight				  = pPaintManager->GetGroupCaptionHeight();
	int nGroupHeight					  = GetRibbonBar()->CalcGroupsHeight();
	int nGroupClientHeight = nGroupHeight - XTP_DPI_Y(2) - XTP_DPI_Y(3) - XTP_DPI_Y(2);

	int i;
	BOOL bFirstItem   = TRUE;
	int nFirstItem	= 0;
	int nColumn		  = 0;
	BOOL bCenterItems = m_bControlsCentering;
	if (IsControlsGrouping())
		bCenterItems = FALSE;

	int nControlCount = m_pLayoutInfo ? m_pLayoutInfo->nControlCount : 0;

	int nLeftOffset = XTP_DPI_X(2);
	if (m_pLayoutInfo && m_bControlsCentering && m_pLayoutInfo->nWidth < m_pLayoutInfo->nMinWidth
		&& nWidth == m_pLayoutInfo->nMinWidth)
		nLeftOffset = XTP_DPI_X(2) + (m_pLayoutInfo->nMinWidth - m_pLayoutInfo->nWidth) / 2;

	for (i = 0; i < nControlCount; i++)
	{
		CONTROLINFO* pControl = &m_pLayoutInfo->pControls[i];

		if (pControl->bHide)
			continue;

		pControl->rcControl.OffsetRect(rcBorder.left + nLeftOffset, rcBorder.top + XTP_DPI_Y(2));
		pControl->rcRow = CRect(rcBorder.left, rcBorder.top, rcBorder.left + nWidth,
								rcBorder.top + nGroupClientHeight - nGroupCaptionHeight);

		if (bCenterItems && !bFirstItem)
		{
			if (nColumn != pControl->rcControl.left)
			{
				CenterColumn(nFirstItem, i - 1,
							 nGroupClientHeight - nGroupCaptionHeight - XTP_DPI_Y(3));
				nColumn	= pControl->rcControl.left;
				nFirstItem = i;
			}
		}
		else
		{
			bFirstItem = FALSE;
			nColumn	= pControl->rcControl.left;
		}
	}
	if (bCenterItems && nControlCount > 0)
		CenterColumn(nFirstItem, m_pLayoutInfo->nControlCount - 1,
					 nGroupClientHeight - nGroupCaptionHeight - XTP_DPI_Y(3));

	int nBeginGroup = rcBorder.left;
	int nEndGroup   = nBeginGroup + nWidth + XTP_DPI_X(5);

	SetRect(CRect(nBeginGroup, rcBorder.top, nEndGroup, rcBorder.top + nGroupClientHeight));
}

void CXTPRibbonGroup::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Int(pPX, _T("Id"), m_nId, 0);
	PX_Int(pPX, _T("IconId"), m_nIconId, 0);
	PX_Bool(pPX, _T("ShowOptionButton"), m_bShowOptionButton, FALSE);
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_Bool(pPX, _T("ControlsGrouping"), m_bControlsGrouping, FALSE);
	PX_Bool(pPX, _T("ControlsCentering"), m_bControlsCentering, FALSE);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);

	if (pPX->IsStoring())
	{
		CXTPPropExchangeSection secControls(pPX->GetSection(_T("Controls")));

		long nCount = 0;
		int i;
		for (i = 0; i < m_arrControls.GetSize(); i++)
		{
			CXTPControl* pControl = GetAt(i);
			if (!pControl->IsTemporary())
				nCount++;
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(secControls->GetEnumerator(_T("Control")));
		POSITION posEnum = pEnumerator->GetPosition(XTPToDWORDChecked(nCount), FALSE);

		for (i = 0; i < (int)m_arrControls.GetSize(); i++)
		{
			CXTPControl* pControl = GetAt(i);
			if (!pControl->IsTemporary())
			{
				CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
				PX_Object(&secItem, pControl, RUNTIME_CLASS(CXTPControl));
			}
		}
	}
	else
	{
		m_pGroups = (CXTPRibbonGroups*)pPX->m_pOwner;
		m_pParent = m_pRibbonBar = m_pGroups->GetRibbonBar();

		OnGroupAdded();

		CXTPPropExchangeSection secControls(pPX->GetSection(_T("Controls")));

		CXTPPropExchangeEnumeratorPtr pEnumerator(secControls->GetEnumerator(_T("Control")));
		POSITION posEnum = pEnumerator->GetPosition(0, FALSE);

		RemoveAll();

		while (posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));

			CXTPControl* pControl = NULL;
			PX_Object(&secItem, pControl, RUNTIME_CLASS(CXTPControl));

			if (!pControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			pControl->m_pRibbonGroup = this;
			pControl->InternalAddRef();
			m_pRibbonBar->GetControls()->InsertAt(pControl, m_pControlGroupOption->GetIndex());

			m_arrControls.Add(pControl);
		}
	}

	CXTPPropExchangeSection secControlGroupOption(pPX->GetSection(_T("ControlGroupOption")));
	m_pControlGroupOption->DoPropExchange(&secControlGroupOption);

	CXTPPropExchangeSection secControlGroupPopup(pPX->GetSection(_T("ControlGroupPopup")));
	m_pControlGroupPopup->DoPropExchange(&secControlGroupPopup);
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonGroup, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "RibbonBar", 100, OleGetRibbonBar, SetNotSupported,
						VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "Groups", 101, OleGetGroups, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "Caption", 6, m_strCaption, VT_BSTR)

	DISP_FUNCTION_ID(CXTPRibbonGroup, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "Add", 2, OleAdd, VT_DISPATCH,
					 VTS_I4 VTS_I4 VTS_BSTR VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "DeleteAt", 4, RemoveAt, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "ShowOptionButton", 7, m_bShowOptionButton, VT_BOOL)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "Id", 8, m_nId, VT_I4)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "IconId", 9, m_nIconId, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "Visible", 10, OleGetVisible, OleSetVisible, VT_BOOL)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "ControlsGrouping", 11, m_bControlsGrouping, VT_BOOL)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "ControlsCentering", 12, m_bControlsCentering, VT_BOOL)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "AllowReduce", 13, m_nAllowReduce, VT_I4)
	DISP_PROPERTY_ID(CXTPRibbonGroup, "AutoArrangeEditCaption", 14, m_bAutoArrangeEditCaption,
					 VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "ControlGroupPopup", 102, OleGetControlGroupPopup,
						SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "ControlGroupOption", 103, OleGetControlGroupOption,
						SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPRibbonGroup, "Index", 104, OleGetIndex, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "AddControl", 108, OleAddControl, VT_EMPTY,
					 VTS_DISPATCH VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPRibbonGroup, "GetRect", 110, OleGetRect, VT_EMPTY,
					 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)

END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonGroup, XTPDIID_IRibbonGroup)

BEGIN_INTERFACE_MAP(CXTPRibbonGroup, CXTPCmdTarget)
	INTERFACE_PART(CXTPRibbonGroup, XTPDIID_IRibbonGroup, Dispatch)
	// INTERFACE_PART(CXTPRibbonGroup, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_ENUM_VARIANT(CXTPRibbonGroup)

LPDISPATCH CXTPRibbonGroup::OleGetRibbonBar()
{
	return GetRibbonBar()->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonGroup::OleGetGroups()
{
	return m_pGroups->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonGroup::OleGetControlGroupPopup()
{
	return m_pControlGroupPopup->GetIDispatch(TRUE);
}

LPDISPATCH CXTPRibbonGroup::OleGetControlGroupOption()
{
	return m_pControlGroupOption->GetIDispatch(TRUE);
}

BOOL CXTPRibbonGroup::OleGetVisible()
{
	return IsVisible();
}

void CXTPRibbonGroup::OleSetVisible(BOOL bVisible)
{
	SetVisible(bVisible);
}

long CXTPRibbonGroup::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPRibbonGroup::OleGetItem(int nIndex)
{
	CXTPControl* pItem = GetAt(nIndex);

	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}

int CXTPRibbonGroup::OleGetIndex()
{
	return m_nIndex;
}

void CXTPRibbonGroup::OleAddControl(LPDISPATCH Control, const VARIANT& varBefore)
{
	int nBefore			  = VariantToLong(&varBefore) - 1;
	CXTPControl* pControl = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPControl,
														 CXTPControl::FromIDispatchSafe(Control));
	if (!pControl)
		return;

	if (pControl->GetControls())
		return;

	InsertAt(pControl, nBefore);
	pControl->InternalAddRef();
}

LPDISPATCH CXTPRibbonGroup::OleAdd(long controlType, int nId, LPCTSTR lpszCaption,
								   const VARIANT& varBefore, const VARIANT& varTemporary)
{
	int nBefore		= VariantToLong(&varBefore) - 1;
	BOOL bTemporary = VariantToBool(&varTemporary, FALSE);
	CString strCaption(lpszCaption);

	CXTPControl* pControl = Add((XTPControlType)controlType, nId, _T(""), nBefore, bTemporary);
	if (!pControl)
		return NULL;

	if (!strCaption.IsEmpty())
	{
		CString strTooltip(strCaption);
		CXTPPaintManager::StripMnemonics(strTooltip);

		CXTPControlAction* pAction = pControl->GetAction();
		if ((pAction != NULL)
			&& (pAction->GetCaption().IsEmpty() || pAction->GetCaption() == strCaption))
		{
			pAction->SetCaption(strCaption);
			pAction->SetTooltip(strTooltip);
		}
		else
		{
			pControl->SetCaption(strCaption);
			pControl->SetTooltip(strTooltip);
		}
	}
	return XTPGetDispatch(pControl);
}

void CXTPRibbonGroup::OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
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
