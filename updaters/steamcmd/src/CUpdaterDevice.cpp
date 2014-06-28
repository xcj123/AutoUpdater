#include <CUpdaterDevice.h>
#include <Utils/Console.h>
#include <Utils/string.h>
#include <Utils/CFileSys.h>
#include <EasyHttp/EasyHttp.h>
#include <Utils/CFile.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <iostream>
#include <string>

#include <unistd.h>
#endif

IUpdaterDevice *getUpdaterDevice()
{
	static CUpdaterDevice device;
	return &device;
}

void CUpdaterDevice::OnLoad()
{
	GOOD("Loading SteamCMD");
	PRINT("----");
	sprintf(m_szWorkingDirectory, "%s", "updaters/steamcmd/");
	PRINT("[SteamCMD] Worknig directory set: %s", m_szWorkingDirectory);
}

void CUpdaterDevice::OnUnload()
{

}

void CUpdaterDevice::initApp(char *szAppName, Json *appConfig)
{
	char szVersionFile[256];
	sprintf(szVersionFile, "download/%s/version.txt", szAppName);
	CFileSys::Delete(szVersionFile);
}

int CUpdaterDevice::getCurrentVersion(char *szAppName)
{
	char szVersionFile[256];
	sprintf(szVersionFile, "download/%s/version.txt", szAppName);
	if (!CFileSys::Exists(szVersionFile))
		return -1;

	CFile vFile;
	vFile.open(szVersionFile, "r+");
	if (vFile.isOpen())
	{
		char szLine[32];
		vFile.getLine(szLine, 32);
		int iVersion = atoi(szLine);
		return iVersion;
		vFile.close();
	}
	vFile.close();
	return -1;
}

int CUpdaterDevice::getVersion(int iAppID)
{
	int iHttpStatus = -1;
	while (iHttpStatus <= 0)
	{
		PRINT("Requesting version info from api.steampowered.com");
		char szRequestURI[512];
		sprintf(szRequestURI, "/ISteamApps/UpToDateCheck/v0001/?appid=%d&version=0", iAppID);
		EasyHttp http("api.steampowered.com", 80, szRequestURI);
		http.setHeader("Connection", "close");
		http.setHeader("Cache-Control", "max-age=0");
		http.setHeader("Accept-Language", "en-US,en;q=0.5");
		http.setHeader("Accept-Encoding", "gzip, deflate");
		http.setHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		http.setHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:30.0) Gecko/20100101 Firefox/30.0");
		std::string content = http.exec();
		iHttpStatus = http.getStatus();
		if (iHttpStatus == 200)
		{
			Json jVersion(content.c_str());
			Json jResponse = jVersion.getObject("response");
			if (!jResponse.getBool("success"))
			{
				string errMsg = jResponse.getString("error");
				FATAL("Failed to retrieve version info: %s", errMsg.c_str());
				return -2;
			}
			return jResponse.getInt("required_version");
		}
	
		WARNING("Faild to request SteamApi HTTP Error: %d \n", iHttpStatus);
	}
	return -1;
}


bool CUpdaterDevice::needUpdate(char *szAppName, char *szDownloadDirectory, Json *appConfig)
{
	int iAppID = appConfig->getInt("api_appid");
	int iNewVersion = getVersion(iAppID);
	int iCurrentVersion = getCurrentVersion(szAppName);
	PRINT("APP: %s (ID: %d) Current: %d New: %d", szAppName, iAppID, iCurrentVersion, iNewVersion);
	if (iNewVersion > iCurrentVersion)
		return true;
	return false;
	
}

bool CUpdaterDevice::Update(char *szAppName, char *szDownloadDirectory, Json *appConfig, bool validate)
{

#ifdef _WIN32
	char szUpdaterExe[] =  "steamcmd.exe";
	
#else
	char szUpdaterExe[] = "steamcmd.sh";
#endif

	char szUpdaterCommand[512];
	
	int iAppID = appConfig->getInt("appid");
	int iAppSetConfig = appConfig->getInt("app_set_config");
	
	char szValidate[32];
	if (validate)
	{
		sprintf(szValidate, "validate");
	}
	else{
		sprintf(szValidate, "");
	}
	if (iAppSetConfig > 0)
	{
		const char *szMod = appConfig->getString("mod").c_str();
		sprintf(szUpdaterCommand, "updaters/steamcmd/%s +login anonymous +force_install_dir %s +app_set_config %d mod %s +app_update %d %s +quit", szUpdaterExe, szDownloadDirectory, iAppSetConfig, szMod, iAppID, szValidate);
	}
	else{
		sprintf(szUpdaterCommand, "updaters/steamcmd/%s +login anonymous +force_install_dir %s +app_update %d %s +quit", szUpdaterExe, szDownloadDirectory, iAppID, szValidate);
	}

#ifdef _WIN32
	
	char szWorkingDirectory[512];
	char szCurrentDirectory[512];
	GetCurrentDir(szCurrentDirectory, 512);
	
	sprintf(szWorkingDirectory, "%s/%s", szCurrentDirectory, "updaters/steamcmd/");
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	
	if (!CreateProcess(NULL, szUpdaterCommand, NULL, NULL, FALSE, 0, NULL, szWorkingDirectory, &si, &pi))
	{
		WARNING("Failed to run steamcmd");
		return false;
	}

	WaitForSingleObject( pi.hProcess, INFINITE );
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
#else
	system(szUpdaterCommand);
#endif

	int iApiAppID = appConfig->getInt("api_appid");
	char szVersionFile[256];
	sprintf(szVersionFile, "download/%s/version.txt", szAppName);
	int iVersion = getVersion(iApiAppID);
	CFile vFile;
	vFile.open(szVersionFile, "w+");
	if (!vFile.isOpen())
	{
		return false;
	}
	char szVersion[32];
	sprintf(szVersion, "%d", iVersion);
	vFile.write(szVersion);
	vFile.close();

	GOOD("UPDATING DONE!");
	return true;
}