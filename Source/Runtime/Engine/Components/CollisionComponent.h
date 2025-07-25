#pragma once

#include "Engine/Actors/Collider.h"

#include "Component.h"

class PCollisionComponent : public PComponent, public ICollider
{
public:
	void ProcessCollision() override;
};