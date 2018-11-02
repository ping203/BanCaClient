#pragma once
#include "cocos2d.h"
#include "GShadowMapTexture.h"
using namespace cocos2d::experimental;
USING_NS_CC;
class GFrameBufferObject : public FrameBuffer
{
public:
	GFrameBufferObject();
	~GFrameBufferObject();

	static GFrameBufferObject* create(uint8_t fid, unsigned int width, unsigned int height);

	bool init(uint8_t fid, unsigned int width, unsigned int height);
	virtual void applyFBO();

	GTexture2D *depthTexture;

	GLuint renderBuffer;
	
};

