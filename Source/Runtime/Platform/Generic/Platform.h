#pragma once

class IPlatform
{
public:
	virtual ~IPlatform() = default;
	virtual void Start() = 0;
	virtual void Stop() = 0;
};
