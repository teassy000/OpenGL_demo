#version 400

in vec3 vert_pos;
in vec3 vert_norm;

uniform mat4 MVP;

out vec3 ex_color;

void main()
{
	vec3 temp_color;
	if(vert_norm.x <= 0.0f && vert_norm.y <= 0.0f && vert_norm.z <= 0.0f)
	{
		temp_color = vec3(0.1f, 0.9f, 0.1f);
	}		
	else
	{
		temp_color = normalize(vec3(max(vert_norm.x, 0.0f) , max(vert_norm.y, 0.0f), max(vert_norm.z, 0.0f)));
	}
	ex_color = temp_color;
	
	gl_Position =  MVP * vec4(vert_pos, 1.0f);
}