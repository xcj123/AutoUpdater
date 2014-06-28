#ifndef H_EASYHTTP
#define H_EASYHTTP

#include <Socket/CSocket.h>
#include <vector>
#include <string>

using namespace std;

struct SHTTPHeader
{
	char szKey[128];
	char szValue[128];
};

class EasyHttp
{
	CAddress *m_pAddres;
	CSocket m_hSocket;
	char m_szHostName[256];
	char m_szPath[512];
	unsigned int m_iPort;
	bool m_bConnected;
	int m_iStatusCode;
	vector<SHTTPHeader> m_hHeaders;
public:
	EasyHttp(char *host, unsigned int port, char *szPath="/");
	bool Connect();
	void setHeader(char *szKey, char *szValue);
	char* getHeader(char *szKey);
	int getStatus(){ return m_iStatusCode;  }
	std::string exec();
	~EasyHttp();
};

#endif