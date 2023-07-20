// Workspace.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "GraphicLibrary/unzip/unzip.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPMarkupTheme.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "TabManager/Includes.h"

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
#		include "DockingPane/Includes.h"
#		include "DockingPane/XTPDockingPaneManager.h"
#	endif
#endif

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPMouseManager.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h"
#include "CommandBars/Themes/XTPCommandBarsOffice2003Theme.h"

#include "CommandBars/XTPTabClientWnd.h"
#include "CommandBars/TabClient/Workspace.h"
#include "CommandBars/TabClient/XTPTabClientDraggingContents.h"
#include "CommandBars/TabClient/XTPTabClientSticker.h"
#include "CommandBars/TabClient/XTPTabClientContext.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CWorkspace

CXTPTabClientWnd::CWorkspace::CWorkspace()
{
	m_pTabClientWnd = NULL;
	m_dHeight		= XTP_DPI_Y(100);

	m_pNewWindowItem = NULL;
}

CXTPTabClientWnd::CWorkspace::~CWorkspace()
{
}

void CXTPTabClientWnd::CWorkspace::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (pItem && pItem == m_pNewWindowItem)
	{
		int nCount = GetItemCount();
		pItem	  = (nCount < 2) ? NULL : GetItem(nCount - 2);
	}

#ifdef _XTP_COMMANDBARS_ACTIVEX
	if (m_pSelected != pItem)
	{
		if (pItem)
			m_pTabClientWnd->FireSelectedChanged(pItem->GetIDispatch(FALSE));
	}
#endif

	CXTPTabManager::SetSelectedItem(pItem);
}

BOOL CXTPTabClientWnd::CWorkspace::IsMouseLocked() const
{
	return CXTPTabManager::IsMouseLocked() || XTPMouseManager()->IsTrackedLock();
}

void CXTPTabClientWnd::CWorkspace::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (!m_pTabClientWnd->m_bLockReposition)
		m_pTabClientWnd->InvalidateRect(lpRect, FALSE);
}

void CXTPTabClientWnd::CWorkspace::Reposition()
{
	if (!m_pTabClientWnd)
		return;

	if (m_pTabClientWnd->m_bLockReposition)
	{
		m_pTabClientWnd->m_bForceToRecalc = TRUE;
	}
	else
	{
		m_pTabClientWnd->Reposition();
	}
}

BOOL CXTPTabClientWnd::CWorkspace::IsAllowReorder() const
{
	return m_pTabClientWnd->m_bAllowReorder;
}

void CXTPTabClientWnd::CWorkspace::SetAllowReorder(BOOL bAllowReorder)
{
	m_pTabClientWnd->m_bAllowReorder = bAllowReorder;
}

BOOL CXTPTabClientWnd::CWorkspace::IsDrawStaticFrame() const
{
	return GetPaintManager()->m_bStaticFrame && !m_pTabClientWnd->m_bEnableGroups;
}

BOOL CXTPTabClientWnd::CWorkspace::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem,
											BOOL bDraw, CSize& szIcon) const
{
	if (pItem == m_pNewWindowItem)
	{
		if (!bDraw)
			return TRUE;

		CXTPCommandBars* pCommandBars   = m_pTabClientWnd->GetCommandBars();
		CXTPImageManager* pImageManager = pCommandBars ? pCommandBars->GetImageManager()
													   : XTPImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(
			XTPToUInt(m_pTabClientWnd->m_nNewWindowTabCommand));
		if (pIcon)
			pIcon->Draw(pDC, pt, szIcon);

		return TRUE;
	}

	if (GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}

	if (pItem->GetImageIndex() != -1)
	{
		CXTPCommandBars* pCommandBars   = m_pTabClientWnd->GetCommandBars();
		CXTPImageManager* pImageManager = pCommandBars ? pCommandBars->GetImageManager()
													   : XTPImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(XTPToUInt(pItem->GetImageIndex()));
		if (pIcon)
			pIcon->Draw(pDC, pt, szIcon);

		return TRUE;
	}

	HICON hIcon = GetItemIcon(pItem);
	if (hIcon)
	{
		DrawIconEx(pDC->GetSafeHdc(), pt.x, pt.y, hIcon, szIcon.cx, szIcon.cy, 0, NULL, DI_NORMAL);
	}

	return TRUE;
}

CXTPTabPaintManager* CXTPTabClientWnd::CWorkspace::GetPaintManager() const
{
	return m_pTabClientWnd ? m_pTabClientWnd->m_pPaintManager : NULL;
}

