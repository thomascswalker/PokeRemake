#include "MapMode.h"

#include "Application/Application.h"

#define PLAYER_MAP "MapName"
#define PLAYER_POSITION "PlayerPosition"

PMapMode::PMapMode()
{
    mSaveState[PLAYER_MAP]      = MAP_PALLET_TOWN;
    mSaveState[PLAYER_POSITION] = JSON::array({800, 800});
    LogDebug("Save State: {}", mSaveState.dump(2).c_str());
}

bool PMapMode::Load()
{
    LogDebug("Load Save State: {}", mSaveState.dump(2).c_str());
    // Load the map from JSON
    auto Map = PMapManager::LoadMap(mSaveState[PLAYER_MAP], false);

    LogDebug("PreStart: Constructing actors.");
    auto Player = ConstructActor<PPlayerCharacter>();
    GetWorld()->SetPlayerCharacter(Player);

    auto JsonPosition = mSaveState[PLAYER_POSITION];
    FVector2 Position(JsonPosition);
    Player->GetMovementComponent()->SnapToPosition(Position, Map);

    SetInputContext(IC_Default);
    return true;
}

bool PMapMode::Unload()
{
    auto World  = GetWorld();
    auto Player = World->GetPlayerCharacter();

    // Current position
    auto PlayerPosition         = Player->GetPosition2D();
    mSaveState[PLAYER_POSITION] = PlayerPosition.ToJson();

    // Current map
    auto Map = PMapManager::GetMapAtPosition(PlayerPosition);
    if (!Map)
    {
        LogError("No map at position: {}", PlayerPosition.ToString().c_str());
        return false;
    }
    mSaveState[PLAYER_MAP] = Map->GetMapName();

    // Destroy all actors
    World->DestroyAllActors();

    // Clear all maps from the world
    PMapManager::ClearMaps();

    // Unset the player character
    World->SetPlayerCharacter(nullptr);

    LogDebug("Unload Save State: {}", mSaveState.dump(2).c_str());

    return true;
}

void PMapMode::OnKeyUp(SInputEvent* Event)
{
    switch (Event->KeyUp)
    {
    case SDLK_Q:
        {
            if (!GetGame()->SetAndLoadCurrentGameMode("BattleMode"))
            {
                Event->Invalidate();
            }
            break;
        }
    default: break;
    }
}

