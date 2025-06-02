#pragma once
#include "Core/Vector.h"

struct PVertex2D
{
	FVector2 Position;
	FVector4 Color;
	FVector2 TexCoords;
};

struct PVertex3D
{
	FVector3 Position;
	FVector4 Color;
	FVector2 TexCoords;
};