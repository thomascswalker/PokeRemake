#pragma once

#include "Core/Vector.h"
#include "Engine/Object.h"
#include "Engine/Sprite.h"
#include "Renderer/IDrawable.h"

class PActor : public PObject, public IDrawable
{
protected:
	FVector2 mPosition;
	FVector2 mSize;

	PSprite mSprite{};
	bool	bBlocking = true;

public:
	PActor() = default;
	~PActor() override = default;
	PActor(const PActor& other)
		: PObject{ other }, IDrawable{ other }, mPosition{ other.mPosition }, mSize{ other.mSize }
	{
	}
	PActor(PActor&& other) noexcept
		: PObject{ std::move(other) }, IDrawable{ std::move(other) },
		  mPosition{ std::move(other.mPosition) }, mSize{ std::move(other.mSize) }
	{
	}
	PActor& operator=(const PActor& other)
	{
		if (this == &other)
			return *this;
		PObject::operator=(other);
		IDrawable::operator=(other);
		mPosition = other.mPosition;
		mSize = other.mSize;
		return *this;
	}
	PActor& operator=(PActor&& other) noexcept
	{
		if (this == &other)
			return *this;
		PObject::operator=(std::move(other));
		IDrawable::operator=(std::move(other));
		mPosition = std::move(other.mPosition);
		mSize = std::move(other.mSize);
		return *this;
	}

	void Start() override {}
	void End() override {}

	void	 Tick(float DeltaTime) override {}
	void	 Draw(const PRenderer* Renderer) const override = 0;
	FVector2 GetDrawPosition() const;

	virtual FRect GetLocalBounds() const { return FRect(); }
	virtual FRect GetWorldBounds() const { return FRect(); }

	FVector2 GetPosition() const { return mPosition; }
	void	 SetPosition(const FVector2& Position) { mPosition = Position; }
	void	 AddPosition(const FVector2& Position) { mPosition += Position; }

	void MoveToTile(int32_t X, int32_t Y);
	bool IsBlocking() const { return bBlocking; }
};
