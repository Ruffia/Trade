// XTPTaskPanelItem.cpp : implementation of the CXTPTaskPanelItem class.
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
#include "Common/XTPImageManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeArchive.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/ScrollBar/XTPScrollBase.h"
#include "Common/ScrollBar/XTPScrollBarContainer.h"

#include "TaskPanel/XTPTaskPanelDefines.h"
#include "TaskPanel/XTPTaskPanelItem.h"
#include "TaskPanel/XTPTaskPanel.h"
#include "TaskPanel/XTPTaskPanelItems.h"
#include "TaskPanel/XTPTaskPanelPaintManager.h"
#include "TaskPanel/XTPTaskPanelIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelItem

IMPLEMENT_DYNCREATE(CXTPTaskPanelItem, CXTPCmdTarget)

CXTPTaskPanelItem::CXTPTaskPanelItem()
{
	m_pPanel = NULL;
	m_pItems = NULL;

	m_nIconIndex = -1;
	m_nID		 = 0;

	m_bEnabled = TRUE;
	m_typeItem = xtpTaskItemTypeLink;

	m_bAllowDrop = TRUE;
	m_nAllowDrag = xtpTaskItemAllowDragDefault;

	m_rcText.SetRectEmpty();
	m_nTextFormat = 0;
	m_bVisible	= TRUE;

	m_dwData = 0;

	m_rcIconPadding = CXTPTaskPanelPaintManager::rectDefault;

	m_pMarkupUIElement = NULL;

	EnableAutomation();
#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif
}

CXTPTaskPanelItem::~CXTPTaskPanelItem()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

BOOL CXTPTaskPanelItem::IsItemHot() const
{
	return m_bEnabled && m_pPanel && m_pPanel->GetHotItem() == this;
}

BOOL CXTPTaskPanelItem::IsItemPressed() const
{
	return m_pPanel && m_pPanel->m_pItemPressed == this;
}

BOOL CXTPTaskPanelItem::IsItemDragging() const
{
	return m_pPanel && m_pPanel->m_pItemDragging == this;
}

BOOL CXTPTaskPanelItem::IsItemDragOver() const
{
	return m_pPanel && m_pPanel->m_pItemDragOver == this;
}

BOOL CXTPTaskPanelItem::IsItemFocused() const
{
	return m_pPanel && m_bEnabled && m_pPanel->GetFocusedItem() == this;
}

BOOL CXTPTaskPanelItem::IsAcceptFocus() const
{
	return (GetType() != xtpTaskItemTypeText) && m_bEnabled && m_bVisible;
}

void CXTPTaskPanelItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		RedrawPanel();
	}
}
BOOL CXTPTaskPanelItem::GetEnabled() const
{
	return m_bEnabled;
}
UINT CXTPTaskPanelItem::GetID() const
{
	return m_nID;
}

XTPTaskPanelItemType CXTPTaskPanelItem::GetType() const
{
	return m_typeItem;
}

int CXTPTaskPanelItem::GetIconIndex() const
{
	return m_nIconIndex;
}

void CXTPTaskPanelItem::SetID(UINT nID)
{
	m_nID = nID;

	CString strPrompt;
	if (strPrompt.LoadString(nID))
	{
		if (strPrompt.Find('\n') != -1)
		{
			AfxExtractSubString(m_strTooltip, strPrompt, 1);
			AfxExtractSubString(m_strCaption, strPrompt, 0);
		}
		else
		{
			m_strCaption = strPrompt;
		}
	}
}

void CXTPTaskPanelItem::SetID(UINT nID, LPCTSTR lpCaption, LPCTSTR lpTooltip /*= NULL*/)
{
	ASSERT(NULL != lpCaption);

	m_nID		 = nID;
	m_strCaption = lpCaption;

	if (NULL != lpTooltip)
		m_strTooltip = lpTooltip;
}

void CXTPTaskPanelItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;

		XTPMarkupReleaseElement(m_pMarkupUIElement);

		CXTPTaskPanel* pTaskPanel = GetTaskPanel();
		if (pTaskPanel && pTaskPanel->GetMarkupContext())
		{
			m_pMarkupUIElement = XTPMarkupParseText(pTaskPanel->GetMarkupContext(), lpszCaption);
		}

		RepositionPanel();
	}
}

