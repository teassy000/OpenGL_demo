#version 400

in vec3 ex_color;
out vec4 FragColor;



void main()
{
	FragColor = vec4(ex_color, 1.0);
}