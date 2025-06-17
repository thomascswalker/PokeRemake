#include "Widget.h"

#include "Layout.h"

PWidget* PWidget::mSender = nullptr;

void PWidget::ProcessEvents(SWidgetEvent* Event)
{
	mSender = this;
	for (const auto& Child : mChildren)
	{
		if (Event->bConsumed)
		{
			return;
		}
		Child->ProcessEvents(Event);
	}
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

void PWidget::LayoutChildren()
{
	switch (mResizeMode)
	{
		case RM_FixedXY:
			switch (mLayoutMode)
			{
				case LM_Horizontal:
					LayoutHorizontal(mChildren, { X, Y }, WIDGET_SPACING);
					break;
				case LM_Vertical:
					LayoutVertical(mChildren, { X, Y }, WIDGET_SPACING);
					break;
			}
		case RM_ExpandXY:
			LayoutResizeXY(mChildren, GetGeometry(), WIDGET_SPACING);
			break;
		case RM_ExpandX:
			LayoutResizeX(mChildren, GetGeometry(), WIDGET_SPACING);
			break;
		case RM_ExpandY:
			LayoutResizeY(mChildren, GetGeometry(), WIDGET_SPACING);
			break;
	}
}