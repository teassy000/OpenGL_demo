#ifndef _LOADTEXTURE_H_
#define _LOADTEXTURE_H_

// this is a class to load png image as texture.
// Note: Texture is not important than you can image. 
// DO NOT let it to be your first solution to figure out rendering problem.

#include "SOIL.h"
#include <string>

class Images
{
public:
	Images();
	~Images(){};

	int LoadImages(std::string);

private:
	int width;
	int hight;
	int channels;
};




#endif