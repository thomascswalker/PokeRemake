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

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);

class PEditorGame : public PGame
{
	float mNewGridSizeX = 5.0f;
	float mNewGridSizeY = 5.0f;

	uint8_t mInputContext;

	DEditModeChanged EditModeChanged;
	EEditMode		 mEditMode = EM_None;

	std::vector<PChunk*> mChunks;
	PChunk*				 mCurrentChunk;

	DelegateHandle mSelectDelegate;
	DelegateHandle mTileDelegate;

	STilesetItem* mCurrentTilesetItem;

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
	void	OnKeyUp(SInputEvent* Event) override;
	void	OnKeyUpSelect(uint32_t ScanCode);

	// Interface

	void OnNewButtonClicked();
	void OnCreateButtonClicked();
	void OnSizeXChanged(float Value) { mNewGridSizeX = Value; }
	void OnSizeYChanged(float Value) { mNewGridSizeY = Value; }
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnSelectButtonChecked(bool State);
	void OnTileButtonChecked(bool State);
	void OnTilesetButtonChecked(bool State);
	void OnActorClicked(PActor* ClickedActor);

	// Scene
	void		  AddChunk(PChunk* Chunk);
	size_t		  GetChunkCount() const { return mChunks.size(); }
	void		  SetCurrentChunk(PChunk* Chunk);
	void		  ConstructChunk(const json& JsonData);
	void		  ActorSelected(PActor* Actor);
	PChunk*		  GetCurrentChunk() const { return mCurrentChunk; }
	STilesetItem* GetCurrentTilesetItem() const { return mCurrentTilesetItem; }

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