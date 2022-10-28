#pragma once
#include <string>
using namespace std;
#include "Tools/pugixml.hpp"
#include "Tools/pugiconfig.hpp"
using namespace pugi;

class ILoadLayout
{
protected:
	virtual void _LoadXML(const CString& strLayoutFile);
	virtual bool _OpenXML(const CString& strLayoutFile);
	virtual void _LoadLayout() = 0;
	virtual void _CloseXML();
protected:
	xml_document m_doc;
};


class CUIData
{
public:
	CUIData();

public:
	string m_strUIClassName;
	int  m_nLeft;
	int  m_nTop;
	int  m_nWidth;
	int  m_nHeight;
};