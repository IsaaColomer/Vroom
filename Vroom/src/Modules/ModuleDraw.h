#ifndef __ModuleDraw_H__
#define __ModuleDraw_H__

#include "Module.h"
#include "SDL/include/SDL.h"


class Application;

class ModuleDraw : public Module
{
public:

	ModuleDraw(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleDraw();
	update_status Update(float dt);
	bool Init();
	bool CleanUp();

public:

};

#endif // __ModuleWindow_H__