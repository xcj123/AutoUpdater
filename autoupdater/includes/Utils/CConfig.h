#ifndef H_CCONFIG
#define H_CCONFIG

#include <iostream>
#include <fstream>
#include <string>

#include <Utils/CTrie.h>
#include <Utils/CFile.h>

using namespace std;


class CCVar
{
	string m_szValue;
	string m_szName;
public:
	CCVar(string m_szName, string sValue);
	int toInt32();
	long toInt64();
	string toString();
	const char *toChar();
	bool toBool();
	string getName(){ return m_szName; }
};


class CConfig
{
	CFile hFile;
	CTrie<CCVar*> m_hTrie;
	bool m_bGood;
public:
	CConfig();
	CConfig(const char *szFile);
	~CConfig();
	bool open(const char *szFile);
	bool good(){ return m_bGood; }
	CCVar *get(string sKey);
	static CCVar *parseLine(char *szLine);
};
#endif