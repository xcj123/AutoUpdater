#include <Utils/CFileSys.h>
#include <Utils/Console.h>
#include <Utils/string.h>

#ifdef _WIN32

#include <Windows.h>

#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <cstdlib>
using namespace std;
#endif

bool CFileSys::Copy(const char *src, const char *dst)
{

#ifdef _WIN32
	return CopyFile(src, dst, false);
	//return CopyFileA(src, dst, FALSE);
#else
	char szCommand[1024];
	sprintf(szCommand, "cp -rf %s %s", src, dst);
	system(szCommand);
	return true;
#endif
}

bool CFileSys::Delete(const char *target)
{
#ifdef WIN32
	return DeleteFileA(target);
#else
	int result = remove(target);
	printf("%d", result);
	return false;
#endif
}

bool CFileSys::Exists(const char *szFile)
{
#ifdef _WIN32
	GetFileAttributes(szFile);
	if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(szFile) && GetLastError()==ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	return true;
#else
	FILE *hFile = fopen(szFile, "r+");
	if(!hFile)
	{
		return false;
	}
	fclose(hFile);
	return true;	
#endif
}

bool CFileSys::Create(const char *target)
{
#ifdef _WIN32
	HANDLE hFile = CreateFileA(target, GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
#else
	ofstream hFile;
	hFile.open(target);
	if (hFile.good())
	{
		hFile.close();
		return true;
	}
	hFile.close();
	return false;
#endif
}

const char* CFileSys::extension(const char *target)
{
	std::string fn = target;
	return fn.substr(fn.find_last_of(".") + 1).c_str();
}

bool CDirectory::Create(const char *targetPath)
{
#ifdef _WIN32
	
	SECURITY_ATTRIBUTES attr;
	if (!CreateDirectoryA(targetPath, NULL))
		return false;
	
#else
	mkdir(targetPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	return true;
}

bool CDirectory::Delete(const char *targetPath, bool recursive)
{
#ifdef _WIN32
	if (recursive)
	{
		vector<SFileInfo> files = CDirectory::List(targetPath);
		int fSize = files.size();
		for (int i = 0; i < fSize; ++i)
		{
			if ((files[i].szFileName[0] == '.' && strlen(files[i].szFileName) == 1) || (files[i].szFileName[0] == '.' && files[i].szFileName[1] == '.' && strlen(files[i].szFileName) == 2))
				continue;

			if (files[i].fileType == EFileType::k_File)
			{
				CFileSys::Delete(files[i].szFullPath);
			}
		}

		for (int i = 0; i < fSize; ++i)
		{
			if ((files[i].szFileName[0] == '.' && strlen(files[i].szFileName) == 1) || (files[i].szFileName[0] == '.' && files[i].szFileName[1] == '.' && strlen(files[i].szFileName) == 2))
				continue;

			if (files[i].fileType == EFileType::k_Directory)
			{
				CDirectory::Delete(files[i].szFullPath, true);
			}
		}


	}
	return RemoveDirectoryA(targetPath);
#else
	rmdir(targetPath);
#endif
}

vector<SFileInfo> CDirectory::List(const char *targetPath, const char *szSearch)
{
	vector<SFileInfo> files;
#ifdef _WIN32
	HANDLE hDir = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA fd;
	char chPath[260];
	sprintf(chPath, "%s\\%s", targetPath, szSearch);
	hDir = FindFirstFileA(chPath, &fd);

	do
	{
		struct SFileInfo eFile;
		sprintf(eFile.szFullPath, "%s/%s", targetPath, fd.cFileName);
		sprintf(eFile.szFileName, "%s", fd.cFileName);
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			eFile.fileType = EFileType::k_Directory;
		else
			eFile.fileType = EFileType::k_File;
		files.push_back(eFile);
	} while (FindNextFileA(hDir, &fd) != 0);
	FindClose(hDir);
#else
	DIR *dp = opendir(targetPath);
	if (dp != NULL)
	{
		struct dirent *dirp;
		while ((dirp = readdir(dp)) != NULL)
		{
			struct SFileInfo file;
			sprintf(file.szFullPath, "%s/%s", targetPath, dirp->d_name);
			strcpy(file.szFileName, dirp->d_name);
			
			if(dirp->d_type == (unsigned char)0x8)
			{
				file.fileType = EFileType::k_File;
			}
			if(dirp->d_type == (unsigned char)0x4)
			{
				file.fileType = EFileType::k_Directory;
			}
			files.push_back(file);
		}
		closedir(dp);
	}
	
#endif
	return files;
}

vector<SFileInfo> CDirectory::Search(const char *path, const char *search, int recursiveLevel)
{
	vector<SFileInfo> foundFiles;
	vector<SFileInfo> files = CDirectory::List(path);
	int fSize = files.size();
	for (int i = 0; i < fSize; ++i)
	{
		string context = files[i].szFileName;
		if (context.find(search) != string::npos && files[i].szFileName[0] != '.' && files[i].szFileName[1] != '.')
		{
			foundFiles.push_back(files[i]);
		}

		if (files[i].fileType == EFileType::k_Directory && (recursiveLevel > 0 || recursiveLevel == -1) && files[i].szFileName[0] != '.' && files[i].szFileName[1] != '.')
		{
			vector<SFileInfo> subFiles = CDirectory::Search(files[i].szFullPath, search, (recursiveLevel == -1?-1:recursiveLevel - 1));
			foundFiles.insert(foundFiles.end(), subFiles.begin(), subFiles.end());
		}
	}
	return foundFiles;
}

bool CDirectory::exists(const char *szPath)
{

#ifdef _WIN32
	DWORD ftyp = GetFileAttributesA(szPath);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

#else
	DIR *dp = opendir(szPath);
	if (dp != NULL)
	{
		closedir(dp);
		return true;
	}
#endif
	return false;
}

bool CDirectory::BuildPath(const char *szBuildPath)
{
	vector<string> path = str_explode('/', szBuildPath);
	int iSize = path.size();
	char szPath[512];
	sprintf(szPath, "%s", path[0].c_str());
	CDirectory::Create(szPath);
	bool noFail = true;
	for (int i = 1; i < iSize; ++i)
	{
		sprintf(szPath, "%s/%s", szPath, path[i].c_str());
		if (!CDirectory::Create(szPath))
		{
			noFail = false;
		}
	}
	return noFail;
}