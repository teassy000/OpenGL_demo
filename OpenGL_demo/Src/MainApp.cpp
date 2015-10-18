#include "demo_base.h"
#include "LoadShaders.h"
#include "vmath.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

GLuint
	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId = 0,
	VboId,
	PositionBufferId,
	ColorBufferId;

GLint render_model_matrix_loc;
GLint render_projection_matrix_loc;

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

	static MainApp* p_mApp;
}app;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);

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
	glBindAttribLocation(ProgramId, 0, "vert_color");
	
	LinkShader(ProgramId, shader_info);

	glUseProgram(ProgramId);


	// "model_matrix" is actually an array of 4 matrices
	render_model_matrix_loc = glGetUniformLocation(ProgramId, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(ProgramId, "projection_matrix");

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}

void MainApp::RenderFunc(void)
{
	FrameCount++;

	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void MainApp::ResizeViewportFunc(int width, int hight)
{
	glViewport(100, 100, width, hight);
}

void MainApp::IdleFunc()
{
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}