#pragma once


// InputEdit

class InputEdit : public CEdit
{
	DECLARE_DYNAMIC(InputEdit)

public:
	InputEdit();
	virtual ~InputEdit();
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	DECLARE_MESSAGE_MAP()
};


