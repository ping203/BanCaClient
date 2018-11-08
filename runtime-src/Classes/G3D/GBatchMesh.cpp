#include "GBatchMesh.h"
#include "renderer/CCGLProgramState.h"
#include "base/CCDirector.h"
#include "renderer/ccGLStateCache.h"
#include "base/ccMacros.h"


GBatchMesh::GBatchMesh()
{
}


GBatchMesh::~GBatchMesh()
{
}

GBatchMesh * GBatchMesh::create(const MeshData &meshData)
{
	GBatchMesh *p = new GBatchMesh();
	p->meshData = meshData;
	p->init();

	return p;
}

// float vertices[] = {
// 	0.5f, 0.5f, 0.0f,  // top right
// 	0.5f, -0.5f, 0.0f,  // bottom right
// 	-0.5f, -0.5f, 0.0f,  // bottom left
// 	-0.5f, 0.5f, 0.0f   // top left 
// };
// unsigned int indices[] = {  // note that we start from 0!
// 	0, 1, 3,   // first triangle
// 	1, 2, 3    // second triangle
// };


Texture2D *tex;
bool GBatchMesh::init()
{
	Node::init();
	setGLProgramState(GLProgramState::create(GLProgram::createWithFilenames("shaders/test.vert", "shaders/test.frag")));
	_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

	int t = sizeof(unsigned short);
	GLfloat *vertices = &meshData.vertex[0];
	GLushort *indices = &meshData.subMeshIndices[0][0];

	tex = Director::getInstance()->getTextureCache()->addImage("sin/thamdiff3.jpg");
	tex->retain();
	
	glMapBuffer(1, 1);


	glGenVertexArrays(1, &_vao);
	GL::bindVAO(_vao);

	glGenBuffers(2, &_vbos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);

	glBufferData(GL_ARRAY_BUFFER, meshData.vertex.size() * sizeof(float), vertices, GL_STATIC_DRAW);


	// vertices
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_N3F_T2F), (GLvoid*)0);
	// normal
 	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_NORMAL);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_N3F_T2F), (GLvoid*)offsetof(V3F_N3F_T2F, normal));
// // 
// 	// tex coords
 	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
 	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_N3F_T2F), (GLvoid*)offsetof(V3F_N3F_T2F, texCoord));
// 
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[1]);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.subMeshIndices[0].size() * sizeof(GLushort), indices, GL_STATIC_DRAW);

	// Must unbind the VAO before changing the element buffer.
	GL::bindVAO(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	return true;
}

void GBatchMesh::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_command.init(_globalZOrder, transform, flags);
	_command.func = CC_CALLBACK_0(GBatchMesh::onDraw, this, transform, flags);
	renderer->addCommand(&_command);


}

void GBatchMesh::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);
	getGLProgramState()->setUniformTexture("u_shadowTex", tex);
	glEnable(GL_DEPTH_TEST);
	RenderState::StateBlock::_defaultState->setDepthTest(true);
	GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	GL::bindVAO(_vao);
	glDrawElements(GL_TRIANGLES, meshData.subMeshIndices[0].size(), GL_UNSIGNED_SHORT, (GLvoid*)0);
	//glDrawElementsInstanced(GL_TRIANGLES, meshData.subMeshIndices[0].size(), GL_UNSIGNED_SHORT, (GLvoid *)0, 2);


	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, meshData.subMeshIndices[0].size());
	CHECK_GL_ERROR_DEBUG();

	
}
