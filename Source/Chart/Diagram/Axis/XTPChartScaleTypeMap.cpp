// XTPChartScaleTypeMap.cpp
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

#include "Common/Math/XTPMathUtils.h"
#include "Common/XTPFramework.h"
#include "Common/XTPResourceManager.h"
#include "Common/Base/Types/XTPPoint2.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "Common/Base/Types/XTPSize.h"
#include "Common/Base/Types/XTPRect.h"

#include "Chart/Types/XTPChartTypes.h"
#include "Chart/XTPChartDefines.h"
#include "Chart/XTPChartElement.h"
#include "Chart/XTPChartLegendItem.h"
#include "Chart/XTPChartElementView.h"
#include "Chart/XTPChartSeries.h"
#include "Chart/XTPChartSeriesView.h"
#include "Chart/XTPChartSeriesPoint.h"

#include "Chart/Utils/XTPChartNumberFormat.h"
#include "Chart/Diagram/Axis/XTPChartScaleTypeMap.h"
#include "Chart/Diagram/Axis/XTPChartAxis.h"
#include "Chart/Diagram/Axis/XTPChartAxisView.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPChartScaleTypeMap::CXTPChartScaleTypeMap(CXTPChartAxis* pAxis)
{
	m_pOwner = pAxis;
}

CXTPChartScaleTypeMap::~CXTPChartScaleTypeMap()
{
}

CXTPChartScaleTypeMap* CXTPChartScaleTypeMap::Create(CXTPChartAxis* pAxis)
{
	CXTPChartScaleTypeMap* pType = NULL;

	switch (pAxis->GetScaleType())
	{
		case xtpChartScaleNumerical: pType = new CXTPChartNumericalScaleTypeMap(pAxis); break;

		case xtpChartScaleQualitative: pType = new CXTPChartQualitativeScaleTypeMap(pAxis); break;

		case xtpChartScaleDateTime: pType = new CXTPChartDateTimeScaleTypeMap(pAxis); break;

		case xtpChartScaleTime: pType = new CXTPChartTimeScaleTypeMap(pAxis); break;
	}

	return pType;
}

void CXTPChartScaleTypeMap::UpdateSeries(CXTPChartAxisView* pAxisView)
{
	CXTPChartAxis* pAxis = GetAxis();

	CArray<CXTPChartSeriesView*, CXTPChartSeriesView*>& arrSeries = pAxisView->m_arrSeries;

	int i;

	for (i = 0; i < arrSeries.GetSize(); i++)
	{
		UpdateSeries(arrSeries.GetAt(i)->GetSeries());
	}

	if (!pAxis->IsValuesAxis())
	{
		for (i = 0; i < arrSeries.GetSize(); i++)
		{
			CXTPChartSeries* pSeries = arrSeries.GetAt(i)->GetSeries();

			for (int j = 0; j < pSeries->GetPoints()->GetCount(); j++)
			{
				UpdateSeriesPointArgument(pSeries->GetPoints()->GetAt(j));
			}
		}
	}
}

void CXTPChartScaleTypeMap::UpdateSeries(CXTPChartSeries* /*pSeries*/)
{
}

void CXTPChartScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* /*pPoint*/)
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartNumericalMap

CXTPChartNumericalScaleTypeMap ::CXTPChartNumericalScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
	m_dMinValue = 0;
	m_dMaxValue = 0;
}

CXTPChartString CXTPChartNumericalScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat,
																double dMark) const
{
	return pFormat->FormatNumber(dMark);
}

double CXTPChartNumericalScaleTypeMap::ValueToInternal(const CXTPChartString& strValue) const
{
	double dValue = atof(XTP_CT2CA(strValue));
	return dValue;
}

void CXTPChartNumericalScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	if (NULL != pPoint)
	{
		pPoint->SetInternalArgumentValue(pPoint->GetArgumentValue());
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartDateTimeScaleTypeMap

CXTPChartDateTimeScaleTypeMap ::CXTPChartDateTimeScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
}

