#pragma once
#include "cocos2d.h"
#include "GameLogic/GameManager.h"
#include "AbtractDisplayLayer.h"

USING_NS_CC;
class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();
	CREATE_FUNC(GameLayer);

	bool init() override;
	void onEnter() override;
	void update(float delta) override;

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override;
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override;
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override;

public:
	GameManager *gameMgr;
    AbtractDisplayLayer *displayLayer;
    Layer *bulletLayer;
};

