#pragma once

#include "Actor.h"

class PChunk;

class PScenery : public PActor
{
	STileData Data{};
	PChunk*	  Chunk = nullptr;

public:
	PScenery();
	~PScenery() override = default;
};