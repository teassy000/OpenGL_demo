#include "demo_base.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"
static const int WIDTH = 1024;
static const int HIGHT = 720;

static void ResizeViewport(int width, int hight);
static void RenderFunc();

class MainApp : public GL_Demo_Base{
public:
	MainApp() :WindowHandler(0){};
	~MainApp(){};
	void InitWindow(int argc, char** argv);
//	void ResizeViewport(int width, int hight);
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

// glutReshapeFunc and glutDisplayFunc expect a c style callback func, so delete them here.
/*
void MainApp::ResizeViewport(int newwidth, int newhight)
{
	GL_Demo_Base::ResizeViewport(newwidth, newwidth);
}
*/

static void ResizeViewport(int width, int hight)
{
	glViewport(0, 0, width, hight);
}

static void RenderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}