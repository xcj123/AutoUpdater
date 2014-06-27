#include <Utils/CLibrary.h>
#include <stdio.h>
CLibrary::CLibrary(const char * name)
{
	const char *dot = strrchr(name, '.');
	if (!dot || strcmp(dot, DLL_EXT))
	{
		m_hLibrary = NULL;
		return;
	}
#ifdef _WIN32
	m_hLibrary = LoadLibraryA(name);
#else
	m_hLibrary = dlopen(name, RTLD_LAZY);
#endif
}

void * CLibrary::getAddress(const char * name)
{
#ifdef _WIN32
	return GetProcAddress(m_hLibrary, name);
#else
	return dlsym(m_hLibrary, name);
#endif
}

bool CLibrary::good()
{
	if (!m_hLibrary)
		return false;
	return true;
}

CLibrary::~CLibrary()
{
#ifdef _WIN32
	FreeLibrary(m_hLibrary);
#else
	if(m_hLibrary != NULL)
		dlclose(m_hLibrary);
#endif
}
