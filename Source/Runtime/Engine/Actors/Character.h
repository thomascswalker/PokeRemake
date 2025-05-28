#pragma once

#include "Actor.h"

#include <bitset>

class PCharacter : public PActor
{
	std::bitset<4> mKeysDown; // Right, Left, Down, Up

public:
	PCharacter() = default;
	~PCharacter() override = default;
	void Tick(float DeltaTime) override;
	void Start() override;
	void End() override {}
	void Draw(IRenderer* Renderer) override;
	void OnKeyDown(uint32_t KeyCode);
	void OnKeyUp(uint32_t KeyCode);
};
