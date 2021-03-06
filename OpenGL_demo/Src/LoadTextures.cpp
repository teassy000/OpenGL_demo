#include "LoadTextures.h"
#include "GL/glew.h"
#include <iostream>
//#include <algorithm>

Images::Images():
	width(0),
	hight(0),
	channels(0),
	TextureID(0)
{
};


GLuint Images::getTextureID() const
{
	return TextureID;
}

bool Images::LoadImages(std::string filename)
{
	GLubyte* pData = SOIL_load_image(filename.c_str(),
		&width, &hight, &channels, SOIL_LOAD_AUTO);
	if (pData == nullptr)
	{
		std::cerr << "Cannot load image: " << filename.c_str() << std::endl;
		exit(EXIT_FAILURE);
		return false;
	}

	int i, j;
	for (j = 0; j * 2 < hight; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (hight - 1 - j) * width * channels;
		for (i = width * channels; i > 0; --i)
		{
			std::swap(pData[index1], pData[index2]);
			++index1;
			++index2;
		}
	}

	std::cout << "width and hight is: " << width << "x" << hight << std::endl;

	// Load texture into OpenGL
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pData);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(pData);

	std::cout << "Tex ID is: " << TextureID << std::endl;

	return true;
}

void Images::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

