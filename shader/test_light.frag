#version 400

in vec3 tnorm;
in vec4 eyeCoords;
in vec2 texCrood;

uniform sampler2D Tex;

out vec4 FragColor;

struct LightInfo {
	vec4	position;
	vec3	Ld;		
	vec3	La;		
	vec3	Ls;		
};
uniform LightInfo light;

struct MaterialInfo {
	vec3	Kd;
	vec3	Ka;
	vec3	Ks;
	float 	shininess;
};
uniform MaterialInfo material;

void main()
{
	vec3 norm = normalize(tnorm);

	//vec3 norm = normalize( texture(TexNorm, texCrood).xyz *2.0f - vec3(1.0));

	vec3 s = normalize( vec3( light.position - eyeCoords));
	vec3 v = normalize( -eyeCoords.xyz);
	vec3 r = reflect( -s, norm);

	vec3 ambient = light.La * material.Ka;
	ambient.x = max(ambient.x, 0.0f);
	ambient.y = max(ambient.y, 0.0f);
	ambient.z = max(ambient.z, 0.0f);
	
	float sDotN = max( dot(norm, s), 0.0f);

	vec3 diffuse = light.Ld * material.Kd * sDotN;
	diffuse.x = max(diffuse.x, 0.0f);
	diffuse.y = max(diffuse.y, 0.0f);
	diffuse.z = max(diffuse.z, 0.0f);

	vec3 spec = vec3(0.0f);
	if(sDotN > 0.0f) {
		spec = light.Ls * material.Ks * pow( max( dot(r, v), 0.0f), material.shininess);
	}

	vec4 color = texture(Tex, texCrood);
	FragColor = vec4(ambient + diffuse, 1.0f) * color + vec4(spec, 1.0f);

}
