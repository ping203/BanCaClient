#pragma once
#include "cocos2d.h"
USING_NS_CC;
class GShadowMapTexture : public Texture2D
{
public:
	GShadowMapTexture();
	~GShadowMapTexture();

	static GShadowMapTexture *create(int width, int height);

	void init(int width,int height);
	
};

class GTexture2D : public Texture2D
{
public:
	GTexture2D();
	static GTexture2D *create(GLuint name);
	void setName(GLuint name);
};
