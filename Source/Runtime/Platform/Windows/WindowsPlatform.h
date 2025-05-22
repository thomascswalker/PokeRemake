#pragma once

#include "Platform/Generic/Platform.h"

class WindowsPlatform : public IPlatform
{
	bool bIsRunning = false;

public:
	void Start() override;
	void Stop() override;
	bool IsRunning() override;
};
