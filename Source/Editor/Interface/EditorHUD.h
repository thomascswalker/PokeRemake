#pragma once
#include "../EditorGame.h"
#include "Interface/Dropdown.h"
#include "Interface/HUD.h"

class PEditorHUD : public PHUD
{
	float mNewMapSizeX = 16.0f;
	float mNewMapSizeY = 16.0f;

	DEditModeChanged EditModeChanged;

public:
	bool	   PreStart() override;
	void	   SetupInterface();
	PGridView* ConstructTilesetView(STileset* Tileset);
	PGridView* ConstructActorView();
	PWidget*   ConstructSelectionView(const PActor* Actor);

	void OnSizeXChanged(float Value);

	void OnSizeYChanged(float Value);

	void OnNewButtonClicked();

	void OnCreateButtonClicked();
	void OnSaveButtonClicked();
	void OnLoadButtonClicked();
	void OnSelectButtonClicked();
	void OnTilesButtonClicked();
	void OnActorsButtonClicked();
	void OnExitButtonClicked();

	void OnSelectionChange(const PActor* Actor);
	void OnTilesetButtonChecked(bool State);
	void OnActorButtonChecked(bool State);
};
