#include "StdAfx.h"
#include "AxisManager.h"

CAxisManager::CAxisManager()
{
	m_mapID2Axis.clear();
}


CAxisManager::~CAxisManager()
{

}


void CAxisManager::AddAxis(CAxis* pAxis)
{
	if(!pAxis) return;
	m_mapID2Axis[pAxis->m_nIdx] = pAxis;
}


CAxis* CAxisManager::GetAxis(int idx)
{
	map<int,CAxis*>::iterator it = m_mapID2Axis.find(idx);
	if (m_mapID2Axis.end() == it)
	{
		return NULL;
	}

	return it->second;
}