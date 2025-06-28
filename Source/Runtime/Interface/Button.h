#pragma once

#include "Core/Delegate.h"
#include "Text.h"
#include "Widget.h"

DECLARE_MULTICAST_DELEGATE(DButtonClicked);

template <typename T>
struct SButtonClickedParams
{
	T* Sender;
	void (T::*Delegate)();

	SButtonClickedParams() : Sender(nullptr), Delegate(nullptr) {}
	SButtonClickedParams(T* Sender, void (T::*Delegate)()) : Sender(Sender), Delegate(Delegate) {}
	SButtonClickedParams(std::initializer_list<void*> init)
	{
		auto it = init.begin();
		if (init.size() == 2)
		{
			Sender = static_cast<T*>(*it);
			Delegate = reinterpret_cast<void (T::*)()>(*(it + 1));
		}
	}
};

struct SButtonClickedStaticParams
{
	void (*Delegate)();
};

class PButton : public PWidget
{
	PText mText;
	bool  bDown = false;

public:
	DButtonClicked Clicked;

	PButton(const std::string& Label) : mText(Label)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
	}

	PButton(const std::string& Label, void (*Delegate)()) : mText(Label)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
		Clicked.AddStatic(Delegate);
	}

	template <typename T>
	PButton(const std::string& Label, T* Sender, void (T::*Delegate)()) : mText(Label)
	{
		PWidget::AddChild(&mText);
		mResizeMode = RM_ExpandX;
		H = WIDGET_HEIGHT;
		Clicked.AddRaw(Sender, Delegate);
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

		Renderer->SetDrawColor(WIDGET_DARK);
		Renderer->DrawRect(Rect);

		Renderer->DrawText(mText.GetText(), FVector2(X + W / 2.0f, Y + H / 2.0f),
						   mText.GetFontSize());
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		mSender = this;

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
				Event->bConsumed = true;
				Clicked.Broadcast(); // Notify listeners
			}
		}
		// Are we outside the button?
		else
		{
			bDown = false; // Reset button state
		}
	}

	float GetFontSize() const { return mText.GetFontSize(); }
	void  SetFontSize(float Size) { mText.SetFontSize(Size); }
};
