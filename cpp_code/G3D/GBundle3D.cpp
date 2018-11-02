#include "GBundle3D.h"



static const char* VERSION = "version";
static const char* ID = "id";
static const char* DEFAULTPART = "body";
static const char* VERTEXSIZE = "vertexsize";
static const char* VERTEX = "vertex";
static const char* VERTICES = "vertices";
static const char* INDEXNUM = "indexnum";
static const char* INDICES = "indices";
static const char* SUBMESH = "submesh";
static const char* ATTRIBUTES = "attributes";
static const char* ATTRIBUTESIZE = "size";
static const char* TYPE = "type";
static const char* ATTRIBUTE = "attribute";
static const char* SKIN = "skin";
static const char* BINDSHAPE = "bindshape";
static const char* MESH = "mesh";
static const char* MESHES = "meshes";
static const char* MESHPARTID = "meshpartid";
static const char* MATERIALID = "materialid";
static const char* NODE = "node";
static const char* NODES = "nodes";
static const char* CHILDREN = "children";
static const char* PARTS = "parts";
static const char* BONES = "bones";
static const char* SKELETON = "skeleton";
static const char* MATERIALS = "materials";
static const char* ANIMATIONS = "animations";
static const char* TRANSFORM = "transform";
static const char* OLDTRANSFORM = "tansform";
static const char* ANIMATION = "animation";
static const char* MATERIAL = "material";
static const char* BASE = "base";
static const char* FILENAME = "filename";
static const char* TEXTURES = "textures";
static const char* LENGTH = "length";
static const char* BONEID = "boneId";
static const char* KEYFRAMES = "keyframes";
static const char* TRANSLATION = "translation";
static const char* ROTATION = "rotation";
static const char* SCALE = "scale";
static const char* KEYTIME = "keytime";
static const char* AABBS = "aabb";
static const char* TANGENT = "tangent";
static const char* BITANGENT = "binormal";

#define BUNDLE_TYPE_SCENE               1
#define BUNDLE_TYPE_NODE                2
#define BUNDLE_TYPE_ANIMATIONS          3
#define BUNDLE_TYPE_ANIMATION           4
#define BUNDLE_TYPE_ANIMATION_CHANNEL   5
#define BUNDLE_TYPE_MODEL               10
#define BUNDLE_TYPE_MATERIAL            16
#define BUNDLE_TYPE_EFFECT              18
#define BUNDLE_TYPE_CAMERA              32
#define BUNDLE_TYPE_LIGHT               33
#define BUNDLE_TYPE_MESH                34
#define BUNDLE_TYPE_MESHPART            35
#define BUNDLE_TYPE_MESHSKIN            36


GBundle3D::GBundle3D()
{
	hasNormalMap = false;
}


GBundle3D::~GBundle3D()
{
}

GBundle3D* GBundle3D::createInstance(string path)
{
	GBundle3D * ret = new GBundle3D();
	if (ret->load(path))
	{
		return ret;
	}
	else
	{
		delete ret;
		return NULL;
	}

	return ret;
}

bool GBundle3D::loadMeshDatas(MeshDatas& meshdatas)
{
	meshdatas.resetData();
	if (_isBinary)
	{
		return loadMeshDatasBinaryCalTangent(meshdatas);
	}
	else
	{
		return loadMeshDatasJsonWithTangent(meshdatas);
	}
	return true;
}

bool GBundle3D::loadMaterials(GMaterialDatas& materialdatas)
{
	materialdatas.resetData();
	if (_isBinary)
	{
		return loadMaterialsBinary(materialdatas);
	}
	else
	{
		return loadMaterialsJson(materialdatas);
	}
	return true;
}

