#pragma once

#include "Actor.h"
#include "Core/Common.h"
#include "Core/Time.h"
#include "Engine/Components/CollisionComponent.h"
#include "Engine/World.h"

class PPortal : public PActor, public IDrawable
{
	std::string mTargetMap      = "";
	FVector2 mTargetPosition    = {0, 0};
	EOrientation mExitDirection = OR_South;

public:
	PPortal();
	void OnOverlapBegin(PActor* Actor) override;
	FRect GetLocalBounds() const override;
	bool DebugDraw(const PRenderer* Renderer) const override;
	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;
};
