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
	NormalBufferId;

float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f };

float colorData[] = {
	1.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

GLfloat Scale = 5.0f;

glm::mat4 scaleMtrx = glm::mat4(
	Scale, 0.0f, 0.0f, 0.0f,
	0.0f, Scale, 0.0f, 0.0f,
	0.0f, 0.0f, Scale, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	);

glm::mat4 transMtrx = glm::mat4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	);


glm::mat4 rotateMtrx = glm::mat4(
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
	);


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
	void CalculateMVP();

	static MainApp* p_mApp;
	glm::mat4 MVP;

private:

	void getProjectionMtrx();
	void getViewMtrx();

	glm::mat4 ProjectionMtrx;
	glm::mat4 ViewMtrx;
	glm::mat4 ModelMtrx = glm::mat4(1.0f);
}app;

static Model_PLY* model = new Model_PLY;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);

	model->Load("../model/bunny_res2.ply");

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
	glBindAttribLocation(ProgramId, 1, "vert_norm");
	
	LinkShader(ProgramId, shader_info);

	glUseProgram(ProgramId);

	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	PositionBufferId = vboHandles[0];
	NormalBufferId = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * (model->numFaces), model->faceTriangles, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NormalBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * (model->numFaces), model->normals, GL_STATIC_DRAW);

	// Create a set of vertex array object.
	
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, NormalBufferId);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (GLubyte*)NULL);

	getProjectionMtrx();
	getViewMtrx();
	CalculateMVP();
}

void MainApp::RenderFunc(void)
{
	FrameCount++;
	double t = double(GetTickCount() & 0x1FFF) / double(0x1FFF);
	double cos_t = glm::cos(t * 3.6f * 2.0f);
	double sin_t = glm::sin(t * 3.6f * 2.0f);
	
	rotateMtrx = glm::mat4(cos_t, 0.0f,-1.0f * sin_t, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							sin_t, 0.0f, cos_t, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f
		);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint Trans_mtrx_loc = glGetUniformLocation(ProgramId, "Trans_Mtrx");
	GLuint Scale_mtrx_loc = glGetUniformLocation(ProgramId, "Scale_Mtrx");
	GLuint Rotate_mtrx_loc = glGetUniformLocation(ProgramId, "Rotate_Mtrx");
	GLuint MVP_loc = glGetUniformLocation(ProgramId, "MVP");

	glUniformMatrix4fv(Scale_mtrx_loc, 1, GL_FALSE, &scaleMtrx[0][0]);
	glUniformMatrix4fv(Rotate_mtrx_loc, 1, GL_FALSE, &rotateMtrx[0][0]);
	glUniformMatrix4fv(Trans_mtrx_loc, 1, GL_FALSE, &transMtrx[0][0]);
	glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &(app.MVP[0][0]));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, model->numFaces * 9);


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

void MainApp::getProjectionMtrx()
{
	ProjectionMtrx = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 20.0f);
}

void MainApp::getViewMtrx()
{
	ViewMtrx = transMtrx * rotateMtrx;
}

void MainApp::CalculateMVP()
{
	//MVP = ProjectionMtrx * ViewMtrx * ModelMtrx;
	// still got some problem on ProjectionMtrx, needs fix temorrow.
	MVP = ViewMtrx * ModelMtrx;
}




int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}