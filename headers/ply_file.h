#ifndef _PLY_FILE_H_
#define _PLY_FILE_H_

#include "glm/glm.hpp"
#include "GL/glew.h"
#include <string>

using glm::vec3;

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
	
	GLuint  Load(GLchar* filename);
	ply_Info* getInfo();

	GLvoid	calculateNormal(vec3 point1, vec3 point2, vec3 point3, vec3 & norm);

// need a interface to get these value
	GLuint	numConnectedTriangles;
	GLuint	numConnectedPoints;
	GLuint	numFaces;

	float*	vertexBuffer;
	float*	faceTriangles;
	float*  verticesNormals; // for each triangle
	float*	uvBuffer;
	float*	vertexuv;

private:
	GLuint	LoadFlat(GLchar* filename);
	GLuint  LoadSmooth(GLchar* filename);
	GLuint	Loaduv(GLchar* filename);

};

#endif
