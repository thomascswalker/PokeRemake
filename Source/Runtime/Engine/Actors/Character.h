#pragma once

#include "Actor.h"

#include <bitset>
#include <memory>

class PCharacter : public PActor
{
	std::bitset<4> mKeysDown; // Right, Left, Down, Up

public:
	PCharacter() = default;
	~PCharacter() override = default;

	void Start() override;
	void End() override {}
	void Tick(float DeltaTime) override;
	void Draw(const PRenderer* Renderer) const override;
	void OnKeyDown(uint32_t KeyCode);
	void OnKeyUp(uint32_t KeyCode);
};
