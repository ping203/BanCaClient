#include "DefaultSocket.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

DefaultSocket::DefaultSocket()
{
}

DefaultSocket::~DefaultSocket()
{
}
//khoi tao cho win32
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
bool DefaultSocket::initWinSock2_0()
{
	WSADATA wsaData ;
	WORD wVersion = MAKEWORD( 2, 0 ) ;
	if ( ! WSAStartup( wVersion, &wsaData ) )
		return true ;
	return false ;
}
#endif
//Mo ket noi
bool DefaultSocket::connectSocket(const char* serverIP, int serverPort)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32|| CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		if(!initWinSock2_0())
			return false;
	#endif
        auto epv6 = DefaultSocket::resolve(serverIP, serverPort);
		hClientSocket = socket(epv6.af(), SOCK_STREAM, IPPROTO_TCP);
	if (hClientSocket == INVALID_SOCKET)
	{
		log("Unable to init socket");
		closeSocket();
		return false ;
	}
	//bKeepAlive
	if (true)
	{
		int		optval = 1;
		if (setsockopt(hClientSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
		{
			log("Unable to setsockopt");
			closeSocket();
			return false;
		}
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32|| CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	DWORD nMode = 1;
	int nRes = ioctlsocket(hClientSocket, FIONBIO, &nMode);
	if (nRes == SOCKET_ERROR) {

		log("Unable to ioctlsocket");

		closeSocket();
		return false;
}
#else
	fcntl(hClientSocket, F_SETFL, O_NONBLOCK);
#endif	

	unsigned long serveraddr = inet_addr(serverIP);
	if (serveraddr == INADDR_NONE)	// 检查IP地址格式错误
	{
		log("Unable to serveraddr: %ld", serveraddr);
		closeSocket();
		return false;
	}

	// Connect to the server
	if ( connect( hClientSocket, &epv6.intri_, epv6.af() == AF_INET6 ? sizeof(epv6.in6_) : sizeof(epv6.in4_)) < 0)
	{
		

		if (hasError()) {
			closeSocket();
			log("Unable to connect to server ");
			return false;
		}
		else	// WSAWOLDBLOCK
		{
			timeval timeout;
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			fd_set writeset, exceptset;
			FD_ZERO(&writeset);
			FD_ZERO(&exceptset);
			FD_SET(hClientSocket, &writeset);
			FD_SET(hClientSocket, &exceptset);

			int ret = select(hClientSocket + 1, NULL, &writeset, &exceptset, &timeout);
			if (ret == 0 || ret < 0) {
				closeSocket();
				return false;
			}
			else	
			{
				ret = FD_ISSET(hClientSocket, &exceptset);
				if (ret)		
				{
					closeSocket();
					return false;
				}
			}
		}
	}

	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	setsockopt(hClientSocket, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger));

	return true;
}
endpoint DefaultSocket::resolve(const char* hostname, unsigned short port)
{
    endpoint ep;
    
    addrinfo hint;
    memset(&hint, 0x0, sizeof(hint));
    
    addrinfo* answer = nullptr;
    getaddrinfo(hostname, nullptr, &hint, &answer);
    
    if (answer == nullptr)
        return ep;
    
    memcpy(&ep, answer->ai_addr, answer->ai_addrlen);
    switch (answer->ai_family)
    {
        case AF_INET:
            ep.in4_.sin_port = htons(port);
            break;
        case AF_INET6:
            ep.in6_.sin6_port = htons(port);
            break;
        default:;
    }
    
    freeaddrinfo(answer);
    
    return ep;
}
bool DefaultSocket::hasError()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32|| CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK) {
#else
	int err = errno;
	if (err != EINPROGRESS && err != EAGAIN) {
#endif
		return true;
	}

	return false;
	
}
//Gui du lieu
int DefaultSocket::sendData(const char* data, int nLength)
{
	return send(hClientSocket, data, nLength, 0);
}

int	DefaultSocket::readData(vector <char> &vectorBuffer, int len)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	return recv(hClientSocket, &vectorBuffer[0], len, 0);
	#else
	return recv(hClientSocket, &vectorBuffer[0], len, 0);
	#endif
}

void DefaultSocket::closeSocket(){
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		shutdown(hClientSocket, SD_SEND);
		closesocket( hClientSocket ) ;
		WSACleanup() ;
	#else
		close(hClientSocket);
	#endif
}
