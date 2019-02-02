#pragma once
#include "cocos2d.h"
USING_NS_CC;

class GEnvironment :
	public Ref
{
public:
	GEnvironment();
	~GEnvironment();

	static GEnvironment* getInstance();

	void setupLight(Vec3 position, Vec3 intensities, float ambient, float anttenuation,float specular, float cullAngle, float cutAngle);
	void setupShadow(Node *parent, int texWidth, int texHeight, int depthCamFlag);
	void addCausticTexture(std::string tex);
	void cleanCausticTexture();
	void runCausticAnimation(bool run, float deltaTime);

	void setLightUpdate(bool update);
	void setCameraUpdate(bool update);
	void update(float dt);

	void setEnabledShadow(bool on){ enableShadow = on; };
	bool isEnabledShadow(){ return enableShadow; };
	bool isEnableCausticAnimation(){ return enableCausticAnimation; };
	float getDeltaTimeAnim(){ return dtCausticAnim; };

	bool isSupportDepthTexture();

protected:
	bool enableShadow;
	bool enableCausticAnimation = false;
	float dtCausticAnim = 0.1f;
	float timeCount = 0.0f;
	int currentCaustic = 0;
public:
	static Mat4 VP_LIGHT_MATRIX;
	static Texture2D *DEPTH_TEX;
	static Texture2D *ACTIVE_CAUSTIC_TEX ;
	std::vector<Texture2D *> causticTextures;
	static CameraFlag DEPTH_CAM_FLAG;
	

	static Mat4 BIAS_MATRIX;
	static Mat4 DEPTH_BIAS_MVP;

	static bool FLAG_UPDATE_CAM;
};