void calculateTangentBinormal(Vec3 &tangent, Vec3& binormal, const Vec3& point0, const Vec2& uv0, const Vec3& point1, const Vec2& uv1, const Vec3& point2, const Vec2& uv2)
{
	Vec2 deltaUV1 = uv1 - uv0;
	Vec2 deltaUV2 = uv2 - uv0;

	Vec3 deltaPos1 = point1 - point0;
	Vec3 deltaPos2 = point2 - point0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
	binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;
	tangent.normalize();
	binormal.normalize();
}
bool  GBundle3D::loadMeshDatasJsonWithTangent(MeshDatas& meshdatas)
{
	return loadMeshDatasJson(meshdatas);

	const rapidjson::Value& mesh_data_array = _jsonReader[MESHES];
	for (rapidjson::SizeType index = 0, mesh_data_array_size = mesh_data_array.Size(); index < mesh_data_array_size; ++index)
	{
		MeshData*   meshData = new (std::nothrow) MeshData();
		const rapidjson::Value& mesh_data = mesh_data_array[index];
		// mesh_vertex_attribute
		const rapidjson::Value& mesh_vertex_attribute = mesh_data[ATTRIBUTES];
		MeshVertexAttrib tempAttrib;
		meshData->attribCount = mesh_vertex_attribute.Size();		// add tangent and bitangent
		meshData->attribs.resize(meshData->attribCount);
		int oldVertexSize = 0;
		bool has_Tangent = false;
		for (rapidjson::SizeType i = 0, mesh_vertex_attribute_size = mesh_vertex_attribute.Size(); i < mesh_vertex_attribute_size; ++i)
		{
			const rapidjson::Value& mesh_vertex_attribute_val = mesh_vertex_attribute[i];

			int size = mesh_vertex_attribute_val[ATTRIBUTESIZE].GetInt(); oldVertexSize += size;
			std::string type = mesh_vertex_attribute_val[TYPE].GetString();
			std::string attribute = mesh_vertex_attribute_val[ATTRIBUTE].GetString();

			tempAttrib.size = size;
			tempAttrib.attribSizeBytes = sizeof(float) * size;
			tempAttrib.type = parseGLType(type);
			tempAttrib.vertexAttrib = parseGLProgramAttribute(attribute);
			meshData->attribs[i] = tempAttrib;

			if (attribute.compare("VERTEX_ATTRIB_TANGENT") == 0)
				has_Tangent = true;
		}

		// add tangent and binormal if not exist
		if (!has_Tangent)
		{
			meshData->attribCount += 2;

			MeshVertexAttrib tangentAttrib;
			// add tangent attribute
			tangentAttrib.size = 3; // vec3 tangent for each vertices
			tangentAttrib.attribSizeBytes = sizeof(float) * tangentAttrib.size;
			tangentAttrib.type = GL_FLOAT;
			tangentAttrib.vertexAttrib = GLProgram::VERTEX_ATTRIB_TANGENT;
			meshData->attribs.push_back(tangentAttrib);

			//
			// add bitangent attribute
			MeshVertexAttrib biAttrib;
			biAttrib.size = 3; // vec3 bitangent for each vertices
			biAttrib.attribSizeBytes = sizeof(float) * biAttrib.size;
			biAttrib.type = GL_FLOAT;
			biAttrib.vertexAttrib = GLProgram::VERTEX_ATTRIB_BINORMAL;
			meshData->attribs.push_back(biAttrib);
		}

		// mesh vertices
		////////////////////////////////////////////////////////////////////////////////////////////////
		const rapidjson::Value& mesh_data_vertex_array = mesh_data[VERTICES];
		auto mesh_data_vertex_array_size = mesh_data_vertex_array.Size();
		meshData->vertexSizeInFloat = mesh_data_vertex_array_size;
		for (rapidjson::SizeType i = 0; i < mesh_data_vertex_array_size; ++i)
		{
			meshData->vertex.push_back(mesh_data_vertex_array[i].GetDouble());

			if (!has_Tangent)
			{
				// add 6 vertices for tangent and binormal
				if ((i % oldVertexSize) == (oldVertexSize - 1))
				{
					for (int i = 0; i < 6; i++)
					{
						meshData->vertex.push_back(1.0f);
					}
				}
			}
			
		}


		int sizePerVertex = oldVertexSize + 2 * 3;		// bao nhieu float trong 1 vertex

		// mesh part
		////////////////////////////////////////////////////////////////////////////////////////////////
		const rapidjson::Value& mesh_part_array = mesh_data[PARTS];
		for (rapidjson::SizeType i = 0, mesh_part_array_size = mesh_part_array.Size(); i < mesh_part_array_size; ++i)
		{
			std::vector<unsigned short>      indexArray;
			const rapidjson::Value& mesh_part = mesh_part_array[i];
			meshData->subMeshIds.push_back(mesh_part[ID].GetString());
			// index_number
			const rapidjson::Value& indices_val_array = mesh_part[INDICES];
			for (rapidjson::SizeType j = 0, indices_val_array_size = indices_val_array.Size(); j < indices_val_array_size; ++j)
			{
				unsigned short idx = (unsigned short)indices_val_array[j].GetUint();
				indexArray.push_back(idx);
				if (has_Tangent && ((j % 3) == 2))
				{
					unsigned int idxStartV0 = sizePerVertex * (unsigned short)indices_val_array[j - 2].GetUint();
					Vec3 point0(meshData->vertex[idxStartV0], meshData->vertex[idxStartV0 + 1], meshData->vertex[idxStartV0 + 2]);
					Vec2 uv0(meshData->vertex[idxStartV0 + 6], meshData->vertex[idxStartV0 + 7]);

					unsigned int idxStartV1 = sizePerVertex * (unsigned short)indices_val_array[j - 1].GetUint();
					Vec3 point1(meshData->vertex[idxStartV1], meshData->vertex[idxStartV1 + 1], meshData->vertex[idxStartV1 + 2]);
					Vec2 uv1(meshData->vertex[idxStartV1 + 6], meshData->vertex[idxStartV1 + 7]);

					unsigned int idxStartV2 = sizePerVertex * (unsigned short)indices_val_array[j].GetUint();
					Vec3 point2(meshData->vertex[idxStartV2], meshData->vertex[idxStartV2 + 1], meshData->vertex[idxStartV2 + 2]);
					Vec2 uv2(meshData->vertex[idxStartV2 + 6], meshData->vertex[idxStartV2 + 7]);

					Vec3 tangent, binormal;

					calculateTangentBinormal(tangent, binormal, point0, uv0, point1, uv1, point2, uv2);

					// apply to buffer vertex

					meshData->vertex[idxStartV0 + oldVertexSize] = meshData->vertex[idxStartV1 + oldVertexSize] = meshData->vertex[idxStartV2 + oldVertexSize] = tangent.x;
					meshData->vertex[idxStartV0 + oldVertexSize + 1] = meshData->vertex[idxStartV1 + oldVertexSize + 1] = meshData->vertex[idxStartV2 + oldVertexSize + 1] = tangent.y;
					meshData->vertex[idxStartV0 + oldVertexSize + 2] = meshData->vertex[idxStartV1 + oldVertexSize + 2] = meshData->vertex[idxStartV2 + oldVertexSize + 2] = tangent.z;

					meshData->vertex[idxStartV0 + oldVertexSize + 3] = meshData->vertex[idxStartV1 + oldVertexSize + 3] = meshData->vertex[idxStartV2 + oldVertexSize + 3] = binormal.x;
					meshData->vertex[idxStartV0 + oldVertexSize + 4] = meshData->vertex[idxStartV1 + oldVertexSize + 4] = meshData->vertex[idxStartV2 + oldVertexSize + 4] = binormal.y;
					meshData->vertex[idxStartV0 + oldVertexSize + 5] = meshData->vertex[idxStartV1 + oldVertexSize + 5] = meshData->vertex[idxStartV2 + oldVertexSize + 5] = binormal.z;
				}
			}


			meshData->subMeshIndices.push_back(indexArray);
			meshData->numIndex = (int)meshData->subMeshIndices.size();

			if (mesh_data.HasMember(AABBS))
			{
				const rapidjson::Value& mesh_part_aabb = mesh_part[AABBS];
				if (mesh_part.HasMember(AABBS) && mesh_part_aabb.Size() == 6)
				{
					Vec3 min(mesh_part_aabb[(rapidjson::SizeType)0].GetDouble(),
						mesh_part_aabb[(rapidjson::SizeType)1].GetDouble(), mesh_part_aabb[(rapidjson::SizeType)2].GetDouble());
					Vec3 max(mesh_part_aabb[(rapidjson::SizeType)3].GetDouble(),
						mesh_part_aabb[(rapidjson::SizeType)4].GetDouble(), mesh_part_aabb[(rapidjson::SizeType)5].GetDouble());
					meshData->subMeshAABB.push_back(AABB(min, max));
				}
				else
				{
					meshData->subMeshAABB.push_back(calculateAABB(meshData->vertex, meshData->getPerVertexSize(), indexArray));
				}
			}
			else
			{
				meshData->subMeshAABB.push_back(calculateAABB(meshData->vertex, meshData->getPerVertexSize(), indexArray));
			}

		}
		meshdatas.meshDatas.push_back(meshData);
	}
	return true;
}


