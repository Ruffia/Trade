#pragma once
#include <string>
using namespace std;

class CUIData
{
public:
	CUIData();

public:
	string m_sName;            //UI 名称
    string m_strUIClassName;   //UI 类名
	string m_strLayout;        //UI界面布局文件
	string m_strTitle;         //UI Title
	int  m_nID;
	int  m_nLeft;
	int  m_nTop;
	int  m_nWidth;
	int  m_nHeight;
	string m_sCaption;         
	string m_sDataProvider;    //数据提供者名称     
	DWORD  m_dwTotalStyle;     //UI 风格
	CWnd* m_pWnd;
};