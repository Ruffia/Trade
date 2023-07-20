// XTPGalleryListBox.cpp : implementation file
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPIntel80Helpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemMetrics.h"

#ifdef _XTP_INCLUDE_RIBBON
#	include "Ribbon/Resource.h"
#endif
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBarsOptions.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPControlGallery.h"
#include "CommandBars/XTPRecentFileList.h"
#include "CommandBars/Gallery/XTPControlGalleryItem.h"
#include "CommandBars/Gallery/XTPControlGalleryItems.h"
#include "CommandBars/Gallery/XTPGalleryListBox.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifndef DI_NOMIRROR
#	define DI_NOMIRROR 0x10
#endif DI_NOMIRROR

#define ID_ICON_PINNED 1000
#define ID_ICON_UNPINNED 1001

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CXTPGalleryListBoxItems : public CXTPControlGalleryItems
{
public:
	CXTPGalleryListBoxItems(CXTPGalleryListBox* pParent)
		: CXTPControlGalleryItems(NULL, pParent->GetCommandBars())
	{
		m_pParent = pParent;
	}
	void OnItemsChanged()
	{
		CXTPControlGallery* pGallery = m_pParent->GetGallery();
		pGallery->OnItemsChanged();
	}

	CXTPGalleryListBox* GetListBox() const
	{
		return m_pParent;
	}

protected:
	CXTPGalleryListBox* m_pParent;
};

class CXTPControlListBoxGallery : public CXTPControlGallery
{
public:
	CXTPControlListBoxGallery(CXTPGalleryListBox* pParent)
	{
		m_pParent = pParent;
	}
	virtual CXTPControlGalleryItems* GetItems() const
	{
		return m_pParent->GetItems();
	}

	void OnExecute()
	{
		CXTPControlGalleryItem* pItem = GetItem(GetSelectedItem());
		if (pItem)
		{
			if (pItem->OnClick())
				return;
		}

		m_pParent->OnExecute();
	}

protected:
	CXTPGalleryListBox* m_pParent;
};

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageList

CXTPGalleryListBox::CXTPGalleryListBox()
	: m_pGallery(NULL)
	, m_pItems(NULL)
{
	CXTPPaintManager::GetInstanceAddRef();

	m_pGallery = new CXTPControlListBoxGallery(this);
	m_pGallery->SetTransparent(TRUE);
	m_pGallery->SetScrollBarAuto(TRUE);
	GetControls()->Add(m_pGallery);

	m_pItems = new CXTPGalleryListBoxItems(this);

	m_pImageManager = m_pItems->GetImageManager();
	m_pImageManager->InternalAddRef();

	SetPaintManager(CXTPPaintManager::CreateTheme(xtpThemeRibbon));

	m_ptLastMousePos = CPoint(-1, -1);
}

CXTPGalleryListBox::~CXTPGalleryListBox()
{
	CMDTARGET_RELEASE(m_pItems);

	CXTPPaintManager::ReleaseInstance();
}

#include "Common/Base/Diagnostic/XTPBeginAfxMap.h"
BEGIN_MESSAGE_MAP(CXTPGalleryListBox, CXTPCommandBar)
	//{{AFX_MSG_MAP(CXTPGalleryListBox)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#include "Common/Base/Diagnostic/XTPEndAfxMap.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageList message handlers

void CXTPGalleryListBox::RecalcLayout()
{
	if (!m_hWnd)
		return;

	CXTPClientRect rc(this);

	m_pGallery->SetRect(rc);
}

void CXTPGalleryListBox::FillCommandBarEntry(CDC* pDC)
{
	CXTPClientRect rc(this);
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));
}

void CXTPGalleryListBox::OnSize(UINT nType, int cx, int cy)
{
	CXTPCommandBar::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CXTPGalleryListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CXTPCommandBar::OnLButtonDown(nFlags, point);
}

void CXTPGalleryListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (OnHookKeyDown(nChar, MAKELPARAM(nRepCnt, nFlags)))
		return;
}

UINT CXTPGalleryListBox::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

BOOL CXTPGalleryListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pGallery->OnHookMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}

void CXTPGalleryListBox::OnExecute()
{
	GetOwner()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
}

void CXTPGalleryListBox::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, TRUE)
#endif
}

void CXTPGalleryListBox::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

#ifdef _XTP_COMMANDBARS_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, FALSE)
#endif
}

