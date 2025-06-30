#pragma once

#include "AbstractButton.h"
#include "Core/Color.h"
#include "Core/Delegate.h"
#include "Text.h"
#include "Widget.h"

constexpr float gButtonTextSize = 16.0f;

class PButton : public PAbstractButton
{
	PText mText;

public:
	PButton(const std::string& Label)
		: mText(Label)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
	}

	PButton(const std::string& Label, void (*Delegate)())
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)())
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)(bool))
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
		Checked.AddRaw(Sender, Delegate);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		const FRect Rect{ X, Y, W, H };

		// Clicked
		if (mMouseDown)
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondaryClicked : PColor::UIPrimaryClicked);
		}
		// Hovered
		else if (Rect.Contains(Renderer->GetMousePosition()))
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondaryHover : PColor::UIPrimaryHover);
		}
		// Normal
		else
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondary : PColor::UIPrimary);
		}
		Renderer->DrawFillRect(Rect);

		// Border
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(Rect);

		// Text
		Renderer->SetDrawColor(PColor::UIText);
		Renderer->DrawText(mText.GetText(), FVector2(X + W / 2.0f, Y + H / 2.0f),
						   mText.GetFontSize());
	}

	float GetFontSize() const { return mText.GetFontSize(); }
	void  SetFontSize(float Size) { mText.SetFontSize(Size); }
};
