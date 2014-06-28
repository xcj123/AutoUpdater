#include <AutoUpdater/UpdateSync/CAppStorage.h>
#include <Utils/CFile.h>
#include <Utils/Console.h>

CAppStorage::CAppStorage(char *szFileName)
{

	CFile configFile;
	if (!configFile.open(szFileName))
	{
		WARNING("Failed to read apps from '%s'", szFileName);
		return;
	}

	unsigned int miFileSize =  configFile.getFileSize();
	char szContent[8128];
	configFile.getFileContents(szContent);
	Json pConfig(szContent);

	typedef std::map<std::string, JsonValue>::iterator it_type;

	m_iAppSize = pConfig.getSize();
	for (it_type iterator = pConfig.jsonKeyValues.begin(); iterator != pConfig.jsonKeyValues.end(); iterator++)
	{
		const char *szAppName = iterator->first.c_str();
		m_pApps.insertElement(szAppName, new Json(pConfig.getObject(szAppName)));
	}
	
}

Json *CAppStorage::getConfig(const char *szAppName)
{
	CTrie<Json*>::Node *m_pNode = m_pApps.findNode(szAppName);
	if (!m_pNode)
		return NULL;
	return m_pNode->value;
}

CAppStorage::~CAppStorage()
{

}