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
	PTexture*	mTexture = nullptr;

	FRect mSourceRect;
	bool  mUseSourceRect = false;

public:
	PButton() = default;

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
	{
	}

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
	{
	}

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
				Color = (mChecked ? PColor::UISecondaryClicked.WithAlpha(128) : PColor::Black.WithAlpha(100));
			}
			// Hovered and not clicked
			else if (mMouseOver)
			{
				Color = (mChecked ? PColor::UISecondaryHover.WithAlpha(128) : PColor::Black.WithAlpha(50));
			}
			// Not hovered and not clicked
			else
			{
				Color = (mChecked ? PColor::UISecondary.WithAlpha(128) : PColor::Black.WithAlpha(0));
			}
			Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);
		}
		else
		{
			if (mMouseDown && mMouseOver)
			{
				Color = (mChecked ? PColor::UISecondaryClicked : PColor::UIPrimaryClicked);
			}
			// Hovered and not clicked
			else if (mMouseOver)
			{
				Color = (mChecked ? PColor::UISecondaryHover : PColor::UIPrimaryHover);
			}
			// Not hovered and not clicked
			else
			{
				Color = (mChecked ? PColor::UISecondary : PColor::UIPrimary);
			}
		}

		Renderer->SetDrawColor(Color);
		Renderer->DrawFillRect(R);
		Renderer->SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);

		// Draw text
		if (!mText.empty())
		{
			Renderer->DrawText(mText, FVector2(X + W / 2.0f, Y + H / 2.0f), gButtonTextSize);
		}

		// Border
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(R);
	}

	void  SetUseSourceRect(bool State) { mUseSourceRect = State; }
	bool  GetUseSourceRect() const { return mUseSourceRect; }
	void  SetSourceRect(const FRect& SourceRect) { mSourceRect = SourceRect; }
	FRect GetSourceRect() const { return mSourceRect; }
};
