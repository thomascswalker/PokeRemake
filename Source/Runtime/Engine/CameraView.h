#pragma once

#define DEFAULT_FOV 90.0f

#include "Core/Matrix.h"

#include "Object.h"

constexpr float gMinZoom = 0.1f;
constexpr float gMaxZoom = 4.0f;

enum EViewMode
{
	VM_Perspective,
	VM_Orthographic,
};

class PCameraView : public PObject
{
	PComponent* mComponent = nullptr;

protected:
	// 2D
	FVector2 mPosition;
	float	 mZoom = 1.0f;

	// 3D
	float	  mFOV = DEFAULT_FOV;
	EViewMode mViewMode = VM_Orthographic;
	FVector2  mDirection;

public:
	void Tick(float DeltaTime) override {}

	PComponent* GetComponent() const;
	void		SetComponent(PComponent* Component);

	float GetFOV() const
	{
		return mFOV;
	}

	EViewMode GetViewMode() const
	{
		return mViewMode;
	}

	void SetFOV(float InFOV)
	{
		mFOV = InFOV;
	}

	void SetViewMode(EViewMode InViewMode)
	{
		mViewMode = InViewMode;
	}

	void SetPosition(const FVector2& Position)
	{
		mPosition = Position;
	}

	FVector2 GetPosition() const
	{
		return mPosition + FVector2(TILE_SIZE, TILE_SIZE);
	}

	float GetZoom() const
	{
		return mZoom;
	}

	void SetZoom(float Zoom)
	{
		mZoom = Zoom;
	}

	void AddZoom(float Value)
	{
		float NewZoom = mZoom + Value * 0.1f;
		mZoom = std::min(gMaxZoom, std::max(gMinZoom, NewZoom));
	}

	FMatrix GetViewMatrix() const
	{
		auto	Target = FVector3(0.0f, 0.0f, 0.0f);
		auto	Up = FVector3(0.0f, 1.0f, 0.0f);
		FMatrix M = MakeLookAtMatrix({ mPosition.X, mPosition.Y, 0 }, Target, Up);
		// LogDebug("{}", M.ToString().c_str());
		return M;
	}
};

DECLARE_STATIC_GLOBAL_GETTER(CameraView)
