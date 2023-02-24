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
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent) = 0;
};


class CControlCreator_CEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};


class CControlCreator_CBusinessEdit : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};

class CControlCreator_CBusinessEditNumber : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};

class CControlCreator_CBusinessComboBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};


class CControlCreator_CBusinessComboBoxTechnicalIndex : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};


class CControlCreator_CBusinessComboBoxTimePeriod : public CControlCreator_CBusinessComboBoxTechnicalIndex
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};


class CControlCreator_CBusinessCheckBox : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};


class CControlCreator_CEditTreeCtrlEx : public IControlCreator
{
public:
	virtual void Create(const CUIData &data,const string& sName, const string &sCaption, DWORD dwTotalStyle,
		const xml_node &node,map<string,CWnd*>& mapUIName2Wnd,map<string,CWnd*>& mapBusiness2Control,CWnd* pParent);
};