double CXTPChartDateTimeScaleTypeMap::ValueToInternal(const CXTPChartString& strValue) const
{
	COleDateTime dt;

	if (dt.ParseDateTime(strValue))
	{
		return DateToInternal((DATE)dt);
	}

	return 0;
}

double CXTPChartDateTimeScaleTypeMap::DateToInternal(DATE dValue) const
{
	switch (((CXTPChartAxis*)m_pOwner)->GetDateTimeScaleUnit())
	{
		case xtpChartScaleHour: return dValue * 24;

		case xtpChartScaleMinute: return dValue * 24 * 60;

		case xtpChartScaleMonth:
		{
			COleDateTime dt(dValue);

			COleDateTime dtBase(dt.GetYear(), dt.GetMonth(), 1, 0, 0, 0);

			dValue = (DATE)dt.GetYear() * 12 + (dt.GetMonth() - 1) + ((DATE)dt - (DATE)dtBase) / 31;
		}
			return dValue;

		case xtpChartScaleYear:
		{
			COleDateTime dt(dValue);

			COleDateTime dtBase(dt.GetYear(), 1, 1, 0, 0, 0);

			dValue = (DATE)dt.GetYear() + ((DATE)dt - (DATE)dtBase) / 366.0;
		}
		break;
	}

	return dValue;
}

DATE CXTPChartDateTimeScaleTypeMap::DoubleToDate(double dValue) const
{
	switch (((CXTPChartAxis*)m_pOwner)->GetDateTimeScaleUnit())
	{
		case xtpChartScaleHour: return dValue / 24;

		case xtpChartScaleMinute: return dValue / 24 / 60;

		case xtpChartScaleMonth:
		{
			int nYearMonth = (int)dValue;
			int nYear	  = nYearMonth / 12;
			int nMonth	 = nYearMonth - nYear * 12 + 1;

			COleDateTime dtBase(nYear, nMonth, 1, 0, 0, 0);

			dValue = (DATE)dtBase + (dValue - nYearMonth) * 31;
		}
			return dValue;

		case xtpChartScaleYear:
		{
			int nYear = (int)dValue;
			COleDateTime dtBase(nYear, 1, 1, 0, 0, 0);

			dValue = (DATE)dtBase + (dValue - nYear) * 366.0;
		}
		break;
	}

	return dValue;
}

void CXTPChartDateTimeScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	COleDateTime dt;

	DATE dValue = 0;

	if (!pPoint->GetArgument().IsEmpty() && dt.ParseDateTime(pPoint->GetArgument()))
	{
		dValue = (DATE)dt;
	}
	else
	{
		dValue = (DATE)pPoint->GetArgumentValue();
	}

	pPoint->SetInternalArgumentValue(DateToInternal(dValue));
}

CXTPChartString CXTPChartDateTimeScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat,
															   double dMark) const
{
	COleDateTime dt(DoubleToDate(dMark));

	return pFormat->FormatDate(dt);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartTimeScaleTypeMap

CXTPChartTimeScaleTypeMap ::CXTPChartTimeScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
}

double CXTPChartTimeScaleTypeMap::ValueToInternal(const CXTPChartString& strValue) const
{
	COleDateTime dt;

	if (dt.ParseDateTime(strValue))
	{
		return DateToInternal((DATE)dt);
	}

	return 0;
}

double CXTPChartTimeScaleTypeMap::DateToInternal(DATE dValue) const
{
	return dValue;
}

DATE CXTPChartTimeScaleTypeMap::DoubleToDate(double dValue) const
{
	return dValue;
}

void CXTPChartTimeScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	COleDateTime dt;

	DATE dValue = 0;

	if (!pPoint->GetArgument().IsEmpty() && dt.ParseDateTime(pPoint->GetArgument()))
	{
		dValue = (DATE)dt;
	}
	else
	{
		dValue = (DATE)pPoint->GetArgumentValue();
	}

	pPoint->SetInternalArgumentValue(DateToInternal(dValue));
}

