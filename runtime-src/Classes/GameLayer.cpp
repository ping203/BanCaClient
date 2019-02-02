#include "GameLayer.h"
#include "G3D/GScene3D.h"
#include "DisplayLayer/Display3DLayer.hpp"
#include "math/Vector2.h"
#include "curve/Spline.h"
using namespace three::math;
GameLayer::GameLayer()
{
}


Sprite *test;
std::vector<Vector2> points;
static int count_ = 0;
static bool chay = false;
static float time_ = 0;

Spline *spline2D = NULL;


GameLayer::~GameLayer()
{
}

bool GameLayer::init()
{
	Layer::init();
    displayLayer = Display3DLayer::create();
    addChild(displayLayer);
    
    bulletLayer = Layer::create();
    addChild(bulletLayer,1);

    
//    fish->node2 = Sprite::create("caustics.png");
//    addChild(fish->node2);

	test = Sprite::create("pointer.png");
	addChild(test);

    
    
	return true;
}

void GameLayer::onEnter()
{
	Layer::onEnter();

	scheduleUpdate();

	// Adds touch event listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}
void GameLayer::update(float delta)
{
	if (chay)
	{
		time_ += delta;
		float tt = time_ * 1.0f / 10;
		if (tt > 1)
			tt = 1;
		Vector2 pos = spline2D->getPoint(tt);
		test->setPosition(pos.x(), pos.y());
	}
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{

	auto location = touch->getLocation();
	points.push_back(Vector2(location.x, location.y));
	count_++;
	if (count_ == 6)
	{
		spline2D = new Spline(points);
	}
	if (count_ >= 6)
		chay = true;

    return true;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
}

void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
