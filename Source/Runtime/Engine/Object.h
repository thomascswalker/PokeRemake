#pragma once

// Non-drawable and position-independent Object
class PObject
{
public:
	PObject() = default;
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
};
