// XTPPropertyGrid.cpp : implementation of the CXTPPropertyGrid class.
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

#include "PropertyGrid/Resource.h"
#include "Common/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarCtrl.h"
#include "Common/ScrollBar/XTPScrollable.h"
#include "Common/XTPMacros.h"

#include "PropertyGrid/XTPPropertyGridDefines.h"
#include "PropertyGrid/XTPPropertyGridInplaceEdit.h"
#include "PropertyGrid/XTPPropertyGridInplaceButton.h"
#include "PropertyGrid/XTPPropertyGridInplaceList.h"
#include "PropertyGrid/XTPPropertyGridItem.h"
#include "PropertyGrid/XTPPropertyGridPaintManager.h"
#include "PropertyGrid/XTPPropertyGridToolBar.h"
#include "PropertyGrid/XTPPropertyGrid.h"
#include "PropertyGrid/XTPPropertyGridToolTip.h"
#include "PropertyGrid/XTPPropertyGridView.h"
#include "PropertyGrid/XTPPropertyGridIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#	define IDC_HAND MAKEINTRESOURCE(32649)
#endif

const int TOOLBAR_HEIGHT = 25; // DPI height is currently not supported due to CToolBar
							   // limitiations.

bool m_bThinSplitter = true;

/////////////////////////////////////////////////////////////////////////////
// CXTPScrollablePropertyGridListBoxT

class CXTPScrollablePropertyGridListBox : public CXTPScrollable<CXTPPropertyGridView>
{
	DECLARE_DYNAMIC(CXTPScrollablePropertyGridListBox);

public:
	CXTPScrollablePropertyGridListBox()
	{
		EnableDefaultBorderDrawing(FALSE);
	}

	virtual CWnd* CreateControl() const
	{
		CXTPPropertyGridView* pNewView = new CXTPPropertyGridView();
		pNewView->m_pGrid			   = m_pGrid;
		return pNewView;
	}

	virtual BOOL RequiresMouseWheelOverriding() const
	{
		return TRUE;
	}

	CWnd* GetControl()
	{
		return GetEmbeddedControl();
	}

	DWORD FilterStyle(DWORD dwStyle) const
	{
		return dwStyle & ~(WS_VSCROLL | WS_HSCROLL);
	}
};

IMPLEMENT_DYNAMIC(CXTPScrollablePropertyGridListBox, CXTPPropertyGridView);

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridUpdateContext

IMPLEMENT_DYNAMIC(CXTPPropertyGridUpdateContext, CXTPCmdTarget);

CXTPPropertyGridUpdateContext::CXTPPropertyGridUpdateContext()
{
	m_nSelected	= 0;
	m_nTopIndex	= 0;
	m_propertySort = xtpPropertyGridSortAlphabetical;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVerb

CXTPPropertyGridVerb::CXTPPropertyGridVerb()
{
	m_nID	= 0;
	m_nIndex = -1;
	m_pVerbs = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVerbs

CXTPPropertyGridVerbs::CXTPPropertyGridVerbs()
{
	m_pGrid = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPropertyGridVerbs::~CXTPPropertyGridVerbs()
{
	for (int i = 0; i < m_arrVerbs.GetSize(); i++)
		m_arrVerbs[i]->InternalRelease();
}

void CXTPPropertyGridVerbs::RemoveAll()
{
	if (IsEmpty())
		return;

	for (int i = 0; i < m_arrVerbs.GetSize(); i++)
		m_arrVerbs[i]->InternalRelease();

	m_arrVerbs.RemoveAll();

	m_pGrid->OnVerbsChanged();
}

int CXTPPropertyGridVerbs::GetCount() const
{
	return (int)m_arrVerbs.GetSize();
}

void CXTPPropertyGridVerbs::Add(LPCTSTR strCaption, UINT nID)
{
	CXTPPropertyGridVerb* pVerb = new CXTPPropertyGridVerb();

	pVerb->m_nID		= XTPToInt(nID);
	pVerb->m_strCaption = strCaption;
	pVerb->m_pVerbs		= this;
	pVerb->m_rcPart.SetRectEmpty();
	pVerb->m_ptClick = CPoint(0);

	pVerb->m_nIndex = XTPToIntChecked(m_arrVerbs.Add(pVerb));

	m_pGrid->OnVerbsChanged();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGrid

IMPLEMENT_DYNAMIC(CXTPPropertyGrid, CWnd)

CXTPPropertyGrid::CXTPPropertyGrid()
{
	RegisterWindowClass();

	m_pView		   = 0;
	m_bViewVisible = TRUE;

	m_hCursorSplit = XTPResourceManager()->LoadCursor(XTP_IDC_VSPLITBAR);
	XTPResourceManager()->AssertValid(m_hCursorSplit != 0);

	m_hCursorHand = AfxGetApp()->LoadStandardCursor(IDC_HAND);

	if (m_hCursorHand == 0)
		m_hCursorHand = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);

	m_pPaintManager = new CXTPPropertyGridPaintManager(this);
	m_pPaintManager->RefreshMetrics();

	m_themeCurrent = xtpPropertyGridThemeDefault;

	m_pVerbs		  = new CXTPPropertyGridVerbs;
	m_pVerbs->m_pGrid = this;

	m_nFocusedVerb	= -1;
	m_bVerbActivate   = FALSE;
	m_bVerbsVisible   = FALSE;
	m_bEnableTooltips = TRUE;

	m_bHelpVisible		 = TRUE;
	m_bToolBarVisible	= FALSE;
	m_bPreSubclassWindow = TRUE;

	m_bTabItems			   = FALSE;
	m_bTabCaptions		   = TRUE;
	m_bVariableItemsHeight = FALSE;

	m_borderStyle = xtpPropertyGridBorderStaticEdge;

	m_bShowInplaceButtonsAlways = FALSE;

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));
	lfIcon.lfWeight = FW_NORMAL;
	lfIcon.lfItalic = FALSE;
	VERIFY(m_pPaintManager->GetItemMetrics()->m_xtpFontNormal.CreateFontIndirect(&lfIcon));

	lfIcon.lfWeight = FW_BOLD;
	VERIFY(m_pPaintManager->GetItemMetrics()->m_xtpFontBold.CreateFontIndirect(&lfIcon));

	lfIcon.lfUnderline = 1;
	lfIcon.lfWeight	= FW_NORMAL;
	VERIFY(m_pPaintManager->GetItemMetrics()->m_xtpFontUnderline.CreateFontIndirect(&lfIcon));

	m_bHighlightChanged		  = FALSE;
	m_bHighlightChangedButton = TRUE;

	m_pImageManager = new CXTPImageManager;

	m_pInplaceEdit	= new CXTPPropertyGridInplaceEdit;
	m_pInplaceListBox = new CXTPPropertyGridInplaceList;

	m_bVariableHelpHeight = TRUE;

	m_pToolTipContext = new CXTPToolTipContext;

	m_bHideSelection = FALSE;

	m_bMultiSelect = FALSE;

	m_pMarkupContext = NULL;

	m_bAllowEdit = TRUE;

#ifdef _XTP_ACTIVEX
	m_bFocusNext = 0;
#endif
}

BOOL CXTPPropertyGrid::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance, XTPPROPERTYGRID_CLASSNAME, 0);
}

