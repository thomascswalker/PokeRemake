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
		return mOwner->GetPosition2D();
	}
	LogWarning("Parent of component {} is invalid.", mInternalName.c_str());
	return FVector2(0, 0);
}

#if _EDITOR

void PComponent::InitializeParameters()
{
	AddParameter("Name", &mInternalName, PT_String);
}

#endif
