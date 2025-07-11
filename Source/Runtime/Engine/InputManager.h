#pragma once

#include <stdint.h>

#include "SDL3/SDL.h"

#include "Core/Delegate.h"
#include "Core/Vector.h"
#include "InputContext.h"

#include <map>

DECLARE_MULTICAST_DELEGATE(DKeyDown, uint32_t);
DECLARE_MULTICAST_DELEGATE(DKeyUp, uint32_t);
DECLARE_MULTICAST_DELEGATE(DMiddleMouseScroll, float);
DECLARE_MULTICAST_DELEGATE(DMouseMotion, float, float);
DECLARE_MULTICAST_DELEGATE(DMouseLeftClick);
DECLARE_MULTICAST_DELEGATE(DMouseRightClick);
DECLARE_MULTICAST_DELEGATE(DMouseMiddleClick);

class IInputManager
{
protected:
	std::map<std::string, SInputContext> mInputContexts;

public:
	// DKeyDown		   KeyDown;
	// DKeyUp			   KeyUp;
	// DMiddleMouseScroll MouseScroll;
	// DMouseMotion	   MouseMotion;
	// DMouseLeftClick	   MouseLeftClick;
	// DMouseRightClick   MouseRightClick;
	// DMouseMiddleClick  MouseMiddleClick;

	virtual ~IInputManager() = default;
	virtual bool IsKeyDown(uint32_t KeyCode) const = 0;
	virtual bool IsKeyUp(uint32_t KeyCode) const = 0;
	bool		 IsShiftDown() const { return IsKeyDown(SDLK_LSHIFT) || IsKeyDown(SDLK_RSHIFT); }
	bool		 IsCtrlDown() const { return IsKeyDown(SDLK_LCTRL) || IsKeyDown(SDLK_RCTRL); }
	bool		 IsAltDown() const { return IsKeyDown(SDLK_LALT) || IsKeyDown(SDLK_RALT); }

	void AddInputContext(const std::string& Name, const SInputContext& Context)
	{
		mInputContexts.emplace(Name, Context);
	}
	SInputContext* GetInputContext(const std::string& Name)
	{
		auto it = mInputContexts.find(Name);
		if (it == mInputContexts.end())
		{
			return nullptr;
		}
		return &it->second;
	}
};

IInputManager* GetInputManager();
void		   SetInputManager(IInputManager* InputManager);

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

	SInputContext*	Context = nullptr;
	EInputEventType Type = IET_None;
	bool			Consumed = false;

	FVector2 MousePosition = FVector2();
	bool	 LeftMouseDown = false;
	bool	 MiddleMouseDown = false;
	bool	 RightMouseDown = false;
	float	 MouseScroll = 0.0f;
	int		 KeyDown = 0;
	int		 KeyUp = 0;

	// Convert from an SDL_Event to a native SInputEvent
	SInputEvent(SDL_Event* SDLEvent)
	{
		Event = SDLEvent;
		switch (Event->type)
		{
			case SDL_EVENT_MOUSE_MOTION:
				{
					Type = IET_MouseMove;
					MousePosition.X = Event->motion.x;
					MousePosition.Y = Event->motion.y;
					break;
				}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					Type = IET_MouseDown;
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
						default:
							break;
					}
					break;
				}
			case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					Type = IET_MouseUp;
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
						default:
							break;
					}
					break;
				}
			case SDL_EVENT_MOUSE_WHEEL:
				{
					Type = IET_MouseScroll;
					MousePosition.X = Event->wheel.mouse_x;
					MousePosition.Y = Event->wheel.mouse_y;
					MouseScroll = Event->wheel.y;
					break;
				}
			case SDL_EVENT_KEY_DOWN:
				{
					Type = IET_KeyDown;
					KeyDown = Event->key.key;
					break;
				}
			case SDL_EVENT_KEY_UP:
				{
					Type = IET_KeyUp;
					KeyUp = Event->key.key;
					break;
				}
			default:
				break;
		}
	}

	void Consume() { Consumed = true; }
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
			default:
				break;
		}
		return Event->Consumed;
	}

protected:
	virtual void OnMouseEvent(SInputEvent* Event) {}
	virtual void OnKeyDown(SInputEvent* Event) {}
	virtual void OnKeyUp(SInputEvent* Event) {}
};
