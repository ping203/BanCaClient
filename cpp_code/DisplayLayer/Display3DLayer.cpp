//
//  Display3DLayer.cpp
//  Fish3D
//
//  Created by Admin on 8/31/18.
//

#include "Display3DLayer.hpp"
#include "../G3D/GEnvironment.h"
#include "../G3D/GSprite3D.h"

Display3DLayer::Display3DLayer()
{
    
}
Display3DLayer::~Display3DLayer()
{
    
}

bool Display3DLayer::init()
{
    Layer::init();
    auto size = Director::getInstance()->getWinSize();
    
//    auto bg = Sprite::create("fish_bg.jpg");
//    addChild(bg);
//    bg->setPosition(size.width/2,size.height/2);
//    bg->setScale(size.width / 700, size.height/392);
    
    
    mainCam = Camera::createPerspective(45, size.width / size.height, 0.1f, 1000);
    //mainCam = Camera::createOrthographic(size.width/5, size.height/5, 0.1, 1000);
    mainCam->setPosition3D(Vec3(0, 100, 100));
    mainCam->lookAt(Vec3(0, 0, 0));
    mainCam->setCameraFlag(CameraFlag::USER1);
    addChild(mainCam);
    mainCam->setDepth(2);
    
    setupLight();
    setupScene();
    
    //scheduleUpdate();
    
    return true;
}

void Display3DLayer::setupLight()
{
    GEnvironment::getInstance()->setupLight(Vec3(0, 100, 30), Vec3(1.15, 1.15, 1.15), 0.5, 0.1);
//    GEnvironment::getInstance()->setupShadow(this, 1024, 1024, (int)CameraFlag::USER2);
//    GEnvironment::getInstance()->setEnabledShadow(true);
    
    
    GEnvironment::CAUSTIC_TEX = Director::getInstance()->getTextureCache()->addImage("caustics.png");
    Texture2D::TexParams texParams;
    texParams.magFilter = GL_LINEAR;
    texParams.minFilter = GL_LINEAR;
    texParams.wrapS = GL_REPEAT;
    texParams.wrapT = GL_REPEAT;
    GEnvironment::CAUSTIC_TEX->setTexParameters(texParams);
    
}
void Display3DLayer::setupScene()
{
//    GSprite3D *fishModel = GSprite3D::create("Fish3D/ca_ngua.c3b");
//    GSprite3D *fishPath = GSprite3D::create("FishPath/Fish_Path1.c3b",false,false);
//    
//    //fishPath->setCameraMask(2);
//    fishModel->setCameraMask(2);
//    fishPath->setScale(.1);
//    this->addChild(fishPath);
//    fishPath->setPosition3D(Vec3(-80,0,0));
//    
//    Animation3D *animation3D = Animation3D::create("FishPath/Fish_Path1.c3b");
//    Animate3D * anim3D = Animate3D::create(animation3D);
//    anim3D->setQuality(Animate3DQuality::QUALITY_HIGH);
//    fishPath->runAction(RepeatForever::create(anim3D));
//    
//    fishPath->getAttachNode("Bone")->addChild(fishModel);
}

