#include "TestGame.h"

#include "Application/Application.h"

int main(int argc, char** argv)
{
	const auto App = PApplication::GetInstance();
	if (App->Initialize())
	{
		App->Start<TestGame>();
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