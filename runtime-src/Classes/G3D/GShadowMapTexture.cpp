#include "GShadowMapTexture.h"


GShadowMapTexture::GShadowMapTexture() : Texture2D()
{
}


GShadowMapTexture::~GShadowMapTexture()
{
}

GShadowMapTexture * GShadowMapTexture::create(int width, int height)
{
	GShadowMapTexture *ret = new GShadowMapTexture();
	ret->init(width, height);
	return ret;
}

void GShadowMapTexture::init(int width, int height)
{
	glGenTextures(1, &_name);
	GL::bindTexture2D(_name);

	const char* hi = (const char*)glGetString(GL_EXTENSIONS);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (false)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24_OES, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);		
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
	}

#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
#endif

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//

GTexture2D::GTexture2D() : Texture2D()
{

}

GTexture2D * GTexture2D::create(GLuint name)
{
	GTexture2D *ret = new GTexture2D();
	ret->_name = name;
	return ret;
}

void GTexture2D::setName(GLuint name)
{
	_name = name;
}