#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	Cube c1(20,5,100);
	Cube c2(10,5,100);
	Cube c3(5,5,100);
	Cube c4(100,5,10);
	Cube c5(10, 10, 20);

	c5.SetRotation(-15, { 1,0,0 });
	
	cube = App->physics->AddBody(c1,0);
	cube2 = App->physics->AddBody(c2,0);
	cube3 = App->physics->AddBody(c3,0);
	cube4 = App->physics->AddBody(c4,0);
	cube5 = App->physics->AddBody(c5,0);

	cube->SetPos(0, 0, 0);
	cube2->SetPos(0, 0, 100);
	cube3->SetPos(0, 0, 200);
	cube4->SetPos(-50, 0, 200);
	cube5->SetPos(0, 0, 100);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	Cube cube_road(20,5,100);
	Cube cube_road2(10,5,100);
	Cube cube_road3(5,5,100);
	Cube cube_road4(100, 5, 10);
	Cube cube_road5(10, 10, 20);

	p.axis = true;
	p.Render();
	
	cube_road.wire = false;
	cube_road.color = Blue;
	cube_road.Render();

	cube_road2.wire = false;
	cube_road2.SetPos(0,0,100);
	cube_road2.color = Red;
	cube_road2.Render();

	cube_road3.wire = false;
	cube_road3.SetPos(0, 0, 200);
	cube_road3.color = Green;
	cube_road3.Render();

	cube_road4.wire = false;
	cube_road4.SetPos(-50, 0, 200);
	cube_road4.color = Green;
	cube_road4.Render();

	cube_road5.wire = false;
	cube_road5.SetPos(0, 0, 100);
	cube_road5.SetRotation(-15, { 1,0,0 });
	cube_road5.color = Blue;
	cube_road5.Render();


	myCurva pa(0, 0, 0, 10, 5, 180);
	pa.wire = false;
	pa.color = White;
	pa.SetPos(50, 0, 50);
	pa.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	
}

