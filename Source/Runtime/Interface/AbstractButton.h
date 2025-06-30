#pragma once
#include "Core/Delegate.h"
#include "Widget.h"

DECLARE_MULTICAST_DELEGATE(DButtonClicked);
DECLARE_MULTICAST_DELEGATE(DButtonChecked, bool);

template <typename T>
struct SButtonClickedParams
{
	T* Sender;
	void (T::*Delegate)();

	SButtonClickedParams()
		: Sender(nullptr), Delegate(nullptr) {}
	SButtonClickedParams(T* Sender, void (T::*Delegate)())
		: Sender(Sender), Delegate(Delegate) {}
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

template <typename T>
struct SButtonCheckedParams
{
	T* Sender;
	void (T::*Delegate)(bool);

	SButtonCheckedParams()
		: Sender(nullptr), Delegate(nullptr) {}
	SButtonCheckedParams(T* Sender, void (T::*Delegate)(bool))
		: Sender(Sender), Delegate(Delegate) {}
	SButtonCheckedParams(std::initializer_list<void*> init)
	{
		auto it = init.begin();
		if (init.size() == 2)
		{
			Sender = static_cast<T*>(*it);
			Delegate = reinterpret_cast<void (T::*)()>(*(it + 1));
		}
	}
};

struct SButtonCheckedStaticParams
{
	void (*Delegate)(bool);
};

class PAbstractButton : public PWidget
{
protected:
	bool mChecked = false;
	bool mCheckable = false;
	bool mMouseDown = false;

public:
	DButtonClicked Clicked;
	DButtonChecked Checked;

	bool GetCheckable() const { return mCheckable; }
	void SetCheckable(bool State) { mCheckable = State; }
	bool GetChecked() const { return mChecked; }
	void SetChecked(bool State) { mChecked = State; }
	void ToggleChecked()
	{
		mChecked = !mChecked;
		LogInfo("Toggling {} == {}", mInternalName.c_str(), mChecked ? "true" : "false");
	}

	void ProcessEvents(SWidgetEvent* Event) override
	{
		mSender = this;

		// Are we inside the button?
		if (GetGeometry().Contains(Event->MousePosition))
		{
			// Is it a new press?
			if (Event->MouseDown && !mMouseDown)
			{
				mMouseDown = true;
			}
			// Is it a release?
			else if (!Event->MouseDown && mMouseDown)
			{
				mMouseDown = false;

				// Consume the event so that we don't pass the click down to widgets
				// underneath this button.
				Event->Consumed = true;

				Clicked.Broadcast();

				// If this button is checkable, flip the checked state
				if (mCheckable)
				{
					mChecked = !mChecked;
					Checked.Broadcast(mChecked);
				}
			}
		}
		// Are we outside the button?
		else
		{
			mMouseDown = false; // Reset button state
		}
	}
};