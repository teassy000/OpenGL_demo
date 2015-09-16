#ifndef _DEMO_BASE_H_
#define _DEMO_BASE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "GL/glew.h"
#include "freeglut.h"


static unsigned FrameCount = 0;

class GL_Demo_Base
{
public:
	GL_Demo_Base() 
		: CurrentWidth(800), CurrentHeight(600), WindowHandle(0){}
	~GL_Demo_Base(){}

	void Init(int argc, char** argv);
	virtual void InitWindow(int argc, char** argv);

	static void ResizeViewport(int width, int hight);
	static void RenderFunc(void);
	static void IdleFunc(void);
//	static void CleanUp(void);

	// here are idle functions.
	void Cleanup(void);
	void CreateVBO(void);
	void DestroyVBO(void);
	void CreateShaders(void);
	void DestroyShaders(void);

	int CurrentWidth;
	int CurrentHeight;
	int WindowHandle;
};

void GL_Demo_Base::Init(int argc, char** argv)
{
	InitWindow(argc, argv);

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

// 	CreateShaders();
//	CreateVBO();

	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
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
}

void GL_Demo_Base::ResizeViewport(int width, int hight)
{
	glViewport(0, 0, width, hight);
}

void GL_Demo_Base::RenderFunc()
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

void GL_Demo_Base::IdleFunc()
{
	glutPostRedisplay();
}



#endif //_DEMO_BASE_H_