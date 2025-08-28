#pragma once

#include "Core/Color.h"
#include "Core/Delegate.h"

#include "AbstractButton.h"
#include "Widget.h"

constexpr float gButtonTextSize = 16.0f;

class PButton : public PAbstractButton
{
protected:
	std::string mText;
	PTexture* mTexture = nullptr;

	FRect mSourceRect;
	bool mUseSourceRect = false;

public:
	PButton()
	{}

	template <typename T>
	PButton(T* Sender, void (T::*Delegate)())
	{
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(T* Sender, void (T::*Delegate)(bool))
	{
		Checked.AddRaw(Sender, Delegate);
	}

	PButton(const std::string& Label)
		: mText(Label)
	{}

	PButton(const std::string& Label, void (*Delegate)())
		: mText(Label)
	{
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)())
		: mText(Label)
	{
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)(bool))
		: mText(Label)
	{
		Checked.AddRaw(Sender, Delegate);
	}

	PButton(PTexture* Texture)
		: mTexture(Texture)
	{}

	PButton(PTexture* Texture, void (*Delegate)())
		: mTexture(Texture)
	{
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(PTexture* Texture, T* Sender, void (T::*Delegate)())
		: mTexture(Texture)
	{
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(PTexture* Texture, T* Sender, void (T::*Delegate)(bool))
		: mTexture(Texture)
	{
		Checked.AddRaw(Sender, Delegate);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		const FRect R = GetGeometry();

		// Clicked and hovered
		PColor Color;
		if (mTexture)
		{
			auto TextureRect = mUseSourceRect ? mSourceRect : mTexture->GetRect();
			Renderer->DrawTexture(mTexture, TextureRect, R);
			if (mMouseDown && mMouseOver)
			{
				Color = (mChecked ? mStyle.SecondaryClicked.WithAlpha(128) : PColor::Black.WithAlpha(100));
			}
			// Hovered and not clicked
			else if (mMouseOver)
			{
				Color = (mChecked ? mStyle.SecondaryHover.WithAlpha(128) : PColor::Black.WithAlpha(50));
			}
			// Not hovered and not clicked
			else
			{
				Color = (mChecked ? mStyle.Secondary.WithAlpha(128) : PColor::Black.WithAlpha(0));
			}
			Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);
		}
		else
		{
			if (mMouseDown && mMouseOver)
			{
				Color = (mChecked ? mStyle.SecondaryClicked : mStyle.PrimaryClicked);
			}
			// Hovered and not clicked
			else if (mMouseOver)
			{
				Color = (mChecked ? mStyle.SecondaryHover : mStyle.PrimaryHover);
			}
			// Not hovered and not clicked
			else
			{
				Color = (mChecked ? mStyle.Secondary : mStyle.Primary);
			}
		}

		Renderer->SetDrawColor(Color);
		Renderer->DrawFillRect(R);
		Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);

		// Draw text
		if (!mText.empty())
		{
			Renderer->SetDrawColor(mStyle.Text);
			Renderer->DrawText(mText, FVector2(X + W / 2.0f, Y + H / 2.0f), gButtonTextSize);
		}

		// Border
		Renderer->SetDrawColor(mStyle.Border);
		Renderer->DrawRect(R);
	}

	void SetUseSourceRect(bool State)
	{
		mUseSourceRect = State;
	}

	bool GetUseSourceRect() const
	{
		return mUseSourceRect;
	}

	void SetSourceRect(const FRect& SourceRect)
	{
		mSourceRect = SourceRect;
	}

	FRect GetSourceRect() const
	{
		return mSourceRect;
	}
};
