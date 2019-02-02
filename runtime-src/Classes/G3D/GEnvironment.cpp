#include "GEnvironment.h"
#include "GMainLight.h"
#include "GFrameBufferObject.h"

#include <cmath>


Mat4 GEnvironment::VP_LIGHT_MATRIX;
Texture2D *GEnvironment::DEPTH_TEX;
Texture2D * GEnvironment::ACTIVE_CAUSTIC_TEX = NULL;

Mat4 GEnvironment::BIAS_MATRIX;
Mat4 GEnvironment::DEPTH_BIAS_MVP;
CameraFlag GEnvironment::DEPTH_CAM_FLAG;
bool GEnvironment::FLAG_UPDATE_CAM = false;
using namespace experimental;
GEnvironment::GEnvironment()
{
	enableShadow = false;
	DEPTH_CAM_FLAG = CameraFlag::USER8;
	timeCount = 0;
}

GEnvironment *g_EnvIns = NULL;
GEnvironment::~GEnvironment()
{
}


GEnvironment * GEnvironment::getInstance()
{
	if (g_EnvIns == NULL)
	{
		g_EnvIns = new GEnvironment();
		Director::getInstance()->getScheduler()->scheduleUpdateForTarget(g_EnvIns, 1, false);
	}
		
	return g_EnvIns;
}

void GEnvironment::setupLight(Vec3 position, Vec3 intensities, float ambient, float anttenuation, float specular,float cullAngle, float cutAngle)
{
	GMainLight::POSITION = position;
	GMainLight::INTENSITIES = intensities;
	GMainLight::AMBIENT_COEFFICIENT = ambient;
	GMainLight::ATTENUATION = anttenuation;



	GMainLight::cullOff = cosf(cullAngle * M_PI / 180.0);
	GMainLight::cutOff = cosf(cutAngle * M_PI / 180.0);

	GMainLight::specular = specular;

	GMainLight::dirtyFlag = true;
}

void GEnvironment::setLightUpdate(bool update)
{
	GMainLight::dirtyFlag = update;
}

void GEnvironment::setCameraUpdate(bool update)
{
	GEnvironment::FLAG_UPDATE_CAM = update;
}

bool GEnvironment::isSupportDepthTexture()
{
	return true;
}

void GEnvironment::addCausticTexture(std::string texPath)
{
	auto tex = Director::getInstance()->getTextureCache()->addImage(texPath);
	if (tex)
	{
		Texture2D::TexParams params;
		params.minFilter = GL_NEAREST;
		params.magFilter = GL_NEAREST;
		params.wrapS = GL_REPEAT;
		params.wrapT = GL_REPEAT;

		tex->setTexParameters(params);
		tex->retain();

		causticTextures.push_back(tex);

		ACTIVE_CAUSTIC_TEX = tex;
	}
}

void GEnvironment::cleanCausticTexture()
{
	for (int i = 0; i < causticTextures.size(); i++)
	{
		Director::getInstance()->getTextureCache()->removeTexture(causticTextures[i]);
	}
	causticTextures.clear();
	ACTIVE_CAUSTIC_TEX = NULL;
}

void GEnvironment::runCausticAnimation(bool run, float deltaTime)
{
	enableCausticAnimation = run;
	dtCausticAnim = deltaTime;
	
}

void GEnvironment::update(float dt)
{
	
	if (causticTextures.size() == 0)
		return;
	timeCount += dt;
	if (timeCount >= dtCausticAnim)
	{
		currentCaustic++;
		if (currentCaustic >= causticTextures.size())
		{
			currentCaustic = 0;
		}
		timeCount = 0;
		ACTIVE_CAUSTIC_TEX = causticTextures[currentCaustic];
	}
}

void GEnvironment::setupShadow(Node *parent, int texWidth, int texHeight, int depthCamFlag)
{
	const unsigned int SHADOW_WIDTH = texWidth, SHADOW_HEIGHT = texHeight;

	auto size = Director::getInstance()->getWinSize();
	Camera *depthCam = Camera::createOrthographic(10, 10, 0.1, 100);
	depthCam->setPosition3D(GMainLight::POSITION);
	depthCam->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));;
	depthCam->setCameraFlag((CameraFlag)depthCamFlag);
	parent->addChild(depthCam);
	depthCam->setDepth(2);

	

	VP_LIGHT_MATRIX = depthCam->getViewProjectionMatrix();
	DEPTH_CAM_FLAG = (CameraFlag)depthCamFlag;

	if (isSupportDepthTexture())
	{
		GFrameBufferObject *frameBuffer = GFrameBufferObject::create(1, SHADOW_WIDTH, SHADOW_HEIGHT);
		depthCam->setFrameBufferObject(frameBuffer);
		DEPTH_TEX = frameBuffer->depthTexture;
	}
	else
	{
		FrameBuffer *frameBuff = FrameBuffer::create(1, SHADOW_WIDTH, SHADOW_HEIGHT);
	
		RenderTarget *target = RenderTarget::create(SHADOW_WIDTH, SHADOW_HEIGHT);
		RenderTargetDepthStencil *depth = RenderTargetDepthStencil::create(SHADOW_WIDTH, SHADOW_HEIGHT);

		frameBuff->attachRenderTarget(target);
		frameBuff->attachDepthStencilTarget(depth);

		depthCam->setFrameBufferObject(frameBuff);

		DEPTH_TEX = target->getTexture();

// 		GFrameBufferObject *frameBuffer = GFrameBufferObject::create(1, SHADOW_WIDTH, SHADOW_HEIGHT);
// 		depthCam->setFrameBufferObject(frameBuffer);
// 		DEPTH_TEX = frameBuffer->depthTexture;
	}

	

	
}
