#include <memory>

#include "Platform/SDL/SDLPlatform.h"
#include "TestGame.h"

int main(int argc, char** argv)
{
	CONSTRUCT_AND_START_GAME(SDLPlatform, TestGame);
	return 0;
}