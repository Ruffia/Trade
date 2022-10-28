#include "StdAfx.h"
#include "Axis.h"

CAxis::CAxis():m_minExtY(DBL_MAX), m_maxExtY(-DBL_MAX), 
	m_startX(DBL_MAX),m_endX(-DBL_MAX),m_axOffsXF(0.0f), m_axXPos(BETWEEN), m_axYPos(MIDDLE),
	m_chModeX(MODE_FULLX), m_chModeY(MODE_FULLY), m_nGridXGroupCount(10),m_nGridYGroupCount(10),
	m_scX(1.0), m_scY(1.0), m_pMatrixD(NULL),m_bDrawXAxis(TRUE),m_fOffSetYAxis(0.f),
	m_colAxis((ARGB)Color::Black), m_colGrid((ARGB)Color::Gray)
{
	m_axOffsXF      = 0.0f;         // Can't be defined until container wnd is created
	m_axXPos        = BETWEEN;
	m_axYPos        = MIDDLE;
}


CAxis::~CAxis()
{
	if (m_pMatrixD != NULL)
	{
		delete m_pMatrixD;
		m_pMatrixD = NULL;
	}
}



// Gets axis X pos AX_YPOS (first) and axis X float offsY(second)
PAIR_XAXPOS CAxis::GetXAxisPos(RectF rChartF, double minY, double maxY)
{
	AX_YPOS axYPos;
	float vertOffs;

	// Let's begin with X axis
	if ((minY < 0)&&(maxY > 0))
	{
		double offsXY = rChartF.Height*fabs(maxY)/(maxY - minY);
		vertOffs = rChartF.GetTop() + float(offsXY);
		axYPos = MIDDLE;        // Y pos of Axis X, somewhere inside the y range
	}
	else if (minY >= 0)
	{
		vertOffs = rChartF.GetBottom();
		axYPos = BOTTOM;        // Y pos of Axis X at bottom of the draw rect
	}
	else if (maxY <= 0)
	{
		vertOffs = rChartF.GetTop();
		axYPos = TOP;           // At the top of the draw rect
	}

	return make_pair(axYPos, vertOffs);
}

// Gets axis Y pos AX_XPOS (first) and axis Y float offsX(second)
PAIR_YAXPOS CAxis::GetYAxisPos(RectF rChartF, double startX, double endX)
{
	// Y axis
	AX_XPOS axXPos;
	float horzOffs;

	if ((startX < 0)&&(endX > 0))
	{
		double offsYX = rChartF.Width*fabs(startX)/(endX - startX);
		horzOffs = rChartF.GetLeft() + float(offsYX);
		axXPos = BETWEEN;      // Somewhere between minX, maxX
	}
	else if (startX >= 0)
	{
		horzOffs = rChartF.GetLeft();
		axXPos = LEFT;         // At the left of the draw rect
	}
	else if (endX <= 0)
	{
		horzOffs = rChartF.GetRight();
		axXPos = RIGHT;       // At the right side of the draw rect
	}

	return make_pair(axXPos, horzOffs);
}


// Restores the mode to ZOOM or FULLX after interruption between the first and second zoom clicks
CH_MODE CAxis::RestoreModeX(void)
{
	if (m_chModeX == MODE_ZOOMINGX)
	{
		m_chModeX = MODE_FULLX;
	}
	return m_chModeX;
}


CH_MODE CAxis::RestoreModeY(void)
{
	if (m_chModeY == MODE_ZOOMINGY)
	{
		m_chModeY = MODE_ZOOMEDY;
	}
	return m_chModeY;
}


