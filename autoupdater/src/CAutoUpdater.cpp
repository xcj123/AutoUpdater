#include <AutoUpdater/CAutoUpdater.h>


CAutoUpdater::CAutoUpdater()
{
	PRINT("Console intialized");
	PRINT("Executing 'cfg/updater.cfg'");
	m_hConfig = new CConfig();
	
	if (!m_hConfig->open("cfg/updater.cfg"))
	{
		WARNING("Failed to load cfg/updater.cfg");
		exit(1);
	}

	PRINT("-----");
	PRINT("Initializing Updater Devices:");
	
	vector<SFileInfo> files = CDirectory::List("updaters");
	int iNumFiles = files.size();
	for (int i = 0; i < iNumFiles; ++i)
	{
		if (files[i].fileType == EFileType::k_File)
		{
			string fileName = files[i].szFileName;
			string libName = strtolower(fileName.substr(0, fileName.find_last_of(".")));

			CLibrary *lib = new CLibrary(files[i].szFullPath);
			if (!lib)
			{
				WARNING("Failed to load: '%s' (Not valid library)", files[i].szFileName);
				delete lib;
				continue;
			}

			getUpdaterDeviceFn getUpdaterDevice = (getUpdaterDeviceFn)lib->getAddress("getUpdaterDevice");
			if (!getUpdaterDevice)
			{
				WARNING("Failed to get IUpdateDevice interface from: '%s' ", files[i].szFileName);
				delete lib;
				continue;
			}
			IUpdaterDevice *pUpdaterDevice = getUpdaterDevice();

			pUpdaterDevice->OnLoad();
			
			GOOD("%s loaded successfully as [%s].", files[i].szFileName, libName.c_str());

			m_hUpdaterDevices.insertElement(libName.c_str(), pUpdaterDevice);
			
		}
	}

	PRINT("Executing apps.cfg");
	m_pAppStorage = new CAppStorage("cfg/apps.cfg");
	
	
	PRINT("-----");
	PRINT("Exploring downloaded files");
	
	vector<SFileInfo> apps = CDirectory::List("download");
	int iNumApps = apps.size();
	PRINT("Found %d applications", iNumApps-2);
	PRINT("-----");
	for (int i = 0; i < iNumApps; ++i)
	{
		if (apps[i].fileType == EFileType::k_Directory && apps[i].szFileName[0] != '.')
		{
			if (!m_pAppStorage->hasConfig(apps[i].szFileName))
			{
				WARNING("Application %s has no config file", apps[i].szFileName);
				continue;
			}
		}
	}
}

void CAutoUpdater::Start()
{
	while (true)
	{
		PRINT("Looking for updates...");
		CTrie<Json*> *pApps = m_pAppStorage->getApps();
		struct CTrie<Json*>::Node *pApp = 0;
		while (pApp = pApps->iterateTrie(pApp))
		{
			Json *pAppConfig = pApp->value;
			std::string szDeviceName = pAppConfig->getString("device");
			char szAppName[128];
			pApps->getNodeKey(pApp, szAppName, 128);

			struct CTrie<IUpdaterDevice*>::Node *m_pDevice = m_hUpdaterDevices.findNode(szDeviceName.c_str());
			if (m_pDevice != NULL)
			{
				IUpdaterDevice *pIDevice = m_pDevice->value;

				char szCurrentDirectory[512];
				if (!GetCurrentDir(szCurrentDirectory, 512))
				{
					WARNING("Failed to retrieve current upadte directory");
					continue;
				}

				char szDownloadDirectory[512];
				sprintf(szDownloadDirectory, "%s/download/%s/content", szCurrentDirectory, szAppName);

				if (pIDevice->needUpdate(szAppName, szDownloadDirectory, pAppConfig))
				{
					PRINT("Updating app: %s", szAppName);
					if (!pIDevice->Update(szAppName, szDownloadDirectory, pAppConfig, false))
					{
						WARNING("Failed to update application: %s", szAppName);
						// TODO SEND ALERT NOTIFICATION TO THE OPERATOR SOMEHOW? 
						continue;
					}
				}
				char szStorageDirectory[512];
				sprintf(szStorageDirectory, "download/%s", szAppName);
				CFileStorage storage(szStorageDirectory);
				unsigned int exploredFiles = storage.Explore();
				GOOD("Found %d files", exploredFiles);
				CFile file;
				std::string szSyncFile = pAppConfig->getString("synclist");
				file.open(szSyncFile.c_str());
				if (!file.isOpen())
				{
					WARNING("Failed to '%s' sync file", szSyncFile.c_str());
					continue;
				}
				char szLine[512];
				while (file.getLine(szLine, 512) > 0)
				{

					PRINT("Syncronizing: %s", szLine);
					unsigned int updatedFiles = storage.Sync(szLine);


					if (updatedFiles > 0)
					{
						GOOD("%d files were updated", updatedFiles);
					}
					else{
						GOOD("%s is already up to date", szLine);
					}
				}
				file.close();
				
			}
		}
		
#ifdef _WIN32
		Sleep(6000);
#else
		sleep(60);
#endif
	}
}
