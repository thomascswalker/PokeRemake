#pragma once

#include "AbstractButton.h"
#include "Core/Color.h"
#include "Core/Delegate.h"
#include "Image.h"
#include "Text.h"
#include "Widget.h"

constexpr float gButtonTextSize = 16.0f;

class PButton : public PAbstractButton
{
	PText  mText{};
	PImage mImage{};

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
		PWidget::AddChild(&mText);
	}

	PButton(const std::string& Label, void (*Delegate)())
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)())
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		Clicked.AddRaw(Sender, Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)(bool))
		: mText(Label, gButtonTextSize)
	{
		PWidget::AddChild(&mText);
		Checked.AddRaw(Sender, Delegate);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		const FRect R = GetGeometry();

		// Clicked and hovered
		if (mMouseDown && mMouseOver)
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondaryClicked : PColor::UIPrimaryClicked);
		}
		// Hovered and not clicked
		else if (mMouseOver)
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondaryHover : PColor::UIPrimaryHover);
		}
		// Not hovered and not clicked
		else
		{
			Renderer->SetDrawColor(mChecked ? PColor::UISecondary : PColor::UIPrimary);
		}
		Renderer->DrawFillRect(R);

		// Border
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(R);
	}

	void DrawChildren(const PRenderer* Renderer) const override
	{
		PWidget::DrawChildren(Renderer);

		if (mChecked)
		{
			Renderer->SetDrawColor(PColor::UISecondary.WithAlpha(128));
			for (auto Child : mChildren)
			{
				Renderer->DrawFillRect(Child->GetGeometry());
			}
		}
	}

	float GetFontSize() const { return mText.GetFontSize(); }
	void  SetFontSize(float Size) { mText.SetFontSize(Size); }
};
