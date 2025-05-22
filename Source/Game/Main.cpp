// #define SDL_MAIN_USE_CALLBACKS

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

SDL_Window* gWindow;

SDL_AppResult SDL_AppInit(void** AppState, int argc, char** argv)
{
	SDL_LogDebug(0, "App is starting.");
	SDL_Log("App is starting");
	// create a window
	gWindow = SDL_CreateWindow("Hello, Triangle!", 960, 540, SDL_WINDOW_RESIZABLE);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* AppState) { return SDL_APP_CONTINUE; }

SDL_AppResult SDL_AppEvent(void* AppState, SDL_Event* Event)
{
	// close the window on request
	if (Event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
	{
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* AppState, SDL_AppResult Result)
{
	// destroy the window
	SDL_DestroyWindow(gWindow);
}

int main(int argc, char** argv)
{
	return SDL_EnterAppMainCallbacks(argc, argv, SDL_AppInit, SDL_AppIterate, SDL_AppEvent, SDL_AppQuit);
}