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
	PButton* mNewButton;
	PButton* mEditButton;
	PButton* mSaveButton;
	PText*	 mModeText;

public:
	PEditorGame() = default;
	void PreStart() override;
	void Start() override;
	void ConstructInterface();

	void OnEditButtonClicked();
};
