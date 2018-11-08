#pragma once
#include "cocos2d.h"
#include "2d/CCCamera.h"

USING_NS_CC;
class GMainLight
{
public:
	static Vec3 POSITION;
	static Vec3 DIR;
	static Vec3 INTENSITIES;
	static float ATTENUATION;
	static float AMBIENT_COEFFICIENT;

	static float cullOff, cutOff;

	static bool dirtyFlag;

	static float specular;

};

