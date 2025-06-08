#pragma once
#include "Core/Rect.h"
#include "Texture.h"

#include <vector>

struct PAnimation
{
	std::string			  Name;
	std::vector<uint32_t> Indexes;
	uint32_t			  CurrentIndex = 0;

	void Next() { CurrentIndex = CurrentIndex + 1 >= Indexes.size() ? 0 : CurrentIndex + 1; }
	void Previous() { CurrentIndex = CurrentIndex - 1 < 0 ? Indexes.size() - 1 : CurrentIndex - 1; }
	void Reset() { CurrentIndex = 0; }
	uint32_t GetCurrentIndex() const
	{
		if (Indexes.empty())
		{
			return 0;
		}
		return Indexes[CurrentIndex];
	}
};

class PSprite : public PObject
{
	using Animations = std::map<std::string, PAnimation>;

	PTexture*	mTexture;
	Animations	mAnimations;
	PAnimation* mCurrentAnim;

	float mAnimationSpeed = DEFAULT_ANIM_SPEED; // Default animation speed in seconds
	float mAnimationTimer = 0.0f;				// Timer to track animation speed

public:
	PSprite() = default;

	void Tick(float DeltaTime) override
	{
		if (!mCurrentAnim)
		{
			return;
		}
		mAnimationTimer += DeltaTime;
		if (mAnimationTimer >= DEFAULT_ANIM_SPEED)
		{
			mCurrentAnim->Next();
			mAnimationTimer = 0.0f; // Reset the timer after updating the animation
		}
	}
	void	  SetTexture(PTexture* InTexture) { mTexture = InTexture; }
	PTexture* GetTexture() const { return mTexture; }
	FRect	  GetCurrentRect() const
	{
		if (mCurrentAnim && !mCurrentAnim->Indexes.empty())
		{
			const auto Index = mCurrentAnim->GetCurrentIndex();
			return { static_cast<float>(Index * 16), 0, 16, 16 };
		}
		return FRect();
	}

	void AddAnimation(const std::string& Name, const std::vector<uint32_t>& Indexes)
	{
		mAnimations[Name] = PAnimation(Name, Indexes);
	}

	PAnimation* GetAnimation(const std::string& Name)
	{
		const auto It = mAnimations.find(Name);
		if (It != mAnimations.end())
		{
			return &It->second;
		}
		return nullptr;
	}

	PAnimation* GetCurrentAnimation() const { return mCurrentAnim; }

	std::string GetCurrentAnimationName() const
	{
		if (mCurrentAnim)
		{
			return mCurrentAnim->Name;
		}
		return "";
	}

	void SetCurrentAnimation(const std::string& Name)
	{
		const auto It = mAnimations.find(Name);
		if (It != mAnimations.end())
		{
			mCurrentAnim = &It->second;
		}
		else
		{
			mCurrentAnim = nullptr;
		}
		mAnimationTimer = 0.0f; // Reset animation timer
	}
};
