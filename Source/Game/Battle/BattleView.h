#pragma once

#include "Core/Pokemon.h"
#include "Engine/Actors/Actor.h"

class PBattleSprite : public PWidget
{
	SPokemon* mPokemon;
	bool	  mIsPlayer;

public:
	PBattleSprite(SPokemon* InPokemon, bool InIsPlayer) : mPokemon(InPokemon), mIsPlayer(InIsPlayer) {}
	void Draw(const PRenderer* Renderer) const override;
};

class PBattleView : public PActor
{
protected:
	PCameraComponent* mCameraComponent = nullptr;

public:
	bool Start() override;
};
