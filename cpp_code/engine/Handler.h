#pragma once
#include <string>
#include <vector>
#include <map>
#include "cocos2d.h"
#include <cmath>
#include "GsnClient.h"
#include "InPacket.h"
#include "OutPacket.h"
#include "sPackage.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Box2D/Box2D.h"


typedef std::function<void(std::string,cocos2d::Ref*)> CallbackHandler;		// callback when finish handler

class Handler : public cocos2d::Ref
{
public:
	Handler(std::string _id);
	Handler();
	~Handler(void);

	void set(CallbackHandler _callback);
	void setTimeOut(double t,bool needTOut);
	void setID(std::string _id);
	void stop(std::string _jdata);
private:
	friend class HandlerManager;
	std::string id;
	std::string jData;
	bool needStop;
	cocos2d::Ref *userData;
	CallbackHandler callback;
	float _timeElapsed;
	float _timeOut;
	bool needTimeOut;
	b2Body *_body;
};

class HandlerManager : public cocos2d::Ref
{
public:
	HandlerManager();
	~HandlerManager();

protected:
	std::map<std::string, Handler* > handlers;
public:
	virtual void update(float dt);

public:
	static HandlerManager *getInstance();
	static void destroyInstance();

	void addHandler(Handler *_handler);
	void addHandler(std::string id, CallbackHandler callback);

	Handler* getHandler(std::string id);

	void stopHandler(std::string id, std::string jdata);
	void forceRemoveHandler(std::string id);
    void exitIOS();
    
};

class StringUtil
{
public:
    static std::string json_2_string(rapidjson::Document &d);
};