#pragma once
#include "cocos2d.h"
#include "3d/CCBundle3D.h"
#include <string>
USING_NS_CC;
using namespace std;


struct GMaterialData
{
public:
	std::vector<NTextureData> textures;
	std::string id;
	const NTextureData* getTextureData(const NTextureData::Usage& type) const
	{
		for (const auto& it : textures)
		{
			if (it.type == type)
				return &it;
		}
		return nullptr;
	}


	Vec3 ambientColor;
	Vec3 diffuseColor;
	Vec3 specularColor;
	Vec3 emissiveColor;
	float shinisess;
	float opacity;

	GMaterialData()
	{
		id = "";
		ambientColor = Vec3(0, 0, 0);
		diffuseColor = Vec3(1, 1, 1);
		specularColor = Vec3(0, 0, 0);
		emissiveColor = Vec3(1, 1, 1);
		shinisess = 1;
		opacity = 1;
	}
};


struct GMaterialDatas
{
public:
	std::vector<GMaterialData> materials;
	void resetData()
	{
		materials.clear();
	}
	const GMaterialData* getMaterialData(const std::string& materialid) const
	{
		for (const auto& it : materials)
		{
			if (it.id == materialid)
				return &it;
		}
		return nullptr;
	}
};


class GBundle3D : public Bundle3D
{
public:
	GBundle3D();
	~GBundle3D();

	static GBundle3D* createInstance(string path);

	virtual bool loadMeshDatas(MeshDatas& meshdatas);
	virtual bool loadMaterials(GMaterialDatas& meshdatas);

protected:
	bool loadMeshDatasJsonWithTangent(MeshDatas& meshdatas);
	bool loadMaterialsJson(GMaterialDatas& materialdatas);
	bool loadMaterialsBinary(GMaterialDatas& materialdatas);
	bool loadMeshDatasBinaryCalTangent(MeshDatas& meshdatas);


	bool hasNormalMap;

};

