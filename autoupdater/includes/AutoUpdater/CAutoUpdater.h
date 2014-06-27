#ifndef H_CAUTOUPDATER
#define H_CAUTOUPDATER

#include <Utils/Console.h>
#include <Utils/CLibrary.h>
#include <Utils/CConfig.h>
#include <Utils/CFileSys.h>
#include <Utils/string.h>
#include <Utils/CTrie.h>

#include <AutoUpdater/FileSync/CFileStorage.h>
#include <AutoUpdater/UpdateSync/Interfaces/IUpdaterDevice.h>
#include <AutoUpdater/UpdateSync/CAppStorage.h>

class CAutoUpdater
{
	CTrie<IUpdaterDevice*> m_hUpdaterDevices;
	CAppStorage *m_pAppStorage;
	CConfig *m_hConfig;

public:
	CAutoUpdater();
	void Start();
	static void UpdaterThread(void *param);
};

#endif