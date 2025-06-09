#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

static SChunkData PaletteTownChunkData = {
	{ 0, 0, 20, 18 }, // Geometry: 20 tiles wide, 18 tiles tall
	"PalletTown.png",
	{
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 1, 1, 5, 1, 1, 0, 0, 0, 1, 1, 5, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 0, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	 }
};

static SChunkData Route1ChunkData = {
	{ 0, -2, 20, 2 }, // Geometry: 20 tiles wide, 2 tiles tall
	"Route1.png",
	{
		{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	 }
};

void PWorld::Start()
{
	LogDebug("Starting world");
	ConstructChunks();
	// mRoute1 = this->ConstructActor<PChunk>();

	for (const auto& Actor : GetActors())
	{
		Actor->Start();
	}
	for (const auto& Component : GetComponents())
	{
		Component->Start();
	}
}

void PWorld::Tick(float DeltaTime)
{
	for (const auto& Actor : GetActors())
	{
		Actor->Tick(DeltaTime);
	}
	for (const auto& Component : GetComponents())
	{
		Component->Tick(DeltaTime);
	}
}

void PWorld::ConstructChunks()
{
	mPalletTown = this->ConstructActor<PChunk>(PaletteTownChunkData);
	mRoute1 = this->ConstructActor<PChunk>(Route1ChunkData);
}