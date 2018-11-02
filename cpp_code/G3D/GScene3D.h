#pragma once
#include "cocos2d.h"

USING_NS_CC;
class GScene3D : public Layer
{
public:
	GScene3D();
	~GScene3D();

	CREATE_FUNC(GScene3D);

	virtual bool init();
protected:
	float _radius = 1;
	cocos2d::Vec3                    _center;            //camera look target
	float                            _distanceZ =21;

	Camera *mainCam;

	void setupLight();

private:
	void updateCameraTransform();
	void onTouchsMoved(const std::vector<cocos2d::Touch*> &touchs, cocos2d::Event *event);
	void calculateArcBall(cocos2d::Vec3 & axis, float & angle, float p1x, float p1y, float p2x, float p2y);//calculate  rotation quaternion parameters
	float projectToSphere(float r, float x, float y);//points on the screen project to arc ball
};

