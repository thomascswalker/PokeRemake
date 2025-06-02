#include "Character.h"

#include "Engine/World.h"

void PCharacter::Start()
{
	const float Size = 32.0f;
	mVertices.push_back({ -Size, -Size });
	mVertices.push_back({ Size, -Size });
	mVertices.push_back({ Size, Size });
	mVertices.push_back({ -Size, Size });
	mIndices = { 0, 1, 2, 0, 2, 3 };
}

void PCharacter::Tick(float DeltaTime)
{
	PActor::Tick(DeltaTime);
}

void PCharacter::Draw(const PRenderer* Renderer) const
{
	Renderer->SetDrawColor(0, 200, 50, 255);
	Renderer->DrawMesh(mVertices, mIndices, { mPosition.X, mPosition.Y });
}
