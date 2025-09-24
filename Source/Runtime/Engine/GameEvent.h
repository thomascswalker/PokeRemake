#pragma once

#include <string>

struct SGameEvent
{
private:
	void* mData;

public:
	PObject*	Instigator;
	std::string Name;

	template <typename T>
	SGameEvent(PObject* InInstigator, const std::string& InName, T* InData)
	{
		Instigator = InInstigator;
		Name = InName;
		mData = std::malloc(sizeof(T));
		std::memcpy(mData, InData, sizeof(T));
	}

	~SGameEvent()
	{
		std::free(mData);
		mData = nullptr;
	}

	template <typename T>
	T* GetData()
	{
		return static_cast<T*>(mData);
	}

	std::string ToString() const
	{
		return std::format("GameEvent: {} from {}", Name.c_str(), Instigator->GetClassName().c_str());
	}
};