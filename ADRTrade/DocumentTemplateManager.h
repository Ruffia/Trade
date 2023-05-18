#pragma once
#include <map>
using namespace std;
#include <afxwin.h>

class CDocumentTemplateManager
{
protected:
	CDocumentTemplateManager();
	~CDocumentTemplateManager(){};

public:
	static CDocumentTemplateManager& Instance();
	CMultiDocTemplate* GetDocumnetTemplate(const string& sName);
	void Register(const string& sName,CMultiDocTemplate* pTemplate);

private:
	//�Ի�������-->IDD
	map<string,CMultiDocTemplate*> m_mapName2Template;
};