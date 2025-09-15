#pragma once

#include "Core/Delegate.h"
#include "Core/Map.h"

#include "Object.h"

struct STimer;
struct STimerHandle;
class PTimerManager;

DECLARE_MULTICAST_DELEGATE(DTimerTriggered);

static size_t gNextId = 0;

struct STimer
{
	DTimerTriggered Trigger;
	DelegateHandle	Callback;
	STimerHandle*	Handle;

	size_t DeltaTime = 0;
	float  Rate;

	template <typename T>
	STimer(STimerHandle* InHandle, float InRate, T* Object, void (T::*Delegate)())
	{
		Handle = InHandle;
		Rate = InRate;
		Callback = Trigger.AddRaw(Object, Delegate);
	}

	bool IsValid() const
	{
		return Trigger.IsBound() && Callback.IsValid();
	}

	void Invalidate()
	{
		Trigger.RemoveAll();
		Callback.Reset();
	}
};

struct STimerHandle
{
private:
	size_t Id;

public:
	STimerHandle()
	{
		Id = ++gNextId;
	}

	friend class PTimerManager;
};

class PTimerManager : public PObject
{
	TMap<size_t, STimer> Timers;

	void DestroyTimerInternal(STimer* Timer)
	{
		Timer->Invalidate();
		Timers.Remove(Timer->Handle->Id);
	}

public:
	template <typename T>
	void SetTimer(STimerHandle& Handle, float Rate, T* Object, void (T::*Delegate)())
	{
		Timers[Handle.Id] = STimer(Handle, Rate, Object, Delegate);
	}

	void DestroyTimer(const STimerHandle& Handle)
	{
		auto Iter = Timers.Find(Handle.Id);
		if (Iter != Timers.end())
		{
			DestroyTimerInternal(&Iter->second);
		}
	}

	void Tick(float DeltaTime) override
	{
		for (const auto& K : Timers | std::views::keys)
		{
			auto Timer = &Timers[K];

			// Destroy any invalid timers
			if (!Timer->IsValid())
			{
				DestroyTimerInternal(Timer);
			}

			// Increment the time for this timer
			Timer->DeltaTime += DeltaTime;

			// If it has passed the rate, trigger the callback, then reset the delta time.
			if (Timer->DeltaTime >= Timer->Rate)
			{
				Timer->Trigger.Broadcast();
				Timer->DeltaTime = 0;
			}
		}
	}
};