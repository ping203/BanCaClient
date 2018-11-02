#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "curve/CubicBezier.h"
#include "Path.hpp"

#include <vector>
using namespace std;
using namespace three::extras::curve;
using namespace three::extras;
USING_NS_CC;
class Entity;
typedef std::function<void(float dt)> EntityUpdateFunc;
typedef std::function<void(Entity *)> EntityFinishFunc;
class Entity : public Ref
{
public:
	Entity();
	~Entity();
    enum Type{
        FISH = 0,
        WALL,
        BULLET
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
    
    void setNodeDisplay(Node *node)
    {
        _nodeDisplay = node;
    }
    Node *getNodeDisplay()
    {
        return _nodeDisplay;
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
public:
	b2Body* _body;
	void *_userData;
    bool need_remove = false;
    Type _type;
    Node *_nodeDisplay;

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
    
    virtual void update(float dt);
public:
    bool paused;
    Path *path;
    float time;
    bool enable_auto_die;
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
    
    int getLive()
    {
        return live;
    }
};





