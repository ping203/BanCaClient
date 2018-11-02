#include "Handler.h"
using namespace std;

Handler::Handler(string _id):
	id(_id),userData(NULL),needStop(false),jData(""),callback(nullptr),_timeElapsed(0),_timeOut(0),needTimeOut(false)
{
	
}

Handler::Handler() :
	id(""),userData(NULL),needStop(false),jData(""),callback(nullptr),_timeElapsed(0),_timeOut(0),needTimeOut(false)
{

}

void Handler::setID(string _id)
{
	id = _id;
}

Handler::~Handler(void)
{
	
}

void Handler::set(CallbackHandler _callback)
{
	callback = _callback;
	//userData = uData;
}

void Handler::setTimeOut(double t,bool needTOut)
{
	_timeOut = t;
	needTimeOut = needTOut;
}

void Handler::stop(std::string jdata){
	if(callback)
	{
		callback(jData,userData);
	}
}

static HandlerManager* instance = NULL;
HandlerManager::HandlerManager()
{
    
}

HandlerManager::~HandlerManager()
{

}

HandlerManager* HandlerManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new HandlerManager();
		Director::getInstance()->getScheduler()->scheduleUpdate(instance,0,false);
	}
	return instance;
}

void HandlerManager::destroyInstance()
{
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(instance);
	CC_SAFE_DELETE(instance);
}

void HandlerManager::update(float dt)
{
	map<string,Handler*>::iterator iter = handlers.begin();

	vector<string> needRemoves;

	for (;iter !=handlers.end();iter++)
	{
		if (iter->second->needTimeOut && !iter->second->needStop)
		{
			iter->second->_timeElapsed += dt;
			if (iter->second->_timeElapsed >= iter->second->_timeOut)
			{
				iter->second->callback(iter->second->jData,iter->second->userData);
				
				needRemoves.push_back(iter->first);
				continue;
			}
		}
		if (iter->second->needStop)
		{
			if (iter->second->callback)
			{
				iter->second->callback(iter->second->jData,iter->second->userData);
			}			
			needRemoves.push_back(iter->first);

		}
	}

	for (int i=0;i<needRemoves.size();i++)
	{
		forceRemoveHandler(needRemoves[i]);
	}
}

Handler* HandlerManager::getHandler(string id)
{
	if (handlers.find(id) != handlers.end())
	{
		return (handlers.find(id)->second);
	}
	else
	{
		return NULL;
	}
}

void HandlerManager::addHandler(string id,CallbackHandler callback)
{
	Handler *_hander = new Handler(id);
	_hander->set(callback);
	addHandler(_hander);
	
}

void HandlerManager::addHandler(Handler *_handler)
{
	if (handlers.find(_handler->id) != handlers.end())
	{
		CCLOG("this handler : %s has in handler manager , return ...",_handler->id.c_str());
	}
	else
	{
		handlers.insert(std::pair<string,Handler*>(_handler->id,_handler));
	}
}

void HandlerManager::stopHandler(string id,string jdata)
{
	
	if (handlers.find(id) != handlers.end())
	{
		handlers.at(id)->jData = jdata;
		handlers.at(id)->needStop = true;
	}
}

void HandlerManager::forceRemoveHandler(string id)
{
	if (handlers.find(id) != handlers.end())
	{
        //CCLOG("deleted handler : %s !!",id.c_str());
		delete handlers[id];
		handlers.erase(id);
	}
    else{
        CCLOG("handler: %s  donot exist !!",id.c_str());
    }
}

void HandlerManager::exitIOS()
{
    Director::getInstance()->end();
    ScriptEngineManager::destroyInstance();
    if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
}

std::string StringUtil::json_2_string(rapidjson::Document &d)
{
    return "";
    
}


extern "C"{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	void Java_gsn_zingplay_utils_HandleHelper_stopHandler(JNIEnv* env,jobject thiz,jstring handlerID,jstring _jdata)
	{
		std::string handler = env->GetStringUTFChars(handlerID, 0);
		std::string data = env->GetStringUTFChars(_jdata, 0);

		HandlerManager::getInstance()->stopHandler(handler,data);

	}

#endif
}
