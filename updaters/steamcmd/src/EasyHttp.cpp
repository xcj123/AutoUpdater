#include <EasyHttp/EasyHttp.h>

EasyHttp::EasyHttp(char *host, unsigned int port, char *szPath)
{
	if (!m_hSocket.initialize(SOCK_STREAM, IPPROTO_TCP, AF_INET))
	{
		printf("EasyHttp error: Failed to initlaize socket!\n");
	}


	struct hostent *nethost = gethostbyname(host);
	if (nethost != NULL)
	{
		sockaddr_in SockAddr;
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr = *(struct in_addr *)nethost->h_addr;
		SockAddr.sin_port = htons(port);
		m_pAddres = new CAddress(SockAddr);
	}
	else{
		m_pAddres = new CAddress(host, port);
	}
	strcpy(m_szHostName, host);
	strcpy(m_szPath, szPath);
	m_iPort = port;

	m_bConnected = false;
	
	//m_hSocket.sendPacket("hello", 2048);
	
}
bool EasyHttp::Connect()
{
	m_hSocket.setBlocking(false);
	m_hSocket.setTimeout(1);
	m_hSocket.setNoDelay(true);
	unsigned int tries = 0;
	while (!m_hSocket.connectAddress(m_pAddres))
	{
		if (tries > 3)
		{
			m_bConnected = false;
			return false;
		}
	}
	m_bConnected = true;
	
	return true;
}

std::string EasyHttp::exec()
{
	Connect();

	unsigned int iHeaderSize = m_hHeaders.size();
	char szRequest[4096];
	sprintf(szRequest, "GET http://%s%s HTTP/1.1\r\nHost: %s\r\n", m_szHostName, m_szPath, m_szHostName);
	for (int i = 0; i < iHeaderSize; ++i)
	{
		char szHeader[512];
		sprintf(szHeader, "%s: %s\r\n", m_hHeaders[i].szKey, m_hHeaders[i].szValue);
		strcat(szRequest, szHeader);
	}
	strcat(szRequest, "\r\n");

	m_hSocket.setBlocking(true);
	m_hSocket.setTimeout(5);
	m_hSocket.setNoDelay(false);

	m_hSocket.sendPacket(szRequest, strlen(szRequest) + 1);

	int receivedBytes = 0;
	int totalBytes = 0;
	unsigned int tries = 0;
	std::string content;
	do
	{
		
		char szContent[2049];
		receivedBytes = m_hSocket.receivePacket(szContent, 2048);
		totalBytes += receivedBytes;

		if (receivedBytes > 0)
		{
			szContent[receivedBytes] = 0;
			content.append(szContent);
		}

		if (tries > 10)
			return std::string();
		++tries;
		
	} while (receivedBytes > 0);
	char *szResponse = new char[content.length() + 1];
	int iResponseSize = content.length() + 1;
	strcpy(szResponse, content.c_str());
	

	m_hHeaders.clear();
	m_iStatusCode = -1;
		
	int parseMode = 0;
	for (int i = 0; i < iResponseSize; ++i)
	{

		if (szResponse[i] == '\r')
			continue;
		if (parseMode == 0 && m_iStatusCode == -1)
		{
			if (szResponse[i] == '\n')
			{
				parseMode = 1;
				continue;
			}
			if (szResponse[i] == ' ')
			{
				++i;
				char num[10];
				for (int x = 0; x < 10; ++x)
				{
					if (szResponse[x + i] == '\r')
						++i;
					if (szResponse[x + i] != ' ')
						num[x] = szResponse[x + i];
					else{
						i += x;
						break;
					}
				}
				m_iStatusCode = atoi(num);
				parseMode = 1;
				continue;
			}
		}

		if (parseMode == 1)
		{
			if (szResponse[i] == '\n')
			{
				++i;
				bool doContent = true;
				bool lNum = 0;
				for (int x = 0; x < 2; ++x)
				{
					if (szResponse[i + x] != '\n' && szResponse[i + x] != '\r')
					{
						doContent = false;
					}
					lNum = x;
				}

				if (doContent)
				{
					i = i + lNum;
					parseMode = 2;
					continue;
				}
				SHTTPHeader header;
				for (int x = 0; x < 128; ++x)
				{
					if (szResponse[x + i] == '\r')
						++i;
					if (szResponse[i + x] != ':')
						header.szKey[x] = szResponse[i + x];
					else{
						header.szKey[x] = 0;
						i = i + x + 2;
						break;
					}
				}
				
				for (int x = 0; x < 128; ++x)
				{
					if (szResponse[i + x] != '\n' && szResponse[i + x] != '\r')
						header.szValue[x] = szResponse[i + x];
					else{
						header.szValue[x] = 0;
						i += x;
						break;
					}
				}
				m_hHeaders.push_back(header);
				
			}
		}
		if (parseMode == 2)
		{
			char *szContent = new char[iResponseSize + 1];
			int p = 0;
			for (int x = i; x < iResponseSize; ++x)
			{
				szContent[p] = szResponse[x];
				++p;
			}

			szContent[iResponseSize] = 0;
			std::string responseContent(szContent);
			if (szContent != NULL)
				delete[] szContent;
			delete[] szResponse;
			return responseContent;
		}
	}
	delete[] szResponse;
	return string("");
}

void EasyHttp::setHeader(char *szKey, char *szValue)
{
	SHTTPHeader header;
	strcpy(header.szKey, szKey);
	strcpy(header.szValue, szValue);
	m_hHeaders.push_back(header);
}

EasyHttp::~EasyHttp()
{
	delete m_pAddres;
}
