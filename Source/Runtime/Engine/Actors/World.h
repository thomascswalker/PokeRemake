#pragma once

#include <memory>
#include <stdint.h>

#include "Actor.h"

class AGrid : public AActor
{
public:
	uint32_t mSizeX;
	uint32_t mSizeY;
	uint32_t mCellSize;

	AGrid(uint32_t InX, uint32_t InY, uint32_t InCellSize);
	~AGrid() override = default;

	void Draw(IRenderer* Renderer) override;
};

class AWorld : public AActor
{
	std::unique_ptr<AGrid> mGrid;

public:
	AWorld();
	~AWorld() override = default;

	AGrid* GetGrid() const { return mGrid.get(); }

	void Draw(IRenderer* Renderer) override;
};