CXTPPropertyGrid::~CXTPPropertyGrid()
{
	if (m_pView)
	{
		delete m_pView;
	}

	CMDTARGET_RELEASE(m_pVerbs);
	CMDTARGET_RELEASE(m_pImageManager);

	SAFE_DELETE(m_pInplaceEdit);
	SAFE_DELETE(m_pInplaceListBox);

	CMDTARGET_RELEASE(m_pPaintManager);

	CMDTARGET_RELEASE(m_pToolTipContext);

	XTPMarkupReleaseContext(m_pMarkupContext);
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPPropertyGrid, CWnd)
	//{{AFX_MSG_MAP(CXTPPropertyGrid)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(XTP_IDC_PROPERTYGRID_ALPHABETIC, OnSortAlphabetic)
	ON_COMMAND(XTP_IDC_PROPERTYGRID_CATEGORIZED, OnSortCategorized)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

void CXTPPropertyGrid::SetInplaceEdit(CXTPPropertyGridInplaceEdit* pInplaceEdit)
{
	ASSERT(pInplaceEdit);

	SAFE_DELETE(m_pInplaceEdit);
	m_pInplaceEdit = pInplaceEdit;
}

void CXTPPropertyGrid::SetInplaceList(CXTPPropertyGridInplaceList* pInplaceList)
{
	ASSERT(pInplaceList);

	SAFE_DELETE(m_pInplaceListBox);
	m_pInplaceListBox = pInplaceList;
}

CXTPPropertyGridView* CXTPPropertyGrid::CreateView() const
{
	// Starting from version 19.2 PropertyGrid supports scroll bar themes,
	// however due to technical challenges having scroll bar themes enabled
	// has also introduced a number of issues for various use cases.
	// If you are affected by any of those issues would prefer to fallback
	// to pre-19.2 behavior at the price of disabling scroll bar themes then
	// you can either uncomment the XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES
	// macro at the beginning of XTPPropertyGrid.h file or specify that macro
	// as defined in the C++ compiler properties of the ToolkitPro or PropertyGrid
	// library project for selected configurations then re-build the ToolkitPro or
	// PropertyGrid projects in order the change to take an effect.

#ifndef XTP_PROPERTY_GRID_DISABLE_SCROLLBAR_THEMES
	return new CXTPScrollablePropertyGridListBox();
#else
	return new CXTPPropertyGridView();
#endif
}

CXTPPropertyGridView& CXTPPropertyGrid::GetGridView() const
{
	if (NULL == m_pView)
	{
		m_pView			 = CreateView();
		m_pView->m_pGrid = (CXTPPropertyGrid*)this;
	}

	CXTPScrollablePropertyGridListBox* pScrollableView =
		DYNAMIC_DOWNCAST(CXTPScrollablePropertyGridListBox, m_pView);
	if (NULL != pScrollableView)
	{
		CXTPPropertyGridView* pEmbeddedView = DYNAMIC_DOWNCAST(CXTPPropertyGridView,
															   pScrollableView->GetControl());
		if (NULL != pEmbeddedView)
		{
			return *pEmbeddedView;
		}
	}

	return *m_pView;
}

CXTPPropertyGridItem* CXTPPropertyGrid::FindItem(LPCTSTR strCaption) const
{
	return GetGridView().m_pCategories->FindItem(strCaption);
}

CXTPPropertyGridItem* CXTPPropertyGrid::FindItem(UINT nID) const
{
	return GetGridView().m_pCategories->FindItem(nID);
}

CXTPPropertyGridItem* CXTPPropertyGrid::AddCategory(LPCTSTR strCaption,
													CXTPPropertyGridItem* pCategory)
{
	return GetGridView().AddCategory(strCaption, pCategory);
}

CXTPPropertyGridItem* CXTPPropertyGrid::InsertCategory(int nIndex, LPCTSTR strCaption,
													   CXTPPropertyGridItem* pCategory)
{
	return GetGridView().InsertCategory(nIndex, strCaption, pCategory);
}

void CXTPPropertyGrid::SetPropertySort(XTPPropertyGridSortOrder sort)
{
	GetGridView().SetPropertySort(sort);
}

XTPPropertyGridSortOrder CXTPPropertyGrid::GetPropertySort() const
{
	return GetGridView().m_properetySort;
}

int CXTPPropertyGrid::GetCount() const
{
	return GetGridView().GetCount();
}

void CXTPPropertyGrid::SetVariableSplitterPos(BOOL bVariable)
{
	GetGridView().m_bVariableSplitterPos = bVariable;
}

BOOL CXTPPropertyGrid::GetVariableSplitterPos() const
{
	return GetGridView().m_bVariableSplitterPos;
}

UINT CXTPPropertyGrid::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

BOOL CXTPPropertyGrid::CreateGridView(DWORD dwListStyle)
{
	CXTPEmptyRect rect;

	if (dwListStyle == NULL)
	{
		dwListStyle = DWORD(m_bVariableItemsHeight ? LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT
												   : LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT);
	}

	if (m_bMultiSelect)
	{
		dwListStyle |= LBS_MULTIPLESEL | LBS_EXTENDEDSEL;
	}

	// Must be owner draw.
	ASSERT((dwListStyle & (LBS_OWNERDRAWVARIABLE | LBS_OWNERDRAWFIXED)) != 0);

	m_bVariableItemsHeight = (dwListStyle & LBS_OWNERDRAWVARIABLE) != 0;

#if (1400 < _MSC_VER)
	if (!GetGridView().CreateEx(0, _T("LISTBOX"), NULL,
								WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE
									| LBS_NOTIFY | WS_VSCROLL | WS_TABSTOP | dwListStyle,
								rect, this, 0))
#else
	if (!static_cast<CWnd&>(GetGridView())
			 .Create(_T("LISTBOX"), NULL,
					 WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | LBS_NOTIFY
						 | WS_VSCROLL | WS_TABSTOP | dwListStyle,
					 rect, this, 0))
#endif
	{
		TRACE0("Error creating property grid view.\n");
		return FALSE;
	}

	GetGridView().ResetContent();
	SetStandardColors();

	return TRUE;
}

BOOL CXTPPropertyGrid::Create(const RECT& rect, CWnd* pParentWnd, UINT nID,
							  DWORD dwListStyle /*= LBS_OWNERDRAWFIXED|LBS_NOINTEGRALHEIGHT*/)
{
	if (!CreateEx(0, XTPPROPERTYGRID_CLASSNAME, _T(""),
				  WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_GROUP, rect,
				  pParentWnd, nID))
	{
		TRACE0("Error creating property grid.\n");
		return FALSE;
	}

	if (!CreateGridView(dwListStyle))
		return FALSE;

	Reposition(rect.right - rect.left, rect.bottom - rect.top);

	SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				 SWP_SHOWWINDOW | SWP_NOZORDER);

	return TRUE;
}

