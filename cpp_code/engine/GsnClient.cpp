#include "GsnClient.h"
#include "PacketHeaderAnalyze.h"
#include "DefaultSocket.h"
#include <thread>
#include <queue>
#include <signal.h>
#include <errno.h>
using namespace std;
USING_NS_CC;
/**
*  @brief Websocket thread helper, it's used for sending message between UI thread and websocket thread.
*/
class ClientThreadHelper : public cocos2d::Object
{
public:
	ClientThreadHelper();
	~ClientThreadHelper();

	// Creates a new thread
	bool createThread(GsnClient* gsnClient);
	// Quits sub-thread (websocket thread).
	void quitSubThread();

	// Schedule callback function
	virtual void update(float dt);

	// Sends message to UI thread. It's needed to be invoked in sub-thread.
	void sendMessageToUIThread(sPackage *msg);

	// Sends message to sub-thread(websocket thread). It's needs to be invoked in UI thread.
	void sendMessageToSubThread(sPackage *msg);

	// Waits the sub-thread (websocket thread) to exit,
	void joinSubThread();


protected:
	void wsThreadEntryFunc();

private:
	std::queue<sPackage*>* _UIWsMessageQueue;
	std::queue<sPackage*>* _subThreadWsMessageQueue;
	std::mutex   _UIWsMessageQueueMutex;
	std::mutex   _subThreadWsMessageQueueMutex;
	std::thread* _subThreadInstance;
	GsnClient* _gsnClient;
	bool _needQuit;
	friend class GsnClient;
};


// Implementation of WsThreadHelper
ClientThreadHelper::ClientThreadHelper()
: _subThreadInstance(nullptr)
, _gsnClient(nullptr)
, _needQuit(false)
{
	_UIWsMessageQueue = new std::queue<sPackage*>();
	_subThreadWsMessageQueue = new std::queue<sPackage*>();

	CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
}

ClientThreadHelper::~ClientThreadHelper()
{
	cocos2d::log("~ClientThreadHelper");
	CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
	joinSubThread();
	if (_subThreadInstance)
		CC_SAFE_DELETE(_subThreadInstance);
	_UIWsMessageQueue->empty();
	_subThreadWsMessageQueue->empty();
	delete _UIWsMessageQueue;
	delete _subThreadWsMessageQueue;
}

bool ClientThreadHelper::createThread(GsnClient* gsnClient)
{
	_gsnClient = gsnClient;
	// Creates websocket thread
	_subThreadInstance = new std::thread(std::bind(&ClientThreadHelper::wsThreadEntryFunc, this));

	return true;
}

void ClientThreadHelper::quitSubThread()
{
	_needQuit = true;
}

void ClientThreadHelper::wsThreadEntryFunc()
{
	_gsnClient->onSubThreadStarted();

	while (!_needQuit)
	{
		if (!_gsnClient->onSubThreadLoop())
		{
			break;
		}
	}

}

void ClientThreadHelper::sendMessageToUIThread(sPackage *msg)
{
	std::lock_guard<std::mutex> lock(_UIWsMessageQueueMutex);
	_UIWsMessageQueue->push(msg);
}

void ClientThreadHelper::sendMessageToSubThread(sPackage *msg)
{
	std::lock_guard<std::mutex> lock(_subThreadWsMessageQueueMutex);
	_subThreadWsMessageQueue->push(msg);
}

void ClientThreadHelper::joinSubThread()
{
	if (_subThreadInstance->joinable())
	{
		_subThreadInstance->join();
	}
}

void ClientThreadHelper::update(float dt)
{

	sPackage *msg = nullptr;
	{
		// Returns quickly if no message
		std::lock_guard<std::mutex> lock(_UIWsMessageQueueMutex);

		if (0 == _UIWsMessageQueue->size())
		{
			return;
		}
		// Gets message
		msg = _UIWsMessageQueue->front();
		_UIWsMessageQueue->pop();
	}
	
	if ((!msg) || (msg->_size < 0))
		return;
	if (_gsnClient)
	{
		_gsnClient->onUIThreadReceiveMessage(msg);
	}

	CC_SAFE_DELETE(msg);
}

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------CLIENT-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/


