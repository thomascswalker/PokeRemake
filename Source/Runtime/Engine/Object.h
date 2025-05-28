#pragma once

// Non-drawable and position-independent Object
class PObject
{
public:
	PObject() = default;
	virtual ~PObject() = default;
	virtual void Tick(float DeltaTime) = 0;
};
