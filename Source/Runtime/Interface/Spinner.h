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

	PButton mUpButton;
	PButton mDownButton;

	DValueChangedUp	  ValueChangedUp;
	DValueChangedDown ValueChangedDown;

	void OnValueChangedUpInternal()
	{
		mValue += 1;
		ValueChanged.Broadcast(mValue);
	}
	void OnValueChangedDownInternal()
	{
		mValue -= 1;
		ValueChanged.Broadcast(mValue);
	}

public:
	explicit PSpinner() : mUpButton("^"), mDownButton("v")
	{
		mUpButton.W = WIDGET_HEIGHT;
		mUpButton.H = WIDGET_HEIGHT / 2.0f;
		mUpButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedUpInternal);

		mDownButton.W = WIDGET_HEIGHT;
		mDownButton.H = WIDGET_HEIGHT / 2.0f;
		mDownButton.Clicked.AddRaw(this, &PSpinner::OnValueChangedDownInternal);

		PWidget::AddChild(&mUpButton);
		PWidget::AddChild(&mDownButton);

		X = W - WIDGET_HEIGHT;
	}

	void Draw(const PRenderer* Renderer) const override
	{
		// Background
		Renderer->SetDrawColor(WIDGET_DARK);
		Renderer->DrawFillRect(GetGeometry());

		// Outline
		Renderer->SetDrawColor(WIDGET_LIGHT);
		Renderer->DrawRect(GetGeometry());

		// Value text
		Renderer->SetDrawColor(WIDGET_TEXT);
		const std::string ValueText = (mMode == SM_Float)
										  ? std::format("{:.2f}", mValue)
										  : std::to_string(static_cast<int>(mValue));
		Renderer->DrawText(ValueText, { X + 10, Y + H / 2.0f }, 14);

		for (const auto& Child : mChildren)
		{
			Child->Draw(Renderer);
		}
	}

	void LayoutChildren() override
	{
		LayoutVertical(mChildren, FVector2(X + W - WIDGET_HEIGHT, Y), 0.0f);
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		mSender = this;
		for (const auto& Child : mChildren)
		{
			Child->ProcessEvents(Event);
		}
	}
};