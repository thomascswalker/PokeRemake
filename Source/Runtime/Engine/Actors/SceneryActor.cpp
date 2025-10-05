#include "SceneryActor.h"

#include "Engine/Components/InteractionComponent.h"
#include "Engine/Game.h"
#include "Engine/GameEvent.h"
#include "Engine/World.h"

PSceneryActor::PSceneryActor()
{
	mPosition.Z = Drawing::Z_FG;
	mBlocking = true;
	mSpriteComponent = ConstructComponent<PSpriteComponent>(this);
	if (mSpriteComponent)
	{
		// Assumes scenery actors use the standard tileset atlases.
		mSpriteComponent->SetSize(16.0f);
		mSpriteComponent->SetIndexSize(8.0f);
	}
}

PSpriteComponent* PSceneryActor::GetSpriteComponent()
{
	return mSpriteComponent;
}

std::string PSceneryActor::GetDisplayName() const
{
	return std::format("{}_{}", mInternalName.c_str(), mType.c_str());
}

FRect PSceneryActor::GetLocalBounds() const
{
	return FRect::Block();
}

JSON PSceneryActor::Serialize() const
{
	JSON Result;
	Result["Position"] = { mPosition.X, mPosition.Y };
	Result["Depth"] = mPosition.Z;
	Result["Type"] = mType;
	Result["Context"] = mContext.Serialize();
	END_SAVE_PROPERTIES;
}

void PSceneryActor::Deserialize(const JSON& Data)
{
	BEGIN_LOAD_PROPERTIES(PActor);
	LOAD_MEMBER_PROPERTY(Type, std::string);
	mSpriteComponent = GetComponent<PSpriteComponent>();

	mContext.Deserialize(Data["Context"]);
}

void PSceneryActor::HandleInteraction()
{
	SGameEvent Event(this, EGameEventType::Dialog, &mContext);
	GGameMode->HandleGameEvent(Event);
}
