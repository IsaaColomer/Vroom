#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ModuleFileSystem.h"
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	char* path = nullptr;
	Mesh mesh;
	bool CleanUp();
public:

};