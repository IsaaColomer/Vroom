﻿#include "Globals.h"
#include "Application.h"
#include "Editor.h"
#include "ModuleInput.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include "imgui.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "SDL_opengl.h"

extern std::list<std::string> logH;

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

    // ------------------- READ THE ABOUT SECTION FROMT TXT FILE -------------------
    std::ifstream aboutFile;
    aboutFile.open("About.txt");
    std::string str;
    while (std::getline(aboutFile, str))
    {
        aboutText += str += "\n";
    }
    aboutFile.close();

    return true;
}
void Editor::RecursiveTree(GameObject& go)
{
    ImGuiTreeNodeFlags flags = 0;
    bool node_open = ImGui::TreeNodeEx(&go, flags, go.name.c_str());
    if (node_open)
    {
        for (unsigned int i = 0; i < go.gameObjects.size(); i++)
        {
            RecursiveTree(*go.gameObjects[i]);
        }
        ImGui::TreePop();
    }
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
                if (ImGui::SliderInt("Frame Rate Limit", &App->maxFps, 1.0f, 144.0f))
                {
                }
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

            }
            ImGui::SameLine();
            if (ImGui::Checkbox("\tToggle FullscreenDesktop", &fullscreenDesktop))
            {
                App->window->SetFullscreenDesktop(fullscreenDesktop);
            }
            if (resizable)
            {
                if (ImGui::SliderFloat("Width", &App->window->winWidth, 500, 1920))
                {
                    App->window->SetWindowSize();
                }
                if (ImGui::SliderFloat("Height", &App->window->winHeight, 500, 1080))
                {
                    App->window->SetWindowSize();
                }
            }
            if (ImGui::Checkbox("\tToggle Borderless", &borderless))
            {
                App->window->SetResizable(borderless);
            }
            if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f))
            {
                App->window->SetWindowBrightness();
            }
        }
        if (ImGui::CollapsingHeader("System Information"))
        {
            ImGui::Text("Ram memory of the System: ");
            ImGui::SameLine();
            ImGui::TextColored({255,255,0,1},"%d Gb", SDL_GetSystemRAM() / 1024);
            ImGui::Text("CPU threats count:");
            ImGui::SameLine();
            ImGui::TextColored({255,255,0,1},"%d", SDL_GetCPUCount());
            ImGui::Text("GPU: ");
            ImGui::SameLine();
            ImGui::TextColored({ 255,255,0,1 }, "%s", glGetString(GL_RENDERER));
            SDL_version version;
            SDL_GetVersion(&version);
            ImGui::Text("SDL Version:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch);
            ImGui::Text("OpenGL Version:");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", glGetString(GL_VERSION));
        }
        if (ImGui::Button("Save", { 50, 50 }))
        {
            App->SaveEditorConfiguration();
            App->LoadEditorConfiguration();
        }
        ImGui::SameLine();
        if (ImGui::Button("Console", { 50,50 }))
        {
            console = !console;
        }
        if (console)
        {
            ImGui::Begin("Console", &console);
            for (auto& a : logH)
            {
                ImGui::TextWrapped(a.c_str());
            }
            ImGui::End();
        }
        ImGui::End();
    }
    if (ImGui::BeginMenu("Inspector"))
    {
        inspector = !inspector;
        ImGui::EndMenu();
    }
    if (inspector)
        InspectorDraw();

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
            showAboutWindow = !showAboutWindow;
        }

        ImGui::EndMenu();
    }

    if (showAboutWindow)
    {
        ImGui::Begin("About", &showAboutWindow);
        ImGui::Text("%s", aboutText.c_str());
        ImGui::End();
    }
    if (ImGui::BeginMenu("Exit"))
    {
        if (ImGui::MenuItem("Close"))
        {
            return UPDATE_STOP;
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Pito"))
    {
        if (ImGui::MenuItem("Inspector"))
        {
            
        }

        if (ImGui::MenuItem("Hierarchy"))
        {
            hier = !hier;
        }
        
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Create"))
    {
        if (ImGui::MenuItem("Cube"))
        {
            App->scene_intro->LoadCube();
        }
        if (ImGui::MenuItem("Sphere"))
        {
            App->scene_intro->LoadSphere();
        }
        if (ImGui::MenuItem("Pyramid"))
        {
            App->scene_intro->LoadPyramid();
        }

        ImGui::EndMenu();
    }
    if (hier)
    {
        ImGui::Begin("Hierarchy", &hier);
        if (App->scene_intro->root != nullptr)
        {
                RecursiveTree(*App->scene_intro->root);
        }
        ImGui::End();
    }

    // ------------------- CALLING THE FPS FUNCTION -------------------
    CalculateFrames(&fps_log, dt, columns);

    // ------------------- CALLING THE MS FUNCTION -------------------
    CalculateMilliseconds(&ms_log, dt, columns);

    ImGui::EndMainMenuBar();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
   //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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

void Editor::InspectorDraw()
{
    if (ImGui::Begin("Inspector"), &inspector)
    {
        ImGui::Text("Velocity: 0.00 0.00 0.00 (0.00 m/s)");
        if (App->scene_intro->root != nullptr)
        {
            Transform* t = dynamic_cast<Transform*>(App->scene_intro->asd->GetComponent(Component::Type::TRANSFORM));
            if (t != nullptr)
            {
                if (ImGui::CollapsingHeader("Local Transformation"))
                {
                    if (ImGui::SliderFloat3("Position", &t->position, -50,50)) t->updateTransform = true;
                    if (ImGui::SliderFloat3("Rotation", &t->rotation, -180, 180)) t->updateTransform = true;
                    if (ImGui::SliderFloat3("Scale", &t->scale, 0,50)) t->updateTransform = true;
                    ImGui::Text("Bounding Box: -not generated-");
                    ImGui::Text("Velocity: 0.00 0.00 0.00 (0.00 m/s)");
                }
            }
        }

        ImGui::End();
    }
}