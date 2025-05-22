//
// Created by thoma on 5/22/2025.
//

#pragma once

enum class PlatformType
{
	Unknown,
	SDL,
	GBA
};

class IPlatform
{
public:
	virtual ~IPlatform() = default;

	virtual bool OnStart(int argc, char** argv) = 0;
	virtual void OnStop() = 0;
	virtual bool OnLoop() = 0;
	virtual void OnEvent(void* Event) = 0;
	virtual void OnDraw() = 0;

	virtual bool		 IsRunning() = 0;
	virtual PlatformType GetPlatformType() = 0;
};