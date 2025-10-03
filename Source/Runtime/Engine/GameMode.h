#pragma once

#include "GameEvent.h"
#include "Input.h"
#include "Object.h"
#include "Transition.h"

struct SGameState
{
	JSON Data;

	JSON GetRaw(const std::string& Key)
	{
		return Data[Key];
	}

	template <typename T>
	T Get(const std::string& Key)
	{
		return Data[Key].get<T>();
	}

	template <typename T>
	void Set(const std::string& Key, const T& Value)
	{
		Data[Key] = Value;
	}

	JSON operator[](const std::string& Key)
	{
		return Data[Key];
	}
};

class PGameMode : public PObject, public IInputHandler
{
protected:
	PTransitionManager mTransitionManager;
	SGameState		   mState;
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

	SGameState* GetState() { return &mState; }

	virtual bool HandleGameEvent(const SGameEvent& GameEvent) = 0;
};
