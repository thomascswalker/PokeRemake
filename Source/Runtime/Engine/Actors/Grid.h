#pragma once

#include <bitset>

#include "Actor.h"

class PGrid : public PActor
{
	float mOffsetX;
	float mOffsetY;

	std::bitset<4> mKeysDown; // Right, Left, Down, Up

public:
	PGrid();
	~PGrid() override = default;

	void Tick(float DeltaTime) override;

	void Draw(const PRenderer* Renderer) const override;
	void OnKeyDown(uint32_t ScanCode);
	void OnKeyUp(uint32_t ScanCode);
	void AddOffsetX(float Value) { mOffsetX += Value; }
	void AddOffsetY(float Value) { mOffsetY += Value; }
};
