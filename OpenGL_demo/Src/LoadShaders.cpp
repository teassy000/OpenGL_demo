#include <fstream>
#include <iostream>
#include <streambuf>

#include "LoadShaders.h"


static const GLchar* ReadShader(std::string filename)
{

	std::fstream fin(filename, std::fstream::in);

	if (!fin)
	{
		std::cerr << "Read shader failed!" << std::endl;
		return NULL;
	}

	std::string tempStr;
	fin.seekg(0, std::ios::end);
	tempStr.reserve(fin.tellg());
	fin.seekg(0, std::ios::beg);

	tempStr.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

	GLchar* source = new GLchar[tempStr.length() + 1];
	strcpy(source, tempStr.c_str());

	return const_cast<const GLchar*>(source);
}

GLuint LoadShader(ShaderInfo* shaders)
{
	// Read shaders 

}

