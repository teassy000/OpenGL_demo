#version 400

in vec3 vert_pos;
in vec3 vert_norm;

struct LightInfo {
	vec4	position;
	vec4	color;

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

uniform mat4 ModelViewMtrx;
uniform mat4 ModelMtrx;
uniform mat4 eyeMtrx;
uniform mat4 MVP;

out vec3 ex_color;

void main()
{
	
	vec3 tnorm = normalize(vec3(ModelViewMtrx) * mat3(ModelMtrx) * vert_norm);
	vec4 eyeCoords = ModelViewMtrx * mat4(1.0f) * vec4(vert_pos, 1.0f);

	vec3 s = normalize( vec3( light.position - eyeCoords));
	vec3 v = normalize( -eyeCoords.xyz);
	vec3 r = reflect( -s, tnorm);
	
	vec3 ambient = normalize(light.La * material.Ka);
  
	float sDotN = max( dot(s, tnorm), 0.0f);

	vec3 diffuse = normalize(light.Ld * material.Kd * sDotN);
	vec3 spec = vec3(0.0f);
	if(sDotN > 0.0f) {
		spec = light.Ls * material.Ks * pow( max( dot(r, v), 0.0f), material.shininess);
	}

	ex_color = diffuse;
	
	gl_Position =  MVP * vec4(vert_pos, 1.0f);
}
