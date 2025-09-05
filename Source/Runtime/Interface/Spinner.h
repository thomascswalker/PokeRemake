#pragma once

#include "Core/Delegate.h"

#include "Box.h"
#include "Button.h"
#include "Text.h"
#include "Widget.h"

enum ESpinnerMode
{
	SM_Integer,
	SM_Float,
};

class PSpinnerButton : public PButton
{
	bool				  mUp = false;
	std::vector<FVector2> mVertices;

public:
	// Required for constructor inheritance
	using PButton::PButton;

	void SetUp(bool Up) { mUp = Up; }

	void OnLayout() override
	{
		for (auto Child : mChildren)
		{
			Child->OnLayout();
		}

		// Button arrows
		float Scale = 3.0f;
		auto  Pos = GetGeometry().GetPosition();
		auto  Size = GetGeometry().GetSize();

		float Factor = mUp ? -1.0f : 1.0f;
		auto  Offset = Pos + (Size / 2.0f) - (Scale / 2.0f);

		auto V0 = FVector2{ -Scale * Factor, -Scale * Factor };
		auto V1 = FVector2{ 0.0f, Scale * Factor };
		auto V2 = FVector2{ Scale * Factor, -Scale * Factor };

		mVertices = { V0 + Offset, V1 + Offset, V2 + Offset };
	}

	void Draw(const PRenderer* Renderer) const override
	{
		PButton::Draw(Renderer);

		Renderer->SetDrawColor(PColor::UIText);
		Renderer->DrawPolygon(mVertices, { 0, 1, 2 });
	}
};

class PSpinner : public PWidget
{
	DECLARE_MULTICAST_DELEGATE(DValueChangedUp);
	DECLARE_MULTICAST_DELEGATE(DValueChangedDown);
	DECLARE_MULTICAST_DELEGATE(DValueChanged, float);

public:
	DValueChanged ValueChanged;

protected:
	float mValue = 0.0f;
	float mMinValue = std::numeric_limits<float>::min();
	float mMaxValue = std::numeric_limits<float>::max();

	ESpinnerMode mMode = SM_Integer;

	PText*			mText;
	PBox*			mButtonBox;
	PSpinnerButton* mUpButton;
	PSpinnerButton* mDownButton;

	DValueChangedUp	  ValueChangedUp;
	DValueChangedDown ValueChangedDown;

	void OnValueChangedInternal()
	{
		mText->SetText(std::format("{}", mValue));
		ValueChanged.Broadcast(mValue);
	}

	void OnValueChangedUpInternal()
	{
		mValue = std::clamp(mValue++, mMinValue, mMaxValue);
		OnValueChangedInternal();
	}

	void OnValueChangedDownInternal()
	{
		mValue = std::clamp(mValue--, mMinValue, mMaxValue);
		OnValueChangedInternal();
	}

public:
	explicit PSpinner(float Value = 0.0f)
		: mValue(Value)
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;

		mText = ConstructWidget<PText>();
		mText->SetText(std::format("{}", mValue));
		//
		// mButtonBox = ConstructWidget<PBox>();
		// mButtonBox->SetLayoutMode(LM_Vertical);
		// mButtonBox->SetResizeMode(RM_Fit, RM_Fit);

		mUpButton = ConstructWidget<PSpinnerButton>(this, &PSpinner::OnValueChangedUpInternal);
		mUpButton->SetUp(true);
		mUpButton->SetResizeMode(RM_Fixed, RM_Fixed);
		mUpButton->SetFixedSize({ mFixedSize.Y, mFixedSize.Y / 2 });
		PWidget::AddChild(mUpButton);

		mDownButton = ConstructWidget<PSpinnerButton>(this, &PSpinner::OnValueChangedDownInternal);
		mDownButton->SetUp(false);
		mDownButton->SetResizeMode(RM_Fixed, RM_Fixed);
		mDownButton->SetFixedSize({ mFixedSize.Y, mFixedSize.Y / 2 });
		PWidget::AddChild(mDownButton);

		// PWidget::AddChild(mButtonBox);
		PWidget::AddChild(mText);
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

	float GetValue() const { return mValue; }
	void  SetValue(float Value)
	{
		mValue = Value;
		OnValueChangedInternal();
	}

	bool ProcessEvents(SInputEvent* Event) override
	{
		if (Event->Type == IET_MouseDown || Event->Type == IET_MouseUp)
		{

			int Test = 5;
		}
		return PWidget::ProcessEvents(Event);
	}

#if _EDITOR
	void Bind(PParameter* Param) override
	{
		// Set the parameter reference
		PWidget::Bind(Param);

		// Override the existing text with the parameter's current text
		mValue = Param->Get<float>();

		// Bind events from this widget to set the parameter value
		ValueChanged.AddRaw(Param, &PParameter::Set<float>);
	}
#endif
};
