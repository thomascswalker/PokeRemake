#include "Application/Application.h"
#include "Application/ArgParser.h"
#include "EditorGame.h"
#include "EditorHUD.h"

int main(int argc, char** argv)
{
	const auto Args = ArgParser::Parse(argc, argv);
	const auto App  = PApplication::GetInstance();

	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor))
	{
		if (!App->Start<PEditorGame, PEditorHUD>())
		{
			return 1;
		}
		while (App->IsRunning())
		{
			if (!App->Loop())
			{
				break;
			}
		}
	}
	else
	{
		return 1;
	}

	return 0;
}
