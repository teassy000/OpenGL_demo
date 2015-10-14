#include "demo_base.h"
#include "LoadShaders.h"

#define WINDOW_TITLE_PREFIX "First_Runnable_Demo"

GLuint
	VertexShaderId,
	FragmentShaderId,
	ProgramId,
	VaoId,
	VboId,
	ColorBufferId;



class MainApp : public GL_Demo_Base{
public:
	MainApp(){};
	~MainApp(){};
	void Init(int argc, char** argv);
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
}


int main(int argc, char** argv)
{
	app.Init(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}