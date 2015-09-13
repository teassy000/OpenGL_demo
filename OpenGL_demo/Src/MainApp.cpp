#include "demo_base.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"
static const int WIDTH = 1024;
static const int HIGHT = 720;

class MainApp : public GL_Demo_Base{
	void InitWindow(int argc, char** argv);
	void ResizeViewport(int width, int hight);
private:
	int WindowHandler;
};

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

void MainApp::ResizeViewport(int newwidth, int newhight)
{
	GL_Demo_Base::ResizeViewport(newwidth, newwidth);
}



int main(int argc, char** argv)
{
	MainApp* app;
	app->Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}