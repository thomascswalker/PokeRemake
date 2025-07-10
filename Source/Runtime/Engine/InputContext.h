#pragma once

#include <string>
#include <vector>

enum EInputType
{
	IT_Keyboard,
	IT_Mouse
};

struct SInputAction
{
	std::string Name;
	EInputType	Type;
	union
	{
		float	 FValue;
		uint32_t IValue;
	};
};

class SInputContext
{
	bool					  mEnabled;
	std::vector<SInputAction> mActions;

public:
	SInputContext()
		: mEnabled(false) {}
	SInputContext(std::initializer_list<SInputAction> Actions)
		: mEnabled(false), mActions(Actions) {}
};