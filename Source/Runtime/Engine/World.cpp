#include "World.h"

#include "Core/Logging.h"
#include "Engine/InputManager.h"

static TileArray PaletteTownTileData = {
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
	SChunkData PalletTownData;
	PalletTownData.Geometry = IRect{ 0, 0, 20, 18 }; // 20 tiles wide, 18 tiles tall
	PalletTownData.TextureName = "PalletTown.png";
	PalletTownData.Data = PaletteTownTileData;
	mPalletTown = this->ConstructActor<PChunk>(PalletTownData);
}