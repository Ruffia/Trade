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
	void Initialize(map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*> mapBusiness2Control,CWnd* pParent);
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node) = 0;

protected:
	CWnd*  m_pParent;
	map<string,CWnd*>  m_mapUIName2Wnd;

	//业务字段--->UI对象
	map<string,CWnd*> m_mapBusiness2Control;
};


class CControlCreator_CEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node);
};


class CControlCreator_CBusinessEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node);
};


class CControlCreator_CBusinessComboBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node);
};


class CControlCreator_CBusinessCheckBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node);
};


class CControlCreator_CEditTreeCtrlEx : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,const xml_node &node);
};
