#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "MeshComponent.h"
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

	//mesh.LoadTexture("Assets/ddd.dds");
	//mesh.LoadTexture("Assets/abc.png");
	//tex.LoadTexture("Assets/abc.png");
	root = new GameObject("root",nullptr);

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


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		asd = new GameObject("Node1", root);
		root->gameObjects.push_back(asd);
		asd->CreateComponent(Component::Type::MESH);
		asd->CreateComponent(Component::Type::TRANSFORM);
		dynamic_cast<Meshs*>(asd->GetComponent(Component::Type::MESH))->LoadMesh("Assets/default_cube.fbx");

		//"Assets/default_cube.fbx"
	}

	root->Update();
	//mesh.DrawWithTexture();
	return UPDATE_CONTINUE;
}