#pragma once

#include "Engine/Actors/Actor.h"
#include "Engine/Actors/Interactable.h"
#include "Engine/Components/SpriteComponent.h"
#include "Engine/Dialog.h"

namespace SceneryTypes
{
#define DECLARE_TYPE(X) inline const char* X = PREPEND($, X)
	DECLARE_TYPE(SignPost);
	DECLARE_TYPE(Tree);
#undef DECLARE_TYPE
} // namespace SceneryTypes

class PSceneryActor : public PActor, public IInteractable
{
	std::string		  mType;
	PSpriteComponent* mSpriteComponent;
	SDialogContext	  mContext;

public:
	PSceneryActor();
	~PSceneryActor() override = default;

	PSpriteComponent* GetSpriteComponent();

	std::string GetDisplayName() const override;

	FRect GetLocalBounds() const override;

	JSON Serialize() const override;
	void Deserialize(const JSON& Data) override;

	void HandleInteraction() override;
};
