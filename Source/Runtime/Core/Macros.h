#pragma once

#include "Core/Logging.h"

#define VALIDATE(Pred)                           \
	if (!(Pred))                                 \
	{                                            \
		LogError("Validation error: {}", #Pred); \
		return false;                            \
	}

//