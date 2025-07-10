#include "Widget.h"

#include "Layout.h"

PWidget* PWidget::mSender = nullptr;

bool PWidget::ProcessEvents(SInputEvent* Event)
{
	// Process all child events first so the parent
	// doesn't consume events meant for children
	for (const auto& Child : mChildren)
	{
		if (Child->ProcessEvents(Event))
		{
			return true;
		}
	}

	mSender = this;
	if (IInputHandler::ProcessEvents(Event))
	{
		return true;
	}
	return false;
}

void PWidget::SetParent(PWidget* Parent)
{
	if (mParent)
	{
		mParent->RemoveChild(this);
	}
	mParent = Parent;
}

void PWidget::AddChild(PWidget* Child)
{
	mChildren.push_back(Child);
	Child->SetParent(this);
}

void PWidget::RemoveChild(PWidget* Child)
{
	const auto it = std::ranges::remove(mChildren, Child).begin();
	if (it != mChildren.end())
	{
		mChildren.erase(it);
		Child->SetParent(nullptr);
	}
}
