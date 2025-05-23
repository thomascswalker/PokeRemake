#pragma once

class Engine
{
	bool bIsRunning = false;

public:
	Engine();
	void Stop();
	void Tick(float DeltaTime);
	bool IsRunning() const { return bIsRunning; }
};
