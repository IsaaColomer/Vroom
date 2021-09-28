#include "Application.h"
#include "shellapi.h"

Application::Application()
{
	const char* glsl_version = "#version 100";

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene_intro = new ModuleSceneIntro(this);
	editor = new Editor(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(editor);
	AddModule(renderer3D);
}

Application::~Application()
{
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;
	maxFps = 60;
	// Call Init() in all modules
	for (auto& i : list_modules)
	{
		i->Init();
	}
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto& i : list_modules)
	{
		i->Start();
	}
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	float k = (1000 / maxFps) - dt;
	if (k>0)
	{
		SDL_Delay((Uint32)k);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();


	for (auto& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->PreUpdate(dt);
		}
	}

	for (auto& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->Update(dt);
		}
	}

	for (auto& i : list_modules)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = i->PostUpdate(dt);
		}
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator i = list_modules.rbegin(); i != list_modules.rend(); ++i)
	{
		if (ret)
		{
			ret = (*i)->CleanUp();
		}
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

bool Application::RequestBrowser(const char* path)
{
	ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWNORMAL);
	return true;
}