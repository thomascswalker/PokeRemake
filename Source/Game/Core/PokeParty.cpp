#include "PokeParty.h"

static PPokemonParty gPlayerParty(PT_Player);

PPokemonParty* GetPlayerParty()
{
	return &gPlayerParty;
}