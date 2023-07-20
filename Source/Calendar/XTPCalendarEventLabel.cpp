// XTPCalendarEventLabel.cpp: implementation of the CXTPCalendarEventLabel and
// CXTPCalendarEventLabels classes.
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

#include "Calendar/Resource.h"
#include "Common/Resource.h"

#include "Common/XTPTypeId.h"
#include "Common/XTPCasting.h"
#include "Common/XTPFramework.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSynchro.h"
#include "Common/XTPApplication.h"
#include "Common/XTPSingleton.h"
#include "Common/XTPGdiObjects.h"
#include "Common/XTPColorManager.h"
#include "Common/PropExchange/XTPPropExchangeSection.h"
#include "Common/XTPPropExchange.h"
#include "Common/PropExchange/XTPPropExchangeEnumerator.h"
#include "Common/PropExchange/XTPPropExchangeEnumeratorPtr.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "Calendar/XTPCalendarPtrCollectionT.h"
#include "Calendar/XTPCalendarEventLabel.h"
#include "Calendar/XTPCalendarDefines.h"
#include "Calendar/XTPCalendarIIDs.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarEventLabel, CXTPCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarEventLabels, CXTPCmdTarget)

IMPLEMENT_DYNCREATE(CXTPCalendarEventCategory, CXTPCmdTarget)
IMPLEMENT_DYNCREATE(CXTPCalendarEventCategories, CXTPCmdTarget)

IMPLEMENT_DYNAMIC(CXTPCalendarUIntArray, CXTPCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarIconIDs, CXTPCalendarUIntArray)
IMPLEMENT_DYNAMIC(CXTPCalendarEventCategoryIDs, CXTPCalendarUIntArray)

////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventLabel

CXTPCalendarEventLabel::CXTPCalendarEventLabel(int nID, COLORREF clrColor, LPCTSTR pcszName)
{
	m_nLabelID = nID;
	m_clrColor = clrColor;
	m_strName  = pcszName;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventLabel::CXTPCalendarEventLabel(CXTPCalendarEventLabel* pSrc)
{
	if (pSrc)
	{
		m_nLabelID = pSrc->m_nLabelID;
		m_clrColor = pSrc->m_clrColor;
		m_strName  = pSrc->m_strName;
	}
	else
	{
		m_nLabelID = 0;
		m_clrColor = 0;
		m_strName  = _T("");
	}

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventLabel::~CXTPCalendarEventLabel()
{
}

const CXTPCalendarEventLabel& CXTPCalendarEventLabel::operator=(const CXTPCalendarEventLabel& rSrc)
{
	m_nLabelID = rSrc.m_nLabelID;
	m_clrColor = rSrc.m_clrColor;
	m_strName  = rSrc.m_strName;
	return *this;
}

////////////////////////////////////////////////////////////////////////////
// CXTPCalendarEventLabels

CXTPCalendarEventLabels::CXTPCalendarEventLabels()
	: m_bDefaultValues(TRUE)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventLabels::~CXTPCalendarEventLabels()
{
	RemoveAll();
}

CXTPCalendarEventLabel* CXTPCalendarEventLabels::Find(int nLabelID) const
{
	int nFIdx = FindIndex(nLabelID);
	if (nFIdx >= 0)
	{
		return GetAt(nFIdx);
	}
	return NULL;
}

int CXTPCalendarEventLabels::FindIndex(int nLabelID) const
{
	int nCount = GetCount();

	if (nLabelID >= 0 && nLabelID < nCount)
	{
		CXTPCalendarEventLabel* pLabel = GetAt(nLabelID);

		if (pLabel && pLabel->m_nLabelID == nLabelID)
		{
			return nLabelID;
		}
	}

	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEventLabel* pLabel = GetAt(i);

		if (pLabel && pLabel->m_nLabelID == nLabelID)
		{
			return i;
		}
	}

	return -1;
}

void CXTPCalendarEventLabels::InitDefaultValues()
{
	m_bDefaultValues = TRUE;

	RemoveAll();

	// XTP_CALENDAR_NONE_LABEL_ID = 0
	CXTPCalendarEventLabel* pLabel;
	pLabel = new CXTPCalendarEventLabel(0, RGB(255, 255, 255),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_NONE));
	InsertAt(0, pLabel);

	pLabel = new CXTPCalendarEventLabel(1, RGB(255, 148, 132),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_IMPORTANT));
	InsertAt(1, pLabel);

	pLabel = new CXTPCalendarEventLabel(2, RGB(132, 156, 231),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_BUSINESS));
	InsertAt(2, pLabel);

	pLabel = new CXTPCalendarEventLabel(3, RGB(165, 222, 99),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_PERSONAL));
	InsertAt(3, pLabel);

	pLabel = new CXTPCalendarEventLabel(4, RGB(231, 231, 214),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_VACATION));
	InsertAt(4, pLabel);

	pLabel = new CXTPCalendarEventLabel(5, RGB(255, 181, 115),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_MUSTATTEND));
	InsertAt(5, pLabel);

	pLabel = new CXTPCalendarEventLabel(6, RGB(132, 239, 247),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_TRAVELREQUIRED));
	InsertAt(6, pLabel);

	pLabel = new CXTPCalendarEventLabel(7, RGB(214, 206, 132),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_NEEDSPREPARATION));
	InsertAt(7, pLabel);

	pLabel = new CXTPCalendarEventLabel(8, RGB(198, 165, 247),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_BIRTHDAY));
	InsertAt(8, pLabel);

	pLabel = new CXTPCalendarEventLabel(9, RGB(165, 206, 198),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_ANNIVERSARY));
	InsertAt(9, pLabel);

	pLabel = new CXTPCalendarEventLabel(10, RGB(255, 231, 115),
										XTPResourceManager()->LoadString(
											XTP_IDS_CALENDAR_EVENT_LABEL_PHONECALL));
	InsertAt(10, pLabel);
}

