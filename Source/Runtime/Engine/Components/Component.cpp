#include "Component.h"

#include "Core/Logging.h"

void PComponent::SetOwner(PActor* Owner)
{
	mOwner = Owner;
}

FVector2 PComponent::GetPosition() const
{
	if (mOwner)
	{
		return mOwner->GetPosition();
	}
	LogDebug("Parent is invalid");
	return FVector2(0, 0);
}