BOOL CXTPPropertyGrid::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//
	// PreCreateWindow is called when a control is dynamically
	// created. We want to set m_bPreSubclassWindow to FALSE
	// here so the control is initialized from CWnd::Create and
	// not CWnd::PreSubclassWindow.
	//

	m_bPreSubclassWindow = FALSE;

	return TRUE;
}

void CXTPPropertyGrid::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (m_bPreSubclassWindow)
	{
		ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

		// Initialize the control.
		VERIFY(CreateGridView(NULL));

		Reposition();
	}
}

void CXTPPropertyGrid::SetVariableItemsHeight(BOOL bVariable /*= TRUE*/)
{
	if (m_bVariableItemsHeight == bVariable)
		return;

	m_bVariableItemsHeight = bVariable;

	RecreateView();
}

void CXTPPropertyGrid::RecreateView()
{
	if (NULL == m_pView)
		return;

	if (NULL == m_pView->GetSafeHwnd())
		return;

	m_pView->ResetContent();
	m_pView->DestroyWindow();
	delete m_pView;
	m_pView = NULL;

	GetGridView();
	ASSERT(NULL != m_pView);

	DWORD dwListStyle = DWORD(m_bVariableItemsHeight ? LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT
													 : LBS_OWNERDRAWFIXED | LBS_NOINTEGRALHEIGHT);
	if (m_bMultiSelect)
	{
		dwListStyle |= LBS_MULTIPLESEL | LBS_EXTENDEDSEL;
	}

	VERIFY(m_pView->CreateEx(0, _T("LISTBOX"), NULL,
							 WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | LBS_NOTIFY
								 | WS_VSCROLL | WS_TABSTOP | dwListStyle,
							 CXTPEmptyRect(), this, 0));

	m_pView->Refresh();

	Reposition();
}

void CXTPPropertyGrid::EnableMultiSelect(BOOL bMultiSelect)
{
	if (m_bMultiSelect == bMultiSelect)
		return;

	m_bMultiSelect = bMultiSelect;

	RecreateView();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGrid message handlers

BOOL CXTPPropertyGrid::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGrid::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);
	OnPrintClient(reinterpret_cast<WPARAM>(dc.m_hDC), 0);
}

LRESULT CXTPPropertyGrid::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (NULL != pDC)
	{
		m_pPaintManager->FillPropertyGrid(pDC);

		if (NULL != m_pView)
		{
			CXTPWindowRect rc(m_pView);
			ScreenToClient(&rc);

			int dx = XTP_DPI_X(1);
			int dy = XTP_DPI_Y(1);
			rc.left -= dx;
			rc.right += dx;
			rc.top -= dy;
			rc.bottom += dy;

			GetPaintManager()->DrawPropertyGridBorder(pDC, rc, xtpPropertyGridGridWindow);
		}
	}

	return TRUE;
}

CWnd* CXTPPropertyGrid::GetNextGridTabItem(BOOL bForward)
{
	CWnd* pThis = this;
#ifdef _XTP_ACTIVEX
	pThis = GetParent();
#endif
	CWnd* pParent = pThis->GetParent();

	if (!pThis || !pParent)
	{
		ASSERT(FALSE);
		return 0;
	}

	if ((pParent->GetExStyle() & WS_EX_CONTROLPARENT) && (pParent->GetStyle() & WS_CHILD))
	{
		pParent = pParent->GetParent();
	}

	CWnd* pNextItem = pParent->GetNextDlgTabItem(pThis, bForward);

	if (pNextItem == pThis)
		return NULL;

	return pNextItem;
}

