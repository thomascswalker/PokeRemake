#include "Trainer.h"

#include "Core/GameConstants.h"
#include "Engine/Dialog.h"
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
	PCharacter::Deserialize(Data);
	mContext = *GBattleManager->GetTrainer(Data["Id"]);
	mSpriteComponent->GetSprite()->SetTexture(GTextureManager->Get(TEX_GARY));
}

void PTrainer::HandleInteraction()
{
	// Face the player
	PPlayerCharacter* Player = GWorld->GetPlayerCharacter();
	FVector2		  Dir = Player->GetPosition2D() - GetPosition2D();
	auto			  Direction = VectorToDirection(Dir);
	mMovementComponent->SetMovementDirection(Direction);

	// Start dialog. Once dialog is complete, execute HandleDialogComplete.
	SDialogContext Context;
	Context.Message = mContext.Dialog;
	Context.DialogCompleted.AddRaw(this, &PTrainer::HandleDialogComplete);
	SGameEvent Event(this, EGameEventType::Dialog, &Context);
	GGameMode->HandleGameEvent(Event);
}

void PTrainer::HandleDialogComplete()
{
	SBattleContext Context;
	Context.Trainer = &mContext;
	SGameEvent Event(this, EGameEventType::BattleStart, &Context);
	GGameMode->HandleGameEvent(Event);
}