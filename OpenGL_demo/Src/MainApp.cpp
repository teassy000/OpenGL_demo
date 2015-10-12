#include "demo_base.h"
#include "LoadShaders.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

GLuint
	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId,
	VboId,
	ColorBufferId;

// Here to set the interim shader. 
// Note: need to integrate a LoadShader class.

// -- This is vertex shader.
const GLchar* VertexShader = 
{
	"#version 400\n"\

	"layout(location=0) in vec4 in_Position;\n"\
	"layout(location=1) in vec4 in_Color;\n"\
	"out vec4 ex_Color;\n"\

	"void main(void)\n"\
	"{\n"\
	"  gl_Position = in_Position;\n"\
	"  ex_Color = in_Color;\n"\
	"}\n"
};

// -- This is fragment shader.
const GLchar* FragmentShader = 
{
	"#version 400\n"\

	"in vec4 ex_color;\n"\
	"out vec4 color\n"\

	"void main(void)\n"\
	"{\n"\
	"	color = ex_color;\n"\
	"}\n"
};


class MainApp : public GL_Demo_Base{
public:
	MainApp(){};
	~MainApp(){};
	void Init(int argc, char** argv);
}app;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);

	static ShaderInfo shader_info[] = {
		{ GL_VERTEX_SHADER, "D:/OpenGL/oglpg-8th-edition/03/ch03_pointsprites/pointsprites.vs.glsl", 0 },
		{ GL_FRAGMENT_SHADER, "D:/OpenGL/oglpg-8th-edition/03/ch03_pointsprites/pointsprites.fs.glsl", 0 },
		{ GL_NONE, "", 0 }
	};

	LoadShader(shader_info);
}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}