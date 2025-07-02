#pragma once
#include "Button.h"
#include "Core/Delegate.h"
#include "Widget.h"

enum ESpinnerMode
{
	SM_Integer,
	SM_Float,
};

DECLARE_MULTICAST_DELEGATE(DValueChangedUp);
DECLARE_MULTICAST_DELEGATE(DValueChangedDown);
DECLARE_MULTICAST_DELEGATE(DValueChanged, float);

class PSpinner : public PWidget
{
public:
	DValueChanged ValueChanged;

protected:
	float		 mValue = 0.0f;
	ESpinnerMode mMode = SM_Integer;

	PText	mText;
	PBox	mButtonBox;
	PButton mUpButton;
	PButton mDownButton;

	DValueChangedUp	  ValueChangedUp;
	DValueChangedDown ValueChangedDown;

	void OnValueChangedUpInternal()
	{
		mValue += 1;
		mText.SetText(std::format("{}", mValue));
		ValueChanged.Broadcast(mValue);
	}
	void OnValueChangedDownInternal()
	{
		mValue -= 1;
		mText.SetText(std::format("{}", mValue));
		ValueChanged.Broadcast(mValue);
	}

public:
	explicit PSpinner(float Value = 0.0f)
		: mValue(Value), mText("0"), mUpButton("^"), mDownButton("v")
	{
		H = WIDGET_HEIGHT;
		mResizeModeW = RM_Grow;

		PWidget::AddChild(&mText);
		mText.SetText(std::format("{}", mValue));
		PWidget::AddChild(&mButtonBox);

		mUpButton.SetResizeMode(RM_Fixed, RM_Fixed);
		mUpButton.W = WIDGET_HEIGHT;
		mUpButton.H = WIDGET_HEIGHT / 2.0f;
		mUpButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedUpInternal);

		mDownButton.SetResizeMode(RM_Fixed, RM_Fixed);
		mDownButton.W = WIDGET_HEIGHT;
		mDownButton.H = WIDGET_HEIGHT / 2.0f;
		mDownButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedDownInternal);

		mButtonBox.AddChild(&mUpButton);
		mButtonBox.AddChild(&mDownButton);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		// Background
		Renderer->SetDrawColor(PColor::UIBackground);
		Renderer->DrawFillRect(GetGeometry());

		// Outline
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(GetGeometry());

		for (const auto& Child : mChildren)
		{
			Child->Draw(Renderer);
		}
	}
};