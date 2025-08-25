#include "Game.h"

#include "Core/Logging.h"

#include <memory>

PGame::PGame()
{
	mWorld    = std::make_shared<PWorld>();
	mSettings = std::make_shared<PSettings>();
}

bool PGame::PreStart()
{
	mWorld->GetHUD()->PreStart();
	return true;
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

void PGame::PostTick()
{
	mWorld->PostTick();
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

void PGame::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
	case SDLK_F1: mSettings->DebugDraw = !mSettings->DebugDraw;
		Event->Consume();
		break;
	default: break;
	}
}
