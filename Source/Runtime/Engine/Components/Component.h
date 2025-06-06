#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Object.h"

class PComponent : public PObject
{
protected:
	PActor* mOwner = nullptr;

public:
	PComponent() = default;
	PComponent(PActor* Owner) : mOwner(Owner) {}
	~PComponent() override = default;

	PActor*	 GetOwner() const { return mOwner; }
	void	 SetOwner(PActor* Owner) { mOwner = Owner; }
	void	 Tick(float DeltaTime) override {}
	FVector2 GetPosition() const;
};
