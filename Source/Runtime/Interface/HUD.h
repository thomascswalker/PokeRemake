#pragma once

#include "Widget.h"

class PHUD : public PWidget
{
public:
	PHUD()
	{
		mResizeModeW = RM_Fixed;
		mResizeModeH = RM_Fixed;
	}
};
