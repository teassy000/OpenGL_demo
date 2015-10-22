#ifndef _PLY_FILE_H_
#define _PLY_FILE_H_

#include "glm/glm.hpp"
#include "GL/GL.h"
#include "GL/GLU.h"


struct ply_Info
{
	GLchar* format;
	GLchar* comment;

	GLuint	numVertex;
	GLuint	numFaces;
};

class Model_PLY {
public:
	Model_PLY();
	
	GLuint Load(char* filename);
	ply_Info* getInfo();

	vec3 calculateNormal();
};

#endif