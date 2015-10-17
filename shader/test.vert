#version 400

in vec3 vert_pos;
in vec4 vert_color;

out vec4 ex_color;

void main()
{
   ex_color = vert_color;
   gl_Position = vec4(vert_pos, 1.0f);
}