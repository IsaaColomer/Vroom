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



private:
    char title[25];
    void RecolVector(std::vector<float>* vec);

    bool showDemoWindow = false;
    bool showFps = false;
    bool showSmallExample = true;
    bool closeWindow = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int columns;
    std::vector<float> fps_log;
    std::vector<float> ms_log;

};