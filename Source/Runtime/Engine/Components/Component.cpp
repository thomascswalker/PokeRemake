#include "Component.h"

#include "Core/Logging.h"

FVector3 PComponent::GetPosition() const
{
	if (mOwner)
	{
		return mOwner->GetPosition();
	}
	LogDebug("Parent is invalid");
	return FVector3(0, 0, 0);
}