void CXTPPropertyGrid::OnNavigate(XTPPropertyGridUI nUIElement, BOOL bForward,
								  CXTPPropertyGridItem* pItem)
{
	int nNextElement = bForward ? +1 : -1;
	int nUI			 = nUIElement + nNextElement;

	CXTPPropertyGridView& wndView = GetGridView();

	if (pItem == NULL)
		pItem = wndView.GetSelectedItem();

	if (m_bTabItems)
		nUIElement = (XTPPropertyGridUI)-3;

	while (nUI != nUIElement)
	{
		if (nUI == xtpPropertyGridUIViewPrev)
		{
			nUI += nNextElement;

			if (m_bTabItems)
			{
				int nIndex = !bForward && pItem ? pItem->m_nIndex - 1 : 0;
				if (nIndex >= 0)
				{
					wndView.SetFocus();
					wndView.SetCurSel(nIndex);
					wndView.OnSelectionChanged();

					if (bForward)
						return;

					nUI   = xtpPropertyGridUIInplaceButton;
					pItem = wndView.GetSelectedItem();
				}
			}
		}
		if (nUI == xtpPropertyGridUIView)
		{
			if (m_bTabCaptions)
			{
				wndView.SetFocus();
				wndView.FocusInplaceButton(NULL);
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIInplaceEdit)
		{
			if (pItem && pItem->GetFlags() & xtpPropertyGridItemHasEdit)
			{
				pItem->OnSelect();
				pItem->SetFocusToInplaceControl();
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIInplaceControl)
		{
			if (pItem && pItem->OnInpaceControlFocus(bForward))
			{
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIInplaceButton)
		{
			CXTPPropertyGridInplaceButtons* pButtons = pItem ? pItem->GetInplaceButtons() : NULL;
			if (pItem && pButtons && pButtons->GetCount() > 0)
			{
				int nIndex = bForward ? -1 : pButtons->GetCount();

				if (wndView.m_pFocusedButton && wndView.m_pFocusedButton->GetItem() == pItem)
					nIndex = wndView.m_pFocusedButton->GetIndex();

				for (;;)
				{
					if (bForward)
						nIndex++;
					else
						nIndex--;

					CXTPPropertyGridInplaceButton* pButton = pButtons->GetAt(nIndex);
					if (!pButton)
						break;

					if (!pItem->IsInplaceButtonVisible(pButton) || !pButton->GetTabStop())
						continue;

					wndView.FocusInplaceButton(pButton);
					return;
				}
			}
			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIViewNext)
		{
			if (m_bTabItems)
			{
				int nIndex = bForward ? (pItem ? pItem->m_nIndex + 1 : 0) : wndView.GetCount() - 1;
				if (nIndex < wndView.GetCount())
				{
					wndView.SetFocus();
					wndView.SetCurSel(nIndex);
					wndView.OnSelectionChanged();

					if (bForward)
					{
						if (!m_bTabCaptions)
						{
							nUI = xtpPropertyGridUIView;
						}
						else
							return;
					}
					pItem = wndView.GetSelectedItem();
				}
			}

			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIVerb)
		{
			if (IsVerbsVisible())
			{
				m_nFocusedVerb  = bForward ? 0 : m_pVerbs->GetCount() - 1;
				m_bVerbActivate = TRUE;
				SetFocus();
				m_bVerbActivate = FALSE;

				Invalidate(FALSE);
				UpdateWindow();

				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpPropertyGridUIParentNext || nUI == xtpPropertyGridUIParentPrev)
		{
#ifdef _XTP_ACTIVEX
			if (m_bFocusNext == -1)
			{
				m_bFocusNext = 1;
				return;
			}
#endif

			CWnd* pWndNext = GetNextGridTabItem(!bForward);

			if (pWndNext != NULL)
			{
				pWndNext->SetFocus();
				return;
			}

			nUI = (nUI == xtpPropertyGridUIParentNext) ? xtpPropertyGridUIViewPrev
													   : xtpPropertyGridUIVerb;
		}
		if (nUI < xtpPropertyGridUIParentPrev || nUI > xtpPropertyGridUIParentNext)
		{
			ASSERT(FALSE);
			return;
		}
	}
}

void CXTPPropertyGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_nFocusedVerb != -1 && IsVerbsVisible())
	{
		if (m_nFocusedVerb >= m_pVerbs->GetCount())
			m_nFocusedVerb = m_pVerbs->GetCount() - 1;

		BOOL bForward = GetKeyState(VK_SHIFT) >= 0;
		CXTPDrawHelpers::KeyToLayout(this, nChar);

		if ((nChar == VK_TAB) && bForward && (m_nFocusedVerb == m_pVerbs->GetCount() - 1))
		{
			OnNavigate(xtpPropertyGridUIVerb, TRUE, NULL);
			return;
		}
		else if ((nChar == VK_TAB) && !bForward && (m_nFocusedVerb == 0))
		{
			OnNavigate(xtpPropertyGridUIVerb, FALSE, NULL);
			return;
		}
		else if (nChar == VK_RIGHT || nChar == VK_DOWN || ((nChar == VK_TAB) && bForward))
		{
			m_nFocusedVerb++;

			if (m_nFocusedVerb >= m_pVerbs->GetCount())
				m_nFocusedVerb = 0;

			Invalidate(FALSE);
			UpdateWindow();
		}
		else if (nChar == VK_LEFT || nChar == VK_UP || nChar == VK_TAB)
		{
			m_nFocusedVerb--;

			if (m_nFocusedVerb < 0)
				m_nFocusedVerb = m_pVerbs->GetCount() - 1;

			Invalidate(FALSE);
			UpdateWindow();
		}
		else if (nChar == VK_RETURN)
		{
			CRect rcPart = m_pVerbs->GetAt(m_nFocusedVerb)->GetPart();
			OnVerbClick(m_nFocusedVerb, CPoint(rcPart.left, rcPart.bottom));
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGrid::OnKillFocus(CWnd* pNewWnd)
{
	if (m_nFocusedVerb != -1)
	{
		m_nFocusedVerb = -1;
		Invalidate(FALSE);
		UpdateWindow();
	}

	CWnd::OnKillFocus(pNewWnd);
}

void CXTPPropertyGrid::OnSetFocus(CWnd* pOldWnd)
{
	if (!m_bVerbActivate)
	{
		if (pOldWnd && pOldWnd == GetNextGridTabItem(FALSE) && GetKeyState(VK_SHIFT) < 0)
		{
			OnNavigate(xtpPropertyGridUIParentNext, FALSE, NULL);
		}
		else if (pOldWnd && pOldWnd == GetNextGridTabItem(TRUE) && GetKeyState(VK_SHIFT) >= 0)
		{
			OnNavigate(xtpPropertyGridUIParentPrev, TRUE, NULL);
		}
		else
		{
			GetGridView().SetFocus();
		}
	}
	else
	{
		CWnd::OnSetFocus(pOldWnd);
	}
}

int CXTPPropertyGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CWnd::OnCreate(lpCreateStruct);
}

void CXTPPropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetGridView().GetSafeHwnd())
		return;

	Reposition(cx, cy);
}

int CXTPPropertyGrid::HitTest(CPoint pt)
{
	CXTPClientRect rc(this);

	if (m_bHelpVisible)
	{
		CRect rcSplitter(CPoint(rc.left, rc.bottom - m_pPaintManager->m_nSplitterHeight
											 - m_pPaintManager->m_nHelpHeight),
						 CSize(rc.Width(), m_pPaintManager->m_nSplitterHeight));

		if (m_bVariableHelpHeight && rcSplitter.PtInRect(pt))
			return xtpPropertyGridHitHelpSplitter;

		rc.bottom -= m_pPaintManager->m_nSplitterHeight + m_pPaintManager->m_nHelpHeight;
	}
	if (IsVerbsVisible())
	{
		CRect rcSplitter(CPoint(rc.left, rc.bottom - m_pPaintManager->m_nSplitterHeight
											 - m_pPaintManager->m_nVerbsHeight),
						 CSize(rc.Width(), m_pPaintManager->m_nSplitterHeight));

		if (rcSplitter.PtInRect(pt))
			return xtpPropertyGridHitVerbsSplitter;

		CRect rcVerbs(rc);
		rcVerbs.top = rc.bottom - m_pPaintManager->m_nVerbsHeight;

		if (rcVerbs.PtInRect(pt))
		{
			CWindowDC dc(this);
			int nIndex = m_pPaintManager->HitTestVerbs(&dc, rcVerbs, pt);
			if (nIndex != -1)
			{
				return xtpPropertyGridHitFirstVerb + nIndex;
			}
		}
	}

	return xtpPropertyGridHitError;
}

BOOL CXTPPropertyGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		int nHit = HitTest(point);

		if (nHit == xtpPropertyGridHitHelpSplitter || nHit == xtpPropertyGridHitVerbsSplitter)
		{
			SetCursor(m_hCursorSplit);
			return TRUE;
		}
		if ((nHit != -1) && (nHit >= xtpPropertyGridHitFirstVerb))
		{
			SetCursor(m_hCursorHand);
			return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPPropertyGrid::OnInvertTracker(CRect rect)
{
	ASSERT(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

	// pat-blt without clip children on
	CDC* pDC = GetDC();

	CBrush brush(GetXtremeColor(COLOR_3DFACE));
	CBrush* pBrush = (CBrush*)pDC->SelectObject(&brush);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	pDC->SelectObject(pBrush);
	brush.DeleteObject();

	ReleaseDC(pDC);
}

void CXTPPropertyGrid::OnSelectionChanged(CXTPPropertyGridItem* pItem)
{
	if (!m_hWnd)
		return;

	CXTPClientRect rc(this);
	CRect rcHelp(rc);
	rcHelp.top = rc.bottom - m_pPaintManager->m_nHelpHeight;

	if (pItem)
		SendNotifyMessage(XTP_PGN_SELECTION_CHANGED, (LPARAM)pItem);

	InvalidateRect(rcHelp, FALSE);
}

void CXTPPropertyGrid::OnVerbsChanged()
{
	if (m_bVerbsVisible != IsVerbsVisible() && GetGridView().m_nLockUpdate == 0)
	{
		Reposition();
	}
	else if (GetSafeHwnd() && GetGridView().m_nLockUpdate == 0)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CXTPPropertyGrid::OnVerbClick(int nIndex, CPoint pt)
{
	ClientToScreen(&pt);
	ASSERT(nIndex < m_pVerbs->GetCount());

	CXTPPropertyGridVerb* pVerb = m_pVerbs->GetAt(nIndex);
	pVerb->m_ptClick			= pt;

	SendNotifyMessage(XTP_PGN_VERB_CLICK, (LPARAM)pVerb);
}

void CXTPPropertyGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseMove(AxButtonState(), AxShiftState(), point.x, point.y);
#endif
}

void CXTPPropertyGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPPropertyGrid::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseUp(VK_RBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPPropertyGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef _XTP_ACTIVEX
	HWND hWnd = m_hWnd;
#endif

	if ((m_nFocusedVerb != -1) && (m_nFocusedVerb == HitTest(point) - xtpPropertyGridHitFirstVerb))
	{
		OnVerbClick(m_nFocusedVerb, point);
	}
	else
	{
		CWnd::OnLButtonUp(nFlags, point);
	}

#ifdef _XTP_ACTIVEX
	if (IsWindow(hWnd))
	{
		((COleControl*)GetParent())->FireMouseUp(VK_LBUTTON, AxShiftState(), point.x, point.y);
	}
#endif
}

void CXTPPropertyGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nHitTest = HitTest(point);

	if ((nHitTest != -1) && (nHitTest >= xtpPropertyGridHitFirstVerb))
	{
		m_nFocusedVerb  = nHitTest - xtpPropertyGridHitFirstVerb;
		m_bVerbActivate = TRUE;
		SetFocus();
		m_bVerbActivate = FALSE;

		Invalidate(FALSE);
		UpdateWindow();

		return;
	}
	SetFocus();

	if ((nHitTest == xtpPropertyGridHitHelpSplitter)
		|| (nHitTest == xtpPropertyGridHitVerbsSplitter))
	{
		SetCapture();
		CXTPClientRect rc(this);
		BOOL bHelpHitTest = nHitTest == xtpPropertyGridHitHelpSplitter;

		CRect rcAvail = bHelpHitTest ? CRect(0, XTP_DPI_Y(20) + XTP_DPI_Y(TOOLBAR_HEIGHT), rc.right,
											 rc.bottom)
									 : CRect(0, XTP_DPI_Y(20) + XTP_DPI_Y(TOOLBAR_HEIGHT), rc.right,
											 rc.bottom
												 - (IsHelpVisible()
														? m_pPaintManager->m_nHelpHeight
															  + m_pPaintManager->m_nSplitterHeight
														: 0));

		ModifyStyle(WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, FALSE);

		m_rectTracker.SetRect(0, point.y, rc.Width(), point.y + XTP_DPI_Y(3));

		OnInvertTracker(m_rectTracker);

		BOOL bAccept = FALSE;
		while (GetCapture() == this)
		{
			MSG msg;
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_PAINT)
			{
				OnInvertTracker(m_rectTracker);
				::DispatchMessage(&msg);
				OnInvertTracker(m_rectTracker);
			}
			else if (msg.message == WM_MOUSEMOVE)
			{
				point   = CPoint(msg.lParam);
				point.y = __min(point.y, rcAvail.bottom - XTP_DPI_Y(20));
				point.y = __max((int)point.y, int(rcAvail.top));

				if (m_rectTracker.top != point.y)
				{
					OnInvertTracker(m_rectTracker);
					m_rectTracker.OffsetRect(0, point.y - m_rectTracker.top);
					OnInvertTracker(m_rectTracker);
				}
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
				break;
			else if (msg.message == WM_LBUTTONUP)
			{
				bAccept = TRUE;
				break;
			}
			else
				::DispatchMessage(&msg);
		}

		ReleaseCapture();

		if (bAccept)
		{
			if (bHelpHitTest)
			{
				m_pPaintManager->m_nHelpHeight = rcAvail.bottom - m_rectTracker.top - XTP_DPI_Y(2);
			}
			else
			{
				m_pPaintManager->m_nVerbsHeight = rcAvail.bottom - m_rectTracker.top - XTP_DPI_Y(2);
			}

			Reposition(rc.Width(), rc.Height());
		}

		Invalidate(FALSE);
		UpdateWindow();

		ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS, FALSE);

		return;
	}

	CWnd::OnLButtonDown(nFlags, point);

#ifdef _XTP_ACTIVEX
	((COleControl*)GetParent())->FireMouseDown(VK_LBUTTON, AxShiftState(), point.x, point.y);
#endif
}

void CXTPPropertyGrid::Reposition()
{
	if (GetSafeHwnd() == 0)
		return;

	CXTPClientRect rc(this);
	Reposition(rc.Width(), rc.Height());
}

void CXTPPropertyGrid::Reposition(int cx, int cy)
{
	CXTPPropertyGridView* pView = &GetGridView();

	if (pView->m_nLockUpdate > 0)
		return;

	if (GetSafeHwnd() == 0)
		return;

	CRect rcView(0, 0, cx, cy);

	int nMinViewHeight = XTP_DPI_Y(20) + (m_bToolBarVisible ? XTP_DPI_Y(TOOLBAR_HEIGHT) : 0);

	if (m_bHelpVisible)
	{
		if (cy > nMinViewHeight
			&& cy - m_pPaintManager->m_nHelpHeight - m_pPaintManager->m_nSplitterHeight
				   < nMinViewHeight
			&& m_bVariableHelpHeight)
		{
			m_pPaintManager->m_nHelpHeight = max(0, cy - m_pPaintManager->m_nSplitterHeight
														- nMinViewHeight);
		}

		rcView.bottom -= m_pPaintManager->m_nHelpHeight + m_pPaintManager->m_nSplitterHeight;
	}

	if (IsVerbsVisible())
	{
		rcView.bottom -= m_pPaintManager->m_nVerbsHeight + m_pPaintManager->m_nSplitterHeight;
	}

	m_bVerbsVisible = IsVerbsVisible();

	if (m_bToolBarVisible)
	{
		ASSERT(m_wndToolbar.GetSafeHwnd());
		CRect rcToolBar(0, 0, cx, XTP_DPI_Y(TOOLBAR_HEIGHT));
		m_wndToolbar.MoveWindow(rcToolBar);

		rcView.top += XTP_DPI_Y(TOOLBAR_HEIGHT);
	}

	if (!m_bViewVisible)
	{
		if (m_bVerbsVisible)
			m_pPaintManager->m_nVerbsHeight += rcView.Height() + m_pPaintManager->m_nSplitterHeight;

		m_pView->ShowWindow(SW_HIDE);
	}
	else
	{
		if (m_pView->IsKindOf(RUNTIME_CLASS(CXTPScrollablePropertyGridListBox)))
		{
			int dx = XTP_DPI_X(1);
			int dy = XTP_DPI_Y(1);
			rcView.left += dx;
			rcView.right -= dx;
			rcView.top += dy;
			rcView.bottom -= dy;
		}

		m_pView->MoveWindow(rcView);
		m_pView->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	Invalidate(FALSE);
	UpdateWindow();
}

void CXTPPropertyGrid::CreateToolbar()
{
#if _MSC_VER < 1200
	m_wndToolbar.Create(this,
						TBSTYLE_TRANSPARENT | TBSTYLE_FLAT | WS_CHILD | WS_VISIBLE | WS_BORDER, 0);
#else
	m_wndToolbar.CreateEx(this, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT,
						  WS_BORDER | WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP);
#endif

	UINT ResourceID = XTPToUInt(XTPDpiHelper()->SelectDpiSpecific(
		XTP_IDR_PROPERTYGRID_TOOLBAR_100, XTP_IDR_PROPERTYGRID_TOOLBAR_125,
		XTP_IDR_PROPERTYGRID_TOOLBAR_150, XTP_IDR_PROPERTYGRID_TOOLBAR_200));

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadToolBar(&m_wndToolbar, ResourceID));
}

void CXTPPropertyGrid::ShowToolBar(BOOL bShow)
{
	if (bShow && !m_wndToolbar.GetSafeHwnd())
	{
		CreateToolbar();
	}
	else
	{
		if (m_wndToolbar.GetSafeHwnd())
			m_wndToolbar.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}

	m_bToolBarVisible = bShow;

	Reposition();
	RefreshToolBarButtons();
}

void CXTPPropertyGrid::RefreshToolBarButtons()
{
	if (m_bToolBarVisible)
	{
		m_wndToolbar.GetToolBarCtrl().SetState(
			XTP_IDC_PROPERTYGRID_CATEGORIZED,
			XTPToUInt(TBSTATE_ENABLED
					  | (GetGridView().m_properetySort == xtpPropertyGridSortCategorized
							 ? TBSTATE_CHECKED
							 : 0)));
		m_wndToolbar.GetToolBarCtrl().SetState(
			XTP_IDC_PROPERTYGRID_ALPHABETIC,
			XTPToUInt(TBSTATE_ENABLED
					  | (GetGridView().m_properetySort == xtpPropertyGridSortAlphabetical
							 ? TBSTATE_CHECKED
							 : 0)));
	}
}

void CXTPPropertyGrid::ShowHelp(BOOL bShow)
{
	m_bHelpVisible = bShow;

	Reposition();
}

void CXTPPropertyGrid::ShowView(BOOL bShow)
{
	m_bViewVisible = bShow;

	Reposition();
}

void CXTPPropertyGrid::OnSortAlphabetic()
{
	SetPropertySort(xtpPropertyGridSortAlphabetical);
}

void CXTPPropertyGrid::OnSortCategorized()
{
	SetPropertySort(xtpPropertyGridSortCategorized);
}

void CXTPPropertyGrid::OnSortChanged()
{
	RefreshToolBarButtons();
	OnSelectionChanged(NULL);
	SendNotifyMessage(XTP_PGN_SORTORDER_CHANGED, (LPARAM)GetDlgCtrlID());
}

LRESULT CXTPPropertyGrid::SendNotifyMessage(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	return GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, wParam, lParam);
}

void CXTPPropertyGrid::SetStandardColors()
{
	m_pPaintManager->GetItemMetrics()->SetDefaultValues();

	m_pPaintManager->RefreshMetrics();
	RedrawControl();
}

void CXTPPropertyGrid::RedrawControl()
{
	if (GetSafeHwnd())
	{
		RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);
		::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0,
					   SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER
						   | SWP_FRAMECHANGED);

		::SetWindowPos(GetGridView().m_hWnd, 0, 0, 0, 0, 0,
					   SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER
						   | SWP_FRAMECHANGED);
	}
}

