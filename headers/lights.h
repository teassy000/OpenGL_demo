#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "GL/glew.h"

#include "glm/glm.hpp"


struct LightInfo
{
	glm::vec4		position;
	glm::vec3		Ld;		// Diffuse intensity
	glm::vec3		La;		// Ambient intensity
	glm::vec3		Ls;		// Specular	intensity
};

struct MaterialInfo
{
	glm::vec3		Kd;		// Diffuse Reflectivity
	glm::vec3		Ka;		// Ambient Reflectivity
	glm::vec3		Ks;		// Specular Reflectivity
	float			shininess; // Specular shininess factor
};

// initialize light to a fixed value.
void InitLight(LightInfo &light_info)
{
	light_info.position = glm::vec4(600.0f, 10000, 40.0f, 1.0f);
	light_info.Ld = vec3(0.4f, 0.5f, 0.2f);
	light_info.La = vec3(0.4f, 0.5f, 0.2f);
	light_info.Ls = vec3(0.4f, 0.5f, 0.2f);
}

void InitMaterial(MaterialInfo &material_info)
{
	material_info.Kd = vec3(0.4f, 0.5f, 0.2f);
	material_info.Ka = vec3(0.4f, 0.5f, 0.2f);
	material_info.Ks = vec3(0.4f, 0.5f, 0.2f);

	material_info.shininess = 0.3f;
}

#endif
