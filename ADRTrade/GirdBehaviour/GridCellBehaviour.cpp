#include "StdAfx.h"
#include "Factory.h"
#include "GridCellBehaviour.h"
#include "GridCellAttributeBehaviour.h"

void GridCellBehaviour::Behave(int row,int column, CField* pField,CString& strValue)
{
	_GetDisplayValue(row,column,pField,strValue);
	_AttributeBehaviour(row,column,pField,strValue);
}


void GridCellBehaviour::_GetDisplayValue(int row,int column, CField* pField,CString& strValue)
{
	if(!pField) return;
	CFieldDesc* pFieldDesc = pField->GetFieldDesc();
	if(!pFieldDesc) return;

	const string strDataType = pFieldDesc->m_strDataType;
	const string strDisplayType = pFieldDesc->GetAttributeString("DisplayType");
	if (strDataType.find("string") != string::npos)
	{
		string sValue = pField->GetValueAsString();
		strValue.Format(strDisplayType.c_str(),sValue.c_str());
	}
	else if (strDataType.find("int") != string::npos)
	{
		strValue.Format(strDisplayType.c_str(),pField->GetValueAsInt());
	}
	else if (strDataType.find("float") != string::npos)
	{
		strValue.Format(strDisplayType.c_str(),pField->GetValueAsFloat());
	}
	else if (strDataType.find("double") != string::npos)
	{
		strValue.Format(strDisplayType.c_str(),pField->GetValueAsDouble());
	}
}



void GridCellBehaviour::_AttributeBehaviour(int row,int column, CField* pField,CString& strValue)
{
	if(!pField) return;
	CFieldDesc* pFieldDesc = pField->GetFieldDesc();
	if(!pFieldDesc) return;

	map<string,CAttribute*>& mapName2Attribute = pFieldDesc->GetAttribute();
	for (map<string,CAttribute*>::iterator it = mapName2Attribute.begin();
		it != mapName2Attribute.end();it++)
	{
		CAttribute* pAttribute = it->second;
		if(!pAttribute) continue;
		IGridCellAttributeBehaviour* pBehaviour = Factory<IGridCellAttributeBehaviour,string>::Instance().BuildProduct(pAttribute->GetAttributeID());
		if (pBehaviour)
		{
			pBehaviour->Attach(m_pGridCtrl);
			pBehaviour->Behave(row,column,pAttribute,strValue);
			delete pBehaviour;
			pBehaviour = NULL;
		}
	}
}


// Time �ֶ�
IMPLEMENT_FACTORY(GridCellBehaviour,GridCellBehaviour_Time,string,"Time")
void GridCellBehaviour_Time::_GetDisplayValue(int row,int column, CField* pField,CString& strValue)
{
	if(!pField) return;

	//�˴�����1000
	//ԭ����ʾʵ��ʱ����ʾ��2022-06-06 15:36::24 ����ʾ����
	time_t lTimeSatamp = pField->GetValueAsLongLong()/pow(10,3);

	lTimeSatamp += 28800;//GTMƫ��8��ʱ���õ�����ʱ��
	tm stime;
	gmtime_s(&stime, &lTimeSatamp);
	char szTime[128] = {0};
	strftime(szTime, 128, "%Y-%m-%d %H:%M:%S", &stime);
	strValue = szTime;
}


// AppStartTime �ֶ�
IMPLEMENT_FACTORY(GridCellBehaviour,GridCellBehaviour_AppStartTime,string,"AppStartTime")
void GridCellBehaviour_AppStartTime::_GetDisplayValue(int row,int column, CField* pField,CString& strValue)
{
	__super::_GetDisplayValue(row,column,pField,strValue);
}


//CCG�ֶ�
IMPLEMENT_FACTORY(GridCellBehaviour,GridCellBehaviour_CCG,string,"CCG")
void GridCellBehaviour_CCG::_GetDisplayValue(int row,int column, CField* pField,CString& strValue)
{
	if(!pField) return;
	CFieldDesc* pFieldDesc = pField->GetFieldDesc();
	if(!pFieldDesc) return;

	const string strDataType = pFieldDesc->m_strDataType;
	const string strDisplayType = pFieldDesc->GetAttributeString("DisplayType");
	double dValue = pField->GetValueAsDouble();
	strValue.Format(strDisplayType.c_str(),dValue);
	//if (dValue > 5E-5)
	//{
	//	m_pGridCtrl->SetItemBkColour(row, column, RGB(255,0,0));
	//}
}