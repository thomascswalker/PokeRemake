#pragma once

#include "Engine/Game.h"
#include "Interface/Button.h"
#include "Interface/Canvas.h"
#include "Interface/Text.h"

constexpr int BUTTON_WIDTH = 50;
constexpr int BUTTON_HEIGHT = 20;

class PEditorGame : public PGame
{
	bool bEditMode = false;

	// Widgets

	PCanvas* mCanvas;
	PButton* mCreateButton;
	PButton* mEditButton;
	PButton* mSaveButton;
	PButton* mLoadButton;
	PText*	 mModeText;

	std::vector<PChunk*> mChunks;
	PChunk*				 mCurrentChunk;

public:
	// Init
	PEditorGame() = default;
	void PreStart() override;
	void Start() override;
	void ConstructInterface();
	void InitializeControls();

	// Interface
	void OnCreateButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnKeyUp(uint32_t ScanCode);

	// Scene
	void AddChunk(PChunk* Chunk);
	void SetCurrentChunk(PChunk* Chunk);
	void ConstructChunk(const json& JsonData);
	void ActorSelected(PActor* Actor);
};