// Draw functions
PointF CAxis::DrawAxes(double startX, double endX, double minY, 
								 double maxY, Graphics* grPtr, float dpiRatio)
{
	Pen axisPen(m_colAxis, AXIS_PENWIDTH*dpiRatio);  

	PointF pntStartF, pntEndF, pntCoordZeroF;
	// Let's begin with X axis

	PAIR_XAXPOS pair_axXYPos = GetXAxisPos(m_drawRF, minY, maxY);
	m_axYPos = pair_axXYPos.first;
	pntStartF.Y = pntEndF.Y = pair_axXYPos.second;
	pntStartF.X = m_drawRF.GetLeft();   // Don't worry: draw area is clipped
	pntEndF.X   = m_drawRF.GetRight();
	if (m_bVisible)
	{
		grPtr->DrawLine(&axisPen, pntStartF, pntEndF);
	}

	pntCoordZeroF.Y = pntStartF.Y;

	// Y axis
	PAIR_YAXPOS pair_axYXPos = GetYAxisPos(m_drawRF, startX, endX);
	m_axXPos = pair_axYXPos.first;
	pntStartF.X = pntEndF.X = pair_axYXPos.second;
	pntStartF.Y = m_drawRF.GetBottom();
	pntEndF.Y   = m_drawRF.GetTop() - DR_SPACE*dpiRatio;
	if (m_bVisible)
	{
		grPtr->DrawLine(&axisPen, pntStartF, pntEndF);
	}
	
	pntCoordZeroF.X = pntStartF.X;

	m_axOffsXF = pntCoordZeroF.Y; 
	return pntCoordZeroF;
}


void CAxis::DrawGrid(PointF coordOrigF, Graphics* grPtr, float dpiRatio)
{
	USES_CONVERSION;
	float fDeltaX = m_drawRF.Width  * 1.f/m_nGridXGroupCount;
	float fDeltaY = m_drawRF.Height * 1.f/m_nGridYGroupCount;

	float limTop    = m_drawRF.GetTop();
	float limBottom = m_drawRF.GetBottom();
	float limLeft   = m_drawRF.GetLeft();
	float limRight  = m_drawRF.GetRight();

	Pen gridPen(m_colGrid, 1.0f*dpiRatio);
	gridPen.SetDashStyle(DashStyleDash);
	gridPen.SetDashCap(DashCapRound);

	float xF, yF;

	yF = coordOrigF.Y + fDeltaY;
	while (yF < limBottom)
	{
		if (m_bVisible)
		{
			grPtr->DrawLine(&gridPen, limLeft, yF, limRight, yF);
		}
		yF += fDeltaY;
	}

	yF = coordOrigF.Y - fDeltaY;
	while (yF > limTop)
	{
		if (m_bVisible)
		{
			grPtr->DrawLine(&gridPen, limLeft, yF, limRight, yF);
		}		
		yF -= fDeltaY;
	}

	xF = coordOrigF.X + fDeltaX;
	while (xF < limRight)
	{
		if (m_bVisible)
		{
			grPtr->DrawLine(&gridPen, xF, limTop, xF, limBottom);
		}
		xF += fDeltaX;
	}

	xF = coordOrigF.X - fDeltaX;
	while (xF > limLeft)
	{
		if (m_bVisible)
		{
			grPtr->DrawLine(&gridPen, xF, limTop, xF, limBottom);
		}
		xF -= fDeltaX;
	}
}


void CAxis::SetTransformMatrixD(MatrixD* pMatrixD) 
{ 
	if (m_pMatrixD != NULL) 
		delete m_pMatrixD; 
	m_pMatrixD = pMatrixD->Clone();
}


// Calculate scaleX, scaleY
double CAxis::UpdateScaleX(const float drawRectWidth, double startX, double endX)
{
	double scX = 1.0;
	double rangeX = endX - startX;
	scX = drawRectWidth/rangeX;
	return scX;
}

double CAxis::UpdateScaleY(const float drawRectHeight, double minY, double maxY)
{
	double scY = 1.0;
	double rangeY = maxY - minY;
	scY = drawRectHeight/rangeY;
	return scY;
}

// Calculates scX, scY but does not set m_scX, m_scY, leaves it to the user
PAIR_DBLS CAxis::UpdateScales(const RectF drawRectF, 
										double startX, double endX, double minY, double maxY)
{
	RectF dRF = drawRectF;
	if (m_chModeY == MODE_FULLY || m_chModeY == MODE_MOVEDY || m_chModeY == MODE_MOVEY)
		dRF.Inflate(0.0f, -0.1f*drawRectF.Height); // Reserve 20% to beautify full picture
	double scX = UpdateScaleX(dRF.Width, startX, endX);
	double scY = UpdateScaleY(dRF.Height, minY, maxY);
	return make_pair(scX, scY);
}