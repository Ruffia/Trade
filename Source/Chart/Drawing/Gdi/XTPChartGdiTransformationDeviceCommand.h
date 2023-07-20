// XTPChartGdiTransformationDeviceCommand.h
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
#if !defined(__XTPCHARTGDITRANSFORMATIONDEVICECOMMAND_H__)
#	define __XTPCHARTGDITRANSFORMATIONDEVICECOMMAND_H__

#	if _MSC_VER >= 1000
#		pragma once
#	endif // _MSC_VER >= 1000

#	include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

namespace Gdiplus
{
class GpRegion;
}

//===========================================================================
// Summary:
//     This class helps to save the drawing state. This object is a kind of
//     CXTPChartDeviceCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGdiSaveStateDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiSaveStateDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartGdiSaveStateDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called before a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called after a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

protected:
	UINT m_nState;
};

class _XTP_EXT_CLASS CXTPChartGdiClipDeviceCommand : public CXTPChartClipDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiClipDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartGdiClipDeviceCommand(CRect rcClip);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called before a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called after a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

protected:
	CRect m_rcState; // The default clipping rectangle.
};

class _XTP_EXT_CLASS CXTPChartGdiClipRegionDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartClipDeviceCommand object.
	//-----------------------------------------------------------------------
	CXTPChartGdiClipRegionDeviceCommand(const CXTPChartPoints& points);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartClipDeviceCommand object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	~CXTPChartGdiClipRegionDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called before a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void BeforeExecute(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method gets called after a command is executed which allows for
	//     the customization of command behavior.
	// Parameters:
	//     pDC - A pointer to the chart device context.
	//-----------------------------------------------------------------------
	virtual void AfterExecute(CXTPChartDeviceContext* pDC);

protected:
	Gdiplus::GpRegion* m_pGpState; // The clipping rectangle.
	Gdiplus::GpRegion* m_pGpClip;  // The default clipping rectangle.
};

//===========================================================================
// Summary:
//     This class abstracts a rotation device command, which does rotation
//     transformations. This is a kind of CXTPChartDeviceCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGdiRotateDeviceCommand : public CXTPChartRotateDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiRotateDeviceCommand object.
	// Parameters:
	//     fAngle - The angle of rotation.
	//-----------------------------------------------------------------------
	CXTPChartGdiRotateDeviceCommand(float fAngle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiRotateDeviceCommand object.
	// Parameters:
	//     fAngle        - The angle of rotation.
	//     rotateVector  - The vector to be rotated.
	//-----------------------------------------------------------------------
	CXTPChartGdiRotateDeviceCommand(float fAngle, const CXTPPoint3d& rotateVector);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext. This
	//     function acts polymorphically to do the actual drawing of the chart element
	//     to which this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	//-----------------------------------------------------------------------
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);
};

//===========================================================================
// Summary:
//     This class abstracts a rotation device command, which does translation
//     on vertices. This is a kind of CXTPChartDeviceCommand.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGdiTranslateDeviceCommand : public CXTPChartTranslateDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGdiTranslateDeviceCommand object.
	// Parameters:
	//     dx - The change in X direction.
	//     dy - The change in Y direction.
	//-----------------------------------------------------------------------
	CXTPChartGdiTranslateDeviceCommand(double dx, double dy);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext. This
	//     function acts polymorphically to do the actual drawing of the chart element
	//     to which this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	//-----------------------------------------------------------------------
	virtual void ExecuteOverride(CXTPChartDeviceContext* pDC);

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;
};

#	include "Common/Base/Diagnostic/XTPEnableNoisyWarnings.h"
#endif //#if !defined(__XTPCHARTGDITRANSFORMATIONDEVICECOMMAND_H__)
//}}AFX_CODEJOCK_PRIVATE
