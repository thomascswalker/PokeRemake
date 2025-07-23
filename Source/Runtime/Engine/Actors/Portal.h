#pragma once

#include "Actor.h"
#include "Core/Time.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/World.h"

class PPortal : public PActor
{
	std::string mTargetMap = "";
	FVector2	mTargetPosition = { 0, 0 };

	PCollisionComponent* mCollisionComponent = nullptr;

public:
	PPortal();
	void  OnOverlapBegin(PActor* Actor) override;
	FRect GetLocalBounds() const override;
	bool  DebugDraw(const PRenderer* Renderer) const override;
	JSON  Serialize() const override;
	void  Deserialize(const JSON& Data) override;
};