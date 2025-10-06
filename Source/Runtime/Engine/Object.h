#pragma once

#include "Core/CoreFwd.h"

#include "Serialization.h"

// Non-drawable and position-independent Object
class PObject : public ISerializable
{
protected:
	std::string mInternalName;
	bool		mSerializable = true;
	bool		mPendingDestruction = false;

public:
	PObject() = default;
	~PObject() override = default;

	// Called before the game begins
	virtual bool PreStart()
	{
		return true;
	}

	// Called when the game begins
	virtual bool Start() { return true; }
	// Called when the game ends
	virtual bool End() { return true; }
	// Called after the game ends
	virtual bool PostEnd() { return true; }
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

	virtual std::string GetDisplayName() const
	{
		return mInternalName;
	}

	JSON Serialize() const override
	{
		JSON Result;
		Result["Class"] = GetClassName();
		SAVE_MEMBER_PROPERTY(InternalName);
		return Result;
	}

	void Deserialize(const JSON& Data) override
	{
		if (Data.contains("InternalName"))
		{
			mInternalName = Data["InternalName"].get<std::string>();
		}
		else
		{
			GenerateInternalName();
		}
	}

	bool IsSerializable() const
	{
		return mSerializable;
	}

	bool IsPendingDestruction() const
	{
		return mPendingDestruction;
	}

	void MarkPendingDestruction()
	{
		mPendingDestruction = true;
	}
};
