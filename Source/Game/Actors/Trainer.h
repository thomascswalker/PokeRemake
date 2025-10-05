#pragma once

#include "../Battle/BattleManager.h"
#include "Engine/Actors/Character.h"
#include "Engine/Actors/Interactable.h"

class PTrainer : public PCharacter, public IInteractable
{
	STrainerContext mContext;

public:
	~PTrainer() override = default;

	STrainerContext* GetData() { return &mContext; }

	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;

	void HandleInteraction() override;
};