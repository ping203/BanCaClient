#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "curve/CubicBezier.h"
#include "Path.hpp"

#include <vector>
#include "Path3D.h"

using namespace std;
using namespace three::extras::curve;
using namespace three::extras;
USING_NS_CC;
class Entity;
typedef std::function<void(float dt)> EntityUpdateFunc;
typedef std::function<void(Entity *)> EntityFinishFunc;


class HoldFishInfo : public Ref{
public:
	HoldFishInfo(){};
	virtual Ref*  autorelease(){ return this; };

	bool prepare_hold = false;
	bool isHolding = false;
	Entity *fishForHold = NULL;
	Node *player = NULL;

	void setIsHolding(bool hold){ isHolding = hold; }
	bool getIsHolding(){ return isHolding; }
	void setPlayer(Node *p){ player = p; }
	Node*getPlayer(){ return player; }
	void setFish(Entity *f){ fishForHold = f; }
	Entity *getFish(){ return fishForHold; }

};

class Entity : public Ref
{
public:
	Entity();
	~Entity();
    enum Type{
        FISH = 0,
        WALL,
        BULLET,
		FISH3D
    };
    
    virtual Ref*  autorelease(){return this;};

    Vec2 getBodyVelocity(){ b2Vec2 vel = _body->GetLinearVelocity(); return Vec2(vel.x,vel.y); };
	Vec2 getBodyPosition(){ b2Vec2 pos = _body->GetPosition(); return Vec2(pos.x,pos.y); };

    void setVelocity(float x, float y){ _body->SetLinearVelocity(b2Vec2(x,y));};
	void setPosition(float x, float y){
	_body->SetTransform(b2Vec2(x, y), _body->GetAngle());
	};
    void setTransform(Vec2 position,float angle)
    {
        _body->SetTransform(b2Vec2(position.x,position.y), angle);
    }
    float rotationFromVel(Vec2 vel);
    
    virtual void setNodeDisplay(Node *node)
    {
        _nodeDisplay = node;
    }
    Node *getNodeDisplay()
    {
        return _nodeDisplay;
    }

	void setUserData(Ref *userDat)
	{
		_userData = userDat;
	}
	Ref* getUserData(){
		return _userData;
	}

    
    virtual void update(float dt){
        if(_updateFunc)
        {
            _updateFunc(dt);
        }
    }
    void release(){delete this;};
    Type getType(){return _type;};
    
    EntityUpdateFunc _updateFunc;
	EntityFinishFunc _finishFunc;
	void setFinishFunc(const EntityFinishFunc& func){
		_finishFunc = func;
	}
    void setUpdateFunc(const EntityUpdateFunc& func){
        _updateFunc = func;
    }
	void setContentSize(Size& size){
		contentSize = size;
	}
	bool isNeedRemove(){ return need_remove; }
public:
	b2Body* _body;
	Ref *_userData;
    bool need_remove = false;
    Type _type;
    Node *_nodeDisplay;

	Size contentSize = Size(0, 0);

};


class Fish : public Entity
{
public:
    Fish();
    ~Fish();
    
    void pause(bool pause);
    void startWithPath(Path *_path,float timeElapsed);
    void enableAutoDie(bool die){
        enable_auto_die = die;
    }
	void enableFlip(bool flip){
		enable_flip = flip;
	}
	void onNewCurve(three::extras::Curve* curve);


	void enableCheckOutsite(bool check){
		enable_check_outside = check;
	}
	bool isOutsite() {
		return outsite;
	}
    
    virtual void update(float dt);
public:
    bool paused;
    Path *path;
    float time;
    bool enable_auto_die = true;
	bool enable_flip = false;

	bool enable_check_outside = false;
	bool outsite = false;
};

class Wall : public Entity
{
public:
    Wall() : Entity(){
        _type = WALL;
        need_remove = false;
    };
};

class Bullet : public Entity
{
public:
    Bullet(int live) : Entity(){
        _type = BULLET;
        need_remove = false;
        this->live = live;
    };
    virtual void update(float dt);
    int live;
	HoldFishInfo *holdInfo = NULL;
	float velLength = 0;
    
    int getLive()
    {
        return live;
    }
	void setHoldInfo(HoldFishInfo *hold){
		holdInfo = hold;
	}
	HoldFishInfo* getHoldInfo(){
		return holdInfo;
	}
	void setVelLength(float vv){
		velLength = vv;
	}
};


class Fish3D : public Entity
{
public:
	Fish3D();
	~Fish3D();

	void pause(bool paused);
	void resume(){
		pause(false);
	}
	void start( float timeElapsed);
	
	void setPath3D(Path3D *_path)
	{
		path = _path;
	}
	void enableAutoDie(bool die){
		enable_auto_die = die;
	}
	void enableCheckOutsite(bool check){
		enable_check_outside = check;
	}
	bool isOutsite() {
		return outsite;
	}
	void setNodeDebug2D(Node* debug){
		_debug = debug;
	}
	virtual void setNodeDisplay(Node *node);

	virtual void update(float dt);
public:
	bool paused;
	Path3D *path;
	Node* _debug;
	Node* _checkRotatin;
	float time;
	bool enable_auto_die = true;

	bool enable_check_outside = false;
	bool outsite = false;
};







