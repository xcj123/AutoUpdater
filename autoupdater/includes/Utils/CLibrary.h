#ifndef H_CLIBRARY
#define H_CLIBRARY

#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#define DLL_EXT ".dll"
#else
#include <dlfcn.h>
#define DLL_EXT ".so"
#endif

class CLibrary
{
#ifdef _WIN32
	HMODULE m_hLibrary;
#else
	void * m_hLibrary;
#endif
	
public:
	CLibrary(const char * name);
	bool good();
	void * getAddress(const char * name);
	~CLibrary();
};

#endif
