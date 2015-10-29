#version 400

in vec3 vert_pos;
in vec3 vert_norm;

uniform mat4 Scale_Mtrx;
uniform mat4 Rotate_Mtrx;

out vec3 ex_color;

void main()
{
   ex_color = normalize(vec3(vert_norm.x , vert_norm.y, vert_norm.z));
   gl_Position =  Scale_Mtrx * Rotate_Mtrx  * vec4(vert_pos, 1.0f);
}