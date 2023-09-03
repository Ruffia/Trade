#pragma once
#include <string>
using namespace std;
#include "FieldMeta.h"
#include "UIData.h"
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class IControlCreator
{
public:
	IControlCreator();
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent) = 0;

	void SetBussinessControl(map<string,CWnd*>* pmapBusiness2Control)
	{
		m_pmapBusiness2Control = pmapBusiness2Control;
	}

protected:
	//对于部分控件，有可能绑定数据库字段
	map<string,CWnd*>*  m_pmapBusiness2Control;
};


class CControlCreator_CEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CBusinessEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};

class CControlCreator_CBusinessEditNumber : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CBusinessEditConflictDetailedDescription : public CControlCreator_CBusinessEditNumber
{
};

class CControlCreator_CBusinessComboBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CBusinessComboBoxTechnicalIndex : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CBusinessComboBoxConflict : public CControlCreator_CBusinessComboBoxTechnicalIndex
{
};

class CControlCreator_CBusinessComboBoxTimePeriod : public CControlCreator_CBusinessComboBoxTechnicalIndex
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CBusinessCheckBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};


class CControlCreator_CEditTreeCtrlEx : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,CWnd* pParent);
};
