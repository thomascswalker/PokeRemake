#pragma once

#include "Engine/Game.h"

#include "EditorData.h"

DECLARE_MULTICAST_DELEGATE(DSelectionChanged, const PActor*);

class PEditorGame : public PGame
{
	EBrushSize mBrushSize = BS_Small;
	EBrushMode mBrushMode = BM_Default;

	STileItem*		mCurrentTilesetItem = nullptr;
	SActorItem*		mCurrentActorItem = nullptr;
	STileset*		mCurrentTileset = nullptr;
	TArray<PActor*> mSelectionQueue;

public:
	DSelectionChanged SelectionChanged;

	// Init
	PEditorGame();
	bool PreStart() override;
	void Start() override;
	void PostTick() override;

	// Input

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

	void			SelectAll();
	void			DeselectAll();
	TArray<PActor*> GetSelectedActors();

	void PaintTile(STile* Tile);
};

PEditorGame* GetEditorGame();
