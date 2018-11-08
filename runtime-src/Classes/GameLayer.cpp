#include "GameLayer.h"
#include "G3D/GScene3D.h"
#include "DisplayLayer/Display3DLayer.hpp"
GameLayer::GameLayer()
{
}


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
	
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{

	auto location = touch->getLocation();

    return true;
}

void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
}

void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
