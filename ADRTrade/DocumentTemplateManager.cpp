#include "stdafx.h"
#include "DocumentTemplateManager.h"

CDocumentTemplateManager::CDocumentTemplateManager()
{
	m_mapName2Template.clear();
}

CDocumentTemplateManager& CDocumentTemplateManager::Instance()
{
	static CDocumentTemplateManager inst;
	return inst;
}


void CDocumentTemplateManager::Register(const string& sName,CMultiDocTemplate* pTemplate)
{
	m_mapName2Template[sName] = pTemplate;
}

CMultiDocTemplate* CDocumentTemplateManager::GetDocumnetTemplate(const string& sName)
{
	map<string,CMultiDocTemplate*>::iterator it = m_mapName2Template.find(sName);
	if (it != m_mapName2Template.end())
	{
		return it->second;
	}

	return NULL;
}
