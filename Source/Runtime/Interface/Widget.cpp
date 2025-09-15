#include "Widget.h"

#include "Layout.h"

PWidget*						 PWidget::sSender = nullptr;
std::shared_ptr<CSS::Stylesheet> PWidget::sStylesheet = nullptr;
TArray<PWidget*>				 PWidget::sFloating = {};
TArray<PWidget*>				 PWidget::sVisible = {};
TArray<PWidget*>				 PWidget::sFocused = {};

PWidget::PWidget()
{
	GenerateInternalName();
	SetVisible(true);
}

bool PWidget::ProcessEvents(SInputEvent* Event)
{
	sSender = this;
	if (IInputHandler::ProcessEvents(Event))
	{
		return true;
	}
	// Process all child events first so the parent
	// doesn't consume events meant for children
	for (const auto Child : mChildren)
	{
		if (Child->ProcessEvents(Event))
		{
			// Event is consumed
			return true;
		}
	}

	return Event->Consumed;
}

void PWidget::SetParent(PWidget* Parent)
{
	if (mParent)
	{
		mParent->RemoveChild(this);
	}
	mParent = Parent;
	if (Parent != nullptr)
	{
		mParent->mChildren.push_back(this);
	}
}

void PWidget::AddChild(PWidget* Child)
{
	mChildren.push_back(Child);
	Child->mParent = this;
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

void PWidget::RemoveAllChildren()
{
	for (auto Child : mChildren)
	{
		RemoveChild(Child);
	}
}
