#pragma once
#include <map>
using namespace std;
#include "Axis.h"

class CAxisManager
{
public:
	CAxisManager();
	~CAxisManager();
	void AddAxis(CAxis*);
	CAxis* GetAxis(int idx);

public:
	//坐标轴名称--->坐标轴对象
	map<int,CAxis*> m_mapID2Axis; 
};
