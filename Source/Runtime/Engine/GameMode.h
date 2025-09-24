#pragma once

#include "Input.h"
#include "Object.h"
#include "Transition.h"

class PGameMode : public PObject, public IInputHandler
{
protected:
	bool mLoaded = false;
	JSON mSaveState;

	PTransitionManager mTransitionManager;

public:
	virtual std::string GetName()
	{
		return "DefaultGameMode";
	}

	virtual bool Load() = 0;
	void		 PostLoad();
	virtual bool Unload() = 0;
	void		 PostUnload();
	virtual void LoadDeferred() {}
	virtual void UnloadDeferred() {}

	bool GetLoaded() const
	{
		return mLoaded;
	}

	void SetLoaded(bool State)
	{
		mLoaded = State;
	}
};
