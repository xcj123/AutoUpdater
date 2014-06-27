#ifndef H_CFILE
#define H_CFILE


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

class CFile
{
	FILE *m_hFile;
	unsigned int m_iFileSize;
	long long pos;
	const char *m_szFileName;
public:
	CFile();
	CFile(const char *szFileName, const char *szFlags);
	~CFile();
	bool close();
	bool open(const char *szFileName, const char *szFlags="r+b");
	bool create(const char *szFileName);
	bool unlink(const char *szFileName);
	bool discard();
	bool exists(const char *szFileName);
	unsigned int getFileContents(char *szFileContent);
	unsigned int read(char *szBuffer, unsigned int bufferSize);
	unsigned int getLine(char *szBuffer, unsigned int bufferSize);
	unsigned int write(char *szBuffer);
	unsigned int writeLine(char *szBuffer);
	unsigned int append(char *szBuffer);
	unsigned int appendLine(char *szBuffer);
	void toStart();
	void toEnd();
	void seek();
	int getPosition(long long *position);
	int setPosition(long long position);
	FILE *getFileHandle(){ return m_hFile; };
	unsigned int getFileSize() { return m_iFileSize; };
	unsigned int checkFileSize();
	bool isOpen() { return (!m_hFile ? false : true); };
	static long long  getFileTime(const char *szFileName);
	static void AppendToFile(const char *szFileName, char *szData);
	static void AppendLineToFile(const char *szFileName, char *szData);
};
#endif