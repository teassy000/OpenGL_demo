#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "GL/glew.h"

#include "glm/glm.hpp"


struct LightInfo
{
	glm::vec4		position;
	glm::vec4		color;

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
	light_info.position = glm::vec4(0.0f, 0.0f, 20.0f, 1.0f);
	// default light is white.
	light_info.color = glm::vec4(1.0f);

	light_info.Ld = vec3(0.01f);
	light_info.La = vec3(0.3f);
	light_info.Ls = vec3(0.3f);
}

void InitMaterial(MaterialInfo &material_info)
{
	material_info.Kd = vec3(0.05f);
	material_info.Ka = vec3(0.1f);
	material_info.Ks = vec3(0.3f);

	material_info.shininess = 0.0f;
}

#endif