void CXTPCalendarEventLabels::SetDefaultValues(BOOL bDefault)
{
	m_bDefaultValues = bDefault;
}

void CXTPCalendarEventLabels::UpdateDefaultValues(COLORREF* clrEventColors, UINT uCount)
{
	if (m_bDefaultValues)
	{
		// If you get this ASSERT and have made changes to the label list,
		// make sure that you have called SetDefaultValues(FALSE) to ignore
		// UpdateDefaultValues() whenever the theme changes.
		if (uCount != (UINT)GetCount())
		{
			ASSERT(0);
			return;
		}

		for (int i = 0; i < (int)uCount; i++)
		{
			CXTPCalendarEventLabel* pLabel = GetAt(i);
			if (pLabel)
			{
				pLabel->m_clrColor = clrEventColors[i];
			}
		}
	}
}

//===========================================================================
// CXTPCalendarEventCategory

CXTPCalendarEventCategory::CXTPCalendarEventCategory(UINT nID, LPCTSTR pcszName, COLORREF clrBorder,
													 COLORREF clrBkBase)
{
	m_nID		= nID;
	m_strName   = pcszName;
	m_clrBorder = clrBorder;
	m_clrBkBase = clrBkBase;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventCategory::CXTPCalendarEventCategory(CXTPCalendarEventCategory* pCategory)
{
	ASSERT(pCategory);

	if (pCategory)
	{
		m_nID		= pCategory->GetID();
		m_strName   = pCategory->GetName();
		m_clrBorder = pCategory->GetBorderColor();
		m_clrBkBase = pCategory->GetBkBaseColor();
	}
	else
	{
		m_nID		= 0;
		m_strName   = _T("");
		m_clrBorder = 0;
		m_clrBkBase = 0;
	}

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventCategory::~CXTPCalendarEventCategory()
{
}

CXTPPaintManagerColorGradient& CXTPCalendarEventCategory::GetBackgroundColor()
{
	if (m_grclrBackground.IsNull())
	{
		m_grclrBackground.clrLight.SetStandardValue(
			XTPDrawHelpers()->LightenColor(156, m_clrBkBase)); // 156 - 242, 178, 179;
		m_grclrBackground.clrDark.SetStandardValue(
			XTPDrawHelpers()->LightenColor(56, m_clrBkBase)); // 56  - 229,100, 107
	}

	return m_grclrBackground;
}

COLORREF CXTPCalendarEventCategory::GetBkColorAllDayEvents() const
{
	return m_clrBkBase;
}

COLORREF CXTPCalendarEventCategory::GetBkColorWorkCell() const
{
	return XTPDrawHelpers()->LightenColor(185, m_clrBkBase); // 185 - 245, 201, 203
}

COLORREF CXTPCalendarEventCategory::GetBkColorNonWorkCell() const
{
	return XTPDrawHelpers()->LightenColor(115, m_clrBkBase); // 115 - 237, 146, 150
}

COLORREF CXTPCalendarEventCategory::GetColorCellBorder() const
{
	return XTPDrawHelpers()->LightenColor(45, m_clrBkBase); // 45 - 227, 91, 98
}

CXTPCalendarEventCategories::CXTPCalendarEventCategories()
	: m_bDefaultValues(TRUE)
{
	InitDefaultValues();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventCategories::~CXTPCalendarEventCategories()
{
}

CXTPCalendarEventCategory* CXTPCalendarEventCategories::Find(UINT nID) const
{
	int nFIdx = FindIndex(nID);
	if (nFIdx >= 0)
	{
		return GetAt(nFIdx);
	}
	return NULL;
}

int CXTPCalendarEventCategories::FindIndex(UINT nID) const
{
	int nCount = GetCount();

	//---------------------------------------------------------
	int nDirectIdx = (int)(nID - 1);

	if (nDirectIdx >= 0 && nDirectIdx < nCount)
	{
		CXTPCalendarEventCategory* pEvCat = GetAt(nDirectIdx);

		if (pEvCat && pEvCat->GetID() == nID)
		{
			return nDirectIdx;
		}
	}

	//---------------------------------------------------------
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEventCategory* pEvCat = GetAt(i);

		if (pEvCat && pEvCat->GetID() == nID)
		{
			return i;
		}
	}

	return -1;
}

CString CXTPCalendarEventCategories::LoadCategoryName(UINT nIDResource) const
{
	CString strFormat = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_CATEGORY);
	CString strColor  = XTPResourceManager()->LoadString(nIDResource);

	CString strCategory;
	strCategory.Format(strFormat, (LPCTSTR)strColor);

	return strCategory;
}

void CXTPCalendarEventCategories::InitDefaultValues()
{
	m_bDefaultValues = TRUE;

	RemoveAll();

	CXTPCalendarEventCategory* pEvCat;

	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryYellow,
										   LoadCategoryName(XTP_IDS_CLR_YELLOW), RGB(154, 149, 25),
										   RGB(255, 251,
											   155)); // RGB(255, 252, 179), RGB(255,249, 91));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryGreen,
										   LoadCategoryName(XTP_IDS_CLR_GREEN), RGB(55, 126, 45),
										   RGB(96, 194,
											   82)); // RGB(196, 232, 190), RGB(136, 210, 127));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryRed,
										   LoadCategoryName(XTP_IDS_CLR_RED), RGB(167, 29, 35),
										   RGB(220, 56,
											   65)); // RGB(242, 178, 179), RGB(229,100, 107));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryPurple,
										   LoadCategoryName(XTP_IDS_CLR_PURPLE), RGB(77, 49, 141),
										   RGB(184, 164,
											   224)); // RGB(202, 187, 232), RGB(150, 125, 210));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryOrange,
										   LoadCategoryName(XTP_IDS_CLR_ORANGE), RGB(173, 77, 13),
										   RGB(247, 187,
											   137)); // RGB(249, 204, 167), RGB(244, 152, 90));
	Add(pEvCat);
	//---------------------------------------
	pEvCat = new CXTPCalendarEventCategory(xtpCalendarEventCategoryBlue,
										   LoadCategoryName(XTP_IDS_CLR_BLUE), RGB(40, 81, 142),
										   RGB(159, 183,
											   232)); // RGB(183, 201, 238), RGB(119, 158, 218));
	Add(pEvCat);
}

