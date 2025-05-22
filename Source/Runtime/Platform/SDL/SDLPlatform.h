//
// Created by thoma on 5/22/2025.
//

#pragma once

#include "Platform/Generic/IPlatform.h"

class SDLPlatform : public IPlatform
{
	bool bIsRunning = false;

public:
	bool OnStart(int argc, char** argv) override;
	void OnStop() override;
	bool OnLoop() override;
	void OnEvent(void* Event) override;
	void OnDraw() override;

	bool		 IsRunning() override { return bIsRunning; }
	PlatformType GetPlatformType() override { return PlatformType::SDL; }
};
