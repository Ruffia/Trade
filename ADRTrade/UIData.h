#pragma once
#include <string>
using namespace std;

class CUIData
{
public:
	CUIData();

public:
	string m_sName;
    string m_strUIClassName;
	string m_strLayout;        //UI界面布局文件
	string m_strTitle;
	int m_nID;
	int  m_nLeft;
	int  m_nTop;
	int  m_nWidth;
	int  m_nHeight;
	CWnd* m_pWnd;
};