#pragma once

#include "Core/Delegate.h"
#include "Core/Map.h"

#include "Object.h"

struct STimer;
struct STimerHandle;
class PTimerManager;

DECLARE_MULTICAST_DELEGATE(DTimerTriggered);

struct STimer
{
	// DTimerTriggered Trigger;
	// DelegateHandle	Callback;

	size_t DeltaTime = 0;
	float  Rate = 1.0f;
	bool   Loop = false;

	STimer() = default;
	//
	// template <typename T>
	// DelegateHandle* Bind(float InRate, bool InLoop, T* Object, void (T::*Delegate)())
	// {
	// 	Rate = InRate;
	// 	Loop = InLoop;
	// 	Callback = Trigger.AddRaw(Object, Delegate);
	// 	return &Callback;
	// }

	void Reset()
	{
		DeltaTime = 0;
	}
};

//
// static size_t gNextId = 0;
//
// struct STimer
// {
// 	DTimerTriggered Trigger;
// 	DelegateHandle	Callback;
// 	STimerHandle*	Handle;
//
// 	size_t DeltaTime = 0;
// 	float  Rate;
// 	bool   Loop = false;
//
// 	STimer() = default;
//
// 	STimer(STimerHandle* InHandle, float InRate, bool InLoop)
// 	{
// 		Handle = InHandle;
// 		Rate = InRate;
// 		Loop = InLoop;
// 	}
//
// 	template <typename T>
// 	DelegateHandle* Bind(T* Object, void (T::*Delegate)())
// 	{
// 		Callback = Trigger.AddRaw(Object, Delegate);
// 		return &Callback;
// 	}
//
// 	bool IsValid() const
// 	{
// 		return Trigger.IsBound() && Callback.IsValid();
// 	}
//
// 	void Invalidate()
// 	{
// 		Trigger.RemoveAll();
// 		Callback.Reset();
// 		Handle = nullptr;
// 	}
// };
//
// struct STimerHandle
// {
// private:
// 	size_t Id = 0;
//
// public:
// 	STimerHandle()
// 	{
// 		Id = ++gNextId;
// 	}
//
// 	friend class PTimerManager;
// };
//
// class PTimerManager : public PObject
// {
// 	TMap<size_t, STimer> Timers;
// 	TArray<size_t>		 DestroyedTimers;
//
// 	void DestroyTimerInternal(STimer* Timer)
// 	{
// 		if (Timer->Handle)
// 		{
// 			size_t Id = Timer->Handle->Id;
// 			Timer->Invalidate();
// 			Timers.Remove(Id);
// 		}
// 	}
//
// public:
// 	template <typename T>
// 	void SetTimer(STimerHandle& Handle, float Rate, bool Loop, T* Object, void (T::*Delegate)())
// 	{
// 		Timers[Handle.Id] = STimer(&Handle, Rate, Loop);
// 		Timers[Handle.Id].Bind(Object, Delegate);
// 	}
//
// 	void DestroyTimer(const STimerHandle& Handle)
// 	{
// 		auto Iter = Timers.Find(Handle.Id);
// 		if (Iter != Timers.end())
// 		{
// 			DestroyedTimers.Add(Handle.Id);
// 		}
// 	}
//
// 	void Tick(float DeltaTime) override
// 	{
// 		// Destroy timers
// 		if (DestroyedTimers.Size() > 0)
// 		{
// 			for (auto Id : DestroyedTimers)
// 			{
// 				STimer* Timer = &Timers[Id];
// 				DestroyTimerInternal(Timer);
// 			}
// 			DestroyedTimers.Clear();
// 		}
//
// 		for (auto Iter = Timers.begin(); Iter != Timers.end(); ++Iter)
// 		{
// 			auto Timer = &Iter->second;
// 			if (!Timer->IsValid())
// 			{
// 				LogError("Timer is invalid: {}", Timer->Handle->Id);
// 				DestroyTimer(*Timer->Handle);
// 				continue;
// 			}
//
// 			// Increment the time for this timer
// 			Timer->DeltaTime += DeltaTime;
//
// 			if (Timer->DeltaTime < Timer->Rate)
// 			{
// 				continue;
// 			}
//
// 			// If it has passed the rate, trigger the callback, then reset the delta time.
// 			Timer->Trigger.Broadcast();
// 			Timer->DeltaTime = 0;
// 		}
// 	}
// };