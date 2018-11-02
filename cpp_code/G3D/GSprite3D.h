#pragma once
#include "cocos2d.h"
USING_NS_CC;
#include <string>
#include "GBundle3D.h"
#include "GMaterial.h"

using namespace std;
class GSprite3D : public Sprite3D
{
public:
	GSprite3D();
	~GSprite3D();

	static GSprite3D *create(string modelPath, bool shadow = false);
	virtual bool initFromPath(string modelPath, bool shadow);
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	void setTexture(const std::string& texFile);
protected:
	bool loadFromFile(const std::string& path, NodeDatas* nodedatas, MeshDatas* meshdatas, GMaterialDatas* materialdatas);
	bool initFrom(const NodeDatas& nodedatas, const MeshDatas& meshdatas, const GMaterialDatas& materialdatas);

	bool loadFromCache(const std::string& path);
	
	void createNode(NodeData* nodedata, Node* root, const GMaterialDatas& materialdatas, bool singleSprite);
	GSprite3D* createSprite3DNode(NodeData* nodedata, ModelData* modeldata, const GMaterialDatas& materialdatas);
	void createAttachSprite3DNode(NodeData* nodedata, const GMaterialDatas& materialdatas);

	void genMaterial( GMaterialData* materialdatas,GLProgramState *state);
	GMaterial* genMaterialSingle(GMaterialData* materialdatas, GLProgramState *state);

	bool hasReceivedShadow;
};


///////////////////////////////////////////////////////
/**
* Sprite3DCache
* @brief the cache data of Sprite3D, use to speed up Sprite3D::create
*/
class GSprite3DCache
{
public:
	struct GSprite3DData
	{
		Vector<MeshVertexData*>   meshVertexDatas;
		Vector<GLProgramState*>   glProgramStates;
		NodeDatas*      nodedatas;
		GMaterialDatas*  materialdatas;
		~GSprite3DData()
		{
			if (nodedatas)
				delete nodedatas;
			if (materialdatas)
				delete materialdatas;
			meshVertexDatas.clear();
			glProgramStates.clear();
		}
	};

	/**get & destroy*/
	static GSprite3DCache* getInstance();
	static void destroyInstance();

	/**
	* get the SpriteData struct
	*
	* @lua NA
	*/
	GSprite3DData* getSpriteData(const std::string& key) const;

	/**
	* add the SpriteData into Sprite3D by given the specified key
	*
	* @lua NA
	*/
	bool addSprite3DData(const std::string& key, GSprite3DData* spritedata);

	/**remove the SpriteData from Sprite3D by given the specified key*/
	void removeSprite3DData(const std::string& key);

	/**remove all the SpriteData from Sprite3D*/
	void removeAllSprite3DData();

CC_CONSTRUCTOR_ACCESS:
	GSprite3DCache();
	~GSprite3DCache();

protected:


	static GSprite3DCache*                        _cacheInstanceG;
	std::unordered_map<std::string, GSprite3DData*> _spriteDatas; //cached sprite data
};