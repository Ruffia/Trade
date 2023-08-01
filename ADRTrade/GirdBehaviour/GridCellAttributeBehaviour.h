#pragma once
#include <string>
using namespace std;
#include "Attribute.h"
#include "GridCtrl.h"

//每个属性决定一个GridCell的行为
class IGridCellAttributeBehaviour
{
public:
	IGridCellAttributeBehaviour(){ m_pGridCtrl = NULL;};

	void Attach(CGridCtrl* pGridCtrl) {m_pGridCtrl = pGridCtrl;}

	virtual void Behave(int row,int column, CAttribute* pAttribute,CString& strValue) = 0;

protected:
	CGridCtrl* m_pGridCtrl;
};


// DisplayName
class GridCellAttributeBehaviour_DisplayName: public IGridCellAttributeBehaviour
{
public:
	virtual void Behave(int row,int column, CAttribute* pAttribute,CString& strValue);
};


// ControlType
class GridCellAttributeBehaviour_ControlType: public IGridCellAttributeBehaviour
{
public:
	virtual void Behave(int row,int column, CAttribute* pAttribute,CString& strValue);
};


// ColumnWidth
class GridCellAttributeBehaviour_ColumnWidth: public IGridCellAttributeBehaviour
{
public:
	virtual void Behave(int row,int column, CAttribute* pAttribute,CString& strValue);
};


// Show
class GridCellAttributeBehaviour_Show: public IGridCellAttributeBehaviour
{
public:
	virtual void Behave(int row,int column, CAttribute* pAttribute,CString& strValue);
};