#pragma once
#include "cocos2d.h"
#include <map>
USING_NS_CC;
using namespace std;
class GShader : public Ref 
{
public:

	enum
	{
		G_SHADER_DIFFUSE,
		G_SHADER_NORMAL_MAP,
		G_SHADER_DEPTH,
		G_SHADER_SHADOW
	};


	GShader();
	~GShader();
	map<int, GLProgram *> mapGL;

	static GShader* getInstance();
	GLProgram * getGLProgram(int key);
protected:
	void initShader();
};

