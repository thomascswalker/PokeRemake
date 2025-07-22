#include "Portal.h"

#include "Engine/Map.h"

PPortal::PPortal()
{
	mBlocking = false;
	mCollisionComponent = ConstructComponent<PCollisionComponent>(this);
	if (mCollisionComponent)
	{
		mCollisionComponent->OverlapBegin.AddRaw(this, &PPortal::OnOverlapBegin);
	}
}

void PPortal::OnOverlapBegin(PActor* Actor)
{
	if (auto Player = dynamic_cast<PPlayerCharacter*>(Actor))
	{
		if (mTargetMap.empty())
		{
			LogWarning("Empty target map.");
		}
		else
		{
			LogDebug("Loading {}", mTargetMap.c_str());
			LogDebug("Moving player {} to {}", Player->GetInternalName().c_str(), mTargetPosition.ToString().c_str());
			if (!mParent)
			{
				LogWarning("Invalid parent.");
				return;
			}
			auto Chunk = static_cast<PChunk*>(mParent);
			if (!Chunk)
			{
				LogWarning("Parent is not chunk");
				return;
			}
			PMapManager::SwitchMap(Chunk->GetMapName(), mTargetMap, mTargetPosition);
		}
	}
}

FRect PPortal::GetLocalBounds() const
{
	return { 0, 0, BLOCK_SIZE, BLOCK_SIZE };
}

bool PPortal::DebugDraw(const PRenderer* Renderer) const
{
	auto Bounds = GetWorldBounds();
	Renderer->SetDrawColor(255, 0, 255, 128);
	Renderer->DrawFillRectAt(Bounds);
	return true;
}

json PPortal::Serialize() const
{
	json Result = PActor::Serialize();
	SAVE_PROPERTY(Result, TargetMap);
	Result["TargetPosition"] = { mTargetPosition.X, mTargetPosition.Y };
	return Result;
}

void PPortal::Deserialize(const json& Data)
{
	PActor::Deserialize(Data);
	mTargetPosition.X = Data.at("TargetPosition")[0].get<int>();
	mTargetPosition.Y = Data.at("TargetPosition")[1].get<int>();
	LOAD_PROPERTY(Data, TargetMap, std::string);
}