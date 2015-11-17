#include "ply_file.h"
#include <iostream>

Model_PLY::Model_PLY() :
numConnectedTriangles(0),
numConnectedPoints(0),
numFaces(0),
vertexBuffer(nullptr),
faceTriangles(nullptr),
verticesNormals(nullptr),
vertexuv(nullptr)
{
	
}

GLuint Model_PLY::Load(GLchar* filename)
{
	char* pch_smooth = strstr(filename, "_smooth");
	char* pch_uv = strstr(filename, "_smooth_uv");
	if (pch_uv != nullptr)
	{
		return  Loaduv(filename);
	}
	else if (pch_smooth != nullptr)
	{
		return LoadSmooth(filename);
	}
	else
	{
		return LoadFlat(filename);
	}
}

void Model_PLY::calculateNormal(vec3 point1, vec3 point2, vec3 point3, vec3 & norm)
{
	vec3 va, vb;

	// calculate the vector that a triangle any two border.
	va = point1 - point2;
	vb = point2 - point3;

	//cross product to compute the direction of normal.
	norm = glm::cross(va, vb);

	glm::normalize(norm);
}

GLuint Model_PLY::LoadFlat(GLchar* filename)
{
	char* pch = strstr(filename, ".ply");

	if (pch != nullptr)
	{
		// Open file and read it.
		FILE* file;
		fopen_s(&file, filename, "rb");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file); //ftell is get the current position of pointer.

		try
		{
			vertexBuffer = (float*)malloc(fileSize);
		}
		catch (char*) // but why should be char* here?
		{
			return -1;
		}

		if (vertexBuffer == nullptr)
		{
			return -1;
		}

		fseek(file, 0, SEEK_SET);

		faceTriangles = (float*)malloc(fileSize * sizeof(float));
		verticesNormals = (float*)malloc(fileSize * sizeof(float));

		if (file != nullptr)
		{
			int i = 0;
			int temp = 0;
			int normalIndex = 0;
			int triangleIndex = 0;
			char buffer[1000]; // every time gets 1000 buffer.

			fgets(buffer, 300, file); //ply

			// READ HEADER
			//-------------------

			// Find number of vertexes
			// e.g. element vertex 12
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer) - strlen("element vertex"), buffer + strlen("element vertex")); //erase "element vertex" header.
			sscanf_s(buffer, "%i", &this->numConnectedPoints); //read integer value for num of points.
			std::cout << "Number of connected point is : " << numConnectedPoints << std::endl;

			// find number of faces
			// e.g. element face 10
			fseek(file, 0, SEEK_SET);
			while (strncmp("element face", buffer, strlen("element face")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer) - strlen("element face"), buffer + strlen("element face")); //erase "element face" header.
			sscanf_s(buffer, "%i", &this->numFaces); //read integer value for num of faces.
			std::cout << "Number of faces is : " << numFaces << std::endl;

			// go to end_header
			while (strncmp(("end_header"), buffer, strlen("end_header")) != 0)
			{
				fgets(buffer, 300, file);
			}


			// Read vertexes
			i = 0;
			for (GLuint iter = 0; iter < this->numConnectedPoints; ++iter)
			{
				fgets(buffer, 300, file);

				sscanf_s(buffer, "%f %f %f", &vertexBuffer[i], &vertexBuffer[i + 1], &vertexBuffer[i + 2]);
				i += 3;
			}

			// Read faces
			i = 0;
			for (GLuint iter = 0; iter < this->numFaces; ++iter)
			{
				fgets(buffer, 300, file);

				if ('3' == buffer[0])
				{
					int vert0 = 0, vert1 = 0, vert2 = 0;
					buffer[0] = ' ';
					sscanf_s(buffer, "%i%i%i", &vert0, &vert1, &vert2);




					// set values to faceTriangles.
 					faceTriangles[triangleIndex] = vertexBuffer[3 * vert0];
 					faceTriangles[triangleIndex + 1] = vertexBuffer[3 * vert0 + 1];
					faceTriangles[triangleIndex + 2] = vertexBuffer[3 * vert0 + 2];
 					faceTriangles[triangleIndex + 3] = vertexBuffer[3 * vert1];
					faceTriangles[triangleIndex + 4] = vertexBuffer[3 * vert1 + 1];
					faceTriangles[triangleIndex + 5] = vertexBuffer[3 * vert1 + 2];
 					faceTriangles[triangleIndex + 6] = vertexBuffer[3 * vert2];
					faceTriangles[triangleIndex + 7] = vertexBuffer[3 * vert2 + 1];
					faceTriangles[triangleIndex + 8] = vertexBuffer[3 * vert2 + 2];
 
 					// going to calculate verticesNormals.
					vec3 point0(vertexBuffer[3 * vert0], vertexBuffer[3 * vert0 + 1], vertexBuffer[3 * vert0 + 2]);
					vec3 point1(vertexBuffer[3 * vert1], vertexBuffer[3 * vert1 + 1], vertexBuffer[3 * vert1 + 2]);
					vec3 point2(vertexBuffer[3 * vert2], vertexBuffer[3 * vert2 + 1], vertexBuffer[3 * vert2 + 2]);
 
 					vec3 norm(0.0f);
 					this->calculateNormal(point0, point1, point2, norm);

 					for (int offset = 0; offset < 9; ++offset)
 					{
						verticesNormals[normalIndex + offset] = norm[offset % 3]; //set normal for each point.
 					}
 
 					normalIndex += 9; //set offset to next triangle. 

					triangleIndex += 9; //set offset to next triangle.
					numConnectedTriangles += 3; // three points per triangle
				}
				i += 3;
			}

			fclose(file);

		}
		else
		{
			std::cerr << "Error : Can not open file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Error : File do not have a \".ply\" extension." << std::endl;
	}
	return 0;
}

