#include "Globals.h"
#include "Application.h"
#include "Editor.h"

#include <stdlib.h>
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
    ms_log.push_back(0.f);

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


    if (ImGui::BeginMenu("Options"))
    {
        //if (ImGui::MenuItem("Frames Per Second"))
        //{
        //    showFps = !showFps;
        //}
        if (ImGui::MenuItem("Settings"))
        {
            settings = !settings;
        }
        ImGui::EndMenu();
    }
    // ------------------- PRINT  FPS -------------------

    if (settings)
    {
        ImGui::Begin("General Options", &settings);
        if (ImGui::CollapsingHeader("Application Settings"))
        {
            sprintf_s(configName, 25, ENGINENAME);
            ImGui::InputText("App Name", configName, 25);

            sprintf_s(configName, 25, UNIVERSITY);
            ImGui::InputText("University Name", configName, 25);

            if (ImGui::CollapsingHeader("Performace Information"))
            {
                sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
                ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
                sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
                ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
            }


        }
        if (ImGui::CollapsingHeader("Screen Settings"))
        {
            if (ImGui::Checkbox("\tToggle Fullscreen", &fullscreen))
            {
                App->window->SetFullscreen(fullscreen);
            }
            if (ImGui::Checkbox("\tToggle Resizable", &resizable))
            {
                App->window->SetResizable(resizable);
            }
        }

        ImGui::End();
    }

    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("GUI Demo"))
        {
            showDemoWindow = !showDemoWindow;
        }
        if (ImGui::MenuItem("Documentation"))
        {
            App->RequestBrowser("https://github.com/IsaaColomer/Vroom/wiki");
        }
        if (ImGui::MenuItem("Download latest"))
        {
            App->RequestBrowser("https://github.com/IsaaColomer/Vroom");
        }
        if (ImGui::MenuItem("Report a bug"))
        {
            App->RequestBrowser("https://github.com/IsaaColomer/Vroom/issues");
        }
        if (ImGui::MenuItem("About"))
        {
            App->RequestBrowser("https://google.com");
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Exit"))
    {
        if (ImGui::MenuItem("Close"))
        {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    // ------------------- CALLING THE FPS FUNCTION -------------------
    CalculateFrames(&fps_log, dt, columns);

    // ------------------- CALLING THE MS FUNCTION -------------------
    CalculateMilliseconds(&ms_log, dt, columns);

    ImGui::EndMainMenuBar();

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

void  Editor::CalculateFrames(std::vector<float>* fps_log, float dt, int columns)
{
    // ------------------- CALCULATE  FPS -------------------
    if (fps_log->size() <= columns)
        fps_log->push_back(1 / dt);
    else
    {
        fps_log->erase(fps_log->begin());
        fps_log->push_back(1 / dt);
    }
}
void  Editor::CalculateMilliseconds(std::vector<float>* ms_log, float dt, int columns)
{
    // ------------------- MILLISECONDS  FPS -------------------
    if (ms_log->size() <= columns)
        ms_log->push_back(dt*1000);
    else
    {
        ms_log->erase(ms_log->begin());
        ms_log->push_back(dt * 1000);
    }
}