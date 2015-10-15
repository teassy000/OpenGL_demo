#include "demo_base.h"
#include "LoadShaders.h"
#include "vmath.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

GLuint
	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId,
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
public:
	MainApp(){};
	~MainApp(){};
	void Init(int argc, char** argv);
	virtual void Render(void);
}app;

void MainApp::Init(int argc, char** argv)
{
	GL_Demo_Base::Init(argc, argv);


	static ShaderInfo shader_info[] = {
		{ GL_VERTEX_SHADER, "../shader/test.vert", 0 },
		{ GL_FRAGMENT_SHADER, "../shader/test.frag", 0 },
		{ GL_NONE, "", 0 }
	};

	ProgramId = LoadShader(shader_info);

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



}

void MainApp::Render(void)
{
	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	

	
}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}