GLuint Model_PLY::LoadSmooth(GLchar* filename)
{
	char* pch = strstr(filename, ".ply");

	if (pch != nullptr)
	{
		// Open file and read it.
		FILE* file;
		fopen_s(&file, filename, "rb");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file); //ftell is get the current position of pointer.

		try
		{
			vertexBuffer = (float*)malloc(fileSize);
		}
		catch (char*) // but why should be char* here?
		{
			return -1;
		}

		if (vertexBuffer == nullptr)
		{
			return -1;
		}

		fseek(file, 0, SEEK_SET);

		faceTriangles = (float*)malloc(fileSize * sizeof(float));
		verticesNormals = (float*)malloc(fileSize * sizeof(float));

		if (file != nullptr)
		{
			int i = 0;
			int temp = 0;
			int normalIndex = 0;
			int triangleIndex = 0;
			char buffer[1000]; // every time gets 1000 buffer.

			fgets(buffer, 300, file); //ply

			// READ HEADER
			//-------------------

			// Find number of vertexes
			// e.g. element vertex 12
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer)-strlen("element vertex"), buffer + strlen("element vertex")); //erase "element vertex" header.
			sscanf_s(buffer, "%i", &this->numConnectedPoints); //read integer value for num of points.
			std::cout << "Number of connected point is : " << numConnectedPoints << std::endl;

			// find number of faces
			// e.g. element face 10
			fseek(file, 0, SEEK_SET);
			while (strncmp("element face", buffer, strlen("element face")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer)-strlen("element face"), buffer + strlen("element face")); //erase "element face" header.
			sscanf_s(buffer, "%i", &this->numFaces); //read integer value for num of faces.
			std::cout << "Number of faces is : " << numFaces << std::endl;

			// go to end_header
			while (strncmp(("end_header"), buffer, strlen("end_header")) != 0)
			{
				fgets(buffer, 300, file);
			}


			// Read vertexes
			i = 0;
			for (GLuint iter = 0; iter < this->numConnectedPoints; ++iter)
			{
				fgets(buffer, 300, file);

				// i to (i+2) is position of vertex, (i+3) to (i+5) is normal of vertex;
				sscanf_s(buffer, "%f %f %f %f %f %f", &vertexBuffer[i], &vertexBuffer[i + 1], &vertexBuffer[i + 2],
					&vertexBuffer[i + 3], &vertexBuffer[i + 4], &vertexBuffer[i + 5]);
				i += 6;
			}

			// Read faces
			i = 0;
			for (GLuint iter = 0; iter < this->numFaces; ++iter)
			{
				fgets(buffer, 300, file);

				if ('3' == buffer[0])
				{
					int vert0 = 0, vert1 = 0, vert2 = 0;
					buffer[0] = ' ';
					sscanf_s(buffer, "%i%i%i", &vert0, &vert1, &vert2);

					// set values to faceTriangles.
					faceTriangles[triangleIndex] = vertexBuffer[6 * vert0];
					faceTriangles[triangleIndex + 1] = vertexBuffer[6 * vert0 + 1];
					faceTriangles[triangleIndex + 2] = vertexBuffer[6 * vert0 + 2];
					faceTriangles[triangleIndex + 3] = vertexBuffer[6 * vert1];
					faceTriangles[triangleIndex + 4] = vertexBuffer[6 * vert1 + 1];
					faceTriangles[triangleIndex + 5] = vertexBuffer[6 * vert1 + 2];
					faceTriangles[triangleIndex + 6] = vertexBuffer[6 * vert2];
					faceTriangles[triangleIndex + 7] = vertexBuffer[6 * vert2 + 1];
					faceTriangles[triangleIndex + 8] = vertexBuffer[6 * vert2 + 2];


					verticesNormals[normalIndex] = vertexBuffer[6 * vert0 + 3];
					verticesNormals[normalIndex + 1] = vertexBuffer[6 * vert0 + 4];
					verticesNormals[normalIndex + 2] = vertexBuffer[6 * vert0 + 5];
					verticesNormals[normalIndex + 3] = vertexBuffer[6 * vert1 + 3];
					verticesNormals[normalIndex + 4] = vertexBuffer[6 * vert1 + 4];
					verticesNormals[normalIndex + 5] = vertexBuffer[6 * vert1 + 5];
					verticesNormals[normalIndex + 6] = vertexBuffer[6 * vert2 + 3];
					verticesNormals[normalIndex + 7] = vertexBuffer[6 * vert2 + 4];
					verticesNormals[normalIndex + 8] = vertexBuffer[6 * vert2 + 5];

					normalIndex += 9; //set offset to next triangle. 

					triangleIndex += 9; //set offset to next triangle.
					numConnectedTriangles += 3; // three points per triangle
				}
				i += 3;
			}

			fclose(file);

		}
		else
		{
			std::cerr << "Error : Can not open file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Error : File do not have a \".ply\" extension." << std::endl;
	}
	return 0;
}

