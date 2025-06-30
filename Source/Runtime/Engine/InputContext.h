#pragma once

#include <string>
#include <vector>

enum EInputType
{
	IT_Keyboard,
	IT_Mouse
};

struct PInputAction
{
	std::string Name;
	EInputType	Type;
	union
	{
		float	 FValue;
		uint32_t IValue;
	};
};

class PInputContext
{
	bool					  mEnabled;
	std::vector<PInputAction> mActions;

	PInputContext()
		: mEnabled(false) {}
	PInputContext(std::initializer_list<PInputAction> Actions)
		: mEnabled(false), mActions(Actions) {}
};