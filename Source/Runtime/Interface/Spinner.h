#pragma once

#include "Core/Delegate.h"

#include "Button.h"
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
	float mValue = 0.0f;
	float mMinValue = std::numeric_limits<float>::min();
	float mMaxValue = std::numeric_limits<float>::max();

	ESpinnerMode mMode = SM_Integer;

	PWidget mLayoutWidget;
	PText	mText;
	PBox	mButtonBox;
	PButton mUpButton;
	PButton mDownButton;

	DValueChangedUp	  ValueChangedUp;
	DValueChangedDown ValueChangedDown;

	void OnValueChangedUpInternal()
	{
		mValue += 1;
		mValue = std::min(mValue, mMaxValue);
		mText.SetText(std::format("{}", mValue));
		ValueChanged.Broadcast(mValue);
	}
	void OnValueChangedDownInternal()
	{
		mValue -= 1;
		mValue = std::max(mValue, mMinValue);
		mText.SetText(std::format("{}", mValue));
		ValueChanged.Broadcast(mValue);
	}

public:
	explicit PSpinner(float Value = 0.0f)
		: mValue(Value), mText("0"), mUpButton("^"), mDownButton("v")
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;
		Padding = { 0 };

		mText.SetText(std::format("{}", mValue));

		mButtonBox.SetLayoutMode(LM_Vertical);
		mButtonBox.SetResizeMode(RM_Fit, RM_Fit);
		mButtonBox.Padding = { 0 };

		mUpButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedUpInternal);
		mUpButton.SetResizeMode(RM_Fixed, RM_Fixed);
		mUpButton.SetFixedSize({ DEFAULT_WIDGET_HEIGHT, DEFAULT_WIDGET_HEIGHT / 2 });

		mDownButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedDownInternal);
		mDownButton.SetResizeMode(RM_Fixed, RM_Fixed);
		mDownButton.SetFixedSize({ DEFAULT_WIDGET_HEIGHT, DEFAULT_WIDGET_HEIGHT / 2 });

		mButtonBox.AddChild(&mUpButton);
		mButtonBox.AddChild(&mDownButton);

		PWidget::AddChild(&mText);
		PWidget::AddChild(&mButtonBox);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		// Background
		Renderer->SetDrawColor(PColor::UIBackground);
		Renderer->DrawFillRect(GetGeometry());

		// Outline
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(GetGeometry());
	}

	void SetRange(float Min, float Max)
	{
		mMinValue = Min;
		mMaxValue = Max;
	}
};