void CXTPTaskPanelItem::OnInserted()
{
	if (m_pMarkupUIElement == 0)
	{
		CXTPTaskPanel* pTaskPanel = GetTaskPanel();
		if (pTaskPanel && pTaskPanel->GetMarkupContext())
		{
			m_pMarkupUIElement = XTPMarkupParseText(pTaskPanel->GetMarkupContext(), m_strCaption);
		}
	}
}

void CXTPTaskPanelItem::SetTooltip(LPCTSTR lpstrTooltip)
{
	m_strTooltip = lpstrTooltip;
}
void CXTPTaskPanelItem::SetType(XTPTaskPanelItemType typeItem)
{
	m_typeItem = typeItem;
	RepositionPanel();
}
void CXTPTaskPanelItem::SetIconIndex(int nIndex)
{
	if (m_nIconIndex != nIndex)
	{
		m_nIconIndex = nIndex;
		RepositionPanel();
	}
}

CXTPTaskPanelPaintManager* CXTPTaskPanelItem::GetPaintManager() const
{
	ASSERT(m_pPanel);
	return m_pPanel ? m_pPanel->GetPaintManager() : NULL;
}

void CXTPTaskPanelItem::EnsureVisible()
{
	SAFE_CALLPTR(m_pPanel, EnsureVisible(this));
}

void CXTPTaskPanelItem::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;
		RepositionPanel();
	}
}

BOOL CXTPTaskPanelItem::IsVisible() const
{
	return this != NULL && m_bVisible;
}

void CXTPTaskPanelItem::RedrawPanel(BOOL bAnimate) const
{
	if (m_pPanel && m_pPanel->GetSafeHwnd())
		m_pPanel->Redraw(GetInvalidateRect(), bAnimate);
}

void CXTPTaskPanelItem::RepositionPanel() const
{
	if (m_pPanel)
		m_pPanel->Reposition();
}

CXTPImageManagerIcon* CXTPTaskPanelItem::GetImage(int nWidth) const
{
	int nIconIndex = GetIconIndex();
	return m_pPanel->GetImageManager()->GetImage(-1 == nIconIndex ? UINT_MAX
																  : XTPToUInt(nIconIndex),
												 nWidth);
}

void CXTPTaskPanelItem::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
		m_pPanel = (CXTPTaskPanel*)pPX->m_dwData;

	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("Tooltip"), m_strTooltip, _T(""));
	PX_Long(pPX, _T("Id"), (long&)m_nID, 0);
	PX_Enum(pPX, _T("Type"), m_typeItem, xtpTaskItemTypeLink);
	PX_Int(pPX, _T("IconIndex"), m_nIconIndex, -1);
	PX_Bool(pPX, _T("Enabled"), m_bEnabled, TRUE);
	PX_Long(pPX, _T("AllowDrag"), m_nAllowDrag, xtpTaskItemAllowDragDefault);
	PX_Bool(pPX, _T("AllowDrop"), m_bAllowDrop, TRUE);

	if (pPX->GetSchema() > _XTP_SCHEMA_95)
	{
		PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
		PX_DPI_Rect(pPX, _T("IconPadding"), m_rcIconPadding,
					CXTPTaskPanelPaintManager::rectDefault);
	}

#ifdef _XTP_ACTIVEX
	PX_Variant(pPX, _T("Tag"), m_oleTag, COleVariant());
#endif

	if (pPX->GetSchema() > _XTP_SCHEMA_100)
	{
		PX_DWord(pPX, _T("Data"), (DWORD&)m_dwData, 0);
	}
}

HGLOBAL CXTPTaskPanelItem::CacheGlobalData()
{
	try
	{
		CSharedFile sharedFile;
		CArchive ar(&sharedFile, CArchive::store);

		CRuntimeClass* pClass = GetRuntimeClass();
		ASSERT(pClass != NULL);

		ar.WriteClass(pClass);

		CXTPPropExchangeArchive px(ar);
		DoPropExchange(&px);

		ar.Close();

		return sharedFile.Detach();
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. OLE exception: %x\r\n"), pEx->m_sc);
		pEx->Delete();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. Archive exception\r\n"));
		pEx->Delete();
	}

	return NULL;
}

CXTPTaskPanelItem* CXTPTaskPanelItem::CreateFromOleData(COleDataObject* pDataObject)
{
	ASSERT(pDataObject != NULL);
	if (!pDataObject)
		return NULL;

	CFile* pFile = pDataObject->GetFileData(CXTPTaskPanel::GetClipboardFormat());
	if (!pFile)
		return NULL;

	CXTPTaskPanelItem* pItem = CreateFromOleFile(pFile);

	delete pFile;

	return pItem;
}

