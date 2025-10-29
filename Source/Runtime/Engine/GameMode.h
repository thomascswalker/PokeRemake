#pragma once

#include "Dialog.h"
#include "GameEvent.h"
#include "Input.h"
#include "Object.h"
#include "Transition.h"

struct SGameState
{
private:
	JSON Schema;

public:
	JSON Data;

	SGameState(const JSON& InSchema = JSON())
		: Schema(InSchema)
	{
	}

	SGameState(const SGameState& Other)
		: Schema(Other.Schema), Data(Other.Data)
	{
	}

	const JSON& GetSchema() const { return Schema; }

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

	virtual bool HandlePressA() { return false; }
	virtual bool HandleReleaseA() { return false; }
	virtual bool HandlePressB() { return false; }
	virtual bool HandleReleaseB() { return false; }
	virtual bool HandlePressDPad(EDPad Direction) { return false; }
	virtual bool HandleReleaseDPad(EDPad Direction) { return false; }
	virtual bool HandlePressStart() { return false; }
	virtual bool HandlePressSelect() { return false; }

	virtual void StartDialog(const SDialogContext& Context) = 0;
};
