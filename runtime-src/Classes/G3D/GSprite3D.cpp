#include "GSprite3D.h"
#include "GBundle3D.h"
#include "GMaterial.h"
#include "GMesh.h"
#include "GShader.h"

Texture2D *genTexture(const NTextureData* textureData)
{
	auto tex = Director::getInstance()->getTextureCache()->addImage(textureData->filename);
	if (tex)
	{
		Texture2D::TexParams texParams;
		texParams.minFilter = GL_LINEAR;
		texParams.magFilter = GL_LINEAR;
		texParams.wrapS = textureData->wrapS;
		texParams.wrapT = textureData->wrapT;
		tex->setTexParameters(texParams);
	}
	return tex;
}


GSprite3D::GSprite3D() : Sprite3D()
{
	hasReceivedShadow = false;
}


GSprite3D::~GSprite3D() 
{
}

GSprite3D* GSprite3D::create(string modelPath,bool shadow)
{
	CCASSERT(modelPath.length() >= 4, "invalid filename for Sprite3D");

	auto sprite = new (std::nothrow) GSprite3D();
	if (sprite && sprite->initFromPath(modelPath,shadow))
	{
		sprite->_contentSize = sprite->getBoundingBox().size;
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

bool GSprite3D::initFromPath(string modelPath, bool shadow)
{
	Node::init();
	hasReceivedShadow = shadow;
	_aabbDirty = true;
	_meshes.clear();
	_meshVertexDatas.clear();
	CC_SAFE_RELEASE_NULL(_skeleton);
	removeAllAttachNode();

	if (shadow)
	{
		CCLOG("%s", modelPath.c_str());
	}

	if (loadFromCache(modelPath))
		return true;

	MeshDatas* meshdatas = new (std::nothrow) MeshDatas();
	GMaterialDatas* materialdatas = new (std::nothrow) GMaterialDatas();
	NodeDatas* nodeDatas = new (std::nothrow) NodeDatas();
	if (loadFromFile(modelPath, nodeDatas, meshdatas, materialdatas))
	{


		if (initFrom(*nodeDatas, *meshdatas, *materialdatas))
		{
			//add to cache
			auto data = new (std::nothrow) GSprite3DCache::GSprite3DData();
			data->materialdatas = materialdatas;
			data->nodedatas = nodeDatas;
			data->meshVertexDatas = _meshVertexDatas;
			for (const auto mesh : _meshes) {
				data->glProgramStates.pushBack(mesh->getGLProgramState());
			}

			GSprite3DCache::getInstance()->addSprite3DData(modelPath, data);
			CC_SAFE_DELETE(meshdatas);
			_contentSize = getBoundingBox().size;
			return true;
		}
	}
	CC_SAFE_DELETE(meshdatas);
	CC_SAFE_DELETE(materialdatas);
	CC_SAFE_DELETE(nodeDatas);

	return false;
}

bool GSprite3D::loadFromCache(const std::string& path)
{
	auto spritedata = GSprite3DCache::getInstance()->getSpriteData(path);
	if (spritedata)
	{
		for (auto it : spritedata->meshVertexDatas) {
			_meshVertexDatas.pushBack(it);
		}
		_skeleton = Skeleton3D::create(spritedata->nodedatas->skeleton);
		CC_SAFE_RETAIN(_skeleton);

		const bool singleSprite = (spritedata->nodedatas->nodes.size() == 1);
		for (const auto& it : spritedata->nodedatas->nodes)
		{
			if (it)
			{
				createNode(it, this, *(spritedata->materialdatas), singleSprite);
			}
		}

		for (const auto& it : spritedata->nodedatas->skeleton)
		{
			if (it)
			{
				createAttachSprite3DNode(it, *(spritedata->materialdatas));
			}
		}

		for (ssize_t i = 0, size = _meshes.size(); i < size; ++i) {
			// cloning is needed in order to have one state per sprite
			auto glstate = spritedata->glProgramStates.at(i);
			_meshes.at(i)->setGLProgramState(glstate->clone());
		}
		return true;
	}

	return false;
}

bool GSprite3D::loadFromFile(const std::string& path, NodeDatas* nodedatas, MeshDatas* meshdatas, GMaterialDatas* materialdatas)
{
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);

	std::string ext = FileUtils::getInstance()->getFileExtension(path);
	if (ext == ".c3b" || ext == ".c3t")
	{
		//load from .c3b or .c3t
		auto bundle = GBundle3D::createInstance(path);
		if (!bundle)
		{
			return false;
		}

		auto ret =  bundle->loadMaterials(*materialdatas) && bundle->loadMeshDatas(*meshdatas) && bundle->loadNodes(*nodedatas);  // load material first to check normal map
		delete bundle;

		return ret;
	}
	return false;
}


bool GSprite3D::initFrom(const NodeDatas& nodeDatas, const MeshDatas& meshdatas, const GMaterialDatas& materialdatas)
{
	for (const auto& it : meshdatas.meshDatas)
	{
		if (it)
		{
			//            Mesh* mesh = Mesh::create(*it);
			//            _meshes.pushBack(mesh);
			auto meshvertex = MeshVertexData::create(*it);
			_meshVertexDatas.pushBack(meshvertex);
		}
	}
	_skeleton = Skeleton3D::create(nodeDatas.skeleton);
	CC_SAFE_RETAIN(_skeleton);

	auto size = nodeDatas.nodes.size();
	for (const auto& it : nodeDatas.nodes)
	{
		if (it)
		{
			createNode(it, this, materialdatas, size == 1);
		}
	}
	for (const auto& it : nodeDatas.skeleton)
	{
		if (it)
		{
			createAttachSprite3DNode(it, materialdatas);
		}
	}
	
	return true;
}

GLProgramState* createGLProgramState(bool hasShadow, NTextureData::Usage usage)
{
	switch (usage)
	{
	case cocos2d::NTextureData::Usage::Normal:
		return GLProgramState::create(GShader::getInstance()->getGLProgram(GShader::G_SHADER_DIFFUSE));
	default:
		return GLProgramState::create(GShader::getInstance()->getGLProgram(GShader::G_SHADER_DIFFUSE));
	}
}

void GSprite3D::createAttachSprite3DNode(NodeData* nodedata, const GMaterialDatas& materialdatas)
{
	for (const auto& it : nodedata->modelNodeDatas)
	{
		if (it && getAttachNode(nodedata->id))
		{
			auto sprite = createSprite3DNode(nodedata, it, materialdatas);
			if (sprite)
			{
				getAttachNode(nodedata->id)->addChild(sprite);
			}
		}
	}
	for (const auto& it : nodedata->children)
	{
		createAttachSprite3DNode(it, materialdatas);
	}
}

void GSprite3D::createNode(NodeData* nodedata, Node* root, const GMaterialDatas& materialdatas, bool singleSprite)
{
	Node* node = nullptr;
	for (const auto& it : nodedata->modelNodeDatas)	// add 1 prite 3D con vao` node root
	{
		if (it)
		{
			if (it->bones.size() > 0 || singleSprite)
			{
				if (root != nullptr)
					root->setName(nodedata->id);
				auto mesh = GMesh::create(nodedata->id, getMeshIndexData(it->subMeshId),NULL);
				mesh->hasShadow = hasReceivedShadow;
				GLProgramState *programState = NULL;
				//_meshes.pushBack(mesh);
				if (mesh)
				{
					_meshes.pushBack(mesh);  
					if (_skeleton && it->bones.size()) 
					{
						auto skin = MeshSkin::create(_skeleton, it->bones, it->invBindPose);
						mesh->setSkin(skin);
					}
					mesh->_visibleChanged = std::bind(&GSprite3D::onAABBDirty, this);
					const GMaterialData* materialData = materialdatas.getMaterialData(it->materialId); // material cho mesh nay`
					if (it->materialId == "" && materialdatas.materials.size())
					{
						const NTextureData* textureData = materialdatas.materials[0].getTextureData(NTextureData::Usage::Diffuse);
						programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Diffuse);
						mesh->setMaterial(genMaterialSingle((GMaterialData *)materialData, programState));
						mesh->setGLProgramState(programState);
						mesh->setTexture(textureData->filename);
					}
					else
					{
						if (materialData)
						{
							const NTextureData* textureDataDiffuse = materialData->getTextureData(NTextureData::Usage::Diffuse);
							const NTextureData* textureDataSpecular = materialData->getTextureData(NTextureData::Usage::Specular);
							const NTextureData* textureDataNormal = materialData->getTextureData(NTextureData::Usage::Normal);

							if (textureDataNormal && textureDataDiffuse)
							{
								programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Normal);
								mesh->setMaterial(genMaterialSingle((GMaterialData *)materialData, programState));
								mesh->setGLProgramState(programState);

								auto tex = genTexture(textureDataDiffuse);
								mesh->setTexture(tex, NTextureData::Usage::Diffuse);
								mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);

								tex = genTexture(textureDataNormal);
								mesh->setTexture(tex, NTextureData::Usage::Normal);
							}
							else if (textureDataDiffuse)
							{
								programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Diffuse);
								mesh->setMaterial(genMaterialSingle((GMaterialData *)materialData, programState));
								mesh->setGLProgramState(programState);
								auto tex = genTexture(textureDataDiffuse);
								mesh->setTexture(tex, NTextureData::Usage::Diffuse);
								mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);
							}
							if (textureDataSpecular && programState)
							{
								auto tex = genTexture(textureDataSpecular);
								mesh->setTexture(tex, NTextureData::Usage::Specular);
							}

						}
					}

					Vec3 pos;
					Quaternion qua;
					Vec3 scale;
					nodedata->transform.decompose(&scale, &qua, &pos);
					setPosition3D(pos);
					setRotationQuat(qua);
					setScaleX(scale.x);
					setScaleY(scale.y);
					setScaleZ(scale.z);
					setGLProgramState(programState);
					node = this;
				}
			}
			else
			{
				auto sprite = createSprite3DNode(nodedata, it, materialdatas);
				if (sprite)
				{
					if (root)
					{
						root->addChild(sprite);
					}
				}
				node = sprite;
			}
		}
	}
	if (nodedata->modelNodeDatas.size() == 0)
	{
		node = Node::create();
		if (node)
		{
			node->setName(nodedata->id);

			// set locale transform
			Vec3 pos;
			Quaternion qua;
			Vec3 scale;
			nodedata->transform.decompose(&scale, &qua, &pos);
			node->setPosition3D(pos);
			node->setRotationQuat(qua);
			node->setScaleX(scale.x);
			node->setScaleY(scale.y);
			node->setScaleZ(scale.z);

			if (root)
			{
				root->addChild(node);
			}
		}
	}

	auto size = nodedata->children.size();
	for (const auto& it : nodedata->children)
	{
		createNode(it, node, materialdatas, size == 1);
	}
}


