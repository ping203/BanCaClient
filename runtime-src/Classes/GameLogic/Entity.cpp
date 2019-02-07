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
	_userData = NULL;
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
	_path->setPathListener(CC_CALLBACK_1(Fish::onNewCurve,this));
	update(0);
}

void Fish::onNewCurve(three::extras::Curve* curve){
	if (!enable_flip || !_nodeDisplay)
		return;
	float offset = 0;
	if (dynamic_cast<CubicBezier*>(curve))
	{
		offset = ((CubicBezier *)curve)->v3.x() - ((CubicBezier *)curve)->v0.x();
	}
	else if (dynamic_cast<QuadraticBezier*>(curve))
	{
		offset = ((QuadraticBezier *)curve)->v2.x() - ((QuadraticBezier *)curve)->v0.x();
	}
	else if (dynamic_cast<Line*>(curve))
	{
		offset = ((Line *)curve)->_v2.x() - ((Line *)curve)->_v1.x();
	}

	Sprite* sp = (Sprite*)(_nodeDisplay->getChildByTag(0));
	if (sp)
	{
		sp->setFlippedX(offset > 0);
	}

	sp = (Sprite*)(_nodeDisplay->getChildByTag(1));
	if (sp)
	{
		sp->setFlippedX(offset > 0);
	}

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
	if (dt>0)
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

	if (enable_check_outside)
	{
		Rect rect = CCRectMake(-1.5, -1.5, WORLD_WIDTH + 3, WORLD_HEIGHT + 3);
		outsite = !rect.containsPoint(Vec2(pos.x / PM_RATIO, pos.y / PM_RATIO));
	}
    
    
}

void Bullet::update(float dt)
{
	Vec2 pos = getBodyPosition();

	if (holdInfo && holdInfo->isHolding){
		Vec2 fish_pos = holdInfo->fishForHold->getBodyPosition();

		Vec2 vel_dir = fish_pos - pos;
		b2Vec2 vel(vel_dir.x, vel_dir.y);
		vel.Normalize();
		vel.x *= velLength;
		vel.y *= velLength;

		_body->SetLinearVelocity(vel);
	}

    if(_nodeDisplay)
    {
        
        _nodeDisplay->setPosition(Vec2(pos.x * PM_RATIO,pos.y * PM_RATIO));
        _nodeDisplay->setRotation(rotationFromVel(getBodyVelocity()));
    }
}


// Fish3D

Fish3D::Fish3D() : Entity()
{
	paused = true;
	_type = FISH;
	path = NULL;
	enable_auto_die = true;
	_debug = NULL;
}
Fish3D::~Fish3D()
{
	if (path)
		path->release();
}

void Fish3D::pause(bool pause)
{
	paused = pause;
}

void Fish3D::start(float timeElapsed)
{
	paused = false;
	time = timeElapsed;
	
	update(0);
}



void Fish3D::update(float dt)
{
	if (paused)
		return;
	time += dt;
	if (time >= path->getDuration())
	{
		if (_finishFunc)
		{
			_finishFunc(this);
		}
		if (enable_auto_die)
			need_remove = true;
		time = 0;
	}

// 	Vec2 pos = path->getPositionFromTime(time);
// 	float angle = path->getCurrentAngleRad();
// 	if (dt > 0)
// 		setTransform(Vec2(pos.x / PM_RATIO, pos.y / PM_RATIO), -angle);


// 	if (enable_check_outside)
// 	{
// 		Rect rect = CCRectMake(-1.5, -1.5, WORLD_WIDTH + 3, WORLD_HEIGHT + 3);
// 		outsite = !rect.containsPoint(Vec2(pos.x / PM_RATIO, pos.y / PM_RATIO));
// 	}

	Mat4 transform = path->getTransformFromTimeline(time);
	Vec3 pos;
	cocos2d::Quaternion rot;
	transform.getTranslation(&pos);
	transform.getRotation(&rot);
	if (_nodeDisplay)
	{
		_nodeDisplay->setPosition3D(pos);
		_nodeDisplay->setRotationQuat(rot);
	}

	Camera *cam = dynamic_cast<Camera *>(_userData);
	if (cam)
	{
		Vec2 screenPositon = cam->project(pos);
		screenPositon.y = Director::getInstance()->getWinSize().height - screenPositon.y;
		if (_debug)
		{
			_debug->setPosition(screenPositon);
			
		}
		if (dt > 0)
			setTransform(Vec2(screenPositon.x / PM_RATIO, screenPositon.y / PM_RATIO), 0);
	}

	


}
