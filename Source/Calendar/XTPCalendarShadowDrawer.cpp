// XTPCalendarShadowDrawer.cpp : implementation file
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

#include "Common/Base/Diagnostic/XTPDisableAdvancedWarnings.h"
#include <math.h>
#include "Common/Base/Diagnostic/XTPEnableAdvancedWarnings.h"

#include "Common/XTPCasting.h"
#include "Calendar/XTPCalendarShadowDrawer.h"

#include "Common/Base/Diagnostic/XTPDisableNoisyWarnings.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

CXTPCalendarShadowDrawer::CXTPCalendarShadowDrawer(CDC* pDC, double dMaxAlpha)
	: m_pDC(pDC)
	, m_dMaxAlpha(dMaxAlpha)
{
	ASSERT(m_pDC);
}

CXTPCalendarShadowDrawer::~CXTPCalendarShadowDrawer()
{
}

int AFX_CDECL CXTPCalendarShadowDrawer::CheckValue(int iValue)
{
	return ((iValue > 255) ? 255 : ((iValue < 0) ? 0 : iValue));
}

COLORREF AFX_CDECL CXTPCalendarShadowDrawer::AlphaPixel(const COLORREF crPixel, const UINT i)
{
	return RGB(CheckValue(XTPToInt(GetRValue(crPixel) - i)),
			   CheckValue(XTPToInt(GetGValue(crPixel) - i)),
			   CheckValue(XTPToInt(GetBValue(crPixel) - i)));
}

