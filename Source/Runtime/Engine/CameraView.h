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

	FVector2 mPosition;
	FVector2 mDirection;

public:
	float	  GetFOV() const { return mFOV; }
	EViewMode GetViewMode() const { return mViewMode; }

	void SetFOV(float InFOV) { mFOV = InFOV; }
	void SetViewMode(EViewMode InViewMode) { mViewMode = InViewMode; }

	void	 SetPosition(const FVector2& Position) { mPosition = Position; }
	FVector2 GetPosition() const { return mPosition; }

	FMatrix GetViewMatrix() const
	{
		auto	Target = FVector3(0.0f, 0.0f, 0.0f);
		auto	Up = FVector3(0.0f, 1.0f, 0.0f);
		FMatrix M = MakeLookAtMatrix({ mPosition.X, mPosition.Y, 0 }, Target, Up);
		// LogDebug("{}", M.ToString().c_str());
		return M;
	}
};

PCameraView* GetActiveCameraView();