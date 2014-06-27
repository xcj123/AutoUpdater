#include <CUpdaterDevice.h>
#include <Utils/Console.h>
#include <Utils/string.h>
#include <Utils/CFileSys.h>

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

bool CUpdaterDevice::needUpdate(char *szAppName, char *szDownloadDirectory, Json *appConfig)
{
	return true;
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

	GOOD("UPDATING DONE!");
	return true;
}