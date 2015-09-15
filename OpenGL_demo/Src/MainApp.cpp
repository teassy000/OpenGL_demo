#include "demo_base.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"
static const int WIDTH = 1024;
static const int HIGHT = 720;

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
	MainApp() :WindowHandler(0){};
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
}
/*
static void GL_Demo_Base::ResizeViewport(int width, int hight)
{
	glViewport(0, 0, width, hight);
}

static void RenderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}
*/
static void CleanUp()
{
	// here the problem is 
	// 
}

int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}