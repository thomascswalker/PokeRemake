#pragma once

#include "EditorData.h"

#include "Engine/Game.h"

class PEditorGame : public PGame
{
	uint8_t mInputContext;

	EBrushSize mBrushSize = BS_Small;
	EBrushMode mBrushMode = BM_Default;

	STileItem* mCurrentTilesetItem = nullptr;
	SActorItem* mCurrentActorItem  = nullptr;
	STileset* mCurrentTileset      = nullptr;
	TArray<PActor*> mSelectionQueue;

public:
	// Init
	PEditorGame() = default;
	bool PreStart() override;
	void Start() override;
	void PostTick() override;

	// Input

	uint8_t GetInputContext()
	{
		return mInputContext;
	}

	void ClearInputContext()
	{
		mInputContext = IC_None;
	}

	void AddInputContext(uint8_t InputContext);
	void RemoveInputContext(uint8_t InputContext);
	bool HasInputContext(uint8_t InputContext);

	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;

	void SetBrushSize(EBrushSize Size)
	{
		mBrushSize = Size;
	}

	void SetBrushMode(EBrushMode Mode)
	{
		mBrushMode = Mode;
	}

	void SetCurrentTilesetItem(STileItem* Item)
	{
		mCurrentTilesetItem = Item;
	}

	void SetCurrentActorItem(SActorItem* Item)
	{
		mCurrentActorItem = Item;
	}

	STileset* GetCurrentTileset() const
	{
		return mCurrentTileset;
	}

	void SetCurrentTileset(STileset* Tileset)
	{
		mCurrentTileset = Tileset;
	}

	// Scene

	void UpdateSelection(PActor* ClickedActor);
	void OnActorClicked(PActor* ClickedActor);

	void SelectAll();
	void DeselectAll();
	TArray<PActor*> GetSelectedActors();

	void PaintTile(STile* Tile);
};

PEditorGame* GetEditorGame();
