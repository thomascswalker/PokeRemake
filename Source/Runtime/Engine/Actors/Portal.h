#pragma once

#include "Actor.h"
#include "Map.h"
#include "Core/Time.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"

class PPortal : public PActor
{
	std::string mTargetMap = "";
	IVector2	mTargetPosition = { -1, -1 };

	PCollisionComponent* mCollisionComponent = nullptr;

public:
	PPortal();
	void  OnOverlapBegin(PActor* Actor) override;
	FRect GetLocalBounds() const override;
	bool  DebugDraw(const PRenderer* Renderer) const override;
	json  Serialize() const override;
	void  Deserialize(const json& Data) override;
};