void CXTPCalendarEventCategories::SetDefaultValues(BOOL bDefault)
{
	m_bDefaultValues = bDefault;
}

void CXTPCalendarEventCategories::UpdateDefaultValues(COLORREF* pClrBkBase, COLORREF* pClrBorder,
													  int nCount)
{
	if (m_bDefaultValues)
	{
		if (nCount != GetCount())
		{
			// If you get this ASSERT and have made changes to the category list,
			// make sure that you have called SetDefaultValues(FALSE) to ignore
			// UpdateDefaultValues() whenever the theme changes.
			ASSERT(0);
			return;
		}

		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarEventCategory* pCategory = GetAt(i);
			if (pCategory)
			{
				pCategory->SetBkBaseColor(pClrBkBase[i]);
				pCategory->SetBorderColor(pClrBorder[i]);
			}
		}
	}
}

//===========================================================================
int CXTPCalendarUIntArray::Find(UINT uID) const
{
	return TBase::FindElement(uID);
}

BOOL CXTPCalendarUIntArray::RemoveID(UINT uID)
{
	return TBase::RemoveElement(uID);
}

CString CXTPCalendarUIntArray::SaveToString() const
{
	CString strData;

	TCHAR szNumber[34];

	int nCount = GetSize();
	for (int i = 0; i < nCount; i++)
	{
		UINT uElement = GetAt(i);

		ITOT_S((int)uElement, szNumber, _countof(szNumber));
		szNumber[33] = 0;

		if (!strData.IsEmpty())
		{
			strData += _T(",");
		}

		strData += szNumber;
	}

	return strData;
}