void GSprite3D::setTexture(const std::string& texFile)
{
	//auto tex = Director::getInstance()->getTextureCache()->addImage(texFile);
	for (auto mesh : _meshes) {
		((GMesh *)mesh)->setTexture(texFile);
	}
	//Sprite3D::setTexture(texFile);
}


GSprite3D* GSprite3D::createSprite3DNode(NodeData* nodedata, ModelData* modeldata, const GMaterialDatas& materialdatas)
{
	auto sprite = new (std::nothrow) GSprite3D();
	if (sprite)
	{
		sprite->setName(nodedata->id);
		auto mesh = GMesh::create(nodedata->id, getMeshIndexData(modeldata->subMeshId),NULL);
		//_meshes.pushBack(mesh);
		mesh->hasShadow = hasReceivedShadow;
		GLProgramState *programState = NULL;
		
		
		if (_skeleton && modeldata->bones.size())
		{
			auto skin = MeshSkin::create(_skeleton, modeldata->bones, modeldata->invBindPose);
			mesh->setSkin(skin);
		}

		if (modeldata->materialId == "" && materialdatas.materials.size())
		{
			const NTextureData* textureData = materialdatas.materials[0].getTextureData(NTextureData::Usage::Diffuse);
			
			programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Diffuse);
			sprite->genMaterial((GMaterialData *)materialdatas.getMaterialData(modeldata->materialId), programState);
			mesh->setTexture(textureData->filename);
		}
		else
		{
			const GMaterialData* materialData = materialdatas.getMaterialData(modeldata->materialId); // material cho mesh nay`
			if (materialData)
			{
				const NTextureData* textureDataDiffuse = materialData->getTextureData(NTextureData::Usage::Diffuse);
				const NTextureData* textureDataSpecular = materialData->getTextureData(NTextureData::Usage::Specular);
				const NTextureData* textureDataNormal = materialData->getTextureData(NTextureData::Usage::Normal);

				if (textureDataNormal && textureDataDiffuse)
				{
					programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Normal);
					mesh->setMaterial(genMaterialSingle((GMaterialData *)materialData, programState));
					mesh->setGLProgramState(programState);

					auto tex = genTexture(textureDataDiffuse);
					mesh->setTexture(tex, NTextureData::Usage::Diffuse);
					mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);

					tex = genTexture(textureDataNormal);
					mesh->setTexture(tex, NTextureData::Usage::Normal);
				}
				else if (textureDataDiffuse)
				{
					programState = createGLProgramState(hasReceivedShadow, NTextureData::Usage::Diffuse);
					mesh->setMaterial(genMaterialSingle((GMaterialData *)materialData, programState));
					mesh->setGLProgramState(programState);
					auto tex = genTexture(textureDataDiffuse);
					mesh->setTexture(tex, NTextureData::Usage::Diffuse);
					mesh->_isTransparent = (materialData->getTextureData(NTextureData::Usage::Transparency) != nullptr);
				}
				else
				{
						
				}
				if (textureDataSpecular && programState)
				{
					auto tex = genTexture(textureDataSpecular);
					mesh->setTexture(tex, NTextureData::Usage::Specular);
				}

			}
		}

		// set locale transform
		Vec3 pos;
		Quaternion qua;
		Vec3 scale;
		nodedata->transform.decompose(&scale, &qua, &pos);
		sprite->setPosition3D(pos);
		sprite->setRotationQuat(qua);
		sprite->setScaleX(scale.x);
		sprite->setScaleY(scale.y);
		sprite->setScaleZ(scale.z);
		sprite->addMesh(mesh);
		sprite->autorelease();
	}
	return   sprite;
}