CXTPTaskPanelItem* CXTPTaskPanelItem::CreateFromOleFile(CFile* pFile)
{
	ASSERT(pFile != NULL);

	CXTPTaskPanelItem* pItem = NULL;

	try
	{
		CArchive ar(pFile, CArchive::load);

		CRuntimeClass* pClass = ar.ReadClass();

		if (pClass != NULL)
		{
			pItem = (CXTPTaskPanelItem*)pClass->CreateObject();

			if (pItem != NULL)
			{
				CXTPPropExchangeArchive px(ar);
				pItem->DoPropExchange(&px);
			}
		}

		ar.Close();
		return pItem;
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. OLE exception: %x\r\n"), pEx->m_sc);
		pEx->Delete();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. Archive exception\r\n"));
		pEx->Delete();
	}
	catch (CNotSupportedException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. \"Not Supported\" exception\r\n"));
		pEx->Delete();
	}

	if (pItem != NULL)
	{
		delete pItem;
	}

	return NULL;
}

void CXTPTaskPanelItem::CopyToClipboard()
{
	if (!m_pPanel->OpenClipboard())
		return;

	if (!::EmptyClipboard())
	{
		::CloseClipboard();
		return;
	}

	HGLOBAL hGlobal = CacheGlobalData();

	if (hGlobal)
	{
		SetClipboardData(CXTPTaskPanel::GetClipboardFormat(), hGlobal);
	}

	::CloseClipboard();
}

CXTPTaskPanelItem* CXTPTaskPanelItem::PasteFromClipboard()
{
	COleDataObject data;

	if (!data.AttachClipboard())
	{
		return 0;
	}

	if (!data.IsDataAvailable(CXTPTaskPanel::GetClipboardFormat()))
		return 0;

	return CXTPTaskPanelItem::CreateFromOleData(&data);
}

void CXTPTaskPanelItem::Remove()
{
	if (m_pItems)
		m_pItems->Remove(this);
}

int CXTPTaskPanelItem::GetIndex() const
{
	return this == NULL ? -1 : m_pItems->GetIndex(this);
}

BOOL CXTPTaskPanelItem::IsAllowDrop() const
{
	return m_bAllowDrop && ((CXTPTaskPanelItem*)GetItemGroup())->m_bAllowDrop && m_pPanel
		   && m_pPanel->m_bAllowDrop;
}
void CXTPTaskPanelItem::AllowDrop(BOOL bAllowDrop)
{
	m_bAllowDrop = bAllowDrop;
}

long CXTPTaskPanelItem::IsAllowDrag() const
{
	if (m_nAllowDrag != xtpTaskItemAllowDragDefault)
		return m_nAllowDrag;

	CXTPTaskPanelItem* pGroup = (CXTPTaskPanelItem*)GetItemGroup();
	if (pGroup && pGroup->m_nAllowDrag != xtpTaskItemAllowDragDefault)
		return pGroup->m_nAllowDrag;

	return !m_pPanel || (m_pPanel->m_nAllowDrag == xtpTaskItemAllowDragDefault)
			   ? xtpTaskItemAllowDrag
			   : m_pPanel->m_nAllowDrag;
}

void CXTPTaskPanelItem::AllowDrag(long nAllowDrag /* = xtpTaskItemAllowDragDefault */)
{
	m_nAllowDrag = nAllowDrag;
}

CFont* CXTPTaskPanelItem::GetItemFont() const
{
	return m_pPanel->GetPaintManager()->GetItemFont((CXTPTaskPanelItem*)this);
}

void CXTPTaskPanelItem::SetItemFont(LOGFONT* pLogFont)
{
	m_xtpFontItem.DeleteObject();
	if (pLogFont)
		m_xtpFontItem.CreateFontIndirect(pLogFont);
}

void CXTPTaskPanelItem::DrawCaptionText(CDC* pDC, CRect rc, UINT nFormat)
{
	if (m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(GetTaskPanel()->GetMarkupContext(),
								(HFONT)::GetCurrentObject(pDC->GetSafeHdc(), OBJ_FONT),
								pDC->GetTextColor());
		XTPMarkupRenderElement(m_pMarkupUIElement, pDC->GetSafeHdc(), rc);
	}
	else
	{
		pDC->DrawText(GetCaption(), rc, nFormat);
	}
}

