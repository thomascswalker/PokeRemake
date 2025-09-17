#pragma once

#include <functional>

#include "Core/Delegate.h"
#include "Core/Map.h"

struct STimer;
struct STimerHandle;
class PTimerManager;

DECLARE_MULTICAST_DELEGATE(DTimerDelegate);
using DTimerCallback = std::function<void()>;
using FTimerDelegate = std::function<void()>;

struct STimerHandle
{
private:
	size_t Index;

public:
	STimerHandle() : Index(0) {}
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

	STimerDelegate() {}
	STimerDelegate(const DTimerDelegate& D) : ObjectCallback(D), StaticCallback(nullptr) {}
	STimerDelegate(const DTimerCallback& D) : StaticCallback(D) {}

	void Execute()
	{
		if (ObjectCallback.IsBound())
		{
			ObjectCallback.Broadcast();
		}
		else
		{
			StaticCallback();
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
	float		   Rate;
	STimerHandle   Handle;
	STimerDelegate Delegate;
};

class PTimerManager
{
	static size_t		 sNextHandle;
	TMap<size_t, STimer> mTimers;

	STimerHandle AddTimer(STimer&& Timer)
	{
		STimerHandle Handle;
		auto		 NewIndex = sNextHandle++;
		mTimers.Emplace(sNextHandle++, std::move(Timer));
		Handle.SetIndex(NewIndex);
		return Handle;
	}

	void RemoveTimer(const STimerHandle& Handle)
	{
		mTimers.Remove(Handle.Index);
	}

	STimer* FindTimer(const STimerHandle& Handle)
	{
		return mTimers.At(Handle.Index);
	}

	STimer& GetTimer(const STimerHandle& Handle)
	{
		return mTimers[Handle.Index];
	}

	void SetTimerInternal(STimerHandle& Handle, STimerDelegate&& InDelegate, float Rate, float Loop)
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

		STimer NewTimer;
		NewTimer.Delegate = std::move(InDelegate);
		NewTimer.Rate = Rate;
		NewTimer.Loop = Loop;

		STimerHandle NewHandle = AddTimer(std::move(NewTimer));
		Handle = NewHandle;
	}

public:
	template <typename T>
	void SetTimer(STimerHandle& Handle, T* Object, void (T::*Delegate)(), float Rate, float Loop)
	{
		STimerDelegate NewDelegate;
		NewDelegate.ObjectCallback.AddRaw(Object, Delegate);
		SetTimerInternal(Handle, std::move(NewDelegate), Rate, Loop);
	}

	void SetTimer(STimerHandle& Handle, const DTimerCallback& Callback, float Rate, float Loop)
	{
		STimerDelegate NewDelegate;
		NewDelegate.StaticCallback = Callback;
		SetTimerInternal(Handle, std::move(NewDelegate), Rate, Loop);
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
};
