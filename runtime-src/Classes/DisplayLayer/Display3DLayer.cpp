//
//  Display3DLayer.cpp
//  Fish3D
//
//  Created by Admin on 8/31/18.
//

#include "Display3DLayer.hpp"
#include "../G3D/GEnvironment.h"
#include "../G3D/GSprite3D.h"
#include "curve/Spline.h"
#include "math/Vector3.h"
#include "DrawNode3D.h"

#include "GameLogic/Path3D.h"


using namespace three::extras::curve;
using namespace three::math;


Spline3D *spline = 0;
float __time = 0;
Node *model;
DrawNode3D *__draw = NULL;
CatmullRomCal *__catMull = NULL;
Path3D *_path;

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
    mainCam->setPosition3D(Vec3(0, 0, 100));
    mainCam->lookAt(Vec3(0, 0, 0));
    mainCam->setCameraFlag(CameraFlag::USER1);
    addChild(mainCam);
    mainCam->setDepth(2);

	__draw = DrawNode3D::create();
	__draw->setCameraMask(2);
	//__draw->drawLine(Vec3(0, 0, 0), Vec3(0, 100, 0), ccc4f(1, 1, 1, 1));
	addChild(__draw);
	//__draw->setVisible(false);
	
    
    setupLight();
    setupScene();
    
    scheduleUpdate();
    
    return true;
}

void Display3DLayer::setupLight()
{
    GEnvironment::getInstance()->setupLight(Vec3(0, 100, 30), Vec3(1.15, 1.15, 1.15), 0.5,1.0, 0.1,0,10);
//    GEnvironment::getInstance()->setupShadow(this, 1024, 1024, (int)CameraFlag::USER2);
//    GEnvironment::getInstance()->setEnabledShadow(true);
    
    
//     GEnvironment::CAUSTIC_TEX = Director::getInstance()->getTextureCache()->addImage("caustics.png");
//     Texture2D::TexParams texParams;
//     texParams.magFilter = GL_LINEAR;
//     texParams.minFilter = GL_LINEAR;
//     texParams.wrapS = GL_REPEAT;
//     texParams.wrapT = GL_REPEAT;
//     GEnvironment::CAUSTIC_TEX->setTexParameters(texParams);
    
}
void Display3DLayer::setupScene()
{
   auto fishModel = Sprite3D::create("fish3D/TienNu_1mesh.c3t");
   
   
   //fishModel->setScale(10);
   
   Animation3D *animation3D = Animation3D::create("fish3D/TienNu_1mesh.c3t","Move");
   Animate3D * anim3D = Animate3D::create(animation3D);
   anim3D->setQuality(Animate3DQuality::QUALITY_HIGH);
   fishModel->runAction(RepeatForever::create(anim3D));

   fishModel->setRotation3D(Vec3(90, 0, 0));


   model = Sprite3D::create();
   model->setCameraMask(2);
   addChild(model);
   
   model->addChild(fishModel);
   fishModel->setCameraMask(2);
   
   AABB aabb = fishModel->getAABBRecursively();
   fishModel->setScale(10);
   
   Vec3 *corner = new Vec3[8];
   aabb.getCorners(corner);

   DrawNode3D *_aabbDraw = DrawNode3D::create();
   _aabbDraw->setCameraMask(2);
   fishModel->addChild(_aabbDraw);
   _aabbDraw->drawCube(corner, ccc4f(1, 0, 0, 1));
   //_aabbDraw->setScale(10);


   std::vector<Vector3> points;
   points.push_back(Vector3(-100, 0, -100));
   points.push_back(Vector3(0, 0, 0));
   points.push_back(Vector3(100, 0, -100));

   spline = new Spline3D(points);


   __catMull = new CatmullRomCal(points, 200, true);
   _path = new Path3D(15,true);
   _path->addPathPoint(Vec3(-100, 0, -100));
   _path->addPathPoint(Vec3(0, 0, 0));
   _path->addPathPoint(Vec3(100, 0, -100));

   
}


void Display3DLayer::update(float dt)
{
	__time += dt;
	Mat4 transform = _path->getTransformFromTimeline(__time);
	Vec3 pos;
	cocos2d::Quaternion rot;
	transform.getTranslation(&pos);
	transform.getRotation(&rot);
	model->setPosition3D(pos);
	model->setRotationQuat(rot);

	for (int i = 0; i < 8 * 40; i++)
	{
		mainCam->unproject(pos);
	}
}

