#pragma once

#include "../ParamBlock.h"
#include "Engine/Actors/Actor.h"
#include "Engine/Input.h"
#include "Engine/Object.h"

class PComponent :
#if _EDITOR
	public IParamBlock,
#endif
	public PObject,
	public IInputHandler
{
protected:
	PActor* mOwner = nullptr;

public:
	PComponent() = default;
	~PComponent() override = default;

	PActor* GetOwner() const
	{
		return mOwner;
	}

	void	 SetOwner(PActor* Owner);
	void	 Tick(float DeltaTime) override {}
	FVector2 GetPosition() const;

#if _EDITOR
	void InitializeParameters() override;
#endif
};

#define BEGIN_CONSTRUCT_COMPONENT                         \
	auto ClassName = Data.at("Class").get<std::string>();
#define CONSTRUCT_COMPONENT(Class)                                                        \
	if (ClassName == PREPEND(P, Class))                                                   \
	{                                                                                     \
		if (PCLASS(Class)* NewComponent = ConstructComponent<PCLASS(Class)>(Owner, Data)) \
		{                                                                                 \
			NewComponent->Deserialize(Data);                                              \
			return NewComponent;                                                          \
		}                                                                                 \
	}
#define CONSTRUCT_EACH_COMPONENT(...) FOR_EACH(CONSTRUCT_COMPONENT, __VA_ARGS__)
