#pragma once

#include "Widget.h"

class PSpacer : public PWidget
{
public:
	PSpacer()
	{
		Padding = { 0 };
		mResizeModeW = RM_Grow;
		mResizeModeH = RM_Grow;
	}
};