CString CXTPTabClientWnd::CWorkspace::GetItemTooltip(const CXTPTabManagerItem* pItem) const
{
	return m_pTabClientWnd->GetItemTooltip(pItem);
}

// finds the tab item for specified window. Returns NULL if not found
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::FindItem(const HWND hWnd) const
{
	// loop through all tab items
	for (int nIndex = 0; nIndex < GetItemCount(); nIndex++)
	{
		// check for window handle
		if (GetItem(nIndex)->GetHandle() == hWnd)
		{
			return GetItem(nIndex);
		}
	}

	return NULL;
}

// add new item into the tab control for specified MDIChild. If bRedraw is
// set to TRUE then framework will be redrawn in order to show new item.
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::AddItem(const CWnd* pChildWnd)
{
	ASSERT(pChildWnd != NULL);
	if (!pChildWnd)
		return NULL;
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));

	// make sure we add MDIChild window
	if ((pChildWnd->GetExStyle() & WS_EX_MDICHILD) == 0)
		return NULL;

	if (GetItemCount() == 0)
		AddNewWindowItem();

	int nIndex = GetItemCount();

	if (m_pNewWindowItem)
		nIndex--;

	switch (m_pTabClientWnd->GetNewTabPositon())
	{
		case xtpWorkspaceNewTabLeftMost: nIndex = 0; break;

		case xtpWorkspaceNewTabNextToActive:
			if (!(m_pNewWindowItem && m_pNewWindowItem == m_pSelected))
			{
				nIndex = GetCurSel() + 1;
			}
			break;

		case xtpWorkspaceNewTabRightMost:
		default: break;
	}

	// save information about new entry
	CXTPTabManagerItem* pItem = CXTPTabManager::AddItem(nIndex);
	pItem->SetCaption(m_pTabClientWnd->GetItemText(pChildWnd));

	return pItem;
}

BOOL CXTPTabClientWnd::CWorkspace::OnBeforeItemClick(CXTPTabManagerItem* pItem)
{
	return m_pTabClientWnd->OnBeforeItemClick(pItem);
}

void CXTPTabClientWnd::CWorkspace::OnItemClick(CXTPTabManagerItem* pItem)
{
	ASSERT(pItem);
	if (!pItem)
		return;

	if (pItem == m_pNewWindowItem)
	{
#ifdef _XTP_COMMANDBARS_ACTIVEX

		CXTPControlButton* pControl = new CXTPControlButton();
		pControl->SetID(m_pTabClientWnd->m_nNewWindowTabCommand);

		NMXTPCONTROL tagNMCONTROL;
		pControl->NotifySite(m_pTabClientWnd->GetParentFrame(), CBN_XTP_EXECUTE, &tagNMCONTROL);

		pControl->InternalRelease();
#else
		m_pTabClientWnd->GetParent()->SendMessage(WM_COMMAND,
												  XTPToWPARAM(
													  m_pTabClientWnd->m_nNewWindowTabCommand));
#endif

		return;
	}

	CWnd* pWnd = CWnd::FromHandle(pItem->GetHandle());
	if (pWnd)
	{
		m_pTabClientWnd->MDIActivate(pWnd);

		CWnd* pFocus = GetFocus();

		BOOL bHasFocus = pFocus->GetSafeHwnd()
						 && (pFocus == pWnd || pWnd->IsChild(pFocus)
							 || (pFocus->GetOwner()->GetSafeHwnd()
								 && pWnd->IsChild(pFocus->GetOwner())));

		if (!bHasFocus)
			pWnd->SetFocus();
	}
}

CWnd* CXTPTabClientWnd::CWorkspace::GetWindow() const
{
	return m_pTabClientWnd;
}

void CXTPTabClientWnd::CWorkspace::OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton)
{
	if (pButton->GetID() != xtpTabNavigateButtonClose)
		return;

	CXTPTabManagerItem* pItem = pButton->GetItem() ? pButton->GetItem() : m_pSelected;

	if (!pItem)
		return;

	m_pTabClientWnd->PostMessage(WM_IDLEUPDATECMDUI);

	HWND hWnd = pItem->GetHandle();
	::SendMessage(hWnd, WM_CLOSE, 0, 0);
}

void CXTPTabClientWnd::CWorkspace::OnNavigateButtonClick(UINT nID)
{
	CXTPTabManager::OnNavigateButtonClick(nID);
}

