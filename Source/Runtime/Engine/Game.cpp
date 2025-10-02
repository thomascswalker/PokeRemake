#include "Game.h"

#include <memory>

#include "Core/CSS.h"
#include "Core/Logging.h"

PGameMode* GGameMode = nullptr;

PGame::PGame()
{
	// Construct the world
	mWorld = std::make_shared<PWorld>();
	GWorld = mWorld.get();
}

bool PGame::PreStart()
{
	PTextureManager::LoadAllTextures();
	TilesetManager::LoadAllTilesets();
	StyleManager::LoadAllStylesheets();

	return true;
}

bool PGame::Start()
{
	if (mGameStarted)
	{
		LogWarning("Game is already started.");
		return false;
	}
	assert(mWorld != nullptr);
	mWorld->Start();
	assert(mGameMode != nullptr);

	ASSERT(mGameModes.Size() != 0, "No game modes added.");

	for (auto Mode : mGameModes | std::views::values)
	{
		Mode->Start();
	}

	ASSERT(mGameMode != nullptr, "No game mode set.");
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
	mWorld->End();
	return true;
}

void PGame::Tick(float DeltaTime)
{
	mWorld->Tick(DeltaTime);
}

void PGame::UpdateCameraView()
{
	auto Comps = mWorld->GetComponents();
	for (auto Comp : Comps)
	{
		// Just set the first camera component found as the active camera
		if (auto CameraComp = dynamic_cast<PCameraComponent*>(Comp))
		{
			GRenderer->SetCameraView(CameraComp->GetCameraView());
			return;
		}
	}
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
	if (!mGameModes.Contains(Name))
	{
		LogError("Target game mode {} does not exist.", Name.c_str());
		return false;
	}

	// Get the new game mode
	auto NewGameMode = GetGameMode(Name);

	// Unload the current game mode
	if (mGameMode != nullptr && mGameMode != NewGameMode && mGameMode->IsLoaded())
	{
		if (!mGameMode->Unload())
		{
			LogError("Failed to unload game mode.");
			return false;
		}
		mGameMode->SetLoaded(false);
		OnGameModeUnloaded(mGameMode);
	}

	if (!NewGameMode)
	{
		LogError("New game mode {} cannot be retrieved.", Name.c_str());
		return false;
	}

	mGameMode = NewGameMode;
	GGameMode = mGameMode;
	return true;
}

bool PGame::LoadCurrentGameMode()
{
	if (!GGameMode)
	{
		GGameMode = mGameMode;
	}
	mGameMode->Start();
	if (mGameMode->IsLoaded())
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
