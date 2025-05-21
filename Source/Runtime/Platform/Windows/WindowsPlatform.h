#pragma once

#include "Platform/Generic/Platform.h"

class WindowsPlatform : public IPlatform
{
public:
	void Start() override;
	void Stop() override;
};