BOOL CXTPGalleryListBox::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#ifdef _XTP_COMMANDBARS_ACTIVEX
	if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);
		AxGetParentOleControl(this)->FireMouseDown(message == WM_LBUTTONDOWN
													   ? LEFT_BUTTON
													   : message == WM_RBUTTONDOWN ? RIGHT_BUTTON
																				   : MIDDLE_BUTTON,
												   AxShiftState(), point.x, point.y);
	}

	if (message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_MBUTTONUP)
	{
		CPoint point((DWORD)lParam);
		AxGetParentOleControl(this)->FireMouseUp(message == WM_LBUTTONUP
													 ? LEFT_BUTTON
													 : message == WM_RBUTTONUP ? RIGHT_BUTTON
																			   : MIDDLE_BUTTON,
												 AxShiftState(), point.x, point.y);
	}

#endif

	return CXTPCommandBar::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPGalleryListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_ptLastMousePos == point)
		return;

	m_ptLastMousePos = point;

	CXTPCommandBar::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListBoxPinItem

IMPLEMENT_DYNAMIC(CXTPRecentFileListBoxPinItem, CXTPControlGalleryItem);

CXTPRecentFileListBoxPinItem::CXTPRecentFileListBoxPinItem(CXTPRecentFileListItem* pRecentItem)
{
	m_pRecentItem = pRecentItem;
	SetData((DWORD_PTR)pRecentItem);
}

CSize CXTPRecentFileListBoxPinItem::GetSize()
{
	return XTP_DPI(CSize(20, 36));
}

BOOL CXTPRecentFileListBoxPinItem::OnClick()
{
	m_pRecentItem->SetPinned(!m_pRecentItem->IsPinned());

	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPRecentFileListBox* pListBox = (CXTPRecentFileListBox*)pItems->GetListBox();

	pListBox->BuildItems(m_pRecentItem->GetList());

	return TRUE;
}

void CXTPRecentFileListBoxPinItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem,
										BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	ASSERT_VALID(pGallery);

	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	if (bSelected && m_pRecentItem->IsPinned())
		bPressed = TRUE;

	pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE,
								 xtpBarTypeNormal, xtpBarPopup);

	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPRecentFileListBox* pListBox = (CXTPRecentFileListBox*)pItems->GetListBox();

	CXTPImageManagerIcon* pIcon = pListBox->GetImageManager()->GetImage(
		UINT(m_pRecentItem->IsPinned() ? ID_ICON_PINNED : ID_ICON_UNPINNED));

	if (pIcon)
	{
		CSize sz = (pIcon->IsVectorIcon()
						? CSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON))
						: CXTPPaintManager::AdjustDpiIconSize(pIcon, pIcon->GetExtent(), pGallery,
															  rcItem.Size()));

		pIcon->Draw(pDC,
					CPoint((rcItem.left + rcItem.right - sz.cx) / 2,
						   (rcItem.top + rcItem.bottom - sz.cy) / 2),
					sz);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPGalleryListBoxSeparatorItem

IMPLEMENT_DYNAMIC(CXTPGalleryListBoxSeparatorItem, CXTPControlGalleryItem);

CXTPGalleryListBoxSeparatorItem::CXTPGalleryListBoxSeparatorItem()
{
	m_bLabel = TRUE;
}

CSize CXTPGalleryListBoxSeparatorItem::GetSize()
{
	if (m_szItem.cy != 0)
		return CSize(0, m_szItem.cy);

	return CSize(0, XTP_DPI_Y(14));
}

void CXTPGalleryListBoxSeparatorItem::Draw(CDC* pDC, CXTPControlGallery* /*pGallery*/, CRect rcItem,
										   BOOL /*bEnabled*/, BOOL /*bSelected*/, BOOL /*bPressed*/,
										   BOOL /*bChecked*/)
{
	const unsigned short _cb[] = { 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc };

	CBitmap bmp;
	bmp.CreateBitmap(8, 8, 1, 1, _cb);

	CBrush brush;
	brush.CreatePatternBrush(&bmp);

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(199, 200, 201));

	PatBlt(pDC->GetSafeHdc(), rcItem.left, (rcItem.top + rcItem.bottom) / 2,
		   rcItem.Width() - XTP_DPI_X(10), XTP_DPI_Y(1), PATCOPY);

	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageRecentFileListItem

IMPLEMENT_DYNAMIC(CXTPRecentFileListBoxItem, CXTPControlGalleryItem);

CXTPRecentFileListBoxItem::CXTPRecentFileListBoxItem(CXTPRecentFileListItem* pRecentItem)
	: m_bUseShellIcon(FALSE)
{
	ASSERT(pRecentItem);
	m_pRecentItem = pRecentItem;
	SetData((DWORD_PTR)pRecentItem);

	m_nImage	 = m_pRecentItem ? m_pRecentItem->GetIconId() : -1;
	m_strCaption = m_pRecentItem ? m_pRecentItem->GetPathName() : CString();

	SetToolTip(m_strCaption);
}

