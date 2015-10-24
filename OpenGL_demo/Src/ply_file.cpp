#include "ply_file.h"
#include <iostream>

Model_PLY::Model_PLY() :
numConnectedTriangles(0),
numConnectedQuads(0),
numConnectedPoints(0),
numFaces(0)
{
	
}


void Model_PLY::calculateNormal(vec3 point1, vec3 point2, vec3 point3, vec3 & norm)
{
	vec3 va, vb;

	// calculate the vector that a triangle any two border.
	va = point1 - point2;
	vb = point1 - point3;

	//cross product to compute the direction of normal.
	norm = glm::cross(va, vb);

	glm::normalize(norm);
}

GLuint Model_PLY::Load(GLchar* filename)
{
	char* pch = strstr(filename, ".ply");

	if (pch != NULL)
	{
		// Open file and read it.
		FILE* file;
		fopen_s(&file, filename, "rb");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		try
		{
			vertexBuffer = (float*)malloc(fileSize);
		}
		catch (char*) // but why should be char* here?
		{
			return -1;
		}

		if (vertexBuffer == NULL)
		{
			return -1;
		}

		fseek(file, 0, SEEK_SET);

		// TODO: malloc some buffer and read binary file.
	}
	else
	{
		std::cerr << "Error : File do not have a \".ply\" extension" << std::endl;
	}
	return 0;
}


