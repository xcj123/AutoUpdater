#include <AutoUpdater/FileSync/CFileStorage.h>


CFileStorage::CFileStorage()
{
	m_iNumFiles = 0;
	m_hFiles = NULL;
}

CFileStorage::CFileStorage(char *szRootDirectory)
{
	m_iNumFiles = 0;
	m_hFiles = NULL;
	this->setRootDirectory(szRootDirectory);
}

CFileStorage::~CFileStorage()
{
	delete[] m_hFiles;
}

bool CFileStorage::setRootDirectory(char *szRootDirectory)
{
	if (!CDirectory::exists(szRootDirectory))
		return false;
	m_szRoot = szRootDirectory;
}

void CFileStorage::AddFile(const char *szFileName)
{
	SFile sFile;
	sFile.eFileType = EFileType::k_File;
	sFile.m_lTimeStamp = CFile::getFileTime(szFileName);
	strcpy(sFile.szFilePath, szFileName);
	m_hFiles[m_iNumFiles] = sFile;
	++m_iNumFiles;
}

void CFileStorage::AddDirectory(const char *szDirectoryName)
{
	SFile sFile;
	strcpy(sFile.szFilePath, szDirectoryName);
	sFile.eFileType = EFileType::k_Directory;
	sFile.m_lTimeStamp = CFile::getFileTime(szDirectoryName);
	m_hFiles[m_iNumFiles] = sFile;
	++m_iNumFiles;
}

unsigned int CFileStorage::Explore()
{
	if (m_hFiles != NULL)
	{
		m_iNumFiles = 0;
		delete[] m_hFiles;
	}

	char szWorkingDirectory[1024];
	sprintf(szWorkingDirectory, "%s/content", m_szRoot);
	vector<SFileInfo> files = CDirectory::Search(szWorkingDirectory, "", -1);
	unsigned int iNumFiles = files.size();

	m_hFiles = new SFile[iNumFiles];
	for (int i = 0; i < iNumFiles; ++i)
	{
		if (files[i].fileType == EFileType::k_Directory)
		{
			this->AddDirectory(files[i].szFullPath);
		}

		if (files[i].fileType == EFileType::k_File)
		{
			this->AddFile(files[i].szFullPath);
		}
	}
	return iNumFiles;
}

unsigned int CFileStorage::Sync(const char *szTargetPath)
{
	// Create directory structure
	PRINT("Creating directory structure");
	unsigned int iszRootSize = strlen(m_szRoot)+9;
	for (unsigned int i = 0; i < m_iNumFiles; ++i)
	{
		if (m_hFiles[i].eFileType == EFileType::k_Directory)
		{
			char szDirectory[512];
			unsigned int istrLen = strlen(m_hFiles[i].szFilePath)+1;
			for (int x = 0; x < istrLen; ++x)
			{
				szDirectory[x] = m_hFiles[i].szFilePath[x + iszRootSize];
				if (szDirectory[x] == 0x00)
					break;
			}
			char szTargetDirectory[512];
			sprintf(szTargetDirectory, "%s/%s", szTargetPath, szDirectory);
			CDirectory::BuildPath(szTargetDirectory);
		}
	}

	PRINT("Updating files");
	unsigned int iUpdatedFiles = 0;
	for (unsigned int i = 0; i < m_iNumFiles; ++i)
	{
		if (m_hFiles[i].eFileType == EFileType::k_File)
		{
			char szFilePath[512];
			unsigned int istrLen = strlen(m_hFiles[i].szFilePath) + 1;
			for (int x = 0; x < istrLen; ++x)
			{
				szFilePath[x] = m_hFiles[i].szFilePath[x + iszRootSize];
				if (szFilePath[x] == 0x00)
					break;
			}
			
			char szTargetFilePath[512];
			sprintf(szTargetFilePath, "%s/%s", szTargetPath, szFilePath);
			
			if (!CFileSys::Exists(szTargetFilePath) || CFile::getFileTime(szTargetFilePath) < m_hFiles[i].m_lTimeStamp)
			{
				if (!CFileSys::Copy(m_hFiles[i].szFilePath, szTargetFilePath))
				{
					WARNING("Failed to update file: %s", szTargetFilePath);
					continue;
				}
				++iUpdatedFiles;
			}
		}
	}
	
	return iUpdatedFiles;
}