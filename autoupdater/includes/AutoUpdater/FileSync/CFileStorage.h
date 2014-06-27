#ifndef H_CFILESTORAGE
#define H_CFILESTORAGE

#include <Utils/CFileSys.h>
#include <Utils/CFile.h>
#include <Utils/Console.h>

#include <map>
#include <vector>

using namespace std;

struct SFile
{
	EFileType eFileType;
	char szFilePath[512];
	unsigned long long m_lTimeStamp;
};

class CFileStorage
{
	char *m_szRoot;
	SFile *m_hFiles;
	unsigned int m_iNumFiles;
public:
	CFileStorage();
	CFileStorage(char *szRootDirectory);
	unsigned int Explore();
	void AddFile(const char *szFileName);
	void AddDirectory(const char *szDirectoryName);
	unsigned int Sync(const char *szSyncDirectory); // Syncronizes the files and updates the older files in this directory.
	bool setRootDirectory(char *szRootDirectory);
	char *getRootDirectory(){ return m_szRoot; };
	unsigned int getFiles(SFile * files) {
		files = m_hFiles;
		return m_iNumFiles;
	};
	
	~CFileStorage();
};
#endif