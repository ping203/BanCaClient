#include "GShader.h"


GShader::GShader()
{
}


GShader::~GShader()
{
}

GShader *g_Ins = NULL;
GShader *GShader::getInstance()
{
	if (g_Ins == NULL)
	{
		g_Ins = new GShader();
		g_Ins->initShader();

	}
	return g_Ins;
}

void GShader::initShader()
{
	GLProgram *program = GLProgram::createWithFilenames("shaders/diffuse.vert", "shaders/diffuse.frag");
	mapGL.insert(make_pair(G_SHADER_DIFFUSE, program));

// 	program = GLProgram::createWithFilenames("shaders/normal_mapping.vert", "shaders/normal_mapping.frag");
// 	program->bindAttribLocation("a_tangent", GLProgram::VERTEX_ATTRIB_TANGENT);
// 	program->bindAttribLocation("a_binormal", GLProgram::VERTEX_ATTRIB_BINORMAL);
// 	mapGL.insert(make_pair(G_SHADER_NORMAL_MAP, program));

// 	program = GLProgram::createWithFilenames("shaders/shadow.vert", "shaders/shadow.frag");
// 	mapGL.insert(make_pair(G_SHADER_SHADOW, program));  

	program = GLProgram::createWithFilenames("shaders/depth.vert", "shaders/depth.frag");
	mapGL.insert(make_pair(G_SHADER_DEPTH, program));
}

GLProgram* GShader::getGLProgram(int key)
{
	//return GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_3D_POSITION_NORMAL_TEXTURE);
	return mapGL.at(key);
}