void GSprite3D::genMaterial(GMaterialData* materialdatas,GLProgramState *state)
{
	if (!materialdatas)
		return;
	for (auto mesh : _meshes)
	{
		GMaterial *material = GMaterial::createWithGLProgramState(state);;
		material->id = materialdatas->id;
		material->ambientColor = materialdatas->ambientColor;
		material->diffuseColor = materialdatas->diffuseColor;
		material->specularColor = materialdatas->specularColor;
		material->shinisess = materialdatas->shinisess;
		material->emissiveColor = materialdatas->emissiveColor;
		material->opacity = materialdatas->opacity;
		material->hasNormalMap = materialdatas->getTextureData(NTextureData::Usage::Normal) != NULL;

		((GMesh *)mesh)->setMaterial(material);
	}
}

GMaterial* GSprite3D::genMaterialSingle(GMaterialData* materialdatas, GLProgramState *state)
{
	if (!materialdatas)
		return NULL;
	GMaterial *material = GMaterial::createWithGLProgramState(state);;
	material->id = materialdatas->id;
	material->ambientColor = materialdatas->ambientColor;
	material->diffuseColor = materialdatas->diffuseColor;
	material->specularColor = materialdatas->specularColor;
	material->shinisess = materialdatas->shinisess;
	material->emissiveColor = materialdatas->emissiveColor;
	material->opacity = materialdatas->opacity;
	material->hasNormalMap = materialdatas->getTextureData(NTextureData::Usage::Normal) != NULL;

	return material;
}


