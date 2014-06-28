#ifndef H_CAPPSTORAGE
#define H_CAPPSTORAGE

#include <AutoUpdater/FileSync/CFileStorage.h>
#include <Utils/EasyJson.h>
#include <Utils/CTrie.h>

class CAppStorage
{
	CTrie<Json*> m_pApps;
	unsigned int m_iAppSize;
public:
	CAppStorage(char *szFileName);
	~CAppStorage();
	
	unsigned int getVersion(const char *szAppName);
	bool hasConfig(const char *szAppName){
		if (!this->getConfig(szAppName))
			return false;
		return true;
	}
	Json *getConfig(const char *szAppName);
	CTrie<Json*> *getApps() {
		return &m_pApps;
	};

	unsigned int getNumApps() {
		return m_iAppSize;
	}
	
};
#endif