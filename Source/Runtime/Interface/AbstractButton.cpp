#include "AbstractButton.h"

#include "ButtonGroup.h"

void PAbstractButton::SetButtonGroup(PButtonGroup* Group)
{
	mButtonGroup = Group;
}

// Override the base Widget::OnMouseEvent in order to handle button
// clicks.
void PAbstractButton::OnMouseEvent(SInputEvent* Event)
{
	switch (Event->Type)
	{
		case IET_MouseMove:
			// Hover begin/end events
			if (!mMouseOver && GetGeometry().Contains(Event->MousePosition))
			{
				HoverBegin.Broadcast();
			}
			else if (mMouseOver && !GetGeometry().Contains(Event->MousePosition))
			{
				HoverEnd.Broadcast();
			}
			// Are we inside the button?
			mMouseOver = GetGeometry().Contains(Event->MousePosition);
			return;
		case IET_MouseUp:
		case IET_MouseDown:
			if (!mMouseOver)
			{
				break;
			}
			switch (Event->Type)
			{
				case IET_MouseUp:
					if (!Event->LeftMouseDown && mMouseDown)
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
					break;
				case IET_MouseDown:
					if (Event->LeftMouseDown && !mMouseDown)
					{
						mMouseDown = true;
						Event->Consume();
						return;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	mMouseDown = false;
}