void CXTPPropertyGrid::SetCustomColors(COLORREF clrHelpBack, COLORREF clrHelpFore,
									   COLORREF clrViewLine, COLORREF clrViewBack,
									   COLORREF clrViewFore, COLORREF clrCategoryFore)
{
	CXTPPropertyGridItemMetrics* pMetrics = m_pPaintManager->GetItemMetrics();

	pMetrics->m_clrHelpBack.SetCustomValue(clrHelpBack);
	pMetrics->m_clrHelpFore.SetCustomValue(clrHelpFore);
	pMetrics->m_clrLine.SetCustomValue(clrViewLine);
	pMetrics->m_clrBack.SetCustomValue(clrViewBack);
	pMetrics->m_clrFore.SetCustomValue(clrViewFore);
	pMetrics->m_clrCategoryFore.SetCustomValue(clrCategoryFore);

	RedrawControl();
}

void CXTPPropertyGrid::ResetContent()
{
	m_pVerbs->RemoveAll();
	GetGridView().ResetContent();

	GetGridView().m_nLockUpdate++;
	GetGridView().m_pCategories->Clear();
	GetGridView().m_nLockUpdate--;
}

CXTPPropertyGridItems* CXTPPropertyGrid::GetCategories() const
{
	return GetGridView().m_pCategories;
}

