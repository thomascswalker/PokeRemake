#pragma once
#include "Engine/Actors/Character.h"

struct SNPCContext : ISerializable
{
	std::string Name;
	std::string Dialog;
};

class PNPC : public PCharacter
{
};
