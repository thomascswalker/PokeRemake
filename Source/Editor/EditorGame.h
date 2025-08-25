#pragma once

#include "EditorData.h"

#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/ButtonGroup.h"

class PEditorGame : public PGame
{
	uint8_t mInputContext;

	std::vector<PMap*> mMaps;
	PMap* mCurrentMap;

	EBrushSize mBrushSize = BS_Small;
	EBrushMode mBrushMode = BM_Default;

	STileItem* mCurrentTilesetItem = nullptr;
	SActorItem* mCurrentActorItem  = nullptr;
	STileset* mCurrentTileset      = nullptr;
	Array<PActor*> mSelectionQueue;

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

	size_t GetMapCount() const
	{
		return mMaps.size();
	}

	void AddMap(PMap* Map);
	void SetCurrentMap(PMap* Map);
	void ConstructMap(const JSON& JsonData);

	PMap* GetCurrentMap() const
	{
		return mCurrentMap;
	}

	void ClearMaps()
	{
		mMaps.clear();
		mCurrentMap = nullptr;
	}

	void SelectAll();
	void DeselectAll();
	Array<PActor*> GetSelectedActors();

	void PaintTile(STile* Tile);

	template <typename T = PActor>
	T* GetActorUnderMouse()
	{
		const auto W = GetWorld();
		for (const auto& Actor : W->GetActors())
		{
			const auto TActor = dynamic_cast<T*>(Actor);
			if (TActor && Actor->mMouseOver)
			{
				return TActor;
			}
		}
		return nullptr;
	}
};

PEditorGame* GetEditorGame();
