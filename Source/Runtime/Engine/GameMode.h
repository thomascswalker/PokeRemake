#pragma once

#include "Input.h"
#include "Object.h"

class PGameMode : public PObject, public IInputHandler
{
protected:
	bool mLoaded = false;
	JSON mSaveState;

public:
	virtual std::string GetName()
	{
		return "DefaultGameMode";
	}

	virtual bool Load() = 0;
	virtual bool Unload() = 0;
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
