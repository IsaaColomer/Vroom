#include "Globals.h"
#include "Application.h"
#include "Editor.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
Editor::~Editor()
{}

// Called before render is available
bool Editor::Init()
{
	bool ret = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(); 
	// Projection matrix for

	return ret;
}

// PreUpdate: clear buffer
update_status Editor::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status Editor::Update(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();


	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
			{
				//showcase = !showcase;
			}
			if (ImGui::MenuItem("Documentation"))
			{
				App->RequestBrowser("https://github.com//wiki");
			}
			if (ImGui::MenuItem("Download latest"))
			{
				App->RequestBrowser("https://github.com//releases");
			}
			if (ImGui::MenuItem("Report a bug TUS MUERTOS"))
			{
				App->RequestBrowser("https://github.com//issues");
			}
			if (ImGui::MenuItem("About"))
			{
				//about->SwitchActive();
			}
			ImGui:EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Close me to close the App!");                          // Create a window called "Hello, world!" and append into it.

		if (ImGui::Button("Exit"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			return UPDATE_STOP;

		ImGui::End();
	}
	{
		ImGui::Begin("Test Window!");
		ImGui::Checkbox("Demo Window", &show_demo_window);
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::End();
	}



	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}
// PostUpdate present buffer to screen
update_status Editor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool Editor::CleanUp()
{
	return true;
}