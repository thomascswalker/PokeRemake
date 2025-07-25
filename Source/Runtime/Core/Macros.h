#pragma once

#include "Core/Logging.h"

#define DECLARE_STATIC_GLOBAL_GETTER(ClassName) P##ClassName* Get##ClassName();

#define VALIDATE(Pred)                           \
	if (!(Pred))                                 \
	{                                            \
		LogError("Validation error: {}", #Pred); \
		return false;                            \
	}

//