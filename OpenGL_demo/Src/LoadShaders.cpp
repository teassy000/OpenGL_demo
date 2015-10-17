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
	strcpy_s(source, tempStr.length() + 1, tempStr.c_str());

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
		if (source == "")
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

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
#ifdef _DEBUG
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(shader, len, &len, log);
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete [] log;
#endif // _DEBUG
			return 0;
		}

		glAttachShader(program, shader);

		++entry;
	}

	return program;
}

GLuint LinkShader(GLuint programID, ShaderInfo* shaders)
{
	glLinkProgram(programID);
	GLint linked;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
#ifdef _DEBUG
		GLsizei len;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(programID, len, &len, log);
		std::cerr << "Shader compilation failed: " << log << std::endl;
		delete[] log;
#endif // _DEBUG
		ShaderInfo* entry = shaders;

		for (entry = shaders; entry->type != GL_NONE; ++entry)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		return 0;
	}

	return programID;
}

