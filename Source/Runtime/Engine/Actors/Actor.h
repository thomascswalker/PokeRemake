#pragma once

#include "Core/Vector.h"
#include "Engine/Object.h"
#include "Renderer/IDrawable.h"

class PActor : public PObject, public IDrawable
{
protected:
	FVector2 mPosition;

public:
	PActor() = default;
	~PActor() override = default;

	void Start() override {}
	void End() override {}

	void Tick(float DeltaTime) override {}
	void Draw(const PRenderer* Renderer) const override = 0;

	FVector2 GetPosition() const { return mPosition; }
	void	 SetPosition(const FVector2& Position) { mPosition = Position; }
	void	 AddPosition(const FVector2& Position) { mPosition += Position; }

	template <typename T, typename... ArgsType>
	T* AddComponent(ArgsType&&... Args)
	{
		T* Component = new T(this, Args...);
		return Component;
	}
};
