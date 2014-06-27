#ifndef H_IUPDATERDEVICE
#define H_IUPDATERDEVICE

class Json;

class IUpdaterDevice
{
public:
	virtual void OnLoad() = 0;
	virtual void OnUnload() = 0;
	virtual bool needUpdate(char *szAppName, char *szDownloadDirectory, Json *appConfig) = 0;
	virtual bool Update(char *szAppName, char *szDownloadDirectory, Json *appConfig, bool validate) = 0;
};

typedef IUpdaterDevice*(*getUpdaterDeviceFn)();

#ifdef _WIN32
	#ifdef UPDATEDEVICE_EXPORT
extern "C" __declspec(dllexport) IUpdaterDevice *getUpdaterDevice();
	#else
extern "C" __declspec(dllimport) IUpdaterDevice *getUpdaterDevice();
	#endif
#else
	#ifdef UPDATEDEVICE_EXPORT
extern "C" IUpdaterDevice *getUpdaterDevice();
	#else
extern "C" IUpdaterDevice *getUpdaterDevice();
	#endif
#endif

#endif