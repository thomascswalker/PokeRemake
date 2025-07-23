#pragma once

#include "Core/Bitmask.h"
#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/ButtonGroup.h"
#include "Interface/Dropdown.h"
#include "Interface/GridView.h"
#include "Interface/Group.h"

#define NEW_GRID_SIZE 5

enum EInputContext : uint8_t
{
	IC_None = 1U << 0,
	IC_Select = 1U << 2,
	IC_Tile = 1U << 3,
	IC_Actor = 1U << 4,
};
DEFINE_BITMASK_OPERATORS(EInputContext);

enum EEditMode
{
	EM_None,
	EM_Select,
	EM_Tile
};

enum EBrushSize
{
	BS_Small, // 1X1
	BS_Large, // 2X2
};

enum EBrushMode
{
	BM_Default, // Paint from the source tile into all four quadrants.
	BM_Copy,	// Paint from the source tile the three adjacent tiles, where the source tile is the top left.
	BM_Fill		// Fills the entire map with the tile
};

struct SInputModeData
{
	EInputContext InputContext;
	PGroup*		  Group;
};

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);

class PEditorGame : public PGame
{
	float mNewMapSizeX = 16.0f;
	float mNewMapSizeY = 16.0f;

	uint8_t mInputContext;

	DEditModeChanged EditModeChanged;
	EEditMode		 mEditMode = EM_None;

	std::vector<PMap*> mMaps;
	PMap*				 mCurrentMap;

	std::map<std::string, PGridView*> mTilesetViews;

	PButtonGroup* mTilesetViewButtonGroup;
	STileset*	  mCurrentTileset = nullptr;
	STileItem*	  mCurrentTilesetItem = nullptr;

	PButtonGroup* mActorViewButtonGroup;
	SActorItem*	  mCurrentActorItem = nullptr;

	EBrushSize mBrushSize = BS_Small;
	EBrushMode mBrushMode = BM_Default;

public:
	// Init
	PEditorGame() = default;
	bool PreStart() override;
	void Start() override;
	void SetupInterface();

	PGridView* ConstructTilesetView(STileset* Tileset);
	PGridView* ConstructActorView();

	// Input

	uint8_t GetInputContext() { return mInputContext; }
	void	ClearInputContext() { mInputContext = IC_None; }
	void	AddInputContext(uint8_t InputContext);
	void	RemoveInputContext(uint8_t InputContext);
	bool	HasInputContext(uint8_t InputContext);

	void OnKeyDown(SInputEvent* Event) override;
	void OnKeyUp(SInputEvent* Event) override;

	// Interface

	void OnNewButtonClicked();
	void OnCreateButtonClicked();
	void OnSizeXChanged(float Value) { mNewMapSizeX = Value; }
	void OnSizeYChanged(float Value) { mNewMapSizeY = Value; }
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnEditModeClicked(SDropdownItemData* DropdownItemData);
	void OnTilesetButtonChecked(bool State);
	void OnActorButtonChecked(bool State);
	void UpdateSelection(PActor* ClickedActor);
	void OnActorClicked(PActor* ClickedActor);
	void OnDropdownClicked(SDropdownItemData* Data) const;

	EBrushSize GetBrushSize() { return mBrushSize; }

	// Scene
	void	   AddMap(PMap* Map);
	size_t	   GetMapCount() const { return mMaps.size(); }
	void	   SetCurrentMap(PMap* Map);
	void	   ConstructMap(const JSON& JsonData);
	void	   ActorSelected(PActor* Actor);
	PMap*	   GetCurrentMap() const { return mCurrentMap; }
	STileItem* GetCurrentTilesetItem() const { return mCurrentTilesetItem; }
	void	   PaintTile(STile* Tile);

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