#pragma once

#include <vector>

#include "Core/Delegate.h"

class PActor;
class PCollisionComponent;

DECLARE_MULTICAST_DELEGATE(DOverlapBegin, PActor*);
DECLARE_MULTICAST_DELEGATE(DOverlapEnd, PActor*);

class ICollider
{
protected:
	std::vector<PActor*> mOverlappingActors;
	bool				 mCollideable = true;

public:
	DOverlapBegin OverlapBegin;
	DOverlapEnd	  OverlapEnd;

	virtual ~ICollider() = default;
	virtual void ProcessCollision(PCollisionComponent* Other) = 0;
	void		 SetCollideable(bool State) { mCollideable = State; }
	bool		 GetCollideable() { return mCollideable; }
};