bool GBundle3D::loadMaterialsJson(GMaterialDatas& materialdatas)
{
	if (!_jsonReader.HasMember(MATERIALS))
		return false;
	const rapidjson::Value& material_array = _jsonReader[MATERIALS];

	for (rapidjson::SizeType i = 0; i < material_array.Size(); ++i)
	{
		GMaterialData materialData;
		const rapidjson::Value& material_val = material_array[i];
		materialData.id = material_val[ID].GetString();

		if (material_val.HasMember("ambient"))
		{
			const rapidjson::Value& ambientColor = material_val["ambient"];
			materialData.ambientColor = Vec3(ambientColor[0].GetDouble(), ambientColor[1].GetDouble(), ambientColor[2].GetDouble());
		}
		if (material_val.HasMember("diffuse"))
		{
			const rapidjson::Value& ambientColor = material_val["diffuse"];
			//materialData.diffuseColor = Vec3(ambientColor[0].GetFloat(), ambientColor[1].GetFloat(), ambientColor[2].GetFloat());
		}
		if (material_val.HasMember("emissive"))
		{
			const rapidjson::Value& ambientColor = material_val["emissive"];
			materialData.emissiveColor = Vec3(ambientColor[0].GetDouble(), ambientColor[1].GetDouble(), ambientColor[2].GetDouble());
		}
		if (material_val.HasMember("specular"))
		{
			const rapidjson::Value& ambientColor = material_val["specular"];
			materialData.specularColor = Vec3(ambientColor[0].GetDouble(), ambientColor[1].GetDouble(), ambientColor[2].GetDouble());
		}

		if (material_val.HasMember("opacity"))
		{
			materialData.opacity = material_val["opacity"].GetDouble();
		}
		if (material_val.HasMember("shininess"))
		{
			materialData.shinisess = material_val["shininess"].GetDouble();
		}
		

		if (material_val.HasMember(TEXTURES))
		{
			const rapidjson::Value& texture_array = material_val[TEXTURES];
			for (rapidjson::SizeType j = 0; j < texture_array.Size(); ++j)
			{
				NTextureData  textureData;
				const rapidjson::Value& texture_val = texture_array[j];
				std::string filename = texture_val[FILENAME].GetString();
				textureData.filename = filename.empty() ? filename : _modelPath + filename;
				textureData.type = parseGLTextureType(texture_val["type"].GetString());
				textureData.wrapS = parseGLType(texture_val["wrapModeU"].GetString());
				textureData.wrapT = parseGLType(texture_val["wrapModeV"].GetString());
				materialData.textures.push_back(textureData);

				if (textureData.type == NTextureData::Usage::Normal)
					hasNormalMap = true;
			}
		}
		materialdatas.materials.push_back(materialData);
	}
	return true;
}

