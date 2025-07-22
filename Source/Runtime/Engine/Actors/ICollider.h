#pragma once

#include "Core/Delegate.h"

#include <vector>

class PActor;
DECLARE_MULTICAST_DELEGATE(DOverlapBegin, PActor*);
DECLARE_MULTICAST_DELEGATE(DOverlapEnd, PActor*);

class ICollider
{
protected:
	std::vector<PActor*> mOverlappingActors;

public:
	DOverlapBegin OverlapBegin;
	DOverlapEnd	  OverlapEnd;
};