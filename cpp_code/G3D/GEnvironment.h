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

	void setLightUpdate(bool update);
	void setCameraUpdate(bool update);

	void setEnabledShadow(bool on){ enableShadow = on; };
	bool isEnabledShadow(){ return enableShadow; };


	bool isSupportDepthTexture();

protected:
	bool enableShadow;
public:
	static Mat4 VP_LIGHT_MATRIX;
	static Texture2D *DEPTH_TEX;
	static CameraFlag DEPTH_CAM_FLAG;
	

	static Mat4 BIAS_MATRIX;
	static Mat4 DEPTH_BIAS_MVP;

	static bool FLAG_UPDATE_CAM;
};

