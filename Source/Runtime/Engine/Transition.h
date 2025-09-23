#pragma once
#include <string>

#include "Core/Delegate.h"

#include "Timer.h"
#include "World.h"

struct STransition;

DECLARE_MULTICAST_DELEGATE(DTransitionStarted, STransition*);
DECLARE_MULTICAST_DELEGATE(DTransitionEnded);

struct STransition
{
	DTransitionStarted TransitionStarted;
	DTransitionEnded   TransitionEnded;

	std::string Name;
	float		Duration;
	bool		Loop;

	virtual ~STransition() = default;
	virtual void Start() { TransitionStarted.Broadcast(this); }
	virtual void End() { TransitionEnded.Broadcast(); }
};

class PTransitionManager
{
	std::shared_ptr<STransition> mCurrentTransition = nullptr;

public:
	PTransitionManager() = default;

	STransition* GetCurrentTransition()
	{
		return mCurrentTransition.get();
	}

	template <typename T>
	T* StartTransition()
	{
		mCurrentTransition = std::make_shared<T>();
		mCurrentTransition->TransitionEnded.AddRaw(this, &PTransitionManager::EndTransition);
		mCurrentTransition->Start();
		return dynamic_cast<T*>(mCurrentTransition.get());
	}

	void EndTransition()
	{
		mCurrentTransition->TransitionStarted.RemoveAll();
		mCurrentTransition->TransitionEnded.RemoveAll();
		mCurrentTransition = nullptr;
	}
};