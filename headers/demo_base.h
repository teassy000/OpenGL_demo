#ifndef _DEMO_BASE_H_
#define _DEMO_BASE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <ostream>

#include <sstream>

#include "GL/glew.h"
#include "freeglut.h"

static const int WIDTH = 1024;
static const int HIGHT = 720;
static unsigned FrameCount = 0;

class GL_Demo_Base
{
protected:
	static void TimerFunc(int);

	static void CleanUpFunc(void);
	static void DestroyVBO(void);
	static void CreateShaders(void);
	static void DestroyShaders(void);

public:
	GL_Demo_Base() 
		: CurrentWidth(WIDTH), CurrentHeight(HIGHT), WindowHandle(0){}
	~GL_Demo_Base(){}

	static GL_Demo_Base* s_app;

	void Init(int argc, char** argv);
	void InitWindow(int argc, char** argv);

	int CurrentWidth;
	int CurrentHeight;
	int WindowHandle;
};

void GL_Demo_Base::Init(int argc, char** argv)
{
	InitWindow(argc, argv);
	glutInitWindowSize(WIDTH, HIGHT);
	glutInitWindowPosition(0, 0);
	WindowHandle = glutCreateWindow("OpenGL_Demo");


	if (WindowHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window.\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	glewExperimental = GL_TRUE;
	int err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "ERROR: Init glew failed. Info : " << glewGetErrorString(err)<< std::endl;
	}

	std::cout << "INFO: OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

}

void GL_Demo_Base::InitWindow(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutTimerFunc(10, TimerFunc, 1);
}

void GL_Demo_Base::TimerFunc(int value)
{
	if (0 != value)
	{
		std::ostringstream oss;
		oss << "GL_Demo" << " : " << FrameCount << " Frames Per Second @ "
			<< WIDTH << " x " << HIGHT << std::endl;

		std::string str = oss.str();
		const char* chr = str.c_str();

		glutSetWindowTitle(chr);
	}

	FrameCount = 0;
	glutTimerFunc(10, TimerFunc, 1);
}

#endif //_DEMO_BASE_H_