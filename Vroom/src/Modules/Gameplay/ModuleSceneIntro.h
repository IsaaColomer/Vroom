#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "GameObject.h"
class GameObject;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	char* path = nullptr;
	GameObject* root = nullptr;
	bool open = true;
	void LoadCube();
	void LoadSphere();
	void LoadCylinder();
	//Texture tex;
	bool CleanUp();

	GameObject* CreateGameObject(const char* name, GameObject* Parent, const char* Filename, const char* Textname);
};