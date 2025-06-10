#pragma once

#include "Core/Delegate.h"
#include "Core/Logging.h"
#include "Engine/InputManager.h"
#include "Renderer/Renderer.h"

#define WIDGET_LIGHT 150, 150, 150, 255
#define WIDGET_MED 100, 100, 100, 255
#define WIDGET_DARK 50, 50, 50, 255

struct SWidgetEvent
{
	bool	 bMouseDown;
	FVector2 MousePosition;
};

class PWidget : public PObject
{
public:
	float X = 0.0f;
	float Y = 0.0f;
	float W = 0.0f;
	float H = 0.0f;

	PWidget() = default;

	virtual void Draw(const PRenderer* Renderer) const = 0;
	void		 Tick(float DeltaTime) override {}
	virtual void ProcessEvents(SWidgetEvent* Event) {}
	virtual void LayoutChildren() const {}
};

DECLARE_MULTICAST_DELEGATE(DButtonClicked);

class PButton : public PWidget
{
	std::string mLabel;
	bool		bDown = false;

public:
	DButtonClicked Clicked;

	explicit PButton(const std::string& Label) : mLabel(Label) {}
	FRect GetGeometry() const { return FRect{ X, Y, W, H }; }

	void OnMouseClick() { Clicked.Broadcast(); }
	void Start() override
	{
		if (const auto Input = GetInputManager())
		{
			Input->MouseClick.AddRaw(this, &PButton::OnMouseClick);
		}
	}

	void Draw(const PRenderer* Renderer) const override
	{
		const FRect Rect{ X, Y, W, H };
		if (bDown)
		{
			Renderer->SetDrawColor(WIDGET_DARK);
		}
		else if (Rect.Contains(Renderer->GetMousePosition()))
		{
			Renderer->SetDrawColor(WIDGET_LIGHT);
		}
		else
		{
			Renderer->SetDrawColor(WIDGET_MED);
		}
		Renderer->DrawFillRect(Rect);

		Renderer->DrawText(mLabel, FVector2(X, Y));
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		// Are we inside the button?
		if (GetGeometry().Contains(Event->MousePosition))
		{
			// Is it a new press?
			if (Event->bMouseDown && !bDown)
			{
				bDown = true;
			}
			// Is it a release?
			else if (!Event->bMouseDown && bDown)
			{
				bDown = false;
				Clicked.Broadcast(); // Notify listeners
			}
		}
		// Are we outside the button?
		else
		{
			bDown = false; // Reset button state
		}
	}
};

class PVerticalLayout : public PWidget
{
	std::vector<PWidget*> mChildren;
	float				  mSpacing = 5.0f;

public:
	PVerticalLayout() = default;

	void AddChild(PWidget* Child) { mChildren.push_back(Child); }

	void LayoutChildren() const override
	{
		float CY = Y;
		for (const auto& Child : mChildren)
		{
			Child->X = X + mSpacing;
			Child->Y = CY + mSpacing;
			CY += Child->H + mSpacing;
		}
	}

	void Draw(const PRenderer* Renderer) const override
	{
		for (const auto& Child : mChildren)
		{
			Child->Draw(Renderer);
		}
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		for (const auto& Child : mChildren)
		{
			Child->ProcessEvents(Event);
		}
	}
};