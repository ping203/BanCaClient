#include "GMesh.h"
#include "GMainLight.h"
#include "renderer/CCMeshCommand.h"
#include "GShader.h"
#include "GEnvironment.h"
std::string s_uniformSamplerNameFull[] =
{
	"",//NTextureData::Usage::Unknown,
	"",//NTextureData::Usage::None
	"",//NTextureData::Usage::Diffuse
	"",//NTextureData::Usage::Emissive
	"",//NTextureData::Usage::Ambient
	"u_specularTex",//NTextureData::Usage::Specular
	"",//NTextureData::Usage::Shininess
	"u_normalTex",//NTextureData::Usage::Normal
	"",//NTextureData::Usage::Bump
	"",//NTextureData::Usage::Transparency
	"",//NTextureData::Usage::Reflection
};


GMesh::GMesh() : Mesh()
{
	g_material = NULL;
	hasShadow = false;
	hasNormal = false;
	hasSpecular = false;

	depthProgramState = GLProgramState::create(GShader::getInstance()->getGLProgram(GShader::G_SHADER_DEPTH));
	depthMaterial = GMaterial::createWithGLProgramState(depthProgramState);
	depthMaterial->retain();

	mainProgramState = NULL;
}


GMesh::~GMesh()
{
	delete depthMaterial;
}

GMesh* GMesh::create(const std::string& name, MeshIndexData* indexData, MeshSkin* skin)
{
	auto state = new (std::nothrow) GMesh();
	state->autorelease();
	state->bindMeshCommand();
	state->_name = name;
	state->setMeshIndexData(indexData);
	state->setSkin(skin);

	return state;
}

void GMesh::setMaterial(GMaterial *material)
{
	g_material = material;
	Mesh::setMaterial(material);

	if (g_material)
	{
		for (auto technique : g_material->getTechniques())
		{
			for (auto pass : technique->getPasses())
			{
				//auto vertexAttribBinding = VertexAttribBinding::create(_meshIndexData, pass->getGLProgramState());
				//pass->setVertexAttribBinding(vertexAttribBinding);

				auto programState = pass->getGLProgramState();

				// material
				programState->setUniformFloat("material_shininess", g_material->shinisess);
				//programState->setUniformVec3("material_ambient",Vec3(0,0,0));
				//programState->setUniformVec3("material_diffuse", g_material->diffuseColor);
				programState->setUniformVec3("material_specular", g_material->specularColor);

				programState->setUniformInt("u_hasSkin", _skin ? 1 : 0);


				programState->setUniformVec3("light_position", GMainLight::POSITION);
				programState->setUniformVec3("light_intensities", GMainLight::INTENSITIES);
				programState->setUniformFloat("light_ambient_coefficient", GMainLight::AMBIENT_COEFFICIENT);
				//programState->setUniformFloat("light_attenuation", GMainLight::ATTENUATION);

				programState->setUniformFloat("spot_light_cullOff", GMainLight::cullOff);
				programState->setUniformFloat("spot_light_cutOff", GMainLight::cutOff);

				programState->setUniformFloat("light_specular", GMainLight::specular);

// 				Vec3 viewPos; Camera::getVisitingCamera()->getNodeToWorldTransform().getTranslation(&viewPos);
// 				programState->setUniformVec3("u_viewPosition", viewPos
			}
		}
	}
}

void GMesh::setMaterial(GMaterial *material, bool inDepth)
{
	if (_material != material) {
		CC_SAFE_RELEASE(_material);
		_material = material;
		CC_SAFE_RETAIN(_material);
	}
	g_material = material;

	if (_material)
	{
		for (auto technique : _material->getTechniques())
		{
			for (auto pass : technique->getPasses())
			{
				auto vertexAttribBinding = VertexAttribBinding::create(_meshIndexData, pass->getGLProgramState());
				pass->setVertexAttribBinding(vertexAttribBinding);

				auto programState = pass->getGLProgramState();

				// material
				programState->setUniformFloat("material_shininess", g_material->shinisess);
				//programState->setUniformVec3("material_ambient",Vec3(0,0,0));
				//programState->setUniformVec3("material_diffuse", g_material->diffuseColor);
				programState->setUniformVec3("material_specular", g_material->specularColor);

				programState->setUniformFloat("spot_light_cullOff", GMainLight::cullOff);
				programState->setUniformFloat("spot_light_cutOff", GMainLight::cutOff);
			}
		}
	}
	

	if (_blendDirty)
		setBlendFunc(_blend);

	bindMeshCommand();

	if (inDepth)
		return;

	// Was the texture set before the GLProgramState ? Set it
// 	for (auto& tex : _textures)
// 		setTexture(tex.second, tex.first);
	setTexture(_texture, NTextureData::Usage::Diffuse);



}
// 
// void GMesh::setTexture(const std::string& texPath)
// {
// 	auto tex = Director::getInstance()->getTextureCache()->addImage(texPath);
// 	setTexture(tex, NTextureData::Usage::Diffuse);
// }

void GMesh::setGLProgramState(GLProgramState* glProgramState)
{
	mainProgramState = glProgramState;
	auto material = GMaterial::createWithGLProgramState(glProgramState);
	if (g_material)
		material->setStateBlock(g_material->getStateBlock());
	this->setMaterial(g_material);
}

