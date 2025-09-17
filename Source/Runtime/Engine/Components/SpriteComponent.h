#pragma once
#include "Component.h"

class PSpriteComponent : public PComponent, public IDrawable
{
	std::shared_ptr<PSprite> mSprite;
	FVector2				 mOffset{};

public:
	PSpriteComponent(float InSpeed = DEFAULT_ANIM_SPEED);
	~PSpriteComponent() override = default;

	PSpriteComponent(const PSpriteComponent& other)
		: PComponent{ other },
		  mSprite{ other.mSprite },
		  mOffset{ other.mOffset } {}

	PSpriteComponent(PSpriteComponent&& other) noexcept
		: PComponent{ std::move(other) },
		  mSprite{ std::move(other.mSprite) },
		  mOffset{ other.mOffset } {}

	PSpriteComponent& operator=(const PSpriteComponent& other)
	{
		if (this == &other)
			return *this;
		PComponent::operator=(other);
		mSprite = other.mSprite;
		mOffset = other.mOffset;
		return *this;
	}

	PSpriteComponent& operator=(PSpriteComponent&& other) noexcept
	{
		if (this == &other)
			return *this;
		PComponent::operator=(std::move(other));
		mSprite = std::move(other.mSprite);
		mOffset = other.mOffset;
		return *this;
	}

	bool  Draw(const PRenderer* Renderer) const override;
	void  Tick(float DeltaTime) override;
	float GetDepth() const override;

	void SetOffset(const FVector2& Offset)
	{
		mOffset = Offset;
	}

	PSprite* GetSprite()
	{
		return mSprite.get();
	}

	void SetSize(float Size)
	{
		mSprite->SetSize(Size);
	}

	void SetIndexSize(float Size)
	{
		mSprite->SetIndexSize(Size);
	}

	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;
};
