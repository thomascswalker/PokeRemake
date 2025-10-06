#pragma once
#include "Engine/Actors/Interactable.h"

#include "Component.h"

DECLARE_MULTICAST_DELEGATE(DInteracted);

class PInteractionComponent : public PComponent
{
	void* mData = nullptr;

public:
	DInteracted Interacted;

	PInteractionComponent() = default;
	~PInteractionComponent() override
	{
		if (mData)
		{
			free(mData);
		}
	};

	template <typename T>
	T* GetData() const
	{
		return static_cast<T*>(mData);
	}

	template <typename T>
	void SetData(T* InData)
	{
		mData = std::malloc(sizeof(InData));
		memcpy(mData, InData, sizeof(InData));
	}

	void Interact(PPlayerCharacter* Player);
};
