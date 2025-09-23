#include "../Runtime/Interface/Game/GameHUD.h"
#include "Application/Application.h"
#include "Application/ArgParser.h"

#include "MainGame.h"

int main(int argc, char** argv)
{
	const auto Args = ArgParser::Parse(argc, argv);
	const auto App = PApplication::GetInstance();
	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor))
	{
		if (!App->Start<PMainGame, PGameHUD>())
		{
			LogError("Failed to start application.");
			return 1;
		}
		while (App->IsRunning())
		{
			if (!App->Loop())
			{
				break;
			}
		}
		if (!App->Stop())
		{
			LogError("Failed to stop application.");
			return 1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
	;
}
