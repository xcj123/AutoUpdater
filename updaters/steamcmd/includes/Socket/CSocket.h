#ifndef H_CSOCKET
#define H_CSOCKET

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#define SOCKET_WOULDBLOCK WSAEWOULDBLOCK
#else

#define TCP_NODELAY 1

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#define INVALID_SOCKET -1
typedef int SOCKET;
typedef timeval TIMEVAL;
#define WOULDBLOCK EWOULDBLOCK
#endif

#include <Socket/CAddress.h>

class CSocket
{
private:
	int m_iAF;
	int m_iType;
	int m_iProtocol;
	bool m_bBlocking;
	unsigned int m_iTimeout;
	SOCKET m_sSocket;
	CAddress * m_RemoteAddress;
public:
	CSocket();
	CSocket(int type, int protocol, int af=AF_INET);
	bool initialize(int type, int protocol, int af=AF_INET);
	bool isInitialized();
	bool setBlocking(bool blocking);
	bool setNoDelay(bool nodelay);
	bool bindAddress(CAddress * addr);
	bool connectAddress(CAddress * addr);
	bool startListening(int backlog);
	int sendPacket(char * buf, int buflen = -1, CAddress * dest = 0);
	int receivePacket(char * buf, int buflen, CAddress * from = 0);
	CSocket * acceptIncoming(CAddress * from=0);
	int getLastError();
	void setSocket(SOCKET sock);
	void setTimeout(unsigned int seconds);
	void setRemoteAddress(CAddress * addr) { if (m_RemoteAddress) delete m_RemoteAddress; m_RemoteAddress = addr; };
	CAddress * getRemoteAddress() { return m_RemoteAddress; };
	void destroy();
	~CSocket();
};

#endif