void CXTPCalendarUIntArray::LoadFromString(LPCTSTR pcszData)
{
	RemoveAll();

	CString strData = pcszData;

	while (!strData.IsEmpty())
	{
		CString strNumber = strData.SpanExcluding(_T(","));

		UINT uElement = (UINT)_ttoi(strNumber);
		Add(uElement);

		int nDelCount = strNumber.GetLength() + 1;
		nDelCount	 = min(nDelCount, strData.GetLength());
		strData.Delete(0, nDelCount);
	}
}

void CXTPCalendarUIntArray::DoPropExchange(CXTPPropExchange* pPX, LPCTSTR pcszSection,
										   LPCTSTR pcszElement)
{
	if (!pPX || !pcszSection || !pcszElement)
	{
		ASSERT(FALSE);
		return;
	}

	long nVersion = 1;

	if (pPX->IsStoring())
	{
		CXTPPropExchangeSection secArray(pPX->GetSection(pcszSection));
		secArray->EmptySection();

		PX_Long(&secArray, _T("Version"), nVersion, 1);

		int nCount = GetSize();
		CXTPPropExchangeEnumeratorPtr pEnumerator(secArray->GetEnumerator(pcszElement));
		POSITION posStorage = pEnumerator->GetPosition(XTPToUIntChecked(nCount));

		for (int i = 0; i < nCount; i++)
		{
			ULONG ulElement = (ULONG)GetAt(i);

			CXTPPropExchangeSection secElement(pEnumerator->GetNext(posStorage));
			PX_ULong(&secElement, _T("Value"), ulElement);
		}
	}
	else if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeSection secArray(pPX->GetSection(pcszSection));

		PX_Long(&secArray, _T("Version"), nVersion, 1);

		if (nVersion != 1)
		{
			TRACE(_T("ERROR! CXTPCalendarIconIDs::DoPropExchange: Unsupported data version: %d \n"),
				  nVersion);
			return;
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(secArray->GetEnumerator(pcszElement));
		POSITION posStorage = pEnumerator->GetPosition();

		while (posStorage)
		{
			ULONG ulElement = 0;

			CXTPPropExchangeSection secElement(pEnumerator->GetNext(posStorage));
			PX_ULong(&secElement, _T("Value"), ulElement, 0);

			Add((ULONG)ulElement);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

//===========================================================================
CXTPCalendarIconIDs::CXTPCalendarIconIDs()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarIconIDs::~CXTPCalendarIconIDs()
{
}

//===========================================================================
CXTPCalendarEventCategoryIDs::CXTPCalendarEventCategoryIDs()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarEventCategoryIDs::~CXTPCalendarEventCategoryIDs()
{
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPCalendarEventLabel, CXTPCmdTarget)
	DISP_PROPERTY_ID(CXTPCalendarEventLabel, "LabelID", DISPID_VALUE, m_nLabelID, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPCalendarEventLabel, "Color", 1, OleGetColor, OleSetColor, VT_COLOR)
	DISP_PROPERTY_ID(CXTPCalendarEventLabel, "Name", 2, m_strName, VT_BSTR)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarEventLabel, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEventLabel, XTPDIID_CalendarEventLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEventLabel, XTPDIID_CalendarEventLabel)

////////////////////////////////////////////////////////////////////////////
BEGIN_DISPATCH_MAP(CXTPCalendarEventLabels, CXTPCmdTarget)

	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_DISPATCH,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "Find", 2, OleFind, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "AddLabel", 3, OleAddLabel, VT_EMPTY,
					 VTS_I4 VTS_COLOR VTS_BSTR)
	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "RemoveAll", 4, RemoveAll, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarEventLabels, "InitDefaultValues", 5, InitDefaultValues, VT_EMPTY,
					 VTS_NONE)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CXTPCalendarEventLabels, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEventLabels, XTPDIID_CalendarEventLabels, Dispatch)
	// INTERFACE_PART(CXTPCalendarEventLabels, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEventLabels, XTPDIID_CalendarEventLabels)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarEventLabels);

