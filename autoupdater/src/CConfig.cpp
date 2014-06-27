#include <Utils/CConfig.h>
#include <Utils/Console.h>
#include <Utils/string.h>


CConfig::CConfig()
{
	m_bGood = false;
}

CConfig::CConfig(const char *szFile)
{
	m_bGood = false;
	m_bGood = open(szFile);
}

CConfig::~CConfig()
{
	struct CTrie<CCVar*>::Node *m_Node = m_hTrie.iterateTrie(0);
	do
	{
		if (m_Node != NULL)
		{
			delete m_Node->value;
		}
		m_Node = m_hTrie.iterateTrie(m_Node);
	} while (m_Node != NULL);
}

bool CConfig::open(const char *szFile)
{
	string sBuffer;
	hFile.open(szFile);
	
	if(!hFile.isOpen())
	{
		WARNING("Failed to open '%s'\n", szFile);
		m_bGood = false;
		return false;
	}
	
	char szBuffer[1024];
	unsigned int read = 0;
	hFile.getFileContents(szBuffer);
	unsigned int m_iLine = 0;
	do
	{
		read = hFile.getLine(szBuffer, 1024);
		
		if (read > 0)
		{
			CCVar *cvar = CConfig::parseLine(szBuffer);
			if (!cvar)
			{
				WARNING("Line %d is corrupted", m_iLine);
				continue;
			}
			++m_iLine;
			m_hTrie.insertElement(cvar->getName().c_str(), cvar);
		}
		
	} while (read > 0);
	hFile.close();
	m_bGood = true;
	return true;
}

CCVar *CConfig::parseLine(char *szLine)
{
	vector<string> splits = str_explode('=', (char*)szLine, 2);
	if(splits.size()<2)
	{
		return NULL;
	}
	return new CCVar(splits[0], splits[1]);
}

CCVar *CConfig::get(string sKey)
{
	struct CTrie<CCVar*>::Node *m_Node = m_hTrie.findNode(sKey.c_str());
	if (!m_Node)
	{
		DEBUG("Key '%s' not found.", sKey.c_str());
		return NULL;
	}
	return m_Node->value;
}

CCVar::CCVar(string szName, string szValue)
{
	m_szName = szName;
	m_szValue = szValue;
}

int CCVar::toInt32()
{
	return atoi(m_szValue.c_str());
}

long CCVar::toInt64()
{
	return atol(m_szValue.c_str());
}

string CCVar::toString()
{
	return m_szValue;
}

const char * CCVar::toChar()
{
	return m_szValue.c_str();
}

bool CCVar::toBool()
{
	return (atoi(m_szValue.c_str())>0);
}