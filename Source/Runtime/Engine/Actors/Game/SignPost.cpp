#include "SignPost.h"

#include "Engine/World.h"
#include "Interface/Game/GameHUD.h"

const uint32_t gSignPostTileIndex = 35; // 36th 2x2 block

PSignPost::PSignPost()
{
    mBlocking        = true;
    mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
    if (mSpriteComponent)
    {
        auto Tileset = GetTileset(TILESET_1);
        auto Sprite  = mSpriteComponent->GetSprite();
        Sprite->SetTexture(Tileset->Texture);
        Sprite->SetWidth(Tileset->Width);
        Sprite->SetSize(BLOCK_ITEM_SIZE);
        Sprite->AddAnimation("SignPost", {gSignPostTileIndex}); // 36th 2x2 block
    }
}

FRect PSignPost::GetLocalBounds() const
{
    return FRect::Block();
}

void PSignPost::Interact(PPlayerCharacter* Player)
{
    GetHUD<PGameHUD>()->DialogBox("This is a signpost.");
}

