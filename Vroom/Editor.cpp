#include "Globals.h"
#include "Application.h"
#include "Editor.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "SDL\include\SDL_opengl.h"



Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Editor::~Editor()
{
}

bool Editor::Start()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();


    columns = 64;
    fps_log.push_back(0.f);
    return true;
}

update_status Editor::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // ------------------- TOP MENU -------------------
    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("Exit"))
    {
        if (ImGui::MenuItem("Close"))
        {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Options"))
    {
    // ------------------- PRINT  FPS -------------------
        ImGui::Begin("Frames Per Second", &showFps);
        sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
        ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
        ImGui::End();
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("GUI Demo"))
        {
            showDemoWindow = !showDemoWindow;
        }
        if (ImGui::MenuItem("Documentation"))
        {
            App->RequestBrowser("https://google.com");
        }
        if (ImGui::MenuItem("Download latest"))
        {
            App->RequestBrowser("https://google.com");
        }
        if (ImGui::MenuItem("Report a bug"))
        {
            App->RequestBrowser("https://google.com");
        }
        if (ImGui::MenuItem("About"))
        {
            App->RequestBrowser("https://google.com");
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    // ------------------- CALCULATE  FPS -------------------
    if (fps_log.size() <= columns)
        fps_log.push_back(1 / dt);
    else
    {
        fps_log.erase(fps_log.begin());
        fps_log.push_back(1 / dt);
    }


    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return UPDATE_CONTINUE;
}

bool Editor::CleanUp()
{
    return true;
}

void Editor::RecolVector(std::vector<float>* vec)
{
    vec->erase(vec->begin());
}