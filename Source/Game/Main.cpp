#include <memory>

#include <Platform/SDL/SDLPlatform.h>

int main(int argc, char** argv)
{
	const auto Platform = std::make_unique<SDLPlatform>();

	if (Platform->OnStart(argc, argv))
	{
		while (Platform->IsRunning())
		{
			Platform->OnLoop();
		}
	}

	return 0;
}