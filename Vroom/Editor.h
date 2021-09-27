#pragma once
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <gl/GL.h>

class Editor : public Module
{
public:
	Editor(Application* app, bool start_enabled = true);
	~Editor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	bool show_demo_window = false;
	bool show_another_window = false;
	bool help = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};