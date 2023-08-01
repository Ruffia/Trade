#pragma once
#include <string>
using namespace std;
#include "FieldMeta.h"
 
class CGridFieldDesc: public CFieldDesc
{
public: 
	CGridFieldDesc();

	virtual void Initialize(int nParaCount,...);

public:
	string m_strDataSourceName;
	string m_strDisplayType;
	string m_strDisplayName;
	int m_nColumnWidth;
	int m_nColumnHeight;
	string m_strControlType;
	int m_nTextColor;
};