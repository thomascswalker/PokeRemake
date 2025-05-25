#pragma once

#include <bitset>

#include "Actor.h"

class PGrid : public PActor
{
	uint32_t mSizeX;
	uint32_t mSizeY;
	float	 mZoomFactor;

	float mOffsetX;
	float mOffsetY;

	std::bitset<4> mKeysDown; // Right, Left, Down, Up

public:
	PGrid(uint32_t InX, uint32_t InY, uint32_t InCellSize);
	~PGrid() override = default;

	void Tick(float DeltaTime) override;

	void Draw(IRenderer* Renderer) override;
	void OnKeyDown(uint32_t ScanCode);
	void OnKeyUp(uint32_t ScanCode);
	void OnMouseScroll(float Value);
	void AddOffsetX(float Value) { mOffsetX += Value; }
	void AddOffsetY(float Value) { mOffsetY += Value; }
};
