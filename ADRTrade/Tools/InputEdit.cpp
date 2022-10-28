// InputEdit.cpp : 实现文件
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



// InputEdit 消息处理程序

void InputEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if ( (nChar >= '0' && nChar <= '9') || 
		(nChar >= 'a' && nChar <= 'f') || 
		(nChar >= 'A' && nChar <= 'F') || 
		nChar == VK_BACK || 
		nChar == VK_DELETE) //msdn的virtual key
	{ 
		nChar = (UINT)::CharUpperW(LPWSTR(nChar));                 //修改过的字母字符，转换为大写字母
		DefWindowProc(WM_CHAR, nChar, MAKELPARAM(nRepCnt, nFlags)); //用修改过的nChar调用
	} 
}
