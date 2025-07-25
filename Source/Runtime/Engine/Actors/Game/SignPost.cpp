#include "SignPost.h"

#include "Engine/World.h"
#include "Interface/Game/GameHUD.h"

const FVector2 gSignPostTileIndex = {6.0f, 4.0f}; // Index 70

PSignPost::PSignPost()
{
    mBlocking = true;
}

bool PSignPost::Draw(const PRenderer* Renderer) const
{
    auto Tileset = GetTileset(TILESET_1);
    FRect Source = FRect::BlockItem(gSignPostTileIndex);
    FRect Dest   = GetWorldBounds();
    Renderer->DrawTextureAt(Tileset->Texture, Source, Dest);
    return true;
}

FRect PSignPost::GetLocalBounds() const
{
    return FRect::Block();
}

void PSignPost::Interact(PPlayerCharacter* Player)
{
    auto HUD = GetHUD<PGameHUD>();
    HUD->DialogBox("This is a signpost.");
}

