#include "StdAfx.h"
#include "Factory.h"
#include "GridCellAttributeBehaviour.h"
#include "GridCellNumeric.h"
#include "GridCellCheck.h"
#include "GridCellCombo.h"
#include "GridCellDateTime.h"
#include "GridURLCell.h"


// DisplayName 属性
IMPLEMENT_FACTORY(IGridCellAttributeBehaviour,GridCellAttributeBehaviour_DisplayName,string,"DisplayName")
void GridCellAttributeBehaviour_DisplayName::Behave(int row,int column, CAttribute* pAttribute,CString& strValue)
{
	const string strDisplayName = pAttribute->GetAttributeValueString();
}


// ControlType 属性
IMPLEMENT_FACTORY(IGridCellAttributeBehaviour,GridCellAttributeBehaviour_ControlType,string,"ControlType")
void GridCellAttributeBehaviour_ControlType::Behave(int row,int col, CAttribute* pAttribute,CString& strValue)
{
	const string strControlType = pAttribute->GetAttributeValueString();
	if (strControlType == "CGridCell")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCell));
	}
	else if (strControlType == "CGridCellNumeric")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellNumeric));
	}
	else if (strControlType == "CGridCellCheck")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellCheck));
	}
	else if (strControlType == "CGridCellCombo")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));
	}
	else if (strControlType == "CGridCellDateTime")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
	}
	else if (strControlType == "CGridURLCell")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridURLCell));
	}
	else if (strControlType == "CGridCellCheck")
	{
		m_pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellCheck));
	}
}


// ColumnWidth 属性
IMPLEMENT_FACTORY(IGridCellAttributeBehaviour,GridCellAttributeBehaviour_ColumnWidth,string,"ColumnWidth")
void GridCellAttributeBehaviour_ColumnWidth::Behave(int row,int column, CAttribute* pAttribute,CString& strValue)
{
	const int nColumnWidth = pAttribute->GetAttributeValueInt();
	m_pGridCtrl->SetColumnWidth(column,nColumnWidth);
}


// Show 属性
IMPLEMENT_FACTORY(IGridCellAttributeBehaviour,GridCellAttributeBehaviour_Show,string,"Show")
void GridCellAttributeBehaviour_Show::Behave(int row,int column, CAttribute* pAttribute,CString& strValue)
{

}