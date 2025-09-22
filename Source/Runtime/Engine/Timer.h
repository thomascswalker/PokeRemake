/**
 * Shameless copy of Unreal Engine's FTimer/FTimerManager.
 */

#pragma once

#include <functional>
#include <ranges>

#include "Core/Delegate.h"
#include "Core/Logging.h"
#include "Core/Map.h"

struct STimer;
struct STimerHandle;
class PTimerManager;

DECLARE_MULTICAST_DELEGATE(DTimerDelegate);
using DTimerCallback = std::function<void()>;

struct STimerHandle
{
private:
	size_t Index;

public:
	STimerHandle() : Index(0) {}
	~STimerHandle()
	{
		Invalidate();
	}
	bool IsValid() const { return Index != 0; }
	void Invalidate() { Index = 0; }
	void SetIndex(size_t NewIndex) { Index = NewIndex; }

	bool operator==(const STimerHandle& Other) const { return Index == Other.Index; }
	bool operator!=(const STimerHandle& Other) const { return Index != Other.Index; }

	friend class PTimerManager;
};

struct STimerDelegate
{
	DTimerDelegate ObjectCallback;
	DTimerCallback StaticCallback;

	STimerDelegate() = default;

	template <typename T>
	STimerDelegate(T* Object, void (T::*Delegate)())
	{
		ObjectCallback.AddRaw(Object, Delegate);
	}

	STimerDelegate(const DTimerDelegate& D) : ObjectCallback(D), StaticCallback(nullptr) {}
	STimerDelegate(const DTimerCallback& D) : StaticCallback(D) {}

	void Execute()
	{
		if (ObjectCallback.IsBound() || ObjectCallback.GetSize() > 0)
		{
			ObjectCallback.Broadcast();
		}
		else if (StaticCallback != nullptr)
		{
			StaticCallback();
		}
		else
		{
			LogError("Delegate is not bound to any function.");
		}
	}

	bool IsBound() const { return ObjectCallback.IsBound() || StaticCallback != nullptr; }

	void Unbind()
	{
		ObjectCallback.RemoveAll();
		StaticCallback = nullptr;
	}
};

struct STimer
{
	bool		   Loop = true;
	float		   Rate = 0;
	float		   Elapsed = 0;
	STimerHandle   Handle;
	STimerDelegate Delegate;

	STimer() = default;
	STimer(bool					 InLoop,
		   float				 InRate,
		   float				 InInitialDelay,
		   const STimerHandle&	 InHandle,
		   const STimerDelegate& InDelegate)
		: Loop(InLoop), Rate(InRate), Elapsed(-InInitialDelay), Handle(InHandle), Delegate(InDelegate) {}

	bool IsValid() const { return Handle.IsValid() || Delegate.IsBound(); }
};

class PTimerManager
{
public:
	PTimerManager() = default;
	PTimerManager(const PTimerManager& other)
		: mTimers{ other.mTimers }
	{
	}
	PTimerManager(PTimerManager&& other) noexcept
		: mTimers{ std::move(other.mTimers) }
	{
	}
	~PTimerManager() = default;
	PTimerManager& operator=(const PTimerManager& other)
	{
		if (this == &other)
		{
			return *this;
		}
		mTimers = other.mTimers;
		return *this;
	}
	PTimerManager& operator=(PTimerManager&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		mTimers = std::move(other.mTimers);
		return *this;
	}

private:
	static size_t			 sNextHandle;
	std::map<size_t, STimer> mTimers;

	void RemoveTimer(const STimerHandle& Handle)
	{
		mTimers.erase(Handle.Index);
	}

	STimer* FindTimer(const STimerHandle& Handle)
	{
		if (!mTimers.contains(Handle.Index))
		{
			return nullptr;
		}
		return &mTimers.at(Handle.Index);
	}

	void SetTimerInternal(STimerHandle& Handle, STimerDelegate&& InDelegate, float Rate, float InitialDelay, float Loop)
	{
		if (FindTimer(Handle))
		{
			RemoveTimer(Handle);
		}

		if (Rate <= 0.0f)
		{
			Handle.Invalidate();
			return;
		}

		STimerHandle NewHandle;
		auto		 NewIndex = sNextHandle++;
		mTimers.emplace(NewIndex, STimer(Loop, Rate, InitialDelay, NewHandle, std::move(InDelegate)));
		NewHandle.SetIndex(NewIndex);
		Handle = NewHandle;
	}

public:
	void Tick(float DeltaTime)
	{
		if (mTimers.size() == 0)
		{
			return;
		}

		std::vector<STimerHandle> ClearTimerQueue;
		for (auto& Timer : mTimers | std::views::values)
		{
			Timer.Elapsed += DeltaTime;
			if (Timer.Elapsed >= Timer.Rate)
			{
				Timer.Delegate.Execute();
				Timer.Elapsed = 0;
				if (!Timer.Loop)
				{
					ClearTimerQueue.push_back(Timer.Handle);
				}
			}
		}

		for (auto Handle : ClearTimerQueue)
		{
			RemoveTimer(Handle);
		}
		ClearTimerQueue.clear();
	}

	template <typename T>
	void SetTimer(STimerHandle& Handle, T* Object, void (T::*Delegate)(), float Rate, float Loop = false, float InitialDelay = 0.0f)
	{
		SetTimerInternal(Handle, std::move(STimerDelegate(Object, Delegate)), Rate, InitialDelay, Loop);
	}

	void SetTimer(STimerHandle& Handle, const DTimerCallback& Callback, float Rate, float Loop = false, float InitialDelay = 0.0f)
	{
		SetTimerInternal(Handle, std::move(STimerDelegate(Callback)), Rate, InitialDelay, Loop);
	}

	STimer& GetTimer(const STimerHandle& Handle)
	{
		return mTimers[Handle.Index];
	}

	void ClearTimer(STimerHandle& Handle)
	{
		if (auto Timer = FindTimer(Handle))
		{
			Timer->Delegate.Unbind();
			RemoveTimer(Handle);
		}
		Handle.Invalidate();
	}

	void ClearAllTimers()
	{
		for (auto& Timer : mTimers | std::views::values)
		{
			ClearTimer(Timer.Handle);
		}
	}

	template <typename T>
	void Delay(STimerHandle& Handle, float Time, T* Object, void (T::*Delegate)())
	{
		SetTimerInternal(Handle, std::move(STimerDelegate(Object, Delegate)), 0.01f, Time, false);
	}

	void Delay(STimerHandle& Handle, float Time, const DTimerCallback& Callback)
	{
		SetTimerInternal(Handle, std::move(STimerDelegate(Callback)), 0.01f, Time, false);
	}
};

// Defined in World.h
static PTimerManager* GetTimerManager();