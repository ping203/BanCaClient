#include "Entity.h"

#include "math/Math2.h"
#include "math/Vector2.h"
#include "GameManager.h"
using namespace three::math;
Entity::Entity() 
{
    
	_body = NULL;
    //_updateFunc = NULL;
    _nodeDisplay = NULL;
}


Entity::~Entity()
{
}

float Entity::rotationFromVel(Vec2 vel)
{
    if(vel.y == 0)
    {
        return vel.x > 0?90:-90;
    }
    else if(vel.y > 0)
    {
        return atanf(vel.x / vel.y) * 180 / 3.14;
    }
    else
    {
        return atanf(vel.x / vel.y) * 180 / 3.14 + 180;
    }
}


// Fish
Fish::Fish() : Entity()
{
    paused = false;
    _type = FISH;
    path = NULL;
	enable_auto_die = true;
}
Fish::~Fish()
{
    if(path)
        delete path;
}

void Fish::pause(bool pause)
{
    paused = pause;
}

void Fish::startWithPath(Path *_path, float timeElapsed)
{
    path = _path;
    paused = false;
    time = timeElapsed;
}

void Fish::update(float dt)
{
    if(paused)
        return;
    time += dt;
    if(time >= path->m_duration)
    {
		if (_finishFunc)
		{
			_finishFunc(this);
		}
        if(enable_auto_die)
            need_remove = true;
        time = 0;
    }
    
    Vec2 pos = path->getPositionFromTime(time);
    float angle = path->getCurrentAngleRad();
    setTransform(Vec2(pos.x / PM_RATIO,pos.y/PM_RATIO), -angle );

    if(_nodeDisplay)
    {
        if(_nodeDisplay->getUserData())
        {
                //CCLOG("%f %f",pos.x,pos.y);
        }
        else
        {
            _nodeDisplay->setPosition(Vec2(pos.x,pos.y));
            _nodeDisplay->setRotation(angle * 180 / M_PI);
        }
        
    }
    
    
}

void Bullet::update(float dt)
{
    if(_nodeDisplay)
    {
        Vec2 pos = getBodyPosition();
        _nodeDisplay->setPosition(Vec2(pos.x * PM_RATIO,pos.y * PM_RATIO));
        _nodeDisplay->setRotation(rotationFromVel(getBodyVelocity()));
    }
}

