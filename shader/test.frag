#version 400

in vec3 ex_color;
out vec4 FragColor;



void main()
{
	vec4 InnerColor = vec4(ex_color, 1.0f);
	vec4 OuterColor = vec4(0.0f,0.0f,0.0f,0.0f);

	float RadiusInner = 0.7f;
	float RadiusOuter = 0.3f;

	float dx = ex_color.x - 0.3;
	float dy = ex_color.y - 0.3;

	float dist = sqrt(dx * dx + dy * dy);

	FragColor = 
	mix( OuterColor, InnerColor,
		 smoothstep( RadiusOuter, RadiusInner, dist)
		);
}