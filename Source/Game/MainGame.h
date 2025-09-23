#pragma once

#include "Engine/Game.h"

#include "Party.h"
#include "Pokedex.h"

class PMainGame : public PGame
{
	PPokedexManager Pokedex;
	PParty			Party;
	PPokeStorage	Storage;

public:
	PMainGame();
	~PMainGame() override {}

	bool PreStart() override;
	bool Start() override;
};
