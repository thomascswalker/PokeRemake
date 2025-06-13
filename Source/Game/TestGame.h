#pragma once

#include "Engine/Actors/Character.h"
#include "Engine/Game.h"

class TestGame : public PGame
{
public:
	TestGame() = default;
	~TestGame() override {}
	void		PreStart() override;
	void		Start() override;
	std::string Serialize() const override { return ""; }
	void		Deserialize(const std::string& Data) override {}
};
