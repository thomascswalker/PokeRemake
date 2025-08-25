#pragma once

#include "Core/CoreFwd.h"
#include "SDL3/SDL.h"

#include "InputContext.h"

#include "Core/Containers.h"

DECLARE_MULTICAST_DELEGATE(DHoverBegin);
DECLARE_MULTICAST_DELEGATE(DHoverEnd);

enum EInputEventType
{
	IET_None,
	IET_MouseMove,
	IET_MouseUp,
	IET_MouseDown,
	IET_MouseScroll,
	IET_KeyDown,
	IET_KeyUp,
};

struct SInputEvent
{
	// SDL
	SDL_Event* Event = nullptr;

	EInputEventType Type = IET_None;
	bool Consumed        = false;
	bool Valid           = true;

	FVector2 MousePosition = FVector2();
	bool LeftMouseDown     = false;
	bool MiddleMouseDown   = false;
	bool RightMouseDown    = false;
	float MouseScroll      = 0.0f;
	int KeyDown            = 0;
	int KeyUp              = 0;

	// Convert from an SDL_Event to a native SInputEvent
	SInputEvent(SDL_Event* SDLEvent)
	{
		Event = SDLEvent;
		switch (Event->type)
		{
		case SDL_EVENT_MOUSE_MOTION:
			{
				Type            = IET_MouseMove;
				MousePosition.X = Event->motion.x;
				MousePosition.Y = Event->motion.y;
				break;
			}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				Type            = IET_MouseDown;
				MousePosition.X = Event->button.x;
				MousePosition.Y = Event->button.y;
				switch (Event->button.button)
				{
				case SDL_BUTTON_LEFT:
					{
						LeftMouseDown = true;
						break;
					}
				case SDL_BUTTON_MIDDLE:
					{
						MiddleMouseDown = true;
						break;
					}
				case SDL_BUTTON_RIGHT:
					{
						RightMouseDown = true;
						break;
					}
				default: break;
				}
				break;
			}
		case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				Type            = IET_MouseUp;
				MousePosition.X = Event->button.x;
				MousePosition.Y = Event->button.y;
				switch (Event->button.button)
				{
				case SDL_BUTTON_LEFT:
					{
						LeftMouseDown = false;
						break;
					}
				case SDL_BUTTON_MIDDLE:
					{
						MiddleMouseDown = false;
						break;
					}
				case SDL_BUTTON_RIGHT:
					{
						RightMouseDown = false;
						break;
					}
				default: break;
				}
				break;
			}
		case SDL_EVENT_MOUSE_WHEEL:
			{
				Type            = IET_MouseScroll;
				MousePosition.X = Event->wheel.mouse_x;
				MousePosition.Y = Event->wheel.mouse_y;
				MouseScroll     = Event->wheel.y;
				break;
			}
		case SDL_EVENT_KEY_DOWN:
			{
				Type    = IET_KeyDown;
				KeyDown = Event->key.key;
				break;
			}
		case SDL_EVENT_KEY_UP:
			{
				Type  = IET_KeyUp;
				KeyUp = Event->key.key;
				break;
			}
		default: break;
		}
	}

	void Consume()
	{
		Consumed = true;
	}

	bool Validate(const SInputContext* Context)
	{
		if (!Valid)
		{
			return false;
		}

		if (Context->Any)
		{
			return true;
		}

		switch (Event->type)
		{
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
			{
				if (!Containers::Contains(Context->Keyboard, static_cast<int>(Event->key.key)))
				{
					return false;
				}
				break;
			}
		default:
			{
				break;
			}
		}
		return true;
	}

	void Invalidate()
	{
		Consume();
		Valid = false;
	}
};

class IInputHandler
{
public:
	virtual ~IInputHandler() = default;

	virtual bool ProcessEvents(SInputEvent* Event)
	{
		switch (Event->Type)
		{
		case IET_MouseMove:
		case IET_MouseDown:
		case IET_MouseUp:
		case IET_MouseScroll:
			{
				OnMouseEvent(Event);
				break;
			}
		case IET_KeyDown:
			{
				OnKeyDown(Event);
				break;
			}
		case IET_KeyUp:
			{
				OnKeyUp(Event);
				break;
			}
		default: break;
		}
		return Event->Consumed;
	}

protected:
	virtual void OnMouseEvent(SInputEvent* Event) {}
	virtual void OnKeyDown(SInputEvent* Event) {}
	virtual void OnKeyUp(SInputEvent* Event) {}
};
