#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	void SetWindowSize();
	void SetWindowBrightness();
	void SetFullscreen(bool fullscreen);
	void SetFullscreenDesktop(bool fullscreenDesktop);
	void SetResizable(bool resizable);
	void SetBorderless(bool resizable);

	float winWidth;
	float winHeight;
	float brightness;
};

#endif // __ModuleWindow_H__