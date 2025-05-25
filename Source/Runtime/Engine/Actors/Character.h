#pragma once

#include "Actor.h"

class PCharacter : public PActor
{
public:
	void Draw(IRenderer* Renderer) override;
};
