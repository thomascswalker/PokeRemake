#pragma once

#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/Canvas.h"

constexpr int BUTTON_WIDTH = 50;
constexpr int BUTTON_HEIGHT = 20;

enum EEditMode
{
	EM_None,
	EM_Select,
	EM_Tile
};

DECLARE_MULTICAST_DELEGATE(DEditModeChanged, EEditMode);

class PEditorGame : public PGame
{
	DEditModeChanged EditModeChanged;
	EEditMode		 mEditMode = EM_None;

	std::vector<PChunk*> mChunks;
	PChunk*				 mCurrentChunk;

	// Interface

	PButton* mEditModeSelect;
	PButton* mEditModeTile;

public:
	// Init
	PEditorGame() = default;
	void PreStart() override;
	void Start() override;
	void ConstructInterface();
	void InitializeControls();

	// Interface
	void OnKeyUp(uint32_t ScanCode);
	void OnCreateButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnSelectButtonClicked();
	void OnTileButtonClicked();

	// Scene
	void AddChunk(PChunk* Chunk);
	void SetCurrentChunk(PChunk* Chunk);
	void ConstructChunk(const json& JsonData);
	void ActorSelected(PActor* Actor);
};
