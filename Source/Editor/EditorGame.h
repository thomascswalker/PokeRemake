#pragma once

#include "Core/Bitmask.h"
#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/ButtonGroup.h"
#include "Interface/Canvas.h"

constexpr int BUTTON_WIDTH = 50;
constexpr int BUTTON_HEIGHT = 20;

enum EInputContext : uint8_t
{
	IC_None = 1U << 0,
	IC_Move = 1U << 1,
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
	uint8_t mInputContext;

	DEditModeChanged EditModeChanged;
	EEditMode		 mEditMode = EM_None;

	std::vector<PChunk*> mChunks;
	PChunk*				 mCurrentChunk;

public:
	// Init
	PEditorGame() = default;
	void PreStart() override;
	void Start() override;
	void ConstructInterface();
	void InitializeControls();

	uint8_t GetInputContext() { return mInputContext; }
	void	ClearInputContext() { mInputContext = IC_None; }
	void	AddInputContext(uint8_t InputContext);
	void	RemoveInputContext(uint8_t InputContext);

	// Interface
	void OnKeyUp(uint32_t ScanCode);
	void OnCreateButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnSelectButtonChecked(bool State);
	void OnTileButtonChecked(bool State);

	// Scene
	void AddChunk(PChunk* Chunk);
	void SetCurrentChunk(PChunk* Chunk);
	void ConstructChunk(const json& JsonData);
	void ActorSelected(PActor* Actor);
};

PEditorGame* GetEditorGame();