#pragma once

#include "Core/Color.h"
#include "Core/Delegate.h"
#include "Text.h"
#include "Engine/World.h"

#include "AbstractButton.h"
#include "Widget.h"

constexpr float gButtonTextSize = 16.0f;

class PButton : public PAbstractButton
{
protected:
	PText* mText;
	PTexture* mTexture = nullptr;

	FRect mSourceRect;
	bool mUseSourceRect = false;

public:
	PButton() : mText(ConstructWidget<PText>())
	{
		PWidget::AddChild(mText);
	}

	template <typename T>
	PButton(T* Sender, void (T::*Delegate)()) : mText(ConstructWidget<PText>())
	{
		PWidget::AddChild(mText);
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(T* Sender, void (T::*Delegate)(bool)) : mText(ConstructWidget<PText>())
	{
		PWidget::AddChild(mText);
		Checked.AddRaw(Sender, Delegate);
	}

	PButton(const std::string& Label)
		: mText(ConstructWidget<PText>(Label))
	{
		PWidget::AddChild(mText);
	}

	PButton(const std::string& Label, void (*Delegate)())
		: mText(ConstructWidget<PText>(Label))
	{
		PWidget::AddChild(mText);
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)())
		: mText(ConstructWidget<PText>(Label))
	{
		PWidget::AddChild(mText);
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)(bool))
		: mText(ConstructWidget<PText>(Label))
	{
		PWidget::AddChild(mText);
		Checked.AddRaw(Sender, Delegate);
	}

	PButton(PTexture* Texture)
		: mText(ConstructWidget<PText>()), mTexture(Texture)
	{
		PWidget::AddChild(mText);
	}

	PButton(PTexture* Texture, void (*Delegate)())
		: mText(ConstructWidget<PText>()), mTexture(Texture)
	{
		PWidget::AddChild(mText);
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(PTexture* Texture, T* Sender, void (T::*Delegate)())
		: mText(ConstructWidget<PText>()), mTexture(Texture)
	{
		PWidget::AddChild(mText);
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(PTexture* Texture, T* Sender, void (T::*Delegate)(bool))
		: mText(ConstructWidget<PText>()), mTexture(Texture)
	{
		PWidget::AddChild(mText);
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