void GSprite3D::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
	// camera clipping
	if (_children.size() == 0 && Camera::getVisitingCamera() && !Camera::getVisitingCamera()->isVisibleInFrustum(&getAABB()))
		return;
#endif

	if (_skeleton)
		_skeleton->updateBoneMatrix();

	Color4F color(getDisplayedColor());
	color.a = getDisplayedOpacity() / 255.0f;

	//check light and determine the shader used
	const auto& scene = Director::getInstance()->getRunningScene();

	for (auto mesh : _meshes)
	{
		((GMesh *)mesh)->onDraw(renderer,
			_globalZOrder,
			transform,
			flags,
			_lightMask,
			Vec4(color.r, color.g, color.b, color.a),
			_forceDepthWrite);

	}
}

///////////////////////////////////////////////////////////////////////////////////
GSprite3DCache* GSprite3DCache::_cacheInstanceG = nullptr;
GSprite3DCache* GSprite3DCache::getInstance()
{
	if (_cacheInstanceG == nullptr)
		_cacheInstanceG = new (std::nothrow) GSprite3DCache();
	return _cacheInstanceG;
}
void GSprite3DCache::destroyInstance()
{
	if (_cacheInstanceG)
	{
		delete _cacheInstanceG;
		_cacheInstanceG = nullptr;
	}
}

