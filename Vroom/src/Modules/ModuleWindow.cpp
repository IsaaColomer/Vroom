#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	OUR_LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		OUR_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth, winHeight, flags);

		if(window == NULL)
		{
			OUR_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	OUR_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}
void ModuleWindow::SetFullscreen(bool fullscreen)
{
	Uint32 flags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
	SDL_SetWindowFullscreen(App->window->window, flags);

}
void ModuleWindow::SetFullscreenDesktop(bool fullscreenDesktop)
{
	Uint32 flags = (fullscreenDesktop) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
	SDL_SetWindowFullscreen(App->window->window, flags);
}
void ModuleWindow::SetResizable(bool resizable)
{
	//Uint32 flags = (resizable) ? SDL_WINDOW_RESIZABLE : 0;
	//SDL_SetWindowRes
}
void ModuleWindow::SetBorderless(bool borderless)
{
	SDL_SetWindowBordered(App->window->window, (SDL_bool)borderless);
}
void ModuleWindow::SetWindowSize()
{
	SDL_SetWindowSize(window, (int)winWidth, (int)winHeight);
}

void ModuleWindow::SetWindowBrightness()
{
	SDL_SetWindowBrightness(App->window->window, (float)brightness);
}