static GsnClient *pGsnClient = NULL;
std::vector<GsnClient*> GsnClient::gZPListClient;

GsnClient::GsnClient(){
	listener = NULL;
	_readyState = State::kStateClosed;
	socket = NULL;
	createSocket();
	isSending = false;
	_autoReconnect = false;
	_isDisconnected = false;
	_serverPort = 0;
    gZPListClient.push_back(this);
}
GsnClient::~GsnClient(){
	doCloseConnection();
	if (socket)
	{
		delete socket;
		socket = NULL;
	}
    removeClient(this);
}
GsnClient* GsnClient::create()
{
	return new GsnClient();
}
void GsnClient::createSocket()
{
	if (socket)
		delete socket;
	//nen tang nao thi dung socket cho nen tang do
	socket = new DefaultSocket();
}
/** Return the shared instance **/
GsnClient *GsnClient::getInstance()
{
	if (pGsnClient == NULL) {
		pGsnClient = new GsnClient();
	}
	return pGsnClient;
}
/** Release the shared instance **/
void GsnClient::destroyInstance()
{
	if (pGsnClient)
	{
		delete  pGsnClient;
	}
	pGsnClient = NULL;
}
/** ket noi**/
void GsnClient::connect(const char* serverIP, int serverPort)
{
	_serverIp = serverIP;
	_serverPort = serverPort;
	_wsHelper = new ClientThreadHelper();
	_wsHelper->createThread(this);
	_readyState = State::kStateConnecting;
}
/** ngat ket noi**/
void GsnClient::disconnect()
{
	if (_readyState == kStateClosed)
		return;
	_wsHelper->quitSubThread();
	sPackage* pk = new sPackage();
	pk->packageType = PackageType::kTypeConnectionClosed;

	_wsHelper->sendMessageToUIThread(pk);
}
/** Gui goi tin**/
void GsnClient::send(OutPacket* packet){

	int dataLength = 0;
	char* data = packet->getData(dataLength);
	if (dataLength == 0)
	{
		CCAssert(false, "Du lieu trong");
	}
	send(data, dataLength);
}
//Gui yeu cau
void GsnClient::send(char* data, int length){
	sPackage* pk = new sPackage();
	pk->_data = data;
	pk->_size = length;

	_wsHelper->sendMessageToSubThread(pk);
	if (!isSending)
	{
		processSending();
	}
	else
	{
		//printf("push to queue\n");
	}

}
//thuc hien gui tin
void GsnClient::processSending()
{
	std::lock_guard<std::mutex> lock(_wsHelper->_subThreadWsMessageQueueMutex);
	int nCntSend;
	isSending = true;

	int bytesWrite = 0;
	while (!_wsHelper->_subThreadWsMessageQueue->empty())
	{
		sPackage* subThreadMsg = _wsHelper->_subThreadWsMessageQueue->front();
		bytesWrite = socket->sendData(subThreadMsg->_data, subThreadMsg->_size);

		// Buffer overrun?
		if (bytesWrite < 0)
		{
			break;
		}
		// Safely done!
		else
		{
			//log("<< SENT:%d: [%s]", subThreadMsg->_size, byte_2_str(subThreadMsg->_data, subThreadMsg->_size).c_str());
			_wsHelper->_subThreadWsMessageQueue->pop();
			CC_SAFE_DELETE(subThreadMsg);
		}
	}

	isSending = false;
}
void GsnClient::onSubThreadStarted()
{
	std::lock_guard<std::mutex> lk(_clientMutex);
	bool result = socket->connectSocket(_serverIp.c_str(), _serverPort);
	if (result)
	{
		_isReconnecting = false;
		_readyState = State::kStateOpen;
	}
	sPackage* pk = new sPackage();
	pk->packageType = PackageType::kTypeConnectionOpened;
	pk->result = result;

	_wsHelper->sendMessageToUIThread(pk);
}
bool GsnClient::onSubThreadLoop()
{
	if (_readyState != State::kStateConnecting)
	{
		std::lock_guard<std::mutex> lk(_clientMutex);
		bool result = read();

		//error to reading from socket
		if (!result)
		{
			disconnect();
			return false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return true;
}

void GsnClient::onSubThreadEnded()
{

}
void GsnClient::onUIThreadReceiveMessage(sPackage* msg)
{
	switch (msg->packageType)
	{
		//ket noi thanh cong
	case PackageType::kTypeConnectionOpened:

		if (listener)
		{
			listener->onFinishConnect(msg->result);
		}

		if (this->_finishConnectListener)
			this->_finishConnectListener(msg->result);
		break;
	case PackageType::kTypeConnectionClosed:
		doCloseConnection();
		if (listener)
		{
			listener->onDisconnected();
		}
		if (this->_disconnectListener)
		{
			this->_disconnectListener();
		}	
		break;
	case PackageType::kTypeData:
	{
		char* data = msg->_data;
		int size = msg->_size;
		int cmd = PacketHeaderAnalyze::getCmdIdFromData(data);
		if (this->_receiveDataListener) 
			this->_receiveDataListener(cmd, msg);
	}
		break;
	default:
		break;
	}
}

//doc du lieu
bool GsnClient::read(){
	if (_readyState == State::kStateOpen)
	{
		vector <char> szBuffer(5000);
		int nLength = 0;
		nLength = socket->readData(szBuffer, szBuffer.size());
		if (nLength > 0)
		{
			//cocos2d::log("Receive %d", nLength);
			//log("<< Receive:%d: [%s]", nLength, byte_2_str(&szBuffer[0], nLength).c_str());
			szBuffer.resize(nLength);
			// xu ly slow connection
			buff.insert(buff.end(), szBuffer.begin(), szBuffer.end());
			while (true){

				int validSize = PacketHeaderAnalyze::getValidSize(buff.data(), buff.size());
				if (validSize < 0 || validSize > buff.size())
				{
					//log("validSize: %d", validSize);
					break;
				}
				sPackage* pk = new sPackage();
				pk->process(buff,validSize);
				buff.erase(buff.begin(), buff.begin() + validSize);

				_wsHelper->sendMessageToUIThread(pk);
			}
		}
		else{

			if (socket->hasError()) {
				return false;
			}
			else {	
				return true;
			}
			
		}
	}
	// return true to continue the loop.
	return true;
}
bool GsnClient::reconnect()
{
	if (_serverPort == 0)
	{
		return false;
	}
	_isReconnecting = true;
	doCloseConnection();
	connect(_serverIp.c_str(), _serverPort);

	return true;
}
bool GsnClient::isDoConnection()
{
	return _serverPort != 0;
}
void GsnClient::setListener(ClientListener* listener)
{
	this->listener = listener;
}

void GsnClient::clearQueue()
{

}
bool GsnClient::initThread()
{
	return true;
}
void GsnClient::doCloseConnection()
{

	if (_readyState == State::kStateClosing || _readyState == State::kStateClosed)
	{
		return;
	}
	_readyState = State::kStateClosed;
	socket->closeSocket();
	if (_wsHelper)
	{
		_wsHelper->quitSubThread();
		_wsHelper->joinSubThread();
		delete _wsHelper;
		_wsHelper = NULL;
	}
}
std::string GsnClient::byte_2_str(const char* bytes, int size){
	char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	std::string str;
	for (int i = 0; i < size; ++i)
	{
		const char ch = bytes[i];
		str.append(&hex[(ch & 0xF0) >> 4], 1);
		str.append(&hex[ch & 0xF], 1);
		str.append(" ");
	}
	return str;
}
void GsnClient::setFinishConnectListener(const FinishConnectListener& listener)
{
	this->_finishConnectListener = listener;
}
void GsnClient::setDisconnectListener(const DisconnectListener& listener)
{
	this->_disconnectListener = listener;
}
void GsnClient::setReceiveDataListener(const ReceiveDataListener& listener)
{
	this->_receiveDataListener = listener;
}
void GsnClient::removeClient(GsnClient* client)
{
    gZPListClient.erase(std::remove(gZPListClient.begin(), gZPListClient.end(), client), gZPListClient.end());
}
