// #define SDL_MAIN_USE_CALLBACKS

#include <memory>

#include <Platform/SDL/SDLPlatform.h>

std::unique_ptr<IPlatform> gPlatform = nullptr;

int main(int argc, char** argv)
{
	gPlatform = std::make_unique<SDLPlatform>();

	if (gPlatform->OnStart(argc, argv))
	{
		while (gPlatform->IsRunning())
		{
			gPlatform->OnLoop();
		}
	}

	return 0;
}