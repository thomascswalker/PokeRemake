#pragma once

#include "Core/CoreFwd.h"

// Non-drawable and position-independent Object
class PObject
{
protected:
	std::string mInternalName;
	bool mSerializable = true;

public:
	PObject()          = default;
	virtual ~PObject() = default;

	// Called before the game begins
	virtual bool PreStart()
	{
		return true;
	}

	// Called when the game begins
	virtual void Start() {}
	// Called when the game ends
	virtual void End() {}
	// Called after the game ends
	virtual void PostEnd() {}
	virtual void Tick(float DeltaTime) {}
	virtual void PostTick() {}

	std::string GetClassName() const;

	std::string GetInternalName() const
	{
		return mInternalName;
	}

	void SetInternalName(const std::string& Name)
	{
		mInternalName = Name;
	}

	void GenerateInternalName();

	virtual JSON Serialize() const
	{
		JSON Result;
		Result["Class"] = GetClassName();
		SAVE_MEMBER_PROPERTY(InternalName);
		return Result;
	}

	virtual void Deserialize(const JSON& Data)
	{
		if (Data.contains("InternalName"))
		{
			mInternalName = Data["InternalName"].get<std::string>();
		}
		else
		{
			GenerateInternalName();
		}
		LogDebug("Deserializing {}: {}", GetClassName().c_str(), mInternalName.c_str());
	}

	bool IsSerializable() const
	{
		return mSerializable;
	}
};
