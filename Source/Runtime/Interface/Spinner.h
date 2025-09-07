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
		auto  Offset = Pos + Size / 2.0f - Scale / 2.0f;

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

	DelegateHandle mValueChangedDelegate;

public:
	DValueChanged ValueChanged;

protected:
	float mValue = 0.0f;
	float mMinValue = std::numeric_limits<float>::lowest();
	float mMaxValue = std::numeric_limits<float>::max();
	float mStepValue = 1.0f;

	ESpinnerMode mMode = SM_Integer;

	PBox*			mBox;
	PText*			mText;
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
		mValue = std::max(mMinValue, std::min(mMaxValue, mValue + mStepValue));
		OnValueChangedInternal();
	}

	void OnValueChangedDownInternal()
	{
		mValue = std::max(mMinValue, std::min(mMaxValue, mValue - mStepValue));
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

		PBox* ButtonBox = ConstructWidget<PBox>();
		ButtonBox->SetResizeMode(RM_Fixed, RM_Fixed);
		ButtonBox->SetFixedSize({ mFixedSize.Y, mFixedSize.Y });

		mUpButton = ConstructWidget<PSpinnerButton>(this, &PSpinner::OnValueChangedUpInternal);
		mUpButton->SetUp(true);
		ButtonBox->AddChild(mUpButton);

		mDownButton = ConstructWidget<PSpinnerButton>(this, &PSpinner::OnValueChangedDownInternal);
		mDownButton->SetUp(false);
		ButtonBox->AddChild(mDownButton);

		mBox = ConstructWidget<PBox>();
		mBox->SetResizeMode(RM_Fixed, RM_Fixed);
		mBox->SetFixedSize({ mFixedSize.Y / 2, mFixedSize.Y });

		PWidget::AddChild(mBox);
		PWidget::AddChild(mText);
		PWidget::AddChild(ButtonBox);
	}

	void Draw(const PRenderer* Renderer) const override
	{
		// Background
		Renderer->SetDrawColor(Style.Background);
		Renderer->DrawFillRect(GetGeometry());

		// Outline
		Renderer->SetDrawColor(Style.Border);
		Renderer->DrawRect(GetGeometry());
	}

	float GetValue() const { return mValue; }
	void  SetValue(float Value)
	{
		mValue = Value;
		OnValueChangedInternal();
	}

	void SetRange(float Min, float Max)
	{
		mMinValue = Min;
		mMaxValue = Max;
	}

	void SetStep(float Value)
	{
		mStepValue = Value;
	}

	void SetColor(const PColor& Color)
	{
		mBox->Style.Primary = Color;
	}

#if _EDITOR
	void Bind(PParameter* NewParam) override
	{
		// Set the parameter reference
		PWidget::Bind(NewParam);

		// Override the existing value with the parameter's current value
		mValue = Param->Get<float>();
		OnValueChangedInternal(); // Update text to match

		// Bind events from this widget to set the parameter value
		mValueChangedDelegate = ValueChanged.AddRaw(Param, &PParameter::Set<float>);
	}

	void Bind(PParameter* NewParam, int32_t Index) override
	{
		PWidget::Bind(NewParam, Index);

		auto Ptr = static_cast<float*>(Param->GetRaw());
		mValue = Ptr[Index];
		OnValueChangedInternal(); // Update text to match

		mValueChangedDelegate = ValueChanged.AddLambda([this, Index](float Value) {
			Param->Set<float>(Value, Index);
		});
	}

	void Unbind() override
	{
		PWidget::Unbind();

		ValueChanged.Remove(mValueChangedDelegate);
	}
#endif
};
