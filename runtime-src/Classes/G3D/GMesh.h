#pragma once
#include "cocos2d.h"
#include "GMaterial.h"

USING_NS_CC;

class GMaterial;
class GMesh : public Mesh
{
	friend class GSprite3D;
public:
	GMesh();
	~GMesh();

	static GMesh* create(const std::string& name, MeshIndexData* indexData, MeshSkin* skin);

	void setMaterial(GMaterial *material);
	void setMaterial(GMaterial *material,bool inDepth);
	virtual void onDraw(Renderer* renderer, float globalZOrder, const Mat4& transform, uint32_t flags, unsigned int lightMask, const Vec4& color, bool forceDepthWrite);
	

	virtual void setGLProgramState(GLProgramState* glProgramState);

	GLProgramState *mainProgramState;
	GLProgramState *depthProgramState;

	

	void receivedShadow(bool ret);
	//void setTexture(const std::string& texPath);
protected:
	GMaterial *g_material;
	bool hasShadow;
	bool hasSpecular;
	bool hasNormal;
	void setTexture(Texture2D* tex, NTextureData::Usage usage, bool cacheFileName = true);
	void setTexture(const std::string& texPath, NTextureData::Usage usage = NTextureData::Usage::Diffuse);

protected:
	// for shadow
	MeshCommand _depthCommand;
	GMaterial *depthMaterial;

};

