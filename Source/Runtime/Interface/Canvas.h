#pragma once
#include "Layout.h"
#include "Widget.h"

class PCanvas : public PWidget
{
public:
	PCanvas()
	{
		mResizeModeW = RM_Fixed;
		mResizeModeH = RM_Fixed;
	}
};