#include <Utils/CFile.h>

CFile::CFile()
{
	m_hFile = NULL;
}

CFile::CFile(const char *szFileName, const char *szFlags)
{
	m_hFile = NULL;
	open(szFileName, szFlags);
}

bool CFile::open(const char *szFileName, const char *szFlags)
{
	m_szFileName = szFileName;
	if (isOpen())
	{
		m_hFile = freopen(szFileName, szFlags, m_hFile);
	}
	else{
		m_hFile = fopen(szFileName, szFlags);
	}
	if (isOpen())
	{
		m_iFileSize = checkFileSize();
		toStart();
		return true;
	}
	return false;
}

CFile::~CFile()
{
	close();
}

bool CFile::close()
{
	if (m_hFile != NULL)
	{
		int result = fclose(m_hFile);
		m_hFile = NULL;
		if (!result)
			return true;
	}
	return false;
}


bool CFile::create(const char *szFileName)
{
	FILE *hFile = fopen(szFileName, "w");
	if (!hFile)
	{
		fclose(hFile);
		return false;
	}
	fclose(hFile);
	return true;
}

bool CFile::exists(const char *szFileName)
{
	FILE *hFile = fopen(szFileName, "r");
	if (!hFile)
	{
		return false;
	}
	fclose(hFile);
	return true;
}

bool CFile::unlink(const char *szFileName)
{
	if (!remove(m_szFileName))
		return true;
	return false;
}

bool CFile::discard()
{
	close();
	if (this->unlink(m_szFileName))
	{
		return create(m_szFileName);
	}
	return open(m_szFileName);
}

void CFile::toEnd()
{
	setPosition(m_iFileSize);
}

void CFile::toStart()
{
	rewind(m_hFile);
}

unsigned int CFile::checkFileSize()
{
	long long position;
	getPosition(&position);
	fseek(m_hFile, 0, SEEK_END);
	unsigned int fileSize = ftell(m_hFile);
	setPosition(position);
	return fileSize;
}


unsigned int CFile::getFileContents(char *szFileContent)
{
	toStart();
	unsigned int result = read(szFileContent, m_iFileSize);
	szFileContent[result] = 0;
	toStart();
	return result;
}

int CFile::setPosition(long long position)
{
	return fseek(m_hFile, position, SEEK_SET);
}

int CFile::getPosition(long long *position)
{
	return fgetpos(m_hFile, (fpos_t*)position);
}

unsigned int CFile::read(char *szBuffer, unsigned int bufferSize)
{
	return fread(szBuffer, 1, bufferSize, m_hFile);
}

unsigned int CFile::getLine(char *szBuffer, unsigned int bufferSize)
{
	unsigned int bytesRead = 0;
	char ch;
	do
	{
		ch = fgetc(m_hFile);
		if (ch == '\r')
		{
			continue;
		}
		if (ch == '\n' || ch == EOF)
		{
			szBuffer[bytesRead] = 0;
			return bytesRead;
		}
		if (bytesRead >= bufferSize - 1)
		{
			szBuffer[bufferSize] = 0;
			return bytesRead;
		}
		szBuffer[bytesRead] = ch;
		++bytesRead;
	} while (ch != EOF);
	szBuffer[bytesRead] = 0;
	return bytesRead - 1;
}

unsigned int CFile::write(char *szBuffer)
{
	int result = fputs(szBuffer,  m_hFile);
	m_iFileSize = checkFileSize();
	return result;
}

unsigned int CFile::writeLine(char *szBuffer)
{
	int iSize = strlen(szBuffer);
	char *szLine = new char[iSize + 1];
	sprintf(szLine, "%s\n", szBuffer);
	unsigned int result = fputs(szLine, m_hFile);
	m_iFileSize = checkFileSize();
	return result;
}

unsigned int CFile::append(char *szBuffer)
{
	long long m_iPosition;
	getPosition(&m_iPosition);
	toEnd();
	int result = fwrite(szBuffer, sizeof(char), strlen(szBuffer), m_hFile);
	m_iFileSize = checkFileSize();
	setPosition(m_iPosition);
	return result;
}

unsigned int CFile::appendLine(char *szBuffer)
{
	//printf("LINE: %s\n", szBuffer);
	long long m_iPosition;
	getPosition(&m_iPosition);
	toEnd();
	int iSize = strlen(szBuffer);
	char *szLine = new char[iSize + 1];
	sprintf(szLine, "%s\n", szBuffer);
	unsigned int result = fwrite(szLine, sizeof(char), iSize + 1, m_hFile);
	m_iFileSize = checkFileSize();
	setPosition(m_iPosition);
	return result;
}

void CFile::AppendToFile(const char *szFileName, char *szBuffer)
{
	CFile file;
	file.open(szFileName, "a+");
	if (file.isOpen())
	{
		file.append(szBuffer);
	}
	file.close();
}

void CFile::AppendLineToFile(const char *szFileName, char *szBuffer)
{
	CFile file;
	file.open(szFileName, "a+");
	if (file.isOpen())
	{
		file.appendLine(szBuffer);
	}
	file.close();
}

long long CFile::getFileTime(const char *szFileName)
{
	struct stat status;
	stat(szFileName, &status);
	return status.st_mtime;
}