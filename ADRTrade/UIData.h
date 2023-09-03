#pragma once
#include <string>
using namespace std;

class CUIData
{
public:
	CUIData();

public:
	string m_sName;            //UI ����
    string m_strUIClassName;   //UI ����
	string m_strLayout;        //UI���沼���ļ�
	string m_strTitle;         //UI Title
	int  m_nID;
	int  m_nLeft;
	int  m_nTop;
	int  m_nWidth;
	int  m_nHeight;
	string m_sCaption;         
	string m_sDataProvider;    //�����ṩ������     
	DWORD  m_dwTotalStyle;     //UI ���
	CWnd* m_pWnd;
};