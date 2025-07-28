#include "Portal.h"

#include "Engine/MapManager.h"

PPortal::PPortal()
{
	mBlocking           = false;
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
			if (!mParent)
			{
				LogWarning("Invalid parent.");
				return;
			}
			auto Map = dynamic_cast<PMap*>(mParent);
			if (!Map)
			{
				LogWarning("Parent is not map");
				return;
			}
			PMapManager::SwitchMap(Map->GetMapName(), mTargetMap, mTargetPosition, mExitDirection);
		}
	}
}

FRect PPortal::GetLocalBounds() const
{
	return {0, 0, BLOCK_SIZE, BLOCK_SIZE};
}

bool PPortal::DebugDraw(const PRenderer* Renderer) const
{
	auto Bounds = GetWorldBounds();
	Renderer->SetDrawColor(255, 0, 255, 128);
	Renderer->DrawFillRectAt(Bounds);
	return true;
}

JSON PPortal::Serialize() const
{
	JSON Result = PActor::Serialize();
	SAVE_MEMBER_PROPERTY(TargetMap);
	SAVE_MEMBER_PROPERTY(ExitDirection);
	Result["TargetPosition"] = {mTargetPosition.X, mTargetPosition.Y};
	return Result;
}

void PPortal::Deserialize(const JSON& Data)
{
	PActor::Deserialize(Data);
	mTargetPosition.X = Data.at("TargetPosition")[0].get<float>();
	mTargetPosition.Y = Data.at("TargetPosition")[1].get<float>();
	LOAD_MEMBER_PROPERTY(TargetMap, std::string);
	LOAD_MEMBER_PROPERTY(ExitDirection, EOrientation);
}
