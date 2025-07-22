#pragma once
#include "Component.h"

class PCollisionComponent : public PComponent, public ICollider
{
public:
	void Tick(float DeltaTime) override;
};