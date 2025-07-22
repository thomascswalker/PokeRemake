#include "Portal.h"

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
			LogDebug("Moving player {} to {}", Player->GetInternalName().c_str(), mTargetIndex);
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
	SAVE_PROPERTY(Result, TargetIndex);
	return Result;
}

void PPortal::Deserialize(const json& Data)
{
	PActor::Deserialize(Data);
	LOAD_PROPERTY(Data, TargetIndex, int32_t);
	LOAD_PROPERTY(Data, TargetMap, std::string);
}