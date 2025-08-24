#include "CameraComponent.h"

PCameraComponent::PCameraComponent()
{
	mCameraView.SetComponent(this);
}

void PCameraComponent::Tick(float DeltaTime)
{
	if (mOwner)
	{
		mCameraView.SetPosition(mOwner->GetPosition2D());
	}
}
