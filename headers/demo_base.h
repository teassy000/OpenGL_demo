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
	static void ResizeViewportFunc(int width, int hight);
	static void RenderFunc(void);
	static void IdleFunc(void);
	static void TimerFunc(int);

	static void CleanUpFunc(void);
	static void CreateVBO(void);
	static void DestroyVBO(void);
	static void CreateShaders(void);
	static void DestroyShaders(void);

public:
	GL_Demo_Base() 
		: CurrentWidth(800), CurrentHeight(600), WindowHandle(0){}
	~GL_Demo_Base(){}

	static GL_Demo_Base* s_app;

	void Init(int argc, char** argv);
	void InitWindow(int argc, char** argv);

	void ResizeViewport(int width, int hight);
	void Render(void);
	void Idle(void);
	void CleanUp(void);
	void Timer(int);


	int CurrentWidth;
	int CurrentHeight;
	int WindowHandle;
};

void GL_Demo_Base::Init(int argc, char** argv)
{
	InitWindow(argc, argv);

	WindowHandle = glutCreateWindow("OpenGL_Demo");

	if (WindowHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window.\n" << std::endl;
		exit(EXIT_FAILURE);
	}

	int err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "ERROR: Init glew failed. Info : " << glewGetErrorString(err)<< std::endl;
	}

	std::cout << "INFO: OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glutReshapeFunc(ResizeViewportFunc);
	glutDisplayFunc(RenderFunc);
	glutIdleFunc(IdleFunc);

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

	glutTimerFunc(0, TimerFunc, 0);
	glutCloseFunc(CleanUpFunc);
}

void GL_Demo_Base::RenderFunc()
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Need: Draw stuff in another file.
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void GL_Demo_Base::ResizeViewportFunc(int width, int hight)
{

	glViewport(800, 600, width, hight);
}

void GL_Demo_Base::IdleFunc()
{
	glutPostRedisplay();
}

void GL_Demo_Base::CleanUpFunc()
{
	//DestroyShaders();
	//DestroyVBO();
}

void GL_Demo_Base::TimerFunc(int value)
{
	if (0 != value)
	{
		std::ostringstream oss;
		oss << "GL_Demo" << FrameCount * 4 << " : " << " Frames Per Second @ "
			<< WIDTH << " x " << HIGHT << std::endl;

		std::string str = oss.str();
		const char* chr = str.c_str();

		glutSetWindowTitle(chr);
	}

	FrameCount = 0;
	glutTimerFunc(250, TimerFunc, 1);
}



//===============================================
//===== here to point all callback functions.
//===============================================

void GL_Demo_Base::Render()
{
	s_app->RenderFunc();
}

void GL_Demo_Base::ResizeViewport(int width, int hight)
{
	s_app->ResizeViewportFunc(width, hight);
}

void GL_Demo_Base::Idle()
{
	s_app->IdleFunc();
}

void GL_Demo_Base::CleanUp()
{
	s_app->CleanUpFunc();
}

void GL_Demo_Base::Timer(int value)
{
	s_app->TimerFunc(value);
}

#endif //_DEMO_BASE_H_