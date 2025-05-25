#pragma once

// Non-drawable and position-independent Object
class PObject
{
public:
	virtual ~PObject() = default;
	virtual void Tick(float DeltaTime) {}
};
