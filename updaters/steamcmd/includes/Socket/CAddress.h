#ifndef H_CADDRESS
#define H_CADDRESS

#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <netdb.h>
#endif

class CAddress
{
private:
	int m_iAF;
	struct sockaddr_in m_sAddr;
public:
	CAddress(sockaddr_in addr);
	CAddress(const char * ip = 0, unsigned int port = 0, int type = AF_INET);
	bool setAddress(sockaddr_in addr);
	bool setAddress(const char * ip = 0, unsigned int port = 0, int type = AF_INET);
	sockaddr_in * getSockAddr();
	bool getIP(char * out, unsigned int maxlen);
	unsigned int getPort();
};

#endif
