#pragma once

#include "Actor.h"
#include "Chunk.h"
#include "Engine/World.h"

class PPortal : public PActor
{
	int32_t		mSourceIndex = -1;
	std::string mTargetMap = "";
	int32_t		mTargetIndex = -1;

public:
	void Start() override
	{
		mBlocking = false;
	}

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

	json Serialize() const override
	{
		json Result = PActor::Serialize();
		Result["TargetMap"] = mTargetMap;
		Result["TargetPosition"] = mTargetIndex;
		return Result;
	}

	void Deserialize(const json& Data) override
	{
		PActor::Deserialize(Data);

		CHECK_PROPERTY(Data, "TargetIndex");
		CHECK_PROPERTY(Data, "TargetMap");

		mTargetIndex = Data["TargetIndex"].get<int32_t>();
		mTargetMap = Data["TargetMap"].get<std::string>();
	}
};