#include "Game.h"

#include "Core/Logging.h"

#include <memory>

PGame::PGame()
{
	mWorld = std::make_shared<PWorld>();
	mSettings = std::make_shared<PSettings>();
	mSettings->Initialize();
}

void PGame::Start()
{
	mWorld->Start();
	FindActiveCamera();
}

void PGame::Tick(float DeltaTime)
{
	mWorld->Tick(DeltaTime);
}

void PGame::FindActiveCamera()
{
	for (auto Comp : mWorld->GetComponents())
	{
		// Just set the first camera component found as the active camera
		if (auto CameraComp = dynamic_cast<PCameraComponent*>(Comp))
		{
			mActiveCameraView = CameraComp->GetCameraView();
			LogDebug("Found active camera");
			return;
		}
	}

	LogError("No camera found");
}
