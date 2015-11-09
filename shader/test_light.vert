#version 400

in vec3 vert_pos;
in vec3 vert_norm;


//out vec3 ex_color;
uniform mat4 WorldTransMtrx;
uniform mat4 MVP;


out vec3 tnorm;
out vec4 eyeCoords;

void main()
{
	tnorm = (WorldTransMtrx * vec4(vert_norm, 0.0)).xyz;

	eyeCoords = MVP * mat4(1.0f) * vec4(vert_pos, 1.0f);
		
	gl_Position =  MVP * vec4(vert_pos, 1.0f);
}