CXTPPropertyGridItem* CXTPPropertyGrid::AddCategory(int nID, CXTPPropertyGridItem* pCategory)
{
	CString strCaption;
	VERIFY(strCaption.LoadString(XTPToUInt(nID)));
	CXTPPropertyGridItem* pItem = GetGridView().AddCategory(strCaption, pCategory);
	pItem->SetID(XTPToUInt(nID));
	return pItem;
}

CXTPPropertyGridItem* CXTPPropertyGrid::InsertCategory(int nIndex, int nID,
													   CXTPPropertyGridItem* pCategory)
{
	CString strCaption;
	VERIFY(strCaption.LoadString(XTPToUInt(nID)));
	CXTPPropertyGridItem* pItem = GetGridView().InsertCategory(nIndex, strCaption, pCategory);
	pItem->SetID(XTPToUInt(nID));
	return pItem;
}

void CXTPPropertyGrid::SetFont(CFont* pFont)
{
	ASSERT_VALID(pFont);

	LOGFONT lf;
	if (pFont->GetLogFont(&lf))
	{
		CXTPPropertyGridItemMetrics* pMetrics = m_pPaintManager->GetItemMetrics();

		pMetrics->m_xtpFontNormal.DeleteObject();
		pMetrics->m_xtpFontBold.DeleteObject();
		pMetrics->m_xtpFontUnderline.DeleteObject();

		lf.lfWeight = FW_NORMAL;
		VERIFY(pMetrics->m_xtpFontNormal.CreateFontIndirect(&lf));

		lf.lfWeight = FW_BOLD;
		VERIFY(pMetrics->m_xtpFontBold.CreateFontIndirect(&lf));

		if (m_pInplaceEdit->GetSafeHwnd())
		{
			m_pInplaceEdit->SetFont(&pMetrics->m_xtpFontNormal);
		}

		lf.lfUnderline = 1;
		lf.lfWeight	= FW_NORMAL;
		VERIFY(pMetrics->m_xtpFontUnderline.CreateFontIndirect(&lf));

		Refresh();
		Reposition();
	}
}

void CXTPPropertyGrid::Refresh()
{
	GetGridView().SetPropertySort(GetPropertySort(), TRUE);
}

