#pragma once

#include "Core/Bitmask.h"
#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/ButtonGroup.h"

#define NEW_GRID_SIZE 5

enum EInputContext : uint8_t
{
	IC_None = 1U << 0,
	IC_Select = 1U << 2,
	IC_Tile = 1U << 3,
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
	BS_Large  // 2X2
};

enum EBrushMode
{
	BM_Default, // Paint from the source tile into all four quadrants.
	BM_Copy		// Paint from the source tile the three adjacent tiles, where the source tile is the top left.
};

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);

class PEditorGame : public PGame
{
	float mNewChunkSizeX = 16.0f;
	float mNewChunkSizeY = 16.0f;

	uint8_t mInputContext;

	DEditModeChanged EditModeChanged;
	EEditMode		 mEditMode = EM_None;

	std::vector<PChunk*> mChunks;
	PChunk*				 mCurrentChunk;
	EBrushSize			 mBrushSize = BS_Small;
	EBrushMode			 mBrushMode = BM_Default;

	STileItem* mCurrentTilesetItem;

public:
	// Init
	PEditorGame() = default;
	void PreStart() override;
	void Start() override;
	void SetupInterface();

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
	void OnSizeXChanged(float Value) { mNewChunkSizeX = Value; }
	void OnSizeYChanged(float Value) { mNewChunkSizeY = Value; }
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnSelectButtonChecked(bool State);
	void OnTileButtonChecked(bool State);
	void OnTilesetButtonChecked(bool State);
	void UpdateSelection(PActor* ClickedActor);
	void OnActorClicked(PActor* ClickedActor);

	EBrushSize GetBrushSize() { return mBrushSize; }

	// Scene
	void	   AddChunk(PChunk* Chunk);
	size_t	   GetChunkCount() const { return mChunks.size(); }
	void	   SetCurrentChunk(PChunk* Chunk);
	void	   ConstructChunk(const json& JsonData);
	void	   ActorSelected(PActor* Actor);
	PChunk*	   GetCurrentChunk() const { return mCurrentChunk; }
	STileItem* GetCurrentTilesetItem() const { return mCurrentTilesetItem; }

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