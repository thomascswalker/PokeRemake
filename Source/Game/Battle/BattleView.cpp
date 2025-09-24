#include "BattleView.h"

#include "Engine/Game.h"
#include "Engine/World.h"

#include "BattleHUD.h"
#include "BattleManager.h"

static FVector2 gPlayerSpritePosition(0, 180);
static FVector2 gBattleSpritePosition(210, 0);

void PBattleSprite::Draw(const PRenderer* Renderer) const
{
	FVector2 Position = mIsPlayer ? gPlayerSpritePosition : gBattleSpritePosition;
	if (!mPokemon)
	{
		LogError("Pokemon is invalid.");
		return;
	}
	SPokemonDef* Def = mPokemon->GetDef();
	std::string	 Name = Def->Name;

	Renderer->SetDrawColor(PColor::Red);
	Renderer->DrawText(Name, Position, 16.0f);
}

bool PBattleView::Start()
{
	if (const auto W = GetWorld())
	{
		mCameraComponent = W->ConstructComponent<PCameraComponent>(this);
	}

	return true;
}
