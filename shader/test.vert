#version 400

in vec3 vert_pos;
in vec3 vert_norm;

uniform mat4 MVP;

out vec3 ex_color;

void main()
{
   ex_color = normalize(vec3(max(vert_norm.x, 0.0f) , max(vert_norm.y, 0.0f), max(vert_norm.z, 0.0f)));
   gl_Position =  MVP * vec4(vert_pos, 1.0f);
}