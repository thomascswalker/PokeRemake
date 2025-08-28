#pragma once
#include "Core/Rect.h"
#include "Texture.h"

#include <vector>

#include "Core/String.h"

struct PAnimation
{
	std::string Name;
	std::vector<uint32_t> Indexes;
	uint32_t CurrentIndex = 0;

	void Next()
	{
		auto PrevIndex = Indexes[CurrentIndex];
		CurrentIndex   = CurrentIndex + 1 >= Indexes.size() ? 0 : CurrentIndex + 1;
	}

	void Previous()
	{
		CurrentIndex = CurrentIndex - 1 < 0 ? Indexes.size() - 1 : CurrentIndex - 1;
	}

	void Reset()
	{
		CurrentIndex = 0;
	}

	uint32_t GetCurrentIndex() const
	{
		if (Indexes.empty())
		{
			return 0;
		}
		return Indexes[CurrentIndex];
	}

	JSON Serialize() const
	{
		JSON Result;
		Result["Name"]    = Name;
		Result["Indexes"] = Indexes;
		return Result;
	}

	void Deserialize(const JSON& Json)
	{
		Indexes.clear();
		Indexes.resize(Json["Indexes"].size());
		for (int i = 0; i < Json["Indexes"].size(); i++)
		{
			Indexes[i] = Json["Indexes"][i]["Index"];
		}
		Name         = Json.at("Name");
		CurrentIndex = 0;
	}
};

enum ESpriteSize
{
	SS_8  = 8,
	SS_16 = 16
};

class PSprite : public PObject
{
	PTexture* mTexture;

	// Pixel size (width and height) of each sprite
	float mSize = 16.0f;
	// Pixel size of each index within the texture atlas
	float mIndexSize = 8.0f;
	int32_t mWidth   = 1;

	std::map<std::string, PAnimation> mAnimations;
	PAnimation* mCurrentAnim;

	float mAnimationSpeed = DEFAULT_ANIM_SPEED; // Default animation speed in seconds
	float mAnimationTimer = 0.0f;               // Timer to track animation speed

public:
	PSprite() : mTexture(nullptr), mCurrentAnim(nullptr) {}

	PSprite(const PSprite& other)
		: PObject{other},
		  mTexture{other.mTexture},
		  mSize{other.mSize},
		  mAnimations{other.mAnimations},
		  mCurrentAnim{other.mCurrentAnim},
		  mAnimationSpeed{other.mAnimationSpeed},
		  mAnimationTimer{other.mAnimationTimer} {}

	PSprite(PSprite&& other) noexcept
		: PObject{std::move(other)},
		  mTexture{other.mTexture},
		  mSize{other.mSize},
		  mAnimations{std::move(other.mAnimations)},
		  mCurrentAnim{other.mCurrentAnim},
		  mAnimationSpeed{other.mAnimationSpeed},
		  mAnimationTimer{other.mAnimationTimer} {}

	PSprite& operator=(const PSprite& other)
	{
		if (this == &other)
			return *this;
		PObject::operator =(other);
		mTexture        = other.mTexture;
		mSize           = other.mSize;
		mAnimations     = other.mAnimations;
		mCurrentAnim    = other.mCurrentAnim;
		mAnimationSpeed = other.mAnimationSpeed;
		mAnimationTimer = other.mAnimationTimer;
		return *this;
	}

	PSprite& operator=(PSprite&& other) noexcept
	{
		if (this == &other)
			return *this;
		PObject::operator =(std::move(other));
		mTexture        = other.mTexture;
		mSize           = other.mSize;
		mAnimations     = std::move(other.mAnimations);
		mCurrentAnim    = other.mCurrentAnim;
		mAnimationSpeed = other.mAnimationSpeed;
		mAnimationTimer = other.mAnimationTimer;
		return *this;
	}

	void Tick(float DeltaTime) override
	{
		if (!mCurrentAnim || mCurrentAnim->Indexes.size() <= 1)
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

	void SetTexture(PTexture* InTexture)
	{
		mTexture = InTexture;
		mWidth   = mTexture->GetWidth() / mIndexSize;
	}

	PTexture* GetTexture() const
	{
		return mTexture;
	}

	FRect GetCurrentRect() const
	{
		if (mCurrentAnim && !mCurrentAnim->Indexes.empty())
		{
			const auto Index = mCurrentAnim->GetCurrentIndex();
			const auto X     = Index % static_cast<uint32_t>(mWidth);
			const auto Y     = Index / static_cast<uint32_t>(mWidth);
			return {X * mIndexSize, Y * mIndexSize, mSize, mSize};
		}
		return FRect();
	}

	void AddAnimation(const std::string& Name, const std::vector<uint32_t>& Indexes)
	{
		mAnimations[Name] = PAnimation(Name, Indexes);
		if (!mCurrentAnim)
		{
			mCurrentAnim = &mAnimations[Name];
		}
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

	PAnimation* GetCurrentAnimation() const
	{
		return mCurrentAnim;
	}

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

	void SetSize(float Size)
	{
		mSize = Size;
	}

	float GetSize() const
	{
		return mSize;
	}

	float GetIndexSize() const
	{
		return mIndexSize;
	}

	void SetIndexSize(float Size)
	{
		mIndexSize = Size;
	}

	JSON Serialize() const override
	{
		JSON Result          = PObject::Serialize();
		Result["Animations"] = {};
		for (const auto& V : mAnimations | std::views::values)
		{
			Result["Animations"].push_back(V.Serialize());
		}
		Result["Texture"] = GetTexture()->GetName();
		Result["Size"]    = mSize;
		return Result;
	}

	void Deserialize(const JSON& Data) override
	{
		PObject::Deserialize(Data);

		auto Texture = Data["Texture"].get<std::string>();
		mTexture     = TextureManager::Get(Texture);
		LogDebug("Loaded texture: {}", mTexture->GetName());
		LOAD_MEMBER_PROPERTY(Size, float);
		LOAD_MEMBER_PROPERTY(IndexSize, float);
		LOAD_MEMBER_PROPERTY(Width, int32_t);

		LogDebug("Loading {} animations:\n{}", Data["Animations"].size(), Data["Animations"].dump());
		for (auto& Anim : Data["Animations"])
		{
			std::string Name              = Anim["Name"];
			std::vector<uint32_t> Indexes = Anim["Indexes"];
			AddAnimation(Name, Indexes);
		}
	}
};