////////////////////////////////////////////////////////////////////////////
OLE_COLOR CXTPCalendarEventLabel::OleGetColor()
{
	return (OLE_COLOR)m_clrColor;
}

void CXTPCalendarEventLabel::OleSetColor(OLE_COLOR clrOleColor)
{
	m_clrColor = AxTranslateColor(clrOleColor);
}

void CXTPCalendarEventLabels::OleAddLabel(long nLabelID, OLE_COLOR clrOleColor, LPCTSTR pcszName)
{
	COLORREF clrColor = AxTranslateColor(clrOleColor);

	CXTPCalendarEventLabel* pLabel = new CXTPCalendarEventLabel(nLabelID, clrColor, pcszName);

	if (!pLabel)
	{
		AfxThrowOleException(E_OUTOFMEMORY);
	}

	Add(pLabel, FALSE);
}

LPDISPATCH CXTPCalendarEventLabels::OleFind(long nLabelID)
{
	int nFIdx = FindIndex(nLabelID);
	if (nFIdx >= 0)
	{
		return OleGetItem(nFIdx);
	}
	return NULL;
}

//===========================================================================

BEGIN_DISPATCH_MAP(CXTPCalendarUIntArray, CXTPCmdTarget)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_PROPERTY_PARAM_ID(CXTPCalendarUIntArray, "Item", DISPID_VALUE, OleGetItem, OleSetItem,
						   VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_DISPATCH,
					 VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "Add", 2, OleAdd, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "AddIfNeed", 3, AddIfNeed, VT_EMPTY, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "RemoveID", 4, RemoveID, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "Remove", 5, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "RemoveAll", 6, OleRemoveAll, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "Find", 7, Find, VT_I4, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "LoadFromString", 8, OleLoadFromString, VT_EMPTY,
					 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPCalendarUIntArray, "SaveToString", 9, OleSaveToString, VT_BSTR, VTS_NONE)

END_DISPATCH_MAP()

IMPLEMENT_ENUM_VARIANTLIST(CXTPCalendarUIntArray);

BOOL CXTPCalendarUIntArray::OleGetNextItem(/* [in, out] */ POSITION& rPos,
										   /* [out] */ VARIANT* pVar)
{
	INT_PTR nPos = (INT_PTR)rPos;
	ASSERT(pVar);
	if (!pVar || nPos >= OleGetItemCount())
	{
		return FALSE;
	}

	pVar->vt   = VT_I4;
	pVar->lVal = OleGetItem((long)nPos);

	rPos = (POSITION)(nPos + 1);

	return TRUE;
}

POSITION CXTPCalendarUIntArray::OleGetFirstItemPosition()
{
	return 0;
}

void CXTPCalendarUIntArray::OleLoadFromString(LPCTSTR strData)
{
	LoadFromString(strData);
}

BSTR CXTPCalendarUIntArray::OleSaveToString()
{
	CString strData = SaveToString();

	return strData.AllocSysString();
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CXTPCalendarIconIDs, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarIconIDs, XTPDIID_CalendarIconIDs, Dispatch)
	// INTERFACE_PART(CXTPCalendarIconIDs, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarIconIDs, XTPDIID_CalendarIconIDs)

