#ifndef _LOAD_SHADERS_H_
#define _LOAD_SHADERS_H_

#include <string>
#include "GL/glew.h"
#include "freeglut.h"


struct ShaderInfo
{
	GLenum		type;
	std::string fileName;
	GLuint		shader;
};

GLuint LoadShader(ShaderInfo*);


#endif // !_LOAD_SHADERS_H_
