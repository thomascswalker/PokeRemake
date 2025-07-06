#pragma once
#include "Widget.h"

class PImage : public PWidget
{
	PTexture* mTexture;
	FRect	  mSourceRect;
	bool	  mUseSourceRect;

public:
	PImage(PTexture* Texture)
		: mTexture(Texture), mUseSourceRect(false) {}
	PImage(const std::string& Name)
		: mUseSourceRect(false)
	{
		mTexture = PTextureManager::Get(Name);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		if (!mTexture)
		{
			return;
		}

		FRect Rect = mUseSourceRect ? mSourceRect : FRect{ 0, 0, mTexture->GetWidth(), mTexture->GetHeight() };
		Renderer->DrawTexture(mTexture, Rect, GetGeometry());
	}

	PTexture* GetTexture() const { return mTexture; }
	void	  SetTexture(PTexture* Texture) { mTexture = Texture; }

	bool GetUseSourceRect() const { return mUseSourceRect; }
	void SetUseSourceRect(bool UseSourceRect) { mUseSourceRect = UseSourceRect; }

	FRect GetSourceRect() const { return mSourceRect; }
	void  SetSourceRect(FRect SourceRect) { mSourceRect = SourceRect; }
};