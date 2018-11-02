#include "GMaterial.h"


GMaterial::GMaterial()
{
	hasNormalMap = false;
}


GMaterial::~GMaterial()
{
}

GMaterial *GMaterial::createWithGLProgramState(GLProgramState *state)
{
	GMaterial *ret = new GMaterial();
	ret->initWithGLProgramState(state);
	return ret;

}