CSize CXTPTaskPanelItem::GetCaptionTextSize(CDC* pDC, CRect rc, UINT nFormat)
{
	if (m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(GetTaskPanel()->GetMarkupContext(),
								(HFONT)::GetCurrentObject(pDC->GetSafeHdc(), OBJ_FONT),
								COLORREF_NULL);
		return XTPMarkupMeasureElement(m_pMarkupUIElement, rc.Width(), INT_MAX);
	}

	CRect rcText(rc.left, 0, rc.right, 0);
	CString strCaption = GetCaption();

	pDC->DrawText(strCaption.IsEmpty() ? CString(_T(" ")) : strCaption, rcText,
				  (nFormat | DT_CALCRECT));

	return strCaption.IsEmpty() ? CSize(0, rcText.Height()) : rcText.Size();
}

void CXTPTaskPanelItem::SetTextRect(CDC* pDC, CRect rc, UINT nFormat)
{
	CSize szText = GetCaptionTextSize(pDC, rc, nFormat & (~(DT_END_ELLIPSIS | DT_VCENTER)));

	int nDelta  = rc.Height() - szText.cy;
	int nOffset = nFormat & DT_VCENTER ? nDelta / 2 : 0;

	rc.DeflateRect(0, nOffset, 0, nDelta - nOffset);

	m_nTextFormat = nFormat;
	m_rcText	  = rc;
}

CRect CXTPTaskPanelItem::GetTextRect(UINT* pFormat) const
{
	if (pFormat)
		*pFormat = m_nTextFormat;

	return m_rcText;
}

CRect CXTPTaskPanelItem::GetHitTestRect() const
{
	return CRect(0, 0, 0, 0);
}

CRect CXTPTaskPanelItem::GetInvalidateRect() const
{
	return GetHitTestRect();
}

BOOL CXTPTaskPanelItem::IsGroup() const
{
	return GetType() == xtpTaskItemTypeGroup;
}

CXTPTaskPanelGroup* CXTPTaskPanelItem::GetItemGroup() const
{
	return (CXTPTaskPanelGroup*)this;
}

CCmdTarget* CXTPTaskPanelItem::GetAccessible()
{
	return this;
}

HRESULT CXTPTaskPanelItem::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	if (NULL == ppdispParent)
		return E_INVALIDARG;

	*ppdispParent = GetTaskPanel()->CreateAccProxy();
	return S_OK;
}

HRESULT CXTPTaskPanelItem::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	if (NULL == pszName)
		return E_INVALIDARG;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	CString strCaption = GetCaption();

	*pszName = strCaption.AllocSysString();
	return S_OK;
}

HRESULT CXTPTaskPanelItem::GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription)
{
	if (NULL == pszDescription)
		return E_INVALIDARG;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDescription = GetTooltip().AllocSysString();
	return S_OK;
}

HRESULT CXTPTaskPanelItem::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	if (NULL == pvarRole)
		return E_INVALIDARG;

	pvarRole->vt = VT_EMPTY;

	if (GetChildIndex(&varChild) == CHILDID_SELF)
	{
		pvarRole->vt   = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_LISTITEM;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPTaskPanelItem::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	if (NULL == pvarState)
		return E_INVALIDARG;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	pvarState->vt   = VT_I4;
	pvarState->lVal = STATE_SYSTEM_FOCUSABLE | (IsItemFocused() ? STATE_SYSTEM_FOCUSED : 0);

	if (!IsVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	if (!GetEnabled())
		pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;

	return S_OK;
}

HRESULT CXTPTaskPanelItem::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth,
											  long* pcyHeight, VARIANT varChild)
{
	if (NULL == pxLeft || NULL == pyTop || NULL == pcxWidth || NULL == pcyHeight)
		return E_INVALIDARG;

	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	if (!GetTaskPanel()->GetSafeHwnd())
		return S_OK;

	CRect rcItem = GetHitTestRect();
	GetTaskPanel()->ClientToScreen(&rcItem);

	*pxLeft	= rcItem.left;
	*pyTop	 = rcItem.top;
	*pcxWidth  = rcItem.Width();
	*pcyHeight = rcItem.Height();

	return S_OK;
}

HRESULT CXTPTaskPanelItem::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (NULL == pvarID)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!GetTaskPanel()->GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(GetTaskPanel()).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt   = VT_I4;
	pvarID->lVal = 0;

	CPoint pt(xLeft, yTop);
	GetTaskPanel()->ScreenToClient(&pt);

	if (!GetHitTestRect().PtInRect(pt))
		return S_FALSE;

	return S_OK;
}

