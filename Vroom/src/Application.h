#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "Editor.h"
#include "ModuleDraw.h"
#include "ModuleFileSystem.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	Editor* editor;
	ModuleDraw* draw;

	int maxFps;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> listModules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool RequestBrowser(const char* path);

	void LoadEditorConfiguration();
	void SaveEditorConfiguration();

	void LookAt();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};