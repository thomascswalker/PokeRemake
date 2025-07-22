#pragma once

#include "Core/Logging.h"

#include <string>

#include "Core/Meta.h"

// Non-drawable and position-independent Object
class PObject
{
protected:
	std::string mInternalName;
	bool		mSerializable = true;

public:
	PObject() = default;
	virtual ~PObject() = default;

	// Called before the game begins
	virtual bool PreStart() { return true; }
	// Called when the game begins
	virtual void Start() {}
	// Called when the game ends
	virtual void End() {}
	// Called after the game ends
	virtual void PostEnd() {}
	virtual void Tick(float DeltaTime) {};

	std::string GetClassName() const;
	std::string GetInternalName() const { return mInternalName; }
	void		SetInternalName(const std::string& Name) { mInternalName = Name; }
	void		GenerateInternalName();

	virtual json Serialize() const { return json(); }
	virtual void Deserialize(const json& Data)
	{
		LogDebug("Deserializing {}", GetClassName().c_str());
	}
	bool IsSerializable() const { return mSerializable; }
};
