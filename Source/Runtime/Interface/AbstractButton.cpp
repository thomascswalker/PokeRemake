#include "AbstractButton.h"

#include "ButtonGroup.h"

void PAbstractButton::SetButtonGroup(PButtonGroup* Group)
{
	mButtonGroup = Group;
}

void PAbstractButton::OnMouseEvent(SInputEvent* Event)
{
	// Are we inside the button?
	mMouseOver = GetGeometry().Contains(Event->MousePosition);

	// Hover begin/end events
	if (!mMouseOver && GetGeometry().Contains(Event->MousePosition))
	{
		HoverBegin.Broadcast();
	}
	else if (mMouseOver && !GetGeometry().Contains(Event->MousePosition))
	{
		HoverEnd.Broadcast();
	}

	if (mMouseOver)
	{
		// Is it a new press?
		if (Event->Type == IET_MouseDown && Event->LeftMouseDown && !mMouseDown)
		{
			mMouseDown = true;
			Event->Consume();
			return;
		}
		// Is it a release?
		if (Event->Type == IET_MouseUp && !Event->LeftMouseDown && mMouseDown)
		{
			// Broadcast that we've clicked
			Clicked.Broadcast();
			// Reset the mouse state
			mMouseDown = false;

			// If this button is checkable, flip the checked state
			if (mCheckable)
			{
				mChecked = !mChecked;
				Checked.Broadcast(mChecked);

				// If this button is part of a group, send a signal to that group
				// that this button was checked.
				if (mButtonGroup)
				{
					mButtonGroup->OnButtonChecked(mChecked);
				}
			}

			// Consume the event so that we don't pass the click down to widgets
			// underneath this button.
			Event->Consume();
			return;
		}

		return;
	}

	// Are we outside the button?
	mMouseDown = false; // Reset button state
}
