#include "demo_base.h"
#include "LoadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ply_file.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

GLuint
	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId = 0,
	VboId,
	PositionBufferId,
	ColorBufferId;

float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f };

float colorData[] = {
	1.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};


class MainApp : public GL_Demo_Base{
protected:
	static void ResizeViewportFunc(int width, int hight);
	static void RenderFunc(void);
	static void IdleFunc(void);
public:
	MainApp(){};
	~MainApp(){};
	void Init(int argc, char** argv);

	void ResizeWindow(int width, int hight);

	static MainApp* p_mApp;
}app;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);


	Model_PLY* model = new Model_PLY;
	model->Load("../model/bunny_res4.ply");

	glutReshapeFunc(ResizeViewportFunc);
	glutDisplayFunc(RenderFunc);
	glutIdleFunc(IdleFunc);

	static ShaderInfo shader_info[] = {
		{ GL_VERTEX_SHADER, "../shader/test.vert", 0 },
		{ GL_FRAGMENT_SHADER, "../shader/test.frag", 0 },
		{ GL_NONE, "", 0 }
	};

	ProgramId = LoadShader(shader_info);

	glBindAttribLocation(ProgramId, 0, "vert_pos");
	glBindAttribLocation(ProgramId, 1, "vert_color");
	
	LinkShader(ProgramId, shader_info);

	glUseProgram(ProgramId);

	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	PositionBufferId = vboHandles[0];
	ColorBufferId = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	// Create a set of vertex array object.
	
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}

void MainApp::RenderFunc(void)
{
	FrameCount++;

	glClear(GL_COLOR_BUFFER_BIT);

	// use uniform value to control rotation of triangle.
	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	static const glm::vec3 Z(0.0f, 0.0f, 1.0f);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 30.0f, Z);

	GLuint location = glGetUniformLocation(ProgramId, "RotationMtrx");

	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
	}

	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void MainApp::ResizeViewportFunc(int width, int hight)
{
	glViewport(0, 0, WIDTH, HIGHT);
}

void MainApp::IdleFunc()
{
	glutPostRedisplay();
}

void MainApp::ResizeWindow(int width, int hight)
{
	app.ResizeViewportFunc(width, hight);
}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}