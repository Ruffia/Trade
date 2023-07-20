// XTPChartElement.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTELEMENT_H__)
#	define __XTPCHARTELEMENT_H__
//}}AFX_CODEJOCK_PRIVATE

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

class CXTPChartFont;
class CXTPChartContent;
class CXTPChartElementView;
class CXTPPropExchange;
class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartAppearance;

enum XTPChartUpdateOptions
{
	xtpChartUpdateView   = 1, // Recreate View
	xtpChartUpdateLayout = 2, // Don't recreate View but call Calculate
	xtpChartUpdateRedraw = 4, // Rebuild Command Tree
};

//-----------------------------------------------------------------------
// Summary:
//      A base class for all chart objects.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPChartObject
#	ifdef _XTP_ACTIVEX
	: public CXTPCmdTarget
#	else
	: public CObject
#	endif
{
	DECLARE_DYNAMIC(CXTPChartObject);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//      Constructs an empty chart object.
	// Parameters:
	//      bIs3D - By default all derived objects are assumed to work in 2D
	//              mode which corresponds to FALSE used as a value for this parameter.
	//              However if a derived object is associated with 3D mode,
	//              e.g. a command the render 3D graphics or 3D device context,
	//              this value must be TRUE.
	//-----------------------------------------------------------------------
	CXTPChartObject();
	CXTPChartObject(BOOL bIs3D); // <combine CXTPChartObject::CXTPChartObject>

public:
	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if a derived object is used in 3D mode.
	// Returns:
	//      TRUE if a derived object is used in 3D mode, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL Is3D() const;

	//{{AFX_CODEJOCK_PRIVATE
protected:
	void Set3D();
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL m_bIs3D; // TRUE indicated the derived object is used in 3D mode.
};

AFX_INLINE CXTPChartObject::CXTPChartObject()
	: m_bIs3D(FALSE)
{
}
AFX_INLINE CXTPChartObject::CXTPChartObject(BOOL bIs3D)
	: m_bIs3D(bIs3D)
{
}
AFX_INLINE void CXTPChartObject::Set3D()
{
	m_bIs3D = TRUE;
}
AFX_INLINE BOOL CXTPChartObject::Is3D() const
{
	return m_bIs3D;
}

//===========================================================================
// Summary:
//     This class abstracts a chart element, this class acts as a base class for
//     all chart elements.
// Remarks:
//     This is an MFC class, which has capacity to route Windows command messages.
//     Each element has an owner element.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartElement : public CXTPChartObject
{
	DECLARE_DYNAMIC(CXTPChartElement);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartElement object.
	//-----------------------------------------------------------------------
	CXTPChartElement();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartElement object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartElement();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to decrease the usage count of the object.
	//-------------------------------------------------------------------------
	virtual void Release();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the owner element.
	// Returns
	//     Returns the parent CXTPChartElement.
	//-------------------------------------------------------------------------
	CXTPChartElement* GetOwner() const;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this virtual function to trigger a re-painting when there is
	//     a change in the intrinsic properties of the chart.
	//-------------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this virtual function to get the chart content object.
	// Returns:
	//     A pointer to the chart content object.
	// Remarks:
	//     A chart content object is an aggregate of various chart components
	//     like panels, titles, series, legend etc.
	//-------------------------------------------------------------------------
	CXTPChartContent* GetContent() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the the appearance of the chart.
	// Returns:
	//     A pointer to a CXTPChartAppearance object.
	//-----------------------------------------------------------------------
	CXTPChartAppearance* GetAppearance() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads this object from or writes this object to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:
#	ifdef _XTP_ACTIVEX

public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPChartElement);
//}}AFX_CODEJOCK_PRIVATE
#	else
	DWORD InternalAddRef();
	DWORD InternalRelease();

	long m_dwRef;
#	endif

protected:
	CXTPChartElement* m_pOwner; // Owner of the element.

	friend class CXTPChartElementCollection;
};