GSprite3DCache::GSprite3DData* GSprite3DCache::getSpriteData(const std::string& key) const
{
	auto it = _spriteDatas.find(key);
	if (it != _spriteDatas.end())
		return it->second;
	return nullptr;
}

bool GSprite3DCache::addSprite3DData(const std::string& key, GSprite3DCache::GSprite3DData* spritedata)
{
	auto it = _spriteDatas.find(key);
	if (it == _spriteDatas.end())
	{
		_spriteDatas[key] = spritedata;
		return true;
	}
	return false;
}

void GSprite3DCache::removeSprite3DData(const std::string& key)
{
	auto it = _spriteDatas.find(key);
	if (it != _spriteDatas.end())
	{
		delete it->second;
		_spriteDatas.erase(it);
	}
}

void GSprite3DCache::removeAllSprite3DData()
{
	for (auto& it : _spriteDatas) {
		delete it.second;
	}
	_spriteDatas.clear();
}

GSprite3DCache::GSprite3DCache()
{

}
GSprite3DCache::~GSprite3DCache()
{
	removeAllSprite3DData();
}

//
// MARK: Helpers
//
static GMaterial* getSprite3DMaterialForAttribs(MeshVertexData* meshVertexData, bool usesLight)
{
// 	bool textured = meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_TEX_COORD);
// 	bool hasSkin = meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_BLEND_INDEX)
// 		&& meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
// 	bool hasNormal = meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_NORMAL);
// 	bool hasTangentSpace = meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_TANGENT)
// 		&& meshVertexData->hasVertexAttrib(GLProgram::VERTEX_ATTRIB_BINORMAL);
// 	Sprite3DMaterial::MaterialType type;
// 	if (textured)
// 	{
// 		if (hasTangentSpace){
// 			type = hasNormal && usesLight ? Sprite3DMaterial::MaterialType::BUMPED_DIFFUSE : Sprite3DMaterial::MaterialType::UNLIT;
// 		}
// 		else{
// 			type = hasNormal && usesLight ? Sprite3DMaterial::MaterialType::DIFFUSE : Sprite3DMaterial::MaterialType::UNLIT;
// 		}
// 	}
// 	else
// 	{
// 		type = hasNormal && usesLight ? Sprite3DMaterial::MaterialType::DIFFUSE_NOTEX : Sprite3DMaterial::MaterialType::UNLIT_NOTEX;
// 	}
// 
// 	return Sprite3DMaterial::createBuiltInMaterial(type, hasSkin);
	return NULL;
}