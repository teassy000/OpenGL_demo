#include "LoadTextures.h"
#include "GL/glew.h"
#include <iostream>
//#include <algorithm>

Images::Images():
	width(0),
	hight(0),
	channels(0)
{
};


int Images::LoadImages(std::string filename)
{
	GLubyte* pData = SOIL_load_image(filename.c_str(),
		&width, &hight, &channels, SOIL_LOAD_AUTO);
	if (pData == nullptr)
	{
		std::cerr << "Cannot load image: " << filename.c_str() << std::endl;
		exit(EXIT_FAILURE);
		return -1;
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
	return 0;
}