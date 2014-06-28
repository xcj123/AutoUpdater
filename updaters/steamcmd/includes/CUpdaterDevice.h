#ifndef H_CUPDATERDEVICE
#define H_CUPDATERDEVICE

#include <Utils/EasyJson.h>
#include <UpdateSync/Interfaces/IUpdaterDevice.h>
#include <EasyHttp/EasyHttp.h>

class CUpdaterDevice : public IUpdaterDevice
{
	char m_szWorkingDirectory[512];
public:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void initApp(char *szAppName, Json *appConfig);
	virtual bool needUpdate(char *szAppName, char *szDownloadDirectory, Json *appConfig);
	int getVersion(int iAppID);
	int getCurrentVersion(char *szAppName);
	virtual bool Update(char *szAppName, char *szDownloadDirectory, Json *appConfig, bool validate);
};
#endif