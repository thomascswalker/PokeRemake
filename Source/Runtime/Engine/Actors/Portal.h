#pragma once

#include "Actor.h"
#include "Chunk.h"
#include "Core/Time.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"

class PPortal : public PActor
{
	int32_t		mSourceIndex = -1;
	std::string mTargetMap = "";
	int32_t		mTargetIndex = -1;

	PCollisionComponent* mCollisionComponent = nullptr;

public:
	PPortal();
	void  OnOverlapBegin(PActor* Actor) override;
	FRect GetLocalBounds() const override;
	bool  DebugDraw(const PRenderer* Renderer) const override;
	json  Serialize() const override;
	void  Deserialize(const json& Data) override;
};