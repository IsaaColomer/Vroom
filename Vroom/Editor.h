#pragma once
#include "Module.h"
#include <vector>

class Editor : public Module
{
public:
    Editor(Application* app, bool start_enabled = true);
    ~Editor();

    bool Start();
    update_status Update(float dt);
    bool CleanUp();
    void CalculateFrames(std::vector<float>* fps_log, float dt, int columns);
    void CalculateMilliseconds(std::vector<float>* ms_log, float dt, int columns);


private:
    char title[25];
    char configName[25];

    bool showDemoWindow = false;
    bool showFps = false;
    bool showSmallExample = true;
    bool closeWindow = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //CONFIG
    bool fullscreen = false;
    bool resizable = false;
    bool settings = false;


    int columns;
    std::vector<float> fps_log;
    std::vector<float> ms_log;

};