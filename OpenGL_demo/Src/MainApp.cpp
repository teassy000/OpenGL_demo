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
	void InitViewMtrx();
	void getProjectionMtrx();
	void getViewMtrx();
	void updateRotate_Y();

	glm::mat4 ProjectionMtrx;
	glm::mat4 ViewMtrx;
	glm::mat4 ModelMtrx = glm::mat4(1.0f);

	glm::mat4 transMtrx = glm::mat4(1.0f);
	glm::mat4 scaleMtrx = glm::mat4(1.0f);
	glm::mat4 rotateMtrx = glm::mat4(1.0f);
	glm::mat4 lookAt_Mtrx = glm::mat4(1.0f);

	GLuint t = 0;
	GLuint slow_tag;
}app;

static Model_PLY* model = new Model_PLY;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);

	model->Load("../model/bunny_res3.ply");

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
	InitViewMtrx();
	CalculateMVP();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << ModelMtrx[i][j] << std::ends;
		}
		std::cout << std::endl;
	}
}

void MainApp::RenderFunc(void)
{
	FrameCount++;
	app.updateRotate_Y();
	app.getViewMtrx();
	app.CalculateMVP();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint MVP_loc = glGetUniformLocation(ProgramId, "MVP");

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
	glm::mat4 temp = glm::perspectiveLH(45.0f, 1024.0f/720.0f, 1.0f, 100.0f);

 	ProjectionMtrx = temp;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << ProjectionMtrx[j][i] << std::ends;
		}
		std::cout << std::endl;
	}
}

void MainApp::InitViewMtrx()
{
	glm::mat4 tempR, tempT, tempS;
	rotateMtrx = glm::rotate(tempR, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	transMtrx = glm::translate(tempT, vec3(0.0f, -2.0f, -5.0f));
	scaleMtrx = glm::scale(tempS, vec3(20.0f));
 
	lookAt_Mtrx = glm::lookAtLH(vec3(0.0f, 0.0f, 0.01f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void MainApp::getViewMtrx()
{
	ViewMtrx = transMtrx * rotateMtrx * scaleMtrx;
}

void MainApp::CalculateMVP()
{
	MVP = ProjectionMtrx * lookAt_Mtrx  * ViewMtrx;
}

void MainApp::updateRotate_Y()
{
	t++;

	double tem = double((t & 0x1FFF)) / double(0x1FFF);

	glm::mat4(temp);

	rotateMtrx = glm::rotate_slow(temp, (float)tem*18.0f, vec3(0.0f, 1.0f, 0.0f));
}




int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}