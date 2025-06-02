#include "Component.h"

#include "Core/Logging.h"

FVector2 PComponent::GetPosition() const
{
	if (mOwner)
	{
		return mOwner->GetPosition();
	}
	LogDebug("Parent is invalid");
	return FVector2(0, 0);
}