bool GBundle3D::loadMaterialsBinary(GMaterialDatas& materialdatas)
{
	if (!seekToFirstType(BUNDLE_TYPE_MATERIAL))
		return false;
	unsigned int materialnum = 1;
	_binaryReader.read(&materialnum, 4, 1);
	for (unsigned int i = 0; i < materialnum; ++i)
	{
		GMaterialData materialData;
		materialData.id = _binaryReader.readString();

		// skip: diffuse(3), ambient(3), emissive(3), opacity(1), specular(3), shininess(1)
		float  data[14];
		_binaryReader.read(&data, sizeof(float), 14);

		materialData.diffuseColor = Vec3(data[0], data[1], data[2]);
		materialData.ambientColor = Vec3(data[3], data[4], data[5]);
		materialData.emissiveColor = Vec3(data[6], data[7], data[8]);
		materialData.opacity = data[9];
		materialData.specularColor = Vec3(data[10], data[11], data[12]);
		materialData.shinisess = data[13];

		unsigned int textureNum = 1;
		_binaryReader.read(&textureNum, 4, 1);
		for (unsigned int j = 0; j < textureNum; ++j)
		{
			NTextureData  textureData;
			textureData.id = _binaryReader.readString();
			if (textureData.id.empty())
			{
				CCLOG("warning: Failed to read Materialdata: texturePath is empty '%s'.", textureData.id.c_str());
				return false;
			}
			std::string texturePath = _binaryReader.readString();
			if (texturePath.empty())
			{
				CCLOG("warning: Failed to read Materialdata: texturePath is empty '%s'.", _path.c_str());
				return false;
			}

			textureData.filename = texturePath.empty() ? texturePath : _modelPath + texturePath;
			float  uvdata[4];
			_binaryReader.read(&uvdata, sizeof(float), 4);
			textureData.type = parseGLTextureType(_binaryReader.readString());
			textureData.wrapS = parseGLType(_binaryReader.readString());
			textureData.wrapT = parseGLType(_binaryReader.readString());
			materialData.textures.push_back(textureData);

			if (textureData.type == NTextureData::Usage::Normal)
				hasNormalMap = true;

		}
		materialdatas.materials.push_back(materialData);
	}
	return true;
}

