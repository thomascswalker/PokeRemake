// ReSharper disable CppDFAUnreachableCode

#include "EditorGame.h"

#include "Actors/ActorManager.h"
#include "Actors/EditorView.h"
#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "Engine/Actors/Portal.h"
#include "Engine/Actors/SceneryActor.h"
#include "Engine/Input.h"
#include "Engine/MapManager.h"
#include "Interface/EditorHUD.h"
#include "Interface/Group.h"
#include "Interface/Spinner.h"

#include "EditorMode.h"

PEditorGame* GetEditorGame()
{
	return dynamic_cast<PEditorGame*>(GEngine->GetGame());
}

PEditorGame::PEditorGame()
{
	AddGameMode<PEditorMode>();
}

bool PEditorGame::PreStart()
{
	PGame::PreStart();

	auto HUD = GWorld->ConstructWidget<PEditorHUD>();
	GWorld->GetRootWidget()->AddChild(HUD);

	GSettings->DebugDraw = true;

	const auto EditorView = mWorld->ConstructActor<PEditorView>();

	if (!EditorView)
	{
		LogError("Failed to create Editor View");
	}

	return true;
}

bool PEditorGame::Start()
{
	PGame::Start();

	// Bind the world actor clicked event to handle selection within the editor.
	GWorld->ActorClicked.AddRaw(this, &PEditorGame::OnActorClicked);

	return true;
}

void PEditorGame::PostTick()
{
	PGame::PostTick();

	if (mSelectionQueue.Size() == 0)
	{
		return;
	}

	PSelectionComponent* Comp = nullptr;

	if (mSelectionQueue.Size() > 1)
	{
		auto TempQueue = mSelectionQueue.Sorted(DepthSort);
		Comp = TempQueue[0]->GetSelectionComponent();
	}
	else
	{
		Comp = mSelectionQueue[0]->GetSelectionComponent();
	}

	bool NewState = !Comp->GetSelected();

	// Deselect all actors, then set the new state of the first actor.
	DeselectAll();
	Comp->SetSelected(NewState);

	// Clear the selection queue.
	mSelectionQueue.Clear();
	SelectionChanged.Broadcast(NewState ? Comp->GetOwner() : nullptr);
}

void PEditorGame::OnKeyDown(SInputEvent* Event)
{
	PGame::OnKeyDown(Event);
	if (Event->Consumed)
	{
		return;
	}

	switch (Event->KeyDown)
	{
		case SDLK_LSHIFT:
			{
				if (IsInputContext(IC_Tile))
				{
					mBrushMode = BM_Copy;
				}
				break;
			}
		case SDLK_LCTRL:
			{
				if (IsInputContext(IC_Tile))
				{
					mBrushMode = BM_Fill;
				}
				break;
			}
		default:
			break;
	}
}

void PEditorGame::OnKeyUp(SInputEvent* Event)
{
	PGame::OnKeyUp(Event);
	if (Event->Consumed)
	{
		return;
	}

	switch (Event->KeyUp)
	{
		case SDLK_DELETE:
			{
				if (IsInputContext(IC_Select))
				{
					for (auto Actor : GetSelectedActors())
					{
						mWorld->DestroyActor(Actor);
					}
				}
				Event->Consume();
			}
			break;
		case SDLK_UP:
		case SDLK_DOWN:
			{
				if (IsInputContext(IC_Tile))
				{
					mBrushSize = Event->KeyUp == SDLK_UP ? BS_Large : BS_Small;
				}
				break;
			}
		case SDLK_LSHIFT:
		case SDLK_LCTRL:
			{
				if (IsInputContext(IC_Tile))
				{
					mBrushMode = BM_Default;
				}
				break;
			}
		case SDLK_F:
			{
				if (!IsInputContext(IC_Text) && mActiveCameraView != nullptr)
				{
					mActiveCameraView->GetComponent()->GetOwner()->SetPosition2D({ 0, 0 });
					mActiveCameraView->SetZoom(1.0f);
				}
				break;
			}
		default:
			break;
	}
}

void PEditorGame::UpdateSelection(PActor* ClickedActor)
{
	// Add the clicked actor to the selection queue. The queue will be processed within
	// PEditorGame::PostTick().
	mSelectionQueue.Add(ClickedActor);
}

