#include "stdafx.h"
#include "BusinessNode.h"
#include "DBDataManager.h"

IBusinessNode::IBusinessNode():m_strBusinessName("")
{
}

IBusinessNode::~IBusinessNode()
{
}


void IBusinessNode::LoadData()
{
	m_mapFieldName2FieldDesc = CDBDataManager::Instance().GetTableMeta(m_strBusinessName);
	//int nCount = CDBDataManager::Instance().LoadData(const string& strSQL,const string& strTableName, CDataSet& ds)
}



