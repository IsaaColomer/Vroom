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
	CreateGameObject("BakerHouse", root, "Assets/BakerHouse.fbx", "Assets/bakeHouse.png");
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
{		Primitive::Plane p(0, 1, 0, 0);
		p.axis = true;
		p.Render();
		//Primitive::Cube c(1, 1, 1);
		//c.SetPos(10, 10, 10); //NO FUCNIONA
		//c.axis = true;
		//c.wire = true;
		//c.Render();
		//Primitive::Pyramid py(2, 2);
		//py.Render();
		//Primitive::Sphere s(1.0f, 10, 20, true);
		//s.Render();
		//Primitive::Cylinder cc;
		//cc.Render();


		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			LoadCube();
		}
		if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		{
			LoadSphere();
		}
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
		{
			LoadCylinder();
		}

		root->Update();
		//mesh.DrawWithTexture();
		return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(const char* name, GameObject* parent, const char* Filename, const char* Textname)
{
	GameObject* n = new GameObject(name, parent);
	parent->gameObjects.push_back(n);
	n->CreateComponent(Component::Type::MESH);
	n->CreateComponent(Component::Type::TRANSFORM);
	n->CreateComponent(Component::Type::MATERIAL);
	dynamic_cast<Meshs*>(n->GetComponent(Component::Type::MESH))->LoadMesh(Filename);
	dynamic_cast<Materialss*>(n->GetComponent(Component::Type::MATERIAL))->LoadTextures(Textname);

	return n;
}

void ModuleSceneIntro::LoadCube()
{
	CreateGameObject("default_cube", root, "Assets/default_cube.fbx", "Assets/lenna.png");
}

void ModuleSceneIntro::LoadSphere()
{
	CreateGameObject("default_sphere", root, "Assets/default_sphere.fbx", "Assets/lenna.png");
}

void ModuleSceneIntro::LoadCylinder()
{
	CreateGameObject("default_cylinder", root, "Assets/default_cylinder.fbx", "Assets/lenna.png");
}