CXTPChartString CXTPChartTimeScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat,
														   double dMark) const
{
	UNREFERENCED_PARAMETER(pFormat);

	CXTPChartTime time(dMark);

	CXTPChartString sTime;
	sTime.Format(_T("%02d:%02d:%02d"), time.GetHours() % 24, time.GetMinutes(), time.GetSeconds());
	return sTime;
}

CXTPChartTime::CXTPChartTime(double dTime)
	: m_dTime(dTime)
{
}

CXTPChartTime::CXTPChartTime(int nHours, int nMinutes, int nSeconds, int nMilliseconds)
	: m_dTime(nHours * Hours + nMinutes * Minutes + nSeconds * Seconds + nMilliseconds)
{
}

int CXTPChartTime::GetHours() const
{
	int nHours = static_cast<int>(m_dTime) / Hours;

	return nHours;
}

int CXTPChartTime::GetMinutes() const
{
	int nMinutes = (static_cast<int>(m_dTime) - (GetHours() * Hours)) / Minutes;

	return nMinutes;
}

int CXTPChartTime::GetSeconds() const
{
	int nSeconds = (static_cast<int>(m_dTime) - (GetHours() * Hours) - (GetMinutes() * Minutes))
				   / Seconds;

	return nSeconds;
}

int CXTPChartTime::GetMilliseconds() const
{
	int nMilliseconds = (static_cast<int>(m_dTime) - (GetHours() * Hours) - (GetMinutes() * Minutes)
						 - (GetSeconds() * Seconds));

	return nMilliseconds;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartQualitativeScaleTypeMap

template<>
AFX_INLINE UINT AFXAPI HashKey(const CXTPChartString& key)
{
	// default identity hash - works for most primitive values
	return HashKey((LPCTSTR)key);
}

//===========================================================================
// Summary:
//     This class abstracts a storage mechamism for strings which are used in
//     charts with qualitative scale types.
// Remarks:
//===========================================================================
class CXTPChartQualitativeScaleTypeMap::CStorage
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the zero based index of an arbitrary string.
	// Parameter:
	//     strValue - CXTPChartString denoting the value.
	// Returns:
	//     An integer denoting the index of the string.
	// Remarks:
	//-----------------------------------------------------------------------
	int IndexOf(const CXTPChartString& strValue) const
	{
		int nValue = m_arrList.IndexOf(strValue);
		return nValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to find an arbitrary string in the storage.
	// Parameter:
	//     strValue - A reference to the CXTPChartString denoting the value.
	// Returns:
	//     TRUE if it could find the string and FALSE if not.
	// Remarks:
	//-----------------------------------------------------------------------
	BOOL Find(const CXTPChartString& strValue) const
	{
		return IndexOf(strValue) != -1;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a collection of strings to the storage.
	// Parameter:
	//     strArray - A reference to the CXTPChartStrings denoting the strings
	//     collection.
	// Remarks:
	//-----------------------------------------------------------------------
	void AddRange(const CXTPChartStrings& strArray)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
			m_arrList.Add(strArray[i]);
	}
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a string to the storage.
	// Parameter:
	//     str - A reference to the CXTPChartString denoting the string to be
	//           added to the storage.
	// Remarks:
	//-----------------------------------------------------------------------
	void Add(const CXTPChartString& str)
	{
		m_arrList.Add(str);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to insert a collection of strings to the storage,
	//     after a particular index.
	// Parameters:
	//     index    - The index after which the strings to be inserted.
	//     strArray - A reference to the CXTPChartStrings denoting the strings
	//     collection.
	// Remarks:
	//-----------------------------------------------------------------------
	void InsertRange(int index, const CXTPChartStrings& strArray)
	{
		for (int i = 0; i < strArray.GetSize(); i++)
			m_arrList.InsertAt(index + i, strArray[i]);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to insert a string to the storage,after a
	//     particular index.
	// Parameters:
	//     index    - The index after which the strings to be inserted.
	//     str      - A reference to the CXTPChartString denoting the string
	//     to be added.
	// Remarks:
	//-----------------------------------------------------------------------
	void Insert(int index, const CXTPChartString& str)
	{
		m_arrList.InsertAt(index, str);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get a string at a particular index.
	// Parameters:
	//     index    - The index from which the string to be extracted.
	// Returns:
	//     A CXTPChartString value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartString GetAt(int nIndex)
	{
		if (nIndex >= 0 && nIndex < m_arrList.GetSize())
			return m_arrList[nIndex];

		return CXTPChartString();
	}

protected:
	CXTPChartStrings m_arrList; // The collection of strings.
};

CXTPChartQualitativeScaleTypeMap::CXTPChartQualitativeScaleTypeMap(CXTPChartAxis* pAxis)
	: CXTPChartScaleTypeMap(pAxis)
{
	m_pStorage = new CStorage();
}

CXTPChartQualitativeScaleTypeMap::~CXTPChartQualitativeScaleTypeMap()
{
	delete m_pStorage;
}

int CXTPChartStrings::IndexOf(const CXTPChartString& strValue) const
{
	for (int i = 0; i < GetSize(); i++)
	{
		if (GetAt(i) == strValue)
			return i;
	}
	return -1;
}

int CXTPChartQualitativeScaleTypeMap::FillNextsList(CXTPChartSeries* pSeries, int i,
													CXTPChartStrings& nexts,
													const CXTPChartString& argument_i)
{
	nexts.RemoveAll();

	for (int j = i + 1; j < pSeries->GetPoints()->GetCount(); j++)
	{
		const CXTPChartSeriesPoint* pPoint = pSeries->GetPoints()->GetAt(j);

		if (NULL != pPoint)
		{
			CXTPChartString argument_j = pPoint->GetArgument();
			if (argument_j == argument_i)
				continue;

			if (nexts.IndexOf(argument_j) >= 0)
				continue;

			if (m_pStorage->Find(argument_j))
				return m_pStorage->IndexOf(argument_j);

			nexts.Add(argument_j);
		}
	}
	return -1;
}

void CXTPChartQualitativeScaleTypeMap::UpdateSeries(CXTPChartSeries* pSeries)
{
	int i = 0;
	CXTPChartStrings nexts;

	while (i < pSeries->GetPoints()->GetCount())
	{
		CXTPChartString argument_i;

		const CXTPChartSeriesPoint* pPoint = pSeries->GetPoints()->GetAt(i);

		if (NULL != pPoint)
		{
			argument_i = pPoint->GetArgument();
		}

		if (m_pStorage->Find(argument_i))
		{
			i++;
			continue;
		}

		int index = FillNextsList(pSeries, i, nexts, argument_i);
		if (index == -1)
		{
			m_pStorage->Add(argument_i);
			m_pStorage->AddRange(nexts);
		}
		else
		{
			m_pStorage->InsertRange(index, nexts);
			m_pStorage->Insert(index, argument_i);
		}
		i += (int)nexts.GetSize() + 1;
	}
}

void CXTPChartQualitativeScaleTypeMap::UpdateSeriesPointArgument(CXTPChartSeriesPoint* pPoint)
{
	if (NULL != pPoint)
	{
		pPoint->SetInternalArgumentValue(ValueToInternal(pPoint->GetArgument()));
	}
}

double CXTPChartQualitativeScaleTypeMap::ValueToInternal(const CXTPChartString& str) const
{
	return m_pStorage->IndexOf(str);
}

CXTPChartString CXTPChartQualitativeScaleTypeMap::InternalToValue(CXTPChartNumberFormat* pFormat,
																  double dMark) const
{
	CXTPChartString str = m_pStorage->GetAt((int)dMark);

	return pFormat->FormatString(str);
}
