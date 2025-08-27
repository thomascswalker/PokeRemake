#pragma once

#include "Core/Macros.h"
#include "Engine/Input.h"

#include "Widget.h"

class PScrollArea : public PWidget
{
	float mScrollValue = 0.0f;
	float mScrollSpeed = 20.0f;

public:
	PScrollArea()
	{
		mLayoutMode  = LM_Vertical;
		mResizeModeH = RM_Grow;
		mPadding      = {0};
	}

	void OnMouseEvent(SInputEvent* Event) override
	{
		if (Event->Type != IET_MouseScroll)
		{
			return;
		}
		if (GetGeometry().Contains(Event->MousePosition))
		{
			AddScrollValue(Event->MouseScroll);
			Event->Consume();
		}
	}

	/**
	 * The maximum scroll value is equal to the summed height and top padding of each child,
	 * plus the last child's bottom padding, minus the height of this widget.
	 *
	 *	- c = Children
	 *	- nc = Child count
	 *	- n = Nth Child
	 *
	 *	`[ c[n]->h + c[n]->pt ... nc ] + c[nc - 1]->pb - height`
	 */
	float GetMaximumScrollValue() const
	{
		float MaximumScrollValue = 0.0f;
		for (auto Child : mChildren)
		{
			MaximumScrollValue += Child->H;
			MaximumScrollValue += Child->mPadding.Top;
		}
		MaximumScrollValue += mChildren[mChildren.size() - 1]->mPadding.Bottom;
		return std::max(0.0f, MaximumScrollValue - H);
	}

	void AddScrollValue(float Value)
	{
		// Value comes in as +1 for a forwards scroll, -1 for a backwards scroll. Invert this
		// so a forwards scroll will scroll up, and a backwards scroll will scroll down.
		mScrollValue += -Value * mScrollSpeed;

		// Clamp the new scroll value between 0 and the maximum possible scroll value.
		mScrollValue = std::min(std::max(mScrollValue, 0.0f), GetMaximumScrollValue());
	}

	void OnLayout() override
	{
		// Update the Y offset for each child to be the inverse of the current scroll value.
		for (auto Child : mChildren)
		{
			Child->SetOffsetY(-mScrollValue, true);
		}
		PWidget::OnLayout();
	}

	void Draw(const PRenderer* Renderer) const override
	{
		Renderer->SetDrawColor(PColor::UIBorder);
		Renderer->DrawRect(GetGeometry());
	}

	bool DrawChildren(const PRenderer* Renderer) const override
	{
		Renderer->SetClipRect(GetGeometry());
		VALIDATE(PWidget::DrawChildren(Renderer), "Failed to draw ScrollArea children.");
		Renderer->ReleaseClipRect();

		return true;
	}
};
