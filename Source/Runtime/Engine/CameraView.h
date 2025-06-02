#pragma once

#define DEFAULT_FOV 90.0f

#include "Core/Matrix.h"

enum EViewMode
{
	VM_Perspective,
	VM_Orthographic,
};

class PCameraView
{
protected:
	float	  mFOV = DEFAULT_FOV;
	EViewMode mViewMode = VM_Orthographic;

	FVector3 mPosition;
	FVector3 mDirection;

public:
	float	  GetFOV() const { return mFOV; }
	EViewMode GetViewMode() const { return mViewMode; }

	void SetFOV(float InFOV) { mFOV = InFOV; }
	void SetViewMode(EViewMode InViewMode) { mViewMode = InViewMode; }

	void	 SetPosition(const FVector3& Position) { mPosition = Position; }
	FVector3 GetPosition() const { return mPosition; }

	FMatrix GetViewMatrix() const
	{
		auto	Target = FVector3(0.0f, 0.0f, 0.0f);
		auto	Up = FVector3(0.0f, 1.0f, 0.0f);
		FMatrix M = MakeLookAtMatrix(mPosition, Target, Up);
		// LogDebug("{}", M.ToString().c_str());
		return M;
	}
};

PCameraView* GetActiveCameraView();