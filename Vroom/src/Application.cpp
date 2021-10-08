#include "Application.h"
#include "shellapi.h"
#include "parson.h"

Application::Application()
{
	const char* glsl_version = "#version 100";

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene_intro = new ModuleSceneIntro(this);
	editor = new Editor(this);
	draw = new ModuleDraw(this);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);
	AddModule(draw);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);
}

Application::~Application()
{
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;
	LoadEditorConfiguration();
	// Call Init() in all modules
	for (auto& i : list_modules)
	{
		i->Init();
	}
	// After all Init calls we call Start() in all modules
	OUR_LOG("Application Start --------------");
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

void Application::SaveEditorConfiguration()
{
	// ------------------- CREATE FILE -------------------
	JSON_Value *config = json_parse_file("editor_config.json");
	config = json_value_init_object();

	// ------------------- SAVE INFORMATION -------------------
	json_object_set_number(json_object(config), "max_fps", maxFps);
	json_object_dotset_number(json_object(config), "window.width", window->winWidth);
	json_object_dotset_number(json_object(config), "window.height", window->winHeight);
	json_object_dotset_number(json_object(config), "window.brightness", window->brightness);

	// ------------------- CLOSE FILE -------------------
	json_serialize_to_file(config, "editor_config.json");
	json_value_free(config);
}
void Application::LoadEditorConfiguration()
{
	// ------------------- LOAD FILE -------------------
	JSON_Value* root_value = json_parse_file("editor_config.json");
	
	if (root_value == nullptr)
	{
		OUR_LOG("FILE editor_config.json couldn't be loaded\n");
		maxFps = 60;

		window->winWidth = 1280;
		window->winHeight = 720;

		window->brightness = 1;
	}
	else
	{
		JSON_Object* root_object = json_value_get_object(root_value);
		OUR_LOG("LODAING...\n");

		maxFps = (int)json_object_get_number(root_object, "max_fps");

		window->winWidth = (float)json_object_dotget_number(json_object(root_value), "window.width");
		window->winHeight = (float)json_object_dotget_number(root_object, "window.height");

		window->brightness = (float)json_object_dotget_number(root_object, "window.brightness");

		OUR_LOG("%f", window->winWidth);
		char* serialized_string = json_serialize_to_string_pretty(root_value);
		OUR_LOG("%s\n", serialized_string);
		json_free_serialized_string(serialized_string);

		window->SetWindowSize();
		window->SetWindowBrightness();
	}

	json_value_free(root_value);
}