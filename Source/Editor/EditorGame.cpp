#include "EditorGame.h"

#include "ActorManager.h"
#include "EditorHUD.h"
#include "EditorView.h"
#include "Application/Application.h"
#include "Core/CoreFwd.h"
#include "Engine/Input.h"
#include "Engine/MapManager.h"
#include "Engine/Actors/Portal.h"
#include "Engine/Actors/SceneryActor.h"
#include "Interface/Group.h"
#include "Interface/Spinner.h"

PEditorGame* GetEditorGame()
{
	return dynamic_cast<PEditorGame*>(GetGame());
}

bool PEditorGame::PreStart()
{
	mWorld->GetHUD<PEditorHUD>()->PreStart();
	GetSettings()->DebugDraw = true;

	const auto EditorView = mWorld->ConstructActor<PEditorView>();
	if (!EditorView)
	{
		LogError("Failed to create Editor View");
	}

	return true;
}

void PEditorGame::Start()
{
	PGame::Start();

	// Default to selection context
	AddInputContext(IC_Select);

	// Bind the world actor clicked event to handle selection within the editor.
	GetWorld()->ActorClicked.AddRaw(this, &PEditorGame::OnActorClicked);
}

void PEditorGame::PostTick()
{
	PGame::PostTick();

	if (mSelectionQueue.Size() == 0)
	{
		return;
	}
	if (mSelectionQueue.Size() == 1)
	{
		auto Comp     = mSelectionQueue[0]->GetSelectionComponent();
		bool NewState = !Comp->GetSelected();
		DeselectAll();
		Comp->SetSelected(NewState);
		mSelectionQueue.Clear();
		return;
	}

	// Sort the selection queue by Z depth.
	auto TempQueue = mSelectionQueue.Sorted(DepthSort);

	// Store the opposite selection state of the first actor.
	auto Comp     = TempQueue[0]->GetSelectionComponent();
	bool NewState = !Comp->GetSelected();

	// Deselect all actors, then set the new state of the first actor.
	DeselectAll();
	TempQueue[0]->GetSelectionComponent()->SetSelected(NewState);

	// Clear the selection queue.
	mSelectionQueue.Clear();
}

void PEditorGame::AddInputContext(uint8_t InputContext)
{
	mInputContext |= InputContext;
}

void PEditorGame::RemoveInputContext(uint8_t InputContext)
{
	mInputContext &= ~InputContext;
}

bool PEditorGame::HasInputContext(uint8_t InputContext)
{
	return (mInputContext & InputContext) == InputContext;
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
	case SDLK_LSHIFT: if (HasInputContext(IC_Tile))
		{
			mBrushMode = BM_Copy;
		}
		break;
	case SDLK_LCTRL: if (HasInputContext(IC_Tile))
		{
			mBrushMode = BM_Fill;
		}
		break;
	default: break;
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
			if (HasInputContext(IC_Select))
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
			if (HasInputContext(IC_Tile))
			{
				mBrushSize = Event->KeyUp == SDLK_UP ? BS_Large : BS_Small;
			}
			break;
		}
	case SDLK_LSHIFT:
	case SDLK_LCTRL:
		{
			if (HasInputContext(IC_Tile))
			{
				mBrushMode = BM_Default;
			}
			break;
		}
	case SDLK_F:
		{
			if (mActiveCameraView)
			{
				mActiveCameraView->GetComponent()->GetOwner()->SetPosition2D({0, 0});
				mActiveCameraView->SetZoom(1.0f);
			}
			break;
		}
	default: break;
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
	if (HasInputContext(IC_Select))
	{
		UpdateSelection(ClickedActor);
		return;
	}
	auto Map = GetCurrentMap();
	if (HasInputContext(IC_Tile))
	{
		if (!mCurrentTilesetItem)
		{
			LogWarning("No tileset item selected.");
			return;
		}

		switch (mBrushMode)
		{
		case BM_Default:
		case BM_Copy: PaintTile(Map->GetTileUnderMouse());
			break;
		case BM_Fill: for (auto Tile : Map->GetTiles())
			{
				Tile->Index   = mCurrentTilesetItem->Index;
				Tile->Tileset = mCurrentTileset;
			}
			break;
		}
	}
	else if (HasInputContext(IC_Actor))
	{
		if (!mCurrentActorItem)
		{
			LogWarning("No actor item selected.");
			return;
		}
		auto Position = Map->GetTileUnderMouse()->GetPosition();
		auto Actors   = mWorld->GetActorsAtPosition(Position);
		Actors        = Containers::Filter(Actors, [](PActor* Actor)
		{
			return dynamic_cast<PMap*>(Actor) == nullptr;
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
				Map->AddChild(NewActor);
			}
		}
	}
}

void PEditorGame::AddMap(PMap* Map)
{
	mMaps.emplace_back(Map);
	SetCurrentMap(Map);
}

void PEditorGame::SetCurrentMap(PMap* Map)
{
	mCurrentMap = Map;
}

void PEditorGame::ConstructMap(const JSON& JsonData)
{
	// Create the map
	const auto Map = SpawnActor<PMap>(JsonData);
	if (!Map)
	{
		LogError("Failed to create map");
		return;
	}
	AddMap(Map);
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

Array<PActor*> PEditorGame::GetSelectedActors()
{
	Array<PActor*> SelectedActors;
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
	if (!Tile)
	{
		return;
	}

	// Fill all tiles
	PMap* Map = Tile->Map;

	// Set hit tile
	Tile->Tileset = mCurrentTileset;
	Tile->Index   = mCurrentTilesetItem->Index;

	// Set adjacent tiles
	if (mBrushSize == BS_Large)
	{
		auto MousePosition = GetRenderer()->GetMouseWorldPosition();

		// TODO: Clean this up
		if (auto Tile2 = Map->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, 0)))
		{
			Tile2->Tileset = mCurrentTileset;
			Tile2->Index   = mBrushMode == BM_Copy ? mCurrentTilesetItem->Index + 1 : mCurrentTilesetItem->Index;
		}
		if (auto Tile3 = Map->GetTileAtPosition(MousePosition + FVector2(0, TILE_SIZE)))
		{
			Tile3->Tileset = mCurrentTileset;
			Tile3->Index   = mBrushMode == BM_Copy
				               ? mCurrentTilesetItem->Index + Tile->Tileset->Width
				               : mCurrentTilesetItem->Index;
		}
		if (auto Tile4 = Map->GetTileAtPosition(MousePosition + FVector2(TILE_SIZE, TILE_SIZE)))
		{
			Tile4->Tileset = mCurrentTileset;
			Tile4->Index   = mBrushMode == BM_Copy
				               ? mCurrentTilesetItem->Index + Tile->Tileset->Width + 1
				               : mCurrentTilesetItem->Index;
		}
	}
}