HRESULT CXTPTaskPanelItem::AccessibleSelect(long flagsSelect, VARIANT varChild)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	if (flagsSelect == SELFLAG_TAKEFOCUS)
	{
		GetTaskPanel()->SetFocusedItem(this);
	}

	return S_OK;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTaskPanelItem, CXTPCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Caption", dispidCaption, OleGetCaption, SetCaption,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Tooltip", dispidTooltip, OleGetTooltip, SetTooltip,
						VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Enabled", dispidEnabled, GetEnabled, SetEnabled,
						VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Type", dispidType, GetType, SetType, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Id", dispidId, GetID, SetID_, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "IconIndex", dispidIconIndex, GetIconIndex, SetIconIndex,
						VT_I4)

	DISP_PROPERTY_ID(CXTPTaskPanelItem, "AllowDrag", 7, m_nAllowDrag, VT_I4)
	DISP_PROPERTY_ID(CXTPTaskPanelItem, "AllowDrop", 8, m_bAllowDrop, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Index", 9, OleGetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Group", 100, OleGetGroup, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "ParentItems", 101, OleGetParentItems, SetNotSupported,
						VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPTaskPanelItem, "IsGroup", 102, IsGroup, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskPanelItem, "Remove", 103, Remove, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_ID(CXTPTaskPanelItem, "Tag", 30, m_oleTag, VT_VARIANT)
	DISP_FUNCTION_ID(CXTPTaskPanelItem, "EnsureVisible", 105, EnsureVisible, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Visible", 31, IsVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTaskPanelItem, "SetIconPadding", 32, OleSetIconPadding, VT_EMPTY,
					 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPTaskPanelItem, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_FONT)

END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelItem, XTPDIID_ITaskPanelItem)

CXTPTaskPanelItem* AFX_CDECL CXTPTaskPanelItem::FromDispatch(LPDISPATCH pDisp)
{
	CXTPTaskPanelItem* pInst = NULL;
	if (NULL != pDisp)
	{
		pInst = XTP_DYNAMIC_DOWNCAST_REMOTE_(CXTPTaskPanelItem,
											 CXTPTaskPanelItem::FromIDispatchSafe(pDisp));
		if (NULL == pInst)
		{
			AfxThrowOleException(E_INVALIDARG);
		}
	}

	return pInst;
}

LPFONTDISP CXTPTaskPanelItem::OleGetFont()
{
	return AxCreateOleFont(GetItemFont(), this, (LPFNFONTCHANGED)&CXTPTaskPanelItem::OleSetFont);
}

void CXTPTaskPanelItem::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) || AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_xtpFontItem.DeleteObject();

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_xtpFontItem.CreateFontIndirect(&lf);
	}
}

BSTR CXTPTaskPanelItem::OleGetCaption()
{
	return m_strCaption.AllocSysString();
}
BSTR CXTPTaskPanelItem::OleGetTooltip()
{
	return m_strTooltip.AllocSysString();
}

LPDISPATCH CXTPTaskPanelItem::OleGetGroup()
{
	CXTPTaskPanelGroup* pGroup = GetItemGroup();
	return pGroup ? ((CXTPTaskPanelItem*)pGroup)->GetIDispatch(TRUE) : 0;
}
LPDISPATCH CXTPTaskPanelItem::OleGetParentItems()
{
	return GetParentItems()->GetIDispatch(TRUE);
}

int CXTPTaskPanelItem::OleGetIndex()
{
	return GetIndex() + 1;
}

void CXTPTaskPanelItem::OleSetIconPadding(long nLeft, long nTop, long nRight, long nBottom)
{
	m_rcIconPadding.SetRect(nLeft, nTop, nRight, nBottom);

	GetTaskPanel()->Reposition();
}

void CXTPTaskPanelItem::SetID_(UINT nID)
{
	SetID(nID);
}

#endif

BEGIN_INTERFACE_MAP(CXTPTaskPanelItem, CXTPCmdTarget)
	INTERFACE_PART(CXTPTaskPanelItem, IID_IAccessible, ExternalAccessible)
#ifdef _XTP_ACTIVEX
	INTERFACE_PART(CXTPTaskPanelItem, XTPDIID_ITaskPanelItem, Dispatch)
#endif
END_INTERFACE_MAP()