void PEditorGame::OnActorClicked(PActor* ClickedActor)
{
	if (IsInputContext(IC_Select))
	{
		UpdateSelection(ClickedActor);
		return;
	}
	auto GameMap = GetMapManager()->GetMapUnderMouse();
	if (IsInputContext(IC_Tile))
	{
		if (!mCurrentTilesetItem)
		{
			LogWarning("No tileset item selected.");
			return;
		}

		switch (mBrushMode)
		{
			case BM_Default:
			case BM_Copy:
				PaintTile(GameMap->GetTileUnderMouse());
				break;
			case BM_Fill:
				for (auto Tile : GameMap->GetTiles())
				{
					Tile->Index = mCurrentTilesetItem->Index;
					Tile->Tileset = mCurrentTileset;
				}
				break;
		}
	}
	else if (IsInputContext(IC_Actor))
	{
		if (!mCurrentActorItem)
		{
			LogWarning("No actor item selected.");
			return;
		}
		auto Position = GameMap->GetTileUnderMouse()->GetPosition();
		auto Actors = mWorld->GetActorsAtPosition(Position);
		Actors = Containers::Filter(Actors, [](PActor* Actor) {
			return dynamic_cast<PGameMap*>(Actor) == nullptr;
		});
		if (Actors.size() == 0)
		{
			PActor* NewActor = nullptr;
			if (mCurrentActorItem->Name == "Portal")
			{
				NewActor = SpawnActor<PPortal>();
			}
			else
			{
				const JSON Json = mCurrentActorItem->Data;
				LogDebug("Creating new Scenery Actor:\n{}", Json.dump(4));
				NewActor = SpawnActor<PSceneryActor>(Json);
			}

			if (NewActor)
			{
				LogDebug("Placing {}", mCurrentActorItem->Name.c_str());
				NewActor->SetPosition2D(Position);
				GameMap->AddChild(NewActor);
			}
		}
	}
}

void PEditorGame::SelectAll()
{
	for (auto Actor : mWorld->GetSelectableActors())
	{
		Actor->GetSelectionComponent()->SetSelected(true);
	}
}

void PEditorGame::DeselectAll()
{
	for (auto Actor : mWorld->GetSelectableActors())
	{
		Actor->GetSelectionComponent()->SetSelected(false);
	}
}

TArray<PActor*> PEditorGame::GetSelectedActors()
{
	TArray<PActor*> SelectedActors;
	for (auto Actor : mWorld->GetSelectableActors())
	{
		if (Actor->GetSelectionComponent()->GetSelected())
		{
			SelectedActors.Add(Actor);
		}
	}
	return SelectedActors;
}

void PEditorGame::PaintTile(STile* Tile)
{
	if (!Tile || !mCurrentTilesetItem)
	{
		return;
	}

	// Fill all tiles
	PGameMap* GameMap = Tile->GameMap;

	// Set hit tile
	Tile->Tileset = mCurrentTileset;
	Tile->Index = mCurrentTilesetItem->Index;

	// Set adjacent tiles
	if (mBrushSize == BS_Large)
	{
		auto MousePosition = GRenderer->GetMouseWorldPosition();

		// TODO: Clean this up
		if (auto Tile2 = GameMap->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, 0)))
		{
			Tile2->Tileset = mCurrentTileset;
			Tile2->Index = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + 1 : mCurrentTilesetItem->Index;
		}
		if (auto Tile3 = GameMap->GetTileAtPosition(MousePosition + FVector2(0, TILE_SIZE)))
		{
			Tile3->Tileset = mCurrentTileset;
			Tile3->Index = mBrushMode == BM_Copy
							   ? mCurrentTilesetItem->Index + Tile->Tileset->Width
							   : mCurrentTilesetItem->Index;
		}
		if (auto Tile4 = GameMap->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, TILE_SIZE)))
		{
			Tile4->Tileset = mCurrentTileset;
			Tile4->Index = mBrushMode == BM_Copy
							   ? mCurrentTilesetItem->Index + Tile->Tileset->Width + 1
							   : mCurrentTilesetItem->Index;
		}
	}
}
