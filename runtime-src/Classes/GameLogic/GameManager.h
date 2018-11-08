#pragma once
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <vector>
#include "Entity.h"
#include "AbtractDisplayLayer.h"
using namespace std;
USING_NS_CC;

extern float PM_RATIO;
class Setting : public Ref
{
public:
    Setting(){
        using_constant_fps = true;
        FPS = 1.0/60;
        world_size = Vec2(40,22.5);
        gravity = Vec2(0,0);
        bullet_vel = 40;
        PM_RATIO = 32;
    };
    virtual Ref*  autorelease(){return this;};
public:
    bool using_constant_fps;
    float FPS;
    Vec2 world_size;
    Vec2 gravity;
    float bullet_vel;
    float PM_RATIO;
    
};


typedef std::function<void(Entity *e1,Entity *e2,Vec2 pointCollide)> EntityCollisionListener;   // callback khi cham
typedef std::function<bool(Entity *e1,Entity *e2,Vec2 pointCollide)> OnContactPreSolve;         // callback khi chuan bi tinh toan va cham
typedef std::function<void(Fish*,Path*)> PathListener; // callback khi fish done path

class ContactListener;
class GameManager : public Ref
{
public:
	GameManager(Setting *_setting);
	~GameManager();
    
    virtual Ref*  autorelease(){return this;};

	void initWorld();
    Entity* createWall(Entity *wall);
    Entity* createBodyForBullet(Entity *bullet,Vec2 body_box);
    Entity *createBodyForFish(Entity* fish,Vec2 body_box);
    
    Entity* shootBullet(Entity *bullet,Vec2 start_pos,Vec2 vel_dir);

	Fish* getFishByPos(Vec2 pos);
    
	virtual void destroyEntity(Entity *entity);
	virtual void update(float dt);
    void setEntityCollisionListener(const EntityCollisionListener& _lis)
    {
        _entityCollisionListener = _lis;
    }
    void setOnContactPreSolve(const OnContactPreSolve& _lis)
    {
        _contactPreSolve = _lis;
    }
    
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void BeginContact(b2Contact* contact);
public:
	b2World *m_world;
	vector<Entity *> m_entities;
    vector<Entity* > m_needDestroy;
    
    AbtractDisplayLayer *m_displayLayer;

protected:
	b2BodyDef bulletBodyDef;
	b2BodyDef fishBodyDef;

	b2FixtureDef bulletFixtureDef;
	b2FixtureDef fishFixtureDef;
    
    Setting *m_setting;

    ContactListener *c_listener;
    EntityCollisionListener _entityCollisionListener;
    OnContactPreSolve _contactPreSolve;
    
};

class ContactListener : public b2ContactListener{
public:
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
        gameMgr->PreSolve(contact, oldManifold);
    }
    virtual void BeginContact(b2Contact* contact){
        gameMgr->BeginContact(contact);
    }
    GameManager *gameMgr;
};