void CXTPPropertyGrid::SaveExpandingState(CXTPPropertyGridItems* pItems,
										  CXTPPropertyGridUpdateContext& context)
{
	int nTopIndex = GetGridView().GetTopIndex();

	for (int nItem = 0; nItem < pItems->GetCount(); nItem++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(nItem);

		if (pItem->GetID() != 0)
		{
			if (pItem->HasChilds())
			{
				BOOL bExpanded = pItem->IsExpanded();
				context.m_mapState.SetAt(pItem->GetID(), bExpanded);
			}

			if (pItem->IsSelected())
				context.m_nSelected = pItem->GetID();
			if (pItem->GetIndex() == nTopIndex)
				context.m_nTopIndex = pItem->GetID();
		}
		SaveExpandingState(pItem->GetChilds(), context);
	}
}

void CXTPPropertyGrid::RestoreExpandingState(CXTPPropertyGridItems* pItems,
											 CXTPPropertyGridUpdateContext& context)
{
	for (int nItem = 0; nItem < pItems->GetCount(); nItem++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(nItem);

		BOOL bExpanded;
		if (pItem->GetID() != 0 && context.m_mapState.Lookup(pItem->GetID(), bExpanded))
		{
			if (bExpanded)
				pItem->Expand();
			else
				pItem->Collapse();
		}
		RestoreExpandingState(pItem->GetChilds(), context);
	}
}

void CXTPPropertyGrid::BeginUpdate(CXTPPropertyGridUpdateContext& context, BOOL bResetContent)
{
	GetGridView().m_nLockUpdate = 1;
	GetGridView().SetRedraw(FALSE);

	context.m_nSelected	= 0;
	context.m_nTopIndex	= 0;
	context.m_propertySort = GetPropertySort();

	if (bResetContent)
	{
		SaveExpandingState(GetCategories(), context);
		ResetContent();
	}
	else
	{
		GetGridView().ResetContent();
	}
}

void CXTPPropertyGrid::EndUpdate(CXTPPropertyGridUpdateContext& context)
{
	RestoreExpandingState(GetCategories(), context);
	GetGridView().m_nLockUpdate = 0;

	GetGridView().SetPropertySort(context.m_propertySort, TRUE, FALSE);
	if (context.m_nSelected > 0 || context.m_nTopIndex > 0)
	{
		for (int i = 0; i < GetGridView().GetCount(); i++)
		{
			CXTPPropertyGridItem* pItem = GetGridView().GetItem(i);

			if (context.m_nTopIndex > 0 && pItem && pItem->GetID() == context.m_nTopIndex)
			{
				SetTopIndex(pItem->GetIndex());
				context.m_nTopIndex = 0;
				if (context.m_nSelected == 0)
					break;
			}

			if (context.m_nSelected > 0 && pItem && pItem->GetID() == context.m_nSelected)
			{
				pItem->Select();
				break;
			}
		}
	}

	GetGridView().SetRedraw(TRUE);

	Reposition();
}

void CXTPPropertyGrid::SetVerbsHeight(int nHeight)
{
	ASSERT(nHeight > 0);
	m_pPaintManager->m_nVerbsHeight = nHeight;

	Reposition();
}

int CXTPPropertyGrid::GetVerbsHeight() const
{
	return m_pPaintManager->m_nVerbsHeight;
}

void CXTPPropertyGrid::SetHelpHeight(int nHeight)
{
	ASSERT(nHeight > 0);
	m_pPaintManager->m_nHelpHeight = nHeight;

	Reposition();
}

void CXTPPropertyGrid::SetViewDivider(double dDivider)
{
	ASSERT(dDivider > 0 && dDivider < 1);
	GetGridView().m_dDivider = dDivider;

	Reposition();
}

double CXTPPropertyGrid::GetViewDivider() const
{
	return GetGridView().m_dDivider;
}

void CXTPPropertyGrid::SetViewDividerPos(int nDivider, BOOL bLockDivider /*= FALSE*/)
{
	if (bLockDivider)
	{
		GetGridView().LockDivider();
	}

	GetGridView().SetDividerPos(nDivider);

	Reposition();
}

CXTPPropertyGridItem* CXTPPropertyGrid::GetSelectedItem() const
{
	return GetGridView().GetSelectedItem();
}

CXTPPropertyGridItem* CXTPPropertyGrid::GetItem(int nIndex) const
{
	return GetGridView().GetItem(nIndex);
}

int CXTPPropertyGrid::GetSelectedItems(CArray<int, int>* pItems) const
{
	CXTPPropertyGridView* pView = &GetGridView();

	if (pView->GetStyle() & LBS_MULTIPLESEL)
	{
		int nCount = pView->GetSelCount();
		if (!pItems)
			return nCount;

		pItems->SetSize(nCount);
		pView->GetSelItems(nCount, pItems->GetData());
	}
	else
	{
		CXTPPropertyGridItem* pItem = GetSelectedItem();
		if (!pItems)
			return pItem ? 1 : 0;

		if (pItem)
		{
			pItems->RemoveAll();
			pItems->Add(pItem->GetIndex());
		}
	}

	return XTPToInt(pItems->GetSize());
}

int CXTPPropertyGrid::GetTopIndex() const
{
	return GetGridView().GetTopIndex();
}

void CXTPPropertyGrid::SetTopIndex(int nIndex)
{
	GetGridView().SetTopIndex(nIndex);
}

void CXTPPropertyGrid::SetTheme(XTPPropertyGridPaintTheme paintTheme)
{
	switch (paintTheme)
	{
		case xtpPropertyGridThemeDefault:
			SetCustomTheme(new CXTPPropertyGridPaintManager(this));
			break;
		case xtpPropertyGridThemeCool: SetCustomTheme(new CXTPPropertyGridCoolTheme(this));
		case xtpPropertyGridThemeDelphi:
			SetCustomTheme(new CXTPPropertyGridDelphiTheme(this));
			break;
		case xtpPropertyGridThemeSimple:
			SetCustomTheme(new CXTPPropertyGridSimpleTheme(this));
			break;
		case xtpPropertyGridThemeNativeWinXP:
			SetCustomTheme(new CXTPPropertyGridNativeXPTheme(this));
			break;
		case xtpPropertyGridThemeOfficeXP:
			SetCustomTheme(new CXTPPropertyGridOfficeXPTheme(this));
			break;
		case xtpPropertyGridThemeOffice2003:
			SetCustomTheme(new CXTPPropertyGridOffice2003Theme(this));
			break;
		case xtpPropertyGridThemeOffice2007:
			SetCustomTheme(new CXTPPropertyGridOffice2007Theme(this));
			break;
		case xtpPropertyGridThemeOffice2013:
			SetCustomTheme(new CXTPPropertyGridOffice2013Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2005:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2005Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2010:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2010Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2012Light:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2012Theme(this, TRUE));
			break;
		case xtpPropertyGridThemeVisualStudio2012Dark:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2012Theme(this, FALSE));
			break;
		case xtpPropertyGridThemeVisualStudio2015:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2015Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2017:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2017Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2019:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2019Theme(this));
			break;
		case xtpPropertyGridThemeVisualStudio2022:
			SetCustomTheme(new CXTPPropertyGridVisualStudio2022Theme(this));
			break;
		case xtpPropertyGridThemeNativeWindows10:
			SetCustomTheme(new CXTPPropertyGridNativeWindows10Theme(this));
			break;
		default: ASSERT("Unexpected theme identifier is being used" && FALSE); break;
	}

	m_themeCurrent = paintTheme;
}

