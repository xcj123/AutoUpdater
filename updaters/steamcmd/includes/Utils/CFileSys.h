#ifndef H_CFILESYS
#define H_CFILESYS

#include <vector>
#include <stdio.h>
#include <string.h>

#define LoopFiles(a, b) for (unsigned int a = 0; a < b.size(); ++a)
using namespace std;
enum EFileType
{
	k_Unkown = 0,
	k_File = 1,
	k_Directory = 2
};

struct SFileInfo
{
	EFileType fileType;
	char szFileName[512];
	char szFullPath[512];
};

class CFileSys
{
public:
	static bool Copy(const char *src, const char *dest);
	static bool Delete(const char *target);
	static bool Create(const char *target);
	static bool Exists(const char *szFileName);
	static const char *extension(const char *szFile);
};

class CDirectory
{
public:
	static bool Create(const char *targetPath);
	static bool Delete(const char *targetPath, bool recusive=false);
	static vector<SFileInfo> List(const char *targetPath, const char *szSearch = "*");
	static vector<SFileInfo> Search(const char *path, const char *search, int recursiveLevel = 0);
	static bool exists(const char *szPath);
	static bool BuildPath(const char *szPath);
};

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


#endif