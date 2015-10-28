#version 400

in vec3 vert_pos;
in vec3 vert_norm;


out vec3 ex_color;

void main()
{
   ex_color = normalize(vec3(vert_norm.x + 0.5, vert_norm.y + 0.5, vert_norm.z + 0.5));
   gl_Position = vec4(vert_pos, 1.0f);
}