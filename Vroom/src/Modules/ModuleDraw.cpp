#include "Globals.h"
#include "glew.h"
#include "Application.h"
#include "ModuleDraw.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
ModuleDraw::ModuleDraw(Application* app, bool start_enabled) : Module(app, start_enabled)
{	
}

// Destructor
ModuleDraw::~ModuleDraw()
{
}

// Called before render is available
bool ModuleDraw::Init()
{
	OUR_LOG("Init SDL window & surface");
	bool ret = true;
	GLenum err = glewInit();

	OUR_LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	return ret;
}
update_status ModuleDraw::Update(float dt)
{


	return UPDATE_CONTINUE;
}
// Called before quitting
bool ModuleDraw::CleanUp()
{

	return true;
}
