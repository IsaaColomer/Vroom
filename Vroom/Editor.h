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

    void RecolVector(std::vector<float>* vec);

    bool showDemoWindow = false;
    bool showSmallExample = true;
    bool closeWindow = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::vector<float> fps_log;

};