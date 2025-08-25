#pragma once
#include "EditorGame.h"

#include "Interface/Dropdown.h"
#include "Interface/HUD.h"

class PEditorHUD : public PHUD
{
    float mNewMapSizeX = 16.0f;
    float mNewMapSizeY = 16.0f;

    DEditModeChanged EditModeChanged;

public:
    bool PreStart() override;
    void SetupInterface();
    PGridView* ConstructTilesetView(STileset* Tileset);
    PGridView* ConstructActorView();

    void OnSizeXChanged(float Value);

    void OnSizeYChanged(float Value);

    void OnEditModeClicked(SDropdownItemData* DropdownItemData);

    void OnNewButtonClicked();

    void OnCreateButtonClicked();
    void OnSaveButtonClicked();
    void OnLoadButtonClicked();
    void OnTilesetButtonChecked(bool State);
    void OnActorButtonChecked(bool State);
};
