#pragma once

#include "Core/Delegate.h"

#include "Widget.h"

DECLARE_MULTICAST_DELEGATE(DButtonClicked);
DECLARE_MULTICAST_DELEGATE(DButtonChecked, bool);

class PButtonGroup;

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

	PButtonGroup* mButtonGroup = nullptr;
	void		  OnMouseEvent(SInputEvent* Event) override;

public:
	DButtonClicked Clicked;
	DButtonChecked Checked;

	PAbstractButton()
	{
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Fixed;
		mFixedSize.Y = DEFAULT_WIDGET_HEIGHT;
	}

	bool GetCheckable() const { return mCheckable; }
	void SetCheckable(bool State) { mCheckable = State; }
	bool GetChecked() const { return mChecked; }
	void SetChecked(bool State) { mChecked = State; }
	void ToggleChecked() { mChecked = !mChecked; }

	void SetButtonGroup(PButtonGroup* Group);
};