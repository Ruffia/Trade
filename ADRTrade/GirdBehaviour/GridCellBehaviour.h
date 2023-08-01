#pragma once
#include <string>
using namespace std;
#include "Field.h"
#include "GridCtrl.h"


class GridCellBehaviour
{
public:
	GridCellBehaviour(){ m_pGridCtrl = NULL;};

	void Attach(CGridCtrl* pGridCtrl) {m_pGridCtrl = pGridCtrl;}

	virtual void Behave(int row,int column, CField* pField,CString& strValue);

protected:
	virtual void _GetDisplayValue(int row,int column, CField* pField,CString& strValue);
	virtual void _AttributeBehaviour(int row,int column, CField* pField,CString& strValue);

protected:
	CGridCtrl* m_pGridCtrl;
};


// Time ×Ö¶Î
class GridCellBehaviour_Time: public GridCellBehaviour
{
public:
	virtual void _GetDisplayValue(int row,int column, CField* pField,CString& strValue);
};


// Time ×Ö¶Î
class GridCellBehaviour_AppStartTime: public GridCellBehaviour_Time
{
public:
	virtual void _GetDisplayValue(int row,int column, CField* pField,CString& strValue);
};

//CCG×Ö¶Î
class GridCellBehaviour_CCG: public GridCellBehaviour
{
public:
	virtual void _GetDisplayValue(int row,int column, CField* pField,CString& strValue);
};