#pragma once
#include "nlohmann/json.hpp"

#include <format>
#include <string>

using namespace nlohmann;

// Non-drawable and position-independent Object
class PObject
{
protected:
	std::string mInternalName;
	bool		bSerializable = true;

public:
	PObject() = default;
	PObject(const json& JsonData) {}
	virtual ~PObject() = default;

	// Called before the game begins
	virtual void PreStart() {}
	// Called when the game begins
	virtual void Start() {}
	// Called when the game ends
	virtual void End() {}
	// Called after the game ends
	virtual void PostEnd() {}
	virtual void Tick(float DeltaTime) = 0;

	std::string GetClassName() const { return typeid(*this).name(); }
	std::string GetInternalName() const { return mInternalName; }
	void		SetInternalName(const std::string& Name) { mInternalName = Name; }
	void		GenerateInternalName()
	{
		auto	   Name = typeid(*this).name();
		static int Count = 0;
		mInternalName = std::format("{}_{}", Name, Count++);
	}

	virtual json Serialize() const { return json(); }
	virtual void Deserialize(const json& Data) {}
	bool		 IsSerializable() const { return bSerializable; }
};