void CXTPTabClientWnd::CWorkspace::OnRecalcLayout()
{
}

CXTPMarkupContext* CXTPTabClientWnd::CWorkspace::GetMarkupContext() const
{
	return m_pTabClientWnd->m_pMarkupContext;
}

HICON CXTPTabClientWnd::CWorkspace::GetItemIcon(const CXTPTabManagerItem* pItem) const
{
	return m_pTabClientWnd->GetItemIcon(pItem);
}

COLORREF CXTPTabClientWnd::CWorkspace::GetItemColor(const CXTPTabManagerItem* pItem) const
{
	COLORREF clr = m_pTabClientWnd->GetItemColor(pItem);

	if (clr != COLORREF_NULL)
	{
		if (clr >= xtpTabColorBlue && clr <= xtpTabColorMagenta)
			return CXTPTabPaintManager::GetOneNoteColor((XTPTabOneNoteColor)clr);

		return clr;
	}

	return CXTPTabManager::GetItemColor(pItem);
}

void CXTPTabClientWnd::CWorkspace::ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	BOOL bGroupingEnabled = m_pTabClientWnd->IsGroupingEnabled();
	BOOL bTearOffEnabled  = m_pTabClientWnd->IsTearOffEnabled();

	if (pItem == m_pNewWindowItem)
	{
		m_pTabClientWnd->SetActiveWorkspace(this);

		m_pHighlighted = NULL;

		OnItemClick(pItem);

		return;
	}

	if (!bTearOffEnabled && m_pTabClientWnd->GetWorkspaceCount() == 1
		&& pItem->GetTabManager()->GetItemCount() == 1)
	{
		OnItemClick(pItem);
		return;
	}

	if (m_pSelected != pItem)
	{
		m_pSelected = pItem;
#ifdef _XTP_COMMANDBARS_ACTIVEX
		m_pTabClientWnd->FireSelectedChanged(pItem->GetIDispatch(FALSE));
#endif
	}

	m_pTabClientWnd->m_bLockReposition = TRUE;
	m_pTabClientWnd->SetActiveWorkspace(this);
	m_pTabClientWnd->m_bLockReposition = FALSE;

	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	::SetCapture(hWnd);

	Reposition();

	CArray<CRect, CRect&> arrRects;

	for (int j = 0; j < GetItemCount(); j++)
	{
		CRect rc = GetItem(j)->GetRect();
		arrRects.Add(rc);
	}

	BOOL bAccept					  = FALSE;
	m_pTabClientWnd->m_nFocusedAction = XTP_TABCLIENT_NO_ACTION;
	BOOL bStickers = m_pTabClientWnd->m_pTabClientContext->GetShowTabClientContextStickers();
	CRect rcDragRect;
	CXTPTabClientWnd* pTargetTab = NULL;
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	CXTPDockingPaneTabbedContainer* pTargetPane = NULL;
#	endif
#endif
	POINT pos = { 0 };

	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != hWnd)
		{
			DispatchMessage(&msg);
			goto ExitLoop;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
			{
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				::GetCursorPos(&pos);
				pTargetTab = m_pTabClientWnd->FindTargetTabClient(pos);
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
				if (!pTargetTab)
					pTargetPane = m_pTabClientWnd->FindTargetDockPane(pos);
				else
				{
					m_pTabClientWnd->m_pTabClientContext->DisableDockSticker();
					m_pTabClientWnd->m_pTabClientContext->DisableDockContextWindow();
					m_pTabClientWnd->m_pTabClientContext->DisableDockFocusRect();
				}
#	endif
#endif
				if (pTargetTab == m_pTabClientWnd) // Same tab
				{
					if (pTargetTab->GetParent() != CWnd::FromHandle(::GetForegroundWindow()))
						pTargetTab->GetParent()->SetForegroundWindow();

					if (m_rcHeaderRect.IsRectEmpty() || m_rcHeaderRect.PtInRect(pt))
					{
						if (bGroupingEnabled || bTearOffEnabled)
						{
							m_pTabClientWnd->m_pTabClientContext->DisableTabSticker();
							m_pTabClientWnd->m_pTabClientContext->DisableTabContextWindow();
							m_pTabClientWnd->m_pTabClientContext->DisableTabFocusRect();
#ifndef _XTP_COMMANDBARS_ACTIVEX
							m_pTabClientWnd->m_pTabClientContext->EnableDraggingContents();
#endif
						}

						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						m_pTabClientWnd->m_nFocusedAction = XTP_TABCLIENT_ACTION_CANCEL;

						for (int i = 0; i < arrRects.GetSize(); i++)
						{
							if (i != pItem->GetIndex() && arrRects[i].PtInRect(pt)
								&& m_arrItems[i] != m_pNewWindowItem)
							{
								CXTPTabManagerItem* p		  = pItem;
								m_arrItems[pItem->GetIndex()] = m_arrItems[i];
								m_arrItems[i]				  = p;

								OnItemsChanged();
								break;
							}
						}
					}
					else if (bGroupingEnabled)
					{
						rcDragRect.SetRectEmpty();

						m_pTabClientWnd->m_pTabClientContext->EnableTabSticker();
						m_pTabClientWnd->m_pTabClientContext->EnableTabFocusRect();

						m_pTabClientWnd->m_pFocusWorkspace = NULL;
						m_pTabClientWnd->m_nFocusedAction  = bTearOffEnabled
																? XTP_TABCLIENT_ACTION_DETACH
																: XTP_TABCLIENT_NO_ACTION;
						CXTPClientRect rc(m_pTabClientWnd);
						if (rc.PtInRect(pt))
						{
							::SetCursor(m_pTabClientWnd->m_hCursorNew);

							m_pTabClientWnd->m_nFocusedAction =
								bStickers
									? GetTabStickerAction(pos, &m_pTabClientWnd->m_pFocusWorkspace)
									: GetTabSideAction(pos, pItem,
													   &m_pTabClientWnd->m_pFocusWorkspace,
													   rcDragRect);
						}
					}
				}
				else if (pTargetTab == NULL) // No tab
				{
					::SetCursor(bTearOffEnabled ? m_pTabClientWnd->m_hCursorNew
												: m_pTabClientWnd->m_hCursorDelete);

					m_pTabClientWnd->m_nFocusedAction = XTP_TABCLIENT_ACTION_DETACH;

					m_pTabClientWnd->m_pTabClientContext->DisableTabSticker();
					m_pTabClientWnd->m_pTabClientContext->DisableTabContextWindow();
					m_pTabClientWnd->m_pTabClientContext->DisableTabFocusRect();
#ifndef _XTP_COMMANDBARS_ACTIVEX
					m_pTabClientWnd->m_pTabClientContext->EnableDraggingContents();
#endif

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
					if (pTargetPane) // Dock pane
					{
						m_pTabClientWnd->m_pTabClientContext->EnableDockSticker();
						m_pTabClientWnd->m_pTabClientContext->EnableDockContextWindow();
						m_pTabClientWnd->m_pTabClientContext->EnableDockFocusRect();

						m_pTabClientWnd->m_nFocusedAction = bStickers
																? GetDockStickerAction()
																: GetDockSideAction(pTargetPane,
																					pos,
																					rcDragRect);
					}
#	endif
#endif
				}
				else // Another tab
				{
					if (pTargetTab->GetParent() != CWnd::FromHandle(::GetForegroundWindow()))
						pTargetTab->GetParent()->SetForegroundWindow();

					m_pTabClientWnd->m_pTabClientContext->EnableTabFocusRect();

					if (bTearOffEnabled || !bGroupingEnabled)
					{
						::SetCursor(m_pTabClientWnd->m_hCursorNew);
						m_pTabClientWnd->m_nFocusedAction = XTP_TABCLIENT_ACTION_DETACH;

						m_pTabClientWnd->m_nFocusedAction = bStickers
																? GetTabStickerAction()
																: GetTabSideAction(pTargetTab, pos,
																				   rcDragRect);
					}
				}

				if (bGroupingEnabled || bTearOffEnabled)
				{
					m_pTabClientWnd->m_pTabClientContext->UpdateTabSticker();
					m_pTabClientWnd->m_pTabClientContext->UpdateTabContextWindow();
					m_pTabClientWnd->m_pTabClientContext->UpdateTabFocusRect(rcDragRect);
#ifndef _XTP_COMMANDBARS_ACTIVEX
					m_pTabClientWnd->m_pTabClientContext->UpdateDraggingContents();
#endif

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
					m_pTabClientWnd->m_pTabClientContext->UpdateDockSticker();
					m_pTabClientWnd->m_pTabClientContext->UpdateDockContextWindow();
					m_pTabClientWnd->m_pTabClientContext->UpdateDockFocusRect(rcDragRect);
#	endif
#endif
				}
			}

			break;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_CANCELMODE:
			case WM_RBUTTONDOWN: goto ExitLoop;

			case WM_LBUTTONUP: bAccept = TRUE; goto ExitLoop;

			default: DispatchMessage(&msg); break;
		}
	}

