#version 400

in vec3 vert_pos;
in vec4 vert_color;

uniform mat4 RotationMtrx;

out vec4 ex_color;

void main()
{
   ex_color = vert_color;
   gl_Position = RotationMtrx * vec4(vert_pos, 1.0f);
}