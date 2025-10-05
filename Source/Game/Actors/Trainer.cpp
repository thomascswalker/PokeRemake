#include "Trainer.h"

#include "Engine/Game.h"
#include "Engine/GameEvent.h"
#include "Engine/World.h"

JSON PTrainer::Serialize() const
{
	JSON Result = PCharacter::Serialize();
	Result["Id"] = mContext.Id;
	return Result;
}

void PTrainer::Deserialize(const JSON& Data)
{
	mContext = *GBattleManager->GetTrainer(Data["Id"]);
}

void PTrainer::HandleInteraction()
{
	SBattleContext Context;
	Context.Trainer = &mContext;
	SGameEvent Event(this, EGameEventType::BattleStart, &Context);
	GGameMode->HandleGameEvent(Event);
}