#include "demo_base.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"
static const int WIDTH = 1024;
static const int HIGHT = 720;

static void TimerFunc(int);

// callback by glut, should same as "c" style.

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
	void InitWindow(int argc, char** argv);
private:
	int WindowHandler;
}app;

void MainApp::InitWindow(int argc, char** argv)
{
	GL_Demo_Base::InitWindow(argc, argv);

	WindowHandler = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (WindowHandler < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(ResizeViewport);
	glutDisplayFunc(RenderFunc);
	glutIdleFunc(IdleFunc);
	glutTimerFunc(0, TimerFunc, 0);
}

static void CleanUp()
{

}

void TimerFunc(int value)
{
	if (0 != value)
	{

		std::ostringstream oss;
		oss << WINDOW_TITLE_PREFIX << FrameCount * 4 << " : " << " Frames Per Second @ " 
			<< app.CurrentWidth << " x " << app.CurrentHeight << std::endl;

		std::string str = oss.str();

		const char* chr = str.c_str();

		glutSetWindowTitle(chr);
	}

	FrameCount = 0;
	glutTimerFunc(250, TimerFunc, 1);
}

int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}