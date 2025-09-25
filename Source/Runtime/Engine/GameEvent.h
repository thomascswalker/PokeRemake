#pragma once

#include <string>
#include <type_traits>

enum class EGameEventType
{
	// Misc
	Default = 0,
	// Dialog boxes
	Dialog = 100,
	// Battle
	BattleStart = 200,
	BattleEnd,
};

static const char* GameEventTypeToString(EGameEventType Type)
{
	switch (Type)
	{
		case EGameEventType::Dialog:
			return "Dialog";
		default:
			return "Unknown";
	}
}

struct SGameEvent
{
private:
	void* mData = nullptr;

public:
	PObject*	   Instigator;
	EGameEventType Type;

	template <typename T = void>
	SGameEvent(PObject* InInstigator, EGameEventType InType, T* InData = nullptr)
	{
		Instigator = InInstigator;
		Type = InType;
		if (InData && !std::is_void_v<T>)
		{
			mData = std::malloc(sizeof(T));
			if (mData)
			{
				std::memcpy(mData, InData, sizeof(T));
			}
		}
	}

	~SGameEvent()
	{
		if (mData)
		{
			std::free(mData);
		}
		mData = nullptr;
	}

	template <typename T>
	T* GetData()
	{
		return static_cast<T*>(mData);
	}

	std::string ToString() const
	{
		return std::format("GameEvent: {} from {}", GameEventTypeToString(Type), Instigator->GetClassName().c_str());
	}
};