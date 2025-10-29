#pragma once

#include <string>
#include <type_traits>

#include "Object.h"

enum class EGameEventType
{
	// Misc
	Default = 0,
	// Dialog boxes
	Dialog = 100,
	// Battle
	BattleStart = 200,
	BattleEnd,
	BattleChangeSelection,
	BattleEnterMove,
	BattleExitMove,
	BattleUseMove,
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
	PObject*	   Instigator = nullptr;
	EGameEventType Type = EGameEventType::Default;

	SGameEvent() = default;

	template <typename T = uint8_t>
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

	virtual ~SGameEvent()
	{
		if (mData)
		{
			std::free(mData);
		}
		mData = nullptr;
	}

	template <typename T>
	T GetData() const
	{
		return *static_cast<T*>(mData);
	}

	std::string ToString() const
	{
		return std::format("GameEvent: {} from {}", GameEventTypeToString(Type), Instigator->GetClassName().c_str());
	}

	virtual void Execute() const {}
};