void CXTPCalendarShadowDrawer::DrawShadowRectPart(const CRect& rcShadow, const XTPShadowPart part)
{
	ASSERT(m_pDC);
	if (!m_pDC || !m_pDC->GetSafeHdc())
	{
		return;
	}

	// start processing
	HDC hdc;		 // handle of the DC we will create
	HBITMAP hbitmap; // bitmap handle
	BITMAPINFO bmi;  // bitmap header
	VOID* pvBits;	// pointer to DIB section

	// create a DC for our bitmap -- the source DC for AlphaBlend
	hdc = ::CreateCompatibleDC(m_pDC->GetSafeHdc());
	if (!hdc)
		return;

	// setup bitmap info
	// zero the memory for the bitmap info
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	// set the bitmap width and height. Later on, the blending will occur in the center of each of
	// the three areas.
	bmi.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth		= max(rcShadow.Width(), 0);
	bmi.bmiHeader.biHeight		= max(rcShadow.Height(), 0);
	bmi.bmiHeader.biPlanes		= 1;
	bmi.bmiHeader.biBitCount	= 32; // four 8-bit components
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = XTPToDWORD(bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 4);
	// create our DIB section and select the bitmap into the dc
	if ((bmi.bmiHeader.biWidth > 0) && (bmi.bmiHeader.biHeight > 0))
	{
		hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
		if (hbitmap)
		{
			HGDIOBJ hOld = SelectObject(hdc, hbitmap);
			// Copy old picture values to the created bitmap
			if (::BitBlt(hdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight,
						 m_pDC->GetSafeHdc(), rcShadow.left, rcShadow.top, SRCCOPY))
			{
				ULONG ulBitmapWidth  = XTPToULong(bmi.bmiHeader.biWidth);
				ULONG ulBitmapHeight = XTPToULong(bmi.bmiHeader.biHeight);

				// Adjust all pixels of the picture
				switch (part)
				{
					case xtpShadowLR:
						ApplyShadowLR((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadowTB:
						ApplyShadowTB((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadowTR:
						ApplyShadowTR((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadowBR:
						ApplyShadowBR((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadowBL:
						ApplyShadowBL((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007Right:
						ApplyShadow2007Right((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007Bottom:
						ApplyShadow2007Bottom((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007TopRight:
						ApplyShadow2007TR((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007BottomLeft:
						ApplyShadow2007BL((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007BottomRight:
						ApplyShadow2007BR((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					case xtpShadow2007Center:
						ApplyShadow2007Center((UINT*)pvBits, ulBitmapWidth, ulBitmapHeight);
						break;
					default: break;
				}

				// Copy modified picture to the old DC
				if (pvBits)
				{
					m_pDC->BitBlt(rcShadow.left, rcShadow.top, bmi.bmiHeader.biWidth,
								  bmi.bmiHeader.biHeight, CDC::FromHandle(hdc), 0, 0, SRCCOPY);
				}
			}
			// do cleanup
			SelectObject(hdc, hOld);
			DeleteObject(hbitmap);
		}
	}
	// do cleanup
	DeleteDC(hdc);
}

void CXTPCalendarShadowDrawer::ApplyShadowLR(UINT* pBitmap, const ULONG ulBitmapWidth,
											 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	UINT nAlpha					   = 0;
	const double dPercentsPerPixel = m_dMaxAlpha / (double)ulBitmapWidth;
	double dAlpha				   = m_dMaxAlpha;

	for (ULONG x = 0; x < ulBitmapWidth; x++)
	{
		nAlpha = (UINT)dAlpha;
		if (nAlpha != 0)
		{
			for (ULONG y = 0; y < ulBitmapHeight; y++)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
		dAlpha -= dPercentsPerPixel;
	}
}

void CXTPCalendarShadowDrawer::ApplyShadowTB(UINT* pBitmap, const ULONG ulBitmapWidth,
											 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	UINT nAlpha					   = 0;
	const double dPercentsPerPixel = m_dMaxAlpha / (double)ulBitmapHeight;
	double dAlpha				   = 0;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		nAlpha = (UINT)(dAlpha);
		if (nAlpha != 0)
		{
			for (ULONG x = 0; x < ulBitmapWidth; x++)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
		dAlpha += dPercentsPerPixel;
	}
}

void CXTPCalendarShadowDrawer::ApplyShadowTR(UINT* pBitmap, const ULONG ulBitmapWidth,
											 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	UINT nAlpha					   = 0;
	const double dPercentsPerPixel = m_dMaxAlpha / (double)min(ulBitmapWidth, ulBitmapHeight);

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			nAlpha = (UINT)(m_dMaxAlpha
							- sqrt((double)(ulBitmapHeight - y) * (x)) * dPercentsPerPixel);
			nAlpha = min(nAlpha, (UINT)((ulBitmapWidth - x) * dPercentsPerPixel));
			nAlpha = min(nAlpha, (UINT)((y + 1) * dPercentsPerPixel));
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007Right(UINT* pBitmap, const ULONG ulBitmapWidth,
													const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	static const UINT uiShadow[5] = { 38, 34, 30, 27, 25 };
	UINT nAlpha					  = 0;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			ULONG dx = ulBitmapWidth - 5;
			nAlpha   = uiShadow[x < dx ? 0 : x - dx];

			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007Bottom(UINT* pBitmap, const ULONG ulBitmapWidth,
													 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	static const UINT uiShadow[3] = { 50, 40, 25 };
	UINT nAlpha					  = 0;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		if (0 == y)
			nAlpha = uiShadow[2];
		else if (1 == y)
			nAlpha = uiShadow[1];
		else
			nAlpha = uiShadow[0];

		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007TR(UINT* pBitmap, const ULONG ulBitmapWidth,
												 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	static const UINT nShadowTR[6][5] = { { 22, 17, 7, 0, 0 },	{ 31, 33, 35, 15, 0 },
										  { 32, 30, 28, 34, 10 }, { 35, 32, 29, 30, 21 },
										  { 37, 33, 30, 27, 25 }, { 38, 34, 30, 27, 25 } };

	UINT nAlpha = 0;
	ULONG dx	= ulBitmapWidth - 5;
	ULONG dy	= ulBitmapHeight - 6;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		ULONG y1 = y < dy ? 0 : y - dy;

		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			ULONG x1 = x < dx ? 0 : x - dx;
			nAlpha   = nShadowTR[5 - y1][x1];
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007BL(UINT* pBitmap, const ULONG ulBitmapWidth,
												 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	static const UINT uiShadowBL[5][5] = { { 33, 30, 34, 44, 50 },
										   { 19, 33, 33, 40, 50 },
										   { 10, 35, 30, 35, 43 },
										   { 0, 18, 35, 30, 33 },
										   { 0, 0, 11, 24, 25 } };

	UINT nAlpha = 0;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			ULONG y1 = (y <= 4) ? y : 4;
			ULONG x1 = (x <= 4) ? x : 4;

			nAlpha = uiShadowBL[4 - y1][x1];
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007BR(UINT* pBitmap, const ULONG ulBitmapWidth,
												 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	const int sz						 = 5;
	static const UINT uiShadowBL[sz][sz] = { { 213, 215, 219, 224, 230 },
											 { 217, 219, 223, 224, 233 },
											 { 219, 221, 225, 221, 241 },
											 { 226, 226, 221, 236, 254 },
											 { 230, 231, 241, 254, 255 } };

	UINT nAlpha = 0;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		ULONG y1 = y > (sz - 1) ? sz - 1 : y;

		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			ULONG x1 = x < (ulBitmapWidth - sz) ? 0 : x - (ulBitmapWidth - sz);

			nAlpha = 255 - uiShadowBL[4 - y1][x1];
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadow2007Center(UINT* pBitmap, const ULONG ulBitmapWidth,
													 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
			pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, 42);
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadowBR(UINT* pBitmap, const ULONG ulBitmapWidth,
											 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	UINT nAlpha					   = 0;
	const double dPercentsPerPixel = m_dMaxAlpha / (double)min(ulBitmapHeight, ulBitmapWidth);

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			nAlpha = (UINT)(m_dMaxAlpha - sqrt((double)y * x) * dPercentsPerPixel);
			nAlpha = min(nAlpha, (UINT)((ulBitmapWidth - x) * dPercentsPerPixel));
			nAlpha = min(nAlpha, (UINT)((ulBitmapHeight - y) * dPercentsPerPixel));
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}

void CXTPCalendarShadowDrawer::ApplyShadowBL(UINT* pBitmap, const ULONG ulBitmapWidth,
											 const ULONG ulBitmapHeight)
{
	if (!pBitmap)
		return;

	UINT nAlpha					   = 0;
	const double dPercentsPerPixel = m_dMaxAlpha / (double)min(ulBitmapHeight, ulBitmapWidth);

	for (ULONG y = 0; y < ulBitmapHeight; y++)
	{
		for (ULONG x = 0; x < ulBitmapWidth; x++)
		{
			nAlpha = (UINT)(m_dMaxAlpha
							- sqrt((double)(ulBitmapHeight - y) * (ulBitmapWidth - x))
								  * dPercentsPerPixel);
			nAlpha = min(nAlpha, (UINT)(x * dPercentsPerPixel));
			nAlpha = min(nAlpha, (UINT)(y * dPercentsPerPixel));
			if (nAlpha != 0)
			{
				UINT uiPixel				   = pBitmap[x + y * ulBitmapWidth];
				pBitmap[x + y * ulBitmapWidth] = AlphaPixel(uiPixel, nAlpha);
			}
		}
	}
}
