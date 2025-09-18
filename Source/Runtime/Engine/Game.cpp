#include "Game.h"

#include <memory>

#include "Core/CSS.h"
#include "Core/Logging.h"

PGame::PGame()
{
	mWorld = std::make_shared<PWorld>();
	mSettings = std::make_shared<PSettings>();
}

bool PGame::PreStart()
{
	TextureManager::LoadAllTextures();
	TilesetManager::LoadAllTilesets();
	StyleManager::LoadAllStylesheets();

	return true;
}

bool PGame::Start()
{
	if (!this->PreStart())
	{
		LogError("Failed to start game");
		return false;
	}

	if (mGameStarted)
	{
		LogWarning("Game is already started.");
		return false;
	}
	assert(mWorld != nullptr);
	mWorld->Start();
	assert(mGameMode != nullptr);
	if (!LoadCurrentGameMode())
	{
		return false;
	}

	mGameStarted = true;
	GameStarted.Broadcast();
	return true;
}

bool PGame::End()
{
	GameEnded.Broadcast();
	return true;
}

void PGame::Tick(float DeltaTime)
{
	mWorld->Tick(DeltaTime);
}

void PGame::PostTick()
{
	mWorld->PostTick();
}

void PGame::UpdateCameraView()
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
		case SDLK_F1:
			mSettings->DebugDraw = !mSettings->DebugDraw;
			Event->Consume();
			break;
		default:
			break;
	}
}

bool PGame::ProcessEvents(SInputEvent* Event)
{
	// Handle events for the game
	if (IInputHandler::ProcessEvents(Event))
	{
		return true;
	}

	// Handle game mode events
	if (mGameMode->ProcessEvents(Event))
	{
		return true;
	}

	// Handle world events
	return mWorld->ProcessEvents(Event);
}

bool PGame::SetCurrentGameMode(const std::string& Name)
{
#if _DEBUG
	if (!mGameModes.Contains(Name))
	{
		LogError("Target game mode {} does not exist.", Name.c_str());
		return false;
	}
#endif

	// Get the new game mode
	auto NewGameMode = GetGameMode(Name);

	// Unload the current game mode
	if (mGameMode != nullptr && mGameMode != NewGameMode && mGameMode->GetLoaded())
	{
		if (!mGameMode->Unload())
		{
			LogError("Failed to unload game mode.");
			return false;
		}
		mGameMode->SetLoaded(false);
		OnGameModeUnloaded(mGameMode);
	}

#if _DEBUG
	if (!NewGameMode)
	{
		LogError("New game mode {} cannot be retrieved.", Name.c_str());
		return false;
	}
#endif
	mGameMode = NewGameMode;
	return true;
}

bool PGame::LoadCurrentGameMode()
{
	mGameMode->Start();
	if (mGameMode->GetLoaded())
	{
		LogWarning("Game mode is already loaded.");
		return true;
	}
	if (!mGameMode->Load())
	{
		LogError("Failed to load game mode.");
		return false;
	}
	mGameMode->SetLoaded(true);
	OnGameModeLoaded(mGameMode);

	return true;
}

bool PGame::SetAndLoadCurrentGameMode(const std::string& Name)
{
	if (!SetCurrentGameMode(Name))
	{
		return false;
	}
	return LoadCurrentGameMode();
}

void PGame::OnGameModeLoaded(PGameMode* GameMode)
{
	UpdateCameraView();
}

void PGame::OnGameModeUnloaded(PGameMode* GameMode) {}