//===========================================================================
// Summary:
//     This class represents the chart container. This class acts as a container
//     for the chart content.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartContainer
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartContainer object.
	//-----------------------------------------------------------------------
	CXTPChartContainer();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartContainer object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartContainer();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to trigger a re-painting when there is a change
	//     in the intrinsic properties of the chart.
	//-------------------------------------------------------------------------
	virtual void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set capture to view.
	// Parameters:
	//     pView - View that will receive all mouse messages.
	//-----------------------------------------------------------------------
	virtual void SetCapture(CXTPChartElementView* pView);

	//-----------------------------------------------------------------------
	// Summary:
	//      Enters a mode in which a container is expected to be frequently
	//      updated.
	// See also:
	//      EndFrequentUpdates, IsBeingFrequentlyUpdated
	//-----------------------------------------------------------------------
	void BeginFrequentUpdates();

	//-----------------------------------------------------------------------
	// Summary:
	//      Determines if the container is currently being frequently updated.
	// Returns:
	//      TRUE if the container is currently being frequently updated, FALSE otherwise.
	// See also:
	//      BeginFrequentUpdates, EndFrequentUpdates
	//-----------------------------------------------------------------------
	BOOL IsBeingFrequentlyUpdated() const;

	//-----------------------------------------------------------------------
	// Summary:
	//      Leave the frequent update mode and performs the final update.
	// Parameters:
	//      updateOptions - The final update type.
	// See also:
	//      BeginFrequentUpdates, IsBeingFrequentlyUpdated
	//-----------------------------------------------------------------------
	void EndFrequentUpdates(XTPChartUpdateOptions updateOptions = xtpChartUpdateRedraw);

private:
	BOOL m_bBeingFrequentlyUpdated;
};

AFX_INLINE BOOL CXTPChartContainer::IsBeingFrequentlyUpdated() const
{
	return m_bBeingFrequentlyUpdated;
}

//===========================================================================
// Summary:
//     This class abstracts a chart text element. This class is a kind of
//     CXTPChartElement class.
// Remarks:
//     This class acts as an abstract base class for various specialized chart text
//     elements like axis label, axis title etc.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTextElement : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartTextElement);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the font of the text element.
	// Returns:
	//     A pointer to a chart font object, which holds the current font of the
	//     text element.
	//-------------------------------------------------------------------------
	virtual CXTPChartFont* GetFont() const = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check if anti-aliasing is ON or OFF. Anti-aliasing is
	//     the smoothing of sharp edges of text and drawing.
	// Returns:
	//     A BOOL value. TRUE if anti-aliasing is ON, FALSE if anti-aliasing is OFF.
	//-------------------------------------------------------------------------
	virtual BOOL GetAntialiasing() const = 0;
};

//===========================================================================
// Summary:
//     This class abstracts a chart collection. This class is a kind of
//     CXTPChartElement class.
// Remarks:
//     This class acts as an abstract base class for collection objects
//     such as a collection of points, titles, panels, strips.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartElementCollection : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartElementCollection)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartElementCollection object.
	//-----------------------------------------------------------------------
	CXTPChartElementCollection();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartElementCollection object, handles cleanup.
	//-------------------------------------------------------------------------
	~CXTPChartElementCollection();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function removes all the individual objects
	//     from this collection object.
	// See Also:
	//     RemoveAt
	//-------------------------------------------------------------------------
	virtual void RemoveAll();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the number of objects in
	//     the internal array of this object.
	// Returns:
	//     The number of point elements present in this object.
	//-------------------------------------------------------------------------
	virtual int GetCount() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to remove an object from the collection.
	// Parameters:
	//     nIndex - Index of the object to remove.
	//-------------------------------------------------------------------------
	virtual void RemoveAt(int nIndex);

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to remove an object from the collection.
	// Parameters:
	//     pElement - Object to remove.
	//-------------------------------------------------------------------------
	virtual void Remove(CXTPChartElement* pElement);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get an element by its index.
	// Parameters:
	//     nIndex - Index of the object to get.
	// Returns:
	//     Element by its index.
	//-------------------------------------------------------------------------
	CXTPChartElement* GetAt(int nIndex) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the index of a specified element.
	// Parameters:
	//     pElement - Object to test.
	// Returns:
	//     Index of pElement inside internal collection.
	//-------------------------------------------------------------------------
	int IndexOf(CXTPChartElement* pElement) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new element to the collection.
	// Parameters:
	//     pElement - Object to add.
	//     nIndex - Index of object to add.
	//-------------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPChartElement* pElement);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to decrease the usage count of the object.
	//-------------------------------------------------------------------------
	virtual void Release();

protected:
	CArray<CXTPChartElement*, CXTPChartElement*> m_arrElements; // Array of CXTPChartElement
																// pointer.
};

AFX_INLINE CXTPChartElement* CXTPChartElement::GetOwner() const
{
	return m_pOwner;
}

AFX_INLINE CXTPChartElement* CXTPChartElementCollection::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < m_arrElements.GetSize() ? m_arrElements.GetAt(nIndex) : NULL;
}
AFX_INLINE int CXTPChartElementCollection::GetCount() const
{
	return (int)m_arrElements.GetSize();
}

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTELEMENT_H__)
