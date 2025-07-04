#include "AbstractButton.h"
#include "ButtonGroup.h"

void PAbstractButton::SetButtonGroup(PButtonGroup* Group)
{
	mButtonGroup = Group;
}

void PAbstractButton::ProcessEvents(SWidgetEvent* Event)
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

				// If this button is part of a group, send a signal to that group
				// that this button was checked.
				if (mButtonGroup)
				{
					mButtonGroup->OnButtonChecked(mChecked);
				}
			}
		}
	}
	// Are we outside the button?
	else
	{
		mMouseDown = false; // Reset button state
	}
}