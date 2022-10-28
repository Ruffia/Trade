// InputEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InputEdit.h"


// InputEdit

IMPLEMENT_DYNAMIC(InputEdit, CEdit)

InputEdit::InputEdit()
{

}

InputEdit::~InputEdit()
{
}


BEGIN_MESSAGE_MAP(InputEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// InputEdit ��Ϣ�������

void InputEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if ( (nChar >= '0' && nChar <= '9') || 
		(nChar >= 'a' && nChar <= 'f') || 
		(nChar >= 'A' && nChar <= 'F') || 
		nChar == VK_BACK || 
		nChar == VK_DELETE) //msdn��virtual key
	{ 
		nChar = (UINT)::CharUpperW(LPWSTR(nChar));                 //�޸Ĺ�����ĸ�ַ���ת��Ϊ��д��ĸ
		DefWindowProc(WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags)); //���޸Ĺ���nChar����
	} 
}
