#ifndef WPGsnClient_h__
#define WPGsnClient_h__

#include <queue>
#include "CommonSocket.h"
#include "cocos2d.h"
#include "InPacket.h"
#include <string>
#include <mutex>
#include "OutPacket.h"



#define MAX_BUFFER 100240
typedef unsigned char BYTE;


class ClientThreadHelper;

/**
*  @brief Errors in Client
*/
enum class ErrorCode
{
	TIME_OUT,
	CONNECTION_FAILURE,
	UNKNOWN,
};

/**
*  Client state
*/
enum State
{
	kStateConnecting = 0,
	kStateOpen,
	kStateClosing,
	kStateClosed
};



class ClientListener:
	public cocos2d::Ref
{
public:
	virtual void onFinishConnect(bool isSuccess){};
	virtual void onDisconnected(){};
	virtual void onReceived(InPacket* packet){};
};

typedef std::function<void(bool isSuccess)> FinishConnectListener;
typedef std::function<void()> DisconnectListener;
typedef std::function<void(int cmdId, sPackage* pkg )> ReceiveDataListener;

class GsnClient:
	public cocos2d::Object{
public:
	GsnClient();
	~GsnClient();
	static GsnClient* create();
public:
	virtual void onSubThreadStarted();
	virtual bool onSubThreadLoop();
	virtual void onSubThreadEnded();
	virtual void onUIThreadReceiveMessage(sPackage* msg);
private:
	CommonSocket* socket;
	std::mutex   _clientMutex;
	State        _readyState;
	friend class ClientThreadHelper;
	ClientThreadHelper* _wsHelper;

	bool isSending;
	int _timeoutForConnect;
    int _timeoutForRead;
	bool _autoReconnect;
	bool _isReconnecting;
	


	std::queue<sPackage> s_requestQueue;
	std::queue<sPackage> s_responseQueue;

	ClientListener* listener;
	bool read();
	void processSending();
	std::string byte_2_str(const char* bytes, int size);
	//
	void send(char* data, int length);
	void emptyRequestQueue();
	void doCloseConnection();
	void onDisconnected();
	std::vector<char> buff;
	std::string _serverIp;
	int _serverPort;
    bool _isDisconnected;
public:
	void createSocket();
	/** Return the shared instance **/
    static GsnClient *getInstance();
    /** Release the shared instance **/
    static void destroyInstance();
	/** ket noi**/
	void connect(const char* serverIP, int serverPort);
	/** ngat ket noi**/
	void disconnect();
    /** Gui goi tin**/
	void send(OutPacket* packet);
	/** Poll function called from main thread to dispatch callbacks when http requests finished **/
	bool initThread();
	
	
	//timeout
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
    inline int getTimeoutForConnect() {return _timeoutForConnect;}
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
    inline int getTimeoutForRead() {return _timeoutForRead;};
	bool isDoConnection();
	bool reconnect();
		
	void clearQueue();

	void setListener(ClientListener* listener);
	//setListener
	void setFinishConnectListener(const FinishConnectListener& listener);
	void setDisconnectListener(const DisconnectListener& listener);
	void setReceiveDataListener(const ReceiveDataListener& listener);
    static std::vector<GsnClient*> gZPListClient;
    static void removeClient(GsnClient* client);
private: 
	FinishConnectListener _finishConnectListener;
	DisconnectListener _disconnectListener;
	ReceiveDataListener _receiveDataListener;
};

#endif // WPGsnClient_h__
