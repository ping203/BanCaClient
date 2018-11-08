#pragma once
#include "cocos2d.h"

USING_NS_CC;

class CC_DLL GBatchMesh : public Node
{
public:
	GBatchMesh();
	~GBatchMesh();

	static GBatchMesh* create(const MeshData &meshData);

	virtual bool init() override;

	GLuint _vao;
	GLuint _vbos[2];

	BlendFunc _blendFunc;
	CustomCommand _command;

	MeshData meshData;

	struct V3F_N3F_T2F
	{
		Vec3 vertex;
		Vec3 normal;
		Vec2 texCoord;
	};

	V3F_N3F_T2F *_buffer;


public:
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

	// Overrides
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

};