CSize CXTPRecentFileListBoxItem::GetSize()
{
	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPGalleryListBox* pListBox = pItems->GetListBox();
	if (!pListBox->GetSafeHwnd())
		return 0;

	CXTPControlGallery* pGallery = pListBox->GetGallery();

	CRect rcItems = pGallery->GetItemsRect();

	return CSize(rcItems.Width() - XTP_DPI_X(30), XTP_DPI_Y(36));
}

void CXTPRecentFileListBoxItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem,
									 BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	CXTPImageManagerIcon* pImage = GetImage();

	pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE,
								 xtpBarTypeNormal, xtpBarPopup);

	if (pImage)
	{
		CPoint pt(rcItem.left + XTP_DPI_X(7), rcItem.top + XTP_DPI_Y(2));
		CSize sz = CXTPPaintManager::AdjustDpiIconSize(pImage, XTPSystemMetrics()->GetIconSize(),
													   pGallery, rcItem.Size());
		pImage->Draw(pDC, pt, sz);
	}
	else
	{
		DWORD dwFlags	= SHGFI_LARGEICON | SHGFI_ICON;
		LPCTSTR lpszFile = m_strCaption;
		DWORD dwFileAttr = 0;

		if (m_bUseShellIcon)
		{
			dwFileAttr = FILE_ATTRIBUTE_NORMAL;
			dwFlags |= SHGFI_USEFILEATTRIBUTES;
			lpszFile = ::PathFindExtension(m_strCaption);
		}

		SHFILEINFO sfi;
		if (SUCCEEDED(::SHGetFileInfo(lpszFile, dwFileAttr, &sfi, sizeof(SHFILEINFO), dwFlags))
			&& sfi.hIcon)
		{
			DrawIconEx(pDC->GetSafeHdc(), rcItem.left + XTP_DPI_X(7), rcItem.top + XTP_DPI_Y(2),
					   sfi.hIcon, XTP_DPI_X(32), XTP_DPI_X(32), 0, 0, DI_NORMAL | DI_NOMIRROR);

			DestroyIcon(sfi.hIcon);
		}
	}

	CRect rcText(rcItem);
	rcText.DeflateRect(XTP_DPI_X(50), XTP_DPI_Y(2), XTP_DPI_X(2), XTP_DPI_Y(2));

	CRect rcFileName(rcText.left, rcText.top, rcText.right, (rcText.bottom + rcText.top) / 2);
	CRect rcFileFolder(rcText.left, rcFileName.bottom, rcText.right, rcText.bottom);

	int nIndex = m_strCaption.ReverseFind(_T('\\'));

	CString strFileName;
	CString strFileFolder;
	if (nIndex != -1)
	{
		strFileName   = m_strCaption.Mid(nIndex + 1);
		strFileFolder = m_strCaption.Left(nIndex);
	}
	else
	{
		strFileName = m_strCaption;
	}

	CXTPFontDC font(pDC, pPaintManager->GetRegularFont());

	pDC->SetTextColor(pPaintManager->m_clrEditTextNormal);
	pDC->DrawText(strFileName, rcFileName,
				  DT_END_ELLIPSIS | DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(pPaintManager->m_clrEditTextDisabled);
	pDC->DrawText(strFileFolder, rcFileFolder,
				  DT_END_ELLIPSIS | DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);
}

int CXTPRecentFileListBoxItem::GetImageIndex()
{
	m_nImage = m_pRecentItem ? m_pRecentItem->GetIconId() : -1;
	return m_nImage;
}

void CXTPRecentFileListBoxItem::SetImageIndex(int nImageIndex)
{
	if (m_pRecentItem)
		m_pRecentItem->SetIconId(nImageIndex);
	m_nImage = nImageIndex;
}

//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListBox

