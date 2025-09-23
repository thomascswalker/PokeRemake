#pragma once

#include "Engine/Game.h"

#include "Pokedex.h"

class PMainGame : public PGame
{
	PPokedexManager Pokedex;

public:
	PMainGame();
	~PMainGame() override {}

	bool PreStart() override;
};
