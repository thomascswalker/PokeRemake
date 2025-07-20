#include "TestGame.h"

#include "Application/Application.h"
#include "Application/ArgParser.h"

int main(int argc, char** argv)
{
	const auto Args = ArgParser::Parse(argc, argv);
	const auto App = PApplication::GetInstance();

	if (App->Initialize(Args.WindowFlags, Args.GPUMode, Args.IsEditor))
	{
		App->Start<TestGame>();
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