GLuint Model_PLY::Loaduv(GLchar* filename)
{
	char* pch = strstr(filename, ".ply");

	if (pch != nullptr)
	{
		// Open file and read it.
		FILE* file;
		fopen_s(&file, filename, "rb");

		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file); //ftell is get the current position of pointer.

		try
		{
			vertexBuffer = (float*)malloc(fileSize);
		}
		catch (char*) // but why should be char* here?
		{
			return -1;
		}

		if (vertexBuffer == nullptr)
		{
			return -1;
		}

		fseek(file, 0, SEEK_SET);

		faceTriangles = (float*)malloc(fileSize * sizeof(float));
		verticesNormals = (float*)malloc(fileSize * sizeof(float));
		vertexuv = (float*)malloc(fileSize * sizeof(float));


		if (file != nullptr)
		{

			int temp = 0;
			int normalIndex = 0, uvIndex = 0;
			int triangleIndex = 0;
			char buffer[1000]; // every time gets 1000 buffer.

			fgets(buffer, 300, file); //ply

			// READ HEADER
			//-------------------

			// Find number of vertexes
			// e.g. element vertex 12
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer) - strlen("element vertex"), buffer + strlen("element vertex")); //erase "element vertex" header.
			sscanf_s(buffer, "%i", &this->numConnectedPoints); //read integer value for num of points.
			std::cout << "Number of connected point is : " << numConnectedPoints << std::endl;

			// find number of faces
			// e.g. element face 10
			fseek(file, 0, SEEK_SET);
			while (strncmp("element face", buffer, strlen("element face")) != 0)
			{
				fgets(buffer, 300, file);
			}
			strcpy_s(buffer, sizeof(buffer) - strlen("element face"), buffer + strlen("element face")); //erase "element face" header.
			sscanf_s(buffer, "%i", &this->numFaces); //read integer value for num of faces.
			std::cout << "Number of faces is : " << numFaces << std::endl;

			// go to end_header
			while (strncmp(("end_header"), buffer, strlen("end_header")) != 0)
			{
				fgets(buffer, 300, file);
			}


			// Read vertexes
			int i = 0, j = 0;
			for (GLuint iter = 0; iter < this->numConnectedPoints; ++iter)
			{
				fgets(buffer, 300, file);

				// i to (i+2) is position of vertex, (i+3) to (i+5) is normal of vertex;
				sscanf_s(buffer, "%f %f %f %f %f %f %f %f", &vertexBuffer[i], &vertexBuffer[i + 1], &vertexBuffer[i + 2],
					&vertexBuffer[i + 3], &vertexBuffer[i + 4], &vertexBuffer[i + 5], &vertexuv[j], &vertexuv[j + 1]);
				i += 6;
				j += 2;
			}

			// Read faces
			i = 0;
			uvBuffer = (float*)malloc(6 * numFaces * sizeof(float));

			for (GLuint iter = 0; iter < this->numFaces; ++iter)
			{
				fgets(buffer, 300, file);

				if ('3' == buffer[0])
				{
					int vert0 = 0, vert1 = 0, vert2 = 0;
					buffer[0] = ' ';
					sscanf_s(buffer, "%i%i%i", &vert0, &vert1, &vert2);

					// set values to faceTriangles.
					faceTriangles[triangleIndex] = vertexBuffer[6 * vert0];
					faceTriangles[triangleIndex + 1] = vertexBuffer[6 * vert0 + 1];
					faceTriangles[triangleIndex + 2] = vertexBuffer[6 * vert0 + 2];
					faceTriangles[triangleIndex + 3] = vertexBuffer[6 * vert1];
					faceTriangles[triangleIndex + 4] = vertexBuffer[6 * vert1 + 1];
					faceTriangles[triangleIndex + 5] = vertexBuffer[6 * vert1 + 2];
					faceTriangles[triangleIndex + 6] = vertexBuffer[6 * vert2];
					faceTriangles[triangleIndex + 7] = vertexBuffer[6 * vert2 + 1];
					faceTriangles[triangleIndex + 8] = vertexBuffer[6 * vert2 + 2];


					verticesNormals[normalIndex] = vertexBuffer[6 * vert0 + 3];
					verticesNormals[normalIndex + 1] = vertexBuffer[6 * vert0 + 4];
					verticesNormals[normalIndex + 2] = vertexBuffer[6 * vert0 + 5];
					verticesNormals[normalIndex + 3] = vertexBuffer[6 * vert1 + 3];
					verticesNormals[normalIndex + 4] = vertexBuffer[6 * vert1 + 4];
					verticesNormals[normalIndex + 5] = vertexBuffer[6 * vert1 + 5];
					verticesNormals[normalIndex + 6] = vertexBuffer[6 * vert2 + 3];
					verticesNormals[normalIndex + 7] = vertexBuffer[6 * vert2 + 4];
					verticesNormals[normalIndex + 8] = vertexBuffer[6 * vert2 + 5];

					// set uv for each triangle.
					uvBuffer[uvIndex] = vertexuv[vert0];
					uvBuffer[uvIndex + 1] = vertexuv[vert0 + 1];
					uvBuffer[uvIndex + 2] = vertexuv[vert1];
					uvBuffer[uvIndex + 3] = vertexuv[vert1 + 1];
					uvBuffer[uvIndex + 4] = vertexuv[vert2];
					uvBuffer[uvIndex + 5] = vertexuv[vert2 + 1];

					uvIndex += 6;
					normalIndex += 9; //set offset to next triangle. 

					triangleIndex += 9; //set offset to next triangle.
					numConnectedTriangles += 3; // three points per triangle
				}
				i += 3;
			}

			fclose(file);

		}
		else
		{
			std::cerr << "Error : Can not open file." << std::endl;
		}
	}
	else
	{
		std::cerr << "Error : File do not have a \".ply\" extension." << std::endl;
	}
	return 0;
}
