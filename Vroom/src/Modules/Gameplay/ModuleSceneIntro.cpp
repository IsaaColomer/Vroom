#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
//#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{

	//LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	warrior.LoadMesh("Assets/cube2.fbx");
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	Primitive::Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	Primitive::Cube c(1, 1, 1);
	c.SetPos(10, 10, 10); //NO FUCNIONA
	//c.axis = true;
	//c.wire = true;
	//c.Render();
	Primitive::Pyramid py(2, 2);
	//py.Render();
	Primitive::Sphere s(1.0f, 10, 20, true);
	//s.Render();
	Primitive::Cylinder cc;
	//cc.Render();
	warrior.Render();
	return UPDATE_CONTINUE;
}