BEGIN_DISPATCH_MAP(CXTPCalendarIconIDs, CXTPCalendarUIntArray)
	// common members map implemented in the base class
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEventCategory, XTPDIID_ICalendarEventCategory)

BEGIN_DISPATCH_MAP(CXTPCalendarEventCategory, CXTPCmdTarget)

	DISP_PROPERTY_ID(CXTPCalendarEventCategory, "Name", DISPID_VALUE, m_strName, VT_BSTR)
	DISP_PROPERTY_ID(CXTPCalendarEventCategory, "Id", 1, m_nID, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPCalendarEventCategory, "BorderColor", 2, OleGetBorderColor,
						OleSetBorderColor, VT_COLOR)

	DISP_FUNCTION_ID(CXTPCalendarEventCategory, "Background", 10, OleGetBackground, VT_DISPATCH,
					 VTS_NONE)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CXTPCalendarEventCategory, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEventCategory, XTPDIID_ICalendarEventCategory, Dispatch)
END_INTERFACE_MAP()

OLE_COLOR CXTPCalendarEventCategory::OleGetBorderColor()
{
	return m_clrBorder;
}

void CXTPCalendarEventCategory::OleSetBorderColor(OLE_COLOR oleColor)
{
	m_clrBorder = AxTranslateColor(oleColor);
}

////////////////////////////////////////////////////////////////////////////

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEventCategories, XTPDIID_ICalendarEventCategories)
IMPLEMENT_ENUM_VARIANT(CXTPCalendarEventCategories);

BEGIN_DISPATCH_MAP(CXTPCalendarEventCategories, CXTPCmdTarget)

	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_PROPERTY_PARAM_ID(CXTPCalendarEventCategories, "Item", DISPID_VALUE, OleGetItem,
						   OleSetItem, VT_DISPATCH, VTS_I4)

	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "Remove", 3, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "RemoveAll", 4, RemoveAll, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "Find", 5, OleFind, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "InitDefaultValues", 6, InitDefaultValues,
					 VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarEventCategories, "_NewEnum", DISPID_NEWENUM, OleNewEnum,
					 VT_DISPATCH, VTS_NONE)
END_DISPATCH_MAP()

////////////////////////////////////////////////////////////////////////////
BEGIN_INTERFACE_MAP(CXTPCalendarEventCategories, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEventCategories, XTPDIID_ICalendarEventCategories, Dispatch)
	// INTERFACE_PART(CXTPCalendarEventCategories, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

void CXTPCalendarEventCategories::OleSetItem(long nIndex, LPDISPATCH pdispItem)
{
	if (nIndex < 0 || nIndex >= GetCount())
		AfxThrowOleException(DISP_E_BADINDEX);

	CXTPCalendarEventCategory* pCat = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPCalendarEventCategory, CXTPCalendarEventCategory::FromIDispatchSafe(pdispItem));
	if (!pCat)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	pCat->InternalAddRef();
	SetAt(nIndex, pCat);
}

void CXTPCalendarEventCategories::OleAdd(LPDISPATCH pdispItem)
{
	CXTPCalendarEventCategory* pCat = XTP_DYNAMIC_DOWNCAST_REMOTE_(
		CXTPCalendarEventCategory, CXTPCalendarEventCategory::FromIDispatchSafe(pdispItem));
	if (!pCat)
	{
		AfxThrowOleException(E_INVALIDARG);
	}

	Add(pCat, TRUE);
}

LPDISPATCH CXTPCalendarEventCategories::OleFind(long nID)
{
	int nFIdx = FindIndex(nID);
	if (nFIdx >= 0)
	{
		return OleGetItem(nFIdx);
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_INTERFACE_MAP(CXTPCalendarEventCategoryIDs, CXTPCmdTarget)
	INTERFACE_PART(CXTPCalendarEventCategoryIDs, XTPDIID_CalendarEventCategoryIDs, Dispatch)
	// INTERFACE_PART(CXTPCalendarEventCategoryIDs, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarEventCategoryIDs, XTPDIID_CalendarEventCategoryIDs)

BEGIN_DISPATCH_MAP(CXTPCalendarEventCategoryIDs, CXTPCalendarUIntArray)
	// common members map implemented in the base class
END_DISPATCH_MAP()

#endif