CXTPRecentFileListBox::CXTPRecentFileListBox()
	: m_bUseShellIcon(FALSE)
{
	m_pToolTipContext = new CXTPToolTipContext();

	GetImageManager()->SetVectorIcon(XTPResourceManager()->GetResourceHandle(), _T("RT_XAML"),
									 XTP_XAML_ICON_PIN_FORBIDDEN, ID_ICON_PINNED,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
	GetImageManager()->SetVectorIcon(XTPResourceManager()->GetResourceHandle(), _T("RT_XAML"),
									 XTP_XAML_ICON_PIN, ID_ICON_UNPINNED,
									 XTPToUInt(GetSystemMetrics(SM_CXSMICON)));
}

CRecentFileList* CXTPRecentFileListBox::GetRecentFileList() const
{
#ifndef _XTP_COMMANDBARS_ACTIVEX
	_XTP_USES_PROTECTED_ACCESS(CXTPRecentFileListBox, CWinApp, CRecentFileList*, m_pRecentFileList)
	return _XTP_PROTECTED_ACCESS(CWinApp, AfxGetApp(), m_pRecentFileList);
#else
	CXTPCommandBars* pCommandBars = GetCommandBars();
	return pCommandBars->m_pRecentFileList;
#endif
}

void CXTPRecentFileListBox::BuildItems()
{
	CXTPRecentFileList* pRecentFileList = (CXTPRecentFileList*)GetRecentFileList();
	ASSERT(pRecentFileList && pRecentFileList->m_strOriginal == _T("PinableRecentFileList"));

	if (pRecentFileList && pRecentFileList->m_strOriginal == _T("PinableRecentFileList"))
	{
		BuildItems((CXTPRecentFileList*)pRecentFileList);
	}
}
void CXTPRecentFileListBox::BuildItems(CXTPRecentFileList* pRecentFileList)
{
	ASSERT(pRecentFileList);
	if (!pRecentFileList)
		return;

	DWORD_PTR dwSelectedRecentItem = 0;

	CXTPControlGalleryItem* pSelectedItem = GetGallery()->GetItem(GetGallery()->GetSelectedItem());
	if (pSelectedItem)
	{
		dwSelectedRecentItem = pSelectedItem->GetData();
	}
	pSelectedItem = NULL;

	m_pItems->RemoveAll();

	int nIndex;
	BOOL bAllPinned = TRUE;

	for (nIndex = 0; nIndex < pRecentFileList->m_nSize; nIndex++)
	{
		CXTPRecentFileListItem* pItem = pRecentFileList->GetItem(nIndex);
		if (!pItem)
			break;

		if (pItem->IsPinned())
		{
			CXTPRecentFileListBoxItem* pRFLItem = new CXTPRecentFileListBoxItem(pItem);
			pRFLItem->SetUseShellIcon(m_bUseShellIcon);

			CXTPControlGalleryItem* p = m_pItems->AddItem(pRFLItem, -1, pItem->GetIconId());
			p->SetID(ID_FILE_MRU_FILE1 + nIndex);

			m_pItems->AddItem(new CXTPRecentFileListBoxPinItem(pItem));

			if ((DWORD_PTR)pItem == dwSelectedRecentItem)
			{
				pSelectedItem = p;
			}
		}
		else
		{
			bAllPinned = FALSE;
		}
	}

	if (m_pItems->GetItemCount() && !bAllPinned)
	{
		m_pItems->AddItem(new CXTPGalleryListBoxSeparatorItem());
	}

	for (nIndex = 0; nIndex < pRecentFileList->m_nSize; nIndex++)
	{
		CXTPRecentFileListItem* pItem = pRecentFileList->GetItem(nIndex);
		if (!pItem)
			break;

		if (!pItem->IsPinned())
		{
			CXTPRecentFileListBoxItem* pRFLItem = new CXTPRecentFileListBoxItem(pItem);
			pRFLItem->SetUseShellIcon(m_bUseShellIcon);

			CXTPControlGalleryItem* p = m_pItems->AddItem(pRFLItem, -1, pItem->GetIconId());
			p->SetID(ID_FILE_MRU_FILE1 + nIndex);

			m_pItems->AddItem(new CXTPRecentFileListBoxPinItem(pItem));

			if ((DWORD_PTR)pItem == dwSelectedRecentItem)
			{
				pSelectedItem = p;
			}
		}
	}

	if (pSelectedItem)
	{
		GetGallery()->SetSelectedItem(pSelectedItem->GetIndex());
	}

	Redraw(0, FALSE);
}

void CXTPRecentFileListBox::SetUseShellIcon(BOOL bUseShellIcon)
{
	m_bUseShellIcon = bUseShellIcon;

	int nCount = m_pItems->GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		CXTPRecentFileListBoxItem* pItem = DYNAMIC_DOWNCAST(CXTPRecentFileListBoxItem,
															m_pItems->GetItem(i));
		if (NULL != pItem)
		{
			pItem->SetUseShellIcon(bUseShellIcon);
		}
	}
}

#ifdef _XTP_COMMANDBARS_ACTIVEX

LPDISPATCH CXTPControlGalleryItems::OleAddSeparator()
{
	return XTPGetDispatch(AddItem(new CXTPGalleryListBoxSeparatorItem()));
}

#endif