ExitLoop:

	ReleaseCapture();
	PerformMouseMove(hWnd, pt);
	OnItemClick(pItem);

	int nLastWorkspace = m_pTabClientWnd->m_pTabClientContext->GetLastWorkspace();

	m_pTabClientWnd->m_pTabClientContext->DisableTabSticker();
	m_pTabClientWnd->m_pTabClientContext->DisableTabContextWindow();
#ifndef _XTP_COMMANDBARS_ACTIVEX
	m_pTabClientWnd->m_pTabClientContext->DisableDraggingContents();
#endif
	m_pTabClientWnd->m_pTabClientContext->DisableTabFocusRect();
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
	m_pTabClientWnd->m_pTabClientContext->DisableDockSticker();
	m_pTabClientWnd->m_pTabClientContext->DisableDockContextWindow();
	m_pTabClientWnd->m_pTabClientContext->DisableDockFocusRect();
#	endif
#endif

	if (m_pTabClientWnd->m_nFocusedAction != XTP_TABCLIENT_NO_ACTION)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (bAccept)
		{
			switch (m_pTabClientWnd->m_nFocusedAction)
			{
				case XTP_TABCLIENT_ACTION_DETACH:
				case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT:
				case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP:
				case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT:
				case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM:
				case XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER:
				{
					m_pTabClientWnd->DoWorkspaceCommand(pItem, m_pTabClientWnd->m_pFocusWorkspace,
														m_pTabClientWnd->m_nFocusedAction, pos,
														pTargetTab);
					return;
				}
				case XTP_TABCLIENT_ACTION_ATTACH_LEFT:
				case XTP_TABCLIENT_ACTION_ATTACH_RIGHT:
				case XTP_TABCLIENT_ACTION_ATTACH_TOP:
				case XTP_TABCLIENT_ACTION_ATTACH_BOTTOM:
				{
					CWorkspace* pSelWorkspace = nLastWorkspace > -1
													? m_pTabClientWnd->GetWorkspace(nLastWorkspace)
													: m_pTabClientWnd->m_pFocusWorkspace;

					m_pTabClientWnd->DoWorkspaceCommand(pItem, pSelWorkspace,
														m_pTabClientWnd->m_nFocusedAction);
					break;
				}
#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE
				case XTP_TABCLIENT_ACTION_DOCK_CENTER:
				case XTP_TABCLIENT_ACTION_DOCK_LEFT:
				case XTP_TABCLIENT_ACTION_DOCK_RIGHT:
				case XTP_TABCLIENT_ACTION_DOCK_TOP:
				case XTP_TABCLIENT_ACTION_DOCK_BOTTOM:
				{
					m_pTabClientWnd->DoWorkspaceCommand(pItem, NULL,
														m_pTabClientWnd->m_nFocusedAction, pos,
														NULL, pTargetPane);
					return;
				}
#	endif
#endif
				default:
				{
					m_pTabClientWnd->DoWorkspaceCommand(pItem, m_pTabClientWnd->m_pFocusWorkspace,
														m_pTabClientWnd->m_nFocusedAction);
					break;
				}
			};
		}
	}

	pItem->GetTabManager()->EnsureVisible(pItem);
}

