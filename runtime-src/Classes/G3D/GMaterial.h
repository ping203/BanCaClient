#pragma once
#include "cocos2d.h"
USING_NS_CC;
#include <string>

using namespace std;
class GMesh;
class GMaterial : public Material
{
	friend class GMesh;
public:
	GMaterial();
	~GMaterial();

	static GMaterial* createWithGLProgramState(GLProgramState *state);

	Vec3 ambientColor;
	Vec3 diffuseColor;
	Vec3 specularColor;
	Vec3 emissiveColor;
	float shinisess;
	float opacity;

	string id;

	bool hasNormalMap;
};

