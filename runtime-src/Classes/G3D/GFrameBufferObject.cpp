#include "GFrameBufferObject.h"
#include "GEnvironment.h"


GFrameBufferObject::GFrameBufferObject() : FrameBuffer()
{
	depthTexture = NULL;
}


GFrameBufferObject::~GFrameBufferObject()
{
}

GFrameBufferObject* GFrameBufferObject::create(uint8_t fid, unsigned int width, unsigned int height)
{
	auto result = new (std::nothrow) GFrameBufferObject();
	if (result && result->init(fid, width, height))
	{
		result->autorelease();
		return result;
	}
	else
	{
		CC_SAFE_DELETE(result);
		return nullptr;
	}
}

bool GFrameBufferObject::init(uint8_t fid, unsigned int width, unsigned int height)
{
	FrameBuffer::init(fid, width, height);

	// init render texture and depth texure

	GLuint depthTextureId;

	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT16, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
// 	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	depthTexture = GTexture2D::create(depthTextureId);



	return true;
}

void GFrameBufferObject::applyFBO() 
{
	CHECK_GL_ERROR_DEBUG();
	glEnable(GL_DEPTH_TEST);

	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&_previousFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glEnable(GL_DEPTH_TEST);
	//    CCASSERT(_fbo==0 || _fbo != _previousFBO, "calling applyFBO without restoring the previous one");
	CHECK_GL_ERROR_DEBUG();
	if (_fboBindingDirty && !isDefaultFBO())
	{
		CHECK_GL_ERROR_DEBUG();
		glBindTexture(GL_TEXTURE_2D, depthTexture->getName());
		if (!GEnvironment::getInstance()->isSupportDepthTexture())
		{
			//glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			
			// depth attachment
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

			// color attachment
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getName(), 0);
		}
		else
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getName(), 0);
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
#endif
			
		
		}


		
		CHECK_GL_ERROR_DEBUG();
		//CCLOG("FBO is %d _fbo %d color, %d ds", _fbo, RenderTargetBase::Type::Texture2D == _rt->getType() ? _rt->getTexture()->getName() : _rt->getBuffer(), nullptr == _rtDepthStencil ? 0 : _rtDepthStencil->getBuffer());
		_fboBindingDirty = false;
	}
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		CCLOG("FrameBuffer Status Error %d", (int)glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}
	//glCullFace(GL_BACK);
	CHECK_GL_ERROR_DEBUG();
}