BOOL CXTPTabClientWnd::CWorkspace::DrawParentBackground(CDC* pDC, CRect rc)
{
	if (m_pTabClientWnd->m_themeCommandBars == xtpThemeVisualStudio2008)
	{
		CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
		if (!pCommandBars)
			return FALSE;

		CXTPOffice2003Theme* pPaintManager = (CXTPOffice2003Theme*)pCommandBars->GetPaintManager();
		if (((CXTPPaintManager*)pPaintManager)->BaseTheme() != xtpThemeVisualStudio2008)
			return FALSE;

		CWnd* pWnd = m_pTabClientWnd->GetParentFrame();
		CXTPWindowRect rcFill((CWnd*)pWnd);

		m_pTabClientWnd->ScreenToClient(rcFill);
		rcFill.right  = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
		rcFill.top	= rc.top;
		rcFill.bottom = rc.bottom;

		XTPDrawHelpers()->GradientFill(pDC, rcFill, pPaintManager->m_clrDockBar, TRUE, rc);
		return TRUE;
	}
	return FALSE;
}

int CXTPTabClientWnd::CWorkspace::GetTabStickerAction(POINT pos, CWorkspace** pWorkspace)
{
	ASSERT(NULL != pWorkspace);
	*pWorkspace = NULL;

	int value = m_pTabClientWnd->IsTearOffEnabled() ? XTP_TABCLIENT_ACTION_DETACH
													: XTP_TABCLIENT_NO_ACTION;

	POINT ptWorkspace(pos);
	m_pTabClientWnd->ScreenToWorkspace(&ptWorkspace);
	int nWorkspace = -1;

	for (int i = 0; i < m_pTabClientWnd->GetWorkspaceCount(); i++)
	{
		CRect rcHeader = m_pTabClientWnd->GetWorkspace(i)->GetHeaderRect();
		CRect rcClient = m_pTabClientWnd->GetWorkspace(i)->GetClientRect();

		if (rcHeader.PtInRect(ptWorkspace) || rcClient.PtInRect(ptWorkspace))
		{
			nWorkspace = i;
			break;
		}
	}

	if (m_pTabClientWnd->m_pTabClientContext->GetLastWorkspace() == nWorkspace)
	{
		XTPTabClientStickerPart sticker =
			m_pTabClientWnd->m_pTabClientContext->GetLastSelectedTabClientSticker();

		switch (sticker)
		{
			case xtpTabClientStickerLeft:
				(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		  = XTP_TABCLIENT_ACTION_ATTACH_LEFT;
				break;
			case xtpTabClientStickerRight:
				(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		  = XTP_TABCLIENT_ACTION_ATTACH_RIGHT;
				break;
			case xtpTabClientStickerTop:
				(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		  = XTP_TABCLIENT_ACTION_ATTACH_TOP;
				break;
			case xtpTabClientStickerBottom:
				(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		  = XTP_TABCLIENT_ACTION_ATTACH_BOTTOM;
				break;
			case xtpTabClientStickerCenter:
				(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		  = XTP_TABCLIENT_ACTION_ATTACH_CENTER;
				break;
			case xtpTabClientStickerNone:
			default:
				//
				break;
		}
	}

	return value;
}

int CXTPTabClientWnd::CWorkspace::GetTabStickerAction()
{
	int value = XTP_TABCLIENT_ACTION_DETACH;

	XTPTabClientStickerPart sticker =
		m_pTabClientWnd->m_pTabClientContext->GetLastSelectedTabClientSticker();
	switch (sticker)
	{
		case xtpTabClientStickerLeft: value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT; break;
		case xtpTabClientStickerRight: value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT; break;
		case xtpTabClientStickerTop: value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP; break;
		case xtpTabClientStickerBottom: value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM; break;
		case xtpTabClientStickerCenter: value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER; break;
		case xtpTabClientStickerNone: value = XTP_TABCLIENT_ACTION_DETACH; break;
		default: value = XTP_TABCLIENT_ACTION_DETACH; break;
	}

	return value;
}

int CXTPTabClientWnd::CWorkspace::GetTabSideAction(POINT pos, CXTPTabManagerItem* pItem,
												   CWorkspace** pWorkspace, CRect& rcDragRect)
{
	ASSERT(NULL != pWorkspace);
	*pWorkspace = NULL;

	int delta = 50;
	int value = m_pTabClientWnd->IsTearOffEnabled() ? XTP_TABCLIENT_ACTION_DETACH
													: XTP_TABCLIENT_NO_ACTION;

	POINT ptWorkspace(pos);
	m_pTabClientWnd->ScreenToWorkspace(&ptWorkspace);
	int nWorkspace = -1;

	for (int i = 0; i < m_pTabClientWnd->GetWorkspaceCount(); i++)
	{
		CRect rcHeader = m_pTabClientWnd->GetWorkspace(i)->GetHeaderRect();
		CRect rcClient = m_pTabClientWnd->GetWorkspace(i)->GetClientRect();

		if (rcHeader.PtInRect(ptWorkspace) || rcClient.PtInRect(ptWorkspace))
		{
			nWorkspace = i;
			break;
		}
	}

	CRect rcControl(m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect());
	CRect rcTop(rcControl);
	CRect rcBottom(rcControl);
	CRect rcLeft(rcControl);
	CRect rcRight(rcControl);

	rcTop.bottom = rcTop.top + XTP_DPI_Y(delta);
	rcBottom.top = rcBottom.bottom - XTP_DPI_Y(delta);
	rcLeft.right = rcLeft.left + XTP_DPI_X(delta);
	rcRight.left = rcRight.right - XTP_DPI_X(delta);

	if (m_pTabClientWnd->GetWorkspace(nWorkspace)->GetHeaderRect().PtInRect(ptWorkspace))
	{
		(*pWorkspace) = m_pTabClientWnd->GetWorkspace(nWorkspace);
		value		  = XTP_TABCLIENT_ACTION_ATTACH_CENTER;
		rcDragRect	= m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect();
	}
	else
	{
		BOOL bSelfAvail = pItem->GetTabManager() != m_pTabClientWnd->GetWorkspace(nWorkspace)
						  || m_pTabClientWnd->GetWorkspace(nWorkspace)->GetItemCount() > 1;

		if ((m_pTabClientWnd->GetWorkspaceCount() == 1 || m_pTabClientWnd->m_bHorizSplitting)
			&& bSelfAvail)
		{
			if (rcTop.PtInRect(ptWorkspace))
			{
				(*pWorkspace)	 = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value			  = XTP_TABCLIENT_ACTION_ATTACH_TOP;
				rcDragRect		  = m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect();
				rcDragRect.bottom = rcDragRect.CenterPoint().y;
			}
			else if (rcBottom.PtInRect(ptWorkspace))
			{
				(*pWorkspace)  = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value		   = XTP_TABCLIENT_ACTION_ATTACH_BOTTOM;
				rcDragRect	 = m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect();
				rcDragRect.top = rcDragRect.CenterPoint().y;
			}
		}
		if ((m_pTabClientWnd->GetWorkspaceCount() == 1 || !m_pTabClientWnd->m_bHorizSplitting)
			&& bSelfAvail)
		{
			if (rcLeft.PtInRect(ptWorkspace))
			{
				(*pWorkspace)	= m_pTabClientWnd->GetWorkspace(nWorkspace);
				value			 = XTP_TABCLIENT_ACTION_ATTACH_LEFT;
				rcDragRect		 = m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect();
				rcDragRect.right = rcDragRect.CenterPoint().x;
			}
			else if (rcRight.PtInRect(ptWorkspace))
			{
				(*pWorkspace)   = m_pTabClientWnd->GetWorkspace(nWorkspace);
				value			= XTP_TABCLIENT_ACTION_ATTACH_RIGHT;
				rcDragRect		= m_pTabClientWnd->GetWorkspace(nWorkspace)->GetControlRect();
				rcDragRect.left = rcDragRect.CenterPoint().x;
			}
		}
	}

	return value;
}

int CXTPTabClientWnd::CWorkspace::GetTabSideAction(CXTPTabClientWnd* pTarget, POINT pos,
												   CRect& rcDragRect)
{
	int value = XTP_TABCLIENT_ACTION_DETACH;
	int delta = 50;

	if (!pTarget)
		return value;

	if (pTarget == m_pTabClientWnd)
		return value;

	ASSERT(pTarget && pTarget != m_pTabClientWnd);

	POINT ptWorkspace(pos);
	pTarget->ScreenToWorkspace(&ptWorkspace);
	int n = -1;

	for (int i = 0; i < pTarget->GetWorkspaceCount(); i++)
	{
		CRect rcHeader = pTarget->GetWorkspace(i)->GetHeaderRect();
		CRect rcClient = pTarget->GetWorkspace(i)->GetClientRect();

		if (rcHeader.PtInRect(ptWorkspace) || rcClient.PtInRect(ptWorkspace))
		{
			n = i;
			break;
		}
	}

	if (n == -1)
	{
		value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER;
		pTarget->GetClientRect(&rcDragRect);
	}
	else
	{
		ASSERT(n > -1);

		rcDragRect = pTarget->GetWorkspace(n)->GetControlRect();

		CRect rcHeader = pTarget->GetWorkspace(n)->GetHeaderRect();
		CRect rcClient = pTarget->GetWorkspace(n)->GetClientRect();
		pTarget->ClientToScreen(&rcHeader);
		pTarget->ClientToScreen(&rcClient);

		if (rcHeader.PtInRect(pos))
		{
			value = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_CENTER;
		}
		else if (rcClient.PtInRect(pos))
		{
			CRect rcLeft(rcClient);
			CRect rcTop(rcClient);
			CRect rcRight(rcClient);
			CRect rcBottom(rcClient);

			rcLeft.right = rcLeft.left + XTP_DPI_X(delta);
			rcTop.bottom = rcTop.top + XTP_DPI_Y(delta);
			rcRight.left = rcRight.right - XTP_DPI_X(delta);
			rcBottom.top = rcBottom.bottom - XTP_DPI_Y(delta);

			if (rcLeft.PtInRect(pos))
			{
				rcDragRect.right = rcDragRect.CenterPoint().x;
				value			 = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_LEFT;
			}
			else if (rcTop.PtInRect(pos))
			{
				rcDragRect.bottom = rcDragRect.CenterPoint().y;
				value			  = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_TOP;
			}
			else if (rcRight.PtInRect(pos))
			{
				rcDragRect.left = rcDragRect.CenterPoint().x;
				value			= XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_RIGHT;
			}
			else if (rcBottom.PtInRect(pos))
			{
				rcDragRect.top = rcDragRect.CenterPoint().y;
				value		   = XTP_TABCLIENT_ACTION_ATTACH_TEAR_OFF_BOTTOM;
			}
			else
			{
				rcDragRect.SetRectEmpty();
			}
		}
	}

	return value;
}

#ifndef _XTP_COMMANDBARS_ACTIVEX
#	ifdef _XTP_INCLUDE_DOCKINGPANE

int CXTPTabClientWnd::CWorkspace::GetDockStickerAction()
{
	int value = XTP_TABCLIENT_ACTION_DETACH;

	XTPDockingPaneStickerType sticker =
		m_pTabClientWnd->m_pTabClientContext->GetLastSelectedDockSticker();
	switch (sticker)
	{
		case xtpPaneStickerLeft: value = XTP_TABCLIENT_ACTION_DOCK_LEFT; break;
		case xtpPaneStickerRight: value = XTP_TABCLIENT_ACTION_DOCK_RIGHT; break;
		case xtpPaneStickerTop: value = XTP_TABCLIENT_ACTION_DOCK_TOP; break;
		case xtpPaneStickerBottom: value = XTP_TABCLIENT_ACTION_DOCK_BOTTOM; break;
		case xtpPaneStickerCenter: value = XTP_TABCLIENT_ACTION_DOCK_CENTER; break;

		case xtpPaneStickerNone: value = XTP_TABCLIENT_ACTION_DETACH; break;
		default: value = XTP_TABCLIENT_ACTION_DETACH; break;
	}

	return value;
}

int CXTPTabClientWnd::CWorkspace::GetDockSideAction(CXTPDockingPaneTabbedContainer* pTarget,
													POINT pos, CRect& rcDragRect)
{
	int value = XTP_TABCLIENT_ACTION_DETACH;
	int delta = 50;

	CXTPWindowRect rc(pTarget);
	if (rc.PtInRect(pos))
	{
		CRect rcLeft(rc);
		CRect rcTop(rc);
		CRect rcRight(rc);
		CRect rcBottom(rc);

		rcLeft.right = rcLeft.left + XTP_DPI_X(delta);
		rcTop.bottom = rcTop.top + XTP_DPI_Y(delta);
		rcRight.left = rcRight.right - XTP_DPI_X(delta);
		rcBottom.top = rcBottom.bottom - XTP_DPI_Y(delta);

		rcDragRect = rc;

		if (rcLeft.PtInRect(pos))
		{
			rcDragRect.right = rcDragRect.CenterPoint().x;
			value			 = XTP_TABCLIENT_ACTION_DOCK_LEFT;
		}
		else if (rcTop.PtInRect(pos))
		{
			rcDragRect.bottom = rcDragRect.CenterPoint().y;
			value			  = XTP_TABCLIENT_ACTION_DOCK_TOP;
		}
		else if (rcRight.PtInRect(pos))
		{
			rcDragRect.left = rcDragRect.CenterPoint().x;
			value			= XTP_TABCLIENT_ACTION_DOCK_RIGHT;
		}
		else if (rcBottom.PtInRect(pos))
		{
			rcDragRect.top = rcDragRect.CenterPoint().y;
			value		   = XTP_TABCLIENT_ACTION_DOCK_BOTTOM;
		}
		else
		{
			rcDragRect;
			value = XTP_TABCLIENT_ACTION_DOCK_CENTER;
		}

		pTarget->ScreenToClient(&rcDragRect);
	}

	return value;
}

#	endif
#endif