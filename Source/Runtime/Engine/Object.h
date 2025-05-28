#pragma once

// Non-drawable and position-independent Object
class PObject
{
public:
	PObject() = default;
	virtual ~PObject() = default;
	virtual void Start() {}
	virtual void End() {}
	virtual void Tick(float DeltaTime) = 0;
};
