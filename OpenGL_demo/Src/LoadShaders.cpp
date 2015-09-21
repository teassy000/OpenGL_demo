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
	if (shaders == NULL) return 0;

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;
	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;

		const GLchar* source = ReadShader(entry->fileName);
		if (source == NULL)
		{
			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			return 0;
		}
		glShaderSource(shader, 1, &source, NULL);
		delete[] source;

		glCompileShader(shader);

		// check error

		//attach shader to program
		// check error

		++entry;
	}


	// link program
	// check error
	return program;
}

