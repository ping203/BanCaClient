#ifndef DEFAULT_SOCKET_H_
#define DEFAULT_SOCKET_H_
#include "CommonSocket.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#pragma comment(lib, "Ws2_32.lib")
	# include <winsock2.h>
	# include <ws2tcpip.h>
#else
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		#include <winsock2.h>
	#else
        #include <sys/types.h>
		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <arpa/inet.h>
		#include <fcntl.h> 
		#include <errno.h> 
		#include <stdio.h>
		#include <stdlib.h>
		#include <unistd.h>
        #include <netdb.h>

		#define SOCKET int
		#define SOCKET_ERROR -1
		#define INVALID_SOCKET -1
	#endif
#endif

union endpoint
{
public:
    endpoint(void)
    {
        ::memset(this, 0x0, sizeof(*this));
    }
    void assign(const sockaddr* addr)
    {
        // intri_.sa_family = af;
        switch (addr->sa_family)
        {
            case AF_INET:
                ::memcpy(&in4_, addr, sizeof(sockaddr_in));
                break;
            case AF_INET6:
                ::memcpy(&in6_, addr, sizeof(sockaddr_in6));
                break;
        }
    }
    
    int af() const
    {
        return intri_.sa_family;
    }
    unsigned short port(void) const
    {
        return ntohs(in4_.sin_port);
    }
    sockaddr intri_;
    mutable sockaddr_in in4_;
    mutable sockaddr_in6 in6_;
};
class DefaultSocket:
public CommonSocket
{
public:
    DefaultSocket();
    ~DefaultSocket();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	static bool initWinSock2_0();
#endif
public:
	int peerSocket;
	bool connectSocket(const char* serverIP, int serverPort);
	void closeSocket();
	bool hasError();
	int sendData(const char* data, int len);
	int	readData(std::vector <char> &vectorBuffer,int len);
    endpoint resolve(const char* hostname, unsigned short port);
private:
	int hClientSocket ;
};

#endif // !1
