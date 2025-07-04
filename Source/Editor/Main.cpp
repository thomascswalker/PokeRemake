#include "Application/Application.h"
#include "Application/ArgParser.h"
#include "EditorGame.h"

int main(int argc, char** argv)
{
	const auto Args = ArgParser::Parse(argc, argv);
	const auto App = PApplication::GetInstance();

	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor))
	{
		App->Start<PEditorGame>();
		while (App->IsRunning())
		{
			App->Loop();
		}
	}
	else
	{
		return 1;
	}

	return 0;
}