void CXTPPropertyGrid::SetCustomTheme(CXTPPropertyGridPaintManager* pPaintManager)
{
	ASSERT(NULL != pPaintManager);

	CXTPPropertyGridItemMetrics* pMetrics = m_pPaintManager->m_pMetrics;
	m_pPaintManager->m_pMetrics			  = 0;

	ASSERT(m_pPaintManager);
	CMDTARGET_RELEASE(m_pPaintManager);

	m_pPaintManager = pPaintManager;

	CMDTARGET_RELEASE(m_pPaintManager->m_pMetrics);
	m_pPaintManager->m_pMetrics = pMetrics;
	m_pPaintManager->RefreshMetrics();

	CXTPScrollablePropertyGridListBox* pScrollableListBox =
		DYNAMIC_DOWNCAST(CXTPScrollablePropertyGridListBox, m_pView);
	if (NULL != pScrollableListBox)
	{
		pScrollableListBox->SetScrollBarTheme(pPaintManager->GetScrollBarTheme());
	}

	Reposition();
	RedrawControl();
}

CXTPImageManager* CXTPPropertyGrid::GetImageManager() const
{
	return m_pImageManager ? m_pImageManager : XTPImageManager();
}

void CXTPPropertyGrid::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;
}

void CXTPPropertyGrid::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	m_pPaintManager->RefreshMetrics();
	RedrawControl();
}

BOOL CXTPPropertyGrid::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	CString strTipText;

	// don't handle the message if no string resource found
	if (!XTPResourceManager()->LoadString(&strTipText, (UINT)pNMHDR->idFrom))
		return FALSE;

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
				   SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);

	return TRUE; // message was handled
}

BOOL CXTPPropertyGrid::IsLayoutRTL() const
{
	return GetExStyle() & WS_EX_LAYOUTRTL ? TRUE : FALSE;
}

void CXTPPropertyGrid::SetLayoutRTL(CWnd* pWnd, BOOL bRTLLayout)
{
	pWnd->ModifyStyleEx(bRTLLayout ? 0 : DWORD(WS_EX_LAYOUTRTL),
						!bRTLLayout ? 0 : DWORD(WS_EX_LAYOUTRTL));

	// walk through HWNDs to avoid creating temporary CWnd objects
	// unless we need to call this function recursively
	for (CWnd* pChild = pWnd->GetWindow(GW_CHILD); pChild != NULL;
		 pChild		  = pChild->GetWindow(GW_HWNDNEXT))
	{
		// send to child windows after parent
		SetLayoutRTL(pChild, bRTLLayout);
	}
}

void CXTPPropertyGrid::SetLayoutRTL(BOOL bRightToLeft)
{
	if (!XTPSystemVersion()->IsLayoutRTLSupported())
		return;

	SetLayoutRTL(this, bRightToLeft);

	if (m_pInplaceEdit && m_pInplaceEdit->GetSafeHwnd())
	{
		m_pInplaceEdit->HideWindow();
		m_pInplaceEdit->m_bDelayCreate = TRUE;
	}

	if (GetSelectedItem())
		GetSelectedItem()->OnSelect();

	Reposition();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE | RDW_ERASE);
}

void CXTPPropertyGrid::AllowEdit(BOOL bAllowEdit)
{
	m_bAllowEdit = bAllowEdit;
	Reposition();
}

void CXTPPropertyGrid::SetBorderStyle(XTPPropertyGridBorderStyle borderStyle)
{
	m_borderStyle = borderStyle;

	Reposition();

	if (GetGridView().GetSafeHwnd())
	{
		GetGridView().SetWindowPos(0, 0, 0, 0, 0,
								   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}

XTPPropertyGridBorderStyle CXTPPropertyGrid::GetBorderStyle() const
{
	return m_borderStyle;
}

BOOL CXTPPropertyGrid::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pMarkupContext)
	{
		CXTPPropertyGridItem* pItem = GetSelectedItem();
		if (pItem && pItem->GetMarkupDescription())
		{
			if (XTPMarkupRelayMessage(pItem->GetMarkupDescription(), message, wParam, lParam,
									  pResult))
				return TRUE;
		}
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPPropertyGrid::EnableMarkup(BOOL bEnable)
{
	if (bEnable && m_pMarkupContext)
		return;

	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd, TRUE);
	}

	Refresh();
}

void CXTPPropertyGrid::RefeshBindings(CXTPPropertyGridItem* pItem)
{
	if (pItem)
	{
		pItem->OnBeforeInsert();
	}

	CXTPPropertyGridItems* pItems = pItem ? pItem->GetChilds() : GetCategories();

	for (int i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItem* pChild = pItems->GetAt(i);
		RefeshBindings(pChild);
	}
}

COLORREF CXTPPropertyGrid::GetHelpBackColor() const
{
	return m_pPaintManager->GetItemMetrics()->m_clrHelpBack;
}

COLORREF CXTPPropertyGrid::GetHelpForeColor() const
{
	return m_pPaintManager->GetItemMetrics()->m_clrHelpFore;
}

int CXTPPropertyGrid::GetHelpHeight() const
{
	return m_pPaintManager->m_nHelpHeight;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPropertyGridUpdateContext, CXTPCmdTarget)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridUpdateContext, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridUpdateContext, XTPDIID_IPropertyGridUpdateContext, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridUpdateContext, XTPDIID_IPropertyGridUpdateContext)

BEGIN_DISPATCH_MAP(CXTPPropertyGridVerb, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPPropertyGridVerb, "Caption", DISPID_VALUE, m_strCaption, VT_BSTR)
	DISP_PROPERTY_ID(CXTPPropertyGridVerb, "Id", 1, m_nID, VT_I4)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridVerb, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridVerb, XTPDIID_IPropertyGridVerb, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridVerb, XTPDIID_IPropertyGridVerb)

BEGIN_DISPATCH_MAP(CXTPPropertyGridVerbs, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPPropertyGridVerbs, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridVerbs, "Clear", 3, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridVerbs, "Add", 5, Add, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridVerbs, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridVerbs, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPPropertyGridVerbs, CXTPCmdTarget)
	INTERFACE_PART(CXTPPropertyGridVerbs, XTPDIID_IPropertyGridVerbs, Dispatch)
	// INTERFACE_PART(CXTPPropertyGridVerbs, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridVerbs, XTPDIID_IPropertyGridVerbs)

LPDISPATCH CXTPPropertyGridVerbs::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}

int CXTPPropertyGridVerbs::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPPropertyGridVerbs::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPPropertyGridVerb* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPPropertyGridVerbs)

#endif
