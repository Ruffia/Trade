#pragma once
#include <vector>
using namespace std;
#include "ChartDef.h"
#include "Util.h"

class CAxis
{
public:
	CAxis();
	~CAxis();

	double GetMinY(void) const {return m_minExtY;}
	double GetMaxY(void) const {return m_maxExtY;}

	double GetStartX(void) const {return m_startX;}
	double GetEndX(void) const {return m_endX;}

	// To use with UpdateDataLegend
	// Gets axis X pos AX_YPOS (first) and axis X float offsY(second)
	AX_XPOS GetXAxisPos(void) const {return m_axXPos;} 
	PAIR_XAXPOS GetXAxisPos(RectF rChartF, double minY, double maxY);
	// Gets axis Y pos AX_XPOS (first) and axis Y float offsX(second)
	AX_YPOS GetYAxisPos(void) const {return m_axYPos;}
	PAIR_YAXPOS GetYAxisPos(RectF rChartF, double startX, double endX);

	void SetAxesColor(Color col) {m_colAxis = col;}
	void SetGridColor(Color col) {m_colGrid = col;}

	float GetAxisOffsetX(void) const {return m_axOffsXF;}

	CH_MODE GetChModeX(void) const {return m_chModeX;}
	CH_MODE GetChModeY(void) const {return m_chModeY;}

	void  SetGridXGroupCount(int nGridXGroupCount) {m_nGridXGroupCount = nGridXGroupCount;}
	void  SetGridYGroupCount(int nGridYGroupCount) {m_nGridYGroupCount = nGridYGroupCount;}
	void  SetGridXGraducation(const vector<double>& vXGraducation) {m_vecGridXGraduation = vXGraducation;}
	void  SetGridYGraducation(const vector<double>& vYGraducation) {m_vecGridYGraduation = vYGraducation;}

	// Access functions: access to m_startX, m_endX, m_minExtY, m_maxExtY
	double GetInitialStartX(void) const
	{return m_startX;}

	double GetInitialEndX(void) const
	{return m_endX;}

	double GetInitialMinExtY(void) const
	{return m_minExtY;}

	double GetInitialMaxExtY(void) const
	{return m_maxExtY;}

	vector<AXISSECTION>& GetYSection() 
	{ return m_vYSection;}

	void SetDrawRect(const RectF& drawRF)
	{ m_drawRF = drawRF;}

	void SetYSection(const vector<AXISSECTION>& vSection) {m_vYSection = vSection;}

	// Access to the member m_pMatrix:
	MatrixD* GetTransformMatrixD(void) const {return m_pMatrixD;}

	double GetScX(void) const {return m_scX;}
	double GetScY(void) const {return m_scY;}

	Color GetAxesColor(void) const { return m_colAxis;}
	Color GetGridColor(void) const {return m_colGrid;}

	// Restores the state to ZOOM or FULLX after interruption between the first and second zoom clicks
	CH_MODE RestoreModeX(void);
	CH_MODE RestoreModeY(void);

	double UpdateScaleX(const float drawRectWidth, double startX, double endX); 
	double UpdateScaleY(const float drawRectHeight, double minY, double maxY);
	// Calculates scX, scY but does not set m_scX, m_scY
	std::pair<double, double> UpdateScales(const RectF drawRectF, 
		double minX, double maxX, double minY, double maxY);

	void SetTransformMatrixD(MatrixD* pMatrixD); 

	virtual PointF DrawAxes(double startX, double endX, double minY, 
		double maxY, Graphics* grPtr, float dpiRatio = 1.0f);

	virtual void DrawGrid(PointF coordOrigF, Graphics* grPtr, float dpiRatio = 1.f);

	void SetVisibility(bool bVisible) { m_bVisible = bVisible;}

	bool ToggleVisibility() 
	{
		m_bVisible = !m_bVisible; 
		return m_bVisible;
	}

public:
	double m_minExtY;         // Min coordinate of the Y axis
	double m_maxExtY;         // Max coordinate of the Y axis
	double m_startX;          // Start X coord
	double m_endX;            // Last X coord (not included)
	float  m_axOffsXF;        // Y Offset of X axis
	AX_XPOS m_axXPos;         // Position of zero on the X axis 
	AX_YPOS m_axYPos;         // Position of zero on the Y axis
	CH_MODE m_chModeX;     
	CH_MODE m_chModeY;     

	int  m_nGridXGroupCount;                       //����X�����������
	int  m_nGridYGroupCount;                       //����, Y�����������
	vector<double>   m_vecGridXGraduation;         //����X�᷽��Ŀ̶�
	vector<double>   m_vecGridYGraduation;         //����Y�᷽��Ŀ̶�
	vector<AXISSECTION>  m_vYSection;              //Y����ֵ-->����ӳ��
	int  m_nIdx;                                   //��������
	string m_strName;                              //����������

	double m_scX;
	double m_scY;
	MatrixD* m_pMatrixD;      // Transform matrix from the data pnt space to the client coord

	bool m_bDrawXAxis;       // �Ƿ����X ��
	RectF  m_drawRF;         // ��ͼ����
	float  m_fOffSetYAxis;        // ���׼Y ���ƫ�ƾ���,����Y���ˮƽλ��
	Color m_colAxis;
	Color m_colGrid;
	bool  m_bVisible;
};
