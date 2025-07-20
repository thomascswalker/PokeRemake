#pragma once

#include "Actor.h"
#include "Chunk.h"
#include "Engine/World.h"

class PPortal : public PActor
{
	PChunk*	 mTargetMap = nullptr;
	IVector2 mTargetPosition;

public:
	FRect GetLocalBounds() const override
	{
		return { 0, 0, BLOCK_SIZE, BLOCK_SIZE };
	}

	bool DebugDraw(const PRenderer* Renderer) const override
	{
		auto Bounds = GetWorldBounds();
		Renderer->SetDrawColor(255, 0, 255, 128);
		Renderer->DrawFillRectAt(Bounds);
		return true;
	}
};