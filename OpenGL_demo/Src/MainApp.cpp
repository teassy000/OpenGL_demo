#include "demo_base.h"
#include "LoadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ply_file.h"
#include "lights.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

class MainApp : public GL_Demo_Base{
public:
	MainApp(){};
	~MainApp(){};
	void Init(int argc, char** argv);

	void ResizeWindow(int width, int hight);
	void CalculateMVP();

	static	MainApp*	p_mApp;
	glm::mat4			MVP;

	LightInfo			mLight;
	MaterialInfo		mMaterial;

protected:
	static void ResizeViewportFunc(int width, int hight);
	static void RenderFunc(void);
	static void IdleFunc(void);

private:
	void InitViewMtrx();
	void getProjectionMtrx();
	void getViewMtrx();
	void updateRotate_Y();
	void calcuateNormalMtrx();

	glm::mat4 ProjectionMtrx;
	glm::mat4 ViewMtrx;
	glm::mat4 ModelMtrx = glm::mat4(1.0f);
	glm::mat3 NormalMtrx = glm::mat3(1.0f);

	glm::mat4 transMtrx = glm::mat4(1.0f);
	glm::mat4 scaleMtrx = glm::mat4(1.0f);
	glm::mat4 rotateMtrx = glm::mat4(1.0f);
	glm::mat4 lookAt_Mtrx = glm::mat4(1.0f);

	GLuint t = 0;
	GLuint slow_tag;
}app;

GLuint	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId = 0,
	VboId,
	PositionBufferId,
	NormalBufferId;

static Model_PLY* model = new Model_PLY;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);


	glutReshapeFunc(ResizeViewportFunc);
	glutDisplayFunc(RenderFunc);
	glutIdleFunc(IdleFunc);


	static ShaderInfo shader_info[] = {
		{ GL_VERTEX_SHADER, "../shader/test_light.vert", 0 },
		{ GL_FRAGMENT_SHADER, "../shader/test_light.frag", 0 },
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
	
	model->Load("../model/bunny.ply");

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * (model->numFaces), model->faceTriangles, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NormalBufferId);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * (model->numFaces), model->normals, GL_STATIC_DRAW);

	InitLight(mLight);
	InitMaterial(mMaterial);

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

	GLuint model_view_loc = glGetUniformLocation(ProgramId, "ModelViewMtrx");
	glUniformMatrix4fv(model_view_loc, 1, GL_FALSE, &app.ViewMtrx[0][0]);

	GLuint model_loc = glGetUniformLocation(ProgramId, "ModelMtrx");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, &app.ModelMtrx[0][0]);

	// share light info to shader.
	GLuint light_pos_loc = glGetUniformLocation(ProgramId, "light.position");
	glUniform4fv(light_pos_loc, 1, &app.mLight.position[0]);
	GLuint light_Ld_loc = glGetUniformLocation(ProgramId, "light.Ld");
	glUniform3fv(light_Ld_loc, 1, &app.mLight.Ld[0]);
	GLuint light_La_loc = glGetUniformLocation(ProgramId, "light.La");
	glUniform3fv(light_La_loc, 1, &app.mLight.La[0]);
	GLuint light_Ls_loc = glGetUniformLocation(ProgramId, "light.Ls");
	glUniform3fv(light_Ls_loc, 1, &app.mLight.Ls[0]);

	// share material info to shader
	GLuint mtr_Kd_loc = glGetUniformLocation(ProgramId, "material.Kd");
	glUniform3fv(mtr_Kd_loc, 1, &app.mMaterial.Kd[0]);
	GLuint mtr_Ka_loc = glGetUniformLocation(ProgramId, "material.Ka");
	glUniform3fv(mtr_Ka_loc, 1, &app.mMaterial.Ka[0]);
	GLuint mtr_Ks_loc = glGetUniformLocation(ProgramId, "material.Ks");
	glUniform3fv(mtr_Ks_loc, 1, &app.mMaterial.Ks[0]);
	GLuint mtr_shns_loc = glGetUniformLocation(ProgramId, "material.shininess");
	glUniform1f(mtr_shns_loc, app.mMaterial.shininess);

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
	ProjectionMtrx = glm::perspectiveLH(45.0f, 1024.0f / 720.0f, 1.0f, 80.0f);
}

void MainApp::InitViewMtrx()
{
	glm::mat4 tempR, tempT, tempS;
	rotateMtrx = glm::rotate(tempR, 90.0f, vec3(0.0f, 1.0f, 0.0f));
	transMtrx = glm::translate(tempT, vec3(0.0f, -3.0f, -5.0f));
	scaleMtrx = glm::scale(tempS, vec3(20.0f));

	lookAt_Mtrx = glm::lookAtLH(vec3(0.0f, 0.0f, 0.01f), vec3(0.0f, -1.0f, -5.0f), vec3(0.0f, 1.0f, 0.0f));
}

void MainApp::getViewMtrx()
{
	ViewMtrx = lookAt_Mtrx * transMtrx * rotateMtrx * scaleMtrx;
}

void MainApp::CalculateMVP()
{
	MVP = ProjectionMtrx * ViewMtrx * ModelMtrx;
}

void MainApp::updateRotate_Y()
{
	t++;

	double tem = double((t & 0x1FFF)) / double(0x1FFF);

	glm::mat4(temp);

	rotateMtrx = glm::rotate_slow(temp, (float)tem*36.0f, vec3(0.0f, 1.0f, 0.0f));
}

void MainApp::calcuateNormalMtrx()
{

}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}
