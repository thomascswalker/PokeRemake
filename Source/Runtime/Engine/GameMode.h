#pragma once

#include "GameEvent.h"
#include "Input.h"
#include "Object.h"
#include "Transition.h"

class PGameMode : public PObject, public IInputHandler
{
protected:
	PTransitionManager mTransitionManager;
	JSON			   mState;
	bool			   mLoaded = false;

public:
	virtual std::string GetName()
	{
		return "DefaultGameMode";
	}

	virtual bool Load() = 0;
	virtual bool Unload() = 0;

	bool IsLoaded() const { return mLoaded; }
	void SetLoaded(bool State) { mLoaded = State; }
	void ToggleLoaded() { mLoaded = !mLoaded; }

	JSON* GetState() { return &mState; }

	virtual bool HandleGameEvent(SGameEvent& GameEvent) = 0;
};
