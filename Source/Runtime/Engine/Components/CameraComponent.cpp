#include "CameraComponent.h"

void PCameraComponent::Tick(float DeltaTime)
{
	if (mOwner)
	{
		mCameraView.SetPosition(mOwner->GetPosition2D());
	}
}