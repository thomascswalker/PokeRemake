#pragma once

#include "Component.h"
#include "Engine/CameraView.h"

class PCameraComponent : public PComponent
{


protected:
	FVector3	mTargetPosition;
	PCameraView mCameraView;

public:
	FVector3 GetTargetPosition() const { return mTargetPosition; }
	void	 SetTargetPosition(const FVector3& Position) { mTargetPosition = Position; }
	void	 AddTargetPosition(const FVector3& Position) { mTargetPosition += Position; }

	void		 Tick(float DeltaTime) override;
	PCameraView* GetCameraView() { return &mCameraView; }
};