#include "demo_base.h"

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
//	void InitWindow(int argc, char** argv);
}app;

int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}