bool  GBundle3D::loadMeshDatasBinaryCalTangent(MeshDatas& meshdatas)
{
	return loadMeshDatasBinary(meshdatas);

	if (!seekToFirstType(BUNDLE_TYPE_MESH))
		return false;
	unsigned int meshSize = 0;
	if (_binaryReader.read(&meshSize, 4, 1) != 1)
	{
		CCLOG("warning: Failed to read meshdata: attribCount '%s'.", _path.c_str());
		return false;
	}
	MeshData*   meshData = nullptr;
	for (unsigned int i = 0; i < meshSize; ++i)
	{
		unsigned int attribSize = 0;
		// read mesh data
		if (_binaryReader.read(&attribSize, 4, 1) != 1 || attribSize < 1)
		{
			CCLOG("warning: Failed to read meshdata: attribCount '%s'.", _path.c_str());
			goto FAILED;
		}
		meshData = new (std::nothrow) MeshData();
		meshData->attribCount = attribSize;
		meshData->attribs.resize(meshData->attribCount);
		int oldVertexSize = 0;
		bool has_Tangent = false;

		for (ssize_t j = 0; j < meshData->attribCount; ++j)
		{
			std::string attribute = "";
			unsigned int vSize;
			if (_binaryReader.read(&vSize, 4, 1) != 1)
			{
				CCLOG("warning: Failed to read meshdata: usage or size '%s'.", _path.c_str());
				goto FAILED;
			}
			oldVertexSize += vSize;
			std::string type = _binaryReader.readString();
			attribute = _binaryReader.readString();
			meshData->attribs[j].size = vSize;
			meshData->attribs[j].attribSizeBytes = meshData->attribs[j].size * 4;
			meshData->attribs[j].type = parseGLType(type);
			meshData->attribs[j].vertexAttrib = parseGLProgramAttribute(attribute);

			if (attribute.compare("VERTEX_ATTRIB_TANGENT") == 0)
				has_Tangent = true;

		}

		// add tangent and binormal if not exist
		if (!has_Tangent)
		{
			meshData->attribCount += 2;

			MeshVertexAttrib tangentAttrib;
			// add tangent attribute
			tangentAttrib.size = 3; // vec3 tangent for each vertices
			tangentAttrib.attribSizeBytes = sizeof(float) * tangentAttrib.size;
			tangentAttrib.type = GL_FLOAT;
			tangentAttrib.vertexAttrib = GLProgram::VERTEX_ATTRIB_TANGENT;
			meshData->attribs.push_back(tangentAttrib);

			//
			// add bitangent attribute
			MeshVertexAttrib biAttrib;
			biAttrib.size = 3; // vec3 bitangent for each vertices
			biAttrib.attribSizeBytes = sizeof(float) * biAttrib.size;
			biAttrib.type = GL_FLOAT;
			biAttrib.vertexAttrib = GLProgram::VERTEX_ATTRIB_BINORMAL;
			meshData->attribs.push_back(biAttrib);
		}
		//////////////////////////////////////

		

		unsigned int vertexSizeInFloat = 0;
		// Read vertex data
		if (_binaryReader.read(&vertexSizeInFloat, 4, 1) != 1 || vertexSizeInFloat == 0)
		{
			CCLOG("warning: Failed to read meshdata: vertexSizeInFloat '%s'.", _path.c_str());
			goto FAILED;
		}

		meshData->vertex.resize(vertexSizeInFloat);
		if (_binaryReader.read(&meshData->vertex[0], 4, vertexSizeInFloat) != vertexSizeInFloat)
		{
			CCLOG("warning: Failed to read meshdata: vertex element '%s'.", _path.c_str());
			goto FAILED;
		}

		if (!has_Tangent)
		{
			int sizePerVertex = oldVertexSize + 2 * 3;		// bao nhieu float trong 1 vertex

			unsigned int count = 0;
			float plus[] = { 1, 1, 1, 1, 1, 1 };
			while ((count + oldVertexSize) <= meshData->vertex.size())
			{		
				meshData->vertex.insert(meshData->vertex.begin() + (count + oldVertexSize), plus, plus + 6);
				count += (oldVertexSize + 6);
			}
			int a = 0;
		}

		// Read index data
		unsigned int meshPartCount = 1;
		_binaryReader.read(&meshPartCount, 4, 1);

		for (unsigned int k = 0; k < meshPartCount; ++k)
		{
			std::vector<unsigned short>      indexArray;
			std::string meshPartid = _binaryReader.readString();
			meshData->subMeshIds.push_back(meshPartid);
			unsigned int nIndexCount;
			if (_binaryReader.read(&nIndexCount, 4, 1) != 1)
			{
				CCLOG("warning: Failed to read meshdata: nIndexCount '%s'.", _path.c_str());
				goto FAILED;
			}
			indexArray.resize(nIndexCount);
			if (_binaryReader.read(&indexArray[0], 2, nIndexCount) != nIndexCount)
			{
				CCLOG("warning: Failed to read meshdata: indices '%s'.", _path.c_str());
				goto FAILED;
			}
			meshData->subMeshIndices.push_back(indexArray);
			meshData->numIndex = (int)meshData->subMeshIndices.size();
			//meshData->subMeshAABB.push_back(calculateAABB(meshData->vertex, meshData->getPerVertexSize(), indexArray));
			if (_version != "0.3" && _version != "0.4" && _version != "0.5")
			{
				//read mesh aabb
				float aabb[6];
				if (_binaryReader.read(aabb, 4, 6) != 6)
				{
					CCLOG("warning: Failed to read meshdata: aabb '%s'.", _path.c_str());
					goto FAILED;
				}
				meshData->subMeshAABB.push_back(AABB(Vec3(aabb[0], aabb[1], aabb[2]), Vec3(aabb[3], aabb[4], aabb[5])));
			}
			else
			{
				meshData->subMeshAABB.push_back(calculateAABB(meshData->vertex, meshData->getPerVertexSize(), indexArray));
			}

			if (!has_Tangent)
			{

			}
		}
		meshdatas.meshDatas.push_back(meshData);
	}
	return true;

FAILED:
	{
		CC_SAFE_DELETE(meshData);
		for (auto& meshdata : meshdatas.meshDatas) {
			delete meshdata;
		}
		meshdatas.meshDatas.clear();
		return false;
	}
}