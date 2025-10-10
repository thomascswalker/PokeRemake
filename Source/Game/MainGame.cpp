#include "MainGame.h"

#include "Application/Application.h"
#include "Battle/BattleMode.h"
#include "Core/GameSerializer.h"
#include "Core/Ids.h"
#include "Core/TextRenderer.h"
#include "Modes/MapMode.h"

PMainGame::PMainGame()
{
	mSerializer = std::make_shared<PGameSerializer>();
	GSerializer = mSerializer.get();
}

bool PMainGame::PreStart()
{
	if (!PGame::PreStart())
	{
		return false;
	}

	// Load all pokemon info
	if (!PPokedexManager::Instance()->Init())
	{
		return false;
	}

	// Construct the main HUD used across all modes (map, battle, storage, etc.)
	mHUD = ConstructWidget<PGameHUD>();

	auto MapMode = AddGameMode<PMapMode>();
	MapMode->PreStart();

	auto BattleMode = AddGameMode<PBattleMode>();
	BattleMode->PreStart();

	// Initialize Player Party
	auto Mon = mPlayerStorage.Construct(MON_NIDOKING, 64);
	mPlayerParty.Add(Mon);

	LogInfo("Game PreStart successful.");

	return true;
}

bool PMainGame::Start()
{
	if (!PGame::Start())
	{
		return false;
	}
	GWorld->GetRootWidget()->AddChild(mHUD);

	GPlayerParty = &mPlayerParty;
	GPlayerStorage = &mPlayerStorage;

	TextRenderer::LoadFontData();

	return true;
}

void PMainGame::OnKeyUp(SInputEvent* Event)
{
	switch (Event->KeyUp)
	{
		case SDLK_F1:
			GSettings->DebugDraw = !GSettings->DebugDraw;
			LogDebug("Debug mode: {}", GSettings->DebugDraw ? "ON" : "OFF");
			break;
		default:
			break;
	}
}
