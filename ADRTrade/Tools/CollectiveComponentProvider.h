#pragma once

class CCollectiveComponentProvider
{
protected:
	CCollectiveComponentProvider();
	~CCollectiveComponentProvider();

public:
	static CCollectiveComponentProvider& Instance();
	CFont* GetFont(){return m_pFont;}

	void Release();

protected:
	void _CreateFont();

private:
	CFont* m_pFont;
};