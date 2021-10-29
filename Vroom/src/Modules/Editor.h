#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include "ModuleSceneIntro.h"

class Editor : public Module{
public:
    Editor(Application* app, bool start_enabled = true);
    ~Editor();

    bool Start();
    void RecursiveTree(GameObject* go);
    update_status Update(float dt);
    bool CleanUp();
    void CalculateFrames(std::vector<float>* fps_log, float dt, int columns);
    void CalculateMilliseconds(std::vector<float>* ms_log, float dt, int columns);
    void InspectorDraw();
    int w = 300;
    int h = 500;
    int cw = 300;
    int ch = 200;
private:
    char title[25];
    char configName[25];
    std::string aboutText;


    bool showDemoWindow = false;
    bool showAboutWindow = false;
    bool hier = true;
    bool inspector = true;
    bool showFps = false;
    bool showSmallExample = true;
    bool closeWindow = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //CONFIG
    bool fullscreen = false;
    bool fullscreenDesktop = false;
    bool resizable = false;
    bool borderless = false;
    bool settings = false;
    bool console = true;
    bool showConsole = false;


    GameObject* selectedNode;
    int columns;
    std::vector<float> fps_log;
    std::vector<float> ms_log;
};