void GMesh::onDraw(Renderer* renderer, float globalZOrder, const Mat4& transform, uint32_t flags, unsigned int lightMask, const Vec4& color, bool forceDepthWrite)
{
	if (!isVisible())
		return;

	bool isTransparent = (_isTransparent || color.w < 1.f);
	float globalZ = isTransparent ? 0 : globalZOrder;
	if (isTransparent)
		flags |= Node::FLAGS_RENDER_AS_3D;

	
	bool inDepth = GEnvironment::getInstance()->isEnabledShadow() && (Camera::getVisitingCamera()->getCameraFlag() == GEnvironment::DEPTH_CAM_FLAG);
	GMaterial *materialCurrent = g_material;
// 	if (!materialCurrent)
// 		return;
 	//setMaterial(materialCurrent, inDepth);

	_meshCommand.init(globalZ,
		materialCurrent,
		getVertexBuffer(),
		getIndexBuffer(),
		getPrimitiveType(),
		getIndexFormat(),
		getIndexCount(),
		transform,
		flags);

	if (isTransparent && !forceDepthWrite)
		materialCurrent->getStateBlock()->setDepthWrite(false);
	else
		materialCurrent->getStateBlock()->setDepthWrite(true);


	_meshCommand.setSkipBatching(isTransparent);
	_meshCommand.setTransparent(isTransparent);
	_meshCommand.set3D(!_force2DQueue);
	materialCurrent->getStateBlock()->setBlend(_force2DQueue || isTransparent);

	// set default uniforms for Mesh
	// 'u_color' and others
	const auto scene = Director::getInstance()->getRunningScene();
	auto technique = materialCurrent->_currentTechnique;
	for (const auto pass : technique->getPasses())
	{
		auto programState = pass->getGLProgramState();
		
		if (_skin)
		{
			programState->setUniformVec4v("u_matrixPalette", (GLsizei)_skin->getMatrixPaletteSize(), _skin->getMatrixPalette());
		}
		
				
		if (!inDepth)
		{
			programState->setUniformVec4("u_color", color);
			//programState->setUniformInt("u_hasShadow", hasShadow ? 1 : 0);

			// add Light
			if (GMainLight::dirtyFlag)
			{
				programState->setUniformVec3("light_position", GMainLight::POSITION);
				programState->setUniformVec3("light_intensities", GMainLight::INTENSITIES);
				programState->setUniformFloat("light_ambient_coefficient", GMainLight::AMBIENT_COEFFICIENT);
				//programState->setUniformFloat("light_attenuation", GMainLight::ATTENUATION);

				programState->setUniformFloat("spot_light_cullOff", GMainLight::cullOff);
				programState->setUniformFloat("spot_light_cutOff", GMainLight::cutOff);

				programState->setUniformFloat("light_specular", GMainLight::specular);
			}

			//material
			//programState->setUniformFloat("material_shininess", materialCurrent->shinisess);
			//programState->setUniformVec3("material_ambient",Vec3(0,0,0));
			//programState->setUniformVec3("material_diffuse", g_material->diffuseColor);
			//programState->setUniformVec3("material_specular", materialCurrent->specularColor);

			// camera pos
			if (GEnvironment::FLAG_UPDATE_CAM)
			{
				Vec3 viewPos; Camera::getVisitingCamera()->getNodeToWorldTransform().getTranslation(&viewPos);
				programState->setUniformVec3("u_viewPosition", viewPos);
			}
			
			// model matrix
			//programState->setUniformMat4("u_VPMatrix", Camera::getVisitingCamera()->getViewProjectionMatrix());

			// shadow
// 			if (hasShadow && GEnvironment::getInstance()->isEnabledShadow())
// 			{
// 				programState->setUniformMat4("u_LightMatrix", GEnvironment::VP_LIGHT_MATRIX);		// VP Light space matrix
// 				programState->setUniformTexture("u_shadowTex", GEnvironment::DEPTH_TEX);
// 				programState->setUniformMat4("u_MMatrix", transform);							// Model matrix
// 			}
		}
		else
		{
// 			programState->setUniformMat4("u_MMatrix", transform);							// Model matrix
// 			programState->setUniformMat4("u_VPLightMatrix", GEnvironment::VP_LIGHT_MATRIX);		// VP Light space matrix
		}	
	}

	renderer->addCommand(&_meshCommand);
}

void GMesh::receivedShadow(bool ret)
{
	hasShadow = ret;
}

void GMesh::setTexture(const std::string& texPath, NTextureData::Usage usage /* = NTextureData::Usage::Diffuse */)
{
	auto tex = Director::getInstance()->getTextureCache()->addImage(texPath);
	setTexture(tex, usage);
}


void GMesh::setTexture(Texture2D* tex, NTextureData::Usage usage, bool cacheFileName)
{
	// Texture must be saved for future use
	// it doesn't matter if the material is already set or not
	// This functionality is added for compatibility issues
	CCASSERT(tex != NULL, "Texture not be NULL");

	CC_SAFE_RETAIN(tex);


	if (usage == NTextureData::Usage::Diffuse){ 
		CC_SAFE_RELEASE(_texture);
		_texture = tex;
		if (g_material) {
			auto technique = g_material->_currentTechnique;
			for (auto& pass : technique->getPasses())
			{
				// FIXME: Ideally it should use glProgramState->setUniformTexture()
				// and set CC_Texture0 that way. But trying to it, will trigger
				// another bug
				pass->setTexture(tex);
			}
		}

		bindMeshCommand();
// 		if (cacheFileName)
// 			_texFile = tex->getPath();
	}
	else if (usage == NTextureData::Usage::Specular) 
	{
		if (g_material){
			auto technique = g_material->_currentTechnique;
			for (auto& pass : technique->getPasses())
			{
				pass->getGLProgramState()->setUniformTexture(s_uniformSamplerNameFull[(int)usage], tex);
			}
			hasSpecular = true;
		}
	}

	else if (usage == NTextureData::Usage::Normal) 
	{
		if (g_material){
			auto technique = g_material->_currentTechnique;
			for (auto& pass : technique->getPasses())
			{
				pass->getGLProgramState()->setUniformTexture(s_uniformSamplerNameFull[(int)usage], tex);
			}
			hasNormal = true;
		}
	}
}