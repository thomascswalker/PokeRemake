#include "Character.h"

#include "Engine/World.h"

void PCharacter::Start() {}

void PCharacter::Tick(float DeltaTime)
{
	PActor::Tick(DeltaTime);
}

void PCharacter::Draw(const PRenderer* Renderer) const {}
